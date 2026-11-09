#ifndef FILA_ATENDIMENTO_H
#define FILA_ATENDIMENTO_H

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
    const std::vector<Atendimento*>& getFila() const;
};


#endif