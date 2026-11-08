#ifndef RECURSOHOSPITALAR_H
#define RECURSOHOSPITALAR_H

#include <string>

class RecursoHospitalar {
protected:
    int id;
    std::string nome;
    std::string descricao;

public:
    RecursoHospitalar(int id, std::string nome, std::string descricao);
    virtual ~RecursoHospitalar();

    int getId();
    std::string getNome();
    std::string getDescricao();
};
#endif // RECURSOHOSPITALAR_H
