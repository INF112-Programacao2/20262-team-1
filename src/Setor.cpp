
#include "Setor.h"
#include "Leito.h"
#include "RecursoHospitalar.h"
#include "Especialidade.h"

Setor::Setor(int id, std::string nome, std::string descricao){
    m_id=id;
    m_nome=nome;
    m_descricao=descricao;
    m_capacidadeMaxima=0;
}

Setor::~Setor(){
    for (Leito* l : m_leitos){
        delete l;
    }

    for (RecursoHospitalar* r : m_recursos){
        delete r;
    }

    for (Especialidade* e : m_especialidades){
        delete e;
    }

    m_leitos.clear();
    m_recursos.clear();
    m_especialidades.clear();
}

// LEITOS

void Setor::adicionarLeito(Leito* l){
    if (l != nullptr){
        for (Leito* existente : m_leitos){
            if (existente == l || existente->getId() == l->getId()){
                return;
            }
        }

        m_leitos.push_back(l);
        m_capacidadeMaxima++;
    }
}

void Setor::removerLeito(Leito* l){
    for (int i=0;i<static_cast<int>(m_leitos.size());i++){
        if (m_leitos[i] == l){
            delete m_leitos[i];
            m_leitos.erase(m_leitos.begin()+i);
            m_capacidadeMaxima--;
            return;
        }
    }
}

Leito* Setor::buscarLeito(int id){
    for (Leito* l : m_leitos){
        if (l != nullptr && l->getId() == id){
            return l;
        }
    }
    return nullptr;
}

// RECURSOS

void Setor::adicionarRecurso(RecursoHospitalar* r){
    if (r != nullptr){
        for (RecursoHospitalar* existente : m_recursos){
            if (existente == r || existente->getId() == r->getId()){
                return;
            }
        }

        m_recursos.push_back(r);
    }
}

void Setor::removerRecurso(RecursoHospitalar* r){
    for (int i=0;i<static_cast<int>(m_recursos.size());i++){
        if (m_recursos[i] == r){
            delete m_recursos[i];
            m_recursos.erase(m_recursos.begin()+i);
            return;
        }
    }
}

RecursoHospitalar* Setor::buscarRecurso(int id){
    for (RecursoHospitalar* r : m_recursos){
        if (r != nullptr && r->getId() == id){
            return r;
        }
    }

    return nullptr;
}

// ESPECIALIDADES

void Setor::adicionarEspecialidade(Especialidade* e){
    if (e != nullptr){
        for (Especialidade* existente : m_especialidades){
            if (existente == e || existente->getId() == e->getId()){
                return;
            }
        }

        m_especialidades.push_back(e);
    }
}

void Setor::removerEspecialidade(Especialidade* e){
    for (int i=0;i<static_cast<int>(m_especialidades.size());i++){
        if (m_especialidades[i] == e){
            delete m_especialidades[i];
            m_especialidades.erase(m_especialidades.begin()+i);
            return;
        }
    }
}

Especialidade* Setor::buscarEspecialidade(int id){
    for (Especialidade* e : m_especialidades){
        if (e != nullptr && e->getId() == id){
            return e;
        }
    }

    return nullptr;
}

// CONSULTAS

int Setor::calcularLotacao(){
    int lotacao = 0;

    for (Leito* l : m_leitos){
        if (l != nullptr && !l->verificarDisponibilidade()){
            lotacao++;
        }
    }

    return lotacao;
}

std::vector<Leito*> Setor::consultarLeitosDisponiveis(){
    std::vector<Leito*> disponiveis;

    for (Leito* l : m_leitos){
        if (l != nullptr && l->verificarDisponibilidade()){
            disponiveis.push_back(l);
        }
    }

    return disponiveis;
}

std::vector<RecursoHospitalar*> Setor::consultarRecursos(){
    return m_recursos;
}

// GETTERS

int Setor::getId(){
    return m_id;
}

std::string Setor::getNome(){
    return m_nome;
}

std::string Setor::getDescricao(){
    return m_descricao;
}

int Setor::getCapacidade(){
    return m_capacidadeMaxima;
}
