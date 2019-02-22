#include "Random_keys.h"
#include <unistd.h>

using namespace std;

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

            popAnterior.at(pai1)->cruzamentoMedia(popAnterior.at(pai2), popAtual.at(i));
            popAtual.at(i)->mutacao();
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

            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            popAtual.at(i)->mutacao();
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

            Individuo *ind = popAtual.at(id1)->prs(popAtual.at(id2), g, indRef);

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

            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            popAtual.at(i)->mutacao();
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->geraPesosAleatorios();

    }

    this->ordenaPopulacaoAtual(g);

    return melhorGeracao;
}

void Random_keys::prsEvolutivo(vector<Individuo*> pool, vector<Individuo*> &populacao, Grafo *g){

    Individuo *indRef, *ind;

    sort(pool.begin(), pool.end(), ordenacaoIndividuo);//ordenar novamente a pool atualizada

    //path relinking evolutivo entre todos os individuos da pool, par a par
    for(unsigned int i=0; i<pool.size(); i++){

        for(unsigned int j=0; j<pool.size(); j++){

            if(i!=j){

                indRef = pool.at(0);

                ind = pool.at(i)->prs(pool.at(j), g, indRef);//path relinking

                if(ind->getPerdaAtiva() < indRef->getPerdaAtiva()){//atualizacao da pool

                    pool.at(0) = ind;
                    i = 0;
                    j = 0;

                    sort(pool.begin(), pool.end(), ordenacaoIndividuo);//ordenar novamente a pool atualizada

                }

            }

        }

    }

    //acrescenta pool atualizada na populacao
    for(unsigned int i=0; i<pool.size(); i++)
        populacao.push_back(pool.at(i));

}

int Random_keys::avancaGeracoesPRSEvolutivoFinal(Grafo *g){

    int num_piores, num_melhores;
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

        num_piores = 0.05*this->tamPop;
        num_melhores = 0.1*this->tamPop;

        for(int i=num_piores; i<this->tamPop-num_melhores; i++){

            /** cruzamento entre pai1 e pai2 entre os
            individuos aleatorios da populacao anterior
            modificar por uma escolha em roleta no futuro**/
            int pai1 = rand() % this->tamPop;

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

            popAnterior.at(pai1)->cruzamentoMedia2(popAnterior.at(pai2), popAtual.at(i));
            popAtual.at(i)->mutacao();
        }

        /**aleatorio ao inves de manter piores**/
        for(int i=0; i<num_piores; i++)
            popAtual.at(i)->geraPesosAleatorios();
    }

    vector<Individuo*> pool;
    for(int i=0; i<num_melhores; i++)
        pool.push_back(popAtual.at(tamPop - num_melhores + i));

    clock_t inicio = clock();
    prsEvolutivo(pool, popAtual, g);//execucao do path reliking evolutivo no final
    clock_t fim = clock();
    printf("tempo prsEvolutivo(isoladamente) (pool size = %d):  %.2lf\n",  num_melhores, (float)(fim-inicio)/CLOCKS_PER_SEC);

    this->tamPop = popAtual.size();

    this->ordenaPopulacaoAtual(g);

    return melhorGeracao;
}
