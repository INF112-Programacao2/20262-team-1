#ifndef GERENCIADOR_FLUXO_HPP
#define GERENCIADOR_FLUXO_HPP

#include <vector>
#include <string>

class Atendimento;
class Setor;
class FilaAtendimento; 

class GerenciadorFluxo {
private:
    int m_limiteLotacaoAlerta;   // Para o Setor (ex: 80%)
    int m_limiteTamanhoFila;     // Quantidade máx de pessoas aceitável na fila
    double m_limiteTempoEspera;  // Tempo máx de espera tolerável (em minutos)

public:
    GerenciadorFluxo(int alertaLotacao, int alertaFila, double alertaTempo );
    ~GerenciadorFluxo();

    Setor* recomendarProximoSetor(Atendimento* a, const std::vector<Setor*>& setoresAtivos);
    bool verificarMudancaSetor(Atendimento* a);
    bool monitorarSobrecargas(Setor* s);
    
    std::string detectarGargalos(const FilaAtendimento* fila) const;

    // Setters/Getters se precisar alterar os limites depois
    void setLimitesFila(int tamanhoMax, double tempoMax);
    void setLimiteLotacaoAlerta(int limite);

    int getLimiteLotacaoAlerta() const;
    int getLimitesFila() const;
};

#endif