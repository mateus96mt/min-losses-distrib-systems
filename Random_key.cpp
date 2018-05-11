#include <algorithm>

#include "Random_keys.h"
#include "time.h"

bool ordenacaoCromossomo(cromossomo *c1, cromossomo *c2){return c1->peso > c2->peso;}

vector<cromossomo*> geraSolucaoAleatoria(Grafo *g){

    vector<cromossomo*> solucao;

    cromossomo *c;

    for(No *no = g->getListaNos(); no != NULL; no = no->getProxNo()){

        for(Arco *a = no->getListaArcos(); a != NULL; a = a->getProxArco()){

            c = new cromossomo();
            c->peso = rand() % 1000; //peso de cada cromossomo eh um valor aleatorio
            c->a = a;

            solucao.push_back(c);
        }
    }
    return solucao;
}

double *perdaTotalSolucao(vector<cromossomo*> solucao, Grafo *g){

    //abrir todas as chaves, cada no fica em uma componente conexa
    for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){
        for(Arco *a = no->getListaArcos(); a!=NULL; a = a->getProxArco())
            a->setChave(false);
    }
    g->resetaIdArv(); // reseta os ids de componente conexa

    //ordenar somente a copia para nao alterar a ordem dos cromossomos do individuo
    vector<cromossomo*> solucaoCP = solucao;

    //ordena os cromossomos em ordem decrescente de peso
    sort(solucaoCP.begin(), solucaoCP.end(), ordenacaoCromossomo);

    int n_arc_inseridos = 0, n_arcos_inserir = g->getNumeroNos()-1;

    //percorre vetor de cromossomos ordenados e tenta fechar chave(algoritmo de kruskal)
    for(int i=0; n_arc_inseridos<n_arcos_inserir; i++){

        if( (solucaoCP.at(i)->a->getNoOrigem()->getIdArv() != solucaoCP.at(i)->a->getNoDestino()->getIdArv()) && solucaoCP.at(i)->a->getChave()==false){

            int id = solucaoCP.at(i)->a->getNoOrigem()->getIdArv();
            for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){
                if(no->getIdArv()==id)
                    no->setIdArv(solucaoCP.at(i)->a->getNoDestino()->getIdArv());
            }

            solucaoCP.at(i)->a->setChave(true);
            g->buscaArco(solucaoCP.at(i)->a->getNoDestino()->getID(), solucaoCP.at(i)->a->getNoOrigem()->getID())->setChave(true);

            n_arc_inseridos++;
        }

    }

    printf("\n\nAberto:{");
    for(No *no = g->getListaNos(); no!=NULL; no = no->getProxNo()){
        for(Arco *a = no->getListaArcos(); a!=NULL; a = a->getProxArco()){
            if(a->getChave()==false)
                printf("%d,", a->getID());
        }
    }
    printf("  }\n\n");

    g->define_sentido_fluxos();
    g->calcula_fluxos_e_perdas(1e-5);

    return g->soma_perdas();
}


vector<cromossomo*> cruzamentoMedia(vector<cromossomo*> s1, vector<cromossomo*> s2){

    vector<cromossomo*> filho;

    cromossomo *c;

    for(int i=0; i<s1.size(); i++){
        c = new cromossomo();
        c->a = s1.at(i)->a;
        c->peso = (s1.at(i)->peso + s2.at(i)->peso)/2.0;

        filho.push_back(c);
    }

    return filho;
}

void mutacao1(vector<cromossomo*> &solucao, int chance){
    int i = rand() % 100;

    if(i<=chance){

        int j = rand() % solucao.size();
        solucao.at(j)->peso = rand() % 1000;
    }
}

