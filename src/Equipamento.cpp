#include "../include/StatusEquipamento.h"
#include "../include/Equipamento.h"

Equipamento::Equipamento(int id, std::string nome, std::string descricao) : RecursoHospitalar(id, nome, descricao){
    m_status=StatusEquipamento::DISPONIVEL;
}

Equipamento::~Equipamento()=default;

bool Equipamento::verificarDisponibilidade() {
    return m_status == StatusEquipamento::DISPONIVEL;
}

void Equipamento::reservar() {
    if (m_status == StatusEquipamento::DISPONIVEL) {
        m_status = StatusEquipamento::EM_USO;
    }
}

void Equipamento::liberar() {
    if (m_status == StatusEquipamento::EM_USO) {
        m_status = StatusEquipamento::DISPONIVEL;
    }
}

void Equipamento::iniciarManutencao() {
    if (m_status == StatusEquipamento::DISPONIVEL) {
        m_status = StatusEquipamento::EM_MANUTENCAO;
    }
}

StatusEquipamento Equipamento::getStatus(){
    return m_status;
}

void Equipamento::finalizarManutencao() {
    if (m_status == StatusEquipamento::EM_MANUTENCAO) {
        m_status = StatusEquipamento::DISPONIVEL;
    }
}
