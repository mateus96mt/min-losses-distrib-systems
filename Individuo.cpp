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

            /**individuo possui somente arcos modificaveis e em um sentido
            (antes tinhamos arcos a-b e b-a, agora usamos somente um deles)**/
            if(a->getModificavel()==true && a->getMarcado()==true){
                c = new Cromossomo();
                c->arco = a;
                c->peso = 0;
                cromossomos.push_back(c);
            }
        }

    }

    /** copia peso do individio paca cada cromossomo **/
    for(int i=0; i<this->numArcos; i++)
        cromossomos.at(i)->peso = this->pesos[i];

    sort(cromossomos.begin(), cromossomos.end(), ordenacaoCromossomo);

    int n_arc_inseridos = 0, n_arcos_inserir = g->getNumeroNos() - 1 - g->getN_naoModificaveis();

    /** abre todas as chaves no grafo e zera todos os fluxos e perdas nos arcos**/
    for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){
        for(Arco *a = no->getListaArcos(); a!=NULL; a = a->getProxArco()){

            //arcos nao modificaveis ficam sempre fechados
            if(a->getModificavel()==false)
                a->setChave(true);
            else
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

void Individuo::resetaPesos(float valor){
    for(int i=0; i<this->numArcos; i++)
        this->pesos[i] = valor;
}

void Individuo::geraPesosConfInicial(int *idsAbertos, int n, Grafo *g){
    this->resetaPesos(1.0);
    int j=0;
    for(No *no = g->getListaNos(); no!=NULL; no=no->getProxNo()){

        for(Arco *a = no->getListaArcos(); a!=NULL; a=a->getProxArco()){

            if(a->getModificavel()==true && a->getMarcado()==true){
                for(int i=0; i<n; i++){
                    if(a->getID()==idsAbertos[i]){
                        this->pesos[j] = 0.0;
                    }
                }
                j++;
            }

        }
    }
}

void Individuo::imprimePesos(){
    printf("pesos {");
    for(int i=0; i<this->numArcos; i++)
        printf("%.2f, ", this->pesos[i]);
    printf("}\n");
}


void Individuo::cruzamentoMedia2(Individuo *pai, Individuo *filho){
    for(int i=0; i<this->numArcos; i++){
        int j = rand() % 1000;
        if (j>=800){
            filho->getPesos()[i] = (this->pesos[i] + pai->getPesos()[i])*2.0;
        }else{
            if (j >= 100){
                filho->getPesos()[i] = (this->pesos[i] + pai->getPesos()[i])/2.0;
            }else{
                filho->getPesos()[i] = (this->pesos[i] + pai->getPesos()[i]) * 0.75;
            }
        }
    }
}
