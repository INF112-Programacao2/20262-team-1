#include "Medicamento.h"

#include <sstream>

// Dosagem e quantidade não ficam aqui: dependem da receita (ver ItemReceita)
Medicamento::Medicamento(int id, const std::string& nome, const std::string& descricao)
    : m_id(id),
      m_nome(nome),
      m_descricao(descricao) {}

// Monta um texto com os dados do medicamento, pronto para exibir
std::string Medicamento::consultar() const {
    std::ostringstream texto;
    texto << "Medicamento #" << m_id << ": " << m_nome;
    if (!m_descricao.empty()) {
        texto << " - " << m_descricao;
    }
    return texto.str();
}


//GETTERS
int Medicamento::getId() const {
    return m_id;
}

std::string Medicamento::getNome() const {
    return m_nome;
}

std::string Medicamento::getDescricao() const {
    return m_descricao;
}


//SETTERS
void Medicamento::setDescricao(const std::string& descricao) {
    m_descricao = descricao;
}
