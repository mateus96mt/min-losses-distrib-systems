#include "include/Graph.h"
#include <fstream>
#include <math.h>
#include "time.h"
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

Graph::Graph(){

    this->verticesList = NULL;
    this->verticesSize = 0;
    this->edgesSize = 0;
    this->totalActiveLoss = 0.0;
    this->totalReactiveLoss = 0.0;
    this->numberOfMarked = 0;
    this->maxCapacitorsBus = 0;
    this->loads = vector<LoadLevel>();
}

Graph::~Graph(){
    Vertex *aux = new Vertex(0);
    for(Vertex *v=this->verticesList; v != NULL; v= v->getNext()){
        delete aux;
        for(Edge *e= v->getEdgesList(); e != NULL; e= e->getNext()){
            delete e;
        }
        aux = v;
    }
    delete aux;
}

Vertex *Graph::findVertex(int id){

    Vertex *v = verticesList;
    while(v!=NULL){
        if(v->getID() == id)
            return v;
        v = v->getNext();
    }
    return NULL;

}

Edge *Graph::findEdge(int id){
    for(Vertex *v= get_verticesList();  v!=NULL; v = v->getNext()){
        for(Edge *e= v->getEdgesList(); e!=NULL; e = e->getNext()){
            if(e->getID() == id)
                return e;
        }
    }
    return NULL;
}

Edge *Graph::findEdge(int originID, int destinyID){
    for(Vertex *v= get_verticesList(); v != NULL; v= v->getNext()){
        for(Edge *a= v->getEdgesList(); a != NULL; a= a->getNext()){
            if(a->getOrigin()->getID() == originID && a->getDestiny()->getID() == destinyID)
                return a;
        }
    }
    return NULL;
}
//TODO Pode ser geográfico os fatores de carga por isso o último param
void Graph::insertVertex( int id, double activePower, double reactivePower, double voltage, int idLoadFactor ){

    Vertex *vertex=new Vertex(id);
    vertex->setActivePower(activePower);
    vertex->setReactivePower(reactivePower);
    vertex->setVoltage(voltage);
    vertex->setLoadFactors( this->loads );
    vertex->setNext(verticesList);
    this->verticesList = vertex;
    this->verticesSize++;
}

void Graph::insertEdge(int originID, int destinyID, int id, double resist, double react, bool swit){
    Vertex *originVertex = findVertex(originID);
    Vertex *destinyVertex = findVertex(destinyID);
    Edge *newEdge = new Edge(id);
    newEdge->setDestiny(destinyVertex);
    newEdge->setOrigin(originVertex);

    // Insere Arco no inicio da lista
    newEdge->setNext(originVertex->getEdgesList());
    originVertex->setEdgesList(newEdge);
    newEdge->setResistance(resist);
    newEdge->setReactance(react);
    newEdge->setSwitch(swit);

    originVertex->setOutdegree(originVertex->getIndegree() + 1);
    destinyVertex->setIndegree(destinyVertex->getOutdegree() + 1);

    this->edgesSize++;
}

/**
 * Create types for capacitors in the network
 * @params  id:     capacitor tyoe identifier
 *          power:  reactive power of the capacitor in kVar
 *          cost:   cost in US$/kVar
 */
void Graph::createCapacitorType(int id, double reactive_power, double cost_per_KVAr, int step){
    cout << "Capacitor Created with pot: " << reactive_power/(PB*1000) << endl;
    double cost = cost_per_KVAr * reactive_power;
    this->capacitorType.insert( this->capacitorType.begin(), Capacitor( id, reactive_power/(PB*1000), cost ) );
};

/**
 * Return an array with the total losses in KWh and in US$ in that order
 */
double * Graph::getTotalLoss(){
    double * total_loss = new double[2];
    total_loss[0] = 0.0;
    total_loss[1] = 0.0;
    for(int i = 0; i < this->loads.size() ; i++) {
        double lvl_loss = 100 * 1000 * this->evaluateLossesAndFlows(1e-12, i);
        total_loss[0] += lvl_loss;
        total_loss[1] += lvl_loss * this->loads[i].time * this->loads[i].cost;
    }
    return total_loss;
}

