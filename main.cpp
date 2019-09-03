#include <stdio.h>
#include "Graph_network.h"
#include "RKGA.h"
#include "time.h"
#include "OS_Individual.h"

#include <algorithm>
#include <math.h>
#include <string.h>
#include <ctime>

//bool ordenaCromossomoPorIdArco(Cromossomo *c1, Cromossomo *c2){ return c1->arco->getID() < c2->arco->getID(); }

#define arquivoEntrada "ENTRADAS_MODIFICADAS/sis33.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sis33modif.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sis119.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sis135.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sis215.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sis83.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sis83modif.m"

#define configuracao "inicial"
//#define configuracao "literatura1"
//#define configuracao "literatura2"
//#define configuracao "ARSD"

void abreChaves(Graph_network *g, int *ids, int n);

void defineConfiguracao(Graph_network *g, char *arqIn);

void testeDestrutor();

void testeArestasModificaveis();

void testeArestasModificaveis2();

void testeCopiaGrafo();

void testePopulacaoAleatoria();

void testeEntradas();

bool ordenacao(Graph_network *g1, Graph_network *g2);

void testeMemLeakRandomKeys();

int *configuracaoInicial(char *arqIn);

void testeRandomKeys(char *arqIn);

void testeConfInicial(char *arqIn);

void testeFuncaoObjetivoOtimizada();

void testeprs();

void testeprs2();

void testeprsEvolutivo(int prs, int tamPop, int numGeracoes, char *arqIn);

void testeFO_OS_INDIVIDUAL(char *arqIn);

void testeOSPR(char *arqIn);

void testeContrutorOS(char *arqIn);

void testeEvaluate(char *arqIn);

void testeConstructorOSRK(char *arqIn);

void testeEVP(char *arqIn, int tamPool, int max_it, float pctElite);

void testePATHRELINKING(char *arqIn, int m);

void testeAGgenerico(char *arqIn, int tamPop, int numGeracoes, int tipoCruzamento);

void testeConversoes(char *arqIn);

void testeAGPRCA(char *arqIn, int tamPop, int numGeracoes, int it_s_melhora, int tamPool, int max_it, float pct_elite);

void testeGERAL(char * arqIn, int tamPop, int numGeracoes, int it_s_melhora, int tamPool, int max_it, float pct_elite, int execucao, float taxaTamMut, float taxaPctMut);

void testeOperadoresCruzamentoAG(char *arqIn, int tamPop, int numGeracoes, int tipoCruzamento);

//################################################################

void testePRE(char *arquIn, int tam_pool, int max_it, float pct_pr_elite);
void testeAG_PRE(char *arqIn, int it_s_melhora, int tampop, int numgeracoes, int tam_pool, int max_it, float pct_pr_elite);
//Grafo *g, int tam_pool, int max_it, float pct_pool_elite

//################################################################

int semente = time(NULL);
int main(int c, char *argv[]) {
    srand(semente);
//    printf("\n\n\t\tsemente------>>> %d <<<-----\n\n", semente);

//    testeFO_OS_INDIVIDUAL("networks/sis33.m");
//    testeFO_OS_INDIVIDUAL("networks/sis33modif.m");
//    testeFO_OS_INDIVIDUAL("networks/83.m");
//    testeFO_OS_INDIVIDUAL("networks/sis83_modif.m ");
//    testeFO_OS_INDIVIDUAL("networks/sis119.m");
//    testeFO_OS_INDIVIDUAL("networks/sis135.m");
//    testeFO_OS_INDIVIDUAL("networks/sis215.m");

//    testeOSPR(argv[1]);
//    testeContrutorOS(argv[1]);
//    testeEvaluate(argv[1]);
//    testeConstructorOSRK(argv[1]);

//    char *arqIn = argv[1];
//    int tam_pool = strtol(argv[2], nullptr, 0);
//    int max_it = strtol(argv[3], nullptr, 0);
//    float pct_elite = atof(argv[4]);
//    testeEVP(arqIn, tam_pool, max_it, pct_elite);

//    int m = strtol(argv[2], nullptr, 0);
//    testePATHRELINKING(argv[1], m);

//    char *arqIn = argv[1];
//    int tamPop = strtol(argv[2], nullptr, 0);
//    int numGeracoes = strtol(argv[3], nullptr, 0);
//    int tipoCruzamento = strtol(argv[4], nullptr, 0);

//    testeAGgenerico(argv[1], tamPop, numGeracoes, tipoCruzamento);
//    testeConversoes(arqIn);


    char *arqIn = argv[1];
    int tamPop = strtol(argv[2], nullptr, 0);
    int numGeracoes = strtol(argv[3], nullptr, 0);
    int it_s_melhora = strtol(argv[4], nullptr, 0);
    int tamPool = strtol(argv[5], nullptr, 0);
    int max_it = strtol(argv[6], nullptr, 0);
    float pct_elite = atof(argv[7]);
    int execucao = strtol(argv[8], nullptr, 0);
    float taxaTamMut = atof(argv[9]);
    float taxaPctMut = atof(argv[10]);

    testeGERAL(arqIn, tamPop, numGeracoes, it_s_melhora, tamPool, max_it, pct_elite, execucao, taxaTamMut, taxaPctMut);
//    testeAGPRCA(arqIn, tamPop, numGeracoes, it_s_melhora, tamPool, max_it, pct_elite);
}

void testeRandomKeys(char arqIn[]) {
    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
//    unsigned int tam = Individuo::cromossomos.size();
//    cout << "tamanho vector<cromossomos> : " << tam << endl;
//    cout << "numero de arcos nao modificaveis: " << g->getN_naoModificaveis() << endl;

    /** numero de individuos da populacao,numero de geracaoes **/
    RKGA *rd = new RKGA(100, 1000);

    /** populacao inicial gerada de forma aleatoria **/
//    rd->geraPopAleatoria(g);
    rd->geraPopAleatoriaConfInicial(g, configuracaoInicial(arqIn), g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1));

    /** faz cruzamentos e mutacoes para gerar individuos da nova populacao **/
