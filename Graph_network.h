#ifndef DIGRAFO_H_INCLUDED
#define DIGRAFO_H_INCLUDED

#include <iostream>
#include <fstream>

#include "Vertex.h"
#include "Edge.h"
#include <vector>
#include <algorithm>

using namespace std;

class Graph_network{

private:
    //estrutura basica da lista de adjacencia:
    Vertex *listaNos;
    int numeroNos, numeroArcos;

    //informacoes necessarias para o problema de minimizacao de perdas:
    double perdaAtivaTotal, perdaReativaTotal;

    //informacoes uteis:
    int n_marcados; //contador de nos marcados, util em percursos em profundidade
    int n_naoModificaveis;

public:
    //funcoes do grafo:
    Graph_network();//construtor
    ~Graph_network();//destrutor

    Vertex* buscaNo(int id);
    Edge *buscaArco(int id);
    Edge *buscaArco(int idOrigem, int idDestino);


    void insereNo(int id, double potAtiva, double potReativa, double voltagem);
    void insereArco(int idOrigem, int idDestino, int id, double res, double reat, bool chave);

    void leEntrada(char nome[]);

    void imprime();


    //calcula o somatorio de cargas e perdas ativas pada todo o ramo abaixo do no "no"
    double cargasPerdasRamoAtiv(Vertex *no);
    void auxcargasPerdasRamoAtiv(Vertex *no, double &soma);

    //calcula o somatorio de cargas e perdas reativas pada todo o ramo abaixo do no "no"
    double cargasPerdasRamoReAtiv(Vertex *no);
    void auxcargasPerdasRamoReAtiv(Vertex *no, double &soma);

    void foward(int it);
    void Auxfoward(Vertex *no, Edge *ak, int it);

    void backward();
    void Auxbackward(Vertex *no);

    double *soma_perdas();

    /** funcao que soma as perdas e reseta os arcos
    utilizada para o calculo de funcao objetivo otimizado do individuo
    **/
    double *soma_perdasResetando();

    void calcula_fluxos_e_perdas(double tol);

    double tensaoMinima();


    void desmarcaNos();//util em percursos

    bool ehArvore();
    void auxehArvore(Vertex *no, int &marcados, bool &ciclo);

    bool ehConexo();
    void auxehConexo(Vertex *no, int &n_marcados);

    void define_sentido_fluxos();
    void auxDefine_sentido_fluxos(Vertex *no, Vertex *noAnterior);

    void defineArestasModificaveis();

    Graph_network *retornaCopia();

    void solucaoAleatoria();

    void resetaGrausAuxiliares();
    void resetaIdArv();
    void zeraFluxosEPerdas();

    void fechaArcosNaoModificaveis();
    void resetaModificaveis();

    void resetaArcosMarcados();
    void marcaUmsentidoArcos();

    void percursoPronfundidade(Vertex *start, int value);

    void checaCiclo();

    //GETS e SETS:
    Vertex *getListaNos(){          return this->listaNos; };
    int getNumeroNos(){         return this->numeroNos; };
    int getNumeroArcos(){       return this->numeroArcos; };
    int getN_marcados(){        return this->n_marcados; };
    int getN_naoModificaveis(){ return this->n_naoModificaveis;};

    void setListaNos(Vertex *no){ this->listaNos = no; };
    void marcaComponenteConexa(Vertex *start, int value);
    void aux_marcaComponenteConexa(Vertex *start, Vertex *previous, int value);
};

#endif // DIGRAFO_H_INCLUDED
