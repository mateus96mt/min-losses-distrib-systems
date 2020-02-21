#include "include/Evolutionary_path_relinking.h"

    ///old implementation with RK representation of individual (too slow)
bool ordenacaoIndividuoPR(RK_Individual *i1, RK_Individual *i2){return i1->getPerdaAtiva() > i2->getPerdaAtiva();}

Evolutionary_path_relinking::Evolutionary_path_relinking(vector<RK_Individual*> pool, int max_it, float pct_pr_elite){

    this->pool = pool;
    this->max_it = max_it;
    this->pct_pr_elite = pct_pr_elite;

}

RK_Individual *Evolutionary_path_relinking::pre(Graph *g){

    unsigned int tam_pool = pool.size();

    int tam_elite = round(pct_pr_elite*tam_pool);

    RK_Individual *i;

    int k = 0;
    while(k<max_it){

//        cout << "k: " << k << endl;

        clock_t inicio = clock();
        sort(pool.begin(), pool.end(), ordenacaoIndividuoPR);//ordenando toda hora, overhead deve estar aqui
        clock_t fim = clock();

//        printf("best: %f\npior: %f\n", 100*1000*pool.at(pool.size()-1)->getPerdaAtiva(), 100*1000*pool.at(0)->getPerdaAtiva());

        int ind = (rand() % tam_elite) + (tam_pool - tam_elite);
        int guia = (rand() % tam_elite);

        clock_t inicio2 = clock();
//        i = pool.at(ind)->prs2(pool.at(guia), g);//ja tem funcao objetivo calculada
        i = this->prs2(pool.at(ind), pool.at(guia), g);
        clock_t fim2 = clock();

        if(i->getPerdaAtiva() < pool.at(0)->getPerdaAtiva()){
//            printf("pr: %f\ntempo ordenacao pool: %f\ntempo execucao pr: %f  pior pool: %f\n\n", 100*1000*i->getPerdaAtiva(), (double)(fim-inicio)/CLOCKS_PER_SEC,
//            (double)(fim2-inicio2)/CLOCKS_PER_SEC, 100*1000*pool.at(0)->getPerdaAtiva());
//            swap(pool.at(0), i);
            pool.erase(pool.begin());
            pool.push_back(i);
//            delete i;
            k=0;
        }else{
            k++;
            delete i;
        }

    }
    sort(pool.begin(), pool.end(), ordenacaoIndividuoPR);

//    cout << "fim do pre" << endl;

    return pool.at(pool.size()-1);//retorna o melhor individuo

}

//path relinking "guloso", se nenhum individuo do path foi melhor que this e guia entao retorna um individuo aleatorio
RK_Individual *Evolutionary_path_relinking::prs(RK_Individual *inicio, RK_Individual *guia, Graph *g, RK_Individual *indRef){

    vector<int> candidatos, path;//ids que representam a ordem que this sera igualado ao guia

    RK_Individual *best = new RK_Individual(inicio->getNumArcos());
    RK_Individual *aux = new RK_Individual(inicio->getNumArcos());

    aux->calculaFuncaoObjetivoOtimizado(g);
    best->calculaFuncaoObjetivoOtimizado(g);
    guia->calculaFuncaoObjetivoOtimizado(g);
    indRef->calculaFuncaoObjetivoOtimizado(g);//individuo de referencia, o path vai atualizando equanto obtem resultado melhor que o individuo de referencia

    double bestPerdaAtiva = indRef->getPerdaAtiva();

    for(int i=0; i<inicio->getNumArcos();i++){

        candidatos.push_back(i);
        best->getPesos()[i] = inicio->getPesos()[i];
        aux->getPesos()[i] = inicio->getPesos()[i];

    }

    for(unsigned int i=0; i<candidatos.size(); i++){

        aux->getPesos()[candidatos.at(i)] = guia->getPesos()[candidatos.at(i)];
        aux->calculaFuncaoObjetivoOtimizado(g);

        if(aux->getPerdaAtiva() < bestPerdaAtiva){

            bestPerdaAtiva = aux->getPerdaAtiva();
            path.push_back(candidatos.at(i));
            candidatos.erase(candidatos.begin() + i);
            i = 0;

        }else{

            aux->getPesos()[candidatos.at(i)] = inicio->getPesos()[candidatos.at(i)];

        }

    }


    delete aux;

    for(unsigned int i=0; i<path.size(); i++)
        best->getPesos()[path.at(i)] = guia->getPesos()[path.at(i)];

    best->calculaFuncaoObjetivoOtimizado(g);

    return best;
}


