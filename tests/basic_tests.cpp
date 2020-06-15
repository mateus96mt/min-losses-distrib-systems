//
// Created by mateus on 31/05/2020.
//

#include "tests/basic_tests.h"
#include "include/RK_Individual.h"

void BasicTests::readNetworkTest(string networkSourceFile) {
    Graph *g = new Graph();
    g->input_read(networkSourceFile);
}

void BasicTests::print_tree(const string savefolder, Graph *g) {
    system(("mkdir " + savefolder + " -p").c_str());
    system(("mkdir " + savefolder + "/Originals -p").c_str());

    string savename_e_aux = savefolder + "/original_e_tree.csv";
    string savename_v_aux = savefolder + "/original_v_tree.csv";
    FILE *output_file_edges_aux = fopen((savename_e_aux).c_str(), "w");
    FILE *output_file_vertices_aux = fopen((savename_v_aux).c_str(), "w");

    for (Vertex *v_aux = g->get_verticesList(); v_aux != NULL; v_aux = v_aux->getNext()) {
        for (Edge *e = v_aux->getEdgesList(); e != NULL; e = e->getNext()) {
            if (e->isClosed())
                fprintf(output_file_edges_aux, "%2d, %2d, %4.6f, %4.6f, %4.6f, %4.6f, %4.6f, %4.6f\n",
                        e->getOrigin()->getID(), e->getDestiny()->getID(), e->getActiveLoss(), e->getActivePowerFlow(),
                        e->getReactiveLoss(), e->getReactivePowerFlow(), e->getResistance(), e->getReactance());
        }
        fprintf(output_file_vertices_aux, "%2d, %1d, %4.6f, %4.6f, %4.6f\n",
                v_aux->getID(), (int) v_aux->getCapacitors().size(), v_aux->getActivePower(), v_aux->getReactivePower(),
                v_aux->getVoltage());
    }
    fclose(output_file_vertices_aux);
    fclose(output_file_edges_aux);
    system(("python3 printTree.py " + savefolder).c_str()); // plota grafico da rede
}

void BasicTests::Capacitor_Test_GA(string input_file) {
    string name = input_file;

    Graph *graph = new Graph();

    /// Adiciona níveis de carga (IVO)
    graph->addChargeLevel(0.50, 1000, 0.06);
    graph->addChargeLevel(1.00, 6760, 0.06);
    graph->addChargeLevel(1.80, 1000, 0.06);
//    graph->addChargeLevel( 2.45, 1000, 0.06 );

    graph->input_read(name);
    graph->createCapacitorType(1, 300,
                               3);                  // Colocar o valor do capacitor como um parametro de leitura pra ser convertido pelo Pb

    Random_keys *rd = new Random_keys(100, 1000);           /// numero de individuos da populacao,numero de geracoes
    rd->geraPopAleatoria(graph);                          /// populacao inicial gerada de forma aleatoria
    rd->avancaGeracoes(
            graph);                          /// faz cruzamentos e mutacoes para gerar individuos da nova populacao

    RK_Individual *best = rd->getPopAtual().at(
            rd->getTamPopulacao() - 1); /// melhor individuo eh o ultimo (menor perda)
    best->calculate_fitness(graph,
                            true); /// abre e fecha os arcos correspondentes do grafo *g para calcular funcao Objetivo

    vector<int> openSwitches;
    printf("\n\n\nMELHOR INDIVIDUO FINAL: \n");
    for (Vertex *v = graph->get_verticesList(); v != NULL; v = v->getNext()) {
        for (Edge *e = v->getEdgesList(); e != NULL; e = e->getNext()) {
            Edge *e_reverse = graph->findEdge(e->getDestiny()->getID(), e->getOrigin()->getID());
            if (!e->isClosed() && !e_reverse->isClosed())
                openSwitches.push_back(e->getID());
        }
    }
    sort(openSwitches.begin(), openSwitches.end());

    cout << "Chaves Abertas:" << endl << "| ";
    for (uint i = 0; i < openSwitches.size(); i += 2) cout << openSwitches.at(i) << " | ";
    cout << endl;
    printf("--------------------------------------------------------------------\n");
    printf(" Load lvl \t| \t Losses\t\t| \t     Cost \t\t| \t Min Tension \n");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < 3; i++) {
        graph->evaluateLossesAndFlows(1e-12, i);
        graph->show_losses(100 * 1000 * graph->getLosses()[0], graph->minVoltage(), i);
    }
    graph->show_losses(graph->getTotalLoss()[0], graph->minVoltage(), -1);

    print_tree("out/" + name, graph);
}

void BasicTests::brandaoTest(string input_file) {
    srand(time(NULL));

    string savename = input_file;
    savename.erase(savename.begin(), savename.begin() + 7);
    savename.erase(savename.length() - 2, savename.length() - 1);

//    testeEntradas(savename);//perda total e tensao minima para cada configuration para compara com a tese do leonardo willer

//    Capacitor_Test_Ivo(savename);
    Capacitor_Test_GA(savename);
}

void BasicTests::testRandomKeysAG(string networkSourceFile, int populationSize, int numGenerations) {
    Graph *g = new Graph();
    g->input_read(networkSourceFile);
    g->defineModifiables();
    g->markOneDirectionInEdges();

    RK_Individual::criaCromossomos(g);

    Random_keys *randomKeys = new Random_keys(populationSize, numGenerations);
    randomKeys->geraPopAleatoria(g);
    randomKeys->ordenaPopulacaoAtual(g);
    printf("best individual: %f",
           100 * 1000 * randomKeys->getPopAtual().at(randomKeys->getTamPopulacao() - 1)->getActiveLoss());
}

void BasicTests::testCalculationOfObjectFunctionOpmized(string networkSourceFile) {
    Graph *g = new Graph();
    g->input_read(networkSourceFile);
    g->defineModifiables();
    g->markOneDirectionInEdges();

    for (Vertex *v = g->get_verticesList(); v != NULL; v = v->getNext()) {
        printf("| idarv: %d", v->getIdTree());
        for (Edge *e = v->getEdgesList(); e != NULL; e = e->getNext()) {
            printf("- e->closed: %d\n", e->isClosed());
        }
    }

    RK_Individual::criaCromossomos(g);

    RK_Individual *rkIndividual = new RK_Individual(RK_Individual::cromossomos.size());
    rkIndividual->generate_random_weights();
    rkIndividual->calculaFuncaoObjetivoOtimizado(g);
    printf("random weights individual objective funcion: %f", 100 * 1000 * rkIndividual->getActiveLoss());
}