//    int melhorGeracao = rd->avancaGeracoes(g);
    int melhorGeracao = rd->avancaGeracoes2(g);


    /** melhor individuo eh o ultimo (menor perda) da populacao da ultima geracao **/
    RK_Individual *best = rd->getPopAtual().at(rd->getTamPopulacao() - 1);

//    /**Para imprimir as chaves abertas de forma ordenada, sem repeticao de arcos**/
//    vector<int> chavesAbertas;
//    printf("\n\n\nMELHOR INDIVIDUO FINAL:");
//    printf("\nAberto:{");
//    for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){
//        for(Arco *a = no->getListaArcos(); a!=NULL; a = a->getProxArco()){
//            Arco *volta = g->buscaArco(a->getNoDestino()->getID(), a->getNoOrigem()->getID());
//            if(a->getChave()==false && volta->getChave()==false)
//                chavesAbertas.push_back(a->getID());
//        }
//    }
//    sort(chavesAbertas.begin(), chavesAbertas.end());
//    for(unsigned int i=0; i<chavesAbertas.size();i+=2)
//        printf("%d,", chavesAbertas.at(i));
//    printf("  }\n");
//
//    printf("PerdaAtiva: %f (kw)\n", 100*1000*best->getPerdaAtiva());
//    printf("Tensao minima: %f (pu)\n\n\n", g->tensaoMinima());

    printf("        %.3f  &  ", 100 * 1000 * best->getPerdaAtiva());
    printf("        %.3f  &   ", g->tensaoMinima());
    printf("        %d    &   ", melhorGeracao);

//    /** so para conferir mesmo  e ter certeza da perda do individuo **/
//    g->zeraFluxosEPerdas();
//    g->calcula_fluxos_e_perdas(1e-8);
//    printf("(olhando pro grafo so pra ter certeza do calculo)\nPerdaAtiva: %f (kw)\n\n\n", 100*1000*g->soma_perdas()[0]);
}

/** pequeno vazamento de memoria na funcao "calculaFuncaoObjetivo" do individuo novamente no vector **/
void testeMemLeakRandomKeys() {
    char nome[] = arquivoEntrada;
    Graph_network *g = new Graph_network();

    g->leEntrada(nome);

    printf("\n num nos: %d         num arcos: %d", g->getNumeroNos(), g->getNumeroArcos());

    RK_Individual *i = new RK_Individual(g->getNumeroArcos());

    i->geraPesosAleatorios();
    while (true) {
        i->calculaFuncaoObjetivo(g);
        printf("\n\nperdaAtiv: %f kw,  perdaReativ: %f kw      eh arvore? %d",
               1000 * 100 * i->getPerdaAtiva(), 1000 * 100 * i->getPerdaReativa(), g->ehArvore());
    }
}

void testePopulacaoAleatoria() {
    char nome[] = arquivoEntrada;
    Graph_network *g = new Graph_network();

    g->leEntrada(nome);

    int n_individuos = 500;
    vector<Graph_network *> h;

    for (int i = 0; i < n_individuos; i++) {

        double *perda;
        printf("\n\nsolucao  %d:", i);
        h.push_back(g->retornaCopia());
        h.at(i)->solucaoAleatoria();
        h.at(i)->define_sentido_fluxos();
        h.at(i)->calcula_fluxos_e_perdas(1e-8);
        perda = h.at(i)->soma_perdas();

        printf("\nvalida? %d", h.at(i)->ehArvore());
        printf("  tensao minima: %.5f (p.u)", h.at(i)->tensaoMinima());
        printf("  perdaTotal: %.5f (kW)", 100 * 1000 * perda[0]);

        delete perda;

    }
}

void testeEntradas(char *arqIn) {
    char nome[] = arquivoEntrada;
    Graph_network *g = new Graph_network();
    g->leEntrada(nome);

    defineConfiguracao(g, arqIn);

    g->define_sentido_fluxos();
    g->calcula_fluxos_e_perdas(1e-8);


    printf("\ntensao minima: %.5f (p.u)", g->tensaoMinima());
    printf("\nperdaTotal: %.5f (kW)", 100 * 1000 * g->soma_perdas()[0]);
    printf("\neh Conexo? %d", g->ehConexo());
    printf("\neh arvore? %d\n\n\n", g->ehArvore());
}

void testeCopiaGrafo(char *arqIn) {
    char nome[] = arquivoEntrada;
    Graph_network *g, *h;
    g = new Graph_network();
    g->leEntrada(arqIn);

    defineConfiguracao(g, arqIn);
    g->define_sentido_fluxos();
    g->calcula_fluxos_e_perdas(1e-8);


    for (int i = 0; true; i++) {
        h = g->retornaCopia();
        h->calcula_fluxos_e_perdas(1e-8);

        double *perda = h->soma_perdas();
        printf("\ntensao minima: %.5f (p.u)", h->tensaoMinima());
        printf("\nperdaTotal: %.5f (kW)", 100 * 1000 * perda[0]);
        printf("\neh arvore? %d\n\n\n", h->ehArvore());
        delete perda;

        delete h;
    }
}

void testeArestasModificaveis() {
    char nome[] = arquivoEntrada;
    Graph_network *g;
    g = new Graph_network();
    g->leEntrada(nome);
    g->resetaGrausAuxiliares();
//    g->imprime();

    //TESTE ARESTAS MODIFICAVEIS PARA ARQUIVO DE 33 BARRAS

//    int ids[] = {36, 34, 2, 25, 33};
    int ids[] = {32, 10, 20, 4, 12};

    for (int i = 0; i < 2; i++) {

        printf("\nabriu A{%d}", ids[i]);

        Edge *a = g->buscaArco(ids[i]);

        Vertex *noOrigem = a->getNoOrigem();
        Vertex *noDestino = a->getNoDestino();

        a->setChave(false);
        a->setModificavel(false);
        a = g->buscaArco(noDestino->getID(), noOrigem->getID());
        a->setChave(false);
        a->setModificavel(false);

        printf("modif = %d", a->getModificavel());

        noOrigem->setGrauAux(noOrigem->getGrauAux() - 1);
        noDestino->setGrauAux(noDestino->getGrauAux() - 1);

        g->defineArestasModificaveis();

    }
    g->imprime();

}

