#ifndef MEDICAMENTO_H
#define MEDICAMENTO_H

#include <string>

// Dosagem e quantidade dependem da receita, por isso ficam em ItemReceita
class Medicamento {
private:
    int m_id;
    std::string m_nome;
    std::string m_descricao;

public:
    Medicamento(int id, const std::string& nome, const std::string& descricao);

    std::string consultar() const;   // texto com os dados do medicamento

    int getId() const;
    std::string getNome() const;
    std::string getDescricao() const;

    void setDescricao(const std::string& descricao);
};
#endif