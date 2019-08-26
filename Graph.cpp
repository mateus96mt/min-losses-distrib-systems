#include "Graph.h"
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
    this->num_markeds = 0;
    this->maxCapacitorsBus = 0;
    this->maxCapacitorNetwork = 0;
}

Graph::~Graph(){
    Vertex *aux = new Vertex(0);
    for(Vertex *no=this->verticesList; no!=NULL; no= no->getNext()){
        delete aux;
        for(Edge *arco= no->getEdgesList(); arco!=NULL; arco= arco->getNext()){
            delete arco;
        }
        aux = no;
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

Edge *Graph::findEdge( int originID, int destinyID ){
    for(Vertex *v= get_verticesList(); v!=NULL; v= v->getNext()){
        for(Edge *e= v->getEdgesList(); e!=NULL; e= e->getNext()){
            if(e->getOrigin()->getID() == originID && e->getDestiny()->getID() == destinyID)
                return e;
        }
    }
    return NULL;
}

void Graph::insertVertex(int id, double activePower, double reactivePower, double voltage){

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
    Vertex *originVertex = findVertex(originID);
    Vertex *destinyVertex = findVertex(destinyID);

    Edge *newEdge = new Edge(id);
    newEdge->setDestiny(destinyVertex);
    newEdge->setOrigin(originVertex);

    //insere Arco no inicio da lista
    newEdge->setNext(originVertex->getEdgesList());
    originVertex->setEdgesList(newEdge);

    newEdge->setResistance(resist);
    newEdge->setReactance(react);
    newEdge->setSwitch(swit);


    originVertex->setOutdegree(originVertex->getIndegree() + 1);
    destinyVertex->setIndegree(destinyVertex->getOutdegree() + 1);

    this->edgesSize++;
}

void Graph::input_read(string name){

    cout << name << endl;
    int idArco = 0;
    double VB, PB, ZB, mw_factor = 1e-3;
    double power, resistance, reactance, reactive_power, voltage;

    ifstream input;
    input.open(name);
//    string line;
//    while( getline(input, line) ) cout << line << endl;

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
    int idNo, idOrig, idDest;

    do{
        input >> aux;
    }while(aux != "num_nos");///pula lixo ate chegar na parte de informacoes do no

    int num_nos;
    input >> num_nos;
    for(int i=1 ; i<=num_nos*n_col_v; i++){
        if(i % n_col_v == 1){
            input >> idNo;
//            cout << "idNo: " << idNo;
        }
        else if(i % n_col_v == 4){
            input >> voltage;
//            cout << "      V: " << voltage;
        }
        else if(i % n_col_v == 10){
            input >> power;
            power /= PB;
            power *= mw_factor;
//            cout << "      pot at=power: " << power;
        }
        else if(i % n_col_v == 11){
            input >> reactive_power;
            reactive_power /=PB;
            reactive_power *= mw_factor;
//            cout << "      pot reat: " << reactive_power << endl;
            insertVertex(idNo, power, reactive_power, voltage);
        }
        else
            input >> aux;
    }

    do{
        input >> aux;
    }while(aux != "num_arestas");///pula mais lixo ate chegar na parte de informacoes da aresta

    int num_arestas;
    input >> num_arestas;

    for(int i=1 ; i<=num_arestas*n_col_e; i++){
        if(i % n_col_e == 1){
            input >> idOrig;
//            cout << "idOrig: " << idOrig;
        }
        else if(i % n_col_e == 2){
            input >> idDest;
//            cout << "      idDest: " << idDest;
        }
        else if(i % n_col_e == 3){
            input >> resistance;
            resistance /= ZB;
//            cout << "      r: " << resistance;
        }
        else if(i % n_col_e == 4){
            input >> reactance;
            reactance /= ZB;
//            cout << "      reat: " << reactance << endl;;
            //insere arco i-j e j-i

            idArco++;
            insertEdge(idOrig, idDest, idArco, resistance, reactance, true);
            insertEdge(idDest, idOrig, idArco, resistance, reactance, true);
        }
        else
            input >> aux;
    }
    cout << "\n" << name << " lida!" << endl;
}

void Graph::printGraph(){
    printf("\n\n\nGRAFO  num_Nos: %d    num_arcos: %d \n\n\n", this->verticesSize, this->edgesSize);
    for(Vertex *no = this->verticesList; no!=NULL; no = no->getNext())
        no->show();
}

double Graph::branchActiveLoss(Vertex *no){
    double soma = no->getActivePower();
    auxBranchActiveLoss(no, soma);
    return soma;
}

void Graph::auxBranchActiveLoss(Vertex *no, double &soma){
    for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){

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
    for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){

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
        for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){

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

            //----forward----

            //chute inicial para o fluxo nas arests que partem do no terminal
            if(no==this->verticesList){
                double carcasPerdasAtivRamo = branchActiveLoss(a->getDestiny());
                double carcasPerdasReativRamo = branchReactiveLoss(a->getDestiny());

                a->setActiveFlow(carcasPerdasAtivRamo + a->getActiveLoss());
                a->setReactiveFlow(carcasPerdasReativRamo + a->getReactiveLoss());

            }else{

                //a partir da primeira iteracao considerase a perda calculada na iteracao anterior
                if(it>0){
                    perda_ativ = ak->getResistance()*(pow(ak->getActivePowerFlow(), 2) + pow(ak->getReactivePowerFlow(), 2)) / pow(
                            ak->getOrigin()->getVoltage(), 2);
                    perda_reat =
                            ak->getReactance()*(pow(ak->getActivePowerFlow(), 2) + pow(ak->getReactivePowerFlow(), 2)) / pow(
                                    ak->getOrigin()->getVoltage(), 2);
                }

                a->setActiveFlow(ak->getActivePowerFlow() - perda_ativ - no->getActivePower());
                a->setReactiveFlow(ak->getReactivePowerFlow() - perda_reat - no->getReactivePower());

                //bifurcacao - o fluxo que seque para o arco 'a' deve-se subtrair a soma de cargas e perdas dos ramos de bifurcacoes
                if(a->getOrigin()->getIndegree()>1){
                    double somaAtiv=0.0, somaReAtiv=0.0;
                    for(Edge *aux= no->getEdgesList(); aux!=NULL; aux= aux->getNext()){
                        if(aux!=a && aux->isClosed()==true){
                            somaAtiv+= aux->getActiveLoss() + branchActiveLoss(aux->getDestiny());
                            somaReAtiv+= aux->getReactiveLoss() + branchReactiveLoss(aux->getDestiny());
                        }
                    }

                    a->setActiveFlow(a->getActivePowerFlow() - somaAtiv);
                    a->setReactiveFlow(a->getReactivePowerFlow() - somaReAtiv);
                }
            }

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
        for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){

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

            noDest->setVoltage(pow(noOrig->getVoltage(), 2) - 2 * (a->getResistance() * a->getActivePowerFlow() +
                    a->getReactance() * a->getReactivePowerFlow()) + (pow(a->getResistance(), 2) + pow(a->getReactance(), 2)) *
                               (pow(a->getActivePowerFlow(), 2) + pow(a->getReactivePowerFlow(), 2)) / pow(noOrig->getVoltage(), 2));

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

void Graph::evaluateLossesAndFlows(double tol){
    double *last_total_loss, *current_total_loss , error = 1.0;

    last_total_loss = this->getLosses();
    for( int it = 0; error>tol; it++ ){
        forward(it);        // Calcula fluxos de potencia nas linhas
        backward();         // Calcula voltagem nas barras e perdas nas linhas

        current_total_loss = this->getLosses();
        error = current_total_loss[0] - last_total_loss[0];
        delete last_total_loss;
//        cout << "\n\nPerda total: (" << current_total_loss[0] << " , " << current_total_loss[1] << ")" << endl;

        last_total_loss = current_total_loss;
    }
}

void Graph::unmarkVertices(){
    for(Vertex *i = this->verticesList; i != NULL; i = i->getNext())
        i->setMarked(false);
    this->num_markeds = 0;
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

            for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){

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
    if(marcados<verticesSize)
        cout << "\nARVORE NAO COBRE TODOS OS NOS!!";

    if(ciclo==false && marcados==verticesSize)
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

            for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){

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
    for(Vertex *no=this->verticesList; no!=NULL; no= no->getNext()){
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
        for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){

            //nao descer por arcos com chave aberta e descer no sentido correto
            //a->getNoDestino()==no e o arco de volta do arco 'a' assim nao passamos por ele
            while(a!=NULL && (a->isClosed() == false || a->getDestiny()==noAnterior) ){

                //nao tem fluxo nem perda em arcos abertos
                a->setActiveFlow(0.0);
                a->setReactiveFlow(0.0);
                a->setActiveLoss(0.0);
                a->setReactiveLoss(0.0);

                if(a->getDestiny()==noAnterior)
                    a->setSwitch(false);

                a = a->getNext();
            }
            if(a==NULL)     break;

            auxDefineFlows(a->getDestiny(), no);
        }
    }
}

void Graph::defineModifiables(){

    int n=1;
    for(int i=0; i<this->edgesSize/2; i++){


        //procura arcos que conectam nos folha, esses arcos nao sao modificaveis(nao podem ser abertos)
        for(Vertex *no=this->verticesList; no!=NULL; no= no->getNext()){
            for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){

                if(a->getDestiny()->getAuxDegree()==1 && a->isClosed()==true && a->getModifiable()==true){

//                    printf("\ndefinindo como nao modif A{%d}...", a->getID());

                    a->setModifiable(false);
                    Edge *aVolta = this->findEdge(a->getDestiny()->getID(), a->getOrigin()->getID());
                    aVolta->setModifiable(false);

                    a->getOrigin()->setAuxDegree(a->getOrigin()->getAuxDegree() - 1);
                    a->getDestiny()->setAuxDegree(a->getDestiny()->getAuxDegree() - 1);

//                    printf("done!");

                    n++;
                }
            }
        }
    }
//    printf("\ndefiniu arestas modificaveis!");
}

Graph *Graph::returnCopy(){
    Graph *g = new Graph();
    int idOrig, idDest, idArco, idNo;
    double resistencia, reatancia, potAtiva, potReativa, voltagem;

    for(Vertex *no= this->verticesList->getNext(); no!=NULL; no= no->getNext()){
        idNo = no->getID();
        potAtiva = no->getActivePower();
        potReativa = no->getReactivePower();
        voltagem = no->getVoltage();

        g->insertVertex(idNo, potAtiva, potReativa, voltagem);
    }

    //inserindo no fonte por ultimo pos a funcao que insere nos insere sempre no inicio da lista, assim o no fonte fica no inicio da lista
    Vertex *noFonte = this->verticesList;
    g->insertVertex(noFonte->getID(), noFonte->getActivePower(), noFonte->getReactivePower(), noFonte->getVoltage());

    for(Vertex *no=this->verticesList; no!=NULL; no= no->getNext()){
         for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){
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
    for(Vertex *no = this->get_verticesList(); no!=NULL; no = no->getNext()){
        for(Edge *a = no->getEdgesList(); a!=NULL; a = a->getNext()){
            a->setSwitch(false);
            vetArcos.push_back(a);
        }
    }

    random_shuffle(vetArcos.begin(), vetArcos.end());

    int n_arc_inseridos = 0, n_arcos_inserir = this->verticesSize-1;
    for(int i=0; n_arc_inseridos<n_arcos_inserir; i++){

        if( (vetArcos.at(i)->getOrigin()->getIdTree() != vetArcos.at(i)->getDestiny()->getIdTree()) &&
                vetArcos.at(i)->isClosed()==false){

            int id = vetArcos.at(i)->getOrigin()->getIdTree();
            for(Vertex *no = this->verticesList; no!=NULL; no = no->getNext()){
                if(no->getIdTree()==id)
                    no->setIdTree(vetArcos.at(i)->getDestiny()->getIdTree());
            }

            vetArcos.at(i)->setSwitch(true);
            this->findEdge(vetArcos.at(i)->getDestiny()->getID(), vetArcos.at(i)->getOrigin()->getID())->setSwitch(true);

            n_arc_inseridos++;
        }

    }

    printf("  Aberto:{");
    for(Vertex *no = this->verticesList; no!=NULL; no = no->getNext()){
        for(Edge *a = no->getEdgesList(); a!=NULL; a = a->getNext()){
            if(a->isClosed()==false)
                printf("%d,", a->getID());
        }
    }
    printf("  }");
}

void Graph::resetAuxDegrees(){
    for(Vertex *no = this->verticesList; no!=NULL; no = no->getNext()){
        for(Edge *a = no->getEdgesList(); a!=NULL; a = a->getNext()){
            Vertex *orig, *dest;
            orig = a->getOrigin();
            dest = a->getDestiny();

            orig->setAuxDegree(orig->getOutdegree());
            dest->setAuxDegree(dest->getOutdegree());
        }
    }
}

void Graph::resetIDTree(){
    for(Vertex *no = this->verticesList; no!=NULL; no = no->getNext()){
        no->setIdTree(no->getID());
    }
}

void Graph::resetFlowAndLoss(){
    for(Vertex *no=this->verticesList; no!=NULL; no= no->getNext()){
        for(Edge *a= no->getEdgesList(); a!=NULL; a= a->getNext()){
            a->setActiveFlow(0.0);
            a->setReactiveFlow(0.0);
            a->setActiveLoss(0.0);
            a->setReactiveLoss(0.0);
        }
    }
}
