#ifndef INDIVIDUO_H_INCLUDED
#define INDIVIDUO_H_INCLUDED

#include <math.h>
#include "include/Graph.h"

struct Chromosome{
    Edge *edge;
    double weight;
    int position;
};

class RK_Individual{
    private:
    int number_of_edges;
        double *weights, active_loss, reactive_loss, objective_function;
    public:
        RK_Individual(int numEdges);
        ~RK_Individual(){delete[] weights;}

        int getNumEdges(){                  return this->number_of_edges;   }
        double *getWeights(){               return this->weights;           }
        double getActiveLoss(){             return this->active_loss;       }
        double getObjectiveFunction(){      return this->objective_function;}
        double getReactiveLoss(){           return this->reactive_loss;     }
       
	void generate_random_weights();

        void geraPesosConfInicial(int *idsAbertos, int n, Graph *g);

        void resetaPesos(float valor);

        void calculaFuncaoObjetivo(Graph *g);

        void calculaFuncaoObjetivoOtimizado(Graph *g);

        void imprimePesos();

	void calculate_fitness(Graph *graph,  bool theBest = false);

	///--------------------------conection to graph network--------------------------:

        static vector<Chromosome*> cromossomos;
        static void criaCromossomos(Graph *g);

        ///--------------------------conection to graph network--------------------------:

};
#endif // INDIVIDUO_H_INCLUDED