RK_Individual *Evolutionary_path_relinking::prs2(RK_Individual *inicio, RK_Individual *guia, Graph *g){

//    printf("Niveis:\n\n");

    vector<int> candidatos, path;//ids que representam a ordem que this sera igualado ao guia
    vector<vector<int>> nivel;

    int nivelBest, direcaoBest;//para cada nivel
    int nivelBestGlobal, direcaoBestGlobal;

    RK_Individual *aux = new RK_Individual(inicio->getNumArcos());
    RK_Individual *best = new RK_Individual(inicio->getNumArcos());

    aux->calculaFuncaoObjetivoOtimizado(g);
    best->calculaFuncaoObjetivoOtimizado(g);
    guia->calculaFuncaoObjetivoOtimizado(g);

    double minPerdaNivel = 999999999999, minPerdaGlobal = 999999999999;

    for(int i=0; i<inicio->getNumArcos();i++){

        candidatos.push_back(i);
        aux->getPesos()[i] = inicio->getPesos()[i];
        best->getPesos()[i] = inicio->getPesos()[i];
    }

    int nv = 0;
    while(candidatos.size()>0){

        minPerdaNivel = 999999999999;

        vector<int> v;
        nivel.push_back(v);

        //loop entre os candidatos do nivel
        for(unsigned int i=0; i<candidatos.size(); i++){

            aux->getPesos()[candidatos.at(i)] = guia->getPesos()[candidatos.at(i)];
            aux->calculaFuncaoObjetivoOtimizado(g);

            nivel.at(nv).push_back(candidatos.at(i));

//            printf("%d ", candidatos.at(i));

            //melhor individuo no nivel
            if(aux->getPerdaAtiva() < minPerdaNivel) {
                minPerdaNivel = aux->getPerdaAtiva();
                nivelBest = nv;
                direcaoBest = i;
            }

            aux->getPesos()[candidatos.at(i)] = inicio->getPesos()[candidatos.at(i)];//desfaz alteracao para avaliar proxima

        }

//        printf("{%d}  (nv %d)\n", candidatos.at(direcaoBest), nv);

        aux->getPesos()[candidatos.at(direcaoBest)] = guia->getPesos()[candidatos.at(direcaoBest)];//caminha no path na direcao da melhor local

        path.push_back(candidatos.at(direcaoBest));
        candidatos.erase(candidatos.begin() + direcaoBest);

        //verificar melhor individuo entre todos gerados ao longo do path
        if(minPerdaNivel < minPerdaGlobal){
            minPerdaGlobal = minPerdaNivel;
            nivelBestGlobal = nivelBest;
            direcaoBestGlobal = direcaoBest;
        }

        nv++;

    }

//    printf("\n\nbest:\n");
    //percorre caminho do path
    for(int i=0; i<nivelBestGlobal; i++){
        best->getPesos()[path.at(i)] = guia->getPesos()[path.at(i)];
//        printf("%d\n", path.at(i));
    }
    best->getPesos()[nivel.at(nivelBestGlobal).at(direcaoBestGlobal)] = guia->getPesos()[nivel.at(nivelBestGlobal).at(direcaoBestGlobal)];
//    printf("%d\n\n\n", nivel.at(nivelBestGlobal).at(direcaoBestGlobal));

    best->calculaFuncaoObjetivoOtimizado(g);

//    printf("\n\nminPerdaGlobal: %f KW\n", 100*1000*minPerdaGlobal);
//    printf("best->getPerdaAtiva(): %f KW\n", 100*1000*best->getPerdaAtiva());
//    printf("nivelBestGLobal: %d \n", nivelBestGlobal);
//    printf("direcaoBestGLobal: %d \n", nivel.at(nivelBestGlobal).at(direcaoBestGlobal));

    delete aux;

    return best;

}

