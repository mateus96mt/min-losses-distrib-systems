#include <stdio.h>
#include "Grafo.h"
#include "Random_keys.h"
#include "time.h"
#include <algorithm>
#include <math.h>
#include <string.h>

//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist33barras_Yang.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/SISTEMA119s2.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist135barras.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist215barras.m"
#define arquivoEntrada "ENTRADAS_MODIFICADAS/SISTEMA83_TAIWAN.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/SISTEMA83_TAIWAN_modificado.m"

#define configuracao "inicial"
//#define configuracao "literatura1"
//#define configuracao "literatura2"
//#define configuracao "ARSD"

void abreChaves(Grafo *g, int *ids, int n);
void defineConfiguracao(Grafo *g);
void testeDestrutor();
void testeArestasModificaveis();
void testeArestasModificaveis2();
void testeCopiaGrafo();
void testePopulacaoAleatoria();
void testeEntradas();
bool ordenacao(Grafo *g1, Grafo *g2);
void testeMemLeakRandomKeys();
int *configuracaoInicial();

void testeRandomKeys();
void testeConfInicial();

int main(){

    unsigned long int semente = time(NULL);
    semente = 1537808177; //melhor semente para 119 barras
//    semente = 1530715848; //melhor semente para 33 barras modificado
//    semente = 1536085327; //melhor semente para 94 barras original (470,10 kw)
//    semente = 1536082004; //melhor semente para 94 barras modificado (491,96 kw)
    srand(semente);

//    testeEntradas();//perda total e tensao minima para cada configuracao para compara com a tese do leonardo willer

//    testeDestrutor();

//    testeArestasModificaveis();

//    testeArestasModificaveis2();

//    testeCopiaGrafo();

//    testePopulacaoAleatoria();

//    testeMemLeakRandomKeys();

    testeRandomKeys();

//    testeConfInicial();

    printf("semente: %lu", semente);
}

void testeRandomKeys(){
    char nome[] = arquivoEntrada;
    Grafo *g = new Grafo();

    g->leEntrada(nome);
    g->defineArestasModificaveis();

    /** numero de individuos da populacao,numero de geracaoes **/
    Random_keys *rd = new Random_keys(100, 1000);

    /** populacao inicial gerada de forma aleatoria **/
//    rd->geraPopAleatoria(g);
    rd->geraPopAleatoriaConfInicial(g, configuracaoInicial(), g->getNumeroArcos()/2 - (g->getNumeroNos() - 1));

    /** faz cruzamentos e mutacoes para gerar individuos da nova populacao **/
    rd->avancaGeracoes(g);

    /** melhor individuo eh o ultimo (menor perda) da populacao da ultima geracao **/
    Individuo *best = rd->getPopAtual().at(rd->getTamPopulacao()-1);

    /** abre e fecha os arcos correspondentes do grafo *g para calcular funcao Objetivo**/
    best->calculaFuncaoObjetivo(g);


    /**Para imprimir as chaves abertas de forma ordenada, sem repeticao de arcos**/
    vector<int> chavesAbertas;
    printf("\n\n\nMELHOR INDIVIDUO FINAL:");
    printf("\nAberto:{");
    for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){
        for(Arco *a = no->getListaArcos(); a!=NULL; a = a->getProxArco()){
            Arco *volta = g->buscaArco(a->getNoDestino()->getID(), a->getNoOrigem()->getID());
            if(a->getChave()==false && volta->getChave()==false)
                chavesAbertas.push_back(a->getID());
        }
    }
    sort(chavesAbertas.begin(), chavesAbertas.end());
    for(unsigned int i=0; i<chavesAbertas.size();i+=2)
        printf("%d,", chavesAbertas.at(i));
    printf("  }\n");

    printf("PerdaAtiva: %f (kw)\n", 100*1000*best->getPerdaAtiva());
    printf("Tensao minima: %f (pu)\n\n\n", g->tensaoMinima());

//    /** so para conferir mesmo  e ter certeza da perda do individuo **/
//    g->zeraFluxosEPerdas();
//    g->calcula_fluxos_e_perdas(1e-8);
//    printf("(olhando pro grafo so pra ter certeza do calculo)\nPerdaAtiva: %f (kw)\n\n\n", 100*1000*g->soma_perdas()[0]);
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

void testeArestasModificaveis2(){
    char nome[] = arquivoEntrada;
    Grafo *g;
    g = new Grafo();
    g->leEntrada(nome);
    g->resetaGrausAuxiliares();

    g->defineArestasModificaveis();

    vector<int> naoModificaveis;
    for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){

        for(Arco *a = no->getListaArcos(); a!=NULL; a = a->getProxArco()){

            if(a->getModificavel() == false){
                naoModificaveis.push_back(a->getID());
            }
        }

    }

    sort(naoModificaveis.begin(), naoModificaveis.end());
    printf("arestas nao modificaveis: [");
    for(unsigned int i=0; i<naoModificaveis.size(); i+=2)
        printf(" %d, ", naoModificaveis.at(i));
    printf("] size = %d\n ", (int) naoModificaveis.size()/2);
    printf("%d\n", g->getN_naoModificaveis());
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

    int *ids = configuracaoInicial(), n = g->getNumeroArcos()/2 - (g->getNumeroNos()-1);
    abreChaves(g, ids, n);
}

