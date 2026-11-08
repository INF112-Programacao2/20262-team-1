#include "../include/GerenciadorFluxo.h"
#include "../include/Atendimento.h"
// #include "../include/Setor.h" // mudar quando setor.h estiver pronto

GerenciadorFluxo::GerenciadorFluxo(int limiteAlerta) : limiteLotacaoAlerta(limiteAlerta) {}

GerenciadorFluxo::~GerenciadorFluxo() {}

void GerenciadorFluxo::setLimiteLotacaoAlerta(int limite) {
    limiteLotacaoAlerta = limite;
}

int GerenciadorFluxo::getLimiteLotacaoAlerta() const {
    return limiteLotacaoAlerta;
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
     * ATENÇÃO: Descomente e ajuste essa lógica quando a Pessoa 3 terminar a classe Setor.
     * Ela precisará garantir que os métodos abaixo existam.
     * 
     * int capacidade = s->getCapacidadeMaxima();
     * if (capacidade == 0) return true; // Evita divisão por zero
     * 
     * int ocupacao = s->calcularLotacao();
     * double percentualOcupacao = (static_cast<double>(ocupacao) / capacidade) * 100.0;
     * 
     * return percentualOcupacao >= limiteLotacaoAlerta;
     */
     
    return false; // Retorno provisório para compilar enquanto a classe Setor não existe
}

std::string GerenciadorFluxo::detectarGargalos(Setor* s) {
    if (s == nullptr) {
        return "Erro: Setor não identificado.";
    }

    if (monitorarSobrecargas(s)) {
        return "Gargalo Crítico: Ocupação do setor ultrapassou o limite de " + std::to_string(limiteLotacaoAlerta) + "%.";
    }
    
    /* 
     * Futuramente, você pode usar os métodos do Setor para verificar leitos vazios:
     * if (s->consultarLeitosDisponiveis().empty()) {
     *     return "Gargalo: Fila de espera travada por falta de leitos.";
     * }
     */

    return "Fluxo normal. Sem gargalos detectados.";
}