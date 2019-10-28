#include "Individual.h"
#define RANGEPESO 100

using namespace std;

bool ordenacaoCromossomo(Chromosome *c1, Chromosome *c2){return c1->weight > c2->weight;}


Individual::Individual(int numEdges){
     this->number_of_edges = numEdges;
     weights = new double[ this->number_of_edges ];
}

void Individual::generate_random_weights(){
    for(int i=0; i<this->number_of_edges; i++)
        this->weights[i] = rand() % RANGEPESO;
}

void Individual::crossover_mean(Individual *father, Individual *son){
    for(int i=0; i<this->number_of_edges; i++)
        son->getWeights()[i] = (this->weights[i] + father->getWeights()[i])/2.0;
}

void Individual::mutation(){
    int i = rand() % 100;
    if(i<=5){
        int j = rand() % this->number_of_edges;
        double peso = rand() % RANGEPESO;
        this->weights[j] = peso;
    }
    else{
        if(i<=15){
            int j = rand() % this->number_of_edges;
            int k = rand() % this->number_of_edges;
            double peso1 = rand() % RANGEPESO;
            double peso2 = rand() % RANGEPESO;
            this->weights[j] = peso1;
            this->weights[k] = peso2;
        }
    }
}

void Individual::calculate_fitness(Graph *graph ){
    vector<Chromosome*> chromosomes;

    Chromosome *c;
    for( Vertex *vertex = graph->get_verticesList(); vertex!=NULL; vertex = vertex->getNext() ){
        for(Edge *e = vertex->getEdgesList(); e!=NULL; e= e->getNext()){
            c = new Chromosome();
            c->edge = e;
            c->weight = 0;
            chromosomes.push_back(c);
        }
    }

    /** copia peso do individio paca cada cromossomo **/
    for(int i=0; i<this->number_of_edges; i++)
        chromosomes.at(i)->weight = this->weights[i];

    sort(chromosomes.begin(), chromosomes.end(), ordenacaoCromossomo);

    int inserted_size = 0, toInsert_size = graph->getVerticesSize()-1;

    /** abre todas as chaves no grafo e zera todos os fluxos e perdas nos arcos**/
    for(Vertex *no = graph->get_verticesList(); no!=NULL; no = no->getNext()){
        for(Edge *a = no->getEdgesList(); a!=NULL; a = a->getNext()){
            a->setSwitch(false);
            a->setActiveFlow(0.0);
            a->setReactiveFlow(0.0);
            a->setActiveLoss(0.0);
            a->setReactiveLoss(0.0);
        }
    }

    graph->resetIDTree(); // reseta os ids de componentes conexas para algoritmo de Kruskal

    /** percorre vetor de cromossomos ordenados e tenta fechar chave(algoritmo de kruskal) **/
    for(int i=0; inserted_size<toInsert_size; i++){
        if( (chromosomes.at(i)->edge->getOrigin()->getIdTree() != chromosomes.at(i)->edge->getDestiny()->getIdTree()) &&
                chromosomes.at(i)->edge->isClosed()==false){

            int id = chromosomes.at(i)->edge->getOrigin()->getIdTree();
            for(Vertex *v = graph->get_verticesList(); v!=NULL; v = v->getNext()){
                if(v->getIdTree()==id)
                    v->setIdTree(chromosomes.at(i)->edge->getDestiny()->getIdTree());
            }

            chromosomes.at(i)->edge->setSwitch(true);
            graph->findEdge(chromosomes.at(i)->edge->getDestiny()->getID(),
                        chromosomes.at(i)->edge->getOrigin()->getID())->setSwitch(true);

            inserted_size++;
        }
    }

    graph->defineFlows();

    graph->capacitor_allocation();

    graph->evaluateLossesAndFlows(1e-8); /** calcula fluxos e perda com erro de 1e-5 */

    double *losses = graph->getLosses();/** soma as perdas ativas e reativas em todos os arcos e retorna um double */

    this->active_loss   = losses[0];
    this->reactive_loss = losses[1];

    /** no caso de solucoes que possuem configuracao viavel porem sao muito longas **/
    if(isnan(losses[0]))    this->active_loss   = 9999999999;
    if(isnan(losses[1]))    this->reactive_loss = 9999999999;

    delete losses;

    chromosomes.clear();///deletar vetor de cromossomos(nao esta deletando memoria)
}

