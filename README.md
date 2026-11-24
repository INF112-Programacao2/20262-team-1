# Sistema de Gestão de Atendimento e Fluxo Hospitalar

Sistema desenvolvido em C++ para gerenciamento do atendimento e do fluxo hospitalar, com foco na organização de pacientes, profissionais, setores e atendimentos.

## Funcionalidades

### Modo Interativo
- Cadastro e gerenciamento de pacientes e usuários;
- Gerenciamento do fluxo de consultas e atendimentos;
- Gerenciamento de setores, quartos e leitos;
- Acompanhamento da fila de atendimento e consultas.

### Modo Headless
- Processamento em lote de novos pacientes;
- Geração de relatórios.

## Tecnologias utilizadas

- **C++17:** linguagem de programação;
- **SQLite:** armazenamento e gerenciamento dos dados;
- **Makefile:** automação da compilação do projeto.

## Pré-requisitos

Para compilar e executar o projeto, é necessário ter instalado:

- Compilador C++ com suporte a C++17;
- GNU Make;
- SQLite3 e sua biblioteca de desenvolvimento.

Em distribuições Ubuntu/Debian, as dependências podem ser instaladas com:

```bash
sudo apt update
sudo apt install g++ make libsqlite3-dev sqlite3
```