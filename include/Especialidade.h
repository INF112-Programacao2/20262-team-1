#ifndef ESPECIALIDADE_H
#define ESPECIALIDADE_H

#include <string>

class Especialidade {
private:
    int id;
    std::string nome;
    std::string descricao;

public:
    Especialidade(int id, std::string nome, std::string descricao);
    ~Especialidade();
    
    int getId();
    std::string getNome();
    std::string getDescricao();
};
#endif // ESPECIALIDADE_H