void testeArestasModificaveis2() {
    char nome[] = arquivoEntrada;
    Graph_network *g;
    g = new Graph_network();
    g->leEntrada(nome);
    g->resetaGrausAuxiliares();

    g->defineArestasModificaveis();

    vector<int> naoModificaveis;
    for (Vertex *no = g->getListaNos(); no != NULL; no = no->getProxNo()) {

        for (Edge *a = no->getListaArcos(); a != NULL; a = a->getProxArco()) {

            if (a->getModificavel() == false) {
                naoModificaveis.push_back(a->getID());
            }
        }

    }

    sort(naoModificaveis.begin(), naoModificaveis.end());
    printf("arestas nao modificaveis: [");
    for (unsigned int i = 0; i < naoModificaveis.size(); i += 2)
        printf(" %d, ", naoModificaveis.at(i));
    printf("] size = %d\n ", (int) naoModificaveis.size() / 2);
    printf("%d\n", g->getN_naoModificaveis());
}

void testeDestrutor() {
    char nome[] = arquivoEntrada;

    while (true) {
        Graph_network *g;
        g = new Graph_network();
        g->leEntrada(nome);
        delete g;
    }
}

void defineConfiguracao(Graph_network *g, char *arqIn) {

    int *ids = configuracaoInicial(arqIn), n = g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1);
    abreChaves(g, ids, n);
}

int *configuracaoInicial(char *arqIn) {
    int *ids;
    if (strcmp(arqIn, "networks/sis33.m") == 0) {

        ids = new int[5];

        //CONFIGURACAO INICIAL
        if (strcmp(configuracao, "inicial") == 0) {
            ids[0] = 33;
            ids[1] = 34;
            ids[2] = 35;
            ids[3] = 36;
            ids[4] = 37;
        }
        //CONFIGURACAO DA LITERATURA
        if (strcmp(configuracao, "literatura1") == 0) {
            ids[0] = 7;
            ids[1] = 10;
            ids[2] = 14;
            ids[3] = 32;
            ids[4] = 37;
        }
        //ABORDAGEM ARSD
        if (strcmp(configuracao, "ARSD") == 0) {
            ids[0] = 7;
            ids[1] = 9;
            ids[2] = 14;
            ids[3] = 32;
            ids[4] = 37;
        }

        return ids;
    }

    if (strcmp(arqIn, "networks/sis33modif.m") == 0) {

        ids = new int[5];

        //CONFIGURACAO INICIAL
        if (strcmp(configuracao, "inicial") == 0) {
            ids[0] = 33;
            ids[1] = 34;
            ids[2] = 35;
            ids[3] = 36;
            ids[4] = 37;
        }
        //CONFIGURACAO DA LITERATURA
        if (strcmp(configuracao, "literatura1") == 0) {
            ids[0] = 7;
            ids[1] = 10;
            ids[2] = 14;
            ids[3] = 28;
            ids[4] = 36;
        }
        //ABORDAGEM ARSD
        if (strcmp(configuracao, "ARSD") == 0) {
            ids[0] = 7;
            ids[1] = 10;
            ids[2] = 14;
            ids[3] = 16;
            ids[4] = 28;
        }

        return ids;
    }

    if (strcmp(arqIn, "networks/sis119.m") == 0) {

        ids = new int[15];
        //CONFIGURACAO INICIAL
        if (strcmp(configuracao, "inicial") == 0) {
            ids[0] = 119;
            ids[1] = 120;
            ids[2] = 121;
            ids[3] = 122;
            ids[4] = 123;
            ids[5] = 124;
            ids[6] = 125;
            ids[7] = 126;
            ids[8] = 127;
            ids[9] = 128;
            ids[10] = 129;
            ids[11] = 130;
            ids[12] = 131;
            ids[13] = 132;
            ids[14] = 133;
        }
        //CONFIGURACAO DA LITERATURA
        if (strcmp(configuracao, "literatura1") == 0) {
            ids[0] = 24;
            ids[1] = 27;
            ids[2] = 35;
            ids[3] = 40;
            ids[4] = 43;
            ids[5] = 52;
            ids[6] = 59;
            ids[7] = 72;
            ids[8] = 75;
            ids[9] = 96;
            ids[10] = 99;
            ids[11] = 110;
            ids[12] = 123;
            ids[13] = 130;
            ids[14] = 131;
        }
        //ABORDAGEM ARSD
        if (strcmp(configuracao, "ARSD") == 0) {
            ids[0] = 24;
            ids[1] = 26;
            ids[2] = 35;
            ids[3] = 40;
            ids[4] = 43;
            ids[5] = 51;
            ids[6] = 61;
            ids[7] = 72;
            ids[8] = 75;
            ids[9] = 96;
            ids[10] = 98;
            ids[11] = 110;
            ids[12] = 122;
            ids[13] = 130;
            ids[14] = 131;
        }

        /*
        esse arquivo tem na verdade 132 arcos ao inves de 133
        dessa forma os ids dos arcos abertos deve ser subtraido 1.
        */
        for (int i = 0; i < 15; i++)
            ids[i]--;

        return ids;
    }
    if (strcmp(arqIn, "networks/sis83.m") == 0) {

        ids = new int[13];

        //CONFIGURACAO INICIAL
        if (strcmp(configuracao, "inicial") == 0) {
            ids[0] = 84;
            ids[1] = 85;
            ids[2] = 86;
            ids[3] = 87;
            ids[4] = 88;
            ids[5] = 89;
            ids[6] = 90;
            ids[7] = 91;
            ids[8] = 92;
            ids[9] = 93;
            ids[10] = 94;
            ids[11] = 95;
            ids[12] = 96;
        }
        //CONFIGURACAO DA LITERATURA
        if (strcmp(configuracao, "literatura1") == 0) {
            ids[0] = 7;
            ids[1] = 13;
            ids[2] = 34;
            ids[3] = 39;
            ids[4] = 42;
            ids[5] = 55;
            ids[6] = 62;
            ids[7] = 72;
            ids[8] = 83;
            ids[9] = 86;
            ids[10] = 89;
            ids[11] = 90;
            ids[12] = 92;
        }

        return ids;
    }
    if (strcmp(arqIn, "networks/sis83modif.m") == 0) {

        ids = new int[13];

        //CONFIGURACAO INICIAL
        if (strcmp(configuracao, "inicial") == 0) {
            ids[0] = 84;
            ids[1] = 85;
            ids[2] = 86;
            ids[3] = 87;
            ids[4] = 88;
            ids[5] = 89;
            ids[6] = 90;
            ids[7] = 91;
            ids[8] = 92;
            ids[9] = 93;
            ids[10] = 94;
            ids[11] = 95;
            ids[12] = 96;
        }
        //CONFIGURACAO DA LITERATURA
        if (strcmp(configuracao, "literatura1") == 0) {
            ids[0] = 7;
            ids[1] = 13;
            ids[2] = 33;
            ids[3] = 38;
            ids[4] = 42;
            ids[5] = 63;
            ids[6] = 72;
            ids[7] = 83;
            ids[8] = 84;
            ids[9] = 86;
            ids[10] = 89;
            ids[11] = 90;
            ids[12] = 92;
        }
        if (strcmp(configuracao, "literatura2") == 0) {
            ids[0] = 7;
            ids[1] = 13;
            ids[2] = 34;
            ids[3] = 38;
            ids[4] = 42;
            ids[5] = 63;
            ids[6] = 72;
            ids[7] = 83;
            ids[8] = 84;
            ids[9] = 86;
            ids[10] = 89;
            ids[11] = 90;
            ids[12] = 92;
        }
        if (strcmp(configuracao, "ARSD") == 0) {
            ids[0] = 7;
            ids[1] = 13;
            ids[2] = 34;
            ids[3] = 39;
            ids[4] = 42;
            ids[5] = 63;
            ids[6] = 72;
            ids[7] = 83;
            ids[8] = 84;
            ids[9] = 86;
            ids[10] = 89;
            ids[11] = 90;
            ids[12] = 92;
        }

        return ids;
    }

    ids = new int[1];
    ids[0] = 0;
    return ids;
}

