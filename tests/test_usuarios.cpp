#include <iostream>
#include "../include/Acompanhante.h"
#include "../include/Administrador.h"
#include "../include/Atendente.h"
#include "../include/ProfissionalSaude.h"
#include "../include/ControleAcesso.h"
#include "../include/Atendimento.h"
#include "../include/FilaAtendimento.h"
#include "../include/Especialidade.h"
#include "../include/Hospital.h"
#include "../include/Setor.h"

int main() {
    std::cout << "=================================================\n";
    std::cout << "        TESTE DE USUARIOS E CONTROLE DE ACESSO    \n";
    std::cout << "=================================================\n";

    Especialidade cardio(1, "Cardiologia", "Doencas do coracao");

    Acompanhante acomp(1, "Joana", "111.111.111-11", 45, "mae");
    ProfissionalSaude medico(2, "Dr. Carlos", "222.222.222-22", 40, "M001", "CRM-1234", &cardio);
    Atendente atendente(3, "Ana", "333.333.333-33", 30, "M002");
    Administrador admin(4, "Beto", "444.444.444-44", 50, "M003");

    // 1. Dados de cada tipo (cada um mostra do seu jeito)
    std::cout << "\n[1] Visualizando dados (polimorfismo via Usuario*):\n";
    Usuario* usuarios[] = {&acomp, &medico, &atendente, &admin};
    for (Usuario* u : usuarios) {
        u->visualizarDados();
    }

    // 2. Setor do funcionario
    std::cout << "\n[2] Alocando o medico em um setor...\n";
    Setor* cardiologia = new Setor(10, "Cardiologia", "Setor do coracao");
    medico.setSetor(cardiologia);
    std::cout << "Setor do medico: " << medico.getSetor()->getNome() << " (Esperado: Cardiologia)\n";
    medico.setSetor(nullptr);
    delete cardiologia;

    // 3. Controle de acesso
    std::cout << "\n[3] Controle de acesso:\n";
    ControleAcesso acesso;
    std::cout << "Admin gerencia setores? " << (acesso.podeGerenciarSetores(&admin) ? "SIM" : "NAO") << " (Esperado: SIM)\n";
    std::cout << "Atendente gerencia setores? " << (acesso.podeGerenciarSetores(&atendente) ? "SIM" : "NAO") << " (Esperado: NAO)\n";
    std::cout << "Atendente gerencia fila? " << (acesso.podeGerenciarFila(&atendente) ? "SIM" : "NAO") << " (Esperado: SIM)\n";
    std::cout << "Medico realiza atendimento? " << (acesso.podeRealizarAtendimento(&medico) ? "SIM" : "NAO") << " (Esperado: SIM)\n";
    std::cout << "Acompanhante realiza atendimento? " << (acesso.podeRealizarAtendimento(&acomp) ? "SIM" : "NAO") << " (Esperado: NAO)\n";
    std::cout << "Usuario nulo gerencia fila? " << (acesso.podeGerenciarFila(nullptr) ? "SIM" : "NAO") << " (Esperado: NAO)\n";

    // 4. Atendente e fila
    std::cout << "\n[4] Atendente encaminhando para a fila:\n";
    FilaAtendimento fila;
    Atendimento* a1 = new Atendimento(1, 15.0, "Paciente A", 1, "18/11/2026", nullptr);
    Atendimento* a2 = new Atendimento(5, 10.0, "Paciente B", 2, "18/11/2026", nullptr);
    std::cout << "Encaminhou A? " << (atendente.encaminharParaFila(a1, &fila) ? "SIM" : "NAO") << " (Esperado: SIM)\n";
    std::cout << "Encaminhou B? " << (atendente.encaminharParaFila(a2, &fila) ? "SIM" : "NAO") << " (Esperado: SIM)\n";
    std::cout << "Encaminhar A de novo? " << (atendente.encaminharParaFila(a1, &fila) ? "SIM" : "NAO") << " (Esperado: NAO)\n";
    fila.visualizarFila();

    // 5. Chamar proximo e medico atendendo
    std::cout << "\n[5] Chamando o proximo (Esperado: Paciente B, prioridade 5):\n";
    Atendimento* proximo = atendente.chamarProximo(&fila);
    std::cout << "Chamado: " << proximo->getIdPaciente() << "\n";
    std::cout << "Status em atendimento? " << (proximo->getStatus() == StatusAtendimento::EM_ATENDIMENTO ? "SIM" : "NAO") << " (Esperado: SIM)\n";

    medico.realizarAtendimento(a1);
    medico.realizarAtendimento(a1); // repetido nao deve duplicar
    std::cout << "Equipe do atendimento A: " << a1->getEquipeResponsavel().size() << " (Esperado: 1)\n";
    atendente.chamarProximo(&fila); // chama o A, que era o que sobrava
    std::cout << "Chamar em fila vazia retorna nullptr? " << (atendente.chamarProximo(&fila) == nullptr ? "SIM" : "NAO") << " (Esperado: SIM)\n";

    // 6. Administrador e hospital
    std::cout << "\n[6] Administrador gerenciando setores:\n";
    Hospital hospital("Hospital Teste", "Rua A, 10");
    admin.adicionarSetor(&hospital, new Setor(20, "UTI", "Terapia intensiva"));
    std::cout << "Achou setor 20? " << (hospital.buscarSetor(20) != nullptr ? "SIM" : "NAO") << " (Esperado: SIM)\n";
    admin.removerSetor(&hospital, 20);
    std::cout << "Achou setor 20 depois de remover? " << (hospital.buscarSetor(20) != nullptr ? "SIM" : "NAO") << " (Esperado: NAO)\n";

    delete a1;
    delete a2;

    std::cout << "\n=================================================\n";
    std::cout << "           TODOS OS TESTES CONCLUIDOS!           \n";
    std::cout << "=================================================\n";
    return 0;
}
