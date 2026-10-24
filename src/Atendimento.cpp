#include "../include/Atendimento.hpp"
#include <string>


Atendimento::Atendimento(int prioridade, double tempoEstimadoMin, std::string idPaciente)
    : m_prioridade(prioridade), m_idPaciente(idPaciente), m_tempoEstimadoMin(tempoEstimadoMin) {}

Atendimento::~Atendimento() = default;

int Atendimento::getPrioridade() const { 
    return m_prioridade; 
}

void Atendimento::setPrioridade(int novaPrioridade) { 
    m_prioridade = novaPrioridade; 
}

double Atendimento::getTempoEstimadoMin() const { 
    return m_tempoEstimadoMin; 
}
std::string Atendimento::getIdPaciente() const { 
    return m_idPaciente; 
}