void abreChaves(Graph_network *g, int *ids, int n) {
    Edge *a;
    for (int i = 0; i < n; i++) {
        a = g->buscaArco(ids[i]);
        a->setChave(false);
        a = g->buscaArco(a->getNoDestino()->getID(), a->getNoOrigem()->getID());
        a->setChave(false);
    }
}

void testeConfInicial(char *arqIn) {
    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    RK_Individual *ind = new RK_Individual(g->getNumeroArcos() / 2 - g->getN_naoModificaveis());

    int nAbertos = g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1);

    int *abertos = configuracaoInicial(arqIn);
    ind->geraPesosConfInicial(abertos, nAbertos, g);
    ind->calculaFuncaoObjetivoOtimizado(g);

    printf("PerdaAtiva: %f (kw)\n", 100 * 1000 * ind->getPerdaAtiva());
    printf("Tensao minima: %f (pu)\n\n\n", g->tensaoMinima());
}

void testeFuncaoObjetivoOtimizada() {
    char nome[] = arquivoEntrada;
    Graph_network *g = new Graph_network();

    g->leEntrada(nome);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);

    int num = 10;

    RK_Individual *ind = new RK_Individual(g->getNumeroArcos() / 2 - g->getN_naoModificaveis());

    for (int i = 0; i < 10000; i++) {
        ind->geraPesosAleatorios();
        ind->calculaFuncaoObjetivoOtimizado(g);
        printf("perda: %lf\n", 100 * 1000 * ind->getPerdaAtiva());
    }
}

void testeprs() {
    char nome[] = arquivoEntrada;
    Graph_network *g = new Graph_network();

    g->leEntrada(nome);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);

    RK_Individual *i1 = new RK_Individual(g->getNumeroArcos() / 2 - g->getN_naoModificaveis());
    RK_Individual *i2 = new RK_Individual(g->getNumeroArcos() / 2 - g->getN_naoModificaveis());
    RK_Individual *indRef;


    i1->geraPesosAleatorios();
    i2->geraPesosAleatorios();

    i1->calculaFuncaoObjetivo(g);
    i2->calculaFuncaoObjetivo(g);

    if (i1->getPerdaAtiva() > i2->getPerdaAtiva())
        indRef = i1;
    else
        indRef = i2;

    printf("i1->perdaAtiva: %lf\n", 100 * 1000 * i1->getPerdaAtiva());
    printf("i2->perdaAtiva: %lf\n\n\n", 100 * 1000 * i2->getPerdaAtiva());

    Evolutionary_path_relinking *p = new Evolutionary_path_relinking();
//    RK_Individual *path = i1->prs(i2, g, indRef);
    RK_Individual *path = p->prs(i1, i2, g, indRef);
    delete p;

    printf("\n\npath->perdaAtiva: %lf\n\n\n", 100 * 1000 * path->getPerdaAtiva());
}

