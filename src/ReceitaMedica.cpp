#include "ReceitaMedica.h"

#include <sstream>

#include "Paciente.h"
#include "ProfissionalSaude.h"

// Começa sem itens, sem observações e ainda não emitida
ReceitaMedica::ReceitaMedica(int id, const std::string& data,
                             Paciente* paciente, ProfissionalSaude* medico)
    : m_id(id),
      m_data(data),
      m_paciente(paciente),
      m_medico(medico),
      m_emitida(false) {}

// Não aceita medicamento nulo nem quantidade menor que 1
bool ReceitaMedica::adicionarMedicamento(Medicamento* medicamento, const std::string& dosagem,
                                         int quantidade, const std::string& descricaoUso) {
    if (m_emitida || medicamento == nullptr || quantidade < 1) {
        return false;
    }
    m_itens.emplace_back(medicamento, dosagem, quantidade, descricaoUso);
    return true;
}

// O índice segue a ordem em que os itens foram adicionados (começa em 0)
bool ReceitaMedica::removerMedicamento(int indice) {
    if (m_emitida || indice < 0 || indice >= static_cast<int>(m_itens.size())) {
        return false;
    }
    m_itens.erase(m_itens.begin() + indice);
    return true;
}

// Uma receita sem itens não é emitida; depois de emitida, não muda mais
void ReceitaMedica::emitir() {
    if (m_emitida || m_itens.empty()) {
        return;
    }
    m_emitida = true;
}

std::string ReceitaMedica::gerarTexto() const {
    std::ostringstream texto;

    texto << "========== RECEITA MEDICA #" << m_id << " ==========\n";
    texto << "Data: " << m_data << "\n";
    texto << "Paciente: " << (m_paciente != nullptr ? m_paciente->getNome() : "-") << "\n";
    texto << "Medico: " << (m_medico != nullptr ? m_medico->getNome() : "-") << "\n";
    texto << "-----------------------------------------\n";

    if (m_itens.empty()) {
        texto << "  [Nenhum medicamento prescrito]\n";
    }

    int numero = 1;
    for (const ItemReceita& item : m_itens) {
        const Medicamento* medicamento = item.getMedicamento();
        texto << "  " << numero++ << ". "
              << (medicamento != nullptr ? medicamento->getNome() : "-")
              << " " << item.getDosagem()
              << " | Qtd: " << item.getQuantidade() << "\n"
              << "     Uso: " << item.getDescricaoUso() << "\n";
    }

    if (!m_observacoes.empty()) {
        texto << "-----------------------------------------\n";
        texto << "Observacoes: " << m_observacoes << "\n";
    }

    texto << "Situacao: " << (m_emitida ? "EMITIDA" : "RASCUNHO") << "\n";
    texto << "=========================================\n";
    return texto.str();
}


//GETTERS
int ReceitaMedica::getId() const {
    return m_id;
}

std::string ReceitaMedica::getData() const {
    return m_data;
}

std::string ReceitaMedica::getObservacoes() const {
    return m_observacoes;
}

Paciente* ReceitaMedica::getPaciente() const {
    return m_paciente;
}

ProfissionalSaude* ReceitaMedica::getMedico() const {
    return m_medico;
}

const std::vector<ItemReceita>& ReceitaMedica::getItens() const {
    return m_itens;
}

//IS
bool ReceitaMedica::foiEmitida() const {
    return m_emitida;
}


//SETTERS
// Depois de emitida, as observações também ficam travadas
void ReceitaMedica::setObservacoes(const std::string& observacoes) {
    if (m_emitida) {
        return;
    }
    m_observacoes = observacoes;
}
