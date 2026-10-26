#ifndef PACIENTE_H
#define PACIENTE_H

#include <string>
#include <vector>
#include "Usuario.h"         // Pessoa 4: heranca precisa da classe completa
#include "Prioridade.h"
#include "StatusAtendimento.h"
#include "ReceitaMedica.h"   // composicao: o paciente guarda suas receitas


class Paciente : public Usuario {
private:
    Prioridade prioridade;
    std::vector<ReceitaMedica> receitas;
    Acompanhante* acompanhante;               // nullptr se nao tiver
    std::vector<Atendimento*> atendimentos;   // historico

public:
    Paciente(int id, const std::string& nome, const std::string& cpf, int idade); // heranca do Usuario

    void visualizarDados() const override;

    // Atendimentos
    void adicionarAtendimento(Atendimento* atendimento);   // solicitar atendimento
    Atendimento* getAtendimentoAtual() const;              // nullptr se nao houver
    void acompanharAtendimento() const;                    // mostra status e posicao na fila
    StatusAtendimento consultarStatusAtendimento() const;
    std::vector<ProfissionalSaude*> visualizarEquipeResponsavel() const;
    const std::vector<Atendimento*>& getAtendimentos() const;

    // Prioridade
    void atualizarPrioridade(Prioridade novaPrioridade);
    Prioridade getPrioridade() const;

    // Receitas
    void adicionarReceita(const ReceitaMedica& receita);
    const std::vector<ReceitaMedica>& getReceitas() const;

    // Acompanhante
    bool temAcompanhante() const;
    Acompanhante* getAcompanhante() const;
    void setAcompanhante(Acompanhante* acompanhante);
};

#endif