void Graph::show_losses( double powerLoss, double minTension, int idLoad ){

    if(idLoad == -1) {
        double total_time = 0.0;
        for(int i = 0; i < 3; i++)
            total_time += this->loads[i].time;
        printf(" Total \t\t|  %9.6f \t| \t%9.2f US$ \t|\t  -  \n", powerLoss, this->getTotalLoss()[1]  );
    }
    else {
        double effective_loss = powerLoss * this->loads[idLoad].time;
        double loss_cost = effective_loss * this->loads[idLoad].cost;
        printf(" %1.2f \t\t|  %9.6f \t| \t%9.2f US$ \t|\t %.6f \n", this->loads[idLoad].level, powerLoss, loss_cost, minTension);
    }
    printf("--------------------------------------------------------------------\n");
};

//TODO: MELHORAR ESSA MERDA
void Graph::input_read( string name ){

    cout << name << endl;
    int edgeId = 0;
    double power, resistance, reactance, reactive_power, voltage;

    ifstream input;
    input.open(name);

    string aux;

    int n_col_v = 13, n_col_e = 12; //numero de colunas do arquivo de entradaz
    while(aux != "Vb") input >> aux;

    input >> aux;
    input >> VB;
    input >> aux;input >> aux;
    input >> PB;
    input >> aux;input >> aux;
    input >> ZB;

    input.close();
    input.open(name);
    int vertexId, originId, destinyId;

    do{
        input >> aux;
    }while(aux != "num_nos");///pula lixo ate chegar na parte de informacoes do no

    int num_nos;
    input >> num_nos;
    for(int i=1 ; i<=num_nos*n_col_v; i++){
        if(i % n_col_v == 1){
            input >> vertexId;
        }
        else if(i % n_col_v == 4){
            input >> voltage;
        }
        else if(i % n_col_v == 10){
            input >> power;
            power /= PB;
            power *= mw_factor;
        }
        else if(i % n_col_v == 11){
            input >> reactive_power;
            reactive_power /=PB;
            reactive_power *= mw_factor;
            insertVertex(vertexId, power, reactive_power, voltage);
        }
        else
            input >> aux;
    }

    do{
        input >> aux;
    }while(aux != "num_arestas");

    int n_edges;
    input >> n_edges;

    for(int i=1 ; i<=n_edges*n_col_e; i++){
        if(i % n_col_e == 1){
            input >> originId;
        }
        else if(i % n_col_e == 2){
            input >> destinyId;
        }
        else if(i % n_col_e == 3){
            input >> resistance;
            resistance /= ZB;
        }
        else if(i % n_col_e == 4){
            input >> reactance;
            reactance /= ZB;

            edgeId++;
            insertEdge(originId, destinyId, edgeId, resistance, reactance, false);
            insertEdge(destinyId, originId, edgeId, resistance, reactance, false);
        }
        else
            input >> aux;
    }
    cout << "\n" << name << " lida!" << endl;
}

//TODO: MELHORAR ESSA MERDA
double Graph::branchActiveLoss(Vertex *no){
    double sum = no->getActivePower();
    auxBranchActiveLoss(no, sum);
    return sum;
}

//TODO: MELHORAR ESSA MERDA
// TODO:
// TODO: MUDAR a PARA e
// TODO:
void Graph::auxBranchActiveLoss(Vertex *no, double &soma){
    for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){

        //nao descer por arcos com chave aberta
        while(a!=NULL && a->isClosed() == false){

            //nao tem fluxo nem perda em arcos abertos
            a->setActiveFlow(0.0);
            a->setReactiveFlow(0.0);
            a->setActiveLoss(0.0);
            a->setReactiveLoss(0.0);

            a = a->getNext();
        }
        if(a==NULL)
            break;

        soma+= a->getDestiny()->getActivePower() + a->getActiveLoss();
        auxBranchActiveLoss(a->getDestiny(), soma);
    }
}

//TODO: MELHORAR ESSA MERDA
double Graph::branchReactiveLoss(Vertex *v){
    double sum = v->getReactivePower();
    auxBranchReactiveLoss(v, sum);
    return sum;
}

//TODO: MELHORAR ESSA MERDA
void Graph::auxBranchReactiveLoss(Vertex *v, double &sum){
    for(Edge *e = v->getEdgesList(); e!=NULL; e = e->getNext()){

        //nao descer por arcos com chave aberta
        while(e!=NULL && e->isClosed() == false){

            //nao tem fluxo nem perda em arcos abertos
            e->setActiveFlow(0.0);
            e->setReactiveFlow(0.0);
            e->setActiveLoss(0.0);
            e->setReactiveLoss(0.0);

            e = e->getNext();
        }
        if(e==NULL)
            break;

        sum+= e->getDestiny()->getReactivePower() + e->getReactiveLoss();
        auxBranchReactiveLoss(e->getDestiny(), sum);
    }
}

