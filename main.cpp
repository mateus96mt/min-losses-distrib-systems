#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "Random_keys.h"
#include "time.h"
#include <algorithm>
#include <math.h>
#include <string.h>

//#define input_file "ENTRADAS_MODIFICADAS/sist33barras_Yang.m"
#define input_file "ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m"
//#define input_file "ENTRADAS_MODIFICADAS/SISTEMA119s2.m"
//#define input_file "ENTRADAS_MODIFICADAS/sist135barras.m"
//#define input_file "ENTRADAS_MODIFICADAS/sist215barras.m"
//#define input_file "ENTRADAS_MODIFICADAS/SISTEMA83_TAIWAN.m"
//#define input_file "ENTRADAS_MODIFICADAS/SISTEMA83_TAIWAN_modificado.m"
//#define input_file "ENTRADAS_MODIFICADAS/sist69barras.m"
//#define input_file "ENTRADAS_MODIFICADAS/Caract_Sistem_85.m"

#define configuration "inicial"
//#define configuration "literatura1"
//#define configuration "literatura2"
//#define configuration "ARSD"

void openSwitches(Graph *g, int *ids, int n);
void defineConfiguration(Graph *g);
void testePopulacaoAleatoria();
void testeEntradas();
bool ordenacao(Graph *g1, Graph *g2);

void Capacitor_Test(string savename);

int main(){
    unsigned long int seed =
//            time(NULL); // Set the clock time for the beggining
            1530715368; // Best seed for 119 bus
//            1530715848; // Best seed for 33 bus modificado
//            1536085327; // Best seed for 94 bus (470,10 kw)
//            1536861169; // Best seed for 94 bus modified (491,96 kw)
    srand(seed);

    string savename = input_file;
    savename.erase ( savename.begin()   , savename.begin()+21 );
    savename.erase ( savename.length()-2, savename.length()-1 );

//    testeEntradas();//perda total e tensao minima para cada configuration para compara com a tese do leonardo willer

//    testePopulacaoAleatoria();

//    testMemLeakRandomKeys();
    Capacitor_Test( savename );


    printf("Seed: %lu", seed);
}

void Capacitor_Test(string savename){
    string name = input_file;
    Graph *g = new Graph();
    g->input_read(name);
    Random_keys *rd = new Random_keys(100, 1000);       /// numero de individuos da populacao,numero de geracaoes
    rd->generatePopulation(g);                          /// populacao inicial gerada de forma aleatoria
    rd->forwardGenerations(g);                          /// faz cruzamentos e mutacoes para gerar individuos da nova populacao

    Individual *best = rd->getPopAtual().at(rd->getPopulationSize()-1); /// melhor individuo eh o ultimo (menor perda)
    best->calculate_fitness(g); /// abre e fecha os arcos correspondentes do grafo *g para calcular funcao Objetivo

    vector<int> openSwitches;
    printf("\n\n\nMELHOR INDIVIDUO FINAL: \n");
    for(Vertex *v = g->get_verticesList(); v!=NULL; v = v->getNext()){
        for(Edge *e = v->getEdgesList(); e != NULL; e = e->getNext()){
            Edge *e_reverse = g->findEdge(e->getDestiny()->getID(), e->getOrigin()->getID());
            if(e->isClosed()==false && e_reverse->isClosed()==false)
                openSwitches.push_back(e->getID());
        }
    }
    sort(openSwitches.begin(), openSwitches.end());

    cout << "Chaves Abertas:" << endl << "| ";
    for( uint i=0; i<openSwitches.size();i+=2)   cout << openSwitches.at(i) << " | ";
    printf("\nPerda Ativa: \t %4.6f (kw)\n", 100*1000* best->getActiveLoss());
    printf("Tensao Minima: \t %4.6f (pu)\n\n\n", g->minVoltage());

    int cap1, cap2 = 0, cap3 = 0;
    string savename_cap = "out/teste_capacitor_1caps.csv";
    FILE *output_file_cap = fopen( (savename_cap).c_str(), "w");

    for(Vertex *v_cap1 = g->get_verticesList(); v_cap1!=NULL; v_cap1 = v_cap1->getNext() ){
        cap1 = v_cap1->getID();
        best->calculate_fitness_cap(g, v_cap1);
//        for(Vertex *v_cap2 = v_cap1->getNext(); v_cap2!=NULL; v_cap2 = v_cap2->getNext()){
//            cap2 = v_cap2->getID();
//            best->calculate_fitness_cap(g, v_cap1, v_cap2);
//            for(Vertex *v_cap3 = v_cap2->getNext(); v_cap3!=NULL; v_cap3 = v_cap3->getNext()) {
//                cap3 = v_cap3->getID();
//                best->calculate_fitness_cap(g, v_cap1, v_cap2, v_cap3);

                fprintf(output_file_cap, "%3d, %3d, %3d, %4.6f, %4.6f\n",
                        v_cap1->getID(), cap2, cap3, 100 * 1000 * best->getActiveLoss(), g->minVoltage());
                string namingCaps = to_string(cap1) + "-" + to_string(cap2) + "-" + to_string(cap3);
                string savename_e_aux = "out/caps1/graph_cap" + namingCaps + "_e_tree.csv";
                string savename_v_aux = "out/caps1/graph_cap" + namingCaps + "_v_tree.csv";
                FILE *output_file_edges_aux = fopen((savename_e_aux).c_str(), "w");
                FILE *output_file_vertices_aux = fopen((savename_v_aux).c_str(), "w");

                for (Vertex *v_aux = g->get_verticesList(); v_aux != NULL; v_aux = v_aux->getNext()) {
                    for (Edge *e = v_aux->getEdgesList(); e != NULL; e = e->getNext()) {
                        if (e->isClosed() == true)
                            fprintf(output_file_edges_aux, "%2d, %2d, %4.6f, %4.6f\n",
                                    e->getOrigin()->getID(), e->getDestiny()->getID(), e->getActiveLoss(), e->getActivePowerFlow());
                    }
                    fprintf(output_file_vertices_aux, "%2d, %1d, %4.6f\n",
                            v_aux->getID(), v_aux->getCapacitors().size() > 0, v_aux->getActivePower());
                }

                fclose(output_file_vertices_aux);
                fclose(output_file_edges_aux);
                v_cap1->rmCapacitor(0);
//                v_cap2->rmCapacitor(0);
//                v_cap3->rmCapacitor(0);
//            }
//        }
    }
    fclose(output_file_cap);


//    system("python plotGraph.py"); // plota grafico da rede
}

