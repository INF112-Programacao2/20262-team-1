#ifndef LEITO_H
#define LEITO_H

#include "StatusLeito.h"

class Paciente;

class Leito {
private:
    int m_id;
    StatusLeito m_status;
    Paciente* m_paciente;

public:
    ~Leito();
    Leito(int id);

    void alocarPaciente(Paciente* p);
    void bloquear();
    void liberar();
    void desbloquear();
    bool verificarDisponibilidade();
    StatusLeito getStatus();
    Paciente* getPaciente();
    
    int getId();
};
#endif // LEITO_H
