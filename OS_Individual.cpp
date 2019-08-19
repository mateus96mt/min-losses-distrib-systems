//
// Created by mateus on 09/08/2019.
//

#include "OS_Individual.h"

bool crit1(Cromossome *c1, Cromossome *c2){ return c1->posicao < c2->posicao;}
bool crit2(Cromossome *c1, Cromossome *c2){return c1->peso > c2->peso;}

///constructor that recieves a RK (Random Keys) and convert into OS (Opened Switches)
OS_Individual::OS_Individual(RK_Individual *ind, Graph_network *g) {

    /** colocar os cromossomos na ordem correta em que aparecem no grafo antes de associar os pesos **/
    sort(RK_Individual::cromossomos.begin(), RK_Individual::cromossomos.end(), crit1);

    /** copia peso do individio paca cada cromossomo **/
    for(int i=0; i<ind->getNumArcos(); i++)
        RK_Individual::cromossomos.at(i)->peso = ind->getPesos()[i];

    sort(RK_Individual::cromossomos.begin(), RK_Individual::cromossomos.end(), crit2);

    int n_arc_inseridos = 0, n_arcos_inserir = g->getNumeroNos() - 1 - g->getN_naoModificaveis();


    /** percorre vetor de cromossomos ordenados e tenta fechar chave(algoritmo de kruskal) **/
    for(int i=0; i<RK_Individual::cromossomos.size(); i++){

        if( (RK_Individual::cromossomos.at(i)->arco->getNoOrigem()->getIdArv() != RK_Individual::cromossomos.at(i)->arco->getNoDestino()->getIdArv()) && RK_Individual::cromossomos.at(i)->arco->getChave()==false){

            int id = RK_Individual::cromossomos.at(i)->arco->getNoOrigem()->getIdArv();
            for(Vertex *no = g->getListaNos(); no != NULL; no = no->getProxNo()){
                if(no->getIdArv()==id)
                    no->setIdArv(RK_Individual::cromossomos.at(i)->arco->getNoDestino()->getIdArv());
            }

            RK_Individual::cromossomos.at(i)->arco->setChave(true);
            g->buscaArco(RK_Individual::cromossomos.at(i)->arco->getNoDestino()->getID(), RK_Individual::cromossomos.at(i)->arco->getNoOrigem()->getID())->setChave(true);

            n_arc_inseridos++;
        }
        else if(RK_Individual::cromossomos.at(i)->arco->getChave()==false && RK_Individual::cromossomos.at(i)->arco->getMarcado()==true){
            //opened edge that creates circle
            this->getOpenedSwitches().push_back(RK_Individual::cromossomos.at(i)->arco);
        }

    }

    g->define_sentido_fluxos();
    g->calcula_fluxos_e_perdas(1e-8); /** calcula fluxos e perda com erro de 1e-5 **/

//    cout << "eh arvore: " << g->ehArvore() << "\teh conexo: " << g->ehConexo() << endl;

    double *perdas = g->soma_perdasResetando();/** soma as perdas ativas e reativas em todos os arcos e retorna um double* **/

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

OS_Individual::OS_Individual(int *idsAbertos, int n, Graph_network *g) {
    for(int i=0; i<n; i++){
        this->openedSwitches.push_back(g->buscaArco(idsAbertos[i]));
    }
    this->size = this->openedSwitches.size();
    this->calcObjectiveF(g);
}

void OS_Individual::calcObjectiveF(Graph_network *g) {

    this->openSwitchesInNetwork(g);

    g->define_sentido_fluxos();
    g->calcula_fluxos_e_perdas(1e-8);
    double *loss = g->soma_perdasResetando();//reseta fluxos e perdas

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

vector<int> OS_Individual::evaluate(Edge *e, Graph_network *g) {

//    printf("evaluate\n");

    this->openSwitchesInNetwork(g);

    e->setChave(false);
    g->buscaArco(e->getNoDestino()->getID(), e->getNoOrigem()->getID())->setChave(false);

//    printf("inicio: %d\n", g->getListaNos()->getID());
//    g->define_sentido_fluxos();
//    g->percursoPronfundidade(g->getListaNos(), 1);

    int flag1 = 1, flag2 = 2;
    g->marcaComponenteConexa(e->getNoOrigem(), flag1);
    g->marcaComponenteConexa(e->getNoDestino(), flag2);

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
        if(this->getOpenedSwitches().at(i)->getNoOrigem()->getIdArv() != this->getOpenedSwitches().at(i)->getNoDestino()->getIdArv() && this->getOpenedSwitches().at(i)->getFixed()==false){

            candidates.push_back(i);
        }
    }

//    printf("feito!\n");

    return candidates;
}

void OS_Individual::openSwitchesInNetwork(Graph_network *g) {
    ///close all switches
    for(Vertex *v = g->getListaNos(); v!=NULL; v = v->getProxNo()){
        for(Edge *e = v->getListaArcos(); e!= NULL; e = e->getProxArco()){
            e->setChave(true);//reset switch
        }
    }

    ///open switches of solution
    for(unsigned long int i = 0; i<this->openedSwitches.size(); i++){

        //opens a->b
        openedSwitches.at(i)->setChave(false);

        //opens b->a
        g->buscaArco(openedSwitches.at(i)->getNoDestino()->getID(), openedSwitches.at(i)->getNoOrigem()->getID())->setChave(false);
    }
}

OS_Individual::OS_Individual(OS_Individual *os) {

    for(unsigned int i=0; i<os->getSize(); i++)
        this->openedSwitches.push_back(os->getOpenedSwitches().at(i));

    this->activeLoss = os->getActiveLoss();
    this->reactiveLoss = os->getReactiveLoss();
    this->size = this->openedSwitches.size();
}

void OS_Individual::imprime() {
//    printf("\nopenedSwitches:\t");
    for(unsigned long int i=0; i<this->size;i++)
        printf("%d,", this->openedSwitches.at(i)->getID());
}

bool OS_Individual::checkSolution(Graph_network *g) {
    this->openSwitchesInNetwork(g);
    g->define_sentido_fluxos();

    return g->ehConexo() && g->ehArvore();
}

RK_Individual *OS_Individual::OS_to_RK(Graph_network *g) {

    /** colocar os cromossomos na ordem correta em que aparecem no grafo antes de associar os pesos **/
    sort(RK_Individual::cromossomos.begin(), RK_Individual::cromossomos.end(), crit1);

    RK_Individual *ind = new RK_Individual(g->getNumeroArcos()/2 - g->getN_naoModificaveis());

    for(unsigned long int i=0; i<RK_Individual::cromossomos.size();i++){

        ind->getPesos()[i] = 1.0;

        for(unsigned long int j=0; j<this->size;j++){

            if(RK_Individual::cromossomos.at(i)->arco->getID() == this->openedSwitches.at(j)->getID())
                ind->getPesos()[i] = 0.0;
        }

    }
    ind->calculaFuncaoObjetivoOtimizado(g);
    return ind;
}


