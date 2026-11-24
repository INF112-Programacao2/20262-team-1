
#include "../include/Repositorio.h"
#include <iostream>

Repositorio::Repositorio(const std::string& caminho) {
    m_conexao= nullptr;
    m_caminhoArquivoBD = caminho;

    if (conectar()) {
        if (!criarTabelas()) {
            std::cerr << "Erro ao criar as tabelas do banco.\n";
        }
    }
}

Repositorio::~Repositorio() {
    desconectar();
}

bool Repositorio::conectar() {
    int resultado = sqlite3_open(
        m_caminhoArquivoBD.c_str(),
        &m_conexao
    );

    if (resultado != SQLITE_OK) {
        std::cerr << "Erro ao conectar ao banco\n";
        return false;
    }
    if (!executarSQL("PRAGMA foreign_keys = ON;")) {
        return false;
    }
    

    return true;
}

void Repositorio::desconectar() {
    if (m_conexao!= nullptr) {
        sqlite3_close(m_conexao);
        m_conexao= nullptr;
    }
}

bool Repositorio::executarSQL(const std::string& sql) {
    char* erro = nullptr;

    int resultado = sqlite3_exec(
        m_conexao,
        sql.c_str(),
        nullptr,
        nullptr,
        &erro
    );

    if (resultado != SQLITE_OK) {
        std::cerr << "Erro SQL: " << erro << "\n";
        sqlite3_free(erro);
        return false;
    }

    return true;
}
bool Repositorio::criarTabelas() {
    const char* comandos[] = {

        // PACIENTE: herda Usuario
        R"(
            CREATE TABLE IF NOT EXISTS Paciente (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                cpf TEXT UNIQUE NOT NULL,
                dataNascimento TEXT,
                idade INTEGER,
                telefone TEXT,
                email TEXT,
                login TEXT,
                senhaHash TEXT,
                ativo INTEGER NOT NULL DEFAULT 1,
                prioridade INTEGER NOT NULL
            );
        )",

        // ACOMPANHANTE: herda Usuario
        R"(
            CREATE TABLE IF NOT EXISTS Acompanhante (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                cpf TEXT UNIQUE NOT NULL,
                dataNascimento TEXT,
                telefone TEXT,
                email TEXT,
                login TEXT,
                senhaHash TEXT,
                ativo INTEGER NOT NULL DEFAULT 1,
                parentesco TEXT,
                paciente_id INTEGER,
                autorizado INTEGER NOT NULL DEFAULT 0,
                FOREIGN KEY (paciente_id) REFERENCES Paciente(id)
            );
        )",

        // ATENDENTE: herda Funcionario e Usuario
        R"(
            CREATE TABLE IF NOT EXISTS Atendente (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                cpf TEXT UNIQUE NOT NULL,
                dataNascimento TEXT,
                telefone TEXT,
                email TEXT,
                login TEXT,
                senhaHash TEXT,
                ativo INTEGER NOT NULL DEFAULT 1,
                escala TEXT
            );
        )",

        // ADMINISTRADOR: herda Funcionario e Usuario
        R"(
            CREATE TABLE IF NOT EXISTS Administrador (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                cpf TEXT UNIQUE NOT NULL,
                dataNascimento TEXT,
                telefone TEXT,
                email TEXT,
                login TEXT,
                senhaHash TEXT,
                ativo INTEGER NOT NULL DEFAULT 1,
                escala TEXT
            );
        )",

        // PROFISSIONALSAUDE
        R"(
            CREATE TABLE IF NOT EXISTS ProfissionalSaude (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                cpf TEXT UNIQUE NOT NULL,
                dataNascimento TEXT,
                telefone TEXT,
                email TEXT,
                login TEXT,
                senhaHash TEXT,
                ativo INTEGER NOT NULL DEFAULT 1,
                escala TEXT,
                registroProfissional TEXT,
                especialidade_id INTEGER,
                FOREIGN KEY (especialidade_id) REFERENCES Especialidade(id)
            );
        )",

        // ESPECIALIDADE
        R"(
            CREATE TABLE IF NOT EXISTS Especialidade (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                descricao TEXT
            );
        )",

        // MEDICO: herda ProfissionalSaude, Funcionario e Usuario
        R"(
            CREATE TABLE IF NOT EXISTS Medico (
                id INTEGER PRIMARY KEY,
                FOREIGN KEY (id) REFERENCES ProfissionalSaude(id)
            );
        )",

        // ENFERMEIRO: herda ProfissionalSaude, Funcionario e Usuario
        R"(
            CREATE TABLE IF NOT EXISTS Enfermeiro (
                id INTEGER PRIMARY KEY,
                FOREIGN KEY (id) REFERENCES ProfissionalSaude(id)
            );
        )",

        // HOSPITAL
        R"(
            CREATE TABLE IF NOT EXISTS Hospital (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                endereco TEXT
            );
        )",

        // SETOR
        R"(
            CREATE TABLE IF NOT EXISTS Setor (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                descricao TEXT,
                capacidadeMaxima INTEGER,
                hospital_id INTEGER,
                FOREIGN KEY (hospital_id) REFERENCES Hospital(id)
            );
        )",

        // RELAÇÃO N:N ENTRE SETOR E ESPECIALIDADE
        R"(
            CREATE TABLE IF NOT EXISTS SetorEspecialidade (
                setor_id INTEGER,
                especialidade_id INTEGER,
                PRIMARY KEY (setor_id, especialidade_id),
                FOREIGN KEY (setor_id) REFERENCES Setor(id),
                FOREIGN KEY (especialidade_id) REFERENCES Especialidade(id)
            );
        )",

        // LEITO
        R"(
            CREATE TABLE IF NOT EXISTS Leito (
                id INTEGER PRIMARY KEY,
                status TEXT NOT NULL,
                paciente_id INTEGER,
                setor_id INTEGER,
                FOREIGN KEY (paciente_id) REFERENCES Paciente(id),
                FOREIGN KEY (setor_id) REFERENCES Setor(id)
            );
        )",

        // ATENDIMENTO
        R"(
            CREATE TABLE IF NOT EXISTS Atendimento (
                id INTEGER PRIMARY KEY,
                data TEXT,
                status TEXT,
                prioridade INTEGER,
                tempoEstimadoMin REAL,
                paciente_id INTEGER NOT NULL,
                FOREIGN KEY (paciente_id) REFERENCES Paciente(id)
            );
        )",

        // PROFISSIONAIS QUE PARTICIPAM DE UM ATENDIMENTO
        R"(
            CREATE TABLE IF NOT EXISTS AtendimentoProfissional (
                atendimento_id INTEGER,
                profissional_id INTEGER,
                PRIMARY KEY (atendimento_id, profissional_id),
                FOREIGN KEY (atendimento_id) REFERENCES Atendimento(id),
                FOREIGN KEY (profissional_id) REFERENCES ProfissionalSaude(id)
            );
        )",

        // PROCEDIMENTO
        R"(
            CREATE TABLE IF NOT EXISTS Procedimento (
                id INTEGER PRIMARY KEY,
                nome TEXT,
                descricao TEXT,
                tipo TEXT,
                duracaoMin REAL,
                data TEXT,
                executado INTEGER,
                atendimento_id INTEGER,
                FOREIGN KEY (atendimento_id) REFERENCES Atendimento(id)
            );
        )",

        // RECURSO HOSPITALAR
        R"(
            CREATE TABLE IF NOT EXISTS RecursoHospitalar (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                descricao TEXT
            );
        )",

        // EQUIPAMENTO
        R"(
            CREATE TABLE IF NOT EXISTS Equipamento (
                id INTEGER PRIMARY KEY,
                status TEXT,
                FOREIGN KEY (id) REFERENCES RecursoHospitalar(id)
            );
        )",

        // MATERIAL
        R"(
            CREATE TABLE IF NOT EXISTS Material (
                id INTEGER PRIMARY KEY,
                unidadeMedida TEXT,
                quantidade REAL,
                FOREIGN KEY (id) REFERENCES RecursoHospitalar(id)
            );
        )",

        // RECURSOS DISPONÍVEIS EM CADA SETOR
        R"(
            CREATE TABLE IF NOT EXISTS SetorRecurso (
                setor_id INTEGER,
                recurso_id INTEGER,
                PRIMARY KEY (setor_id, recurso_id),
                FOREIGN KEY (setor_id) REFERENCES Setor(id),
                FOREIGN KEY (recurso_id) REFERENCES RecursoHospitalar(id)
            );
        )",

        // MEDICAMENTO
        R"(
            CREATE TABLE IF NOT EXISTS Medicamento (
                id INTEGER PRIMARY KEY,
                nome TEXT NOT NULL,
                descricao TEXT
            );
        )",

        // RECEITA MÉDICA
        R"(
            CREATE TABLE IF NOT EXISTS ReceitaMedica (
                id INTEGER PRIMARY KEY,
                data TEXT,
                observacoes TEXT,
                emitida INTEGER NOT NULL DEFAULT 0,
                paciente_id INTEGER,
                medico_id INTEGER,
                FOREIGN KEY (paciente_id) REFERENCES Paciente(id),
                FOREIGN KEY (medico_id) REFERENCES Medico(id)
            );
        )",

        // ITENS DE CADA RECEITA
        R"(
            CREATE TABLE IF NOT EXISTS ItemReceita (
                id INTEGER PRIMARY KEY,
                dosagem TEXT,
                quantidade INTEGER,
                descricaoUso TEXT,
                receita_id INTEGER NOT NULL,
                medicamento_id INTEGER NOT NULL,
                FOREIGN KEY (receita_id) REFERENCES ReceitaMedica(id),
                FOREIGN KEY (medicamento_id) REFERENCES Medicamento(id)
            );
        )",

        // FILA DE ATENDIMENTO
        R"(
            CREATE TABLE IF NOT EXISTS FilaAtendimento (
                id INTEGER PRIMARY KEY,
                setor_id INTEGER,
                FOREIGN KEY (setor_id) REFERENCES Setor(id)
            );
        )",

        // PACIENTES EM CADA FILA
        R"(
            CREATE TABLE IF NOT EXISTS ItemFila (
                fila_id INTEGER,
                atendimento_id INTEGER,
                posicao INTEGER,
                PRIMARY KEY (fila_id, atendimento_id),
                FOREIGN KEY (fila_id) REFERENCES FilaAtendimento(id),
                FOREIGN KEY (atendimento_id) REFERENCES Atendimento(id)
            );
        )"
    };

    const int quantidadeComandos =
        sizeof(comandos) / sizeof(comandos[0]);

    for (int i = 0; i < quantidadeComandos; i++) {
        if (!executarSQL(comandos[i])) {
            return false;
        }
    }

    return true;
}