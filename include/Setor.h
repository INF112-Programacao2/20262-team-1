#ifndef SETOR_H
#define SETOR_H

#include <string>
#include <vector>

class Leito;
class RecursosHospitalar;
class Especialidade;

class Setor {
private:
    int m_id;
    std::string m_nome;
    std::string m_descricao;
    std::vector<Leito*> m_leitos;
    std::vector<RecursosHospitalar*> m_recursos;
    std::vector<Especialidade*> m_especialidades;
    int m_capacidadeMaxima;

public:
    ~Setor();
    Setor(int id, std::string nome, std::string descricao,
          int capacidadeMaxima);

    void adicionarLeito(Leito* l);
    void removerLeito(Leito* l);
    Leito* buscarLeito(int id);

    void adicionarRecurso(RecursosHospitalar* r);
    void removerRecurso(RecursosHospitalar* r);
    void removerRecurso(int rID);
    RecursosHospitalar* buscarRecurso(int id);

    void adicionarEspecialidade(Especialidade* e);
    void removerEspecialidade(Especialidade* e);
    void removerEspecialidade(int eID);
    Especialidade* buscarEspecialidade(int id);

    int calcularLotacao();
    std::vector<Leito*> consultarLeitosDisponiveis();
    std::vector<RecursosHospitalar*> consultarRecursos();
    
    int getId();
    std::string getNome();
    std::string getDescricao();
};
#endif // SETOR_H
