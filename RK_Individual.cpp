#include "RK_Individual.h"
#define RANGEPESO 100

using namespace std;

vector<Cromossome*> RK_Individual::cromossomos = vector<Cromossome*>();

bool ordenacaoCromossomo(Cromossome *c1, Cromossome *c2){return c1->peso > c2->peso;}
bool ordenaPosicaoCromossomo(Cromossome *c1, Cromossome *c2){ return c1->posicao < c2->posicao;}
bool ordenaCromossomoPorIdArco(Cromossome *c1, Cromossome *c2){ return c1->arco->getID() < c2->arco->getID(); }


RK_Individual::RK_Individual(int numArcos){
     this->numArcos=numArcos;
     pesos = new double[this->numArcos];
}

void RK_Individual::geraPesosAleatorios(){
    for(int i=0; i<this->numArcos; i++)
        this->pesos[i] = rand() % RANGEPESO;
}

//void Individuo::cruzamentoMedia(Individuo *pai, Individuo *filho){
//    for(int i=0; i<this->numArcos; i++)
//        filho->getPesos()[i] = (this->pesos[i] + pai->getPesos()[i])/2.0;
//}

//void Individuo::mutacao(){
//    int i = rand() % 100;
//    if(i<=5){
//        int j = rand() % this->numArcos;
//        double peso = rand() % RANGEPESO;
//        this->pesos[j] = peso;
//    }
//    else{
//        if(i<=15){
//            int j = rand() % this->numArcos;
//            int k = rand() % this->numArcos;
//            double peso1 = rand() % RANGEPESO;
//            double peso2 = rand() % RANGEPESO;
//            this->pesos[j] = peso1;
//            this->pesos[k] = peso2;
//        }
//    }
//}

