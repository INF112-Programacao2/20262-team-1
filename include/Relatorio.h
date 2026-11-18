#ifndef RELATORIO_H
#define RELATORIO_H

#include <string>
#include <vector>

class Setor;
class FilaAtendimento;
class GerenciadorFluxo;

class Relatorio {
private:
    std::string m_dados;
    std::string m_tipo;

public:
    Relatorio();
    ~Relatorio();


    std::string gerar() const;
    bool exportar(const std::string& caminhoArquivo) const;
    std::string gerarRelatorioOcupacao(const std::vector<Setor*>& setores, GerenciadorFluxo* fluxo = nullptr);
    std::string gerarRelatorioFilas(const std::vector<FilaAtendimento*>& filas, const GerenciadorFluxo* fluxo = nullptr);

    std::string getDados() const;
    std::string getTipo() const;
};

#endif 