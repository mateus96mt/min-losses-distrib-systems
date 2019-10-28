#ifndef RANDOM_KEYS_H_INCLUDED
#define RANDOM_KEYS_H_INCLUDED
#include <math.h>

#include "Individual.h"

using namespace std;

class Random_keys{
    private:
        int generationSize, popSize;

        vector<Individual*> currentPopulation;
        vector<Individual*> lastPopulation;

    public:
        Random_keys(int popSize, int numGenerations);

        /** GET's **/
        vector<Individual*> getCurrentPop(){return this->currentPopulation;}
        vector<Individual*> getLastPop(){return this->lastPopulation;}
        int getGenerationSize(){return this->generationSize;}
        int getPopulationSize(){return this->popSize;}


        void generatePopulation(Graph *g);

        ~Random_keys(){}

        void sort_population(Graph *g);

        void forwardGenerations(Graph *graph);

};
#endif // RANDOM_KEYS_H_INCLUDED
