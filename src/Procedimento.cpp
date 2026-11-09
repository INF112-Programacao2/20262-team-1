#include "Procedimento.h"

#include <iostream>

constexpr const char* TIPO_EXAME = "Exame";
constexpr const char* TIPO_CIRURGIA = "Cirurgia";
constexpr const char* TIPO_CONSULTA = "Consulta";

// Começa sem data marcada e ainda não executado
Procedimento::Procedimento(int id, const std::string& nome, const std::string& descricao,
                           const std::string& tipo, int duracaoMin,
                           ProfissionalSaude* responsavel)
    : m_id(id),
      m_nome(nome),
      m_descricao(descricao),
      m_tipo(tipo),
      m_duracaoMin(duracaoMin),
      m_responsavel(responsavel),
      m_executado(false) {}

// Executar duas vezes não tem efeito: o procedimento só é realizado uma vez
void Procedimento::executar() {
    if (m_executado) {
        return;
    }

    if (m_tipo == TIPO_EXAME) {
        std::cout << "Realizando exame: " << m_nome << " (resultado será registrado)\n";
    } else if (m_tipo == TIPO_CIRURGIA) {
        std::cout << "Realizando cirurgia: " << m_nome << " (" << m_duracaoMin << " min)\n";
    } else if (m_tipo == TIPO_CONSULTA) {
        std::cout << "Realizando consulta: " << m_nome << "\n";
    } else {
        std::cout << "Realizando procedimento: " << m_nome << "\n";
    }

    m_executado = true;
}


//IS
bool Procedimento::foiExecutado() const {
    return m_executado;
}

//GETTERS
int Procedimento::getId() const {
    return m_id;
}

std::string Procedimento::getNome() const {
    return m_nome;
}

std::string Procedimento::getDescricao() const {
    return m_descricao;
}

std::string Procedimento::getTipo() const {
    return m_tipo;
}

int Procedimento::getDuracaoMin() const {
    return m_duracaoMin;
}

std::string Procedimento::getData() const {
    return m_data;
}

ProfissionalSaude* Procedimento::getResponsavel() const {
    return m_responsavel;
}


//SETTERS
void Procedimento::setData(const std::string& data) {
    m_data = data;
}

void Procedimento::setResponsavel(ProfissionalSaude* responsavel) {
    m_responsavel = responsavel;
}


