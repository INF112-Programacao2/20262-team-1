#include <iostream>
#include <iomanip>
#include "../include/Atendimento.hpp"
#include "../include/FilaAtendimento.hpp"

int main() {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "=================================================\n";
    std::cout << "      TESTE DA FILA DE ATENDIMENTO HOSPITALAR     \n";
    std::cout << "=================================================\n";

    FilaAtendimento fila;

    // 1. Criando atendimentos de teste
    // Construtor: Atendimento(prioridade, tempoEstimadoMin, idPaciente)
    Atendimento* a1 = new Atendimento(1, 15.0, "Paciente A (João)");  // Normal
    Atendimento* a2 = new Atendimento(1, 20.0, "Paciente B (Maria)"); // Normal
    Atendimento* a3 = new Atendimento(5, 10.0, "Paciente C (Carlos)");// Emergência

    // 2. Teste de Inserção Básica
    std::cout << "\n[1] Inserindo Paciente A (P1) e Paciente B (P1)...\n";
    fila.adicionarAtendimento(a1);
    fila.adicionarAtendimento(a2);
    fila.visualizarFila();

    std::cout << "Tempo estimado de espera para Maria (atras de Joao): " 
              << fila.estimarTempoDeFila(a2) << " min (Esperado: 15.0 min)\n";

    // 3. Teste de Inserção com Prioridade Maior (Chegada de Emergência)
    std::cout << "\n[2] Inserindo Paciente C (P5 - Emergencia)...\n";
    fila.adicionarAtendimento(a3); // Deve ir automaticamente para a frente
    fila.visualizarFila();

    std::cout << "Tempo estimado de espera para Carlos (Emergencia): " 
              << fila.estimarTempoDeFila(a3) << " min (Esperado: 0.0 min)\n";
    std::cout << "Tempo estimado de espera para Joao: " 
              << fila.estimarTempoDeFila(a1) << " min (Esperado: 10.0 min)\n";
    std::cout << "Tempo estimado de espera para Maria: " 
              << fila.estimarTempoDeFila(a2) << " min (Esperado: 25.0 min [10+15])\n";

    // 4. Teste de Remocao/Desistencia de Paciente
    std::cout << "\n[3] Testando remocao de paciente (desistencia/cancelamento)...\n";
    Atendimento* aTemp = new Atendimento(1, 30.0, "Paciente Temp (Desistente)");
    fila.adicionarAtendimento(aTemp);
    std::cout << "Tamanho apos inserir desistente: " << fila.getTamanho() << "\n";
    
    bool removidoSucesso = fila.removerAtendimento(aTemp);
    std::cout << "Remocao realizada com sucesso? " << (removidoSucesso ? "SIM" : "NAO") << "\n";
    std::cout << "Tamanho apos remover desistente: " << fila.getTamanho() << "\n";
    
    bool removerInexistente = fila.removerAtendimento(aTemp);
    std::cout << "Tentativa de remover novamente retornou false? " << (!removerInexistente ? "SIM" : "NAO") << "\n";
    delete aTemp;

    // 5. Teste Requisito de Entrega: Mudança de Prioridade na Fila
    std::cout << "\n[4] REQUISITO DE ENTREGA: Alterando prioridade no meio da fila!\n";
    std::cout << "Estado da Maria piorou! Mudando prioridade de 1 para 10 (UTI/Maxima)...\n";
    
    a2->setPrioridade(10);
    fila.reorganizarFila(); // Reorganiza a fila apos alteracao de prioridade
    fila.visualizarFila();

    std::cout << "Novo tempo de espera da Maria: " 
              << fila.estimarTempoDeFila(a2) << " min (Esperado: 0.0 min)\n";
    std::cout << "Novo tempo de espera do Carlos: " 
              << fila.estimarTempoDeFila(a3) << " min (Esperado: 20.0 min)\n";

    // 6. Teste de Chamada de Pacientes (Ordem de Atendimento)
    std::cout << "\n[5] Chamando os proximos pacientes da fila:\n";
    
    int ordem = 1;
    while (!fila.estaVazia()) {
        Atendimento* proximo = fila.chamarProximo();
        std::cout << "  " << ordem++ << "º a ser atendido: " << proximo->getIdPaciente() 
                  << " | Prioridade final: " << proximo->getPrioridade() << "\n";
    }

    fila.visualizarFila();

    // 7. Limpeza de memória
    delete a1;
    delete a2;
    delete a3;

    std::cout << "\n=================================================\n";
    std::cout << "           TODOS OS TESTES CONCLUIDOS!           \n";
    std::cout << "=================================================\n";

    return 0;
}