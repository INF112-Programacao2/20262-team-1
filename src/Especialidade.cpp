#include "../include/Especialidade.h"
#include <string>

Especialidade::Especialidade(int id, std::string nome, std::string descricao){
    m_descricao=descricao;
    m_id=id;
    m_nome=nome;
}
std::string Especialidade::getDescricao(){
    return m_descricao;
}
std::string Especialidade::getNome(){
    return m_nome;
}
int Especialidade::getId(){
    return m_id;
}
Especialidade::~Especialidade()=default;
