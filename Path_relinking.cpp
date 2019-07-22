#include "Path_relinking.h"

bool ordenacaoIndividuoPR(Individuo *i1, Individuo *i2){return i1->getPerdaAtiva() > i2->getPerdaAtiva();}

Path_relinking::Path_relinking(vector<Individuo*> pool, int max_it, float pct_pr_elite){

    this->pool = pool;
    this->max_it = max_it;
    this->pct_pr_elite = pct_pr_elite;

}

Individuo *Path_relinking::pre(Grafo *g){

    unsigned int tam_pool = pool.size();

    int tam_elite = round(pct_pr_elite*tam_pool);

    Individuo *i;

    int k = 0;
    while(k<max_it){

        cout << "k: " << k << endl;

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
            printf("pr: %f\ntempo ordenacao pool: %f\ntempo execucao pr: %f  pior pool: %f\n\n", 100*1000*i->getPerdaAtiva(), (double)(fim-inicio)/CLOCKS_PER_SEC,
            (double)(fim2-inicio2)/CLOCKS_PER_SEC, 100*1000*pool.at(0)->getPerdaAtiva());
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
Individuo *Path_relinking::prs(Individuo *inicio,Individuo *guia, Grafo *g, Individuo *indRef){

    vector<int> candidatos, path;//ids que representam a ordem que this sera igualado ao guia

    Individuo *best = new Individuo(inicio->getNumArcos());
    Individuo *aux = new Individuo(inicio->getNumArcos());

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


Individuo *Path_relinking::prs2(Individuo *inicio, Individuo *guia, Grafo *g){

//    printf("Niveis:\n\n");

    vector<int> candidatos, path;//ids que representam a ordem que this sera igualado ao guia
    vector<vector<int>> nivel;

    int nivelBest, direcaoBest;//para cada nivel
    int nivelBestGlobal, direcaoBestGlobal;

    Individuo *aux = new Individuo(inicio->getNumArcos());
    Individuo *best = new Individuo(inicio->getNumArcos());

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
