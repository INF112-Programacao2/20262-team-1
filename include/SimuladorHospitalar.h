#ifndef SIMULADOR_HOSPITALAR_H
#define SIMULADOR_HOSPITALAR_H

#include <string>
#include <vector>
#include "ProcessadorLote.h"

class Setor;
class Leito;
class Paciente;
class FilaAtendimento;
class GerenciadorFluxo;
class ServicoNotificacao;
class RegistroAuditoria;

// Estrutura simples para guardar os dados de cada rodada e comparar depois
struct SnapshotCenario {
    std::string nome;
    int tamanhoFila;
    double tempoEsperaMax;
    int totalLeitosOcupados;
    int totalCapacidade;
    int setoresEmSobrecarga;
    std::string diagnosticoFila;
    std::string relatorioCompleto;
};

class SimuladorHospitalar {
private:
    std::string m_cenarioAtivo;
    bool m_modoHeadless;

    // Ponteiros para os modulos principais que vao reagir aos testes
    FilaAtendimento* m_fila;
    GerenciadorFluxo* m_gerenciador;
    ServicoNotificacao* m_notificador;
    RegistroAuditoria* m_auditoria;

    // Guarda o que foi criado/alterado na simulacao para poder limpar tudo no final
    ProcessadorLote m_processadorLote;
    std::vector<Paciente*> m_pacientesSimulados;
    std::vector<Leito*> m_leitosAlterados;
    std::vector<SnapshotCenario> m_snapshots;

    void registrarLog(const std::string& acao, const std::string& regra);
    void imprimirConsole(const std::string& mensagem) const;

public:
    SimuladorHospitalar(FilaAtendimento* fila,
                        GerenciadorFluxo* gerenciador,
                        ServicoNotificacao* notificador = nullptr,
                        RegistroAuditoria* auditoria = nullptr,
                        bool modoHeadless = true);
    ~SimuladorHospitalar();

    // Metodos principais do diagrama de classes
    void simularDisponibilidade(Setor* s, bool disponivel);
    void simularSobrecarga(Setor* s, int qtd);
    bool carregarCenarioEmLote(const std::string& caminho);
    std::string compararCenarios() const;

    // Controle de cenarios e limpeza de memoria
    void registrarSnapshotCenario(const std::string& nomeCenario, const std::vector<Setor*>& setores);
    void resetarSimulacao();

    void setCenarioAtivo(const std::string& nome);
    std::string getCenarioAtivo() const;

    void setModoHeadless(bool headless);
    bool isModoHeadless() const;
};

#endif // SIMULADOR_HOSPITALAR_H