int *configuracaoInicial(){
    int *ids;
    if(strcmp(arquivoEntrada,"ENTRADAS_MODIFICADAS/sist33barras_Yang.m")==0){

        ids = new int[5];

        //CONFIGURACAO INICIAL
        if( strcmp(configuracao, "inicial") == 0 ){
            ids[0] = 33; ids[1] = 34; ids[2] = 35; ids[3] = 36; ids[4] = 37;
        }
        //CONFIGURACAO DA LITERATURA
        if( strcmp(configuracao, "literatura1") == 0 ){
            ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 32; ids[4] = 37;
        }
        //ABORDAGEM ARSD
        if( strcmp(configuracao, "ARSD") == 0 ){
            ids[0] = 7; ids[1] = 9; ids[2] = 14; ids[3] = 32; ids[4] = 37;
        }

        return ids;
    }

    if( strcmp(arquivoEntrada, "ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m") == 0 ){

        ids = new int[5];

        //CONFIGURACAO INICIAL
        if( strcmp(configuracao, "inicial") == 0 ){
            ids[0] = 33; ids[1] = 34; ids[2] = 35; ids[3] = 36; ids[4] = 37;
        }
        //CONFIGURACAO DA LITERATURA
        if( strcmp(configuracao, "literatura1") == 0 ){
            ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 28; ids[4] = 36;
        }
        //ABORDAGEM ARSD
        if( strcmp(configuracao, "ARSD") == 0 ){
            ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 16; ids[4] = 28;
        }

        return ids;
    }

    if( strcmp(arquivoEntrada, "ENTRADAS_MODIFICADAS/SISTEMA119s2.m") == 0 ){

        ids = new int[15];
        //CONFIGURACAO INICIAL
        if( strcmp(configuracao, "inicial") == 0 ){
            ids[0] = 119; ids[1] = 120; ids[2] = 121; ids[3] = 122; ids[4] = 123;
            ids[5] = 124; ids[6] = 125; ids[7] = 126; ids[8] = 127; ids[9] = 128;
            ids[10] = 129; ids[11] = 130; ids[12] = 131; ids[13] = 132; ids[14] = 133;
        }
        //CONFIGURACAO DA LITERATURA
        if( strcmp(configuracao, "literatura1") == 0 ){
            ids[0] = 24; ids[1] = 27; ids[2] = 35; ids[3] = 40; ids[4] = 43;
            ids[5] = 52; ids[6] = 59; ids[7] = 72; ids[8] = 75; ids[9] = 96;
            ids[10] = 99; ids[11] = 110; ids[12] = 123; ids[13] = 130; ids[14] = 131;
        }
        //ABORDAGEM ARSD
        if( strcmp(configuracao, "ARSD") == 0 ){
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

        return ids;
    }
    if(strcmp(arquivoEntrada,"ENTRADAS_MODIFICADAS/SISTEMA83_TAIWAN.m")==0){

        ids = new int[13];

        //CONFIGURACAO INICIAL
        if( strcmp(configuracao, "inicial") == 0 ){
            ids[0] = 84; ids[1] = 85; ids[2] = 86; ids[3] = 87; ids[4] = 88;
            ids[5] = 89; ids[6] = 90; ids[7] = 91; ids[8] = 92; ids[9] = 93;
            ids[10] = 94; ids[11] = 95; ids[12] = 96;
        }
        //CONFIGURACAO DA LITERATURA
        if( strcmp(configuracao, "literatura1") == 0 ){
            ids[0] = 7; ids[1] = 13; ids[2] = 34; ids[3] = 39; ids[4] = 42;
            ids[5] = 55; ids[6] = 62; ids[7] = 72; ids[8] = 83; ids[9] = 86;
            ids[10] = 89; ids[11] = 90; ids[12] = 92;
        }

        return ids;
    }
    if(strcmp(arquivoEntrada,"ENTRADAS_MODIFICADAS/SISTEMA83_TAIWAN_modificado.m")==0){

        ids = new int[13];

        //CONFIGURACAO INICIAL
        if( strcmp(configuracao, "inicial") == 0 ){
            ids[0] = 84; ids[1] = 85; ids[2] = 86; ids[3] = 87; ids[4] = 88;
            ids[5] = 89; ids[6] = 90; ids[7] = 91; ids[8] = 92; ids[9] = 93;
            ids[10] = 94; ids[11] = 95; ids[12] = 96;
        }
        //CONFIGURACAO DA LITERATURA
        if( strcmp(configuracao, "literatura1") == 0 ){
            ids[0] = 7; ids[1] = 13; ids[2] = 33; ids[3] = 38; ids[4] = 42;
            ids[5] = 63; ids[6] = 72; ids[7] = 83; ids[8] = 84; ids[9] = 86;
            ids[10] = 89; ids[11] = 90; ids[12] = 92;
        }
        if( strcmp(configuracao, "literatura2") == 0 ){
            ids[0] = 7; ids[1] = 13; ids[2] = 34; ids[3] = 38; ids[4] = 42;
            ids[5] = 63; ids[6] = 72; ids[7] = 83; ids[8] = 84; ids[9] = 86;
            ids[10] = 89; ids[11] = 90; ids[12] = 92;
        }
        if( strcmp(configuracao, "ARSD") == 0 ){
            ids[0] = 7; ids[1] = 13; ids[2] = 34; ids[3] = 39; ids[4] = 42;
            ids[5] = 63; ids[6] = 72; ids[7] = 83; ids[8] = 84; ids[9] = 86;
            ids[10] = 89; ids[11] = 90; ids[12] = 92;
        }

        return ids;
    }

    ids = new int[1];
    ids[0] = 0;
    return ids;
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

void testeConfInicial(){
    char nome[] = arquivoEntrada;
    Grafo *g = new Grafo();

    g->leEntrada(nome);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();

    Individuo *ind = new Individuo(g->getNumeroArcos()/2 - g->getN_naoModificaveis());

    int n = g->getNumeroArcos()/2 - (g->getNumeroNos()-1);

    ind->geraPesosConfInicial(configuracaoInicial(), n, g);
    ind->calculaFuncaoObjetivo(g);

    printf("\n\n\npedaToral: %f\n\n", 100*1000*ind->getPerdaAtiva());
}
