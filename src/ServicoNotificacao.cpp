#include "../include/ServicoNotificacao.h"
#include "../include/Atendimento.h"
#include "../include/Setor.h"
#include "../include/Usuario.h"
#include "../include/Paciente.h"

ServicoNotificacao::ServicoNotificacao() {}

ServicoNotificacao::~ServicoNotificacao() {}

bool ServicoNotificacao::enviarNotificacao(Usuario* u, const std::string& msg) {
    if (u == nullptr || msg.empty()) {
        return false;
    }

    std::string registro = "[Para: " + u->getNome() + 
                           " (ID: " + std::to_string(u->getId()) + ")] " + msg;
    
    historicoNotificacoes.push_back(registro);
    return true;
}

void ServicoNotificacao::notificarMudancaSetor(Atendimento* a) {
    if (a == nullptr) {
        return;
    }

    int prioridadeNum = static_cast<int>(a->getPrioridade());
    std::string registro = "[Mudanca de Setor] Paciente ID " + a->getIdPaciente() + 
                           " (Prioridade " + std::to_string(prioridadeNum) + 
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

    std::string registro = "[CHAMADA] Paciente " + p->getNome() + 
                           " (ID: " + std::to_string(p->getId()) + 
                           "), comparecer ao atendimento.";
    
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