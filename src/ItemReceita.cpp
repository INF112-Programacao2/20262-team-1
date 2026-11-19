#include "ItemReceita.h"

constexpr int QUANTIDADE_MINIMA = 1;

bool quantidadeValida(int quantidade) {
    return quantidade >= QUANTIDADE_MINIMA;
}

// Quantidade inválida vira QUANTIDADE_MINIMA, para o item nunca nascer inválido
ItemReceita::ItemReceita(Medicamento* medicamento, const std::string& dosagem,
                         int quantidade, const std::string& descricaoUso)
    : m_medicamento(medicamento),
      m_dosagem(dosagem),
      m_quantidade(quantidadeValida(quantidade) ? quantidade : QUANTIDADE_MINIMA),
      m_descricaoUso(descricaoUso) {}


//GETTERS
Medicamento* ItemReceita::getMedicamento() const {
    return m_medicamento;
}

std::string ItemReceita::getDosagem() const {
    return m_dosagem;
}

int ItemReceita::getQuantidade() const {
    return m_quantidade;
}

std::string ItemReceita::getDescricaoUso() const {
    return m_descricaoUso;
}


//SETTERS
void ItemReceita::setDosagem(const std::string& dosagem) {
    m_dosagem = dosagem;
}

// Ignora quantidades menores que QUANTIDADE_MINIMA
void ItemReceita::setQuantidade(int quantidade) {
    if (quantidadeValida(quantidade)) {
        m_quantidade = quantidade;
    }
}

void ItemReceita::setDescricaoUso(const std::string& descricaoUso) {
    m_descricaoUso = descricaoUso;
}
