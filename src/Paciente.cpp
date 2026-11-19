#include "Paciente.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "Atendimento.h"
#include "FilaAtendimento.h"

// Todo paciente começa como NAO_URGENTE e sem acompanhante
Paciente::Paciente(int id, const std::string& nome, const std::string& cpf, int idade)
    : Usuario(id, nome, cpf, idade),
      m_prioridade(Prioridade::NAO_URGENTE),
      m_acompanhante(nullptr) {}

void Paciente::visualizarDados() const {
    std::cout << "\n--- DADOS DO PACIENTE ---\n";
    std::cout << "  ID: " << getId() << "\n";
    std::cout << "  Nome: " << getNome() << "\n";
    std::cout << "  CPF: " << getCpf() << "\n";
    std::cout << "  Idade: " << getIdade() << "\n";
    std::cout << "  Prioridade: " << static_cast<int>(m_prioridade) << "\n";
    std::cout << "  Acompanhante: " << (temAcompanhante() ? "SIM" : "NAO") << "\n";
    std::cout << "  Atendimentos no historico: " << m_atendimentos.size() << "\n";
    std::cout << "  Receitas: " << m_receitas.size() << "\n";
    std::cout << "-------------------------\n";
}

// O mesmo atendimento não entra duas vezes no histórico
void Paciente::adicionarAtendimento(Atendimento* atendimento) {
    if (atendimento == nullptr) {
        return;
    }
    if (std::find(m_atendimentos.begin(), m_atendimentos.end(), atendimento)
        != m_atendimentos.end()) {
        return;
    }
    m_atendimentos.push_back(atendimento);
}

// Atendimento atual = o mais recente que ainda não foi finalizado
Atendimento* Paciente::getAtendimentoAtual() const {
    for (auto it = m_atendimentos.rbegin(); it != m_atendimentos.rend(); ++it) {
        if ((*it)->getStatus() != StatusAtendimento::FINALIZADO) {
            return *it;
        }
    }
    return nullptr;
}

std::string statusParaTexto(StatusAtendimento status) {
    switch (status) {
        case StatusAtendimento::EM_ESPERA:      return "Em espera";
        case StatusAtendimento::EM_ATENDIMENTO: return "Em atendimento";
        case StatusAtendimento::FINALIZADO:     return "Finalizado";
    }
    return "Desconhecido";
}

// Mostra só os dados do próprio paciente (não expõe os outros da fila)
void Paciente::acompanharAtendimento() const {
    Atendimento* atual = getAtendimentoAtual();

    std::cout << "\n--- ACOMPANHAMENTO DO ATENDIMENTO ---\n";
    if (atual == nullptr) {
        std::cout << "  [Nenhum atendimento em andamento]\n";
        std::cout << "-------------------------------------\n";
        return;
    }

    std::cout << "  Atendimento #" << atual->getId() << " (" << atual->getData() << ")\n";
    std::cout << "  Status: " << statusParaTexto(atual->getStatus()) << "\n";

    FilaAtendimento* fila = atual->getFila();
    if (atual->getStatus() == StatusAtendimento::EM_ESPERA && fila != nullptr) {
        std::cout << "  Tempo estimado de espera: "
                  << fila->estimarTempoDeFila(atual) << " min\n";
    }
    std::cout << "-------------------------------------\n";
}

// Lança exceção se o paciente nunca teve atendimento (não existe status para mostrar)
StatusAtendimento Paciente::consultarStatusAtendimento() const {
    if (m_atendimentos.empty()) {
        throw std::logic_error("Paciente sem atendimentos registrados.");
    }
    Atendimento* atual = getAtendimentoAtual();
    if (atual != nullptr) {
        return atual->getStatus();
    }
    return m_atendimentos.back()->getStatus();   // todos finalizados
}

// Sem atendimento em andamento, a equipe é vazia
std::vector<ProfissionalSaude*> Paciente::visualizarEquipeResponsavel() const {
    Atendimento* atual = getAtendimentoAtual();
    if (atual == nullptr) {
        return {};
    }
    return atual->getEquipeResponsavel();
}

const std::vector<Atendimento*>& Paciente::getAtendimentos() const {
    return m_atendimentos;
}

// Atualiza o atendimento atual e reorganiza a fila em que ele está
void Paciente::atualizarPrioridade(Prioridade novaPrioridade) {
    m_prioridade = novaPrioridade;

    Atendimento* atual = getAtendimentoAtual();
    if (atual == nullptr) {
        return;
    }
    atual->setPrioridade(static_cast<int>(novaPrioridade));

    FilaAtendimento* fila = atual->getFila();
    if (fila != nullptr) {
        fila->reorganizarFila();
    }
}

Prioridade Paciente::getPrioridade() const {
    return m_prioridade;
}

void Paciente::adicionarReceita(const ReceitaMedica& receita) {
    m_receitas.push_back(receita);
}

const std::vector<ReceitaMedica>& Paciente::getReceitas() const {
    return m_receitas;
}


//ACOMPANHANTE
bool Paciente::temAcompanhante() const {
    return m_acompanhante != nullptr;
}

Acompanhante* Paciente::getAcompanhante() const {
    return m_acompanhante;
}

// Passar nullptr remove o acompanhante
void Paciente::setAcompanhante(Acompanhante* acompanhante) {
    m_acompanhante = acompanhante;
}
