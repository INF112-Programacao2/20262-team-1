#include "../include/GerenciadorFluxo.h"
#include "../include/Atendimento.h"
#include "../include/FilaAtendimento.h"
#include "../include/Setor.h"

GerenciadorFluxo::GerenciadorFluxo(int alertaLotacao, int alertaFila, double alertaTempo) 
    : m_limiteLotacaoAlerta(alertaLotacao), m_limiteTamanhoFila(alertaFila), m_limiteTempoEspera(alertaTempo) {}

GerenciadorFluxo::~GerenciadorFluxo() {}

void GerenciadorFluxo::setLimiteLotacaoAlerta(int limite) {
    m_limiteLotacaoAlerta = limite;
}

int GerenciadorFluxo::getLimiteLotacaoAlerta() const {
    return m_limiteLotacaoAlerta;
}

void GerenciadorFluxo::setLimitesFila(int tamanhoMax, double tempoMax) {
    m_limiteTamanhoFila = tamanhoMax;
    m_limiteTempoEspera = tempoMax;
}

int GerenciadorFluxo::getLimiteTamanhoFila() const {
    return m_limiteTamanhoFila;
}

double GerenciadorFluxo::getLimiteTempoEspera() const {
    return m_limiteTempoEspera;
}

Setor* GerenciadorFluxo::recomendarProximoSetor(Atendimento* a, const std::vector<Setor*>& setoresAtivos) {
    if (a == nullptr || setoresAtivos.empty()) {
        return nullptr;
    }

    Setor* melhorSetor = nullptr;
    double menorTaxaOcupacao = 101.0; // Começa acima de 100% para encontrar o menor
    bool isEmergencia = (a->getPrioridade() >= 4);

    for (Setor* setor : setoresAtivos) {
        if (setor == nullptr || monitorarSobrecargas(setor)) {
            continue;
        }

        // Se for urgência/emergência (prioridade 4 ou 5), exige que o setor tenha leito livre
        if (isEmergencia && setor->consultarLeitosDisponiveis().empty()) {
            continue;
        }

        int capacidade = setor->getCapacidade();
        if (capacidade <= 0) {
            continue;
        }

        double taxaOcupacao = (static_cast<double>(setor->calcularLotacao()) / capacidade) * 100.0;

        // Balanceamento de carga: escolhe o setor com menor percentual de lotação
        if (taxaOcupacao < menorTaxaOcupacao) {
            menorTaxaOcupacao = taxaOcupacao;
            melhorSetor = setor;
        }
    }

    return melhorSetor;
}

bool GerenciadorFluxo::verificarMudancaSetor(Atendimento* a) {
    if (a == nullptr) {
        return false;
    }
    
    // Se a prioridade do paciente subir para urgente/emergência (4 ou 5),
    // sinaliza necessidade de transferência (ex: da triagem/enfermaria para UTI)
    return a->getPrioridade() >= 4;
}

bool GerenciadorFluxo::monitorarSobrecargas(Setor* s) {
    if (s == nullptr) {
        return false;
    }
    
    int capacidade = s->getCapacidade();
    if (capacidade <= 0) {
        return true; // Evita divisão por zero e considera setor sem capacidade como indisponível
    }
    
    int ocupacao = s->calcularLotacao();
    double percentualOcupacao = (static_cast<double>(ocupacao) / capacidade) * 100.0;
    
    return percentualOcupacao >= m_limiteLotacaoAlerta;
}

std::string GerenciadorFluxo::detectarGargalos(const FilaAtendimento* fila) const {
    if (fila == nullptr) {
        return "Erro: Fila nao identificada.";
    }

    // 1. Verifica volume de pacientes na fila
    if (fila->getTamanho() >= m_limiteTamanhoFila) {
        return "Gargalo Critico: Fila muito longa! (" + 
               std::to_string(fila->getTamanho()) + " pacientes aguardando).";
    }

    // 2. Verifica o tempo de espera projetado para o último paciente da fila
    if (!fila->estaVazia()) {
        const auto& lista = fila->getFila();
        Atendimento* ultimoDaFila = lista.back();
        
        double tempoEsperaTotal = fila->estimarTempoDeFila(ultimoDaFila);
        
        if (tempoEsperaTotal >= m_limiteTempoEspera) {
            return "Gargalo de Tempo: Pacientes esperando mais de " + 
                   std::to_string(static_cast<int>(m_limiteTempoEspera)) + " minutos.";
        }
    }

    return "Fluxo normal na fila. Sem gargalos detectados.";
}