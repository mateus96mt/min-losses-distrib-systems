#include "Individuo.h"
#define RANGEPESO 100

using namespace std;

bool ordenacaoCromossomo(Cromossomo *c1, Cromossomo *c2){return c1->peso > c2->peso;}

Individuo::Individuo(int numArcos){
     this->numArcos=numArcos;
     pesos = new double[this->numArcos];
}

void Individuo::geraPesosAleatorios(){
    for(int i=0; i<this->numArcos; i++)
        this->pesos[i] = rand() % RANGEPESO;
}

void Individuo::cruzamentoMedia(Individuo *pai, Individuo *filho){
    for(int i=0; i<this->numArcos; i++)
        filho->getPesos()[i] = (this->pesos[i] + pai->getPesos()[i])/2.0;
}

void Individuo::mutacao(){
    int i = rand() % 100;
    if(i<=5){
        int j = rand() % this->numArcos;
        double peso = rand() % RANGEPESO;
        this->pesos[j] = peso;
    }
    else{
        if(i<=15){
            int j = rand() % this->numArcos;
            int k = rand() % this->numArcos;
            double peso1 = rand() % RANGEPESO;
            double peso2 = rand() % RANGEPESO;
            this->pesos[j] = peso1;
            this->pesos[k] = peso2;
        }
    }
}

void Individuo::calculaFuncaoObjetivo(Grafo *g){
    vector<Cromossomo*> cromossomos;

    Cromossomo *c;
    for(No *no = g->getListaNos(); no!=NULL; no=no->getProxNo()){

        for(Arco *a = no->getListaArcos(); a!=NULL; a=a->getProxArco()){
            c = new Cromossomo();
            c->arco = a;
            c->peso = 0;
            cromossomos.push_back(c);
        }

    }

    /** copia peso do individio paca cada cromossomo **/
    for(int i=0; i<this->numArcos; i++)
        cromossomos.at(i)->peso = this->pesos[i];

    sort(cromossomos.begin(), cromossomos.end(), ordenacaoCromossomo);

    int n_arc_inseridos = 0, n_arcos_inserir = g->getNumeroNos()-1;

    /** abre todas as chaves no grafo e zera todos os fluxos e perdas nos arcos**/
    for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){
        for(Arco *a = no->getListaArcos(); a!=NULL; a = a->getProxArco()){
            a->setChave(false);
            a->setFLuxoPAtiva(0.0);
            a->setFLuxoPReativa(0.0);
            a->setPerdaAtiva(0.0);
            a->setPerdaReativa(0.0);
        }
    }

    /** reseta os ids de componentes conexas **/
    g->resetaIdArv();

    /** percorre vetor de cromossomos ordenados e tenta fechar chave(algoritmo de kruskal) **/
    for(int i=0; n_arc_inseridos<n_arcos_inserir; i++){

        if( (cromossomos.at(i)->arco->getNoOrigem()->getIdArv() != cromossomos.at(i)->arco->getNoDestino()->getIdArv()) && cromossomos.at(i)->arco->getChave()==false){

            int id = cromossomos.at(i)->arco->getNoOrigem()->getIdArv();
            for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){
                if(no->getIdArv()==id)
                    no->setIdArv(cromossomos.at(i)->arco->getNoDestino()->getIdArv());
            }

            cromossomos.at(i)->arco->setChave(true);
            g->buscaArco(cromossomos.at(i)->arco->getNoDestino()->getID(), cromossomos.at(i)->arco->getNoOrigem()->getID())->setChave(true);

            n_arc_inseridos++;
        }

    }

    g->define_sentido_fluxos();
    g->calcula_fluxos_e_perdas(1e-8); /** calcula fluxos e perda com erro de 1e-5 **/

    double *perdas = g->soma_perdas();/** soma as perdas ativas e reativas em todos os arcos e retorna um double* **/

    this->perdaAtiva = perdas[0];
    this->perdaReativa = perdas[1];

    /** no caso de solucoes que possuem configuracao viavel porem sao muito longas **/
    if(isnan(perdas[0]))
        this->perdaAtiva = 9999999999;

    if(isnan(perdas[1]))
        this->perdaReativa = 9999999999;

    delete perdas;

    cromossomos.clear();///deletar vetor de cromossomos(nao esta deletando memoria)
}
