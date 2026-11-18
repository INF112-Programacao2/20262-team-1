#ifndef ATENDENTE_H
#define ATENDENTE_H

#include <string>

#include "Funcionario.h"

class Atendimento;
class FilaAtendimento;

// Cuida da recepcao: coloca pacientes na fila e chama o proximo
class Atendente : public Funcionario {
public:
    Atendente(int id, const std::string& nome, const std::string& cpf, int idade,
              const std::string& matricula);

    std::string getCargo() const override;

    // false se o atendimento ou a fila forem nulos ou se ele ja estiver na fila
    bool encaminharParaFila(Atendimento* atendimento, FilaAtendimento* fila);

    // nullptr se a fila estiver vazia
    Atendimento* chamarProximo(FilaAtendimento* fila);
};

#endif