//TODO: MELHORAR ESSA MERDA
void Graph::forward(int it){
    this->verticesList->setVoltage(1.0);//voltagem controlada na estacao
    auxForward(this->verticesList, this->verticesList->getEdgesList(), it);
}

//TODO: MELHORAR ESSA MERDA
void Graph::auxForward(Vertex *vertex, Edge *edge, int it){
    if(vertex == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Edge *e = vertex->getEdgesList(); e!=NULL; e= e->getNext()){

            double active_power     = 0.0;
            double reactive_power   = 0.0;

            /// set open edges with flow-free
            while(e!=NULL && !e->isClosed() ){

                e->setActiveFlow(0.0);
                e->setReactiveFlow(0.0);
                e->setActiveLoss(0.0);
                e->setReactiveLoss(0.0);

                e = e->getNext();
            }
            if(e==NULL)
                break;

            //chute inicial para o fluxo nas arests que partem do no terminal
            if( vertex==this->verticesList ) {
                double activeLossCharge = branchActiveLoss(e->getDestiny());
                double reactiveLossCharge = branchReactiveLoss(e->getDestiny());

                e->setActiveFlow(activeLossCharge + e->getActiveLoss());
                e->setReactiveFlow(reactiveLossCharge + e->getReactiveLoss());

            } else {
                //a partir da primeira iteracao considera-se a perda calculada na iteracao anterior
                if(it>0){
                    active_power = edge->getResistance()*(pow(edge->getActivePowerFlow(), 2) + pow(edge->getReactivePowerFlow(), 2)) / pow(
                            edge->getOrigin()->getVoltage(), 2);    // Q = R*P^2 + Q^2/V^2
                    reactive_power =
                            edge->getReactance()*(pow(edge->getActivePowerFlow(), 2) + pow(edge->getReactivePowerFlow(), 2)) / pow(
                                    edge->getOrigin()->getVoltage(), 2);
                }

                e->setActiveFlow(edge->getActivePowerFlow() - active_power - vertex->getActivePower());
                e->setReactiveFlow(edge->getReactivePowerFlow() - reactive_power - vertex->getReactivePower());

                //bifurcacao - o fluxo que seque para o arco 'a' deve-se subtrair a soma de cargas e perdas dos ramos de bifurcacoes
                if(e->getOrigin()->getIndegree()>1){
                    double somaAtiv=0.0, somaReAtiv=0.0;
                    for(Edge *aux= vertex->getEdgesList(); aux!=NULL; aux= aux->getNext()){
                        if(aux!=e && aux->isClosed()==true){
                            somaAtiv+= aux->getActiveLoss() + branchActiveLoss(aux->getDestiny());
                            somaReAtiv+= aux->getReactiveLoss() + branchReactiveLoss(aux->getDestiny());
                        }
                    }

                    e->setActiveFlow(e->getActivePowerFlow() - somaAtiv);
                    e->setReactiveFlow(e->getReactivePowerFlow() - somaReAtiv);
                }
            }

            auxForward(e->getDestiny(), e, it);
        }
    }
}

//TODO: MELHORAR ESSA MERDA
void Graph::backward(){
    this->verticesList->setVoltage(1.0); // voltagem controlada na estacao
    auxBackward(this->verticesList);
}

//TODO: MELHORAR ESSA MERDA
void Graph::auxBackward(Vertex *vertex){
    if(vertex == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Edge *e= vertex->getEdgesList(); e!=NULL; e= e->getNext()){

            //nao descer por arcos com chave aberta
            while(e!=NULL && e->isClosed() == false){

                //nao tem fluxo nem perda em arcos abertos
                e->setActiveFlow(0.0);
                e->setReactiveFlow(0.0);
                e->setActiveLoss(0.0);
                e->setReactiveLoss(0.0);

                e = e->getNext();
            }
            if(e==NULL)
                break;

            //----backward----
            Vertex *destiny = e->getDestiny();
            Vertex *origin = e->getOrigin();

            destiny->setVoltage(pow(origin->getVoltage(), 2) - 2 * (e->getResistance() * e->getActivePowerFlow() +
                    e->getReactance() * e->getReactivePowerFlow()) + (pow(e->getResistance(), 2) + pow(e->getReactance(), 2)) *
                               (pow(e->getActivePowerFlow(), 2) + pow(e->getReactivePowerFlow(), 2)) / pow(origin->getVoltage(), 2));

            destiny->setVoltage(sqrt(destiny->getVoltage()));

            e->setActiveLoss(e->getResistance() * (pow(e->getActivePowerFlow(), 2) + pow(e->getReactivePowerFlow(), 2))
                             / pow(origin->getVoltage(), 2));
            e->setReactiveLoss(e->getReactance() * (pow(e->getActivePowerFlow(), 2) + pow(e->getReactivePowerFlow(), 2))
                               / pow(origin->getVoltage(), 2));
            //-------------

            auxBackward(e->getDestiny());
        }
    }
}

