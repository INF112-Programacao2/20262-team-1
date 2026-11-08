#ifndef PACIENTE_H
#define PACIENTE_H

#include <string>
#include <vector>
#include "Usuario.h"         // Pessoa 4: heranca precisa da classe completa
#include "Prioridade.h"
#include "StatusAtendimento.h"
#include "ReceitaMedica.h"   // composicao: o paciente guarda suas receitas


class Atendimento;
class Acompanhante;
class ProfissionalSaude;

class Paciente : public Usuario {
private:
    Prioridade m_prioridade;
    std::vector<ReceitaMedica> m_receitas;
    Acompanhante* m_acompanhante;               // nullptr se não tiver
    std::vector<Atendimento*> m_atendimentos;   // histórico

public:
    Paciente(int id, const std::string& nome, const std::string& cpf, int idade);

    void visualizarDados() const override;

    void adicionarAtendimento(Atendimento* atendimento);
    Atendimento* getAtendimentoAtual() const;    // nullptr se não houver
    void acompanharAtendimento() const;          // mostra status e posição na fila
    StatusAtendimento consultarStatusAtendimento() const;
    std::vector<ProfissionalSaude*> visualizarEquipeResponsavel() const;
    const std::vector<Atendimento*>& getAtendimentos() const;

    // Também atualiza a prioridade do atendimento atual, se houver
    void atualizarPrioridade(Prioridade novaPrioridade);
    Prioridade getPrioridade() const;

    void adicionarReceita(const ReceitaMedica& receita);
    const std::vector<ReceitaMedica>& getReceitas() const;

    bool temAcompanhante() const;
    Acompanhante* getAcompanhante() const;
    void setAcompanhante(Acompanhante* acompanhante);
};

#endif

