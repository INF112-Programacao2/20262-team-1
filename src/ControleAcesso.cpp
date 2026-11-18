#include "ControleAcesso.h"
#include "Usuario.h"
#include "Administrador.h"
#include "Atendente.h"
#include "ProfissionalSaude.h"
#include "Paciente.h"

// dynamic_cast devolve nullptr se o usuario nao for do tipo pedido
// Usuario nulo nunca tem permissao
bool ControleAcesso::podeGerenciarSetores(Usuario* usuario) const {
    return dynamic_cast<Administrador*>(usuario) != nullptr;
}

bool ControleAcesso::podeGerenciarFila(Usuario* usuario) const {
    return dynamic_cast<Atendente*>(usuario) != nullptr;
}

bool ControleAcesso::podeRealizarAtendimento(Usuario* usuario) const {
    return dynamic_cast<ProfissionalSaude*>(usuario) != nullptr;
}

bool ControleAcesso::podeVisualizarProprioAtendimento(Usuario* usuario) const {
    return dynamic_cast<Paciente*>(usuario) != nullptr;
}
