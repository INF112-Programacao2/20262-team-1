#ifndef LEITO_H
#define LEITO_H

#include "StatusLeito.h"

class Paciente;

class Leito {
private:
    int id;
    StatusLeito status;
    Paciente* paciente;

public:
    ~Leito();
    Leito(int id);

    void alocarPaciente(Paciente* p);
    void bloquear();
    void liberar();
    bool verificarDisponibilidade();
    
    int getId();
};
#endif // LEITO_H
