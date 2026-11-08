#ifndef PROCEDIMENTO_H
#define PROCEDIMENTO_H

#include <string>

#include "ProfissionalSaude.h"

class Procedimento {
private:
    int m_id;
    std::string m_nome;
    std::string m_descricao;
    std::string m_tipo;                  // ex.: "Exame", "Cirurgia", "Consulta"
    int m_duracaoMin;
    std::string m_data;                  // ex.: "2026-10-25 14:30"
    ProfissionalSaude* m_responsavel;    // médico/enfermeiro responsável
    bool m_executado;

public:
    Procedimento(int id, const std::string& nome, const std::string& descricao,
                 const std::string& tipo, int duracaoMin,
                 ProfissionalSaude* responsavel);

    void executar();   // o que acontece depende do tipo
    bool foiExecutado() const;

    int getId() const;
    std::string getNome() const;
    std::string getDescricao() const;
    std::string getTipo() const;
    int getDuracaoMin() const;
    std::string getData() const;
    ProfissionalSaude* getResponsavel() const;

    void setData(const std::string& data);
    void setResponsavel(ProfissionalSaude* responsavel);
};

#endif
