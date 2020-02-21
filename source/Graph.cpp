#include "include/Graph.h"
#include <fstream>
#include <math.h>
#include "time.h"
#include <algorithm>

using namespace std;

Graph::Graph(){

    this->verticesList = NULL;
    this->verticesSize = 0;
    this->edgesSize = 0;
    this->totalActiveLoss = 0.0;
    this->totalReactiveLoss = 0.0;
    this->numberOfMarked = 0;
}

Graph::~Graph(){
    Vertex *aux = new Vertex(0);
    for(Vertex *no=this->verticesList; no != NULL; no= no->getNext()){
        delete aux;
        for(Edge *arco= no->getEdgesList(); arco != NULL; arco= arco->getNext()){
            delete arco;
        }
        aux = no;
    }
    delete aux;
}

Vertex *Graph::findVertex(int id){

    Vertex *no = verticesList;
    while(no!=NULL){
        if(no->getID() == id)
            return no;
        no = no->getNext();
    }
    return NULL;

}

Edge *Graph::findEdge(int id){
    for(Vertex *no= get_verticesList(); no != NULL; no= no->getNext()){
        for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){
            if(a->getID() == id)
                return a;
        }
    }
    return NULL;
}

Edge *Graph::findEdge(int originID, int destinyID){
    for(Vertex *no= get_verticesList(); no != NULL; no= no->getNext()){
        for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){
            if(a->getOrigin()->getID() == originID && a->getDestiny()->getID() == destinyID)
                return a;
        }
    }
    return NULL;
}

void Graph::createCapacitorType(int id, double reactive_power, double cost_per_KVAr, int step){
    cout << "Capacitor Created with pot: " << reactive_power/(PB*1000) << endl;
    double cost = cost_per_KVAr * reactive_power;
    this->capacitorType.insert( this->capacitorType.begin(), Capacitor( id, reactive_power/(PB*1000), cost ) );
};

void Graph::show_losses( double powerLoss, double minTension, int idLoad ){

    if(idLoad == -1) {
        double total_time = 0.0;
        for(int i = 0; i < 3; i++)
            total_time += this->loads[i].time;
        printf(" Total \t\t|  %9.6f \t| \t%9.2f US$ \t|\t  -  \n", powerLoss, this->getLosses()[1]  );
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
            insertEdge(originId, destinyId, edgeId, resistance, reactance, true);
            insertEdge(destinyId, originId, edgeId, resistance, reactance, true);
        }
        else
            input >> aux;
    }
    cout << "\n" << name << " lida!" << endl;
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

void Graph::capacitor_allocation(){

    int idLoad = 1;
    double capacitor_cost = this->getCapacitorType(1).getCost();   // Custo do capacitor em kW

    this->evaluateLossesAndFlows(1e-12, idLoad);                   // Avalia no nível de carga mais pesado?

    double * last_loss_cost = new double[2];
    last_loss_cost[0] = this->getLosses()[0];
    last_loss_cost[1] = this->getLosses()[1];
    double cost_change = capacitor_cost*2;

//    uint iter = 0;
    Vertex * alloc_vertex ;
    while( capacitor_cost < cost_change ) {
        alloc_vertex = this->greedyCapacitorAllocation();
//        cout << "Vertice cap: " << alloc_vertex->getID() << endl;
        alloc_vertex->addCapacitor( this->getCapacitorType(1) );
        this->evaluateLossesAndFlows(1e-12, idLoad);

        cost_change = last_loss_cost[1] - this->getLosses()[1];
//        cout << "  Total Loss:  " << graph->getTotalLoss()[0] << "KW"  << endl;
//        cout << "  Total Cost:  " << graph->getTotalLoss()[1] << "US$"  << endl;
//        cout << "  Loss Change: " << last_loss_cost[0] - this->getTotalLoss()[0] << "KW"  << endl;
//        cout << "  Cost Change: " << cost_change  << " US$"     << endl;
        last_loss_cost[0] = this->getLosses()[0];
        last_loss_cost[1] = this->getLosses()[1];
//        iter++;
    }
    alloc_vertex->rmLastCapacitor();
//    iter--;
//    cout << "Num caps: " << iter << endl;
}