double * Graph::getLosses(){
    double *loss = new double[2];
    loss[0] = 0.0;
    loss[1] = 0.0;
    for(Vertex *v=verticesList; v!=NULL; v = v->getNext()){
        for(Edge *e = v->getEdgesList(); e!=NULL; e = e->getNext()){
            loss[0] += e->getActiveLoss();
            loss[1] += e->getReactiveLoss();
        }
    }
    return loss;
}

// TODO:
// TODO: MUDAR PORTUGUES PARA INGLES
// TODO:
double *Graph::getLossesReseting(){
    double *perda = new double[2]; perda[0] = perda[1] = 0.0;
    for(Vertex *no=verticesList; no != NULL; no = no->getNext()){
        no->setIdTree(no->getID());
        for(Edge *a = no->getEdgesList(); a != NULL; a = a->getNext()){
            perda[0] += a->getActiveLoss();
            perda[1] += a->getReactiveLoss();

            //arcos nao modificaveis ficam sempre fechados
            if(a->getModifiable() == false)
                a->setSwitch(true);
            else
                a->setSwitch(false);

            a->setActiveFlow(0.0);
            a->setReactiveFlow(0.0);
            a->setActiveLoss(0.0);
            a->setReactiveLoss(0.0);
        }
    }
    return perda;
}

/**
    Return the total active loss in KW
 */
double Graph::evaluateLossesAndFlows(double tol, int idLoadFactorGlobal){
    this->resetFlowAndLoss();
    Vertex::idLF = idLoadFactorGlobal; /// Static Param. Muda rede toda

    double *last_total_loss, *current_total_loss , error = 1.0;

    last_total_loss = this->getLosses();
    for( int it = 0; error>tol; it++ ){
        forward(it);        // Calcula fluxos de potencia nas linhas
        backward();         // Calcula voltagem nas barras e perdas nas linhas

        current_total_loss = this->getLosses();
        error = current_total_loss[0] - last_total_loss[0];
        delete last_total_loss;

        last_total_loss = current_total_loss;
    }
    return this->getLosses()[0];
}

Vertex *Graph::greedyCapacitorAllocation(){
    double tol = 1e-7;
    Vertex *vertex = this->verticesList;
    while(vertex != NULL){
//        cout << endl << " Origin  Vertex #" << vertex->getID() << endl << " Destinations |";
        Edge *edgeChoice = vertex->getEdgesList();
        double criteria = edgeChoice->getReactivePowerFlow();
//        cout << " Vertex #" << edgeChoice->getDestiny()->getID() << ": " << edgeChoice->getReactivePowerFlow() << "\t|";
        for( Edge *e = edgeChoice->getNext(); e != NULL; e = e->getNext()) {
//            cout << " Vertex #" << e->getDestiny()->getID() << ": " << e->getReactivePowerFlow() << "\t|";
            if( (criteria) < (e->getReactivePowerFlow()) ){
                edgeChoice = e;
                criteria = edgeChoice->getReactivePowerFlow();
            }

        }
//        cout << endl << " BEST #" << edgeChoice->getDestiny()->getID() << ":    " << edgeChoice->getReactivePowerFlow() << endl;
        if( fabs(criteria) < tol) break;
        vertex = edgeChoice->getDestiny();
    }
//    cout <<  "              ALLOCATION VERTEX #" << vertex->getID() << endl;
    return vertex;
};

void Graph::unmarkVertices(){
    for(Vertex *i = this->verticesList; i != NULL; i = i->getNext())
        i->setMarked(false);
    this->numberOfMarked = 0;
}

