#include "../include/FilaAtendimento.hpp"
#include "../include/Atendimento.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

FilaAtendimento::FilaAtendimento() = default;
FilaAtendimento::~FilaAtendimento() = default;


bool FilaAtendimento::adicionarAtendimento(Atendimento* atendimento){
    if(atendimento == nullptr){
        return false;
    }

    auto elemento = std::find(m_fila.begin(), m_fila.end(), atendimento); // Pesquisa se o atendimento já está inserido

    if(elemento != m_fila.end()){
        return false;
    }

    m_fila.push_back(atendimento);
    reorganizarFila();

    return true;

}

bool FilaAtendimento::removerAtendimento(Atendimento* atendimento){
    if(atendimento == nullptr ){
        return false;
    }
    auto elemento = std::find(m_fila.begin(), m_fila.end(), atendimento); 

    if(elemento == m_fila.end()){ // Se o atendimento não for encontrado, false;
        return false;
    }

    m_fila.erase(elemento);
    return true;
}


Atendimento* FilaAtendimento::chamarProximo(){
    if(m_fila.empty()){
        return nullptr;
    }

    Atendimento* proximo = m_fila.front();
    m_fila.erase(m_fila.begin());

    return proximo;
}

int FilaAtendimento::getTamanho() const{
    return static_cast<int>(m_fila.size());
}

bool FilaAtendimento::estaVazia() const{
    return m_fila.empty();
}

void FilaAtendimento::reorganizarFila() {
    std::stable_sort(m_fila.begin(), m_fila.end(), [](const Atendimento* a, const Atendimento* b) {
        return a->getPrioridade() > b->getPrioridade(); // Vai comparando cada atendimento pela sua prioridade, de modo que a prioridade maior fique em cima
    });
}

double FilaAtendimento::estimarTempoDeFila(Atendimento* atendimento) const {
    if (atendimento == nullptr) {
        return 0.0;
    }

    double tempoEsperaAcumulado = 0.0;

    // Soma o tempo de atendimento de todas as pessoas que estão à frente na fila
    for (auto* item : m_fila) {
        if (item == atendimento) {
            break; // Chegou na posição do paciente solicitado
        }
        tempoEsperaAcumulado += item->getTempoEstimadoMin();
    }

    return tempoEsperaAcumulado;
}

void FilaAtendimento::visualizarFila() const {
    std::cout << "\n--- ESTADO ATUAL DA FILA (Tamanho: " << getTamanho() << ") ---\n";
    
    if (estaVazia()) {
        std::cout << "  [Fila vazia]\n";
        std::cout << "-----------------------------------------------\n";
        return;
    }

    int posicao = 1;
    for (const auto* atendimento : m_fila) {
        if (atendimento != nullptr) {
            std::cout << "  " << posicao++ << "º -> " << atendimento->getIdPaciente()
                      << " | Prioridade: " << atendimento->getPrioridade()
                      << " | Tempo Consulta: " << atendimento->getTempoEstimadoMin() << " min\n";
        }
    }
    std::cout << "-----------------------------------------------\n";
}