void testeprsEvolutivo(int prs, int tamPop, int numGeracoes, char *arqIn) {

    clock_t inicio, fim;
    RK_Individual *best;

    {
        inicio = clock();

        char *nome = arqIn;
        Graph_network *g = new Graph_network();

        g->leEntrada(nome);
        g->defineArestasModificaveis();
        g->resetaArcosMarcados();
        g->marcaUmsentidoArcos();
        RK_Individual::criaCromossomos(g);

        /** numero de individuos da populacao,numero de geracaoes **/
        RKGA *rd = new RKGA(tamPop, numGeracoes);

        /** populacao inicial gerada de forma aleatoria **/
        rd->geraPopAleatoriaConfInicial(g, configuracaoInicial(nome),
                                        g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1));

        /** faz cruzamentos e mutacoes para gerar individuos da nova populacao **/

        int melhorGeracao;
        if (prs == 0)
            melhorGeracao = rd->avancaGeracoes2(g);
        if (prs == 1)
            melhorGeracao = rd->avancaGeracoesPRS(
                    g);//path relinking simples a cada geracao utilizando 2 individuos aleatorios entre 10% dos melhores
        if (prs == 2)
            melhorGeracao = rd->avancaGeracoesPRSEvolutivoFinal(g);//path relinking evolutivo no final da geracao

        /** melhor individuo eh o ultimo (menor perda) da populacao da ultima geracao **/
        RK_Individual *best = rd->getPopAtual().at(rd->getTamPopulacao() - 1);

        best = rd->getPopAtual().at(rd->getTamPopulacao() - 1);
        best->calculaFuncaoObjetivoOtimizado(g);

        fim = clock();

        printf("        %.2lf &  ", (float) (fim - inicio) / CLOCKS_PER_SEC);
        printf("        %.3f  &  ", 100 * 1000 * best->getPerdaAtiva());
        printf("        %.3f  &  ", g->tensaoMinima());
        printf("        %d\n\n\n", melhorGeracao);
    }
}

void testeprs2(){

    char nome[] = arquivoEntrada;
    Graph_network *g = new Graph_network();

    g->leEntrada(nome);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);

    RK_Individual *i1 = new RK_Individual(g->getNumeroArcos() / 2 - g->getN_naoModificaveis());
    RK_Individual *i2 = new RK_Individual(g->getNumeroArcos() / 2 - g->getN_naoModificaveis());

    i1->geraPesosAleatorios();
    i2->geraPesosAleatorios();

    i1->calculaFuncaoObjetivoOtimizado(g);
    i2->calculaFuncaoObjetivoOtimizado(g);

    RK_Individual *result;
    while(true){
        clock_t inicio = clock();
//        result = i1->prs2(i2, g);
        Evolutionary_path_relinking *p = new Evolutionary_path_relinking();
        result = p->prs2(i1, i2, g);
        clock_t fim = clock();
        printf("tempo de execucao do path-relinking: %f\n", (double)(fim-inicio)/CLOCKS_PER_SEC);
        delete result;
    }
}

void testePRE(char *arqIn, int tam_pool, int max_it, float pct_pr_elite){

    Graph_network *g = new Graph_network();

    //inicializacoes
    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    //fim inicializacoes

//    printf("\n\ntamanho do individuo (arestas modificaveis): %d\n\n", g->getNumeroArcos() / 2 - g->getN_naoModificaveis());

    RKGA *rd = new RKGA(100, 1);

    RK_Individual *inicial = new RK_Individual(g->getNumeroArcos() / 2 - g->getN_naoModificaveis());
    inicial->geraPesosConfInicial(configuracaoInicial(arqIn),  g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1), g);
    inicial->calculaFuncaoObjetivoOtimizado(g);
//    printf("i 0:   %f\n", 100*1000*inicial->getPerdaAtiva());

    vector<RK_Individual*> pool;
    pool.push_back(inicial);

    RK_Individual *ind;
    for(int i=1; i<tam_pool; i++){
        ind = new RK_Individual(g->getNumeroArcos() / 2 - g->getN_naoModificaveis());
        ind->geraPesosAleatorios();
        ind->calculaFuncaoObjetivoOtimizado(g);
        //populacao inicial de individuos aleatorios validos
        while(ind->getPerdaAtiva()==9999999999){
            ind->geraPesosAleatorios();
            ind->calculaFuncaoObjetivoOtimizado(g);
        }
//        printf("i %d:   %f\n", i, 100*1000*ind->getPerdaAtiva());
        pool.push_back(ind);
    }

    Evolutionary_path_relinking p(pool, max_it, pct_pr_elite);

    clock_t inicio = clock();
    RK_Individual *best = p.pre(g);
    clock_t fim = clock();
    printf("%f %f %d\n", 100*1000*best->getPerdaAtiva(), (double)(fim-inicio)/CLOCKS_PER_SEC, semente);
}

void testeAG_PRE(char *arqIn, int it_s_melhora, int tampop, int numgeracoes, int tam_pool, int max_it, float pct_pr_elite){

    Graph_network *g = new Graph_network();

    //inicializacoes
    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    //fim inicializacoes


    RKGA *rd = new RKGA(tampop, numgeracoes);

    /** populacao inicial gerada de forma aleatoria **/
    rd->geraPopAleatoriaConfInicial(g, configuracaoInicial(arqIn), g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1));

    /** faz cruzamentos e mutacoes para gerar individuos da nova populacao **/
    int melhorGeracao = rd->avancaGeracoesPRE(g, it_s_melhora, tam_pool, max_it, pct_pr_elite);

    /** melhor individuo eh o ultimo (menor perda) da populacao da ultima geracao **/
    RK_Individual *best = rd->getPopAtual().at(rd->getTamPopulacao() - 1);


    printf(" %.3f ", 100 * 1000 * best->getPerdaAtiva());
    printf("%.3f ", g->tensaoMinima());
    printf("%d ", melhorGeracao);
}

void testeFO_OS_INDIVIDUAL(char *arqIn) {

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    RK_Individual *ind = new RK_Individual(tam);

    int nAbertos = g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1);

    int *abertos = configuracaoInicial(arqIn);
    ind->geraPesosConfInicial(abertos, nAbertos, g);
    ind->calculaFuncaoObjetivoOtimizado(g);

    OS_Individual *os = new OS_Individual(ind, g);

    printf("%f\n%f", 100 * 1000 * ind->getPerdaAtiva(), 100 * 1000 * os->getActiveLoss());


