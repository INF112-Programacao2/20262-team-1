#include "../include/GerenciadorFluxo.h"
#include "../include/Atendimento.h"
#include "../include/FilaAtendimento.h"
// #include "../include/Setor.h" // mudar quando setor.h estiver pronto

GerenciadorFluxo::GerenciadorFluxo(int alertaLotacao, int alertaFila, double alertaTempo) 
    : m_limiteLotacaoAlerta(alertaLotacao), m_limiteTamanhoFila(alertaFila), m_limiteTempoEspera(alertaTempo) {}

GerenciadorFluxo::~GerenciadorFluxo() {}

void GerenciadorFluxo::setLimiteLotacaoAlerta(int limite) {
    m_limiteLotacaoAlerta = limite;
}

int GerenciadorFluxo::getLimiteLotacaoAlerta() const {
    return m_limiteLotacaoAlerta;
}

void GerenciadorFluxo::setLimitesFila(int tamanhoMax, double tempoMax){
    m_limiteTamanhoFila = tamanhoMax;
    m_limiteTempoEspera = tempoMax;
}

int GerenciadorFluxo::getLimitesFila() const{
    return m_limiteTamanhoFila, m_limiteTempoEspera;
}

Setor* GerenciadorFluxo::recomendarProximoSetor(Atendimento* a, const std::vector<Setor*>& setoresAtivos) {
    if (a == nullptr || setoresAtivos.empty()) {
        return nullptr;
    }

    // Lógica inteligente: Varre a lista de setores e recomenda o primeiro que NÃO esteja sobrecarregado.
    // No futuro, isso pode ser aprimorado para cruzar a especialidade do Setor com a necessidade do Atendimento.
    for (Setor* setor : setoresAtivos) {
        if (!monitorarSobrecargas(setor)) {
            return setor; 
        }
    }

    return nullptr; // Retorna nulo se o hospital inteiro estiver lotado
}

bool GerenciadorFluxo::verificarMudancaSetor(Atendimento* a) {
    if (a == nullptr) {
        return false;
    }
    
    // Regra de negócio: Se a prioridade do paciente for de emergência (ex: 4 ou 5),
    // o sistema sinaliza que ele precisa de mudança de setor (ex: ir direto para UTI).
    if (a->getPrioridade() >= 4) {
        return true; 
    }

    return false;
}

bool GerenciadorFluxo::monitorarSobrecargas(Setor* s) {
    if (s == nullptr) {
        return false;
    }
    
    /* 
     * ajustar a função quando a Ana terminar a classe Setor.
     *
     * 
     * int capacidade = s->getCapacidadeMaxima();
     * if (capacidade == 0) return true; // Evita divisão por zero
     * 
     * int ocupacao = s->calcularLotacao();
     * double percentualOcupacao = (static_cast<double>(ocupacao) / capacidade) * 100.0;
     * 
     * return percentualOcupacao >= limiteLotacaoAlerta;
     */
     
    return false; 
}

std::string GerenciadorFluxo::detectarGargalos(const FilaAtendimento* fila) const {
    if (fila == nullptr) {
        return "Erro: Fila não identificada.";
    }

    //Verifica se tem muita gente na fila
    if (fila->getTamanho() >= m_limiteTamanhoFila) {
        return "Gargalo Crítico: Fila muito longa! (" + 
               std::to_string(fila->getTamanho()) + " pacientes aguardando).";
    }

    //Verifica o tempo de espera do ÚLTIMO paciente da fila
    // Se a fila não está vazia, o último paciente é o que vai esperar mais tempo
    if (!fila->estaVazia()) {
        const auto& lista = fila->getFila();
        Atendimento* ultimoDaFila = lista.back();
        
        double tempoEsperaTotal = fila->estimarTempoDeFila(ultimoDaFila);
        
        if (tempoEsperaTotal >= m_limiteTempoEspera) {
            return "Gargalo de Tempo: Pacientes esperando mais de " + 
                   std::to_string(m_limiteTempoEspera) + " minutos.";
        }
    }

    return "Fluxo normal na fila. Sem gargalos detectados.";
}