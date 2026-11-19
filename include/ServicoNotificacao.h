#ifndef SERVICO_NOTIFICACAO_H
#define SERVICO_NOTIFICACAO_H

#include <vector>
#include <string>

class Usuario;
class Atendimento;
class Setor;
class Paciente;

class ServicoNotificacao{
private:
    std::vector<std::string> historicoNotificacoes;
public:
    ServicoNotificacao();
    ~ServicoNotificacao();

    bool enviarNotificacao(Usuario* u, const std::string& msg);
    void notificarMudancaSetor(Atendimento* a);
    void alertarSobrecarga(Setor* s);
    void chamarPaciente(Paciente* p);


    const std::vector<std::string>& getHistoricoNotificacoes() const;
    std::string getUltimaNotificacao() const;
    void limparHistorico();

};

#endif