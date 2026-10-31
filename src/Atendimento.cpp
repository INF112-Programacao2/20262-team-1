#include "Atendimento.h"

#include <iostream>
#include "Paciente.h"
#include "Prioridade.h"

bool prioridadeValida(int prioridade) {
    return prioridade >= PRIORIDADE_MINIMA && prioridade <= PRIORIDADE_MAXIMA;
}

// Prioridade fora da faixa vira PRIORIDADE_MINIMA, para o objeto nunca nascer inválido
Atendimento::Atendimento(int prioridade, double tempoEstimadoMin, const std::string& idPaciente,
                         int id, const std::string& data, Paciente* paciente)
    : m_prioridade(prioridadeValida(prioridade) ? prioridade : PRIORIDADE_MINIMA),
      m_tempoEstimadoMin(tempoEstimadoMin),
      m_idPaciente(idPaciente),
      m_id(id),
      m_data(data),
      m_status(StatusAtendimento::EM_ESPERA),
      m_paciente(paciente),
      m_fila(nullptr) {}

// Paciente, profissionais e fila não pertencem ao atendimento: nada a liberar
    Atendimento::~Atendimento() = default;

void Atendimento::inicializar() {
    if (m_status == StatusAtendimento::EM_ESPERA) {
        m_status = StatusAtendimento::EM_ATENDIMENTO;
    }
}

// Um atendimento finalizado não muda mais de estado
void Atendimento::atualizarStatus(StatusAtendimento novoStatus) {
    if (m_status == StatusAtendimento::FINALIZADO) {
        return;
    }
    m_status = novoStatus;
}

bool Atendimento::verificarPendencias() const {
    return std::any_of(m_procedimentos.begin(), m_procedimentos.end(),
                       [](const Procedimento& p) { return !p.foiExecutado(); });
}

bool Atendimento::finalizar() {
    if (m_status == StatusAtendimento::FINALIZADO || verificarPendencias()) {
        return false;
    }
    m_status = StatusAtendimento::FINALIZADO;
    return true;
}
void Atendimento::adicionarProcedimento(const Procedimento& procedimento) {
    if (m_status == StatusAtendimento::FINALIZADO) {
        return;
    }
    m_procedimentos.push_back(procedimento);
}

void Atendimento::executarProcedimentos() {
    for (Procedimento& procedimento : m_procedimentos) {
        if (!procedimento.foiExecutado()) {
            procedimento.executar();
        }
    }
}

const std::vector<Procedimento>& Atendimento::getProcedimentos() const {
    return m_procedimentos;
}

void Atendimento::adicionarProfissional(ProfissionalSaude* profissional) {
    if (profissional == nullptr) {
        return;
    }
    if (std::find(m_equipeResponsavel.begin(), m_equipeResponsavel.end(), profissional)
        != m_equipeResponsavel.end()) {
        return;
    }
    m_equipeResponsavel.push_back(profissional);
}


//getters
const std::vector<ProfissionalSaude*>& Atendimento::getEquipeResponsavel() const {
    return m_equipeResponsavel;
}

int Atendimento::getPrioridade() const {
    return m_prioridade;
}

void Atendimento::setPrioridade(int novaPrioridade) {
    if (prioridadeValida(novaPrioridade)) {
        m_prioridade = novaPrioridade;
    }
}

double Atendimento::getTempoEstimadoMin() const {
    return m_tempoEstimadoMin;
}

std::string Atendimento::getIdPaciente() const {
    return m_idPaciente;
}

int Atendimento::getId() const {
    return m_id;
}

std::string Atendimento::getData() const {
    return m_data;
}

StatusAtendimento Atendimento::getStatus() const {
    return m_status;
}

Paciente* Atendimento::getPaciente() const {
    return m_paciente;
}

FilaAtendimento* Atendimento::getFila() const {
    return m_fila;
}

void Atendimento::setFila(FilaAtendimento* fila) {
    m_fila = fila;
}