#include "../include/SimuladorHospitalar.h"
#include "../include/Setor.h"
#include "../include/Leito.h"
#include "../include/Paciente.h"
#include "../include/Atendimento.h"
#include "../include/FilaAtendimento.h"
#include "../include/GerenciadorFluxo.h"
#include "../include/ServicoNotificacao.h"
#include "../include/RegistroAuditoria.h"
#include "../include/Relatorio.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

SimuladorHospitalar::SimuladorHospitalar(FilaAtendimento* fila,GerenciadorFluxo* gerenciador,ServicoNotificacao* notificador,RegistroAuditoria* auditoria,bool modoHeadless)
    : m_cenarioAtivo("Padrao"),m_modoHeadless(modoHeadless),m_fila(fila),m_gerenciador(gerenciador),m_notificador(notificador),m_auditoria(auditoria) {}

SimuladorHospitalar::~SimuladorHospitalar() {
    // Limpa os pacientes ficticios e libera os leitos antes de destruir o simulador
    resetarSimulacao();
}

void SimuladorHospitalar::registrarLog(const std::string& acao, const std::string& regra) {
    if (m_auditoria != nullptr) {
        m_auditoria->registrarDecisao("[Simulacao: " + m_cenarioAtivo + "] " + acao, regra);
    }
}

void SimuladorHospitalar::imprimirConsole(const std::string& mensagem) const {
    // Só imprime no terminal se nao estiver rodando em modo silencioso (web/API)
    if (!m_modoHeadless) {
        std::cout << "[SIMULADOR - " << m_cenarioAtivo << "] " << mensagem << "\n";
    }
}

void SimuladorHospitalar::simularDisponibilidade(Setor* s, bool disponivel) {
    if (s == nullptr) {
        return;
    }

    if (!disponivel) {
        // Preenche com pacientes ficticios para conseguir reabrir o setor depois 
        std::vector<Leito*> livres = s->consultarLeitosDisponiveis();
        int interditadosAgora = 0;

        for (Leito* leito : livres) {
            if (leito != nullptr) {
                int idFicticio = 8000 + static_cast<int>(m_pacientesSimulados.size());
                Paciente* pInterdicao = new Paciente(
                    idFicticio,
                    "Interdicao_Setor_" + std::to_string(idFicticio),
                    "000.000.000-00",
                    30
                );

                m_pacientesSimulados.push_back(pInterdicao);
                leito->alocarPaciente(pInterdicao);

                if (std::find(m_leitosAlterados.begin(), m_leitosAlterados.end(), leito) == m_leitosAlterados.end()) {
                    m_leitosAlterados.push_back(leito);
                }
                interditadosAgora++;
            }
        }

        std::string msg = "Setor " + s->getNome() + " interditado (" + std::to_string(interditadosAgora) + " leitos indisponiveis).";
        imprimirConsole(msg);
        registrarLog(msg, "Simulacao de indisponibilidade de setor");

    } else {
        // Procura nos leitos que o simulador mexeu quais pertencem a esse setor e libera
        int liberadosAgora = 0;
        auto it = m_leitosAlterados.begin();

        while (it != m_leitosAlterados.end()) {
            Leito* leito = *it;
            if (leito != nullptr && s->buscarLeito(leito->getId()) != nullptr) {
                leito->liberar();
                it = m_leitosAlterados.erase(it);
                liberadosAgora++;
            } else {
                ++it;
            }
        }

        std::string msg = "Setor " + s->getNome() + " reaberto (" + std::to_string(liberadosAgora) + " leitos liberados).";
        imprimirConsole(msg);
        registrarLog(msg, "Simulacao de restauracao de disponibilidade");
    }
}

