#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
#include <vector>

class Setor;

class Hospital {
private:
    std::string m_nome;
    std::string m_endereco;
    std::vector<Setor*> m_setores;

public:
    Hospital(std::string nome, std::string endereco);
    ~Hospital();

    void adicionarSetor(Setor* s);
    void removerSetor(Setor* s);
    void removerSetor(int sID);
    Setor* buscarSetor(int id);

    std::string getNome();
    std::string getEndereco();
};
#endif // HOSPITAL_H
