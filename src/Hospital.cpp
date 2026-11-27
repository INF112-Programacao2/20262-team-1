#include "Hospital.h"
#include "Setor.h"

Hospital::Hospital(std::string nome, std::string endereco){
    m_nome=nome;
    m_endereco=endereco;
}

Hospital::~Hospital() {
    for (Setor* s : m_setores) {
        delete s;
    }
    m_setores.clear();
}

void Hospital::adicionarSetor(Setor* s) {
    if (s == nullptr) {
        return;
    }

    for (Setor* existente : m_setores) {
        if (existente == s || existente->getId() == s->getId()) {
            return;
        }
    }

    m_setores.push_back(s);
}

void Hospital::removerSetor(Setor* s) {
    if (s != nullptr) {
        for (int i=0; i<static_cast<int>(m_setores.size()); i++) {
            if (m_setores[i] == s) {
                delete m_setores[i];
                m_setores.erase(m_setores.begin()+i);
                return;
            }
        }
    }
}

void Hospital::removerSetor(int sID) {
    for (int i=0; i<static_cast<int>(m_setores.size()); i++) {
        if (m_setores[i]->getId() == sID) {
            delete m_setores[i];
            m_setores.erase(m_setores.begin()+i);
            return;
        }
    }
}

Setor* Hospital::buscarSetor(int id) {
    for (int i=0; i<static_cast<int>(m_setores.size()); i++) {
        if (m_setores[i]->getId() == id) {
            return m_setores[i];
        }
    }
    return nullptr;
}

const std::vector<Setor*>& Hospital::getSetores() const {
    return m_setores;
}

std::string Hospital::getNome() {
    return m_nome;
}

std::string Hospital::getEndereco() {
    return m_endereco;
}
