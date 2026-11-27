#include "../include/Leito.h"
#include "../include/StatusLeito.h"

Leito::Leito(int id)
    : m_id(id),
      m_status(StatusLeito::DISPONIVEL),
      m_paciente(nullptr) {
}

Leito::~Leito() {
    m_paciente = nullptr;
}

void Leito::alocarPaciente(Paciente* p) {
    if (p != nullptr && verificarDisponibilidade()) {
        m_paciente = p;
        m_status = StatusLeito::OCUPADO;
    }
}

void Leito::bloquear() {
    if (m_status == StatusLeito::DISPONIVEL) {
        m_status = StatusLeito::BLOQUEADO;
    }
}

void Leito::liberar() {
    if (m_status == StatusLeito::OCUPADO) {
        m_paciente = nullptr;
        m_status = StatusLeito::DISPONIVEL;
    }
}
StatusLeito Leito::getStatus(){
    return m_status;
}
Paciente* Leito::getPaciente(){
    return m_paciente;
}
void Leito::desbloquear(){
    if(m_status==StatusLeito::BLOQUEADO){
        m_status=StatusLeito::DISPONIVEL;
    }
}

bool Leito::verificarDisponibilidade() {
    return m_status == StatusLeito::DISPONIVEL;
}

int Leito::getId() {
    return m_id;
}
