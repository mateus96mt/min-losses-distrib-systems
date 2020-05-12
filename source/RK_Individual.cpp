#include "include/RK_Individual.h"
#define RANGEPESO 100

using namespace std;

vector<Chromosome*> RK_Individual::cromossomos = vector<Chromosome*>();
bool sortChromosome(Chromosome *c1, Chromosome *c2){return c1->weight > c2->weight;}

bool ordenaPosicaoCromossomo(Chromosome *c1, Chromosome *c2){ return c1->position < c2->position;}
bool ordenaCromossomoPorIdArco(Chromosome *c1, Chromosome *c2){ return c1->edge->getID() < c2->edge->getID(); }

RK_Individual::RK_Individual(int numEdges){
     this->number_of_edges = numEdges;
     weights = new double[ this->number_of_edges ];
}

void RK_Individual::generate_random_weights(){
    for(int i=0; i<this->number_of_edges; i++)
        this->weights[i] = rand() % RANGEPESO;
}

void RK_Individual::calculate_fitness(Graph *graph , bool theBest ){
    vector<Chromosome*> chromosomes;


    Chromosome *chromosome;
    for( Vertex *vertex = graph->get_verticesList(); vertex!=NULL; vertex = vertex->getNext() ){
        for(Edge *e = vertex->getEdgesList(); e!=NULL; e= e->getNext()){
		if(e->getModifiable() == true && e->getMarked() == true){
            chromosome = new Chromosome();
            chromosome->edge = e;
            chromosome->weight = 0;
            chromosomes.push_back(chromosome);
	        }
	}

    }

    for(int i=0; i < this->number_of_edges; i++) /// Copia peso do individio paca cada cromossomo
        chromosomes[i]->weight = this->weights[i];

    sort(chromosomes.begin(), chromosomes.end(), sortChromosome);

    int inserted_size = 0, toInsert_size = graph->getVerticesSize()-1- graph->getNumberOfNonModifiable();


    for(Vertex *vertex = graph->get_verticesList(); vertex!=NULL; vertex = vertex->getNext()){
        for(Edge *e = vertex->getEdgesList(); e!=NULL; e = e->getNext()){

            //arcos nao modificaveis ficam sempre fechados
            if(e->getModifiable() == false)
                e->setSwitch(true);
            else
                e->setSwitch(false);

            e->setActiveFlow(0.0);
            e->setReactiveFlow(0.0);
            e->setActiveLoss(0.0);
            e->setReactiveLoss(0.0);
        }
    }

    graph->resetIDTree();                               // reseta os ids de componentes conexas para algoritmo de Kruskal
    for(int i=0; inserted_size<toInsert_size; i++){     // percorre vetor de cromossomos ordenados e tenta fechar chave(algoritmo de kruskal)
        if( (chromosomes.at(i)->edge->getOrigin()->getIdTree() != chromosomes.at(i)->edge->getDestiny()->getIdTree()) &&
                chromosomes.at(i)->edge->isClosed()==false){
            int id = chromosomes.at(i)->edge->getOrigin()->getIdTree();
            for(Vertex *v = graph->get_verticesList(); v!=NULL; v = v->getNext()){
                if(v->getIdTree()==id)
                    v->setIdTree(chromosomes.at(i)->edge->getDestiny()->getIdTree());
            }
            chromosomes.at(i)->edge->setSwitch(true);
            graph->findEdge(chromosomes.at(i)->edge->getDestiny()->getID(), chromosomes.at(i)->edge->getOrigin()->getID())->setSwitch(true);
            inserted_size++;
        }
    }

    graph->defineFlows();

    graph->capacitor_allocation();          // TODO: trocar calculo da funcao objetivo para o custo total (todos niveis) em dinheiro US$

    graph->evaluateLossesAndFlows(1e-8);    // calcula fluxos e perda com erro de 1e-5

    double *losses = graph->getLosses();    // soma as perdas ativas e reativas em todos os arcos e retorna um double

    this->active_loss           = losses[0];
    this->reactive_loss         = losses[1];

    // No caso de solucoes que possuem configuracao viavel porem sao muito longas
    if( isnan(losses[0]) )      this->active_loss   = 9999999999;
    if( isnan(losses[1]) )      this->reactive_loss = 9999999999;

    this->objective_function    = graph->getTotalLoss()[1];
    if(isnan( this->objective_function ) )    this->objective_function = 9999999999;

//    cout << "Perda Total:    " << graph->getTotalLoss()[0] << endl;
//    cout << "Custo Total:    " << graph->getTotalLoss()[1] << endl;

    delete losses;

    if( !theBest )
    for (Vertex *vertex_caps = graph->get_verticesList(); vertex_caps != NULL; vertex_caps = vertex_caps->getNext()) {
        vertex_caps->rm_all_capacitors();
    }

    chromosomes.clear();    // Deletar vetor de cromossomos(nao esta deletando memoria)
}

