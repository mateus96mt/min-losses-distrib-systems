#include <stdio.h>
#include "Grafo.h"

//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist33barras_Yang.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/sist33barras_Yang-modificado.m"
//#define arquivoEntrada "ENTRADAS_MODIFICADAS/SISTEMA119s2.m"
#define arquivoEntrada "ENTRADAS_MODIFICADAS/SISTEMA119s2.m"

//#define configuracao "inicial"
//#define configuracao "literatura1"
#define configuracao "ARSD"


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

    if(arquivoEntrada=="ENTRADAS_MODIFICADAS/SISTEMA119s2.m"){

        int ids[15];
        //CONFIGURACAO INICIAL
        if(configuracao == "inicial"){
            ids[0] = 119; ids[1] = 120; ids[2] = 121; ids[3] = 122; ids[4] = 123;
            ids[5] = 124; ids[6] = 125; ids[7] = 126; ids[8] = 127; ids[9] = 128;
            ids[10] = 129; ids[11] = 130; ids[12] = 131; ids[13] = 132; ids[14] = 133;


        }
        //CONFIGURACAO DA LITERATURA
        if(configuracao == "literatura1"){
            ids[0] = 24; ids[1] = 27; ids[2] = 35; ids[3] = 40; ids[4] = 43;
            ids[5] = 52; ids[6] = 59; ids[7] = 72; ids[8] = 75; ids[9] = 96;
            ids[10] = 99; ids[11] = 110; ids[12] = 123; ids[13] = 130; ids[14] = 131;
        }
        //ABORDAGEM ARSD
        if(configuracao == "ARSD"){
            ids[0] = 24; ids[1] = 26; ids[2] = 35; ids[3] = 40; ids[4] = 43;
            ids[5] = 51; ids[6] = 61; ids[7] = 72; ids[8] = 75; ids[9] = 96;
            ids[10] = 98; ids[11] = 110; ids[12] = 122; ids[13] = 130; ids[14] = 131;
        }

        /*
        esse arquivo tem na verdade 132 arcos ao inves de 133
        dessa forma os ids dos arcos abertos deve ser subtraido 1.
        */
        for(int i=0; i<15; i++)
            ids[i]--;

        Arco *a;
        for(int i=0; i<15; i++){
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
