#ifndef RANDOM_KEYS_H_INCLUDED
#define RANDOM_KEYS_H_INCLUDED
#include <math.h>

#include "RK_Individual.h"
#include "Evolutionary_path_relinking.h"

using namespace std;

class RKGA{
    private:
        int numGeracoes, tamPop;

        vector<RK_Individual*> popAtual;
        vector<RK_Individual*> popAnterior;

    public:
        RKGA(int tamPop, int numGeracoes);

        /** GET's **/
        vector<RK_Individual*> getPopAtual(){return this->popAtual;}
        vector<RK_Individual*> getPopAnterior(){return this->popAnterior;}
        int getNumGeracoes(){return this->numGeracoes;}
        int getTamPopulacao(){return this->tamPop;}


        void geraPopAleatoria(Graph_network *g);
        void geraPopAleatoriaConfInicial(Graph_network *g, int *idsAbertos, int nAbertos);

        ~RKGA(){}

        void ordenaPopulacaoAtual(Graph_network *g);

        int avancaGeracoes(Graph_network *g);
        int avancaGeracoes2(Graph_network *g);
        int avancaGeracoesPRS(Graph_network *g);//avanca geracoes aplicando pathrelinking simples(a cada geracao)
        int avancaGeracoesGenerico(Graph_network *g, int tipoCruzamento);
        int avancaGeracaoesAdaptativo(Graph_network *g, int it_s_melhora, float maxMutacao, float max_pct, float taxaTamMut, float taxaPctMut);
        int avancaGeracaoesAdaptativo2(Graph_network *g, int it_s_melhora, float maxMutacao, float max_pct, float taxaTamMut, float taxaPctMut);


    void prsEvolutivo(vector<RK_Individual*> pool, vector<RK_Individual*> &populacao, Graph_network *g);//path relinking simples evolutivo
        int avancaGeracoesPRSEvolutivoFinal(Graph_network *g);


        RK_Individual *pre(vector<RK_Individual*> pool, int max_it, float pct_pr_elite, Graph_network *g);
        int avancaGeracoesPRE(Graph_network *g, int it_s_melhora, int tam_pool, int max_it, float pct_pool_elite);

        ///--------------------------RKGA operators--------------------------:

        void cruzamento1(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho);

        void cruzamento2(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho);

        void cruzamento3(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho);

        void cruzamento4(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho);

        void cruzamento5(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho);

        void cruzamento6(RK_Individual *pai1, RK_Individual *pai2, RK_Individual *filho);

        void mutacao(RK_Individual *ind);

        void mutacaoAdaptavitva(RK_Individual *ind, float pctTam, float pctChance);

        ///--------------------------RKGA operators--------------------------:

        int avancaGeracoesPRECA(Graph_network *g, int it_s_melhora, int tam_pool, int max_it, float pct_pool_elite);
};
#endif // RANDOM_KEYS_H_INCLUDED