bool Graph::isConected(){
    this->unmarkVertices();
    int n_markeds = 0;
    auxIsConnected(this->verticesList, n_markeds);

    if(n_markeds == this->verticesSize)
        return true;
    else
        return false;
}

// TODO:
// TODO: MUDAR PORTUGUEIS PARA INGLEIS
// TODO:
void Graph::auxIsConnected(Vertex *v, int &n_markeds){
    if(v == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        if(v->getMarked() == false){
            v->setMarked(true);
            n_markeds++;

            for(Edge *a= v->getEdgesList(); a!=NULL; a= a->getNext()){

                //nao descer por arcos com chave aberta
                while(a!=NULL && a->isClosed() == false){
                    a = a->getNext();
                }

                if(a==NULL)
                    break;

                auxIsConnected(a->getDestiny(), n_markeds);
            }
        }
    }
}

bool Graph::isTree(){
    unmarkVertices();

    bool isCycle = false;
    int markeds = 0;

    auxIsTree(verticesList, markeds, isCycle);

    if(isCycle==true)
        cout << "\nFECHOU CICLO!!" << endl;
    if(markeds<verticesSize)
        cout << "\nARVORE NAO COBRE TODOS OS NOS!!";

    if(isCycle==false && markeds==verticesSize)
        return true;
    else
        return false;
}

void Graph::auxIsTree(Vertex *v, int &markeds, bool &isCycle){
    if(v == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        if(v->getMarked() == false){
            v->setMarked(true);
            markeds++;

            for(Edge *a= v->getEdgesList(); a!=NULL; a= a->getNext()){

                //nao descer por arcos com chave aberta
                while(a!=NULL && a->isClosed() == false){
                    a = a->getNext();
                }

                if(a==NULL)
                    break;

                auxIsTree(a->getDestiny(), markeds, isCycle);
            }
        }
        else
            isCycle = true;
    }
}

double Graph::minVoltage(){
    double tension_minimum = 1.0;
    for(Vertex *no=this->verticesList; no!=NULL; no= no->getNext()){
        if(no->getVoltage() < tension_minimum)
            tension_minimum = no->getVoltage();
    }
    return tension_minimum;
}

void Graph::defineFlows(){
    auxDefineFlows(this->verticesList, this->verticesList);
}
//TODO: MELHORAR ESSA MERDA
void Graph::auxDefineFlows(Vertex *v, Vertex *v_previous){
    if(v == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Edge *e= v->getEdgesList(); e!=NULL; e= e->getNext()){

            //nao descer por arcos com chave aberta e descer no sentido correto
            //e->getNoDestino()==v e o arco de volta do arco 'a' assim nao passamos por ele
            while(e!=NULL && (e->isClosed() == false || e->getDestiny()==v_previous) ){

                //nao tem fluxo nem perda em arcos abertos
                e->setActiveFlow(0.0);
                e->setReactiveFlow(0.0);
                e->setActiveLoss(0.0);
                e->setReactiveLoss(0.0);

                if(e->getDestiny()==v_previous)
                    e->setSwitch(false);

                e = e->getNext();
            }
            if(e==NULL)     break;

            auxDefineFlows(e->getDestiny(), v);
        }
    }
}

/*
 *
 * Função para definir quais as arestas modificaveis (que podem ser abertas)
 * agora o random keys usa um vetor menor contendo somente valores para arcos modificaveis
 * */
void Graph::defineModifiables(){

    this->resetModifiables();//define todos os arcos como sendo modificaveis
    int n=1;
    for(int i=0; i<this->edgesSize/2; i++){

        //procura arcos que conectam nos folha, esses arcos nao sao modificaveis(nao podem ser abertos)
        for(Vertex *no=this->verticesList; no!=NULL; no= no->getNext()){
            for(Edge *e= no->getEdgesList(); e!=NULL; e= e->getNext()){

                if(e->getDestiny()->getAuxDegree()==1 && e->isClosed()==true && e->getModifiable()==true){

//                    printf("\ndefinindo como nao modif A{%d}...", a->getID());

                    e->setModifiable(false);
		    e->setSwitch(true);//nao modificavel: chave sempre fechad

                    Edge *e_reverse = this->findEdge(e->getDestiny()->getID(), e->getOrigin()->getID());
                    e_reverse->setModifiable(false);

                    e->getOrigin()->setAuxDegree(e->getOrigin()->getAuxDegree() - 1);
                    e->getDestiny()->setAuxDegree(e->getDestiny()->getAuxDegree() - 1);

//                  printf("Modificaveis definidos!");

                    n++;
                }
            }
        }
    }
    this->numberOfNonModifiable = n;
//    printf("\ndefiniu arestas modificaveis!");
}