typedef struct{
    int in, out;//
}operation;

Individual *Evolutionary_path_relinking::path_relingking(Individual *start, Individual *end, Graph *g) {

    //Edges of 'end' to be inserted on 'start'
    vector<int> candidates_in;

    //operations on in and out that define the hole path
    vector<operation> path;

    double bestLocal = 9999999999, bestGlobal = 9999999999;

    Individual *aux = new Individual(); //to run the path

    bool fixed;
    //defining fixed edges
    for(unsigned  long int j=0; j<end->getSize(); j++){

        aux->getOpenedSwitches().push_back(start->getOpenedSwitches().at(j));

        fixed = false;

        for(unsigned long int i=0; i<start->getSize(); i++){

            //edges there are present in star and end are fixed
            if(start->getOpenedSwitches().at(i)->getID() == end->getOpenedSwitches().at(j)->getID()) {
                fixed = true;
                start->getOpenedSwitches().at(i)->setFixed(true);
                break;
            }

        }

        if(fixed == false)
            candidates_in.push_back(j);//positions of end to go inside start
    }

    int j_remove = 0;
    unsigned  long int n = 0, pos_path = 0;

    //while there still candidates
    while (candidates_in.size()>0){

//        printf("\n\n\n\n\ncandidates_in(%ld): ", candidates_in.size());
//        for(unsigned int it = 0; it<candidates_in.size(); it++)
//            printf("%d\t", end->getOpenedSwitches().at(candidates_in.at(it))->getID());
//        printf("\n");
//
//        printf("\nfixeds:\t");
//        for(unsigned long int itt = 0; itt<aux->getOpenedSwitches().size(); itt++) {
//            if(aux->getOpenedSwitches().at(itt)->getFixed()==true)
//                printf("%d\t", aux->getOpenedSwitches().at(itt)->getID());
//        }

        vector<operation> local;
        unsigned  long int pos = 0, pos_local = 0;

        j_remove = -1;//if no candidates_in was able to go in
        //avaliate all candidates
        for(unsigned long int j=0; j<candidates_in.size(); j++){

            vector<int> candidates_out;

            //candidates out exclude fixed edges, those cannot go out
            candidates_out = aux->evaluate(end->getOpenedSwitches().at(candidates_in.at(j)), g);

//            printf("\ncandidate_in[%ld]: %d\t\tcandidates_out(%ld):\t", j, end->getOpenedSwitches().at(candidates_in.at(j))->getID(), candidates_out.size());
//            for(unsigned int it = 0; it<candidates_out.size(); it++)
//                printf("%d\t", aux->getOpenedSwitches().at(candidates_out.at(it))->getID());

            bestLocal = 9999999999;
            //for all possible edges of 'start' that can go out tu put candidate in
            for(unsigned int k=0; k<candidates_out.size();k++){

//                printf("\ncandidate_out: %d\t", aux->getOpenedSwitches().at(candidates_out.at(k))->getID());

                //to undo changes
                ///BUG QUE ME TIROU O DIA: start no lugar de aux, com isso gerava solucoes invalidas
                Edge *e = aux->getOpenedSwitches().at(candidates_out.at(k));

                operation op;
                op.in = candidates_in.at(j);
                op.out = candidates_out.at(k);

                local.push_back(op);

                aux->getOpenedSwitches().at(candidates_out.at(k)) = end->getOpenedSwitches().at(candidates_in.at(j));

//                printf("\naux(%ld)\t:", aux->getOpenedSwitches().size());
//                for(unsigned int it = 0; it<aux->getOpenedSwitches().size(); it++)
//                    printf("%d\t", aux->getOpenedSwitches().at(it)->getID());

//                printf("vai calcular funcaoObjetivo...");
                aux->calcObjectiveF(g);
//                printf("feito!");

//                printf("{%.2f}", 100*1000*aux->getActiveLoss());

                if(aux->getActiveLoss() < bestLocal) {
                    pos_local = pos;
                    j_remove = j;
                    bestLocal = aux->getActiveLoss();
                }

                pos++;

                aux->getOpenedSwitches().at(candidates_out.at(k)) = e;
            }
//            printf("\n");
        }

        //there is no possible candidate in, break while and finish
        if(j_remove==-1)
            break;

//        printf("\ncandidato escolhido_out: %d\tcandidato escolhido_in: %d",
//                aux->getOpenedSwitches().at(local.at(pos_local).out)->getID(),
//                end->getOpenedSwitches().at(local.at(pos_local).in)->getID());

        //because of fixed edges we can have no candidates to go out of aux
        if(local.size()>0) {
            //have localBest now, go to best direction
            aux->getOpenedSwitches().at(local.at(pos_local).out) = end->getOpenedSwitches().at(local.at(pos_local).in);
            aux->calcObjectiveF(g);
            path.push_back(local.at(pos_local));
        }

//        printf("\n\naux(%ld)\tactiveLoss: %f\t: ", aux->getOpenedSwitches().size(), 100*1000*aux->getActiveLoss());
//        for(unsigned int it = 0; it<aux->getOpenedSwitches().size(); it++)
//            printf("%d\t", aux->getOpenedSwitches().at(it)->getID());

        if(aux->getActiveLoss() < bestGlobal){
            bestGlobal = aux->getActiveLoss();
            pos_path = n;
        }

        n++;

//        printf("\ncandidatos_in.erase(%d)[size:%ld]\n", j_remove, candidates_in.size());
        candidates_in.erase(candidates_in.begin() + j_remove);
    }

//    for(unsigned long int i=0; i<path.size()-pos_path-1; i++){
//        aux->getOpenedSwitches().at(path.at(path.size()-1-i).out) = start->getOpenedSwitches().at(path.at(path.size()-1-i).out);
//    }

    for(unsigned long int i=0; i<start->getSize(); i++){
        aux->getOpenedSwitches().at(i) = start->getOpenedSwitches().at(i);
    }
    for(unsigned long int i=0; i<path.size(); i++){
        aux->getOpenedSwitches().at(path.at(i).out) = end->getOpenedSwitches().at(path.at(i).in);
    }
    aux->calcObjectiveF(g);

    //reset fixed to false in all edges
    for(unsigned long int i=0; i<start->getSize(); i++)
        start->getOpenedSwitches().at(i)->setFixed(false);

    return aux;
}