//    for(unsigned long int i = 0; i<os->getOpenedSwitches().size(); i++)
//        printf("\no2[%ld]%d\n", i, os->getOpenedSwitches().at(i)->getID());
//
//
//    g = new Graph_network();
//
//    g->leEntrada(arqIn);
//    g->defineArestasModificaveis();
//    g->resetaArcosMarcados();
//    g->marcaUmsentidoArcos();
//    RK_Individual::criaCromossomos(g);
//
//
//    RK_Individual *ind2 = new RK_Individual(tam);
//    OS_Individual *o2 = new OS_Individual();
//    for (Vertex *no = g->getListaNos(); no != NULL; no = no->getProxNo()) {
//
//        for (Edge *a = no->getListaArcos(); a != NULL; a = a->getProxArco()) {
//
//            if (a->getModificavel() == true && a->getMarcado() == true) {
//
//                for (int i = 0; i < (g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1)); i++) {
////                    printf("abertos[%d]: %d\n", i, abertos[i]);
//                    if (a->getID() == abertos[i]) {
////                        printf("deu push\n");
////                        printf("\nabertos[%d]: %d\n", i, abertos[i]);
//                        o2->getOpenedSwitches().push_back(a);
//                    }
//                }
//            }
//
//        }
//    }
//
////    printf("tamanho de o2: %d\n", o2->getOpenedSwitches().size());
//
////    for(unsigned long int i = 0; i<o2->getOpenedSwitches().size(); i++)
////        printf("\no2[%d]%d\n", i, o2->getOpenedSwitches().at(i)->getID());
//
//    o2->calcObjectiveF(g);
//    printf("\n\n%f\n%f", 100 * 1000 * ind->getPerdaAtiva(), 100 * 1000 * o2->getActiveLoss());
//    printf("\n\n%d", g->getNumeroArcos() / 2);
}

void testeOSPR(char *arqIn) {
    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    RK_Individual *ind = new RK_Individual(tam);

    int nAbertos = g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1);

    int *abertos = configuracaoInicial(arqIn);
    ind->geraPesosConfInicial(abertos, nAbertos, g);
    ind->calculaFuncaoObjetivoOtimizado(g);

    OS_Individual *os = new OS_Individual(ind, g);

    printf("\n\nos:");
    for(unsigned long int i = 0; i<os->getOpenedSwitches().size(); i++)
        printf("\nos[%ld]%d\n", i, os->getOpenedSwitches().at(i)->getID());

    ind->geraPesosAleatorios();
    ind->calculaFuncaoObjetivoOtimizado(g);
    while(ind->getPerdaAtiva()==9999999999){
        ind->geraPesosAleatorios();
        ind->calculaFuncaoObjetivoOtimizado(g);
    }

    OS_Individual *os2 = new OS_Individual(ind, g);

    printf("\n\nos2:");
    for(unsigned long int i = 0; i<os2->getOpenedSwitches().size(); i++)
        printf("\nos2[%ld]%d\n", i, os2->getOpenedSwitches().at(i)->getID());


    Evolutionary_path_relinking *epr = new Evolutionary_path_relinking();

//    OS_Individual *result = epr->path_relingking(os, os2, g);
    vector<int> v = os->evaluate(os2->getOpenedSwitches().at(0), g);

    printf("\n\nevaluate result:\n");
    for(unsigned long int i=0; i<v.size(); i++)
        printf("%d\n", v.at(i));
}

void testeContrutorOS(char *arqIn) {
    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    RK_Individual *rk = new RK_Individual(tam);

    double valor = 139.55;//melhor solucao literatura

    int n;
    cin>>n;
    for (int j = 0; j < n; ++j) {
        rk->geraPesosAleatorios();
        OS_Individual *os = new OS_Individual(rk, g);
        os->calcObjectiveF(g);
        if(os->getSize()!=(g->getNumeroArcos()/2) - (g->getNumeroNos()-1) || 100*1000*os->getActiveLoss()<valor)
            printf("falha! perda: %f\n", os->getActiveLoss());
//        cout << 100*1000*os->getActiveLoss() << endl;
//        printf("\n\n\nsize: %d\tperda:%.6f\t", 100*1000*os->getActiveLoss(), os->getSize());
//        for(unsigned long int i = 0; i<os->getOpenedSwitches().size(); i++) {
//            printf("os[%ld]:%d\t", i, os->getOpenedSwitches().at(i)->getID());
//        }
    }
}

