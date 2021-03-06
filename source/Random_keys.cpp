#include "include/Random_keys.h"
#include <unistd.h>
#include <random>

using namespace std;

#define RANGEPESO 100

bool ordenacaoIndividuo(RK_Individual *i1, RK_Individual *i2){return i1->getActiveLoss() > i2->getReactiveLoss();}

Random_keys::Random_keys(int tamPop, int numGeracoes){
    this->tamPop = tamPop;
    this->numGeracoes = numGeracoes;
}

void Random_keys::geraPopAleatoria(Graph *g){

    g->markOneDirectionInEdges();
    RK_Individual *ind;
    for(int i=0; i<this->tamPop; i++){

        ind = new RK_Individual(g->getEdgesSizes() / 2 - g->getNumberOfNonModifiable());
        ind->generate_random_weights();

        popAtual.push_back(ind);
    }
    popAnterior = popAtual;
}

void Random_keys::geraPopAleatoriaConfInicial(Graph *g, int *idsAbertos, int nAbertos){

    g->markOneDirectionInEdges();
    RK_Individual *ind;
    for(int i=0; i<this->tamPop; i++){

        ind = new RK_Individual(g->getEdgesSizes() / 2 - g->getNumberOfNonModifiable());
        if(i==0){
            ind->geraPesosConfInicial(idsAbertos, nAbertos, g);
        }
        else{
            ind->generate_random_weights();
        }

        popAtual.push_back(ind);
    }
    popAnterior = popAtual;
}

/** ordena populacao em ordem decrescente por valor da funcao objetivo
dado que queremos minimizar a perda os piores individuos ficam no inicio(perda maior)
queremos os melhore (menor perda, fim da lista) **/
void Random_keys::ordenaPopulacaoAtual(Graph *g){
    for(unsigned int i=0; i<popAtual.size(); i++)
        popAtual.at(i)->calculaFuncaoObjetivoOtimizado(g);
//        popAtual.at(i)->calculaFuncaoObjetivo(g);

    sort(popAtual.begin(), popAtual.end(), ordenacaoIndividuo);
}