void testePopulacaoAleatoria(){
    char name[] = input_file;
    Graph *g = new Graph();

    g->input_read(name);

    int n_individuals = 500;
    vector<Graph*> h;

    for(int i=0; i<n_individuals; i++){

        double *loss;
        printf("\n\nsolucao  %d:", i);
        h.push_back(g->returnCopy());
        h.at(i)->randomSolution();
        h.at(i)->defineFlows();
        h.at(i)->evaluateLossesAndFlows(1e-8);
        loss = h.at(i)->getLosses();

        printf("\nvalida? %d", h.at(i)->isTree());
        printf("  tensao minima: %.5f (p.u)", h.at(i)->minVoltage());
        printf("  perdaTotal: %.5f (kW)", 100*1000*loss[0]);

        delete loss;
    }
}

void testeEntradas(){
    char nome[] = input_file;
    Graph *g = new Graph();
    g->input_read(nome);

    defineConfiguration(g);

    g->defineFlows();
    g->evaluateLossesAndFlows(1e-8);


    printf("\ntensao minima: %.5f (p.u)", g->minVoltage());
    printf("\nperdaTotal: %.5f (kW)", 100*1000* g->getLosses()[0]);
    printf("\neh Conexo? %d", g->isConected());
    printf("\neh arvore? %d\n\n\n", g->isTree());
}

