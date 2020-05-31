//#ifndef PATH_RELINKING_H_INCLUDED
//#define PATH_RELINKING_H_INCLUDED
//
//#include "include/Graph.h"
//#include "RK_Individual.h"
//#include "Individual.h"
//
//class Evolutionary_path_relinking{
//
//private:
//
//    ///old implementation with RK representation of individual (too slow)
//    vector <RK_Individual*> pool;//pool of RK individuals
//    int max_it;//maximum iterations without improviment
//    float pct_pr_elite; //percentage of best individuals of pool considered elite
//
//public:
//
//    Evolutionary_path_relinking(){}
//    ~Evolutionary_path_relinking(){}
//
//    Individual *path_relingking(Individual *start, Individual *end, Graph *g);
//    Individual *run(vector<RK_Individual*> pool_RK, int max_it, float pctElite, Graph *g);
//
//    ///old implementation with RK representation of individual (too slow)
//    Evolutionary_path_relinking(vector<RK_Individual*> pool, int max_it, float pct_pr_elite);
//
//    ///----------------------Path-relingking operators-------------------:
//
//    RK_Individual *prs(RK_Individual *inicio, RK_Individual *guia, Graph *g, RK_Individual *indRef);//path relinking simples path de this para guia
//    RK_Individual *prs2(RK_Individual *inicio, RK_Individual *guia, Graph *g);//path relinking com modificacoes sugeridas luciana
//
//    ///----------------------Path-relingking operators-------------------:
//
//    //Current evolutive path-relinking implementation
//    RK_Individual *pre(Graph *g);
//};
//
//#endif // PATH_RELINKING_H_INCLUDED
