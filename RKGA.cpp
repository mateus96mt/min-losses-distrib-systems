#include "RKGA.h"
#include <unistd.h>

using namespace std;

#define RANGEPESO 100

bool ordenacaoIndividuo(Individuo *i1, Individuo *i2){return i1->getPerdaAtiva() > i2->getPerdaAtiva();}

Random_keys::Random_keys(int tamPop, int numGeracoes){
    this->tamPop = tamPop;
    this->numGeracoes = numGeracoes;
}

void Random_keys::geraPopAleatoria(Grafo *g){

    g->marcaUmsentidoArcos();
    Individuo *ind;
    for(int i=0; i<this->tamPop; i++){

        ind = new Individuo(g->getNumeroArcos()/2 - g->getN_naoModificaveis());
        ind->geraPesosAleatorios();

        popAtual.push_back(ind);
    }
    popAnterior = popAtual;
}

void Random_keys::geraPopAleatoriaConfInicial(Grafo *g, int *idsAbertos, int nAbertos){

    g->marcaUmsentidoArcos();
    Individuo *ind;
    for(int i=0; i<this->tamPop; i++){

        ind = new Individuo(g->getNumeroArcos()/2 - g->getN_naoModificaveis());
        if(i==0){
            ind->geraPesosConfInicial(idsAbertos, nAbertos, g);
        }
        else{
            ind->geraPesosAleatorios();
        }

        popAtual.push_back(ind);
    }
    popAnterior = popAtual;
}

/** ordena populacao em ordem decrescente por valor da funcao objetivo
dado que queremos minimizar a perda os piores individuos ficam no inicio(perda maior)
queremos os melhore (menor perda, fim da lista) **/
void Random_keys::ordenaPopulacaoAtual(Grafo *g){
    for(unsigned int i=0; i<popAtual.size(); i++)
        popAtual.at(i)->calculaFuncaoObjetivoOtimizado(g);
//        popAtual.at(i)->calculaFuncaoObjetivo(g);

    sort(popAtual.begin(), popAtual.end(), ordenacaoIndividuo);
}

int Random_keys::avancaGeracoes(Grafo *g){

    int melhorGeracao = 0;
    Individuo *best = popAtual.at(this->tamPop-1);
    this->ordenaPopulacaoAtual(g);
    double perda = 100*1000*best->getPerdaAtiva();
    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
        e ordena a populacao da maior perda(pior individuo)
        pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        Individuo *best = popAtual.at(this->tamPop-1);
        if (100*1000*best->getPerdaAtiva() < perda){
            perda = 100*1000*best->getPerdaAtiva();
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

            if(popAnterior.at(cand1)->getPerdaAtiva() < popAnterior.at(cand2)->getPerdaAtiva()){
                cand2=cand1;
            }
            if(popAnterior.at(cand2)->getPerdaAtiva() < popAnterior.at(cand3)->getPerdaAtiva()){
                cand3=cand2;
            }

            int pai2 = cand3;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

//            popAnterior.at(pai1)->cruzamentoMedia(popAnterior.at(pai2), popAtual.at(i));
            this->cruzamentoMedia(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
//            popAtual.at(i)->mutacao();
            this->mutacao(popAtual.at(i));
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->geraPesosAleatorios();
    }

    this->ordenaPopulacaoAtual(g);

    return melhorGeracao;
}

int Random_keys::avancaGeracoes2(Grafo *g){

    int melhorGeracao = 0;
    Individuo *best = popAtual.at(this->tamPop-1);
    this->ordenaPopulacaoAtual(g);
    double perda = 100*1000*best->getPerdaAtiva();
    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
         e ordena a populacao da maior perda(pior individuo)
         pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        Individuo *best = popAtual.at(this->tamPop-1);
        if (100*1000*best->getPerdaAtiva() < perda){
            perda = 100*1000*best->getPerdaAtiva();
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

            if(popAnterior.at(cand1)->getPerdaAtiva() < popAnterior.at(cand2)->getPerdaAtiva()){
                cand2=cand1;
            }else{aux = cand2;}
            if(popAnterior.at(cand2)->getPerdaAtiva() < popAnterior.at(cand3)->getPerdaAtiva()){
                cand3=cand2;
            }else{aux = cand3;}
            pai1 = aux;

            int pai2 = cand3;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

//            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            this->cruzamentoMedia2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
//            popAtual.at(i)->mutacao();
            this->mutacao(popAtual.at(i));
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->geraPesosAleatorios();
    }

//    cout << "\n--------------------------------------"<<endl;
//    for(unsigned int i=0; i<popAtual.size(); i++)
//        cout << 100*1000*popAtual.at(i)->getPerdaAtiva() << " , ";
//    cout << "\n--------------------------------------"<<endl;

    this->ordenaPopulacaoAtual(g);

    return melhorGeracao;
}

int Random_keys::avancaGeracoesPRS(Grafo *g){

    int melhorGeracao = 0;
    Individuo *best = popAtual.at(this->tamPop-1);
    this->ordenaPopulacaoAtual(g);
    double perda = 100*1000*best->getPerdaAtiva();
    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
         e ordena a populacao da maior perda(pior individuo)
         pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        Individuo *best = popAtual.at(this->tamPop-1);
        if (100*1000*best->getPerdaAtiva() < perda){
            perda = 100*1000*best->getPerdaAtiva();
//            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
            melhorGeracao = k;
        }

        popAnterior = popAtual;

        int num_piores = 0.05*this->tamPop;
        int num_melhores = 0.1*this->tamPop;

        {//path relinking simples em cada geracao
            int id1 = (tamPop-num_melhores) + (rand() % num_melhores);
            int id2 = (tamPop-num_melhores) + (rand() % num_melhores);

            Individuo *indRef;//individuo de referencia para o pathrelinking

            if(popAtual.at(id1)->getPerdaAtiva() > popAtual.at(id2)->getPerdaAtiva())
                indRef = popAtual.at(id1);
            else
                indRef = popAtual.at(id2);

//            Individuo *ind = popAtual.at(id1)->prs(popAtual.at(id2), g, indRef);
            Path_relinking p(this->popAnterior, 0, 0);//objeto necessario para utilizacao de funcoes do path-relinking
            Individuo *ind = p.prs(popAtual.at(id1), popAtual.at(id2), g, indRef);

            if(ind->getPerdaAtiva() < popAtual.at(0)->getPerdaAtiva())//atualiza pior individuo da populacao com resultado do path relinking
                popAtual.at(0) = ind;
            else
                delete ind;

        }

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

            if(popAnterior.at(cand1)->getPerdaAtiva() < popAnterior.at(cand2)->getPerdaAtiva()){
                cand2=cand1;
            }else{aux = cand2;}
            if(popAnterior.at(cand2)->getPerdaAtiva() < popAnterior.at(cand3)->getPerdaAtiva()){
                cand3=cand2;
            }else{aux = cand3;}
            pai1 = aux;

            int pai2 = cand3;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

            //popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            this->cruzamentoMedia2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
            //popAtual.at(i)->mutacao();
            this->mutacao(popAtual.at(i));
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->geraPesosAleatorios();

    }

    this->ordenaPopulacaoAtual(g);

    return melhorGeracao;
}

