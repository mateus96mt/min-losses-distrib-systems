#ifndef DIGRAFO_H_INCLUDED
#define DIGRAFO_H_INCLUDED

#include <iostream>
#include <fstream>

#include "No.h"
#include "Arco.h"
#include <vector>
#include <algorithm>

using namespace std;

class Grafo{

private:
    //estrutura basica da lista de adjacencia:
    No *listaNos;
    int numeroNos, numeroArcos;

    //informacoes necessarias para o problema de minimizacao de perdas:
    double perdaAtivaTotal, perdaReativaTotal;

    //informacoes uteis:
    int n_marcados; //contador de nos marcados, util em percursos em profundidade

public:
    //funcoes do grafo:
    Grafo();//construtor
    ~Grafo();//destrutor

    No* buscaNo(int id);
    Arco *buscaArco(int id);
    Arco *buscaArco(int idOrigem, int idDestino);


    void insereNo(int id, double potAtiva, double potReativa, double voltagem);
    void insereArco(int idOrigem, int idDestino, int id, double res, double reat, bool chave);

    void leEntrada(char nome[]);

    void imprime();


    //calcula o somatorio de cargas e perdas ativas pada todo o ramo abaixo do no "no"
    double cargasPerdasRamoAtiv(No *no);
    void auxcargasPerdasRamoAtiv(No *no, double &soma);

    //calcula o somatorio de cargas e perdas reativas pada todo o ramo abaixo do no "no"
    double cargasPerdasRamoReAtiv(No *no);
    void auxcargasPerdasRamoReAtiv(No *no, double &soma);

    void foward(int it);
    void Auxfoward(No *no, Arco *ak, int it);

    void backward();
    void Auxbackward(No *no);

    double *soma_perdas();
    void calcula_fluxos_e_perdas(double tol);

    double tensaoMinima();


    void desmarcaNos();//util em percursos

    bool ehArvore();
    void auxehArvore(No *no, int &marcados, bool &ciclo);

    bool ehConexo();
    void auxehConexo(No *no, int &n_marcados);

    void define_sentido_fluxos();
    void auxDefine_sentido_fluxos(No *no, No *noAnterior);

    void defineArestasModificaveis();

    Grafo *retornaCopia();

    void solucaoAleatoria();

    void resetaGrausAuxiliares();
    void resetaIdArv();
    void zeraFluxosEPerdas();



    //GETS e SETS:
    No *getListaNos(){    return this->listaNos; };
    int getNumeroNos(){   return this->numeroNos; };
    int getNumeroArcos(){ return this->numeroArcos; };
    int getN_marcados(){  return this->n_marcados; };

    void setListaNos(No *no){ this->listaNos = no; };
};

#endif // DIGRAFO_H_INCLUDED
