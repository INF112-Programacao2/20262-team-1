#ifndef GERENCIADOR_FLUXO_H
#define GERENCIADOR_FLUXO_H

#include <vector>
#include <string>

class Atendimento;
class Setor;
class FilaAtendimento;

class GerenciadorFluxo {
private:
    int m_limiteLotacaoAlerta;
    int m_limiteTamanhoFila;
    double m_limiteTempoEspera;

public:
    GerenciadorFluxo(int alertaLotacao = 80, int alertaFila = 10, double alertaTempo = 60.0);
    ~GerenciadorFluxo();

    Setor* recomendarProximoSetor(Atendimento* a, const std::vector<Setor*>& setoresAtivos);
    bool verificarMudancaSetor(Atendimento* a);
    bool monitorarSobrecargas(Setor* s);
    std::string detectarGargalos(const FilaAtendimento* fila) const;

    void setLimiteLotacaoAlerta(int limite);
    int getLimiteLotacaoAlerta() const;

    void setLimitesFila(int tamanhoMax, double tempoMax);
    int getLimiteTamanhoFila() const;
    double getLimiteTempoEspera() const;
};

#endif