//void Random_keys::prsEvolutivo(vector<Individuo*> pool, vector<Individuo*> &populacao, Grafo *g){
//
//    Individuo *indRef, *ind;
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
//                ind = pool.at(i)->prs(pool.at(j), g, indRef);//path relinking
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

//Individuo *Random_keys::pre(vector<Individuo*> pool, int max_it, float pct_pr_elite, Grafo *g){
//
//    unsigned int tam_pool = pool.size();
//
//    int tam_elite = round(pct_pr_elite*tam_pool);
//
//    Individuo *i;
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
//        i = pool.at(ind)->prs2(pool.at(guia), g);//ja tem funcao objetivo calculada
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

//int Random_keys::avancaGeracoesPRSEvolutivoFinal(Grafo *g){
//
//    int num_piores, num_melhores;
//    int melhorGeracao = 0;
//    Individuo *best = popAtual.at(this->tamPop-1);
//    this->ordenaPopulacaoAtual(g);
//    double perda = 100*1000*best->getPerdaAtiva();
//    for(int k=0; k<this->numGeracoes; k++){
//
//        /** calcula a funcao criterio para cada individuo
//         e ordena a populacao da maior perda(pior individuo)
//         pra menor perda(melhor individuo), perdaAtiva**/
//        this->ordenaPopulacaoAtual(g);
//
//        Individuo *best = popAtual.at(this->tamPop-1);
//        if (100*1000*best->getPerdaAtiva() < perda){
//            perda = 100*1000*best->getPerdaAtiva();
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
//            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
//            popAtual.at(i)->mutacao();
//        }
//
//        /**aleatorio ao inves de manter piores**/
//        for(int i=0; i<num_piores; i++)
//            popAtual.at(i)->geraPesosAleatorios();
//    }
//
//    vector<Individuo*> pool;
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


