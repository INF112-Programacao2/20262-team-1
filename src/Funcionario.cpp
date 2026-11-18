#include "Funcionario.h"
#include "Setor.h"

#include <iostream>

Funcionario::Funcionario(int id, const std::string& nome, const std::string& cpf, int idade,
                         const std::string& matricula)
    : Usuario(id, nome, cpf, idade),
      m_matricula(matricula),
      m_setor(nullptr) {}

// Dados em comum de todo funcionario, as filhas podem acrescentar o resto
void Funcionario::visualizarDados() const {
    std::cout << getCargo() << ": " << getNome()
              << " | Id: " << getId()
              << " | CPF: " << getCpf()
              << " | Idade: " << getIdade()
              << " | Matricula: " << m_matricula;

    if (m_setor != nullptr) {
        std::cout << " | Setor: " << m_setor->getNome();
    }
    std::cout << "\n";
}

//GETTERS
std::string Funcionario::getMatricula() const {
    return m_matricula;
}

Setor* Funcionario::getSetor() const {
    return m_setor;
}

//SETTERS
void Funcionario::setSetor(Setor* setor) {
    m_setor = setor;
}
