#ifndef EQUIPAMENTO_H
#define EQUIPAMENTO_H

#include "RecursoHospitalar.h"
#include "StatusEquipamento.h"

class Equipamento : public RecursoHospitalar {
private:
    StatusEquipamento status;

public:
    Equipamento(int id, std::string nome, std::string descricao);
    ~Equipamento();

    bool verificarDisponibilidade();
    void reservar();
    void liberar();
    void iniciarManutencao();
    void finalizarManutencao();
};
#endif // EQUIPAMENTO_H
