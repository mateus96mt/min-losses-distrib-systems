#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED

#include <vector>

//baixei os arquivos na tarde do dia 22/agosto

using namespace std;

enum TipoCapacitor {
    FIXO,
    CHAVEADO
};

class Capacitor { // características do capacitor (dados de entrada)
private:
    int id;                  // identificador
    TipoCapacitor tipo;
    double custo;            // custo do capacitor
    vector<int> steps;       // fluxos de potencia = (0, f1, f2, ... , fmax) ou (0, fmax)
    int maxCapacitoresBarra; // núm. máx de capacitores alocados em uma mesma barra

public:
    //gets e sets

};

class AlocacaoCapacitor {
private:
    Capacitor* capacitor; // tipo de capacitor alocado
    int numChaveamentos;  // numero de vezes que o capacitor é chaveado ao longo do dia
    vector<int> step;     // fluxos de potencia utilizados nesta alocacao ao longo do dia
                          // sendo que cada periodo do dia ocupa uma posicao do vetor.
                          // Caso não seja utilizado chaveamento, o fluxo de potencia
                          // utilizado ficará na posicão 0 do vetor

public:
    int getStep(int periodo=0);
};


/*
Funcao objetivo:
- custo da perda
- custo do capacitor
- reconfiguração (chaveamento)
/**/

class Arco;

class No{

private:
    //estrutura basica do No
    int id;
    No *proxNo;
    Arco *listaArcos;
    int grauSaida, grauEntrada;
    int grauAux;
    int idArv;
    vector<AlocacaoCapacitor> capacitores; // vetor de capacitores alocados no nó

    //informacoes necessarias para o problema de minimizacao de perdas:
    double potAtiva, potReativa;//demandas
    double voltagem;

    //informacoes uteis:
    bool marcado;

public:
    //Funcoes do No:
    No(int id);//construtor
    ~No();//destrutor

    void imprime();


    //GETS e SETS:
    int getID(){            return this->id;    };
    No *getProxNo(){        return this->proxNo; };
    Arco *getListaArcos(){  return this->listaArcos; };
    double getPotAtiva(){   return this->potAtiva; };
    double getPotReativa();
    double getVoltagem(){   return this->voltagem; };
    int getGrauSaida(){     return this->grauSaida; };
    int getGrauEntrada(){   return this->grauEntrada; };
    bool getMarcado(){      return this->marcado; };
    int getGrauAux(){       return this->grauAux; };
    int getIdArv(){         return this->idArv;};

    vector<AlocacaoCapacitor>* getCapacitores() { return &capacitores; }

    void setID(int id){                 this->id          = id; };
    void setProxNo(No *no){             this->proxNo      = no; };
    void setListaArcos(Arco *Arc){      this->listaArcos  = Arc; };
    void setPotAtiva(double potAt){     this->potAtiva    = potAt; };
    void setPotReativa(double potReat){ this->potReativa  = potReat; };
    void setVoltagem(double volt){      this->voltagem    = volt; };
    void setGrauSaida(int grau){        this->grauSaida   = grau;};
    void setGrauEntrada(int grau){      this->grauEntrada = grau; };
    void setMarcado(bool marca){        this->marcado     = marca; };
    void setGrauAux(int grau){          this->grauAux     = grau;};
    void setIdArv(int idArv){           this->idArv       = idArv;};

};

#endif // NO_H_INCLUDED
