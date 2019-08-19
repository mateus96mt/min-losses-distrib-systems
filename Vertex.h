#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED

using namespace std;

class Edge;

class Vertex{

private:
    //estrutura basica do No
    int id;
    Vertex *proxNo;
    Edge *listaArcos;
    int grauSaida, grauEntrada;
    int grauAux;
    int idArv;

    //informacoes necessarias para o problema de minimizacao de perdas:
    double potAtiva, potReativa;//demandas
    double voltagem;

    //informacoes uteis:
    bool marcado;

public:
    //Funcoes do No:
    Vertex(int id);//construtor
    ~Vertex();//destrutor

    void imprime();


    //GETS e SETS:
    int getID(){            return this->id;    };
    Vertex *getProxNo(){        return this->proxNo; };
    Edge *getListaArcos(){  return this->listaArcos; };
    double getPotAtiva(){   return this->potAtiva; };
    double getPotReativa(){ return this->potReativa; };
    double getVoltagem(){   return this->voltagem; };
    int getGrauSaida(){     return this->grauSaida; };
    int getGrauEntrada(){   return this->grauEntrada; };
    bool getMarcado(){      return this->marcado; };
    int getGrauAux(){       return this->grauAux; };
    int getIdArv(){         return this->idArv;};

    void setID(int id){                 this->id          = id; };
    void setProxNo(Vertex *no){ this->proxNo      = no; };
    void setListaArcos(Edge *Arc){ this->listaArcos  = Arc; };
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
