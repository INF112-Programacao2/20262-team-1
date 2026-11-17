#include "../include/ServicoNotificacao.h"


#include "../include/ServicoNotificacao.hpp"
#include "../include/Atendimento.hpp"
#include "../include/Setor.h" // Ajuste para "setor.h" se o arquivo estiver em minúsculo

// #include "../include/Usuario.hpp"
// #include "../include/Paciente.hpp"

ServicoNotificacao::ServicoNotificacao() {}

ServicoNotificacao::~ServicoNotificacao() {}

bool ServicoNotificacao::enviarNotificacao(Usuario* u, const std::string& msg) {
    if (u == nullptr || msg.empty()) {
        return false;
    }

    /* DEFINITIVO (quando Usuario.hpp estiver pronto):
     * std::string registro = "[Para: " + u->getNome() + "] " + msg;
     */
    std::string registro = "[Notificacao Direta] " + msg;
    
    historicoNotificacoes.push_back(registro);
    return true;
}

void ServicoNotificacao::notificarMudancaSetor(Atendimento* a) {
    if (a == nullptr) {
        return;
    }

    std::string registro = "[Mudanca de Setor] Paciente ID " + a->getIdPaciente() + 
                           " (Prioridade " + std::to_string(a->getPrioridade()) + 
                           ") deve ser transferido de setor.";
                           
    historicoNotificacoes.push_back(registro);
}

void ServicoNotificacao::alertarSobrecarga(Setor* s) {
    if (s == nullptr) {
        return;
    }

    std::string registro = "[ALERTA DE SOBRECARGA] O setor " + s->getNome() + 
                           " (ID: " + std::to_string(s->getId()) + 
                           ") atingiu o limite critico de lotacao (" + 
                           std::to_string(s->calcularLotacao()) + "/" + 
                           std::to_string(s->getCapacidade()) + ")!";
    
    historicoNotificacoes.push_back(registro);
}

void ServicoNotificacao::chamarPaciente(Paciente* p) {
    if (p == nullptr) {
        return;
    }

    /* DEFINITIVO (quando Paciente.hpp estiver pronto):
     * std::string registro = "[CHAMADA] Paciente " + p->getNome() + ", comparecer ao atendimento.";
     */
    std::string registro = "[CHAMADA] Proximo paciente convocado para atendimento.";
    
    historicoNotificacoes.push_back(registro);
}

const std::vector<std::string>& ServicoNotificacao::getHistoricoNotificacoes() const {
    return historicoNotificacoes;
}

std::string ServicoNotificacao::getUltimaNotificacao() const {
    if (historicoNotificacoes.empty()) {
        return "";
    }
    return historicoNotificacoes.back();
}

void ServicoNotificacao::limparHistorico() {
    historicoNotificacoes.clear();
}