#include "Random_keys.h"

using namespace std;

bool orderedIndividual(Individual *i1, Individual *i2){return i1->getActiveLoss() > i2->getActiveLoss();}

Random_keys::Random_keys(int popSize, int numGenerations){
    this->popSize = popSize;
    this->generationSize = numGenerations;
}

void Random_keys::generatePopulation(Graph *g){

    Individual *ind;
    for(int i=0; i<this->popSize; i++){

        ind = new Individual(g->getEdgesSizes());
        ind->generate_random_weights();

        currentPopulation.push_back(ind);
    }
    lastPopulation = currentPopulation;
}

/** Ordena populacao em ordem decrescente por valor da funcao objetivo
dado que queremos minimizar a perda os piores individuos ficam no inicio(perda maior)
queremos os melhore (menor perda, fim da lista) **/
void Random_keys::sort_population(Graph *g){
    for(unsigned int i=0; i<currentPopulation.size(); i++)
        currentPopulation.at(i)->calculate_fitness(g);

    sort(currentPopulation.begin(), currentPopulation.end(), orderedIndividual);
}

void Random_keys::forwardGenerations(Graph *graph){

    int counter = 0;
    double oldLoss = 0.0;
    int k;
    Individual *best;
    for(k=0; k<this->generationSize; k++){
        /** calcula a funcao criterio para cada individuo e ordena a populacao da maior perda(pior individuo)
        pra menor perda(melhor individuo), perdaAtiva**/
        this->sort_population(graph);

        best = currentPopulation.at(this->popSize-1);

        if(oldLoss == best->getActiveLoss()){
            counter++;
            if( counter >= 0.25*generationSize ) break;
        }
        else{
            counter = 0;
            oldLoss = best->getActiveLoss();
            printf("\nGeracao %4.d \t-\tMelhor Individuo: %f kw", k, 100 * 1000 * best->getActiveLoss());
        }

        lastPopulation = currentPopulation;

        int num_worst = 0.05*this->popSize;
        int num_best  = 0.1 *this->popSize;

        for(int i=num_worst; i<this->popSize-num_best; i++){

            int father1 = rand() % this->popSize;
//            int father2 = rand() % this->popSize; //aleatorio

            /**torneio com 3**/
            int cand1, cand2, cand3;
            cand1 = rand() % this->popSize;
            cand2 = rand() % this->popSize;
            cand3 = rand() % this->popSize;

            if(lastPopulation.at(cand1)->getActiveLoss() < lastPopulation.at(cand2)->getActiveLoss()){
                cand2=cand1;
            }
            if(lastPopulation.at(cand2)->getActiveLoss() < lastPopulation.at(cand3)->getActiveLoss()){
                cand3=cand2;
            }
            int father2 = cand3;

            while( father2 == father1 )
                father2 = rand() % this->popSize;

            lastPopulation.at(father1)->crossover_mean(lastPopulation.at(father2), currentPopulation.at(i));
            currentPopulation.at(i)->mutation();
        }

        for(int i=0; i<num_worst; i++)
            currentPopulation.at(i)->generate_random_weights(); // Populacao aleatoria no lugar dos piores
    }
    printf("\nGeracao %4.d \t-\tMelhor Individuo: %9.6f kw", k, 100 * 1000 * best->getActiveLoss()); // Result em kW
}
