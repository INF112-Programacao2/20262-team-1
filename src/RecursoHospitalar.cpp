#include "../include/RecursoHospitalar.h"

RecursoHospitalar::RecursoHospitalar(int id, std::string nome, std::string descricao) {
    m_id = id;
    m_nome = nome;
    m_descricao = descricao;
}

RecursoHospitalar::~RecursoHospitalar()=default;

int RecursoHospitalar::getId() {
    return m_id;
}

std::string RecursoHospitalar::getNome() {
    return m_nome;
}

std::string RecursoHospitalar::getDescricao() {
    return m_descricao;
}
