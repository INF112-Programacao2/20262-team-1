#include <filesystem>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <sqlite3.h>

#include "../include/Repositorio.h"

namespace {

using ChaveEstrangeira =
    std::tuple<std::string, std::string, std::string, std::string>;

class CapturarErros {
public:
    CapturarErros() : bufferOriginal_(std::cerr.rdbuf(conteudo_.rdbuf())) {}
    ~CapturarErros() { std::cerr.rdbuf(bufferOriginal_); }

    std::string conteudo() const { return conteudo_.str(); }

private:
    std::ostringstream conteudo_;
    std::streambuf* bufferOriginal_;
};

bool verificar(bool condicao, const std::string& descricao) {
    std::cout << (condicao ? "[PASSOU] " : "[FALHOU] ") << descricao << '\n';
    return condicao;
}

bool consultarEsquema(
    const std::string& caminho,
    std::set<std::string>& tabelas,
    std::set<ChaveEstrangeira>& chavesEstrangeiras) {
    sqlite3* banco = nullptr;
    int resultado = sqlite3_open_v2(
        caminho.c_str(), &banco, SQLITE_OPEN_READONLY, nullptr);
    if (resultado != SQLITE_OK) {
        std::cerr << "Falha ao abrir o banco para inspecionar o esquema: "
                  << (banco != nullptr ? sqlite3_errmsg(banco) : "sem detalhes")
                  << '\n';
        if (banco != nullptr) {
            sqlite3_close(banco);
        }
        return false;
    }

    sqlite3_stmt* consulta = nullptr;
    resultado = sqlite3_prepare_v2(
        banco,
        "SELECT name FROM sqlite_master "
        "WHERE type = 'table' AND name NOT LIKE 'sqlite_%';",
        -1,
        &consulta,
        nullptr);
    if (resultado != SQLITE_OK) {
        std::cerr << "Falha ao consultar as tabelas: "
                  << sqlite3_errmsg(banco) << '\n';
        sqlite3_close(banco);
        return false;
    }

    while ((resultado = sqlite3_step(consulta)) == SQLITE_ROW) {
        tabelas.insert(
            reinterpret_cast<const char*>(sqlite3_column_text(consulta, 0)));
    }
    sqlite3_finalize(consulta);
    if (resultado != SQLITE_DONE) {
        std::cerr << "Falha ao ler as tabelas: " << sqlite3_errmsg(banco) << '\n';
        sqlite3_close(banco);
        return false;
    }

    const std::vector<std::pair<std::string, std::string>> tabelasComChaves = {
        {"Acompanhante", "Acompanhante"},
        {"ProfissionalSaude", "ProfissionalSaude"},
        {"Medico", "Medico"},
        {"Enfermeiro", "Enfermeiro"},
        {"Setor", "Setor"},
        {"SetorEspecialidade", "SetorEspecialidade"},
        {"Leito", "Leito"},
        {"Atendimento", "Atendimento"},
        {"AtendimentoProfissional", "AtendimentoProfissional"},
        {"Procedimento", "Procedimento"},
        {"Equipamento", "Equipamento"},
        {"Material", "Material"},
        {"SetorRecurso", "SetorRecurso"},
        {"ReceitaMedica", "ReceitaMedica"},
        {"ItemReceita", "ItemReceita"},
        {"FilaAtendimento", "FilaAtendimento"},
        {"ItemFila", "ItemFila"}
    };

    for (const auto& entrada : tabelasComChaves) {
        const std::string sql =
            "PRAGMA foreign_key_list(\"" + entrada.first + "\");";
        resultado = sqlite3_prepare_v2(
            banco, sql.c_str(), -1, &consulta, nullptr);
        if (resultado != SQLITE_OK) {
            std::cerr << "Falha ao consultar as chaves estrangeiras de "
                      << entrada.first << ": " << sqlite3_errmsg(banco) << '\n';
            sqlite3_close(banco);
            return false;
        }

        while ((resultado = sqlite3_step(consulta)) == SQLITE_ROW) {
            const auto texto = [consulta](int coluna) {
                return std::string(reinterpret_cast<const char*>(
                    sqlite3_column_text(consulta, coluna)));
            };
            chavesEstrangeiras.emplace(
                entrada.first, texto(3), texto(2), texto(4));
        }
        sqlite3_finalize(consulta);
        if (resultado != SQLITE_DONE) {
            std::cerr << "Falha ao ler as chaves estrangeiras de "
                      << entrada.first << ": " << sqlite3_errmsg(banco) << '\n';
            sqlite3_close(banco);
            return false;
        }
    }

    sqlite3_close(banco);
    return true;
}

bool dadosDeTestePersistidos(const std::string& caminho) {
    sqlite3* banco = nullptr;
    int resultado = sqlite3_open_v2(
        caminho.c_str(), &banco, SQLITE_OPEN_READONLY, nullptr);
    if (resultado != SQLITE_OK) {
        std::cerr << "Falha ao reabrir o banco para verificar os dados: "
                  << (banco != nullptr ? sqlite3_errmsg(banco) : "sem detalhes")
                  << '\n';
        if (banco != nullptr) {
            sqlite3_close(banco);
        }
        return false;
    }

    const char* sql =
        "SELECT COUNT(*) FROM Paciente AS p "
        "JOIN Atendimento AS a ON a.paciente_id = p.id "
        "WHERE p.id = 900001 AND p.nome = 'Paciente de teste' "
        "AND a.id = 900001 AND a.status = 'Teste de persistencia';";
    sqlite3_stmt* consulta = nullptr;
    resultado = sqlite3_prepare_v2(banco, sql, -1, &consulta, nullptr);
    if (resultado != SQLITE_OK) {
        std::cerr << "Falha ao consultar os dados salvos: "
                  << sqlite3_errmsg(banco) << '\n';
        sqlite3_close(banco);
        return false;
    }

    resultado = sqlite3_step(consulta);
    const bool persistidos =
        resultado == SQLITE_ROW && sqlite3_column_int(consulta, 0) == 1;
    if (resultado != SQLITE_ROW) {
        std::cerr << "Falha ao ler os dados salvos: "
                  << sqlite3_errmsg(banco) << '\n';
    }
    sqlite3_finalize(consulta);
    sqlite3_close(banco);
    return persistidos;
}

const std::set<std::string> TABELAS_ESPERADAS = {
    "Paciente", "Acompanhante", "Atendente", "Administrador",
    "ProfissionalSaude", "Especialidade", "Medico", "Enfermeiro",
    "Hospital", "Setor", "SetorEspecialidade", "Leito", "Atendimento",
    "AtendimentoProfissional", "Procedimento", "RecursoHospitalar",
    "Equipamento", "Material", "SetorRecurso", "Medicamento",
    "ReceitaMedica", "ItemReceita", "FilaAtendimento", "ItemFila"
};

const std::set<ChaveEstrangeira> CHAVES_ESPERADAS = {
    {"Acompanhante", "paciente_id", "Paciente", "id"},
    {"ProfissionalSaude", "especialidade_id", "Especialidade", "id"},
    {"Medico", "id", "ProfissionalSaude", "id"},
    {"Enfermeiro", "id", "ProfissionalSaude", "id"},
    {"Setor", "hospital_id", "Hospital", "id"},
    {"SetorEspecialidade", "setor_id", "Setor", "id"},
    {"SetorEspecialidade", "especialidade_id", "Especialidade", "id"},
    {"Leito", "paciente_id", "Paciente", "id"},
    {"Leito", "setor_id", "Setor", "id"},
    {"Atendimento", "paciente_id", "Paciente", "id"},
    {"AtendimentoProfissional", "atendimento_id", "Atendimento", "id"},
    {"AtendimentoProfissional", "profissional_id", "ProfissionalSaude", "id"},
    {"Procedimento", "atendimento_id", "Atendimento", "id"},
    {"Equipamento", "id", "RecursoHospitalar", "id"},
    {"Material", "id", "RecursoHospitalar", "id"},
    {"SetorRecurso", "setor_id", "Setor", "id"},
    {"SetorRecurso", "recurso_id", "RecursoHospitalar", "id"},
    {"ReceitaMedica", "paciente_id", "Paciente", "id"},
    {"ReceitaMedica", "medico_id", "Medico", "id"},
    {"ItemReceita", "receita_id", "ReceitaMedica", "id"},
    {"ItemReceita", "medicamento_id", "Medicamento", "id"},
    {"FilaAtendimento", "setor_id", "Setor", "id"},
    {"ItemFila", "fila_id", "FilaAtendimento", "id"},
    {"ItemFila", "atendimento_id", "Atendimento", "id"}
};

}  // namespace