void SimuladorHospitalar::simularSobrecarga(Setor* s, int qtd) {
    if (s == nullptr || qtd <= 0) {
        return;
    }

    std::vector<Leito*> livres = s->consultarLeitosDisponiveis();
    int totalParaOcupar = std::min(qtd, static_cast<int>(livres.size()));

    for (int i = 0; i < totalParaOcupar; ++i) {
        Leito* leito = livres[i];
        if (leito == nullptr) {
            continue;
        }

        // Cria um paciente ficticio valido para o Leito::alocarPaciente nao rejeitar nullptr
        int idFicticio = 9000 + static_cast<int>(m_pacientesSimulados.size());
        Paciente* pacienteTeste = new Paciente(
            idFicticio,
            "Paciente_Simulado_" + std::to_string(idFicticio),
            "000.000.000-00",
            35
        );

        m_pacientesSimulados.push_back(pacienteTeste);
        leito->alocarPaciente(pacienteTeste);

        if (std::find(m_leitosAlterados.begin(), m_leitosAlterados.end(), leito) == m_leitosAlterados.end()) {
            m_leitosAlterados.push_back(leito);
        }
    }

    std::string msg = "Injetados " + std::to_string(totalParaOcupar) + " pacientes simulados no setor " + s->getNome() + ".";
    imprimirConsole(msg);
    registrarLog(msg, "Teste de estresse / sobrecarga de leitos");

    // Testa na hora se o GerenciadorFluxo detecta a sobrecarga e dispara o alerta
    if (m_gerenciador != nullptr && m_gerenciador->monitorarSobrecargas(s)) {
        imprimirConsole("ALERTA: O setor " + s->getNome() + " entrou em sobrecarga!");
        
        if (m_notificador != nullptr) {
            m_notificador->alertarSobrecarga(s);
        }
        registrarLog("Sobrecarga detectada no setor " + s->getNome(), "Limite critico de lotacao atingido na simulacao");
    }
}

bool SimuladorHospitalar::carregarCenarioEmLote(const std::string& caminho) {
    if (m_fila == nullptr) {
        imprimirConsole("Erro: Fila de atendimento nao conectada ao simulador.");
        return false;
    }

    try {
        // Antes de carregar um lote novo, tira da fila os atendimentos do lote anterior
        for (Atendimento* a : m_processadorLote.getAtendimentosImportados()) {
            m_fila->removerAtendimento(a);
        }

        m_processadorLote.setArquivoEntrada(caminho);
        if (!m_processadorLote.importarDados()) {
            imprimirConsole("Nenhum paciente encontrado no arquivo: " + caminho);
            return false;
        }

        int inseridos = m_processadorLote.processarPacientes(m_fila);
        std::string msg = "Lote carregado (" + std::to_string(inseridos) + " pacientes inseridos na fila).";
        imprimirConsole(msg);
        registrarLog(msg, "Carga de cenario em lote via CSV");

        // Roda a verificacao de gargalo logo apos a carga para registrar se a fila travou
        if (m_gerenciador != nullptr) {
            std::string statusFila = m_gerenciador->detectarGargalos(m_fila);
            imprimirConsole("Diagnostico pos-carga: " + statusFila);
            registrarLog("Diagnostico de fila: " + statusFila, "Monitoramento automatico pos-carga");
        }

        return inseridos > 0;

    } catch (const std::exception& e) {
        std::string erro = "Falha ao carregar cenario em lote: " + std::string(e.what());
        imprimirConsole(erro);
        registrarLog(erro, "Excecao na leitura do arquivo de lote");
        return false;
    }
}

void SimuladorHospitalar::registrarSnapshotCenario(const std::string& nomeCenario, const std::vector<Setor*>& setores) {
    SnapshotCenario snap;
    snap.nome = nomeCenario;
    snap.tamanhoFila = (m_fila != nullptr) ? m_fila->getTamanho() : 0;
    snap.tempoEsperaMax = 0.0;
    snap.totalLeitosOcupados = 0;
    snap.totalCapacidade = 0;
    snap.setoresEmSobrecarga = 0;

    if (m_fila != nullptr && !m_fila->estaVazia()) {
        Atendimento* ultimo = m_fila->getFila().back();
        snap.tempoEsperaMax = m_fila->estimarTempoDeFila(ultimo);
    }

    if (m_gerenciador != nullptr && m_fila != nullptr) {
        snap.diagnosticoFila = m_gerenciador->detectarGargalos(m_fila);
    } else {
        snap.diagnosticoFila = "Nao avaliado";
    }

    for (Setor* s : setores) {
        if (s == nullptr) {
            continue;
        }
        snap.totalLeitosOcupados += s->calcularLotacao();
        snap.totalCapacidade += s->getCapacidade();

        if (m_gerenciador != nullptr && m_gerenciador->monitorarSobrecargas(s)) {
            snap.setoresEmSobrecarga++;
        }
    }

    // Usa a classe Relatorio para guardar tambem o texto detalhado desse momento
    Relatorio rel;
    snap.relatorioCompleto = rel.gerarRelatorioOcupacao(setores, m_gerenciador);

    m_snapshots.push_back(snap);
    imprimirConsole("Snapshot salvo para o cenario: " + nomeCenario);
}