void RK_Individual::resetaPesos(float valor){
    for(int i=0; i< this->getNumEdges(); i++)
        this->weights[i] = valor;
}

void RK_Individual::geraPesosConfInicial(int *idsAbertos, int n, Graph *g){

//    cout << "vai gerar conf inicial:\n--------------------------------" << endl;

//    cout << "idsAberto = {" << endl;
//    for(int i=0; i<n; i++)
//        cout << idsAbertos[i] << endl;
//    cout << "}" << endl;

    this->resetaPesos(1.0);
    int j=0;
    for(Vertex *no = g->get_verticesList(); no != NULL; no= no->getNext()){

        for(Edge *a = no->getEdgesList(); a != NULL; a= a->getNext()){

            if(a->getModifiable() == true && a->getMarked() == true){
                for(int i=0; i<n; i++){
                    if(a->getID()==idsAbertos[i]){
                        this->weights[j] = 0.0;
//                            cout << "A{" <<a->getID() << "}" << endl;
                    }
                }
                j++;
            }

        }
    }
//    cout << "--------------------------------" << endl;
}

void RK_Individual::imprimePesos(){
    printf("pesos {");
    for(int i=0; i<this->getNumEdges(); i++)
        printf("%.2f, ", this->weights[i]);
    printf("}\n");
}

void RK_Individual::criaCromossomos(Graph *g){
    Chromosome *c;
    int i = 0;
    for(Vertex *v = g->get_verticesList(); v != NULL; v= v->getNext()){

        for(Edge *e = v->getEdgesList(); e != NULL; e= e->getNext()){

            /**individuo possui somente arcos modificaveis e em um sentido
            (antes tinhamos arcos a-b e b-a, agora usamos somente um deles)**/
            if(e->getModifiable() == true && e->getMarked() == true){
                c = new Chromosome();
                c->edge = e;
                c->weight = 0;
                c->position = i;
                cromossomos.push_back(c);
                i++;
            }
        }

    }
}

void RK_Individual::calculaFuncaoObjetivoOtimizado(Graph *g){

    /** colocar os cromossomos na ordem correta em que aparecem no grafo antes de associar os pesos **/
    sort(cromossomos.begin(), cromossomos.end(), ordenaPosicaoCromossomo);

    /** copia peso do individio paca cada cromossomo **/
    for(int i=0; i<this->getNumEdges(); i++)
        cromossomos.at(i)->weight = this->weights[i];

    sort(cromossomos.begin(), cromossomos.end(), sortChromosome);

    int n_arc_inseridos = 0, n_arcos_inserir = g->getVerticesSize() - 1 - g->getNumberOfNonModifiable();


    /** percorre vetor de cromossomos ordenados e tenta fechar chave(algoritmo de kruskal) **/
    for(int i=0; n_arc_inseridos<n_arcos_inserir; i++){

        if((cromossomos.at(i)->edge->getOrigin()->getIdTree() != cromossomos.at(i)->edge->getDestiny()->getIdTree()) &&
                cromossomos.at(i)->edge->isClosed() == false){

            int id = cromossomos.at(i)->edge->getOrigin()->getIdTree();
            for(Vertex *no = g->get_verticesList(); no != NULL; no = no->getNext()){
                if(no->getIdTree() == id)
                    no->setIdTree(cromossomos.at(i)->edge->getDestiny()->getIdTree());
            }

            cromossomos.at(i)->edge->setSwitch(true);
            g->findEdge(cromossomos.at(i)->edge->getDestiny()->getID(),
                        cromossomos.at(i)->edge->getOrigin()->getID())->setSwitch(true);

            n_arc_inseridos++;
        }

    }


    g->defineFlows();
    g->evaluateLossesAndFlows(1e-8); /** calcula fluxos e perda com erro de 1e-5 **/

    double *perdas = g->getLossesReseting();/** soma as perdas ativas e reativas em todos os arcos e retorna um double* **/

    this->active_loss = perdas[0];
    this->reactive_loss = perdas[1];

    /** no caso de solucoes que possuem configuracao viavel porem sao muito longas **/
    if(isnan(perdas[0]))
        this->active_loss = 9999999999;

    if(isnan(perdas[1]))
        this->reactive_loss = 9999999999;

    delete perdas;
}
