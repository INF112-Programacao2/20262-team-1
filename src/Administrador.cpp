#include "Administrador.h"
#include "Hospital.h"
#include "Setor.h"

constexpr const char* CARGO_ADMINISTRADOR = "Administrador";

Administrador::Administrador(int id, const std::string& nome, const std::string& cpf, int idade,
                             const std::string& matricula)
    : Funcionario(id, nome, cpf, idade, matricula) {}

std::string Administrador::getCargo() const {
    return CARGO_ADMINISTRADOR;
}

// So repassa para o hospital, a regra de guardar os setores e dele
void Administrador::adicionarSetor(Hospital* hospital, Setor* setor) {
    if (hospital == nullptr || setor == nullptr) {
        return;
    }
    hospital->adicionarSetor(setor);
}

void Administrador::removerSetor(Hospital* hospital, int idSetor) {
    if (hospital == nullptr) {
        return;
    }
    hospital->removerSetor(idSetor);
}