Graph *Graph::returnCopy(){
    Graph *g = new Graph();
    int originId, destinyId, edgeId, vertexId;
    double resist, react, activePower, reactivePower, voltage;

    for(Vertex *no= this->verticesList->getNext(); no!=NULL; no= no->getNext()){
        vertexId = no->getID();
        activePower = no->getActivePower();
        reactivePower = no->getReactivePower();
        voltage = no->getVoltage();

        g->insertVertex(vertexId, activePower, reactivePower, voltage);
    }

    //inserindo no fonte por ultimo pos a funcao que insere nós insere sempre no inicio da lista, assim o nó fonte fica no inicio da lista
    Vertex *source = this->verticesList;
    g->insertVertex(source->getID(), source->getActivePower(), source->getReactivePower(), source->getVoltage());

    for(Vertex *v=this->verticesList; v!=NULL; v= v->getNext()){
         for(Edge *e= v->getEdgesList(); e!=NULL; e= e->getNext()){
            originId = e->getOrigin()->getID();
            destinyId = e->getDestiny()->getID();
            edgeId = e->getID();
            resist = e->getResistance();
            react = e->getReactance();

             g->insertEdge(originId, destinyId, edgeId, resist, react, e->isClosed());
        }
    }
    return g;
}

/*Algoritmo de Kruskal randomizado*/
void Graph::randomSolution(){
    vector<Edge*> vec_edges;
    for(Vertex *v = this->get_verticesList(); v!=NULL; v = v->getNext()){
        for(Edge *e = v->getEdgesList(); e!=NULL; e = e->getNext()){
            e->setSwitch(false);
            vec_edges.push_back(e);
        }
    }

    random_shuffle(vec_edges.begin(), vec_edges.end());

    int n_inserted_edges = 0, n_to_insert = this->verticesSize-1;
    for(int i=0; n_inserted_edges < n_to_insert; i++){

        if( (vec_edges.at(i)->getOrigin()->getIdTree() != vec_edges.at(i)->getDestiny()->getIdTree()) &&
                vec_edges.at(i)->isClosed()==false){

            int id = vec_edges.at(i)->getOrigin()->getIdTree();
            for(Vertex *no = this->verticesList; no!=NULL; no = no->getNext()){
                if(no->getIdTree()==id)
                    no->setIdTree(vec_edges.at(i)->getDestiny()->getIdTree());
            }
            vec_edges.at(i)->setSwitch(true);
            this->findEdge(vec_edges.at(i)->getDestiny()->getID(), vec_edges.at(i)->getOrigin()->getID())->setSwitch(true);

            n_inserted_edges++;
        }

    }

    printf("  Aberto:{");
    for(Vertex *v = this->verticesList; v!=NULL; v = v->getNext()){
        for(Edge *e = v->getEdgesList(); e!=NULL; e = e->getNext()){
            if(e->isClosed()==false)
                printf("%d,", e->getID());
        }
    }
    printf("  }");
}

void Graph::resetAuxDegrees(){
    for(Vertex *v = this->verticesList; v!=NULL; v = v->getNext()){
        for(Edge *e = v->getEdgesList(); e!=NULL; e = e->getNext()){
            Vertex *orig, *dest;
            orig = e->getOrigin();
            dest = e->getDestiny();

            orig->setAuxDegree(orig->getOutdegree());
            dest->setAuxDegree(dest->getOutdegree());
        }
    }
}

void Graph::resetIDTree(){
    for(Vertex *v = this->verticesList; v!=NULL; v = v->getNext()){
        v->setIdTree(v->getID());
    }
}

void Graph::resetFlowAndLoss(){
    for(Vertex *v=this->verticesList; v!=NULL; v= v->getNext()){
        for(Edge *e= v->getEdgesList(); e!=NULL; e= e->getNext()){
            e->setActiveFlow(0.0);
            e->setReactiveFlow(0.0);
            e->setActiveLoss(0.0);
            e->setReactiveLoss(0.0);
        }
    }
}