std::string SimuladorHospitalar::compararCenarios() const {
    if (m_snapshots.size() < 2) {
        return "Eh necessario registrar pelo menos 2 cenarios para fazer a comparacao.";
    }

    // Pega os dois ultimos cenarios registrados para comparar o antes e o depois
    const SnapshotCenario& c1 = m_snapshots[m_snapshots.size() - 2];
    const SnapshotCenario& c2 = m_snapshots[m_snapshots.size() - 1];

    double taxaC1 = (c1.totalCapacidade > 0) ? (static_cast<double>(c1.totalLeitosOcupados) / c1.totalCapacidade) * 100.0 : 0.0;
    double taxaC2 = (c2.totalCapacidade > 0) ? (static_cast<double>(c2.totalLeitosOcupados) / c2.totalCapacidade) * 100.0 : 0.0;

    std::ostringstream oss;
    oss << "=== COMPARACAO DE CENARIOS DE SIMULACAO ===\n"
        << "Cenario A: " << c1.nome << "  VS  Cenario B: " << c2.nome << "\n"
        << "-------------------------------------------\n"
        << "1. Tamanho da Fila:\n"
        << "   - " << c1.nome << ": " << c1.tamanhoFila << " pacientes\n"
        << "   - " << c2.nome << ": " << c2.tamanhoFila << " pacientes"
        << " (Variacao: " << (c2.tamanhoFila - c1.tamanhoFila >= 0 ? "+" : "") << (c2.tamanhoFila - c1.tamanhoFila) << ")\n\n"
        << "2. Tempo Maximo de Espera na Fila:\n"
        << std::fixed << std::setprecision(1)
        << "   - " << c1.nome << ": " << c1.tempoEsperaMax << " min\n"
        << "   - " << c2.nome << ": " << c2.tempoEsperaMax << " min\n\n"
        << "3. Ocupacao Geral dos Leitos:\n"
        << "   - " << c1.nome << ": " << c1.totalLeitosOcupados << "/" << c1.totalCapacidade << " (" << taxaC1 << "%)\n"
        << "   - " << c2.nome << ": " << c2.totalLeitosOcupados << "/" << c2.totalCapacidade << " (" << taxaC2 << "%)\n\n"
        << "4. Setores em Sobrecarga:\n"
        << "   - " << c1.nome << ": " << c1.setoresEmSobrecarga << "\n"
        << "   - " << c2.nome << ": " << c2.setoresEmSobrecarga << "\n\n"
        << "5. Gargalos Detectados:\n"
        << "   - " << c1.nome << ": " << c1.diagnosticoFila << "\n"
        << "   - " << c2.nome << ": " << c2.diagnosticoFila << "\n"
        << "===========================================\n";

    return oss.str();
}

void SimuladorHospitalar::resetarSimulacao() {
    // 1. Remove da fila os atendimentos que vieram do lote antes de apagar eles da memoria
    if (m_fila != nullptr) {
        for (Atendimento* a : m_processadorLote.getAtendimentosImportados()) {
            m_fila->removerAtendimento(a);
        }
    }
    m_processadorLote.limparDados();

    // 2. Libera todos os leitos que foram ocupados ou bloqueados pela simulacao
    for (Leito* leito : m_leitosAlterados) {
        if (leito != nullptr) {
            leito->liberar();
        }
    }
    m_leitosAlterados.clear();

    // 3. Apaga os pacientes ficticios criados na simulacao de sobrecarga
    for (Paciente* p : m_pacientesSimulados) {
        delete p;
    }
    m_pacientesSimulados.clear();

    m_snapshots.clear();
}

void SimuladorHospitalar::setCenarioAtivo(const std::string& nome) {
    m_cenarioAtivo = nome;
}

std::string SimuladorHospitalar::getCenarioAtivo() const {
    return m_cenarioAtivo;
}

void SimuladorHospitalar::setModoHeadless(bool headless) {
    m_modoHeadless = headless;
}

bool SimuladorHospitalar::isModoHeadless() const {
    return m_modoHeadless;
}