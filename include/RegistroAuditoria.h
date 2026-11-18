#ifndef REGISTRO_AUDITORIA_H
#define REGISTRO_AUDITORIA_H

#include <string>
#include <vector>

class RegistroAuditoria {
private:
    int m_idLog;
    std::string m_timestamp;
    std::vector<std::string> m_historicoLogs;

    std::string gerarTimestampAtual() const;

public:
    RegistroAuditoria();
    ~RegistroAuditoria();

    
    void registrarDecisao(const std::string& acao, const std::string& regra);
    const std::vector<std::string>& consultarHistorico() const;

    int getUltimoIdLog() const;
    std::string getUltimoTimestamp() const;
    void limparHistorico();
};

#endif