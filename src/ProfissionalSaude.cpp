#include "ProfissionalSaude.h"
#include "Atendimento.h"
#include "Especialidade.h"

#include <iostream>

constexpr const char* CARGO_PROFISSIONAL = "Profissional de saude";

ProfissionalSaude::ProfissionalSaude(int id, const std::string& nome, const std::string& cpf,
                                     int idade, const std::string& matricula,
                                     const std::string& registro, Especialidade* especialidade)
    : Funcionario(id, nome, cpf, idade, matricula),
      m_registro(registro),
      m_especialidade(especialidade) {}

std::string ProfissionalSaude::getCargo() const {
    return CARGO_PROFISSIONAL;
}

void ProfissionalSaude::visualizarDados() const {
    Funcionario::visualizarDados();
    std::cout << "   Registro: " << m_registro;
    if (m_especialidade != nullptr) {
        std::cout << " | Especialidade: " << m_especialidade->getNome();
    }
    std::cout << "\n";
}

// adicionarProfissional ja ignora repetido, entao chamar duas vezes nao duplica na equipe
void ProfissionalSaude::realizarAtendimento(Atendimento* atendimento) {
    if (atendimento == nullptr) {
        return;
    }
    atendimento->adicionarProfissional(this);
    atendimento->inicializar();
}

//GETTERS
std::string ProfissionalSaude::getRegistro() const {
    return m_registro;
}

Especialidade* ProfissionalSaude::getEspecialidade() const {
    return m_especialidade;
}

//SETTERS
void ProfissionalSaude::setEspecialidade(Especialidade* especialidade) {
    m_especialidade = especialidade;
}
