#ifndef EQUIPAMENTO_H
#define EQUIPAMENTO_H

#include "RecursoHospitalar.h"
#include "StatusEquipamento.h"

class Equipamento : public RecursoHospitalar {
private:
    StatusEquipamento m_status;

public:
    Equipamento(int id, std::string nome, std::string descricao);
    ~Equipamento();

    bool verificarDisponibilidade();
    void reservar();
    void liberar();
    void iniciarManutencao();
    void finalizarManutencao();
    StatusEquipamento getStatus();
};
#endif // EQUIPAMENTO_H
