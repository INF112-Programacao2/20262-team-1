
#ifndef REPOSITORIO_H
#define REPOSITORIO_H

#include <sqlite3.h>
#include <string>

class Repositorio {
private:
    sqlite3* m_conexao;
    std::string m_caminhoArquivoBD;

    bool conectar();
    void desconectar();
    bool criarTabelas();

public:
    Repositorio(const std::string& caminho);
    ~Repositorio();

    // Operações genéricas
    bool executarSQL(const std::string& sql);

    // Pacientes
    bool inserirPaciente(/* dados */);
    bool atualizarPaciente(/* dados */);
    bool removerPaciente(int id);
    bool buscarPaciente(int id);

    // Atendimentos
    bool inserirAtendimento(/* dados */);
    bool atualizarAtendimento(/* dados */);
    bool removerAtendimento(int id);
    bool buscarAtendimento(int id);

    // Leitos
    bool atualizarLeito(/* dados */);
    bool buscarLeito(int id);

    // Outros métodos...
};

#endif