int Random_keys::avancaGeracoes(Graph *g){

    int melhorGeracao = 0;
    RK_Individual *best = popAtual.at(this->tamPop - 1);
    this->ordenaPopulacaoAtual(g);
    double perda = 100*1000*best->getActiveLoss();
    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
        e ordena a populacao da maior perda(pior individuo)
        pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        RK_Individual *best = popAtual.at(this->tamPop - 1);
        if (100*1000*best->getActiveLoss() < perda){
            perda = 100*1000*best->getActiveLoss();
//            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
            melhorGeracao = k;
        }

        popAnterior = popAtual;

        int num_piores = 0.05*this->tamPop;
        int num_melhores = 0.1*this->tamPop;

        for(int i=num_piores; i<this->tamPop-num_melhores; i++){

            /** cruzamento entre pai1 e pai2 entre os
            individuos aleatorios da populacao anterior
            modificar por uma escolha em roleta no futuro**/
            int pai1 = rand() % this->tamPop;
//            int pai2 = rand() % this->tamPop; //aleatorio

            /**torneio com 3**/
            int cand1, cand2, cand3;
            cand1 = rand() % this->tamPop;
            cand2 = rand() % this->tamPop;
            cand3 = rand() % this->tamPop;

            if(popAnterior.at(cand1)->getActiveLoss() < popAnterior.at(cand2)->getActiveLoss()){
                cand2=cand1;
            }
            if(popAnterior.at(cand2)->getActiveLoss() < popAnterior.at(cand3)->getActiveLoss()){
                cand3=cand2;
            }

            int pai2 = cand3;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

//            popAnterior.at(pai1)->cruzamentoMedia(popAnterior.at(pai2), popAtual.at(i));
            this->cruzamento1(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
//            popAtual.at(i)->mutacao();
            this->mutacao(popAtual.at(i));
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->generate_random_weights();
    }

    this->ordenaPopulacaoAtual(g);

    return melhorGeracao;
}

int Random_keys::avancaGeracoes2(Graph *g){

    int melhorGeracao = 0;
    RK_Individual *best = popAtual.at(this->tamPop - 1);
    this->ordenaPopulacaoAtual(g);
    double perda = 100*1000*best->getActiveLoss();
    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
         e ordena a populacao da maior perda(pior individuo)
         pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        RK_Individual *best = popAtual.at(this->tamPop - 1);
        if (100*1000*best->getActiveLoss() < perda){
            perda = 100*1000*best->getActiveLoss();
//            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
            melhorGeracao = k;
        }

        popAnterior = popAtual;

        int num_piores = 0.05*this->tamPop;
        int num_melhores = 0.1*this->tamPop;

        for(int i=num_piores; i<this->tamPop-num_melhores; i++){

            /** cruzamento entre pai1 e pai2 entre os
            individuos aleatorios da populacao anterior
            modificar por uma escolha em roleta no futuro**/
            int pai1 = rand() % this->tamPop;
//            int pai2 = rand() % this->tamPop; //aleatorio

            /*torneio com 3*/
            int cand1, cand2, cand3;
            cand1 = rand() % this->tamPop;
            cand2 = rand() % this->tamPop;
            cand3 = rand() % this->tamPop;

            int aux = cand1;

            if(popAnterior.at(cand1)->getActiveLoss() < popAnterior.at(cand2)->getActiveLoss()){
                cand2=cand1;
            }else{aux = cand2;}
            if(popAnterior.at(cand2)->getActiveLoss() < popAnterior.at(cand3)->getActiveLoss()){
                cand3=cand2;
            }else{aux = cand3;}
            pai1 = aux;

            int pai2 = cand3;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

//            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            this->cruzamento2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
//            popAtual.at(i)->mutacao();
            this->mutacao(popAtual.at(i));
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->generate_random_weights();
    }

//    cout << "\n--------------------------------------"<<endl;
//    for(unsigned int i=0; i<popAtual.size(); i++)
//        cout << 100*1000*popAtual.at(i)->getPerdaAtiva() << " , ";
//    cout << "\n--------------------------------------"<<endl;

    this->ordenaPopulacaoAtual(g);

    return melhorGeracao;
}

    ///OLD IMPLEMENTATION OF AG+PR
//int Random_keys::avancaGeracoesPRS(Graph *g){
//
//    int melhorGeracao = 0;
//    RK_Individual *best = popAtual.at(this->tamPop - 1);
//    this->ordenaPopulacaoAtual(g);
//    double perda = 100*1000*best->getActiveLoss();
//    for(int k=0; k<this->numGeracoes; k++){
//
//        /** calcula a funcao criterio para cada individuo
//         e ordena a populacao da maior perda(pior individuo)
//         pra menor perda(melhor individuo), perdaAtiva**/
//        this->ordenaPopulacaoAtual(g);
//
//        RK_Individual *best = popAtual.at(this->tamPop - 1);
//        if (100*1000*best->getActiveLoss() < perda){
//            perda = 100*1000*best->getActiveLoss();
////            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
//            melhorGeracao = k;
//        }
//
//        popAnterior = popAtual;
//
//        int num_piores = 0.05*this->tamPop;
//        int num_melhores = 0.1*this->tamPop;
//
//        {//path relinking simples em cada geracao
//            int id1 = (tamPop-num_melhores) + (rand() % num_melhores);
//            int id2 = (tamPop-num_melhores) + (rand() % num_melhores);
//
//            RK_Individual *indRef;//individuo de referencia para o pathrelinking
//
//            if(popAtual.at(id1)->getActiveLoss() > popAtual.at(id2)->getActiveLoss())
//                indRef = popAtual.at(id1);
//            else
//                indRef = popAtual.at(id2);
//
////            Individuo *ind = popAtual.at(id1)->prs(popAtual.at(id2), g, indRef);
//            Evolutionary_path_relinking p(this->popAnterior, 0, 0);//objeto necessario para utilizacao de funcoes do path-relinking
//            RK_Individual *ind = p.prs(popAtual.at(id1), popAtual.at(id2), g, indRef);
//
//            if(ind->getPerdaAtiva() < popAtual.at(0)->getPerdaAtiva())//atualiza pior individuo da populacao com resultado do path relinking
//                popAtual.at(0) = ind;
//            else
//                delete ind;
//
//        }
//
//        for(int i=num_piores; i<this->tamPop-num_melhores; i++){
//
//            /** cruzamento entre pai1 e pai2 entre os
//            individuos aleatorios da populacao anterior
//            modificar por uma escolha em roleta no futuro**/
//            int pai1 = rand() % this->tamPop;
////            int pai2 = rand() % this->tamPop; //aleatorio
//
//            /*torneio com 3*/
//            int cand1, cand2, cand3;
//            cand1 = rand() % this->tamPop;
//            cand2 = rand() % this->tamPop;
//            cand3 = rand() % this->tamPop;
//
//            int aux = cand1;
//
//            if(popAnterior.at(cand1)->getPerdaAtiva() < popAnterior.at(cand2)->getPerdaAtiva()){
//                cand2=cand1;
//            }else{aux = cand2;}
//            if(popAnterior.at(cand2)->getPerdaAtiva() < popAnterior.at(cand3)->getPerdaAtiva()){
//                cand3=cand2;
//            }else{aux = cand3;}
//            pai1 = aux;
//
//            int pai2 = cand3;
//
//            while(pai2==pai1)
//                pai2 = rand() % this->tamPop;
//
//            //popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
//            this->cruzamento2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
//            //popAtual.at(i)->mutacao();
//            this->mutacao(popAtual.at(i));
//        }
//
//        /**aleatorio ao inves de manter piores**/
//        for(int i=0; i<num_piores; i++)
//            popAtual.at(i)->geraPesosAleatorios();
//
//    }
//
//    this->ordenaPopulacaoAtual(g);
//
//    return melhorGeracao;
//}
//
//void Random_keys::prsEvolutivo(vector<RK_Individual*> pool, vector<RK_Individual*> &populacao, Graph *g){
//
//    RK_Individual *indRef, *ind;
//
//    sort(pool.begin(), pool.end(), ordenacaoIndividuo);//ordenar novamente a pool atualizada
//
//    //path relinking evolutivo entre todos os individuos da pool, par a par
//    for(unsigned int i=0; i<pool.size(); i++){
//
//        for(unsigned int j=0; j<pool.size(); j++){
//
//            if(i!=j){
//
//                indRef = pool.at(0);
//
////                ind = pool.at(i)->prs(pool.at(j), g, indRef);//path relinking
//                Evolutionary_path_relinking *p = new Evolutionary_path_relinking();
//                ind = p->prs(pool.at(i), pool.at(j), g, indRef);
//
//                if(ind->getPerdaAtiva() < indRef->getPerdaAtiva()){//atualizacao da pool
//
//                    pool.at(0) = ind;
//                    i = 0;
//                    j = 0;
//
//                    sort(pool.begin(), pool.end(), ordenacaoIndividuo);//ordenar novamente a pool atualizada
//
//                }
//
//            }
//
//        }
//
//    }
//
//    //acrescenta pool atualizada na populacao
//    for(unsigned int i=0; i<pool.size(); i++)
//        populacao.push_back(pool.at(i));
//
//}
//
//RK_Individual *Random_keys::pre(vector<RK_Individual*> pool, int max_it, float pct_pr_elite, Graph *g){
//
//    unsigned int tam_pool = pool.size();
//
//    int tam_elite = round(pct_pr_elite*tam_pool);
//
//    RK_Individual *i;
//
//    int k = 0;
//    while(k<max_it){
//
////        cout << "k: " << k << endl;
//
//        clock_t inicio = clock();
//        sort(pool.begin(), pool.end(), ordenacaoIndividuo);//ordenando toda hora, overhead deve estar aqui
//        clock_t fim = clock();
//
////        printf("best: %f\npior: %f\n", 100*1000*pool.at(pool.size()-1)->getPerdaAtiva(), 100*1000*pool.at(0)->getPerdaAtiva());
//
//        int ind = (rand() % tam_elite) + (tam_pool - tam_elite);
//        int guia = (rand() % tam_elite);
//
//        clock_t inicio2 = clock();
////        i = pool.at(ind)->prs2(pool.at(guia), g);//ja tem funcao objetivo calculada
//        Evolutionary_path_relinking *p = new Evolutionary_path_relinking();
//        i = p->prs2(pool.at(ind), pool.at(guia), g);
//        delete p;
//        clock_t fim2 = clock();
//
//        if(i->getPerdaAtiva() < pool.at(0)->getPerdaAtiva()){
////            printf("pr: %f\ntempo ordenacao pool: %f\ntempo execucao pr: %f\n\n", 100*1000*i->getPerdaAtiva(), (double)(fim-inicio)/CLOCKS_PER_SEC, (double)(fim2-inicio2)/CLOCKS_PER_SEC);
////            swap(pool.at(0), i);
//            pool.erase(pool.begin());
//            pool.push_back(i);
////            delete i;
//            k=0;
//        }else{
//            k++;
//            delete i;
//        }
//
//    }
//    sort(pool.begin(), pool.end(), ordenacaoIndividuo);
//
////    cout << "fim do pre" << endl;
//
//    return pool.at(pool.size()-1);//retorna o melhor individuo
//
//}

//int Random_keys::avancaGeracoesPRSEvolutivoFinal(Graph *g){
//
//    int num_piores, num_melhores;
//    int melhorGeracao = 0;
//    RK_Individual *best = popAtual.at(this->tamPop-1);
//    this->ordenaPopulacaoAtual(g);
//    double perda = 100*1000*best->getActiveLoss();
//    for(int k=0; k<this->numGeracoes; k++){
//
//        /** calcula a funcao criterio para cada individuo
//         e ordena a populacao da maior perda(pior individuo)
//         pra menor perda(melhor individuo), perdaAtiva**/
//        this->ordenaPopulacaoAtual(g);
//
//        RK_Individual *best = popAtual.at(this->tamPop-1);
//        if (100*1000*best->getActiveLoss() < perda){
//            perda = 100*1000*best->getActiveLoss();
////            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
//            melhorGeracao = k;
//        }
//
//        popAnterior = popAtual;
//
//        num_piores = 0.05*this->tamPop;
//        num_melhores = 0.1*this->tamPop;
//
//        for(int i=num_piores; i<this->tamPop-num_melhores; i++){
//
//            /** cruzamento entre pai1 e pai2 entre os
//            individuos aleatorios da populacao anterior
//            modificar por uma escolha em roleta no futuro**/
//            int pai1 = rand() % this->tamPop;
//
//            /*torneio com 3*/
//            int cand1, cand2, cand3;
//            cand1 = rand() % this->tamPop;
//            cand2 = rand() % this->tamPop;
//            cand3 = rand() % this->tamPop;
//
//            int aux = cand1;
//
//            if(popAnterior.at(cand1)->getActiveLoss() < popAnterior.at(cand2)->getActiveLoss()){
//                cand2=cand1;
//            }else{aux = cand2;}
//            if(popAnterior.at(cand2)->getActiveLoss() < popAnterior.at(cand3)->getActiveLoss()){
//                cand3=cand2;
//            }else{aux = cand3;}
//            pai1 = aux;
//
//            int pai2 = cand3;
//
//            while(pai2==pai1)
//                pai2 = rand() % this->tamPop;
//
////            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
//            this->cruzamento2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
////            popAtual.at(i)->mutacao();
//            this->mutacao(popAtual.at(i));
//        }
//
//        /**aleatorio ao inves de manter piores**/
//        for(int i=0; i<num_piores; i++)
//            popAtual.at(i)->generate_random_weights();
//    }
//
//    vector<RK_Individual*> pool;
//    for(int i=0; i<num_melhores; i++)
//        pool.push_back(popAtual.at(tamPop - num_melhores + i));
//
//    clock_t inicio = clock();
//
//
//    prsEvolutivo(pool, popAtual, g);//execucao do path reliking evolutivo no final
//
//    clock_t fim = clock();
//    printf("tempo prsEvolutivo(isoladamente) (pool size = %d):  %.2lf\n",  num_melhores, (float)(fim-inicio)/CLOCKS_PER_SEC);
//
//    this->tamPop = popAtual.size();
//
//    this->ordenaPopulacaoAtual(g);
//
//    return melhorGeracao;
//}

    //OLD IMPLEMENTATION
//int Random_keys::avancaGeracoesPRE(Graph *g, int it_s_melhora, int tam_pool, int max_it, float pct_pool_elite){
//
////    cout << "avanca geracoes\n";
//    vector<int> chamadaGeracao;
//    int numChamadasPr = 0;
//
//    int tam_elite = round(pct_pool_elite*tam_pool);
//    int it = 0;
//    vector<int> chamadasPr;
//    RK_Individual *bestPr;
//
//    int melhorGeracao = 0;
//    RK_Individual *best = popAtual.at(this->tamPop - 1);
//    this->ordenaPopulacaoAtual(g);
//    double perda = 100*1000*best->getActiveLoss();
//    for(int k=0; k<this->numGeracoes; k++){
//
//        /** calcula a funcao criterio para cada individuo
//         e ordena a populacao da maior perda(pior individuo)
//         pra menor perda(melhor individuo), perdaAtiva**/
//        this->ordenaPopulacaoAtual(g);
//
//        RK_Individual *best = popAtual.at(this->tamPop - 1);
//        if (100*1000*best->getActiveLoss() < perda){
//            perda = 100*1000*best->getActiveLoss();
////            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
//            melhorGeracao = k;
//            it=0;
//        }else{
//            it++;
//        }
//
//        //chamada de path-relinking quando AG fica estagnado
//        if(it==it_s_melhora){
//
//            chamadaGeracao.push_back(k);
//
//            chamadasPr.push_back(k);
//            //preenche a pool
//            vector<RK_Individual*> pool;
//            for(int i=0; i<tam_elite; i++)
//                pool.push_back(popAtual.at(popAtual.size()-1-i));
//            for(int i=0; i<tam_pool-tam_elite;i++){
//                int id = rand() % (this->tamPop - tam_elite);
//                pool.push_back(popAtual.at(id));
//            }
//
//            vector<RK_Individual*> pool_cp = pool;//copia
////            printf("\nchamou pre na geracao %d!\n", k);
//            Evolutionary_path_relinking p(pool, max_it, pct_pool_elite);
//            bestPr = p.pre(g);
////            printf("\nterminou pre!\n");
//
//            if(bestPr->getPerdaAtiva() < popAtual.at(0)->getPerdaAtiva()){
////                printf("foi tentar atualizar pop com pr\n");
////                swap(popAtual.at(0), bestPr);
//                popAtual.erase(popAtual.begin());
//                popAtual.push_back(bestPr);
////                delete bestPr;
////                printf("deu\n");
//            }else{
//                delete bestPr;
//            }
//
////            printf("atualizou popAtual!");
//            it=0;
//        }
//
//        popAnterior = popAtual;
//
//        int num_piores = 0.05*this->tamPop;
//        int num_melhores = 0.1*this->tamPop;
//
//        for(int i=num_piores; i<this->tamPop-num_melhores; i++){
//
//            /** cruzamento entre pai1 e pai2 entre os
//            individuos aleatorios da populacao anterior
//            modificar por uma escolha em roleta no futuro**/
//            int pai1 = rand() % this->tamPop;
////            int pai2 = rand() % this->tamPop; //aleatorio
//
//            /*torneio com 3*/
//            int cand1, cand2, cand3;
//            cand1 = rand() % this->tamPop;
//            cand2 = rand() % this->tamPop;
//            cand3 = rand() % this->tamPop;
//
//            int aux = cand1;
//
//            if(popAnterior.at(cand1)->getPerdaAtiva() < popAnterior.at(cand2)->getPerdaAtiva()){
//                cand2=cand1;
//            }else{aux = cand2;}
//            if(popAnterior.at(cand2)->getPerdaAtiva() < popAnterior.at(cand3)->getPerdaAtiva()){
//                cand3=cand2;
//            }else{aux = cand3;}
//            pai1 = aux;
//
//            int pai2 = cand3;
//
//            while(pai2==pai1)
//                pai2 = rand() % this->tamPop;
//
//            //popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
//            this->cruzamento2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
//            //popAtual.at(i)->mutacao();
//            this->mutacao(popAtual.at(i));
//        }
//
//        /**aleatorio ao inves de manter piores**/
//        for(int i=0; i<num_piores; i++)
//            popAtual.at(i)->geraPesosAleatorios();
//    }
//
////    cout << "\n--------------------------------------"<<endl;
////    for(unsigned int i=0; i<popAtual.size(); i++)
////        cout << 100*1000*popAtual.at(i)->getPerdaAtiva() << " , ";
////    cout << "\n--------------------------------------"<<endl;
//
//    this->ordenaPopulacaoAtual(g);
//
////    for(unsigned int i=0; i<chamadasPr.size(); i++)
////        printf("%d,", chamadasPr.at(i));
//
//        for(unsigned long int i=0; i<chamadaGeracao.size();i++)
//            printf("%d,", chamadaGeracao.at(i));
//
//    return melhorGeracao;
//}


void Random_keys::cruzamento1(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho){
    for(int i=0; i< filho->getNumEdges(); i++)
        filho->getWeights()[i] = (pai1->getWeights()[i] + pai2->getWeights()[i])/2.0;
}

void Random_keys::mutacao(RK_Individual *ind){
    int i = rand() % 100;
    if(i<=5){
        int j = rand() % ind->getNumEdges();
        double peso = rand() % RANGEPESO;
        ind->getWeights()[j] = peso;
    }
    else{
        if(i<=15){
            int j = rand() % ind->getNumEdges();
            int k = rand() % ind->getNumEdges();
            double peso1 = rand() % RANGEPESO;
            double peso2 = rand() % RANGEPESO;
            ind->getWeights()[j] = peso1;
            ind->getWeights()[k] = peso2;
        }
    }
}

void Random_keys::cruzamento2(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho){
    for(int i=0; i< filho->getNumEdges(); i++){
        int j = rand() % 1000;
        if (j>=800){
            filho->getWeights()[i] = (pai1->getWeights()[i] + pai2->getWeights()[i])*2.0;
        }else{
            if (j >= 100){
                filho->getWeights()[i] = (pai1->getWeights()[i] + pai2->getWeights()[i])/2.0;
            }else{
                filho->getWeights()[i] = (pai1->getWeights()[i] + pai2->getWeights()[i]) * 0.75;
            }
        }
    }
}

void Random_keys::cruzamento3(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho) {
    shuffle(RK_Individual::cromossomos.begin(), RK_Individual::cromossomos.end(), std::default_random_engine(time(NULL)));

    for(unsigned long int i=0; i<RK_Individual::cromossomos.size()/2; i++)
        filho->getWeights()[RK_Individual::cromossomos.at(i)->position] = pai1->getWeights()[RK_Individual::cromossomos.at(i)->position];

    for(unsigned long int i=RK_Individual::cromossomos.size()/2; i<RK_Individual::cromossomos.size(); i++)
        filho->getWeights()[RK_Individual::cromossomos.at(i)->position] = pai2->getWeights()[RK_Individual::cromossomos.at(i)->position];

}

int Random_keys::avancaGeracoesGenerico(Graph *g, int tipoCruzamento) {

    int melhorGeracao = 0;
    RK_Individual *best = popAtual.at(this->tamPop - 1);
    this->ordenaPopulacaoAtual(g);
    double perda = 100*1000*best->getActiveLoss();
    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
         e ordena a populacao da maior perda(pior individuo)
         pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        RK_Individual *best = popAtual.at(this->tamPop - 1);
        if (100*1000*best->getActiveLoss() < perda){
            perda = 100*1000*best->getActiveLoss();
//            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
            melhorGeracao = k;
        }

        popAnterior = popAtual;

        int num_piores = 0.05*this->tamPop;
        int num_melhores = 0.1*this->tamPop;

        for(int i=num_piores; i<this->tamPop-num_melhores; i++){

            /* cruzamento entre pai1 e pai2 entre os
            individuos aleatorios da populacao anterior
            modificar por uma escolha em roleta no futuro*/

            int pai1 = rand() % this->tamPop;
//            int pai2 = rand() % this->tamPop; //aleatorio

            /*torneio com 3*/
            int cand1, cand2, cand3;
            cand1 = rand() % this->tamPop;
            cand2 = rand() % this->tamPop;
            cand3 = rand() % this->tamPop;

            int aux = cand1;

            if(popAnterior.at(cand1)->getActiveLoss() < popAnterior.at(cand2)->getActiveLoss()){
                cand2=cand1;
            }else{aux = cand2;}
            if(popAnterior.at(cand2)->getActiveLoss() < popAnterior.at(cand3)->getActiveLoss()){
                cand3=cand2;
            }else{aux = cand3;}
            pai1 = aux;

            int pai2 = cand3;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

//            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            if(tipoCruzamento==0)
                this->cruzamento1(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
            if(tipoCruzamento==1)
                this->cruzamento2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
            if(tipoCruzamento==2)
                this->cruzamento3(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
            if(tipoCruzamento==3)
                this->cruzamento4(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
            if(tipoCruzamento==4)
                this->cruzamento5(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
            if(tipoCruzamento==5)
                this->cruzamento6(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));

//            popAtual.at(i)->mutacao();
            this->mutacao(popAtual.at(i));
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->generate_random_weights();
    }

//    cout << "\n--------------------------------------"<<endl;
//    for(unsigned int i=0; i<popAtual.size(); i++)
//        cout << 100*1000*popAtual.at(i)->getPerdaAtiva() << " , ";
//    cout << "\n--------------------------------------"<<endl;

    this->ordenaPopulacaoAtual(g);

    return melhorGeracao;
}

void Random_keys::cruzamento4(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho) {
    shuffle(RK_Individual::cromossomos.begin(), RK_Individual::cromossomos.end(), std::default_random_engine(time(NULL)));

    int tam = RK_Individual::cromossomos.size();

    int tamMelhor = round(0.8*tam);

    RK_Individual *melhorPai = pai2, *piorPai = pai1;
    if(pai1->getActiveLoss()<pai2->getActiveLoss()){
        melhorPai = pai1;
        piorPai = pai2;
    }

    //80% vem do melhor pai
    for(unsigned long int i=0; i<tamMelhor; i++)
        filho->getWeights()[RK_Individual::cromossomos.at(i)->position] = melhorPai->getWeights()[RK_Individual::cromossomos.at(i)->position];

    //20% vem do pior pai
    for(unsigned long int i=tamMelhor; i<RK_Individual::cromossomos.size(); i++)
        filho->getWeights()[RK_Individual::cromossomos.at(i)->position] = piorPai->getWeights()[RK_Individual::cromossomos.at(i)->position];
}

void Random_keys::cruzamento5(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho) {
    shuffle(RK_Individual::cromossomos.begin(), RK_Individual::cromossomos.end(), std::default_random_engine(time(NULL)));

    int tam = RK_Individual::cromossomos.size();

    int tam04 = round(0.4*tam);

    //40% vem do pai1
    for(unsigned long int i=0; i<tam04; i++)
        filho->getWeights()[RK_Individual::cromossomos.at(i)->position] = pai1->getWeights()[RK_Individual::cromossomos.at(i)->position];

    //40% vem do pai12
    for(unsigned long int i=tam04; i<RK_Individual::cromossomos.size()-tam04; i++)
        filho->getWeights()[RK_Individual::cromossomos.at(i)->position] = pai2->getWeights()[RK_Individual::cromossomos.at(i)->position];

    //20% gerado de forma aleatoria
    for(unsigned long int i=RK_Individual::cromossomos.size()-tam04; i<RK_Individual::cromossomos.size(); i++)
        filho->getWeights()[RK_Individual::cromossomos.at(i)->position] = rand() % 100;

}

void Random_keys::cruzamento6(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho) {
    int i = rand() % 5;

    if(i==0)
        this->cruzamento1(pai1, pai2, filho);
    if(i==1)
        this->cruzamento2(pai1, pai2, filho);
    if(i==2)
        this->cruzamento3(pai1, pai2, filho);
    if(i==3)
        this->cruzamento4(pai1, pai2, filho);
    if(i==4)
        this->cruzamento5(pai1, pai2, filho);

}

//int Random_keys::avancaGeracoesPRECA(Graph *g, int it_s_melhora, int tam_pool, int max_it, float pct_pool_elite) {
//
//    int numChamadasPr = 0;
//    vector<int> chamadaGeracao;
//    int tam_elite = round(pct_pool_elite*tam_pool);
//    int it = 0;
//    vector<int> chamadasPr;
//    RK_Individual *bestPr;
//
//    int melhorGeracao = 0;
//    RK_Individual *best = popAtual.at(this->tamPop - 1);
//    this->ordenaPopulacaoAtual(g);
//    double perda = 100*1000*best->getActiveLoss();
//    for(int k=0; k<this->numGeracoes; k++){
//
//        /** calcula a funcao criterio para cada individuo
//         e ordena a populacao da maior perda(pior individuo)
//         pra menor perda(melhor individuo), perdaAtiva**/
//        this->ordenaPopulacaoAtual(g);
//
//        RK_Individual *best = popAtual.at(this->tamPop - 1);
//        if (100*1000*best->getActiveLoss() < perda){
//            perda = 100*1000*best->getActiveLoss();
////            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
//            melhorGeracao = k;
//            it=0;
//        }else{
//            it++;
//        }
//
//        //chamada de path-relinking quando AG fica estagnado
//        if(it==it_s_melhora){
//
//            chamadaGeracao.push_back(k);
//
//            chamadasPr.push_back(k);
//            //preenche a pool
//            vector<RK_Individual*> pool;
//            for(int i=0; i<tam_elite; i++)
//                pool.push_back(popAtual.at(popAtual.size()-1-i));
//            for(int i=0; i<tam_pool-tam_elite;i++){
//                int id = rand() % (this->tamPop - tam_elite);
//                pool.push_back(popAtual.at(id));
//            }
//
//            vector<RK_Individual*> pool_cp = pool;//copia
////            printf("\nchamou pre na geracao %d!\n", k);
//            Evolutionary_path_relinking *p = new Evolutionary_path_relinking();
//            bestPr = (p->run(pool, max_it, pct_pool_elite, g))->OS_to_RK(g);
//            delete  p;
////            printf("\nterminou pre!\n");
//
//            if(bestPr->getPerdaAtiva() < popAtual.at(0)->getPerdaAtiva()){
////                printf("foi tentar atualizar pop com pr\n");
////                swap(popAtual.at(0), bestPr);
//                popAtual.erase(popAtual.begin());
//                popAtual.push_back(bestPr);
////                delete bestPr;
////                printf("deu\n");
//            }else{
//                delete bestPr;
//            }
//
////            printf("atualizou popAtual!");
//            it=0;
//        }
//
//        popAnterior = popAtual;
//
//        int num_piores = 0.05*this->tamPop;
//        int num_melhores = 0.1*this->tamPop;
//
//        for(int i=num_piores; i<this->tamPop-num_melhores; i++){
//
//            /** cruzamento entre pai1 e pai2 entre os
//            individuos aleatorios da populacao anterior
//            modificar por uma escolha em roleta no futuro**/
//            int pai1 = rand() % this->tamPop;
////            int pai2 = rand() % this->tamPop; //aleatorio
//
//            /*torneio com 3*/
//            int cand1, cand2, cand3;
//            cand1 = rand() % this->tamPop;
//            cand2 = rand() % this->tamPop;
//            cand3 = rand() % this->tamPop;
//
//            int aux = cand1;
//
//            if(popAnterior.at(cand1)->getPerdaAtiva() < popAnterior.at(cand2)->getPerdaAtiva()){
//                cand2=cand1;
//            }else{aux = cand2;}
//            if(popAnterior.at(cand2)->getPerdaAtiva() < popAnterior.at(cand3)->getPerdaAtiva()){
//                cand3=cand2;
//            }else{aux = cand3;}
//            pai1 = aux;
//
//            int pai2 = cand3;
//
//            while(pai2==pai1)
//                pai2 = rand() % this->tamPop;
//
//            //popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
//            this->cruzamento2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
//            //popAtual.at(i)->mutacao();
//            this->mutacao(popAtual.at(i));
//        }
//
//        /**aleatorio ao inves de manter piores**/
//        for(int i=0; i<num_piores; i++)
//            popAtual.at(i)->geraPesosAleatorios();
//    }
//
////    cout << "\n--------------------------------------"<<endl;
////    for(unsigned int i=0; i<popAtual.size(); i++)
////        cout << 100*1000*popAtual.at(i)->getPerdaAtiva() << " , ";
////    cout << "\n--------------------------------------"<<endl;
//
//    this->ordenaPopulacaoAtual(g);
//
////    for(unsigned int i=0; i<chamadasPr.size(); i++)
////        printf("%d,", chamadasPr.at(i));
//
//    for(unsigned long int i=0; i<chamadaGeracao.size();i++)
//        printf("%d,", chamadaGeracao.at(i));
//
//    return melhorGeracao;
//}

int Random_keys::avancaGeracaoesAdaptativo(Graph *g, int it_s_melhora, float maxMutacao, float max_pct, float taxaTamMut, float taxaPctMut) {

    int it=0;
    vector<int> chamadaGeracao;

    ///parametros da mutacao adaptativa
    float tamMutacao = 0.05;
    float pctMutacao = 0.1;
    ///--------------------------------

    int melhorGeracao = 0;
    RK_Individual *best = popAtual.at(this->tamPop - 1);
    this->ordenaPopulacaoAtual(g);
    double perda = 100*1000*best->getActiveLoss();
    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
         e ordena a populacao da maior perda(pior individuo)
         pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        RK_Individual *best = popAtual.at(this->tamPop - 1);
        if (100*1000*best->getActiveLoss() < perda){
            perda = 100*1000*best->getActiveLoss();
//            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
            melhorGeracao = k;
            it=0;
        }else{
            it++;
        }

        if(it==it_s_melhora){
            if(tamMutacao<maxMutacao)
                tamMutacao*=taxaTamMut;
            if(pctMutacao<max_pct)
                pctMutacao*=taxaPctMut;
            if(pctMutacao<max_pct || tamMutacao<maxMutacao){
                chamadaGeracao.push_back(k);
//                printf("(%f, %f)", tamMutacao, pctMutacao);
            }
            it=0;
        }

        popAnterior = popAtual;

        int num_piores = 0.05*this->tamPop;
        int num_melhores = 0.1*this->tamPop;

        for(int i=num_piores; i<this->tamPop-num_melhores; i++){

            /** cruzamento entre pai1 e pai2 entre os
            individuos aleatorios da populacao anterior
            modificar por uma escolha em roleta no futuro**/
            int pai1 = rand() % this->tamPop;
//            int pai2 = rand() % this->tamPop; //aleatorio

            /*torneio com 3*/
            int cand1, cand2, cand3;
            cand1 = rand() % this->tamPop;
            cand2 = rand() % this->tamPop;
            cand3 = rand() % this->tamPop;

            int aux = cand1;

            if(popAnterior.at(cand1)->getActiveLoss() < popAnterior.at(cand2)->getActiveLoss()){
                cand2=cand1;
            }else{aux = cand2;}
            if(popAnterior.at(cand2)->getActiveLoss() < popAnterior.at(cand3)->getActiveLoss()){
                cand3=cand2;
            }else{aux = cand3;}
            pai1 = aux;

            int pai2 = cand3;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

//            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            this->cruzamento2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
//            popAtual.at(i)->mutacao();

            this->mutacaoAdaptavitva(popAtual.at(i), tamMutacao, pctMutacao);
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->generate_random_weights();
    }

//    cout << "\n--------------------------------------"<<endl;
//    for(unsigned int i=0; i<popAtual.size(); i++)
//        cout << 100*1000*popAtual.at(i)->getPerdaAtiva() << " , ";
//    cout << "\n--------------------------------------"<<endl;

    this->ordenaPopulacaoAtual(g);

    for(unsigned long int i=0; i<chamadaGeracao.size();i++)
        printf("%d,", chamadaGeracao.at(i));

    return melhorGeracao;
}

void Random_keys::mutacaoAdaptavitva(RK_Individual *ind, float pctTam, float pctChance) {

    int tam = round(pctTam* ind->getNumEdges());
    int n = round(pctChance*100);
    int m = rand() % 100;

    if(m<=n){
        for(int i=0; i<tam; i++){
            int j = rand() % ind->getNumEdges();

            ind->getWeights()[i] = rand() % RANGEPESO;
        }
    }
}

int
Random_keys::avancaGeracaoesAdaptativo2(Graph *g, int it_s_melhora, float maxMutacao, float max_pct, float taxaTamMut,
                                        float taxaPctMut) {

    int it=0;
    vector<int> chamadaGeracao;

    ///parametros da mutacao adaptativa
    float tamMutacao = 0.05;
    float pctMutacao = 0.1;
    ///--------------------------------

    int melhorGeracao = 0;
    RK_Individual *best = popAtual.at(this->tamPop - 1);
    this->ordenaPopulacaoAtual(g);
    double perda = 100*1000*best->getActiveLoss();
    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
         e ordena a populacao da maior perda(pior individuo)
         pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        RK_Individual *best = popAtual.at(this->tamPop - 1);
        if (100*1000*best->getActiveLoss() < perda){
            perda = 100*1000*best->getActiveLoss();
//            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
            melhorGeracao = k;
            it=0;
        }else{
            it++;
        }

        if(it==it_s_melhora){
            if(tamMutacao<maxMutacao)
                tamMutacao*=taxaTamMut;
            if(pctMutacao<max_pct)
                pctMutacao*=taxaPctMut;
            if(pctMutacao<max_pct || tamMutacao<maxMutacao){
                chamadaGeracao.push_back(k);
//                printf("(%f, %f)", tamMutacao, pctMutacao);
            }
            it=0;
        }

        popAnterior = popAtual;

        int num_piores = 0.05*this->tamPop;
        int num_melhores = 0.1*this->tamPop;

        for(int i=num_piores; i<this->tamPop-num_melhores; i++){

            /** cruzamento entre pai1 e pai2 entre os
            individuos aleatorios da populacao anterior
            modificar por uma escolha em roleta no futuro**/
            int pai1 = rand() % this->tamPop;
//            int pai2 = rand() % this->tamPop; //aleatorio

            /*torneio com 3*/
            int cand1, cand2, cand3;
            cand1 = rand() % this->tamPop;
            cand2 = rand() % this->tamPop;
            cand3 = rand() % this->tamPop;

            int aux = cand1;

            if(popAnterior.at(cand1)->getActiveLoss() < popAnterior.at(cand2)->getActiveLoss()){
                cand2=cand1;
            }else{aux = cand2;}
            if(popAnterior.at(cand2)->getActiveLoss() < popAnterior.at(cand3)->getActiveLoss()){
                cand3=cand2;
            }else{aux = cand3;}
            pai1 = aux;

            int pai2 = cand3;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

//            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            this->cruzamento2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
//            popAtual.at(i)->mutacao();

            if(i<=5*num_piores)
                this->mutacaoAdaptavitva(popAtual.at(i), tamMutacao, pctMutacao);
            else
                this->mutacao(popAtual.at(i));
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->generate_random_weights();
    }

//    cout << "\n--------------------------------------"<<endl;
//    for(unsigned int i=0; i<popAtual.size(); i++)
//        cout << 100*1000*popAtual.at(i)->getPerdaAtiva() << " , ";
//    cout << "\n--------------------------------------"<<endl;

    this->ordenaPopulacaoAtual(g);

    for(unsigned long int i=0; i<chamadaGeracao.size();i++)
        printf("%d,", chamadaGeracao.at(i));

    return melhorGeracao;
}
