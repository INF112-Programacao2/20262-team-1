#include "../include/Relatorio.h"
#include "../include/Setor.h"
#include "../include/FilaAtendimento.h"
#include "../include/GerenciadorFluxo.h"
#include <fstream>
#include <sstream>
#include <iomanip>

Relatorio::Relatorio() : m_dados(""), m_tipo("GERAL") {}

Relatorio::~Relatorio() {}

std::string Relatorio::gerar() const {
    if (m_dados.empty()) {
        return "Nenhum relatorio gerado ate o momento.";
    }
    return m_dados;
}

bool Relatorio::exportar(const std::string& caminhoArquivo) const {
    if (caminhoArquivo.empty() || m_dados.empty()) {
        return false;
    }

    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        return false;
    }

    arquivo << m_dados;
    arquivo.close();
    return true;
}

std::string Relatorio::gerarRelatorioOcupacao(const std::vector<Setor*>& setores, GerenciadorFluxo* fluxo) {
    m_tipo = "OCUPACAO";
    std::ostringstream oss;

    oss << "=== RELATORIO DE OCUPACAO HOSPITALAR ===\n";
    if (setores.empty()) {
        oss << "Nenhum setor cadastrado para analise.\n";
        m_dados = oss.str();
        return m_dados;
    }

    for (Setor* setor : setores) {
        if (setor == nullptr) {
            continue;
        }

        int ocupacao = setor->calcularLotacao();
        int capacidade = setor->getCapacidade();
        int leitosLivres = static_cast<int>(setor->consultarLeitosDisponiveis().size());
        double percentual = (capacidade > 0) ? (static_cast<double>(ocupacao) / capacidade) * 100.0 : 0.0;

        oss << "Setor: " << setor->getNome() << " (ID: " << setor->getId() << ")\n"<< "  - Lotacao: " << ocupacao << "/" << capacidade << " (" << std::fixed << std::setprecision(1) << percentual << "%)\n"<< "  - Leitos disponiveis: " << leitosLivres << "\n";

        if (fluxo != nullptr) {
            bool sobrecarga = fluxo->monitorarSobrecargas(setor);
            oss << "  - Status: " << (sobrecarga ? "ALERTA DE SOBRECARGA" : "NORMAL") << "\n";
        }
        oss << "----------------------------------------\n";
    }

    m_dados = oss.str();
    return m_dados;
}

std::string Relatorio::gerarRelatorioFilas(const std::vector<FilaAtendimento*>& filas, const GerenciadorFluxo* fluxo) {
    m_tipo = "FILAS";
    std::ostringstream oss;

    oss << "=== RELATORIO DE FILAS DE ATENDIMENTO ===\n";
    if (filas.empty()) {
        oss << "Nenhuma fila ativa no momento.\n";
        m_dados = oss.str();
        return m_dados;
    }

    for (size_t i = 0; i < filas.size(); ++i) {
        FilaAtendimento* fila = filas[i];
        if (fila == nullptr) {
            continue;
        }

        int tamanho = fila->getTamanho();
        double tempoMaxEspera = 0.0;

        if (!fila->estaVazia()) {
            Atendimento* ultimo = fila->getFila().back();
            tempoMaxEspera = fila->estimarTempoDeFila(ultimo);
        }

        oss << "Fila #" << (i + 1) << "\n"<< "  - Pacientes aguardando: " << tamanho << "\n"<< "  - Tempo estimado para o ultimo da fila: "<< std::fixed << std::setprecision(1) << tempoMaxEspera << " min\n";

        if (fluxo != nullptr) {
            oss << "  - Diagnostico de fluxo: " << fluxo->detectarGargalos(fila) << "\n";
        }
        oss << "----------------------------------------\n";
    }

    m_dados = oss.str();
    return m_dados;
}

std::string Relatorio::getDados() const {
    return m_dados;
}

std::string Relatorio::getTipo() const {
    return m_tipo;
}