void RK_Individual::calculaFuncaoObjetivo(Graph_network *g){
    vector<Cromossome*> cromossomos;

    Cromossome *c;
    for(Vertex *no = g->getListaNos(); no != NULL; no=no->getProxNo()){

        for(Edge *a = no->getListaArcos(); a != NULL; a=a->getProxArco()){

            /**individuo possui somente arcos modificaveis e em um sentido
            (antes tinhamos arcos a-b e b-a, agora usamos somente um deles)**/
            if(a->getModificavel()==true && a->getMarcado()==true){
                c = new Cromossome();
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
    for(Vertex *no = g->getListaNos(); no != NULL; no = no->getProxNo()){
        for(Edge *a = no->getListaArcos(); a != NULL; a = a->getProxArco()){

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
            for(Vertex *no = g->getListaNos(); no != NULL; no = no->getProxNo()){
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

void RK_Individual::resetaPesos(float valor){
    for(int i=0; i<this->numArcos; i++)
        this->pesos[i] = valor;
}

void RK_Individual::geraPesosConfInicial(int *idsAbertos, int n, Graph_network *g){

//    cout << "vai gerar conf inicial:\n--------------------------------" << endl;

//    cout << "idsAberto = {" << endl;
//    for(int i=0; i<n; i++)
//        cout << idsAbertos[i] << endl;
//    cout << "}" << endl;

    this->resetaPesos(1.0);
    int j=0;
    for(Vertex *no = g->getListaNos(); no != NULL; no=no->getProxNo()){

        for(Edge *a = no->getListaArcos(); a != NULL; a=a->getProxArco()){

            if(a->getModificavel()==true && a->getMarcado()==true){
                for(int i=0; i<n; i++){
                    if(a->getID()==idsAbertos[i]){
                        this->pesos[j] = 0.0;
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
    for(int i=0; i<this->numArcos; i++)
        printf("%.2f, ", this->pesos[i]);
    printf("}\n");
}


//void Individuo::cruzamentoMedia2(Individuo *pai, Individuo *filho){
//    for(int i=0; i<this->numArcos; i++){
//        int j = rand() % 1000;
//        if (j>=800){
//            filho->getPesos()[i] = (this->pesos[i] + pai->getPesos()[i])*2.0;
//        }else{
//            if (j >= 100){
//                filho->getPesos()[i] = (this->pesos[i] + pai->getPesos()[i])/2.0;
//            }else{
//                filho->getPesos()[i] = (this->pesos[i] + pai->getPesos()[i]) * 0.75;
//            }
//        }
//    }
//}

void RK_Individual::criaCromossomos(Graph_network *g){
    Cromossome *c;
    int i = 0;
    for(Vertex *no = g->getListaNos(); no != NULL; no=no->getProxNo()){

        for(Edge *a = no->getListaArcos(); a != NULL; a=a->getProxArco()){

            /**individuo possui somente arcos modificaveis e em um sentido
            (antes tinhamos arcos a-b e b-a, agora usamos somente um deles)**/
            if(a->getModificavel()==true && a->getMarcado()==true){
                c = new Cromossome();
                c->arco = a;
                c->peso = 0;
                c->posicao = i;
                cromossomos.push_back(c);
                i++;
            }
        }

    }
}

void RK_Individual::calculaFuncaoObjetivoOtimizado(Graph_network *g){

    /** colocar os cromossomos na ordem correta em que aparecem no grafo antes de associar os pesos **/
    sort(cromossomos.begin(), cromossomos.end(), ordenaPosicaoCromossomo);

    /** copia peso do individio paca cada cromossomo **/
    for(int i=0; i<this->numArcos; i++)
        cromossomos.at(i)->peso = this->pesos[i];

    sort(cromossomos.begin(), cromossomos.end(), ordenacaoCromossomo);

    int n_arc_inseridos = 0, n_arcos_inserir = g->getNumeroNos() - 1 - g->getN_naoModificaveis();


    /** percorre vetor de cromossomos ordenados e tenta fechar chave(algoritmo de kruskal) **/
    for(int i=0; n_arc_inseridos<n_arcos_inserir; i++){

        if( (cromossomos.at(i)->arco->getNoOrigem()->getIdArv() != cromossomos.at(i)->arco->getNoDestino()->getIdArv()) && cromossomos.at(i)->arco->getChave()==false){

            int id = cromossomos.at(i)->arco->getNoOrigem()->getIdArv();
            for(Vertex *no = g->getListaNos(); no != NULL; no = no->getProxNo()){
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

    double *perdas = g->soma_perdasResetando();/** soma as perdas ativas e reativas em todos os arcos e retorna um double* **/

    this->perdaAtiva = perdas[0];
    this->perdaReativa = perdas[1];

    /** no caso de solucoes que possuem configuracao viavel porem sao muito longas **/
    if(isnan(perdas[0]))
        this->perdaAtiva = 9999999999;

    if(isnan(perdas[1]))
        this->perdaReativa = 9999999999;

    delete perdas;
}

////path relinking "guloso", se nenhum individuo do path foi melhor que this e guia entao retorna um individuo aleatorio
//Individuo *Individuo::prs(Individuo *guia, Grafo *g, Individuo *indRef){
//
//    vector<int> candidatos, path;//ids que representam a ordem que this sera igualado ao guia
//
//    Individuo *best = new Individuo(this->numArcos);
//    Individuo *aux = new Individuo(this->numArcos);
//
//    aux->calculaFuncaoObjetivoOtimizado(g);
//    best->calculaFuncaoObjetivoOtimizado(g);
//    guia->calculaFuncaoObjetivoOtimizado(g);
//    indRef->calculaFuncaoObjetivoOtimizado(g);//individuo de referencia, o path vai atualizando equanto obtem resultado melhor que o individuo de referencia
//
//    double bestPerdaAtiva = indRef->getPerdaAtiva();
//
//    for(int i=0; i<this->numArcos;i++){
//
//        candidatos.push_back(i);
//        best->getPesos()[i] = this->pesos[i];
//        aux->getPesos()[i] = this->pesos[i];
//
//    }
//
//    for(unsigned int i=0; i<candidatos.size(); i++){
//
//        aux->getPesos()[candidatos.at(i)] = guia->getPesos()[candidatos.at(i)];
//        aux->calculaFuncaoObjetivoOtimizado(g);
//
//        if(aux->getPerdaAtiva() < bestPerdaAtiva){
//
//            bestPerdaAtiva = aux->getPerdaAtiva();
//            path.push_back(candidatos.at(i));
//            candidatos.erase(candidatos.begin() + i);
//            i = 0;
//
//        }else{
//
//            aux->getPesos()[candidatos.at(i)] = this->getPesos()[candidatos.at(i)];
//
//        }
//
//    }
//
//
//    delete aux;
//
//    for(unsigned int i=0; i<path.size(); i++)
//        best->getPesos()[path.at(i)] = guia->getPesos()[path.at(i)];
//
//    best->calculaFuncaoObjetivoOtimizado(g);
//
//    return best;
//}
//
//
//Individuo *Individuo::prs2(Individuo *guia, Grafo *g){
//
////    printf("Niveis:\n\n");
//
//    vector<int> candidatos, path;//ids que representam a ordem que this sera igualado ao guia
//    vector<vector<int>> nivel;
//
//    int nivelBest, direcaoBest;//para cada nivel
//    int nivelBestGlobal, direcaoBestGlobal;
//
//    Individuo *aux = new Individuo(this->numArcos);
//    Individuo *best = new Individuo(this->numArcos);
//
//    aux->calculaFuncaoObjetivoOtimizado(g);
//    best->calculaFuncaoObjetivoOtimizado(g);
//    guia->calculaFuncaoObjetivoOtimizado(g);
//
//    double minPerdaNivel = 999999999999, minPerdaGlobal = 999999999999;
//
//    for(int i=0; i<this->numArcos;i++){
//
//        candidatos.push_back(i);
//        aux->getPesos()[i] = this->pesos[i];
//        best->getPesos()[i] = this->pesos[i];
//    }
//
//    int nv = 0;
//    while(candidatos.size()>0){
//
//        minPerdaNivel = 999999999999;
//
//        vector<int> v;
//        nivel.push_back(v);
//
//        //loop entre os candidatos do nivel
//        for(unsigned int i=0; i<candidatos.size(); i++){
//
//            aux->getPesos()[candidatos.at(i)] = guia->getPesos()[candidatos.at(i)];
//            aux->calculaFuncaoObjetivoOtimizado(g);
//
//            nivel.at(nv).push_back(candidatos.at(i));
//
////            printf("%d ", candidatos.at(i));
//
//            //melhor individuo no nivel
//            if(aux->getPerdaAtiva() < minPerdaNivel) {
//                minPerdaNivel = aux->getPerdaAtiva();
//                nivelBest = nv;
//                direcaoBest = i;
//            }
//
//            aux->getPesos()[candidatos.at(i)] = this->getPesos()[candidatos.at(i)];//desfaz alteracao para avaliar proxima
//
//        }
//
////        printf("{%d}  (nv %d)\n", candidatos.at(direcaoBest), nv);
//
//        aux->getPesos()[candidatos.at(direcaoBest)] = guia->getPesos()[candidatos.at(direcaoBest)];//caminha no path na direcao da melhor local
//
//        path.push_back(candidatos.at(direcaoBest));
//        candidatos.erase(candidatos.begin() + direcaoBest);
//
//        //verificar melhor individuo entre todos gerados ao longo do path
//        if(minPerdaNivel < minPerdaGlobal){
//            minPerdaGlobal = minPerdaNivel;
//            nivelBestGlobal = nivelBest;
//            direcaoBestGlobal = direcaoBest;
//        }
//
//        nv++;
//
//    }
//
////    printf("\n\nbest:\n");
//    //percorre caminho do path
//    for(int i=0; i<nivelBestGlobal; i++){
//        best->getPesos()[path.at(i)] = guia->getPesos()[path.at(i)];
////        printf("%d\n", path.at(i));
//    }
//    best->getPesos()[nivel.at(nivelBestGlobal).at(direcaoBestGlobal)] = guia->getPesos()[nivel.at(nivelBestGlobal).at(direcaoBestGlobal)];
////    printf("%d\n\n\n", nivel.at(nivelBestGlobal).at(direcaoBestGlobal));
//
//    best->calculaFuncaoObjetivoOtimizado(g);
//
////    printf("\n\nminPerdaGlobal: %f KW\n", 100*1000*minPerdaGlobal);
////    printf("best->getPerdaAtiva(): %f KW\n", 100*1000*best->getPerdaAtiva());
////    printf("nivelBestGLobal: %d \n", nivelBestGlobal);
////    printf("direcaoBestGLobal: %d \n", nivel.at(nivelBestGlobal).at(direcaoBestGlobal));
//
//    delete aux;
//
//    return best;
//
//}
