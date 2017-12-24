#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED

using namespace std;

class Arco;

class No{

private:
    //estrutura basica do No
    int id;
    No *proxNo;
    Arco *listaArcos;
    int grauSaida, grauEntrada;

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
    double getPotReativa(){ return this->potReativa; };
    double getVoltagem(){   return this->voltagem; };
    int getGrauSaida(){     return this->grauSaida; };
    int getGrauEntrada(){   return this->grauEntrada; };
    bool getMarcado(){      return this->marcado; };

    void setID(int id){                 this->id          = id; };
    void setProxNo(No *no){             this->proxNo      = no; };
    void setListaArcos(Arco *Arc){      this->listaArcos  = Arc; };
    void setPotAtiva(double potAt){     this->potAtiva    = potAt; };
    void setPotReativa(double potReat){ this->potReativa  = potReat; };
    void setVoltagem(double volt){      this->voltagem    = volt; };
    void setGrauSaida(int grau){        this->grauSaida   = grau;};
    void setGrauEntrada(int grau){      this->grauEntrada = grau; };
    void setMarcado(bool marca){        this->marcado     = marca; };

};

#endif // NO_H_INCLUDED