void Graph::insertVertex(int id, double activePower, double reactivePower, double voltage, int idLoadFactor){

    Vertex *no=new Vertex(id);
    no->setActivePower(activePower);
    no->setReactivePower(reactivePower);
    no->setVoltage(voltage);

    //insere No no inicio da lista
    no->setNext(verticesList);
    this->verticesList=no;

    this->verticesSize++;

}

void Graph::insertEdge(int originID, int destinyID, int id, double resist, double react, bool swit){
    Vertex *noOrigem = findVertex(originID);
    Vertex *noDestino = findVertex(destinyID);

    Edge *novaArco = new Edge(id);
    novaArco->setDestiny(noDestino);
    novaArco->setOrigin(noOrigem);

    //insere Arco no inicio da lista
    novaArco->setNext(noOrigem->getEdgesList());
    noOrigem->setEdgesList(novaArco);

    novaArco->setResistance(resist);
    novaArco->setReactance(react);
    novaArco->setSwitch(swit);


    noOrigem->setOutdegree(noOrigem->getIndegree() + 1);
    noDestino->setIndegree(noDestino->getOutdegree() + 1);

    this->edgesSize++;
}

void Graph::leEntrada(char nome[])
{

    int idArco = 0;
    double VB, PB, ZB, fator_MW = 1e-3;
    double carga, resistencia, reatancia, potencia_reativa, voltagem;

    ifstream entrada;
    entrada.open(nome);
    string aux;

    int n_col_nos=13, n_col_arestas=12;//numero de colunas do arquivo de entrada

    do{
        entrada >> aux;
    }while(aux != "Vb");
    entrada >> aux;
    entrada >> VB;
    entrada >> aux;entrada >> aux;
    entrada >> PB;
    entrada >> aux;entrada >> aux;
    entrada >> ZB;


    entrada.close();
    entrada.open(nome);
    int idNo, idOrig, idDest;


    do{
        entrada >> aux;
    }while(aux != "num_nos");///pula lixo ate chegar na parte de informacoes do no

    int num_nos;
    entrada >> num_nos;
    for(int i=1 ; i<=num_nos*n_col_nos; i++){
        if(i % n_col_nos == 1){
            entrada >> idNo;
//            cout << "idNo: " << idNo;
        }
        else if(i % n_col_nos == 4){
            entrada >> voltagem;
//            cout << "      V: " << voltagem;
        }
        else if(i % n_col_nos == 10){
            entrada >> carga;
            carga /= PB;
            carga *=fator_MW;
//            cout << "      pot at=carga: " << carga;
        }
        else if(i % n_col_nos == 11){
            entrada >> potencia_reativa;
            potencia_reativa /=PB;
            potencia_reativa *= fator_MW;
//            cout << "      pot reat: " << potencia_reativa << endl;
            insertVertex(idNo, carga, potencia_reativa, voltagem);
        }
        else
            entrada >> aux;
    }

    do{
        entrada >> aux;
    }while(aux != "num_arestas");///pula mais lixo ate chegar na parte de informacoes da aresta

    int num_arestas;
    entrada >> num_arestas;

    for(int i=1 ; i<=num_arestas*n_col_arestas; i++){
        if(i % n_col_arestas == 1){
            entrada >> idOrig;
//            cout << "idOrig: " << idOrig;
        }
        else if(i % n_col_arestas == 2){
            entrada >> idDest;
//            cout << "      idDest: " << idDest;
        }
        else if(i % n_col_arestas == 3){
            entrada >> resistencia;
            resistencia /= ZB;
//            cout << "      r: " << resistencia;
        }
        else if(i % n_col_arestas == 4){
            entrada >> reatancia;
            reatancia /= ZB;
//            cout << "      reat: " << reatancia;
            //insere arco i-j e j-i

            idArco++;
            insertEdge(idOrig, idDest, idArco, resistencia, reatancia, false);
            insertEdge(idDest, idOrig, idArco, resistencia, reatancia, false);
//            cout << "  inseriu arco!" << endl;
        }
        else
            entrada >> aux;
    }

//    cout << "\n" << nome << " lida!" << endl;
}

void Graph::show(){
    printf("\n\n\nGRAFO  num_Nos: %d    num_arcos: %d \n\n\n", this->verticesSize, this->edgesSize);
    for(Vertex *no = this->verticesList; no != NULL; no = no->getNext())
        no->show();
}

double Graph::branchActiveLoss(Vertex *no){
    double soma = no->getActivePower();
    auxBranchActiveLoss(no, soma);
    return soma;
}

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

