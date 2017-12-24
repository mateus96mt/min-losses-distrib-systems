#include <stdio.h>
#include "Grafo.h"

int main(){
    char nome[] = "ENTRADAS_MODIFICADAS/SISTEMA119s2.m";
//    char nome[] = "ENTRADAS_MODIFICADAS/sist33barras_Yang.m";
//    char nome[] = "ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m";


    Grafo *g = new Grafo();

    g->leEntrada(nome);


    /*abre os ultimos arcos do arquivo de entrada
    para termos a configuracao inicial correspondente a tese do Leonardo
    */
    for(int i=g->getNumeroNos(); i<=g->getNUmeroArcos(); i++){
        Arco *a = g->buscaArcoID(i);
        a->setChave(false);
    }

    g->calcula_fluxos_e_perdas(1e-8);

    printf("\ntensao minima: %.5f", g->tensaoMinima());
    printf("\nperdaTotal: %.5f", 100*1000*g->soma_perdas()[0]);
    printf("\neh arvore? %d\n\n\n", g->ehArvore());
}
