#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include <string>

#include "Usuario.h"

class Setor;

// Base de quem trabalha no hospital. Continua abstrata porque cada cargo diz o seu nome
class Funcionario : public Usuario {
private:
    std::string m_matricula;
    Setor* m_setor;   // nullptr se ainda nao foi alocado em nenhum setor

public:
    Funcionario(int id, const std::string& nome, const std::string& cpf, int idade,
                const std::string& matricula);

    virtual std::string getCargo() const = 0;
    void visualizarDados() const override;

    std::string getMatricula() const;
    Setor* getSetor() const;
    void setSetor(Setor* setor);
};

#endif
