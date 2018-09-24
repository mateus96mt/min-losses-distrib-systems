#ifndef INDIVIDUO_H_INCLUDED
#define INDIVIDUO_H_INCLUDED
#include <math.h>

#include "Grafo.h"


struct Cromossomo{
    Arco *arco;
    double peso;
};

class Individuo{
    private:
        int numArcos;
        double *pesos, perdaAtiva, perdaReativa;
    public:
        Individuo(int numArcos);
        ~Individuo(){delete[] pesos;}

        /** GET's **/
        int getNumArcos(){return this->numArcos;}
        double *getPesos(){return this->pesos;}
        double getPerdaAtiva(){return this->perdaAtiva;}
        double getPerdaReativa(){return this->perdaReativa;}

        void geraPesosAleatorios();

        void cruzamentoMedia(Individuo *pai, Individuo *filho);

        void mutacao();

        void calculaFuncaoObjetivo(Grafo *g);
};
#endif // INDIVIDUO_H_INCLUDED
