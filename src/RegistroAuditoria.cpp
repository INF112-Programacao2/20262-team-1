#include "../include/RegistroAuditoria.h"
#include <ctime>
#include <cstdio>

RegistroAuditoria::RegistroAuditoria() : m_idLog(0), m_timestamp("") {}

RegistroAuditoria::~RegistroAuditoria() {}

std::string RegistroAuditoria::gerarTimestampAtual() const {
    std::time_t agora = std::time(nullptr);
    std::tm* tempoLocal = std::localtime(&agora);

    if (tempoLocal == nullptr) {
        return "0000-00-00 00:00:00";
    }

    char buffer[64]; // Cria espaço temporário para os caracteres do tempoLocal
    std::snprintf(
        buffer, // Guarda o resultado em buffer
        sizeof(buffer), // trava o limite em 64 nytes para evitar estouro de memória
        "%04d-%02d-%02d %02d:%02d:%02d", // Formatação "AAAA-MM-DD HH::MM::SS"
        tempoLocal->tm_year + 1900,
        tempoLocal->tm_mon + 1,
        tempoLocal->tm_mday,
        tempoLocal->tm_hour,
        tempoLocal->tm_min,
        tempoLocal->tm_sec
    );

    return std::string(buffer);
}

void RegistroAuditoria::registrarDecisao(const std::string& acao, const std::string& regra) {
    if (acao.empty()) {
        return;
    }

    m_idLog++;
    m_timestamp = gerarTimestampAtual();

    std::string regraAplicada = regra.empty() ? "Nao especificada" : regra;
    std::string entrada = "[Log #" + std::to_string(m_idLog) + " | " + m_timestamp + "] Acao: " + acao + " | Regra: " + regraAplicada;

    m_historicoLogs.push_back(entrada);
}

const std::vector<std::string>& RegistroAuditoria::consultarHistorico() const {
    return m_historicoLogs;
}

int RegistroAuditoria::getUltimoIdLog() const {
    return m_idLog;
}

std::string RegistroAuditoria::getUltimoTimestamp() const {
    return m_timestamp;
}

void RegistroAuditoria::limparHistorico() {
    m_historicoLogs.clear();
}