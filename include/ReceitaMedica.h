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
    int m_id;
    std::string m_data;
    std::string m_observacoes;
    Paciente* m_paciente;
    ProfissionalSaude* m_medico;
    std::vector<ItemReceita> m_itens;
    bool m_emitida;   // depois de emitida, a receita não pode mais mudar

public:
    ReceitaMedica(int id, const std::string& data,
                  Paciente* paciente, ProfissionalSaude* medico);

    // Retornam false se a receita já foi emitida
    bool adicionarMedicamento(Medicamento* medicamento, const std::string& dosagem,
                              int quantidade, const std::string& descricaoUso);
    bool removerMedicamento(int indice);

    void emitir();
    std::string gerarTexto() const;   // receita formatada para exibir/imprimir

    int getId() const;
    std::string getData() const;
    std::string getObservacoes() const;
    Paciente* getPaciente() const;
    ProfissionalSaude* getMedico() const;
    const std::vector<ItemReceita>& getItens() const;
    bool foiEmitida() const;

    void setObservacoes(const std::string& observacoes);
};

#endif