void Graph::capacitor_allocation(){
    int idLoad = 1;
    double capacitor_cost = this->getCapacitorType(1).getCost();   // Custo do capacitor em kW
    this->evaluateLossesAndFlows(1e-12, idLoad);                   // Avalia no nível de carga mais pesado?

    double * last_loss_cost = new double[2];
    last_loss_cost[0] = this->getTotalLoss()[0];
    last_loss_cost[1] = this->getTotalLoss()[1];
    double cost_change = capacitor_cost*2;

//    uint iter = 0;
    Vertex * alloc_vertex ;
    while( capacitor_cost < cost_change ) {
        alloc_vertex = this->greedyCapacitorAllocation();
//        cout << "Vertice cap: " << alloc_vertex->getID() << endl;
        alloc_vertex->addCapacitor( this->getCapacitorType(1) );
        this->evaluateLossesAndFlows(1e-12, idLoad);
        cost_change = last_loss_cost[1] - this->getTotalLoss()[1];
//        cout << "  Total Loss:  " << graph->getTotalLoss()[0] << "KW"  << endl;
//        cout << "  Total Cost:  " << graph->getTotalLoss()[1] << "US$"  << endl;
//        cout << "  Loss Change: " << last_loss_cost[0] - this->getTotalLoss()[0] << "KW"  << endl;
//        cout << "  Cost Change: " << cost_change  << " US$"     << endl;
        last_loss_cost[0] = this->getTotalLoss()[0];
        last_loss_cost[1] = this->getTotalLoss()[1];
//        iter++;
    }

    alloc_vertex->rmLastCapacitor();
//    iter--;
//    cout << "Num caps: " << iter << endl;
}

void Graph::closeNotModifiableEdges(){
    for(Vertex *no = this->verticesList; no != NULL; no = no->getNext()){

        for(Edge *a = no->getEdgesList(); a != NULL; a = a->getNext()){
            if(a->getModifiable() == false){
                a->setSwitch(true);
            }
        }

    }
}

void Graph::resetModifiables(){
    for(Vertex *no = this->verticesList; no != NULL; no = no->getNext()){

        for(Edge *a = no->getEdgesList(); a != NULL; a = a->getNext()){
            a->setModifiable(true);
        }
    }
}

/*
 * Reseta variável "marca" das arestas
 * essa variável é usada para marcar somente um dos arcos para cada verfice
 * indivíduo do random keys fica reduzido pela metade, usando um peso para um arco somente
 * */
void Graph::resetEdgesMarking(){
    for(Vertex *no = this->verticesList; no != NULL; no = no->getNext()){

        for(Edge *a = no->getEdgesList(); a != NULL; a = a->getNext()){
            a->setMarked(false);
        }
    }
}

/** percorre todos os arcos do grafo e marca somente os arcos em um sentido
(funcao necessaria para otimizar o AG utilizando metade do numero de arcos) **/
void Graph::markOneDirectionInEdges(){
    this->resetEdgesMarking();
    Edge *aux;
    for(Vertex *no = this->verticesList; no != NULL; no = no->getNext()){

        for(Edge *a = no->getEdgesList(); a != NULL; a = a->getNext()){

            if(a->getMarked() == false){
                a->setMarked(false);
                aux = this->findEdge(a->getDestiny()->getID(), a->getOrigin()->getID());
                aux->setMarked(true);
            }

        }
    }
}

void Graph::markConexeComponent(Vertex *start, int value) {
    aux_markConexeComponent(start, start, value);
}

void Graph::aux_markConexeComponent(Vertex *start, Vertex *previous, int value) {
    if(start == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Edge *a= start->getEdgesList(); a != NULL; a= a->getNext()){

            if(a->isClosed() == true) {
                this->findEdge(a->getDestiny()->getID(), a->getOrigin()->getID())->setSwitch(false);
//                start->setIdArv(value);
                a->getOrigin()->setIdTree(value);
                a->getDestiny()->setIdTree(value);
                this->aux_markConexeComponent(a->getDestiny(), start, value);
            }
        }
    }
}

void Graph::deepSearchPath(Vertex *start, int value) {
    if(start==NULL)
        printf("inicio NULL\n");
    else{
        for(Edge *e = start->getEdgesList(); e != NULL; e = e->getNext()){
            start->setIdTree(value);
            printf("no: %d\t", start->getID());
            this->deepSearchPath(e->getDestiny(), value);
        }
    }
}
