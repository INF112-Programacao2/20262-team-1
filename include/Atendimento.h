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
    int id;
    std::string data;
    StatusAtendimento status;
    Paciente* paciente;
    std::vector<ProfissionalSaude*> equipeResponsavel;
    FilaAtendimento* fila;   // fila em que o atendimento está esperando
    // unique_ptr: guarda Exame, Cirurgia... como Procedimento (polimorfismo)
    // e apaga tudo junto com o atendimento (composição)
    std::vector<std::unique_ptr<Procedimento>> procedimentos;

public:
    Atendimento(int id, const std::string& data, Paciente* paciente);

    // Ciclo de vida
    void inicializar();                          // EmEspera -> EmAtendimento
    void atualizarStatus(StatusAtendimento novoStatus);
    bool verificarPendencias() const;            // true se há procedimento não executado
    bool finalizar();                            // false se ainda há pendências

    // Procedimentos
    void adicionarProcedimento(std::unique_ptr<Procedimento> procedimento);
    void executarProcedimentos();                // chama executar() de cada um
    std::vector<Procedimento*> getProcedimentos() const;

    // Equipe
    void adicionarProfissional(ProfissionalSaude* profissional);
    const std::vector<ProfissionalSaude*>& getEquipeResponsavel() const;

    // Getters
    int getId() const;
    std::string getData() const;
    StatusAtendimento getStatus() const;
    Paciente* getPaciente() const;
    FilaAtendimento* getFila() const;

    void setFila(FilaAtendimento* fila);
};

#endif