int Random_keys::avancaGeracoesPRE(Grafo *g, int it_s_melhora, int tam_pool, int max_it, float pct_pool_elite){

    cout << "avanca geracoes\n";

    int tam_elite = round(pct_pool_elite*tam_pool);
    int it = 0;
    vector<int> chamadasPr;
    Individuo *bestPr;

    int melhorGeracao = 0;
    Individuo *best = popAtual.at(this->tamPop-1);
    this->ordenaPopulacaoAtual(g);
    double perda = 100*1000*best->getPerdaAtiva();
    for(int k=0; k<this->numGeracoes; k++){

        /** calcula a funcao criterio para cada individuo
         e ordena a populacao da maior perda(pior individuo)
         pra menor perda(melhor individuo), perdaAtiva**/
        this->ordenaPopulacaoAtual(g);

        Individuo *best = popAtual.at(this->tamPop-1);
        if (100*1000*best->getPerdaAtiva() < perda){
            perda = 100*1000*best->getPerdaAtiva();
            printf("\ngeracao (%d)  melhor individuo: %lf kw", k, 100*1000*best->getPerdaAtiva());//resultado ja em kw
            melhorGeracao = k;
            it=0;
        }else{
            it++;
        }

        //chamada de path-relinking quando AG fica estagnado
        if(it==it_s_melhora){

            chamadasPr.push_back(k);
            //preenche a pool
            vector<Individuo*> pool;
            for(int i=0; i<tam_elite; i++)
                pool.push_back(popAtual.at(popAtual.size()-1-i));
            for(int i=0; i<tam_pool-tam_elite;i++){
                int id = rand() % (this->tamPop - tam_elite);
                pool.push_back(popAtual.at(id));
            }

            vector<Individuo*> pool_cp = pool;//copia
//            printf("\nchamou pre na geracao %d!\n", k);
            Path_relinking p(pool, max_it, pct_pool_elite);
            bestPr = p.pre(g);
//            printf("\nterminou pre!\n");

            if(bestPr->getPerdaAtiva() < popAtual.at(0)->getPerdaAtiva()){
//                printf("foi tentar atualizar pop com pr\n");
//                swap(popAtual.at(0), bestPr);
                popAtual.erase(popAtual.begin());
                popAtual.push_back(bestPr);
//                delete bestPr;
//                printf("deu\n");
            }else{
                delete bestPr;
            }

//            printf("atualizou popAtual!");
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

            if(popAnterior.at(cand1)->getPerdaAtiva() < popAnterior.at(cand2)->getPerdaAtiva()){
                cand2=cand1;
            }else{aux = cand2;}
            if(popAnterior.at(cand2)->getPerdaAtiva() < popAnterior.at(cand3)->getPerdaAtiva()){
                cand3=cand2;
            }else{aux = cand3;}
            pai1 = aux;

            int pai2 = cand3;

            while(pai2==pai1)
                pai2 = rand() % this->tamPop;

            //popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            this->cruzamentoMedia2(popAnterior.at(pai1), popAnterior.at(pai2), popAtual.at(i));
            //popAtual.at(i)->mutacao();
            this->mutacao(popAtual.at(i));
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->geraPesosAleatorios();
    }

//    cout << "\n--------------------------------------"<<endl;
//    for(unsigned int i=0; i<popAtual.size(); i++)
//        cout << 100*1000*popAtual.at(i)->getPerdaAtiva() << " , ";
//    cout << "\n--------------------------------------"<<endl;

    this->ordenaPopulacaoAtual(g);

    for(unsigned int i=0; i<chamadasPr.size(); i++)
        printf("%d,", chamadasPr.at(i));

    return melhorGeracao;
}


void Random_keys::cruzamentoMedia(Individuo *pai1, Individuo *pai2, Individuo *filho){
    for(int i=0; i<filho->getNumArcos(); i++)
        filho->getPesos()[i] = (pai1->getPesos()[i] + pai2->getPesos()[i])/2.0;
}

void Random_keys::mutacao(Individuo *ind){
    int i = rand() % 100;
    if(i<=5){
        int j = rand() % ind->getNumArcos();
        double peso = rand() % RANGEPESO;
        ind->getPesos()[j] = peso;
    }
    else{
        if(i<=15){
            int j = rand() % ind->getNumArcos();
            int k = rand() % ind->getNumArcos();
            double peso1 = rand() % RANGEPESO;
            double peso2 = rand() % RANGEPESO;
            ind->getPesos()[j] = peso1;
            ind->getPesos()[k] = peso2;
        }
    }
}

void Random_keys::cruzamentoMedia2(Individuo *pai1, Individuo *pai2, Individuo *filho){
    for(int i=0; i<filho->getNumArcos(); i++){
        int j = rand() % 1000;
        if (j>=800){
            filho->getPesos()[i] = (pai1->getPesos()[i] + pai2->getPesos()[i])*2.0;
        }else{
            if (j >= 100){
                filho->getPesos()[i] = (pai1->getPesos()[i] + pai2->getPesos()[i])/2.0;
            }else{
                filho->getPesos()[i] = (pai1->getPesos()[i] + pai2->getPesos()[i]) * 0.75;
            }
        }
    }
}