int main() {
    bool passou = true;
    const std::string caminhoBanco =
        (std::filesystem::path("tests") / "repositorio_teste.db").string();

    std::cout << "Iniciando testes exclusivos do Repositorio...\n";

    std::string errosConstrucao;
    bool dadosInseridos = false;
    {
        CapturarErros captura;
        Repositorio repositorio(caminhoBanco);
        passou &= verificar(
            repositorio.executarSQL("SELECT 1;"),
            "conexao com o SQLite executa uma consulta");
        const bool pacienteInserido = repositorio.executarSQL(
            "INSERT INTO Paciente (id, nome, cpf, idade, prioridade) "
            "VALUES (900001, 'Paciente de teste', "
            "'TESTE-REPOSITORIO-900001', 35, 1) "
            "ON CONFLICT(id) DO UPDATE SET "
            "nome = excluded.nome, cpf = excluded.cpf, "
            "idade = excluded.idade, prioridade = excluded.prioridade;");
        const bool atendimentoInserido = pacienteInserido &&
            repositorio.executarSQL(
                "INSERT INTO Atendimento "
                "(id, data, status, prioridade, tempoEstimadoMin, paciente_id) "
                "VALUES (900001, '2026-11-24', 'Teste de persistencia', "
                "1, 15.0, 900001) "
                "ON CONFLICT(id) DO UPDATE SET "
                "data = excluded.data, status = excluded.status, "
                "prioridade = excluded.prioridade, "
                "tempoEstimadoMin = excluded.tempoEstimadoMin, "
                "paciente_id = excluded.paciente_id;");
        dadosInseridos = pacienteInserido && atendimentoInserido;
        errosConstrucao = captura.conteudo();
    }
    passou &= verificar(
        errosConstrucao.empty(),
        "construtor conecta e cria o esquema sem erros");
    passou &= verificar(
        dadosInseridos,
        "dados ficticios de paciente e atendimento foram inseridos");

    std::set<std::string> tabelas;
    std::set<ChaveEstrangeira> chavesEstrangeiras;
    const bool esquemaConsultado =
        consultarEsquema(caminhoBanco, tabelas, chavesEstrangeiras);
    passou &= verificar(esquemaConsultado, "esquema do banco pode ser inspecionado");
    if (esquemaConsultado) {
        passou &= verificar(
            tabelas == TABELAS_ESPERADAS,
            "todas as 24 tabelas esperadas existem");
        passou &= verificar(
            chavesEstrangeiras == CHAVES_ESPERADAS,
            "todas as 24 chaves estrangeiras estao corretamente definidas");
    }

    std::string errosRecriacao;
    bool conexaoReaberta = false;
    bool chaveEstrangeiraHabilitada = false;
    {
        CapturarErros captura;
        Repositorio repositorio(caminhoBanco);
        conexaoReaberta = repositorio.executarSQL("SELECT 1;");
        errosRecriacao = captura.conteudo();
        {
            CapturarErros captura;
            chaveEstrangeiraHabilitada = !repositorio.executarSQL(
                "INSERT INTO Acompanhante (id, nome, cpf, paciente_id) "
                "VALUES (1, 'Teste', 'cpf-temporario', 999999);");
        }
    }
    passou &= verificar(
        errosRecriacao.empty(),
        "criarTabelas pode ser executado novamente sem erros (IF NOT EXISTS)");
    passou &= verificar(conexaoReaberta, "segunda conexao com o mesmo banco funciona");
    passou &= verificar(
        chaveEstrangeiraHabilitada,
        "a conexao do Repositorio habilita a validacao de chaves estrangeiras");
    passou &= verificar(
        dadosDeTestePersistidos(caminhoBanco),
        "dados continuam salvos apos fechar e reabrir o banco");

    std::cout << (passou
                      ? "Todos os testes do Repositorio passaram.\n"
                      : "Um ou mais testes do Repositorio falharam.\n");
    std::cout << "Banco de teste mantido em: " << caminhoBanco << '\n';
    return passou ? 0 : 1;
}
