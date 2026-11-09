#ifndef FILA_ATENDIMENTO_HPP
#define FILA_ATENDIMENTO_HPP

#include <vector>

class Atendimento; //Referência pois ainda não temos a classe

class FilaAtendimento{
private: 
    std::vector<Atendimento*> m_fila;
public:
    FilaAtendimento();
    ~FilaAtendimento();

    bool adicionarAtendimento(Atendimento* atendimento);
    bool removerAtendimento(Atendimento* atendimento);
    Atendimento* chamarProximo();
    void reorganizarFila();
    double estimarTempoDeFila(Atendimento* atendimento) const;

    int getTamanho() const;
    bool estaVazia() const;
    void visualizarFila() const;

};


#endif