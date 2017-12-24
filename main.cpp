#include <stdio.h>
#include "Grafo.h"

#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist33barras_Yang.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/SISTEMA119s2.m"

#define configuracao "inicial"
//#define configuracao "literatura1"
//#define configuracao "ARSD"


int main(){
    char nome[] = arquivoEntrada;

    Grafo *g = new Grafo();

    g->leEntrada(nome);

    if(arquivoEntrada=="ENTRADAS_MODIFICADAS/sist33barras_Yang.m"){

            int ids[5];

            //CONFIGURACAO INICIAL
            if(configuracao == "inicial"){
                ids[0] = 33; ids[1] = 34; ids[2] = 35; ids[3] = 36; ids[4] = 37;
            }
            //CONFIGURACAO DA LITERATURA
            if(configuracao == "literatura1"){
                ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 32; ids[4] = 37;
            }
            //ABORDAGEM ARSD
            if(configuracao == "ARSD"){
                ids[0] = 7; ids[1] = 9; ids[2] = 14; ids[3] = 32; ids[4] = 37;
            }

            Arco *a;
            for(int i=0; i<5; i++){
                a = g->buscaArco(ids[i]);
                a->setChave(false);
                a = g->buscaArco(a->getNoDestino()->getID(), a->getNoOrigem()->getID());
                a->setChave(false);
            }
    }

    if(arquivoEntrada=="ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m"){

            int ids[5];

            //CONFIGURACAO INICIAL
            if(configuracao == "inicial"){
                ids[0] = 33; ids[1] = 34; ids[2] = 35; ids[3] = 36; ids[4] = 37;
            }
            //CONFIGURACAO DA LITERATURA
            if(configuracao == "literatura1"){
                ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 28; ids[4] = 36;
            }
            //ABORDAGEM ARSD
            if(configuracao == "ARSD"){
                ids[0] = 7; ids[1] = 10; ids[2] = 14; ids[3] = 16; ids[4] = 28;
            }

            Arco *a;
            for(int i=0; i<5; i++){
                a = g->buscaArco(ids[i]);
                a->setChave(false);
                a = g->buscaArco(a->getNoDestino()->getID(), a->getNoOrigem()->getID());
                a->setChave(false);
            }
    }


    g->define_sentido_fluxos();
    g->calcula_fluxos_e_perdas(1e-8);

    printf("\ntensao minima: %.5f (p.u)", g->tensaoMinima());
    printf("\nperdaTotal: %.5f (kW)", 100*1000*g->soma_perdas()[0]);
    printf("\neh arvore? %d\n\n\n", g->ehArvore());
}