double Graph::branchReactiveLoss(Vertex *no){
    double soma= no->getReactivePower();
    auxBranchReactiveLoss(no, soma);
    return soma;
}

void Graph::auxBranchReactiveLoss(Vertex *no, double &soma){
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

        soma+= a->getDestiny()->getReactivePower() + a->getReactiveLoss();
        auxBranchReactiveLoss(a->getDestiny(), soma);
    }
}

//FOWARD
void Graph::forward(int it){
    this->verticesList->setVoltage(1.0);//voltagem controlada na estacao
    auxForward(this->verticesList, this->verticesList->getEdgesList(), it);
}

void Graph::auxForward(Vertex *no, Edge *ak, int it){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){

            double perda_ativ = 0.0;
            double perda_reat = 0.0;

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

            //----foward----

            //chute inicial para o fluxo nas arests que partem do no terminal
            if(no==this->verticesList){
                double carcasPerdasAtivRamo = branchActiveLoss(a->getDestiny());
                double carcasPerdasReativRamo = branchReactiveLoss(a->getDestiny());

                a->setActiveFlow(carcasPerdasAtivRamo + a->getActiveLoss());
                a->setReactiveFlow(carcasPerdasReativRamo + a->getReactiveLoss());

            }else{

                //a partir da primeira iteracao considerase a perda calculada na iteracao anterior
                if(it>0){
                    perda_ativ = ak->getResistance() * (pow(ak->getActivePowerFlow(), 2) + pow(
                            ak->getReactivePowerFlow(), 2)) / pow(
                            ak->getOrigin()->getVoltage(), 2);
                    perda_reat = ak->getReactance() * (pow(ak->getActivePowerFlow(), 2) + pow(ak->getReactivePowerFlow(), 2)) / pow(
                            ak->getOrigin()->getVoltage(), 2);
                }

                a->setActiveFlow(ak->getActivePowerFlow() - perda_ativ - no->getActivePower());
                a->setReactiveFlow(ak->getReactivePowerFlow() - perda_reat - no->getReactivePower());

                //bifurcacao - o fluxo que seque para o arco 'a' deve-se subtrair a soma de cargas e perdas dos ramos de bifurcacoes
                if(a->getOrigin()->getIndegree() > 1){
                    double somaAtiv=0.0, somaReAtiv=0.0;
                    for(Edge *aux= no->getEdgesList(); aux != NULL; aux= aux->getNext()){
                        if(aux!=a && aux->isClosed() == true){
                            somaAtiv+= aux->getActiveLoss() + branchActiveLoss(aux->getDestiny());
                            somaReAtiv+= aux->getReactiveLoss() + branchReactiveLoss(aux->getDestiny());
                        }
                    }

                    a->setActiveFlow(a->getActivePowerFlow() - somaAtiv);
                    a->setReactiveFlow(a->getReactivePowerFlow() - somaReAtiv);
                }
            }
            //--------------

            auxForward(a->getDestiny(), a, it);
        }
    }
}

void Graph::backward(){
    this->verticesList->setVoltage(1.0);//voltagem controlada na estacao
    auxBackward(this->verticesList);
}

void Graph::auxBackward(Vertex *no){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
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

            //----backward----
            Vertex *noDest = a->getDestiny();
            Vertex *noOrig = a->getOrigin();

            noDest->setVoltage(pow(noOrig->getVoltage(), 2)
                               - 2 * (a->getResistance() * a->getActivePowerFlow() + a->getReactance() *
                                                                                     a->getReactivePowerFlow()) +
                               (pow(a->getResistance(), 2) + pow(a->getReactance(), 2)) *
                               (pow(a->getActivePowerFlow(), 2) + pow(a->getReactivePowerFlow(), 2)) / pow(
                                       noOrig->getVoltage(), 2));

            noDest->setVoltage(sqrt(noDest->getVoltage()));

            a->setActiveLoss(a->getResistance() * (pow(a->getActivePowerFlow(), 2) + pow(a->getReactivePowerFlow(), 2))
                             / pow(noOrig->getVoltage(), 2));
            a->setReactiveLoss(a->getReactance() * (pow(a->getActivePowerFlow(), 2) + pow(a->getReactivePowerFlow(), 2))
                               / pow(noOrig->getVoltage(), 2));
            //-------------

            auxBackward(a->getDestiny());
        }
    }
}

