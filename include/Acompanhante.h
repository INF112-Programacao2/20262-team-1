#ifndef ACOMPANHANTE_H
#define ACOMPANHANTE_H

#include <string>

#include "Usuario.h"

class Paciente;

// Quem acompanha um paciente durante o atendimento
class Acompanhante : public Usuario {
private:
    std::string m_parentesco;   // ex.: "mae", "filho"
    Paciente* m_paciente;       // nullptr enquanto nao estiver ligado a um paciente

public:
    Acompanhante(int id, const std::string& nome, const std::string& cpf, int idade,
                 const std::string& parentesco);

    void visualizarDados() const override;

    std::string getParentesco() const;
    Paciente* getPaciente() const;
    void setPaciente(Paciente* paciente);
};

#endif
