//
// Created by gbm on 21/09/19.
//

#ifndef MINIMIZACAO_PERDAS_SISTEMA_DISTRIBUICAO_CAPACITOR_H
#define MINIMIZACAO_PERDAS_SISTEMA_DISTRIBUICAO_CAPACITOR_H

#include <vector>

using namespace std;

class Capacitor {
private:

    int id;
    double cost;               // custo do capacitor

    /** fluxos de potencia utilizados nesta alocacao ao longo do dia sendo que cada periodo do dia ocupa uma posicao
     *    do vetor. Caso nao seja utilizado chaveamento, o fluxo de potencia utilizado ficar� na posic�o 0 do vetor **/
    vector<double> powerSteps;

public:
    Capacitor(int id, double power, double cost, int step = 0){
        this->id = id;
        this->cost = cost;
        this->powerSteps.insert( this->powerSteps.begin(), power);
    };
    ~Capacitor(){};

    double getPower(int step = 0){                          return powerSteps[step];            }

    int getId() const{                                      return this->id;                    }
    double getCost() const{                                 return this->cost;                  }

    void setId(int id){                                     this->id = id;                      }
    void setCost(double cost){                              this->cost = cost;                  }
};



#endif //MINIMIZACAO_PERDAS_SISTEMA_DISTRIBUICAO_CAPACITOR_H
