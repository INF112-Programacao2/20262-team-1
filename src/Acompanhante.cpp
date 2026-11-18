#include "Acompanhante.h"

#include <iostream>

// Nasce sem paciente, quem faz a ligacao e o Paciente::setAcompanhante
Acompanhante::Acompanhante(int id, const std::string& nome, const std::string& cpf, int idade,
                           const std::string& parentesco)
    : Usuario(id, nome, cpf, idade),
      m_parentesco(parentesco),
      m_paciente(nullptr) {}

void Acompanhante::visualizarDados() const {
    std::cout << "Acompanhante: " << getNome()
              << " | Id: " << getId()
              << " | CPF: " << getCpf()
              << " | Idade: " << getIdade()
              << " | Parentesco: " << m_parentesco << "\n";
}

//GETTERS
std::string Acompanhante::getParentesco() const {
    return m_parentesco;
}

Paciente* Acompanhante::getPaciente() const {
    return m_paciente;
}

//SETTERS
void Acompanhante::setPaciente(Paciente* paciente) {
    m_paciente = paciente;
}
