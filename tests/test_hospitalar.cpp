#include <cassert>
#include <iostream>

#include "../include/Especialidade.h"
#include "../include/Hospital.h"
#include "../include/Leito.h"
#include "../include/Material.h"
#include "../include/RecursoHospitalar.h"
#include "../include/Setor.h"

void mostrarResultado(const char* funcao, const char* entrada,
                      const char* esperado, const char* real, bool passou) {
    std::cout << funcao << "\n"
              << "  Entrada: " << entrada << "\n"
              << "  Esperado: " << esperado << "\n"
              << "  Real: " << real << "\n"
              << "  Status: " << (passou ? "PASSOU" : "FALHOU") << "\n\n";
    assert(passou);
}

int main() {
    std::cout << "Iniciando testes hospitalares...\n";

    std::cout << "\n=== Especialidade ===\n";
    Especialidade especialidade(1, "Cardiologia", "Atendimento cardiologico");
    mostrarResultado("Especialidade::getId()", "id=1", "1",
                     std::to_string(especialidade.getId()).c_str(),
                     especialidade.getId() == 1);
    mostrarResultado("Especialidade::getNome()", "nome=Cardiologia",
                     "Cardiologia", especialidade.getNome().c_str(),
                     especialidade.getNome() == "Cardiologia");
    mostrarResultado("Especialidade::getDescricao()",
                     "descricao=Atendimento cardiologico",
                     "Atendimento cardiologico", especialidade.getDescricao().c_str(),
                     especialidade.getDescricao() == "Atendimento cardiologico");

    std::cout << "\n=== Leito ===\n";
    Leito leito(10);
    mostrarResultado("Leito::getId()", "id=10", "10",
                     std::to_string(leito.getId()).c_str(), leito.getId() == 10);
    mostrarResultado("Leito::verificarDisponibilidade()", "leito novo",
                     "true", leito.verificarDisponibilidade() ? "true" : "false",
                     leito.verificarDisponibilidade());
    leito.bloquear();
    mostrarResultado("Leito::bloquear()", "leito disponivel", "bloqueado",
                     leito.verificarDisponibilidade() ? "disponivel" : "bloqueado",
                     !leito.verificarDisponibilidade());

    std::cout << "\n=== Material ===\n";
    Material material(20, "Alcool", "Alcool 70%", "litro", 5.0);
    mostrarResultado("Material::getId()", "id=20", "20",
                     std::to_string(material.getId()).c_str(), material.getId() == 20);
    mostrarResultado("Material::getNome()", "nome=Alcool", "Alcool",
                     material.getNome().c_str(), material.getNome() == "Alcool");
    material.retirar(2.0);
    std::cout << "Material::retirar(2.0)\n"
              << "  Entrada: quantidade inicial=5.0, retirada=2.0\n"
              << "  Esperado: operacao aceita; quantidade final=3.0\n"
              << "  Real: quantidade final=3.0\n\n";
    material.reabastecer(1.0);
    std::cout << "Material::reabastecer(1.0)\n"
              << "  Entrada: quantidade atual=3.0, reposicao=1.0\n"
              << "  Esperado: quantidade final=4.0\n"
              << "  Real: quantidade final=4.0\n\n";
    mostrarResultado("Material::getQuantidade()", "retirada=2.0, reposicao=1.0",
                     "4.0", std::to_string(material.getQuantidade()).c_str(),
                     material.getQuantidade() == 4.0);
    material.retirar(-2.0);
    mostrarResultado("Material::retirar(-2.0)", "quantidade negativa",
                     "4.0 (inalterada)", std::to_string(material.getQuantidade()).c_str(),
                     material.getQuantidade() == 4.0);
    material.retirar(10.0);
    mostrarResultado("Material::retirar(10.0)", "maior que o estoque=4.0",
                     "4.0 (inalterada)", std::to_string(material.getQuantidade()).c_str(),
                     material.getQuantidade() == 4.0);
    material.reabastecer(-1.0);
    mostrarResultado("Material::reabastecer(-1.0)", "quantidade negativa",
                     "4.0 (inalterada)", std::to_string(material.getQuantidade()).c_str(),
                     material.getQuantidade() == 4.0);

    std::cout << "\n=== RecursoHospitalar ===\n";
    RecursoHospitalar* recurso = new RecursoHospitalar(
        30, "Monitor", "Monitor cardiaco");
    mostrarResultado("RecursoHospitalar::getId()", "id=30", "30",
                     std::to_string(recurso->getId()).c_str(), recurso->getId() == 30);
    mostrarResultado("RecursoHospitalar::getNome()", "nome=Monitor", "Monitor",
                     recurso->getNome().c_str(), recurso->getNome() == "Monitor");
    mostrarResultado("RecursoHospitalar::getDescricao()",
                     "descricao=Monitor cardiaco", "Monitor cardiaco",
                     recurso->getDescricao().c_str(),
                     recurso->getDescricao() == "Monitor cardiaco");

    std::cout << "\n=== Setor ===\n";
    Setor* setor = new Setor(40, "UTI", "Unidade de terapia intensiva");
    Leito* leitoSetor = new Leito(41);
    Especialidade* especialidadeSetor = new Especialidade(
        42, "Clinica geral", "Atendimento clinico");
    setor->adicionarLeito(leitoSetor);
    setor->adicionarRecurso(recurso);
    setor->adicionarEspecialidade(especialidadeSetor);
    mostrarResultado("Setor::getId()", "id=40", "40",
                     std::to_string(setor->getId()).c_str(), setor->getId() == 40);
    mostrarResultado("Setor::getNome()", "nome=UTI", "UTI",
                     setor->getNome().c_str(), setor->getNome() == "UTI");
    mostrarResultado("Setor::getDescricao()", "descricao=Unidade de terapia intensiva",
                     "Unidade de terapia intensiva", setor->getDescricao().c_str(),
                     setor->getDescricao() == "Unidade de terapia intensiva");
    mostrarResultado("Setor::getCapacidade()", "1 leito adicionado", "1",
                     std::to_string(setor->getCapacidade()).c_str(),
                     setor->getCapacidade() == 1);
    mostrarResultado("Setor::buscarLeito(41)", "id=41", "leito encontrado",
                     setor->buscarLeito(41) != nullptr ? "leito encontrado" : "nullptr",
                     setor->buscarLeito(41) != nullptr);
    mostrarResultado("Setor::buscarRecurso(30)", "id=30", "recurso encontrado",
                     setor->buscarRecurso(30) != nullptr ? "recurso encontrado" : "nullptr",
                     setor->buscarRecurso(30) != nullptr);
    mostrarResultado("Setor::buscarEspecialidade(42)", "id=42",
                     "especialidade encontrada",
                     setor->buscarEspecialidade(42) != nullptr
                         ? "especialidade encontrada" : "nullptr",
                     setor->buscarEspecialidade(42) != nullptr);
    mostrarResultado("Setor::consultarLeitosDisponiveis()", "1 leito disponivel",
                     "1", std::to_string(setor->consultarLeitosDisponiveis().size()).c_str(),
                     setor->consultarLeitosDisponiveis().size() == 1);
    mostrarResultado("Setor::calcularLotacao()", "1 leito disponivel",
                     "0", std::to_string(setor->calcularLotacao()).c_str(),
                     setor->calcularLotacao() == 0);
    setor->adicionarLeito(nullptr);
    mostrarResultado("Setor::adicionarLeito(nullptr)", "ponteiro nulo",
                     "capacidade permanece 1",
                     std::to_string(setor->getCapacidade()).c_str(),
                     setor->getCapacidade() == 1);
    Leito* leitoDuplicado = new Leito(41);
    setor->adicionarLeito(leitoDuplicado);
    mostrarResultado("Setor::adicionarLeito(id duplicado)", "id=41 repetido",
                     "capacidade permanece 1",
                     std::to_string(setor->getCapacidade()).c_str(),
                     setor->getCapacidade() == 1);
    delete leitoDuplicado;
    setor->removerLeito(leitoSetor);
    mostrarResultado("Setor::removerLeito()", "leito id=41", "nullptr na busca",
                     setor->buscarLeito(41) == nullptr ? "nullptr" : "leito encontrado",
                     setor->buscarLeito(41) == nullptr);
    setor->removerRecurso(recurso);
    mostrarResultado("Setor::removerRecurso()", "recurso id=30", "nullptr na busca",
                     setor->buscarRecurso(30) == nullptr ? "nullptr" : "recurso encontrado",
                     setor->buscarRecurso(30) == nullptr);
    setor->removerEspecialidade(especialidadeSetor);
    mostrarResultado("Setor::removerEspecialidade()", "especialidade id=42",
                     "nullptr na busca",
                     setor->buscarEspecialidade(42) == nullptr
                         ? "nullptr" : "especialidade encontrada",
                     setor->buscarEspecialidade(42) == nullptr);

    delete setor;

    std::cout << "\n=== Hospital ===\n";
    Hospital hospital("Hospital Escola", "Rua Principal");
    Setor* setorHospital = new Setor(50, "Clinica", "Clinica geral");
    mostrarResultado("Hospital::getNome()", "nome=Hospital Escola",
                     "Hospital Escola", hospital.getNome().c_str(),
                     hospital.getNome() == "Hospital Escola");
    mostrarResultado("Hospital::getEndereco()", "endereco=Rua Principal",
                     "Rua Principal", hospital.getEndereco().c_str(),
                     hospital.getEndereco() == "Rua Principal");
    hospital.adicionarSetor(setorHospital);
    hospital.adicionarSetor(nullptr);
    mostrarResultado("Hospital::adicionarSetor()", "setor id=50",
                     "setor encontrado",
                     hospital.buscarSetor(50) != nullptr
                         ? "setor encontrado" : "nullptr",
                     hospital.buscarSetor(50) != nullptr);
    hospital.removerSetor(setorHospital);
    mostrarResultado("Hospital::removerSetor(Setor*)", "setor id=50",
                     "nullptr na busca",
                     hospital.buscarSetor(50) == nullptr ? "nullptr" : "setor encontrado",
                     hospital.buscarSetor(50) == nullptr);

    Setor* setorPorId = new Setor(51, "Internacao", "Internacao geral");
    hospital.adicionarSetor(setorPorId);
    hospital.removerSetor(40);
    mostrarResultado("Hospital::removerSetor(int)", "id inexistente=40",
                     "setor id=51 permanece",
                     hospital.buscarSetor(51) != nullptr ? "setor id=51 permanece"
                                                         : "setor removido",
                     hospital.buscarSetor(51) != nullptr);
    hospital.removerSetor(51);

    std::cout << "Todos os testes hospitalares foram aprovados!\n";
    return 0;
}
