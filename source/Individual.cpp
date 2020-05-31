//
// Created by mateus on 09/08/2019.
//

#include "include/Individual.h"

bool crit1(Chromosome *c1, Chromosome *c2){ return c1->position < c2->position;}
bool crit2(Chromosome *c1, Chromosome *c2){return c1->weight > c2->weight;}

///constructor that recieves a RK (Random Keys) and convert into OS (Opened Switches)
Individual::Individual(RK_Individual *ind, Graph *g) {

    /** colocar os cromossomos na ordem correta em que aparecem no grafo antes de associar os pesos **/
    sort(RK_Individual::cromossomos.begin(), RK_Individual::cromossomos.end(), crit1);

    /** copia peso do individio paca cada cromossomo **/
    for(int i=0; i< ind->getNumEdges(); i++)
        RK_Individual::cromossomos.at(i)->weight = ind->getWeights()[i];

    sort(RK_Individual::cromossomos.begin(), RK_Individual::cromossomos.end(), crit2);

    int n_arc_inseridos = 0, n_arcos_inserir = g->getVerticesSize() - 1 - g->getNumberOfNonModifiable();


    /** percorre vetor de cromossomos ordenados e tenta fechar chave(algoritmo de kruskal) **/
    for(int i=0; i<RK_Individual::cromossomos.size(); i++){

        if((RK_Individual::cromossomos.at(i)->edge->getOrigin()->getIdTree() !=
            RK_Individual::cromossomos.at(i)->edge->getDestiny()->getIdTree()) &&
                RK_Individual::cromossomos.at(i)->edge->isClosed() == false){

            int id = RK_Individual::cromossomos.at(i)->edge->getOrigin()->getIdTree();
            for(Vertex *no = g->get_verticesList(); no != NULL; no = no->getNext()){
                if(no->getIdTree() == id)
                    no->setIdTree(RK_Individual::cromossomos.at(i)->edge->getDestiny()->getIdTree());
            }

            RK_Individual::cromossomos.at(i)->edge->setSwitch(true);
            g->findEdge(RK_Individual::cromossomos.at(i)->edge->getDestiny()->getID(),
                        RK_Individual::cromossomos.at(i)->edge->getOrigin()->getID())->setSwitch(true);

            n_arc_inseridos++;
        }
        else if(RK_Individual::cromossomos.at(i)->edge->isClosed() == false &&
                RK_Individual::cromossomos.at(i)->edge->getMarked() == true){
            //opened edge that creates circle
            this->getOpenedSwitches().push_back(RK_Individual::cromossomos.at(i)->edge);
        }

    }

    g->defineFlows();
    g->evaluateLossesAndFlows(1e-8); /** calcula fluxos e perda com erro de 1e-5 **/

//    cout << "eh arvore: " << g->ehArvore() << "\teh conexo: " << g->ehConexo() << endl;

    double *perdas = g->getLossesReseting();/** soma as perdas ativas e reativas em todos os arcos e retorna um double* **/

    this->activeLoss = perdas[0];
    this->reactiveLoss = perdas[1];

    /** no caso de solucoes que possuem configuracao viavel porem sao muito longas **/
    if(isnan(perdas[0]))
        this->activeLoss = 9999999999;

    if(isnan(perdas[1]))
        this->reactiveLoss = 9999999999;

    this->size = this->openedSwitches.size();
    delete perdas;
}

Individual::Individual(int *idsAbertos, int n, Graph *g) {
    for(int i=0; i<n; i++){
        this->openedSwitches.push_back(g->findEdge(idsAbertos[i]));
    }
    this->size = this->openedSwitches.size();
    this->calcObjectiveF(g);
}

void Individual::calcObjectiveF(Graph *g) {

    this->openSwitchesInNetwork(g);

    g->defineFlows();
    g->evaluateLossesAndFlows(1e-8);
    double *loss = g->getLossesReseting();//reseta fluxos e perdas

    this->activeLoss = loss[0];
    this->reactiveLoss = loss[1];

    /** no caso de solucoes que possuem configuracao viavel porem sao muito longas **/
    if(isnan(loss[0]))
        this->activeLoss = 9999999999;

    if(isnan(loss[1]))
        this->reactiveLoss = 9999999999;

    this->size = this->getOpenedSwitches().size();

    delete loss;
}

