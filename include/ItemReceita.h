#ifndef ITEMRECEITA_H
#define ITEMRECEITA_H


#include <string>

#include "Medicamento.h"   // associação: o mesmo medicamento aparece em várias receitas

// Uma linha da receita: qual medicamento, quanto e como usar
class ItemReceita {
private:
    Medicamento* medicamento;
    std::string dosagem;        // ex.: "500 mg"
    int quantidade;             // unidades prescritas
    std::string descricaoUso;   // ex.: "1 comprimido a cada 8 horas por 7 dias"

public:
    ItemReceita(Medicamento* medicamento, const std::string& dosagem,
                int quantidade, const std::string& descricaoUso);

    Medicamento* getMedicamento() const;
    std::string getDosagem() const;
    int getQuantidade() const;
    std::string getDescricaoUso() const;

    void setDosagem(const std::string& dosagem);
    void setQuantidade(int quantidade);
    void setDescricaoUso(const std::string& descricaoUso);
};

#endif