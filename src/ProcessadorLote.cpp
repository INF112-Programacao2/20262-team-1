#include "../include/ProcessadorLote.h"
#include "../include/Paciente.h"
#include "../include/Atendimento.h"
#include "../include/FilaAtendimento.h"
#include "../include/Prioridade.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

ProcessadorLote::ProcessadorLote(const std::string& arquivoEntrada)
    : m_arquivoEntrada(arquivoEntrada) {}

ProcessadorLote::~ProcessadorLote() {
    // Garante que tudo que foi alocado com new seja apagado quando o objeto morrer
    limparDados();
}

void ProcessadorLote::setArquivoEntrada(const std::string& caminho) {
    m_arquivoEntrada = caminho;
}

std::string ProcessadorLote::getArquivoEntrada() const {
    return m_arquivoEntrada;
}

void ProcessadorLote::limparDados() {
    // Apaga os atendimentos e pacientes da memoria para nao dar vazamento (memory leak)
    for (Atendimento* a : m_atendimentosImportados) {
        delete a;
    }
    m_atendimentosImportados.clear();

    for (Paciente* p : m_pacientesImportados) {
        delete p;
    }
    m_pacientesImportados.clear();
}

bool ProcessadorLote::importarDados() {
    if (m_arquivoEntrada.empty()) {
        throw std::invalid_argument("Caminho do arquivo de entrada nao informado.");
    }

    std::ifstream arquivo(m_arquivoEntrada);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + m_arquivoEntrada);
    }

    // Limpa antes de ler para nao misturar com dados de uma importacao antiga
    limparDados();

    std::string linha;
    int numeroLinha = 0;

    // Formato esperado do CSV:
    // idPaciente;nome;cpf;idade;prioridade;tempoEstimadoMin;idAtendimento;data
    while (std::getline(arquivo, linha)) {
        numeroLinha++;

        // Pula linhas em branco ou comentarios no arquivo
        if (linha.empty() || linha[0] == '#') {
            continue;
        }

        // Se a primeira linha for o cabecalho do CSV, ignora ela
        if (numeroLinha == 1 && linha.find("idPaciente") != std::string::npos) {
            continue;
        }

        // Quebra a linha nos ';' e guarda cada pedaco no vetor colunas
        std::stringstream ss(linha);
        std::string campo;
        std::vector<std::string> colunas;

        while (std::getline(ss, campo, ';')) {
            colunas.push_back(campo);
        }

        if (colunas.size() < 8) {
            arquivo.close();
            limparDados();
            throw std::runtime_error("Erro na linha " + std::to_string(numeroLinha) + ": colunas insuficientes.");
        }

        Paciente* paciente = nullptr;
        Atendimento* atendimento = nullptr;

        try {
            // Converte os textos do CSV para os tipos certos
            int idPaciente = std::stoi(colunas[0]);
            std::string nome = colunas[1];
            std::string cpf = colunas[2];
            int idade = std::stoi(colunas[3]);
            int prioridade = std::stoi(colunas[4]);
            double tempoEstimado = std::stod(colunas[5]);
            int idAtendimento = std::stoi(colunas[6]);
            std::string data = colunas[7];

            // Trava a prioridade entre 1 e 5 caso venha algum numero fora da faixa no arquivo
            prioridade = std::max(PRIORIDADE_MINIMA, std::min(PRIORIDADE_MAXIMA, prioridade));

            // Cria o paciente e ja seta a prioridade dele usando o enum
            paciente = new Paciente(idPaciente, nome, cpf, idade);
            paciente->atualizarPrioridade(static_cast<Prioridade>(prioridade));

            // Monta o codigo tipo "P1", "P2" e cria o atendimento vinculado a esse paciente
            std::string codigoPaciente = "P" + std::to_string(idPaciente);
            atendimento = new Atendimento(
                prioridade,
                tempoEstimado,
                codigoPaciente,
                idAtendimento,
                data,
                paciente
            );

            // Salva o atendimento no historico do proprio paciente e guarda os ponteiros aqui
            paciente->adicionarAtendimento(atendimento);

            m_pacientesImportados.push_back(paciente);
            m_atendimentosImportados.push_back(atendimento);

        } catch (const std::exception& e) {
            // Se der qualquer erro no meio, limpa o que ja tinha sido alocado antes de estourar o erro
            delete atendimento;
            delete paciente;
            arquivo.close();
            limparDados();
            throw std::runtime_error("Erro na linha " + std::to_string(numeroLinha) + ": " + e.what());
        }
    }

    arquivo.close();
    return !m_atendimentosImportados.empty();
}

int ProcessadorLote::processarPacientes(FilaAtendimento* fila) {
    if (fila == nullptr) {
        throw std::invalid_argument("Fila de atendimento invalida (nula).");
    }

    if (m_atendimentosImportados.empty()) {
        return 0;
    }

    int enfileirados = 0;

    // Vai jogando cada atendimento importado na fila (ela mesma ja ordena por prioridade)
    for (Atendimento* atendimento : m_atendimentosImportados) {
        if (atendimento != nullptr && fila->adicionarAtendimento(atendimento)) {
            enfileirados++;
        }
    }

    return enfileirados;
}

const std::vector<Paciente*>& ProcessadorLote::getPacientesImportados() const {
    return m_pacientesImportados;
}

const std::vector<Atendimento*>& ProcessadorLote::getAtendimentosImportados() const {
    return m_atendimentosImportados;
}

int ProcessadorLote::getQuantidadeImportada() const {
    return static_cast<int>(m_atendimentosImportados.size());
}