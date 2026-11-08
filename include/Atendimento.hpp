//TEMPORÁRIO.
#ifndef ATENDIMENTO_HPP
#define ATENDIMENTO_HPP

#include <string>

class Atendimento {
private:
    int m_prioridade; // 5(MÁXIMA), 4(ALTA), 3(MÉDIA), 2(BAIXA), 1(MINIMA)
    double m_tempoEstimadoMin; //Tempo estimado em min do atendimento;
    std::string m_idPaciente;

public:
    Atendimento(int prioridade, double tempoEstimadoMin, std::string m_idPaciente);
    ~Atendimento();
    int getPrioridade() const;
    void setPrioridade(int novaPrioridade);

    double getTempoEstimadoMin() const;
    std::string getIdPaciente() const;
};


#endif