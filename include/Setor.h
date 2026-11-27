#ifndef SETOR_H
#define SETOR_H

#include <string>
#include <vector>

class Leito;
class RecursoHospitalar;
class Especialidade;

class Setor {
private:
    int m_id;
    std::string m_nome;
    std::string m_descricao;
    std::vector<Leito*> m_leitos;
    std::vector<RecursoHospitalar*> m_recursos;
    std::vector<Especialidade*> m_especialidades;
    int m_capacidadeMaxima;

public:
    ~Setor();
    Setor(int id, std::string nome, std::string descricao);

    void adicionarLeito(Leito* l);
    void removerLeito(Leito* l);
    Leito* buscarLeito(int id);

    void adicionarRecurso(RecursoHospitalar* r);
    void removerRecurso(RecursoHospitalar* r);
    RecursoHospitalar* buscarRecurso(int id);

    void adicionarEspecialidade(Especialidade* e);
    void removerEspecialidade(Especialidade* e);
    Especialidade* buscarEspecialidade(int id);

    int calcularLotacao();
    std::vector<Leito*> consultarLeitosDisponiveis();
    std::vector<RecursoHospitalar*> consultarRecursos();
    
    int getId();
    std::string getNome();
    std::string getDescricao();
    int getCapacidade();
    const std::vector<Leito*>& getLeitos() const;
    const std::vector<Especialidade*>& getEspecialidades() const;
};
#endif // SETOR_H
