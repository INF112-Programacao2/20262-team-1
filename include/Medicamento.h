#ifndef MEDICAMENTO_H
#define MEDICAMENTO_H

#include <string>

// Só os dados do remédio em si. Dosagem e quantidade dependem
// da receita, por isso ficam em ItemReceita.
class Medicamento {
private:
    int id;
    std::string nome;
    std::string descricao;

public:
    Medicamento(int id, const std::string& nome, const std::string& descricao);

    std::string consultar() const;   // texto com os dados do medicamento

    int getId() const;
    std::string getNome() const;
    std::string getDescricao() const;

    void setDescricao(const std::string& descricao);
};

#endif