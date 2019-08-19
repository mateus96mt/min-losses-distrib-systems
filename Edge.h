#ifndef ARCO_H_INCLUDED
#define ARCO_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include "Vertex.h"

class Edge{

private:
    //estrutura basica do Arco:
    int id;
    Edge *proxArco;
    Vertex *noOrigem, *noDestino;

    //informacoes necessarias para o problema de minimizacao de perdas:
    bool chave;//1=chave fechada, ou seja em uso.   0=chave aberta
    double FLuxoPAtiva, FLuxoPReativa;//fluxos de potencia ativa e reativa
    double resistencia, reatancia;
    double perdaAtiva, perdaReativa;//perda de potencia ativa e reativa no arco

    bool modificavel;
    bool marcado;
    bool fixed;

public:
    //funcoes do Arco:
    Edge(int id);//construtor
    ~Edge();//destrutor

    void imprime();



    //GETS e SETS:
    int getID(){              return this->id;    };
    Edge *getProxArco(){      return this->proxArco;  };
    Vertex *getNoOrigem(){        return this->noOrigem; };
    Vertex *getNoDestino(){       return this->noDestino; };
    bool getChave(){          return this->chave; };
    double getFLuxoPAtiva(){  return this->FLuxoPAtiva; };
    double getFLuxoReativa(){ return this->FLuxoPReativa; };
    double getResistencia(){  return this->resistencia; };
    double getReatancia(){    return this->reatancia; };
    double getPerdaAtiva(){   return this->perdaAtiva; };
    double getPerdaReativa(){ return this->perdaReativa; };
    bool getModificavel(){    return this->modificavel; };
    bool getMarcado(){        return this->marcado;};
    bool getFixed(){          return this->fixed;}

    void setID(int id){                      this->id            = id; };
    void setProxArco(Edge *arc){ this->proxArco      = arc; };
    void setNoOrigem(Vertex *no){ this->noOrigem      = no; };
    void setNoDestino(Vertex *no){ this->noDestino     = no; };
    void setChave(bool chave){               this->chave         = chave; };
    void setFLuxoPAtiva(double FpAtiva){     this->FLuxoPAtiva   = FpAtiva; };
    void setFLuxoPReativa(double FpReativa){ this->FLuxoPReativa = FpReativa; };
    void setResistencia(double resist){      this->resistencia   = resist; };
    void setReatancia(double reat){          this->reatancia     = reat; };
    void setPerdaAtiva(double perdaA){       this->perdaAtiva    = perdaA; };
    void setPerdaReativa(double perdaReat){  this->perdaReativa  = perdaReat; };
    void setModificavel(bool modificavel){   this->modificavel   = modificavel; };
    void setMarcado(bool marca){             this->marcado       = marca;};
    void setFixed(bool fixed){               this->fixed         = fixed;}
};

#endif // ARCO_H_INCLUDED