bool OS_sort(Individual *os1, Individual *os2){return os1->getActiveLoss() > os2->getActiveLoss();}

Individual *Evolutionary_path_relinking::run(vector<RK_Individual *> pool_RK, int max_it, float pctElite, Graph *g) {

    int tam_elite = round(pctElite*pool_RK.size());
    int tam_pool = pool_RK.size();

    vector<Individual*> pool_OS;
    Individual *aux, **oss = new Individual*[tam_pool];

    //convert RK pool to OS pool
    for(unsigned long int i=0; i<pool_RK.size(); i++){
        oss[i] = new Individual(pool_RK.at(i), g);
        pool_OS.push_back(oss[i]);
    }

    sort(pool_OS.begin(), pool_OS.end(), OS_sort);

    int it=0;
    while (it<=max_it){

        int i = (rand() % tam_elite) + (tam_pool - tam_elite);
        int j = (rand() % tam_elite);

        //path relinking from non-elite to elite
        aux = this->path_relingking(pool_OS.at(j), pool_OS.at(i), g);

        if(pool_OS.at(0)->getActiveLoss() > aux->getActiveLoss()){
            delete pool_OS.at(0);
            pool_OS.at(0) = aux;
        }else{
            it++;
        }

        sort(pool_OS.begin(), pool_OS.end(), OS_sort);

//        printf("it: %d\tpior da pool: %.2f\tmelhor da pool: %.2f\n",
//                it, 100*1000*pool_OS.at(0)->getActiveLoss(),
//                100*1000*pool_OS.at(pool_OS.size()-1)->getActiveLoss());
    }

    //return best individual of pool
    return pool_OS.at(pool_OS.size()-1);
}



