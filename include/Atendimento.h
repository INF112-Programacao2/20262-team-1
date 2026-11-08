#ifndef ATENDIMENTO_H
#define ATENDIMENTO_H

#include <memory>
#include <string>
#include <vector>

#include "FilaAtendimento.h"
#include "StatusAtendimento.h"
#include "Procedimento.h"   // composicao: o atendimento é dono dos procedimentos
#include "ProfissionalSaude.h"
#include "Paciente.h"


class Atendimento {
private:
    // Usados pela FilaAtendimento para ordenar e calcular o tempo de espera
    int m_prioridade;             // de PRIORIDADE_MINIMA (1) a PRIORIDADE_MAXIMA (5)
    double m_tempoEstimadoMin;    // definido ao criar o atendimento
    std::string m_idPaciente;

    int m_id;
    std::string m_data;
    StatusAtendimento m_status;
    Paciente* m_paciente;
    std::vector<ProfissionalSaude*> m_equipeResponsavel;
    FilaAtendimento* m_fila;
    std::vector<Procedimento> m_procedimentos;

public:
    // Ex.: Atendimento(5, 30.0, "P1", 1, "2026-10-31", &paciente)
    Atendimento(int prioridade, double tempoEstimadoMin, const std::string& idPaciente,
                int id, const std::string& data, Paciente* paciente);
    ~Atendimento();

    void inicializar();                          // EM_ESPERA -> EM_ATENDIMENTO
    void atualizarStatus(StatusAtendimento novoStatus);
    bool verificarPendencias() const;            // true se há procedimento não executado
    bool finalizar();                            // false se ainda há pendências

    void adicionarProcedimento(const Procedimento& procedimento);
    void executarProcedimentos();
    const std::vector<Procedimento>& getProcedimentos() const;

    void adicionarProfissional(ProfissionalSaude* profissional);
    const std::vector<ProfissionalSaude*>& getEquipeResponsavel() const;

    int getPrioridade() const;
    void setPrioridade(int novaPrioridade);      // ignora valores fora da faixa
    double getTempoEstimadoMin() const;
    std::string getIdPaciente() const;

    int getId() const;
    std::string getData() const;
    StatusAtendimento getStatus() const;
    Paciente* getPaciente() const;
    FilaAtendimento* getFila() const;

    void setFila(FilaAtendimento* fila);
};


#endif