void testeEvaluate(char *arqIn) {

    ///-------------initialization-------------

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    ///-------------initialization-------------

    RK_Individual *rk = new RK_Individual(tam);

    OS_Individual *os1, *os2, *pr;

    Evolutionary_path_relinking *evp = new Evolutionary_path_relinking();


    rk->geraPesosAleatorios();
    os1 = new OS_Individual(rk, g);

    rk->geraPesosAleatorios();
    os2 = new OS_Individual(rk, g);

    printf("\n\nos1\tactiveLoss: %.2f\t", 100*1000*os1->getActiveLoss());
    os1->imprime();

    printf("\nos2\tactiveLoss: %.2f\t", 100*1000*os2->getActiveLoss());
    os2->imprime();

    pr = evp->path_relingking(os1, os2, g);

    printf("\npr\tactiveLoss: %.2f\t", 100*1000*pr->getActiveLoss());
    pr->imprime();





//    RK_Individual *ind = new RK_Individual(tam);
//
//    ind->geraPesosAleatorios();
//    OS_Individual *os = new OS_Individual(ind, g);
//
//    ind->geraPesosAleatorios();
//    OS_Individual *os2 = new OS_Individual(ind, g);
//
//    os->setOpenSwitches(0, os2->getOpenedSwitches().at(0));
//    os->setOpenSwitches(2, os2->getOpenedSwitches().at(1));
//    os->calcObjectiveF(g);
//
//    int in = 2;
//    vector<int> v = os->evaluate(os2->getOpenedSwitches().at(in), g);
//
//    printf("\n\nos\tactiveLoss: %.2f\t", 100*1000*os->getActiveLoss());
//    os->imprime();
////    for(unsigned long int i = 0; i<os->getOpenedSwitches().size(); i++)
////        printf("\nos[%ld]%d\n", i, os->getOpenedSwitches().at(i)->getID());
//
//    printf("\n\nos2\tactiveLoss: %.2f\t", 100*1000*os2->getActiveLoss());
//    os2->imprime();
////    for(unsigned long int i = 0; i<os2->getOpenedSwitches().size(); i++)
////        printf("\nos2[%ld]%d\n", i, os2->getOpenedSwitches().at(i)->getID());
//
//
//    printf("\n\ncandidatos para sair de \"os\" ao inserir os2[%d]=%d:\t", in, os2->getOpenedSwitches().at(in)->getID());
//    for(unsigned long int i=0; i<v.size(); i++)
//        printf("%d\t", os->getOpenedSwitches().at(v.at(i))->getID());
//
////    printf("\n\noss:\n");
////    OS_Individual **oss = new OS_Individual*[v.size()];
////    for(unsigned long int i=0;i<v.size();i++){
////        oss[i] = new OS_Individual(os);
////        oss[i]->getOpenedSwitches().at(v.at(i)) = os2->getOpenedSwitches().at(0);
////        oss[i]->calcObjectiveF(g);
////        printf("\n\noss[%ld]\tactiveLoss: %.2f\t", i, 100*1000*oss[i]->getActiveLoss());
////        oss[i]->imprime();
////    }
//
//
//    printf("\n check \"os\": %d\t check \"os2\": %d", os->checkSolution(g), os2->checkSolution(g));
//
//    Evolutionary_path_relinking *evp = new Evolutionary_path_relinking();
//
//    OS_Individual *pr = evp->path_relingking(os, os2, g);
//
//    printf("\n\npr->activeLoss: %.2f\t", 100*1000*pr->getActiveLoss());
//    pr->imprime();
//
////    ofstream saida;
////    saida.open("saida.txt");
//
////    OS_Individual *i1, *i2;
////    int m = 10;
////    for (int j = 0; j < m; ++j) {
////
//////        printf("MINHA PIKA EH BEM GROSSINHA LALALA FINURA DO SEU CU LALALA!!!!\n");
////
////        ind->geraPesosAleatorios();
////        i1 = new OS_Individual(ind, g);
////
////        ind->geraPesosAleatorios();
////        i2 = new OS_Individual(ind, g);
////
////        pr = evp->path_relingking(i1, i2, g);
////
////        cout << "\n\n" << 100*1000*os->getActiveLoss() << "\t" << 100*1000*os2->getActiveLoss() << "\t" << 100*1000*pr->getActiveLoss() << "\n";
////    }
////    saida.close();

}

void testeConstructorOSRK(char *arqIn) {
    ///-------------initialization-------------

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    ///-------------initialization-------------

    RK_Individual *rk;
    OS_Individual *os;

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {

        rk = new RK_Individual(tam);
        rk->geraPesosAleatorios();

        os = new OS_Individual(rk, g);

        printf("%.2f\n", 100*1000*os->getActiveLoss());

        delete rk;
        delete os;
    }
}

void testeEVP(char *arqIn, int tamPool, int max_it, float pctElite) {
    clock_t inicio = clock();

    ///-------------initialization-------------

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    ///-------------initialization-------------




    ///-------------creating pool of RK individuals----------

    RK_Individual **ind = new RK_Individual*[tamPool];
    vector<RK_Individual*> pool_RK;
    for(int i=1; i<tamPool; i++){
        ind[i] = new RK_Individual(tam);
        ind[i]->geraPesosAleatorios();
        pool_RK.push_back(ind[i]);
    }

    //initial configuration individual in pool
    int nAbertos = g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1);
    int *abertos = configuracaoInicial(arqIn);
    ind[0] = new RK_Individual(tam);
    ind[0]->geraPesosConfInicial(abertos, nAbertos, g);
    pool_RK.push_back(ind[0]);

    ///-------------creating pool of RK individuals----------

    Evolutionary_path_relinking *evp = new Evolutionary_path_relinking();
    OS_Individual *best = evp->run(pool_RK, max_it, pctElite, g);

    clock_t fim = clock();

    printf("%f %f %d\n", 100*1000*best->getActiveLoss(), (double)(fim-inicio)/CLOCKS_PER_SEC, semente);
}

void testePATHRELINKING(char *arqIn, int m) {
    ///-------------initialization-------------

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    ///-------------initialization-------------

//    int m;
//    cin >> m;

    RK_Individual*rk = new RK_Individual(tam);
    OS_Individual *o1, *o2, *pr;
    Evolutionary_path_relinking *evp = new Evolutionary_path_relinking();
    for (int i = 0; i < m; ++i) {

        rk->geraPesosAleatorios();
        o1 = new OS_Individual(rk, g);

        rk->geraPesosAleatorios();
        o2 = new OS_Individual(rk, g);

//        printf("\n----------------------0---------------------------");
        pr = evp->path_relingking(o1, o2, g);
//        printf("o1: %.2f\to2: %.2f\tpr: %.2f\n",
//                o1->getActiveLoss()*100*1000,
//                o2->getActiveLoss()*100*1000,
//                pr->getActiveLoss()*100*1000);
//        printf("----------------------x---------------------------\n\n");


        delete o1, o2, pr;
    }
}

void testeAGgenerico(char *arqIn, int tamPop, int numGeracoes, int tipoCruzamento) {
    clock_t inicio = clock();

    ///-------------initialization-------------

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    ///-------------initialization-------------

    RKGA *ga = new RKGA(tamPop, numGeracoes);

    int *abertos = configuracaoInicial(arqIn);
    ga->geraPopAleatoriaConfInicial(g, abertos, g->getNumeroArcos()/2 - (g->getNumeroNos() -1));

    ga->avancaGeracoesGenerico(g, tipoCruzamento);

    OS_Individual *os = new OS_Individual(ga->getPopAtual().at(ga->getPopAtual().size()-1), g);
//    os->imprime();


    clock_t fim = clock();


    printf("%f %f %d\n", 100*1000*ga->getPopAtual().at(ga->getPopAtual().size()-1)->getPerdaAtiva(), (double)(fim-inicio)/CLOCKS_PER_SEC, semente);
}

void testeConversoes(char *arqIn) {
    ///-------------initialization-------------

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    ///-------------initialization-------------

    RK_Individual *rk = new RK_Individual(tam);
    rk->geraPesosAleatorios();
    rk->calculaFuncaoObjetivoOtimizado(g);

    OS_Individual *os = new OS_Individual(rk, g);

    RK_Individual *rk2 = os->OS_to_RK(g);

    printf("%f\t%f\t%f\n", rk->getPerdaAtiva(), os->getActiveLoss(), rk2->getPerdaAtiva());
}

