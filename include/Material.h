#ifndef MATERIAL_H
#define MATERIAL_H

#include "RecursoHospitalar.h"

#include <string>

class Material : public RecursoHospitalar {
private:
    std::string m_unidadeMedida;
    double m_quantidade;

public:
    Material(int id, std::string nome, const std::string descricao, std::string unidadeMedida, double quantidade);

    void retirar(double qtd);
    void reabastecer(double qtd);
    double getQuantidade();
    std::string getUnidadeMedida();
};
#endif // MATERIAL_H
