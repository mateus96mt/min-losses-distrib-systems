#include <stdio.h>
#include "Grafo.h"
#include "Random_keys.h"
#include "time.h"
#include <algorithm>
#include <math.h>

#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist33barras_Yang.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/SISTEMA119s2.m"

#define configuracao "inicial"
//#define configuracao "literatura1"
//#define configuracao "ARSD"

void abreChaves(Grafo *g, int *ids, int n);
void defineConfiguracao(Grafo *g);
void testeDestrutor();
void testeArestasModificaveis();
void testeCopiaGrafo();
void testePopulacaoAleatoria();
void testeEntradas();
bool ordenacao(Grafo *g1, Grafo *g2);
void testeMemLeakRandomKeys();

void testeRandomKeys();

int main(){

    srand(time(NULL));

//    testeEntradas();//perda total e tensao minima para cada configuracao para compara com a tese do leonardo willer

//    testeDestrutor();

//    testeArestasModificaveis();

//    testeCopiaGrafo();

//    testePopulacaoAleatoria();

//    testeMemLeakRandomKeys();

    testeRandomKeys();
}

void testeRandomKeys(){
    char nome[] = arquivoEntrada;
    Grafo *g = new Grafo();

    g->leEntrada(nome);

    /** numero de individuos da populacao/numero de geracaoes **/
    Random_keys *rd = new Random_keys(100, 100);

    /** populacao inicial gerada de forma aleatoria **/
    rd->geraPopAleatoria(g);

    /** faz cruzamentos e mutacoes para gerar individuos da nova populacao **/
    rd->avancaGeracoes(g);

    /** melhor individuo eh o ultimo (menor perda) da populacao da ultima geracao **/
    Individuo *best = rd->getPopAtual().at(rd->getTamPopulacao()-1);

    /** abre e fecha os arcos correspondentes do grafo *g para calcular funcao Objetivo**/
    best->calculaFuncaoObjetivo(g);

    printf("\n\n\nMELHOR INDIVIDUO FINAL:");
    printf("\nAberto:{");
    for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){
        for(Arco *a = no->getListaArcos(); a!=NULL; a = a->getProxArco()){
            Arco *volta = g->buscaArco(a->getNoDestino()->getID(), a->getNoOrigem()->getID());
            if(a->getChave()==false && volta->getChave()==false)
                printf("%d,", a->getID());
        }
    }
    printf("  }\n");
    printf("PerdaAtiva: %f (kw)\n\n\n", 100*1000*best->getPerdaAtiva());


    /** so para conferir mesmo  e ter certeza da perda do individuo **/
    g->zeraFluxosEPerdas();
    g->calcula_fluxos_e_perdas(1e-5);
    printf("(olhando pro grafo so pra ter certeza do calculo)\nPerdaAtiva: %f (kw)\n\n\n", 100*1000*g->soma_perdas()[0]);


}

/** pequeno vazamento de memoria na funcao "calculaFuncaoObjetivo" do individuo novamente no vector **/
void testeMemLeakRandomKeys(){
    char nome[] = arquivoEntrada;
    Grafo *g = new Grafo();

    g->leEntrada(nome);

    printf("\n num nos: %d         num arcos: %d", g->getNumeroNos(), g->getNumeroArcos());

    Individuo *i = new Individuo(g->getNumeroArcos());

    i->geraPesosAleatorios();
    while(true){
        i->calculaFuncaoObjetivo(g);
        printf("\n\nperdaAtiv: %f kw,  perdaReativ: %f kw      eh arvore? %d",
        1000*100*i->getPerdaAtiva(), 1000*100*i->getPerdaReativa(), g->ehArvore());
    }
}

void testePopulacaoAleatoria(){
    char nome[] = arquivoEntrada;
    Grafo *g = new Grafo();

    g->leEntrada(nome);

    int n_individuos = 500;
    vector<Grafo*> h;

    for(int i=0; i<n_individuos; i++){

        double *perda;
        printf("\n\nsolucao  %d:", i);
        h.push_back(g->retornaCopia());
        h.at(i)->solucaoAleatoria();
        h.at(i)->define_sentido_fluxos();
        h.at(i)->calcula_fluxos_e_perdas(1e-8);
        perda = h.at(i)->soma_perdas();

        printf("\nvalida? %d", h.at(i)->ehArvore());
        printf("  tensao minima: %.5f (p.u)", h.at(i)->tensaoMinima());
        printf("  perdaTotal: %.5f (kW)", 100*1000*perda[0]);

        delete perda;

    }
}

void testeEntradas(){
    char nome[] = arquivoEntrada;
    Grafo *g = new Grafo();
    g->leEntrada(nome);

    defineConfiguracao(g);

    g->define_sentido_fluxos();
    g->calcula_fluxos_e_perdas(1e-8);


    printf("\ntensao minima: %.5f (p.u)", g->tensaoMinima());
    printf("\nperdaTotal: %.5f (kW)", 100*1000*g->soma_perdas()[0]);
    printf("\neh Conexo? %d", g->ehConexo());
    printf("\neh arvore? %d\n\n\n", g->ehArvore());
}

