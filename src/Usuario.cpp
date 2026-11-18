#include "Usuario.h"

Usuario::Usuario(int id, const std::string& nome, const std::string& cpf, int idade)
    : m_id(id),
      m_nome(nome),
      m_cpf(cpf),
      m_idade(idade) {}

// Destrutor virtual para apagar certo quando o objeto e guardado como Usuario*
Usuario::~Usuario() = default;

//GETTERS
int Usuario::getId() const {
    return m_id;
}

std::string Usuario::getNome() const {
    return m_nome;
}

std::string Usuario::getCpf() const {
    return m_cpf;
}

int Usuario::getIdade() const {
    return m_idade;
}

//SETTERS
void Usuario::setNome(const std::string& nome) {
    m_nome = nome;
}

// Idade negativa nao faz sentido, entao ignora
void Usuario::setIdade(int idade) {
    if (idade >= 0) {
        m_idade = idade;
    }
}
