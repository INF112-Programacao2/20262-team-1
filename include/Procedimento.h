#ifndef PROCEDIMENTO_H
#define PROCEDIMENTO_H

#include <string>

#include "ProfissionalSaude.h"

class Procedimento {
protected:
    int id;
    std::string nome;
    std::string descricao;
    int duracaoMin;                    // duração estimada em minutos
    std::string data;                  // ex.: "2026-10-25 14:30"
    ProfissionalSaude* responsavel;    // médico/enfermeiro responsável
    bool executado;

public:
    Procedimento(int id, const std::string& nome, const std::string& descricao,
                 int duracaoMin, ProfissionalSaude* responsavel);
    virtual ~Procedimento() = default;

    virtual void executar() = 0;              // cada subclasse implementa
    virtual std::string getTipo() const = 0;  // "Exame", "Cirurgia", ...

    bool foiExecutado() const;

    // Getters
    int getId() const;
    std::string getNome() const;
    std::string getDescricao() const;
    int getDuracaoMin() const;
    std::string getData() const;
    ProfissionalSaude* getResponsavel() const;

    // Setters
    void setData(const std::string& data);
    void setResponsavel(ProfissionalSaude* responsavel);
};

#endif
