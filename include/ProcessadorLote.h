#ifndef PROCESSADOR_LOTE_H
#define PROCESSADOR_LOTE_H

#include <string>
#include <vector>

class Paciente;
class Atendimento;
class FilaAtendimento;

class ProcessadorLote {
private:
    std::string m_arquivoEntrada;
    std::vector<Paciente*> m_pacientesImportados;
    std::vector<Atendimento*> m_atendimentosImportados;

public:
    ProcessadorLote(const std::string& arquivoEntrada = "");
    ~ProcessadorLote();

    bool importarDados();
    int processarPacientes(FilaAtendimento* fila);

    void setArquivoEntrada(const std::string& caminho);
    std::string getArquivoEntrada() const;

    const std::vector<Paciente*>& getPacientesImportados() const;
    const std::vector<Atendimento*>& getAtendimentosImportados() const;
    int getQuantidadeImportada() const;

    void limparDados();
};

#endif