double *Graph::getLosses(){
    double *perda = new double[2]; perda[0] = perda[1] = 0.0;
    for(Vertex *no=verticesList; no != NULL; no = no->getNext()){
        for(Edge *a = no->getEdgesList(); a != NULL; a = a->getNext()){
            perda[0] += a->getActiveLoss();
            perda[1] += a->getReactiveLoss();
        }
    }
    return perda;
}

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

void Graph::unmarkVertices(){
    for(Vertex *i = this->verticesList; i != NULL; i = i->getNext())
        i->setMarked(false);
    this->numberOfMarked = 0;
}

bool Graph::isConected(){
    this->unmarkVertices();
    int n_marcados = 0;
    auxIsConnected(this->verticesList, n_marcados);

    if(n_marcados == this->verticesSize)
        return true;
    else
        return false;
}

void Graph::auxIsConnected(Vertex *no, int &n_marcados){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        if(no->getMarked() == false){
            no->setMarked(true);
            n_marcados++;

            for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){

                //nao descer por arcos com chave aberta
                while(a!=NULL && a->isClosed() == false){
                    a = a->getNext();
                }

                if(a==NULL)
                    break;

                auxIsConnected(a->getDestiny(), n_marcados);
            }
        }
    }
}

bool Graph::isTree(){
    unmarkVertices();

    bool ciclo = false;
    int marcados = 0;

    auxIsTree(verticesList, marcados, ciclo);

    if(ciclo==true)
        cout << "\nFECHOU CICLO!!" << endl;
    if(marcados < verticesSize)
        cout << "\nARVORE NAO COBRE TODOS OS NOS!!";

    if(ciclo==false && marcados == verticesSize)
        return true;
    else
        return false;
}

void Graph::auxIsTree(Vertex *no, int &marcados, bool &ciclo){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        if(no->getMarked() == false){
            no->setMarked(true);
            marcados++;

            for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){

                //nao descer por arcos com chave aberta
                while(a!=NULL && a->isClosed() == false){
                    a = a->getNext();
                }

                if(a==NULL)
                    break;

                auxIsTree(a->getDestiny(), marcados, ciclo);
            }
        }
        else
            ciclo = true;
    }
}

double Graph::minVoltage(){
    double tensao_mim = 1.0;
    for(Vertex *no=this->verticesList; no != NULL; no= no->getNext()){
        if(no->getVoltage() < tensao_mim)
            tensao_mim = no->getVoltage();
    }
    return tensao_mim;
}

void Graph::defineFlows(){
    auxDefineFlows(this->verticesList, this->verticesList);
//    printf("\nsentido de fluxo das arestas definido!\n");
}

void Graph::auxDefineFlows(Vertex *no, Vertex *noAnterior){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){

            //nao descer por arcos com chave aberta e descer no sentido correto
            //a->getNoDestino()==no e o arco de volta do arco 'a' assim nao passamos por ele
            while(a!=NULL && (a->isClosed() == false || a->getDestiny() == noAnterior) ){

                //nao tem fluxo nem perda em arcos abertos
                a->setActiveFlow(0.0);
                a->setReactiveFlow(0.0);
                a->setActiveLoss(0.0);
                a->setReactiveLoss(0.0);

                if(a->getDestiny() == noAnterior)
                    a->setSwitch(false);

                a = a->getNext();
            }
            if(a==NULL)
                break;

//            printf("\nno{%d}", no->getID());
            auxDefineFlows(a->getDestiny(), no);
        }
    }
}