void Individual::calculate_fitness_cap(Graph *g, Vertex *vertexCap1, Vertex *vertexCap2, Vertex *vertexCap3 ){
    vector<Chromosome*> chromosomes;

    Chromosome *c;
    for( Vertex *vertex = g->get_verticesList(); vertex != NULL; vertex = vertex->getNext() ){
        for(Edge *a = vertex->getEdgesList(); a!=NULL; a = a->getNext()){
            c = new Chromosome();
            c->edge = a;
            c->weight = 0;
            chromosomes.push_back(c);
        }
    }

    for(int i=0; i<this->number_of_edges; i++)
        chromosomes.at(i)->weight = this->weights[i];

    sort(chromosomes.begin(), chromosomes.end(), ordenacaoCromossomo);

    int inserted_size = 0, toInsert_size = g->getVerticesSize()-1;

    /** abre todas as chaves no grafo e zera todos os fluxos e perdas nos arcos**/
    for(Vertex *v = g->get_verticesList(); v!=NULL; v = v->getNext()){
        for(Edge *e = v->getEdgesList(); e!=NULL; e = e->getNext()){
            e->setSwitch(false);
            e->setActiveFlow(0.0);
            e->setReactiveFlow(0.0);
            e->setActiveLoss(0.0);
            e->setReactiveLoss(0.0);
        }
    }

    g->resetIDTree(); // reseta os ids de componentes conexas para algoritmo de Kruskal

    /** percorre vetor de cromossomos ordenados e tenta fechar chave(algoritmo de kruskal) **/
    for(int i=0; inserted_size<toInsert_size; i++){
        if( (chromosomes.at(i)->edge->getOrigin()->getIdTree() != chromosomes.at(i)->edge->getDestiny()->getIdTree()) &&
            chromosomes.at(i)->edge->isClosed()==false){

            int id = chromosomes.at(i)->edge->getOrigin()->getIdTree();
            for(Vertex *v = g->get_verticesList(); v!=NULL; v = v->getNext()){
                if(v->getIdTree()==id)
                    v->setIdTree(chromosomes.at(i)->edge->getDestiny()->getIdTree());
            }

            chromosomes.at(i)->edge->setSwitch(true);
            g->findEdge(chromosomes.at(i)->edge->getDestiny()->getID(), chromosomes.at(i)->edge->getOrigin()->getID())->setSwitch(true);

            inserted_size++;
        }
    }
    g->defineFlows();
    vertexCap1->addCapacitor( g->getCapacitorType(1) ); // TODO insere capacitor para teste
    if(vertexCap2 != NULL) vertexCap2->addCapacitor( g->getCapacitorType(1) );
    if(vertexCap3 != NULL) vertexCap3->addCapacitor( g->getCapacitorType(1) );

    g->evaluateLossesAndFlows(1e-8); /** calcula fluxos e perda com erro informado **/

    double *losses = g->getLosses();/** soma as perdas ativas e reativas em todos os arcos e retorna um double* **/

    this->active_loss   = losses[0];
    this->reactive_loss = losses[1];

    /** Problema de solucoes com configuracao viavel porem sao muito longas **/
    if( isnan(losses[0]) )    this->active_loss   = 9999999999;
    if( isnan(losses[1]) )    this->reactive_loss = 9999999999;

    delete losses;

    chromosomes.clear(); /// Deletar vetor de cromossomos(nao esta deletando memoria)
}
