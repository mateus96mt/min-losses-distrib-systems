#ifndef ARCO_H_INCLUDED
#define ARCO_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include "No.h"

class Arco{

private:
    //estrutura basica do Arco:
    int id;
    Arco *proxArco;
    No *noOrigem, *noDestino;

    //informacoes necessarias para o problema de minimizacao de perdas:
    bool chave;//1=chave fechada, ou seja em uso.   0=chave aberta
    double FLuxoPAtiva, FLuxoPReativa;//fluxos de potencia ativa e reativa
    double resistencia, reatancia;
    double perdaAtiva, perdaReativa;//perda de potencia ativa e reativa no arco

    bool modificavel;
    bool marcado;

public:
    //funcoes do Arco:
    Arco(int id);//construtor
    ~Arco();//destrutor

    void imprime();



    //GETS e SETS:
    int getID(){              return this->id;    };
    Arco *getProxArco(){      return this->proxArco;  };
    No *getNoOrigem(){        return this->noOrigem; };
    No *getNoDestino(){       return this->noDestino; };
    bool getChave(){          return this->chave; };
    double getFLuxoPAtiva(){  return this->FLuxoPAtiva; };
    double getFLuxoReativa(){ return this->FLuxoPReativa; };
    double getResistencia(){  return this->resistencia; };
    double getReatancia(){    return this->reatancia; };
    double getPerdaAtiva(){   return this->perdaAtiva; };
    double getPerdaReativa(){ return this->perdaReativa; };
    bool getModificavel(){    return this->modificavel; };
    bool getMarcado(){        return this->marcado;};

    void setID(int id){                      this->id            = id; };
    void setProxArco(Arco *arc){             this->proxArco      = arc; };
    void setNoOrigem(No *no){                this->noOrigem      = no; };
    void setNoDestino(No *no){               this->noDestino     = no; };
    void setChave(bool chave){               this->chave         = chave; };
    void setFLuxoPAtiva(double FpAtiva){     this->FLuxoPAtiva   = FpAtiva; };
    void setFLuxoPReativa(double FpReativa){ this->FLuxoPReativa = FpReativa; };
    void setResistencia(double resist){      this->resistencia   = resist; };
    void setReatancia(double reat){          this->reatancia     = reat; };
    void setPerdaAtiva(double perdaA){       this->perdaAtiva    = perdaA; };
    void setPerdaReativa(double perdaReat){  this->perdaReativa  = perdaReat; };
    void setModificavel(bool modificavel){   this->modificavel   = modificavel; };
    void setMarcado(bool marca){             this->marcado       = marca;};
};

#endif // ARCO_H_INCLUDED
