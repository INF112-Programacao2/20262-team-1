#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdio>

#include "../include/FilaAtendimento.h"
#include "../include/ProcessadorLote.h"
#include "../include/ServicoNotificacao.h"
#include "../include/Paciente.h"
#include "../include/Atendimento.h"
#include "../include/Setor.h"
#include "../include/Leito.h"

void testarImportacaoValidaEFila() {
    std::cout << "\n[1/3] Testando importacao CSV, trava de prioridade e ordenacao na fila...\n";

    // Cria um CSV temporario com comentario, linha em branco e uma prioridade acima de 5
    const std::string arquivoCsv = "temp_lote_valido.csv";
    std::ofstream out(arquivoCsv);
    out << "idPaciente;nome;cpf;idade;prioridade;tempoEstimadoMin;idAtendimento;data\n";
    out << "# Lote de pacientes da triagem\n\n";
    out << "10;Ana Souza;111.111.111-11;30;2;15.0;501;2026-11-21\n";
    out << "20;Carlos Lima;222.222.222-22;65;9;40.0;502;2026-11-21\n"; // prioridade 9 deve ser travada em 5
    out << "30;Beatriz Reis;333.333.333-33;45;4;25.0;503;2026-11-21\n";
    out << "40;Lucas Mendes;444.444.444-44;22;1;10.0;504;2026-11-21\n";
    out.close();

    FilaAtendimento fila;
    ProcessadorLote processador(arquivoCsv);

    bool leu = processador.importarDados();
    assert(leu == true);
    assert(processador.getQuantidadeImportada() == 4);

    int inseridos = processador.processarPacientes(&fila);
    assert(inseridos == 4);
    assert(fila.getTamanho() == 4);

    // Mostra no terminal como a fila ficou organizada automaticamente
    fila.visualizarFila();

    // Confere o tempo estimado de espera para o ultimo da fila (Lucas, prioridade 1)
    Atendimento* ultimo = fila.getFila().back();
    double esperaLucas = fila.estimarTempoDeFila(ultimo);
    std::cout << "  -> Tempo estimado de espera para o ultimo da fila (" << ultimo->getIdPaciente()
              << "): " << esperaLucas << " min\n";
    assert(esperaLucas == 80.0); // 40 (Carlos) + 25 (Beatriz) + 15 (Ana)

    // Vai chamando os pacientes na ordem da fila para conferir
    std::cout << "  -> Ordem de chamada:\n";
    Atendimento* primeiro = fila.chamarProximo();
    assert(primeiro != nullptr && primeiro->getPrioridade() == 5 && primeiro->getIdPaciente() == "P20");
    std::cout << "     1o chamado: " << primeiro->getIdPaciente() << " (Prioridade " << primeiro->getPrioridade() << ")\n";

    Atendimento* segundo = fila.chamarProximo();
    assert(segundo != nullptr && segundo->getPrioridade() == 4 && segundo->getIdPaciente() == "P30");
    std::cout << "     2o chamado: " << segundo->getIdPaciente() << " (Prioridade " << segundo->getPrioridade() << ")\n";

    std::remove(arquivoCsv.c_str());
    std::cout << "  [OK] Lote importado e fila ordenada corretamente.\n";
}

void testarExcecoesProcessadorLote() {
    std::cout << "\n[2/3] Testando disparo de excecoes no ProcessadorLote...\n";

    // Caso 1: Arquivo que nao existe
    ProcessadorLote procInexistente("arquivo_fantasma.csv");
    bool lancouErro = false;
    try {
        procInexistente.importarDados();
    } catch (const std::exception& e) {
        lancouErro = true;
        std::cout << "  -> Excecao 1 capturada (arquivo ausente): " << e.what() << "\n";
    }
    assert(lancouErro == true);

    // Caso 2: Linha com colunas faltando
    const std::string csvColunas = "temp_lote_colunas.csv";
    std::ofstream outCol(csvColunas);
    outCol << "10;Ana Souza;111.111.111-11;30\n";
    outCol.close();

    ProcessadorLote procColunas(csvColunas);
    lancouErro = false;
    try {
        procColunas.importarDados();
    } catch (const std::exception& e) {
        lancouErro = true;
        std::cout << "  -> Excecao 2 capturada (colunas faltando): " << e.what() << "\n";
    }
    assert(lancouErro == true);
    std::remove(csvColunas.c_str());

    // Caso 3: Texto invalido no meio do arquivo (depois de uma linha valida)
    const std::string csvQuebrado = "temp_lote_erro.csv";
    std::ofstream outErr(csvQuebrado);
    outErr << "10;Ana Souza;111.111.111-11;30;2;15.0;501;2026-11-21\n";
    outErr << "20;Pedro;222.222.222-22;IDADE_ERRADA;5;40.0;502;2026-11-21\n";
    outErr.close();

    ProcessadorLote procQuebrado(csvQuebrado);
    lancouErro = false;
    try {
        procQuebrado.importarDados();
    } catch (const std::exception& e) {
        lancouErro = true;
        std::cout << "  -> Excecao 3 capturada (erro de conversao + rollback): " << e.what() << "\n";
    }
    assert(lancouErro == true);
    assert(procQuebrado.getQuantidadeImportada() == 0); // Garante que limpou a linha 1 alocada antes do erro

    std::remove(csvQuebrado.c_str());
    std::cout << "  [OK] Todas as excecoes foram tratadas sem vazamento de memoria.\n";
}

void testarServicoNotificacao() {
    std::cout << "\n[3/3] Testando geracao e exibicao de notificacoes...\n";

    ServicoNotificacao notificador;
    Paciente p(1, "Joao Silva", "999.999.999-99", 50);
    Atendimento a(5, 30.0, "P1", 100, "2026-11-21", &p);

    Setor uti(1, "UTI Adulto", "Unidade de Terapia Intensiva");
    Leito* l1 = new Leito(101);
    l1->alocarPaciente(&p);
    uti.adicionarLeito(l1);

    // Dispara 3 tipos diferentes de notificacao
    notificador.chamarPaciente(&p);
    notificador.notificarMudancaSetor(&a);
    notificador.alertarSobrecarga(&uti);

    const auto& historico = notificador.getHistoricoNotificacoes();
    assert(historico.size() == 3u);

    std::cout << "  -> Historico de notificacoes geradas (" << historico.size() << "):\n";
    for (size_t i = 0; i < historico.size(); ++i) {
        std::cout << "     [" << (i + 1) << "] " << historico[i] << "\n";
    }

    notificador.limparHistorico();
    assert(notificador.getHistoricoNotificacoes().empty());

    std::cout << "  [OK] Notificacoes exibidas e historico limpo com sucesso.\n";
}

int main() {
    std::cout << "=========================================================\n";
    std::cout << "   SUITE DE TESTES: PROCESSADOR LOTE, FILA E ALERTAS     \n";
    std::cout << "=========================================================\n";

    testarImportacaoValidaEFila();
    testarExcecoesProcessadorLote();
    testarServicoNotificacao();

    std::cout << "\n=========================================================\n";
    std::cout << "   TODOS OS TESTES PASSARAM COM SUCESSO!                 \n";
    std::cout << "=========================================================\n";
    return 0;
}