vector<int> Individual::evaluate(Edge *e, Graph *g) {

//    printf("evaluate\n");

    this->openSwitchesInNetwork(g);

    e->setSwitch(false);
    g->findEdge(e->getDestiny()->getID(), e->getOrigin()->getID())->setSwitch(false);

//    printf("inicio: %d\n", g->getListaNos()->getID());
//    g->define_sentido_fluxos();
//    g->percursoPronfundidade(g->getListaNos(), 1);

    int flag1 = 1, flag2 = 2;
    g->markConexeComponent(e->getOrigin(), flag1);
    g->markConexeComponent(e->getDestiny(), flag2);

//    int num = 0;
////    printf("\n(aberto: %d)idsArv:\t", e->getID());
//    for(Vertex *v = g->getListaNos(); v!=NULL; v = v->getProxNo()){
//
////        printf("%d\t", v->getIdArv());
//
//        for(Edge *ed = v->getListaArcos(); ed!=NULL; ed = ed->getProxArco()){
//
//            if(ed->getChave()== true)
//                num++;
//
//        }
//    }

//    printf("num: %d\n", num);

    vector<int> candidates;

//    printf("\nvendo pares:");
    for(unsigned long int i=0; i<this->getOpenedSwitches().size(); i++){

//        printf("(%d,%d)\t", this->getOpenedSwitches().at(i)->getNoOrigem()->getIdArv(), this->getOpenedSwitches().at(i)->getNoDestino()->getIdArv());

        //if openedSwitch connect components
        //do not open fixed edges
        if(this->getOpenedSwitches().at(i)->getOrigin()->getIdTree() !=
           this->getOpenedSwitches().at(i)->getDestiny()->getIdTree() && this->getOpenedSwitches().at(i)->getFixed() == false){

            candidates.push_back(i);
        }
    }

//    printf("feito!\n");

    return candidates;
}

void Individual::openSwitchesInNetwork(Graph *g) {
    ///close all switches
    for(Vertex *v = g->get_verticesList(); v != NULL; v = v->getNext()){
        for(Edge *e = v->getEdgesList(); e != NULL; e = e->getNext()){
            e->setSwitch(true);//reset switch
        }
    }

    ///open switches of solution
    for(unsigned long int i = 0; i<this->openedSwitches.size(); i++){

        //opens a->b
        openedSwitches.at(i)->setSwitch(false);

        //opens b->a
        g->findEdge(openedSwitches.at(i)->getDestiny()->getID(),
                    openedSwitches.at(i)->getOrigin()->getID())->setSwitch(false);
    }
}

Individual::Individual(Individual *os) {

    for(unsigned int i=0; i<os->getSize(); i++)
        this->openedSwitches.push_back(os->getOpenedSwitches().at(i));

    this->activeLoss = os->getActiveLoss();
    this->reactiveLoss = os->getReactiveLoss();
    this->size = this->openedSwitches.size();
}

void Individual::imprime() {
//    printf("\nopenedSwitches:\t");
    for(unsigned long int i=0; i<this->size;i++)
        printf("%d,", this->openedSwitches.at(i)->getID());
}

bool Individual::checkSolution(Graph *g) {
    this->openSwitchesInNetwork(g);
    g->defineFlows();

    return g->isConected() && g->isTree();
}

RK_Individual *Individual::OS_to_RK(Graph *g) {

    /** colocar os cromossomos na ordem correta em que aparecem no grafo antes de associar os pesos **/
    sort(RK_Individual::cromossomos.begin(), RK_Individual::cromossomos.end(), crit1);

    RK_Individual *ind = new RK_Individual(g->getEdgesSizes() / 2 - g->getNumberOfNonModifiable());

    for(unsigned long int i=0; i<RK_Individual::cromossomos.size();i++){

        ind->getWeights()[i] = 1.0;

        for(unsigned long int j=0; j<this->size;j++){

            if(RK_Individual::cromossomos.at(i)->edge->getID() == this->openedSwitches.at(j)->getID())
                ind->getWeights()[i] = 0.0;
        }

    }
    ind->calculaFuncaoObjetivoOtimizado(g);
    return ind;
}