void defineConfiguration(Graph *g){

    if( input_file == "ENTRADAS_MODIFICADAS/sist33barras_Yang.m" ){
        int ids[5];

        //CONFIGURACAO INICIAL
        if(configuration == "inicial"){
            ids[0] = 33; ids[1] = 34; ids[2] = 35; ids[3] = 36; ids[4] = 37;
        }
        //CONFIGURACAO DA LITERATURA
        if(configuration == "literatura1"){
            ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 32; ids[4] = 37;
        }
        //ABORDAGEM ARSD
        if(configuration == "ARSD"){
            ids[0] = 7; ids[1] = 9; ids[2] = 14; ids[3] = 32; ids[4] = 37;
        }

        openSwitches(g, ids, 5);
    }

    if( input_file == "ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m"){
        int ids[5];

        //CONFIGURACAO INICIAL
        if(configuration == "inicial"){
            ids[0] = 33; ids[1] = 34; ids[2] = 35; ids[3] = 36; ids[4] = 37;
        }
        //CONFIGURACAO DA LITERATURA
        if(configuration == "literatura1"){
            ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 28; ids[4] = 36;
        }
        //ABORDAGEM ARSD
        if(configuration == "ARSD"){
            ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 16; ids[4] = 28;
        }

        openSwitches(g, ids, 5);
    }

    if(input_file == "ENTRADAS_MODIFICADAS/SISTEMA119s2.m"){
        int ids[15];
        //CONFIGURACAO INICIAL
        if(configuration == "inicial"){
            ids[0] = 119; ids[1] = 120; ids[2] = 121; ids[3] = 122; ids[4] = 123;
            ids[5] = 124; ids[6] = 125; ids[7] = 126; ids[8] = 127; ids[9] = 128;
            ids[10] = 129; ids[11] = 130; ids[12] = 131; ids[13] = 132; ids[14] = 133;
        }
        //CONFIGURACAO DA LITERATURA
        if(configuration == "literatura1"){
            ids[0] = 24; ids[1] = 27; ids[2] = 35; ids[3] = 40; ids[4] = 43;
            ids[5] = 52; ids[6] = 59; ids[7] = 72; ids[8] = 75; ids[9] = 96;
            ids[10] = 99; ids[11] = 110; ids[12] = 123; ids[13] = 130; ids[14] = 131;
        }
        //ABORDAGEM ARSD
        if(configuration == "ARSD"){
            ids[0] = 24; ids[1] = 26; ids[2] = 35; ids[3] = 40; ids[4] = 43;
            ids[5] = 51; ids[6] = 61; ids[7] = 72; ids[8] = 75; ids[9] = 96;
            ids[10] = 98; ids[11] = 110; ids[12] = 122; ids[13] = 130; ids[14] = 131;
        }

        /*
        esse arquivo tem na verdade 132 arcos ao inves de 133 dessa forma os ids dos arcos abertos deve ser subtraido 1.
        */
        for(int i=0; i<15; i++)
            ids[i]--;

        openSwitches(g, ids, 15);
    }
    if( input_file == "ENTRADAS_MODIFICADAS/SISTEMA83_TAIWAN.m" ){
        int ids[13];

        //CONFIGURACAO INICIAL
        if(configuration == "inicial"){
            ids[0] = 84; ids[1] = 85; ids[2] = 86; ids[3] = 87; ids[4] = 88;
            ids[5] = 89; ids[6] = 90; ids[7] = 91; ids[8] = 92; ids[9] = 93;
            ids[10] = 94; ids[11] = 95; ids[12] = 96;
        }
        //CONFIGURACAO DA LITERATURA
        if(configuration == "literatura1"){
            ids[0] = 7; ids[1] = 13; ids[2] = 34; ids[3] = 39; ids[4] = 42;
            ids[5] = 55; ids[6] = 62; ids[7] = 72; ids[8] = 83; ids[9] = 86;
            ids[10] = 89; ids[11] = 90; ids[12] = 92;
        }
        openSwitches(g, ids, 13);
    }
    if( input_file == "ENTRADAS_MODIFICADAS/SISTEMA83_TAIWAN_modificado.m" ){
        int ids[13];

        //CONFIGURACAO INICIAL
        if(configuration == "inicial"){
            ids[0] = 84; ids[1] = 85; ids[2] = 86; ids[3] = 87; ids[4] = 88;
            ids[5] = 89; ids[6] = 90; ids[7] = 91; ids[8] = 92; ids[9] = 93;
            ids[10] = 94; ids[11] = 95; ids[12] = 96;
        }
        //CONFIGURACAO DA LITERATURA
        if(configuration == "literatura1"){
            ids[0] = 7; ids[1] = 13; ids[2] = 33; ids[3] = 38; ids[4] = 42;
            ids[5] = 63; ids[6] = 72; ids[7] = 83; ids[8] = 84; ids[9] = 86;
            ids[10] = 89; ids[11] = 90; ids[12] = 92;
        }
        if(configuration == "literatura2"){
            ids[0] = 7; ids[1] = 13; ids[2] = 34; ids[3] = 38; ids[4] = 42;
            ids[5] = 63; ids[6] = 72; ids[7] = 83; ids[8] = 84; ids[9] = 86;
            ids[10] = 89; ids[11] = 90; ids[12] = 92;
        }
        if(configuration == "ARSD"){
            ids[0] = 7; ids[1] = 13; ids[2] = 34; ids[3] = 39; ids[4] = 42;
            ids[5] = 63; ids[6] = 72; ids[7] = 83; ids[8] = 84; ids[9] = 86;
            ids[10] = 89; ids[11] = 90; ids[12] = 92;
        }

        openSwitches(g, ids, 13);
    }
    if( input_file == "ENTRADAS_MODIFICADAS/sist69barras.m" ){
        int ids[5];

        ids[0] = 69; ids[1] = 70; ids[2] = 71; ids[3] = 72; ids[4] = 73;

        openSwitches(g, ids, 5);
    }
}

void openSwitches(Graph *g, int *ids, int n){
    Edge *a;
    for(int i=0; i<n; i++){
        a = g->findEdge(ids[i]);
        a->setSwitch(false);
        a = g->findEdge(a->getDestiny()->getID(), a->getOrigin()->getID());
        a->setSwitch(false);
    }
}
