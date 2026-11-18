#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include <string>

#include "Funcionario.h"

class Hospital;
class Setor;

// Gerencia a estrutura do hospital (setores)
class Administrador : public Funcionario {
public:
    Administrador(int id, const std::string& nome, const std::string& cpf, int idade,
                  const std::string& matricula);

    std::string getCargo() const override;

    void adicionarSetor(Hospital* hospital, Setor* setor);
    void removerSetor(Hospital* hospital, int idSetor);
};

#endif
