#ifndef PROFISSIONALSAUDE_H
#define PROFISSIONALSAUDE_H

#include <string>

#include "Funcionario.h"

class Atendimento;
class Especialidade;

// Medico ou enfermeiro, quem realmente atende o paciente
class ProfissionalSaude : public Funcionario {
private:
    std::string m_registro;               // CRM ou COREN
    Especialidade* m_especialidade;       // nao e dono, a especialidade pertence ao Setor

public:
    ProfissionalSaude(int id, const std::string& nome, const std::string& cpf, int idade,
                      const std::string& matricula, const std::string& registro,
                      Especialidade* especialidade);

    std::string getCargo() const override;
    void visualizarDados() const override;

    // Entra na equipe do atendimento e coloca ele em andamento
    void realizarAtendimento(Atendimento* atendimento);

    std::string getRegistro() const;
    Especialidade* getEspecialidade() const;
    void setEspecialidade(Especialidade* especialidade);
};

#endif
