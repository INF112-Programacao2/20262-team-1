#ifndef USUARIO_H
#define USUARIO_H

#include <string>

// Classe base de todo mundo que usa o sistema: paciente, acompanhante e funcionarios
// E abstrata, cada tipo mostra seus dados do seu jeito
class Usuario {
private:
    int m_id;
    std::string m_nome;
    std::string m_cpf;
    int m_idade;

public:
    Usuario(int id, const std::string& nome, const std::string& cpf, int idade);
    virtual ~Usuario();

    virtual void visualizarDados() const = 0;

    int getId() const;
    std::string getNome() const;
    std::string getCpf() const;
    int getIdade() const;

    void setNome(const std::string& nome);
    void setIdade(int idade);
};

#endif
