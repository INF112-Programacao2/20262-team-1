#ifndef RECEITAMEDICA_H
#define RECEITAMEDICA_H

#include <string>
#include <vector>

#include "ItemReceita.h"   // composição: os itens pertencem à receita
#include "Paciente.h"
#include "Medicamento.h"
#include "ProfissionalSaude.h"


class ReceitaMedica {
private:
    int id;
    std::string data;
    std::string observacoes;
    Paciente* paciente;
    ProfissionalSaude* medico;
    std::vector<ItemReceita> itens;   // "remedios" no diagrama
    bool emitida;                     // depois de emitida não pode mais mudar

public:
    ReceitaMedica(int id, const std::string& data,
                  Paciente* paciente, ProfissionalSaude* medico);

    // Retornam false se a receita já foi emitida
    bool adicionarMedicamento(Medicamento* medicamento, const std::string& dosagem,
                              int quantidade, const std::string& descricaoUso);
    bool removerMedicamento(int indice);

    void emitir();
    std::string gerarTexto() const;   // receita formatada para exibir/imprimir

    // Getters
    int getId() const;
    std::string getData() const;
    std::string getObservacoes() const;
    Paciente* getPaciente() const;
    ProfissionalSaude* getMedico() const;
    const std::vector<ItemReceita>& getItens() const;
    bool foiEmitida() const;

};

#endif