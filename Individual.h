#ifndef INDIVIDUO_H_INCLUDED
#define INDIVIDUO_H_INCLUDED
#include <math.h>

#include "Graph.h"


struct Chromosome{
    Edge *edge;
    double weight;
};

class Individual{
    private:
    int number_of_edges;
        double *weights, active_loss, reactive_loss;
    public:
        Individual(int numEdges);
        ~Individual(){delete[] weights;}

        int getNumArcos(){          return this->number_of_edges;   }
        double *getWeights(){       return this->weights;           }
        double getActiveLoss(){     return this->active_loss;       }
        double getReactiveLoss(){   return this->reactive_loss;     }

        void calculate_fitness_cap(Graph *g, Vertex *vertexCap1, Vertex *vertexCap2=NULL, Vertex *vertexCap3=NULL );
        void generate_random_weights();

        void crossover_mean(Individual *father, Individual *son);

        void mutation();

        void calculate_fitness(Graph *graph);

    Individual();
};
#endif // INDIVIDUO_H_INCLUDED