void testeAGPRCA(char *arqIn, int tamPop, int numGeracoes, int it_s_melhora, int tamPool, int max_it, float pct_elite) {
    clock_t inicio = clock();

    ///-------------initialization-------------

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    ///-------------initialization-------------

    RKGA *ga = new RKGA(tamPop, numGeracoes);

    int *abertos = configuracaoInicial(arqIn);
    ga->geraPopAleatoriaConfInicial(g, abertos, g->getNumeroArcos()/2 - (g->getNumeroNos() -1));

    int geracao = ga->avancaGeracoesPRECA(g, it_s_melhora, tamPool, max_it, pct_elite);
//    ga->avancaGeracoes2(g);

    OS_Individual *os = new OS_Individual(ga->getPopAtual().at(ga->getPopAtual().size()-1), g);
//    os->imprime();


    clock_t fim = clock();


    printf("%f %f %d %d\n", 100*1000*ga->getPopAtual().at(ga->getPopAtual().size()-1)->getPerdaAtiva(), (double)(fim-inicio)/CLOCKS_PER_SEC, geracao, semente);
}

void testeGERAL(char *arqIn, int tamPop, int numGeracoes, int it_s_melhora, int tamPool, int max_it, float pct_elite, int execucao, float taxaTamMut, float taxaPctMut) {

    ///-------------initialization-------------

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    ///-------------initialization-------------

    ///-------------creating pool of RK individuals----------

    RK_Individual **ind = new RK_Individual*[tamPool];
    vector<RK_Individual*> pool_RK;
    for(int i=1; i<tamPool; i++){
        ind[i] = new RK_Individual(tam);
        ind[i]->geraPesosAleatorios();
        ind[i]->calculaFuncaoObjetivoOtimizado(g);
        pool_RK.push_back(ind[i]);
    }

    //initial configuration individual in pool
    int nAbertos = g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1);
    int *abertos = configuracaoInicial(arqIn);
    ind[0] = new RK_Individual(tam);
    ind[0]->geraPesosConfInicial(abertos, nAbertos, g);
    ind[0]->calculaFuncaoObjetivoOtimizado(g);
    pool_RK.push_back(ind[0]);

    ///-------------creating pool of RK individuals----------

    //PRE_RK
    if(execucao==1){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        RK_Individual *best = rkga->pre(pool_RK, max_it, pct_elite, g);
        clock_t fim = clock();
        printf("- %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, 0, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //PRE_OS
    if(execucao==2){
        Evolutionary_path_relinking *evpos = new Evolutionary_path_relinking(pool_RK, max_it, pct_elite);
        clock_t inicio = clock();
        OS_Individual *best = evpos->run(pool_RK, max_it, pct_elite, g);
        clock_t fim = clock();
        printf("- %f %f %d %d ", 100*1000*best->getActiveLoss(), (double)(fim - inicio)/CLOCKS_PER_SEC, 0, semente);
        best->imprime();
        printf("\n");
    }
    //AG
    if(execucao==3){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracoes2(g);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf("- %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG_RK
    if(execucao==4){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracoesPRE(g, it_s_melhora, tamPool, max_it, pct_elite);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG_OS
    if(execucao==5){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracoesPRECA(g, it_s_melhora, tamPool, max_it, pct_elite);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG_AD
    if(execucao==6){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracaoesAdaptativo(g, it_s_melhora, 0.5, 1.0, taxaTamMut, taxaPctMut);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG_AD 2
    if(execucao==7){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracaoesAdaptativo2(g, it_s_melhora, 0.5, 1.0, taxaTamMut, taxaPctMut);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG Operador cruzamento media
    if(execucao==8){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracoesGenerico(g, 0);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG Operador cruzamento media2
    if(execucao==9){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracoesGenerico(g, 1);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG Operador cruzamento partes
    if(execucao==10){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracoesGenerico(g, 2);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG Operador cruzamento partes 2
    if(execucao==11){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracoesGenerico(g, 3);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG Operador cruzamento partes 3
    if(execucao==12){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracoesGenerico(g, 4);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
    //AG Operador cruzamento mistura
    if(execucao==13){
        RKGA *rkga = new RKGA(tamPop, numGeracoes);
        clock_t inicio = clock();
        rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
        int geracao = rkga->avancaGeracoesGenerico(g, 5);
        clock_t fim = clock();
        RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
        printf(" %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
        OS_Individual *os = new OS_Individual(best, g);
        os->imprime();
        printf("\n");
    }
}

void testeOperadoresCruzamentoAG(char *arqIn, int tamPop, int numGeracoes, int tipoCruzamento) {
    ///-------------initialization-------------

    Graph_network *g = new Graph_network();

    g->leEntrada(arqIn);
    g->defineArestasModificaveis();
    g->resetaArcosMarcados();
    g->marcaUmsentidoArcos();
    RK_Individual::criaCromossomos(g);
    unsigned int tam = RK_Individual::cromossomos.size();

    ///-------------initialization-------------

    int nAbertos = g->getNumeroArcos() / 2 - (g->getNumeroNos() - 1);
    int *abertos = configuracaoInicial(arqIn);

    RKGA *rkga = new RKGA(tamPop, numGeracoes);
    clock_t inicio = clock();
    rkga->geraPopAleatoriaConfInicial(g, abertos, nAbertos);
    int geracao = rkga->avancaGeracoesGenerico(g, tipoCruzamento);
    clock_t fim = clock();
    RK_Individual *best = rkga->getPopAtual().at(rkga->getPopAtual().size()-1);
    printf("- %f %f %d %d ", 100*1000*best->getPerdaAtiva(), (double)(fim - inicio)/CLOCKS_PER_SEC, geracao, semente);
    OS_Individual *os = new OS_Individual(best, g);
    os->imprime();
    printf("\n");
}

