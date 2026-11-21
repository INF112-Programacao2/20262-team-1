#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstdio>

#include "../include/Setor.h"
#include "../include/Leito.h"
#include "../include/Paciente.h"
#include "../include/Atendimento.h"
#include "../include/FilaAtendimento.h"
#include "../include/GerenciadorFluxo.h"
#include "../include/ServicoNotificacao.h"
#include "../include/RegistroAuditoria.h"
#include "../include/Relatorio.h"
#include "../include/SimuladorHospitalar.h"

int main() {
    std::cout << "=================================================================\n";
    std::cout << "   SUITE DE TESTES: GERENCIADOR DE FLUXO, RELATORIO E SIMULADOR  \n";
    std::cout << "=================================================================\n";

    // 1. Monta a estrutura basica do hospital com 2 setores e 4 leitos cada
    Setor uti(1, "UTI Geral", "Unidade de Terapia Intensiva");
    Setor enfermaria(2, "Enfermaria", "Ala de internacao geral");

    for (int i = 1; i <= 4; ++i) {
        uti.adicionarLeito(new Leito(100 + i));
        enfermaria.adicionarLeito(new Leito(200 + i));
    }
    std::vector<Setor*> setores = { &uti, &enfermaria };

    FilaAtendimento fila;
    // Configura alerta: >= 75% de lotacao no setor, >= 5 pessoas na fila ou >= 60 min de espera
    GerenciadorFluxo gerenciador(75, 5, 60.0);
    ServicoNotificacao notificador;
    RegistroAuditoria auditoria;

    // -------------------------------------------------------------------------
    // ETAPA 1: Gerenciador de Fluxo (Mudanca de setor e balanceamento de carga)
    // -------------------------------------------------------------------------
    std::cout << "\n[1/4] Testando regras do GerenciadorFluxo...\n";

    Paciente pLeve(1, "Lucas Oliveira", "111.222.333-44", 25);
    Atendimento aLeve(2, 15.0, "P1", 801, "2026-11-21", &pLeve);

    Paciente pGrave(2, "Mariana Costa", "444.555.666-77", 70);
    Atendimento aGrave(5, 45.0, "P2", 802, "2026-11-21", &pGrave);

    assert(gerenciador.verificarMudancaSetor(&aLeve) == false);
    assert(gerenciador.verificarMudancaSetor(&aGrave) == true);
    std::cout << "  -> Paciente leve (prioridade 2) exige mudanca de setor? Nao\n";
    std::cout << "  -> Paciente grave (prioridade 5) exige mudanca de setor? Sim\n";

    // Ocupa 1 leito da UTI (25%) enquanto a Enfermaria esta em 0% para testar balanceamento
    Leito* leitoUti1 = uti.consultarLeitosDisponiveis().front();
    leitoUti1->alocarPaciente(&pLeve);

    Setor* setorIndicado = gerenciador.recomendarProximoSetor(&aGrave, setores);
    assert(setorIndicado == &enfermaria);
    std::cout << "  -> UTI com 25% e Enfermaria com 0% -> Setor recomendado: "
              << setorIndicado->getNome() << "\n";

    // Libera o leito antes de comecar a simulacao limpa
    leitoUti1->liberar();
    std::cout << "  [OK] Balanceamento e triagem de gravidade funcionando.\n";

    // -------------------------------------------------------------------------
    // ETAPA 2: Simulador Hospitalar (Modo verbose para mostrar logs ao vivo)
    // -------------------------------------------------------------------------
    std::cout << "\n[2/4] Rodando simulacao de estresse e comparacao de cenarios...\n";
    SimuladorHospitalar simulador(&fila, &gerenciador, &notificador, &auditoria, false);

    // Fotografa o hospital antes do caos
    simulador.setCenarioAtivo("Operacao_Normal");
    simulador.registrarSnapshotCenario("Operacao_Normal", setores);

    // Cria um CSV temporario com 6 pacientes chegando juntos (estoura o limite de 5 da fila)
    const std::string csvSurto = "temp_surto_simulador.csv";
    std::ofstream outCsv(csvSurto);
    outCsv << "idPaciente;nome;cpf;idade;prioridade;tempoEstimadoMin;idAtendimento;data\n";
    outCsv << "101;Paciente Surto 1;101.000.000-01;60;5;30.0;901;2026-11-21\n";
    outCsv << "102;Paciente Surto 2;102.000.000-02;45;4;25.0;902;2026-11-21\n";
    outCsv << "103;Paciente Surto 3;103.000.000-03;33;3;20.0;903;2026-11-21\n";
    outCsv << "104;Paciente Surto 4;104.000.000-04;50;4;25.0;904;2026-11-21\n";
    outCsv << "105;Paciente Surto 5;105.000.000-05;28;2;15.0;905;2026-11-21\n";
    outCsv << "106;Paciente Surto 6;106.000.000-06;72;5;35.0;906;2026-11-21\n";
    outCsv.close();

    // Inicia o cenario de surto
    simulador.setCenarioAtivo("Surto_e_Interdicao");
    bool carregouLote = simulador.carregarCenarioEmLote(csvSurto);
    assert(carregouLote == true);
    assert(fila.getTamanho() == 6);
    std::remove(csvSurto.c_str());

    // Mostra a fila cheia depois da injecao do lote
    fila.visualizarFila();

    // Forca 3 ocupacoes na UTI (3/4 = 75% -> atinge limite critico e dispara alerta)
    simulador.simularSobrecarga(&uti, 3);
    assert(gerenciador.monitorarSobrecargas(&uti) == true);

    // Interdita a Enfermaria inteira
    simulador.simularDisponibilidade(&enfermaria, false);
    assert(gerenciador.recomendarProximoSetor(&aGrave, setores) == nullptr);
    std::cout << "  -> Com UTI sobrecarregada e Enfermaria interditada, nenhum setor disponivel!\n";

    // Fotografa o pior momento da crise
    simulador.registrarSnapshotCenario("Cenario_Critico", setores);

    // Imprime o comparativo entre Operacao_Normal e Cenario_Critico
    std::string comparativo = simulador.compararCenarios();
    assert(comparativo.find("Operacao_Normal") != std::string::npos);
    assert(comparativo.find("Cenario_Critico") != std::string::npos);
    std::cout << "\n" << comparativo;

    // Testa reabertura da Enfermaria durante o cenario
    simulador.simularDisponibilidade(&enfermaria, true);
    assert(gerenciador.recomendarProximoSetor(&aGrave, setores) == &enfermaria);
    std::cout << "  -> Apos reabrir Enfermaria, sistema voltou a recomendar: "
              << gerenciador.recomendarProximoSetor(&aGrave, setores)->getNome() << "\n";

    // Exibe todas as notificacoes disparadas durante a simulacao
    const auto& alertas = notificador.getHistoricoNotificacoes();
    assert(!alertas.empty());
    std::cout << "\n  -> Alertas emitidos pelo ServicoNotificacao (" << alertas.size() << "):\n";
    for (size_t i = 0; i < alertas.size(); ++i) {
        std::cout << "     [" << (i + 1) << "] " << alertas[i] << "\n";
    }
    std::cout << "  [OK] Simulacao de cenarios e alertas validados.\n";

    // -------------------------------------------------------------------------
    // ETAPA 3: Geracao e exibicao do Relatorio gerencial
    // -------------------------------------------------------------------------
    std::cout << "\n[3/4] Gerando e exportando Relatorio de ocupacao...\n";
    Relatorio relatorio;
    std::string textoOcupacao = relatorio.gerarRelatorioOcupacao(setores, &gerenciador);
    assert(textoOcupacao.find("ALERTA DE SOBRECARGA") != std::string::npos);

    std::cout << "\n" << textoOcupacao << "\n";

    const std::string arqRelatorio = "temp_relatorio_teste.txt";
    assert(relatorio.exportar(arqRelatorio) == true);
    std::remove(arqRelatorio.c_str());
    std::cout << "  [OK] Relatorio gerado e exportado em disco sem erros.\n";

    // -------------------------------------------------------------------------
    // ETAPA 4: Reset da simulacao e limpeza de memoria
    // -------------------------------------------------------------------------
    std::cout << "\n[4/4] Resetando simulacao para restaurar o estado inicial...\n";
    std::cout << "  -> Antes do reset: UTI=" << uti.calcularLotacao() << "/" << uti.getCapacidade()
              << " | Fila=" << fila.getTamanho() << " pacientes\n";

    simulador.resetarSimulacao();

    assert(uti.calcularLotacao() == 0);
    assert(enfermaria.consultarLeitosDisponiveis().size() == 4);
    assert(fila.estaVazia() == true);

    std::cout << "  -> Depois do reset: UTI=" << uti.calcularLotacao() << "/" << uti.getCapacidade()
              << " | Fila=" << fila.getTamanho() << " pacientes\n";
    std::cout << "  [OK] Hospital limpo e pronto para operacao real.\n";

    std::cout << "\n=================================================================\n";
    std::cout << "   TODOS OS TESTES DE FLUXO, RELATORIO E SIMULADOR PASSARAM!     \n";
    std::cout << "=================================================================\n";
    return 0;
}