void Graph::defineModifiables(){

    this->resetModifiables();//define todos os arcos como sendo modificaveis
    int n = 0;
    for(int i=0; i< this->edgesSize / 2; i++){


        //procura arcos que conectam nos folha, esses arcos nao sao modificaveis(nao podem ser abertos)
        for(Vertex *no=this->verticesList; no != NULL; no= no->getNext()){
            for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){

                if(a->getDestiny()->getAuxDegree() == 1 && a->isClosed() == true && a->getModifiable() == true){

//                    printf("\ndefinindo como nao modif A{%d}...", a->getID());

                    a->setModifiable(false);
                    a->setSwitch(true);//nao modificavel: chave sempre fechad

                    Edge *aVolta = this->findEdge(a->getDestiny()->getID(), a->getOrigin()->getID());
                    aVolta->setModifiable(false);

                    a->getOrigin()->setAuxDegree(a->getOrigin()->getAuxDegree() - 1);
                    a->getDestiny()->setAuxDegree(a->getDestiny()->getAuxDegree() - 1);

                    //arcos nao modificaveis ficam sempre fechados


//                    printf("done!");

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
    int idOrig, idDest, idArco, idNo;
    double resistencia, reatancia, potAtiva, potReativa, voltagem;

    for(Vertex *no= this->verticesList->getNext(); no != NULL; no= no->getNext()){
        idNo = no->getID();
        potAtiva = no->getActivePower();
        potReativa = no->getReactivePower();
        voltagem = no->getVoltage();

        g->insertVertex(idNo, potAtiva, potReativa, voltagem);
    }

    //inserindo no fonte por ultimo pos a funcao que insere nos insere sempre no inicio da lista, assim o no fonte fica no inicio da lista
    Vertex *noFonte = this->verticesList;
    g->insertVertex(noFonte->getID(), noFonte->getActivePower(), noFonte->getReactivePower(), noFonte->getVoltage());

    for(Vertex *no=this->verticesList; no != NULL; no= no->getNext()){
         for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){
            idOrig = a->getOrigin()->getID();
            idDest = a->getDestiny()->getID();
            idArco = a->getID();
            resistencia = a->getResistance();
            reatancia = a->getReactance();

             g->insertEdge(idOrig, idDest, idArco, resistencia, reatancia, a->isClosed());
        }
    }

    return g;
}

/*Algoritmo de Kruskal randomizado*/
void Graph::randomSolution(){
    vector<Edge*> vetArcos;
    for(Vertex *no = this->get_verticesList(); no != NULL; no = no->getNext()){
        for(Edge *a = no->getEdgesList(); a != NULL; a = a->getNext()){
            a->setSwitch(false);
            vetArcos.push_back(a);
        }
    }

    random_shuffle(vetArcos.begin(), vetArcos.end());

    int n_arc_inseridos = 0, n_arcos_inserir = this->verticesSize - 1;
    for(int i=0; n_arc_inseridos<n_arcos_inserir; i++){

        if((vetArcos.at(i)->getOrigin()->getIdTree() != vetArcos.at(i)->getDestiny()->getIdTree()) &&
                vetArcos.at(i)->isClosed() == false){

            int id = vetArcos.at(i)->getOrigin()->getIdTree();
            for(Vertex *no = this->verticesList; no != NULL; no = no->getNext()){
                if(no->getIdTree() == id)
                    no->setIdTree(vetArcos.at(i)->getDestiny()->getIdTree());
            }

            vetArcos.at(i)->setSwitch(true);
            this->findEdge(vetArcos.at(i)->getDestiny()->getID(), vetArcos.at(i)->getOrigin()->getID())->setSwitch(
                    true);

            n_arc_inseridos++;
        }

    }

    printf("  Aberto:{");
    for(Vertex *no = this->verticesList; no != NULL; no = no->getNext()){
        for(Edge *a = no->getEdgesList(); a != NULL; a = a->getNext()){
            if(a->isClosed() == false)
                printf("%d,", a->getID());
        }
    }
    printf("  }");
}

void Graph::resetAuxDegrees(){
    for(Vertex *no = this->verticesList; no != NULL; no = no->getNext()){
        for(Edge *a = no->getEdgesList(); a != NULL; a = a->getNext()){
            Vertex *orig, *dest;
            orig = a->getOrigin();
            dest = a->getDestiny();

            orig->setAuxDegree(orig->getOutdegree());
            dest->setAuxDegree(dest->getOutdegree());
        }
    }
}

void Graph::resetIDTree(){
    for(Vertex *no = this->verticesList; no != NULL; no = no->getNext()){
        no->setIdTree(no->getID());
    }
}

void Graph::resetFlowAndLoss(){
    for(Vertex *no=this->verticesList; no != NULL; no= no->getNext()){
        for(Edge *a= no->getEdgesList(); a != NULL; a= a->getNext()){
            a->setActiveFlow(0.0);
            a->setReactiveFlow(0.0);
            a->setActiveLoss(0.0);
            a->setReactiveLoss(0.0);
        }
    }
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
