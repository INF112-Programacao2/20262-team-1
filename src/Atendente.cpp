#include "Atendente.h"
#include "Atendimento.h"
#include "FilaAtendimento.h"

constexpr const char* CARGO_ATENDENTE = "Atendente";

Atendente::Atendente(int id, const std::string& nome, const std::string& cpf, int idade,
                     const std::string& matricula)
    : Funcionario(id, nome, cpf, idade, matricula) {}

std::string Atendente::getCargo() const {
    return CARGO_ATENDENTE;
}

// A fila ja reorganiza por prioridade ao adicionar
bool Atendente::encaminharParaFila(Atendimento* atendimento, FilaAtendimento* fila) {
    if (atendimento == nullptr || fila == nullptr) {
        return false;
    }
    if (!fila->adicionarAtendimento(atendimento)) {
        return false;
    }
    atendimento->setFila(fila);
    return true;
}

// Quem sai da fila ja passa para em atendimento
Atendimento* Atendente::chamarProximo(FilaAtendimento* fila) {
    if (fila == nullptr) {
        return nullptr;
    }
    Atendimento* proximo = fila->chamarProximo();
    if (proximo != nullptr) {
        proximo->setFila(nullptr);
        proximo->inicializar();
    }
    return proximo;
}
