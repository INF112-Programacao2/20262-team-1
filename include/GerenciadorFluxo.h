#ifndef GERENCIADOR_FLUXO_HPP
#define GERENCIADOR_FLUXO_HPP

#include <vector>
#include <string>

class Atendimento;
class Setor;

class GerenciadorFluxo {
private:
    int limiteLotacaoAlerta; //limite para geração do alerta

public:
    GerenciadorFluxo(int limiteAlerta = 80);
    ~GerenciadorFluxo();

    // Recebe a lista de setores do hospital para poder analisar e recomendar o melhor
    Setor* recomendarProximoSetor(Atendimento* a, const std::vector<Setor*>& setoresAtivos);
    
    bool verificarMudancaSetor(Atendimento* a);

    // Analisa a lotação do Setor
    bool monitorarSobrecargas(Setor* s);

    std::string detectarGargalos(Setor* s);

    void setLimiteLotacaoAlerta(int limite);
    int getLimiteLotacaoAlerta() const;
};

#endif