#ifndef CONTROLEACESSO_H
#define CONTROLEACESSO_H

class Usuario;

// Diz o que cada tipo de usuario pode fazer no sistema
// Nao guarda nada, so olha o tipo real do usuario recebido
class ControleAcesso {
public:
    bool podeGerenciarSetores(Usuario* usuario) const;               // administrador
    bool podeGerenciarFila(Usuario* usuario) const;                  // atendente
    bool podeRealizarAtendimento(Usuario* usuario) const;            // profissional de saude
    bool podeVisualizarProprioAtendimento(Usuario* usuario) const;   // paciente
};

#endif