void testeCopiaGrafo(){
    char nome[] = arquivoEntrada;
    Grafo *g, *h;
    g = new Grafo();
    g->leEntrada(nome);

    defineConfiguracao(g);
    g->define_sentido_fluxos();
    g->calcula_fluxos_e_perdas(1e-8);


    for(int i=0; true; i++){
        h = g->retornaCopia();
        h->calcula_fluxos_e_perdas(1e-8);

        double *perda = h->soma_perdas();
        printf("\ntensao minima: %.5f (p.u)", h->tensaoMinima());
        printf("\nperdaTotal: %.5f (kW)", 100*1000*perda[0]);
        printf("\neh arvore? %d\n\n\n", h->ehArvore());
        delete perda;

        delete h;
    }
}

void testeArestasModificaveis(){
    char nome[] = arquivoEntrada;
    Grafo *g;
    g = new Grafo();
    g->leEntrada(nome);
    g->resetaGrausAuxiliares();
//    g->imprime();

    //TESTE ARESTAS MODIFICAVEIS PARA ARQUIVO DE 33 BARRAS

//    int ids[] = {36, 34, 2, 25, 33};
    int ids[] = {32, 10, 20, 4, 12};

    for(int i=0; i<2; i++){

        printf("\nabriu A{%d}", ids[i]);

        Arco *a = g->buscaArco(ids[i]);

        No *noOrigem = a->getNoOrigem();
        No *noDestino = a->getNoDestino();

        a->setChave(false);
        a->setModificavel(false);
        a = g->buscaArco(noDestino->getID(), noOrigem->getID());
        a->setChave(false);
        a->setModificavel(false);

        printf("modif = %d", a->getModificavel());

        noOrigem->setGrauAux(noOrigem->getGrauAux()-1);
        noDestino->setGrauAux(noDestino->getGrauAux()-1);

        g->defineArestasModificaveis();

    }
    g->imprime();

}

void testeDestrutor(){
    char nome[] = arquivoEntrada;

    while(true){
        Grafo *g;
        g = new Grafo();
        g->leEntrada(nome);
        delete g;
    }
}

void defineConfiguracao(Grafo *g){

    if(arquivoEntrada=="ENTRADAS_MODIFICADAS/sist33barras_Yang.m"){

        int ids[5];

        //CONFIGURACAO INICIAL
        if(configuracao == "inicial"){
            ids[0] = 33; ids[1] = 34; ids[2] = 35; ids[3] = 36; ids[4] = 37;
        }
        //CONFIGURACAO DA LITERATURA
        if(configuracao == "literatura1"){
            ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 32; ids[4] = 37;
        }
        //ABORDAGEM ARSD
        if(configuracao == "ARSD"){
            ids[0] = 7; ids[1] = 9; ids[2] = 14; ids[3] = 32; ids[4] = 37;
        }

        abreChaves(g, ids, 5);
    }

    if(arquivoEntrada=="ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m"){

        int ids[5];

        //CONFIGURACAO INICIAL
        if(configuracao == "inicial"){
            ids[0] = 33; ids[1] = 34; ids[2] = 35; ids[3] = 36; ids[4] = 37;
        }
        //CONFIGURACAO DA LITERATURA
        if(configuracao == "literatura1"){
            ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 28; ids[4] = 36;
        }
        //ABORDAGEM ARSD
        if(configuracao == "ARSD"){
            ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 16; ids[4] = 28;
        }

        abreChaves(g, ids, 5);
    }

    if(arquivoEntrada=="ENTRADAS_MODIFICADAS/SISTEMA119s2.m"){

        int ids[15];
        //CONFIGURACAO INICIAL
        if(configuracao == "inicial"){
            ids[0] = 119; ids[1] = 120; ids[2] = 121; ids[3] = 122; ids[4] = 123;
            ids[5] = 124; ids[6] = 125; ids[7] = 126; ids[8] = 127; ids[9] = 128;
            ids[10] = 129; ids[11] = 130; ids[12] = 131; ids[13] = 132; ids[14] = 133;


        }
        //CONFIGURACAO DA LITERATURA
        if(configuracao == "literatura1"){
            ids[0] = 24; ids[1] = 27; ids[2] = 35; ids[3] = 40; ids[4] = 43;
            ids[5] = 52; ids[6] = 59; ids[7] = 72; ids[8] = 75; ids[9] = 96;
            ids[10] = 99; ids[11] = 110; ids[12] = 123; ids[13] = 130; ids[14] = 131;
        }
        //ABORDAGEM ARSD
        if(configuracao == "ARSD"){
            ids[0] = 24; ids[1] = 26; ids[2] = 35; ids[3] = 40; ids[4] = 43;
            ids[5] = 51; ids[6] = 61; ids[7] = 72; ids[8] = 75; ids[9] = 96;
            ids[10] = 98; ids[11] = 110; ids[12] = 122; ids[13] = 130; ids[14] = 131;
        }

        /*
        esse arquivo tem na verdade 132 arcos ao inves de 133
        dessa forma os ids dos arcos abertos deve ser subtraido 1.
        */
        for(int i=0; i<15; i++)
            ids[i]--;

        abreChaves(g, ids, 15);
    }
}

void abreChaves(Grafo *g, int *ids, int n){
    Arco *a;
    for(int i=0; i<n; i++){
        a = g->buscaArco(ids[i]);
        a->setChave(false);
        a = g->buscaArco(a->getNoDestino()->getID(), a->getNoOrigem()->getID());
        a->setChave(false);
    }
}
