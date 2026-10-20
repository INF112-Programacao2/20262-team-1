# Contrato de Desenvolvimento — Sistema de Gestão Hospitalar

## 1. Objetivo

Este documento estabelece acordos de organização, programação e colaboração para o desenvolvimento do **Sistema de Gestão de Atendimento e Fluxo Hospitalar**, desenvolvido como projeto final da disciplina de Programação 2 da UFV.

O objetivo é manter o código organizado, facilitar o trabalho em paralelo e garantir que todos os integrantes participem efetivamente do desenvolvimento.

## 2. Organização dos arquivos

O projeto seguirá a seguinte estrutura:

- `include/`: arquivos de cabeçalho (`.h`), contendo declarações de classes, atributos e métodos.
- `src/`: arquivos de implementação (`.cpp`), contendo a implementação dos métodos e a lógica do sistema.
- `tests/`: arquivos destinados aos testes das funcionalidades.
- `.gitignore`: define arquivos e pastas que não devem ser rastreados pelo Git.
- `README.md`: apresenta o projeto, seus objetivos e instruções de execução.
- `LICENSE.md`: contém a licença do projeto.
- `CONTRATO.md`: registra os acordos de desenvolvimento do grupo.

Arquivos novos devem ser colocados no diretório adequado, mantendo a estrutura consistente.

## 3. Padrões de programação

### 3.1. Organização e nomenclatura

Para manter o código legível e consistente, adotaremos as seguintes convenções:

- **Classes e structs:** `PascalCase`, por exemplo, `Paciente`, `Atendimento` e `FilaAtendimento`.
- **Métodos e funções:** `camelCase`, por exemplo, `adicionarPaciente()` e `consultarStatus()`.
- **Atributos privados:** prefixo `m_`, por exemplo, `m_nome` e `m_status`.
- **Constantes:** nomes descritivos, preferencialmente em `UPPER_SNAKE_CASE`, por exemplo, `CAPACIDADE_MAXIMA`.

Os nomes devem ser claros e descrever a finalidade dos elementos.

### 3.2. Orientação a objetos

O sistema deverá aplicar os conceitos de Programação Orientada a Objetos estudados na disciplina.

- Cada classe deve possuir responsabilidades bem definidas.
- Os atributos devem ser privados sempre que possível, com acesso controlado por métodos apropriados.
- A herança deve ser utilizada quando existir uma relação real de especialização entre classes.
- O polimorfismo deve ser aplicado quando diferentes tipos precisarem responder de maneiras distintas a uma mesma operação.
- A composição deve representar relações em que uma classe é formada por ou gerencia componentes de outra.
- Métodos e atributos não devem ser duplicados sem necessidade.
- Classes não devem concentrar responsabilidades que pertencem a outras partes do sistema.

O diagrama de classes servirá como referência para a implementação. Alterações na estrutura são permitidas quando justificadas, desde que sejam comunicadas ao grupo e documentadas quando necessário.

### 3.3. Qualidade do código

- Evitar código duplicado e métodos excessivamente longos.
- Utilizar nomes descritivos para variáveis e métodos.
- Evitar números e valores fixos espalhados pelo código quando puderem ser representados por constantes.
- Incluir comentários quando a lógica não for evidente, sem comentar desnecessariamente cada linha.
- Evitar funções que alterem dados sem deixar claro esse comportamento.
- Corrigir avisos relevantes do compilador sempre que possível.
- Utilizar recursos de C++ compatíveis com o ambiente e as exigências da disciplina.

## 4. Organização do trabalho

Todos os integrantes devem participar ativamente da programação e compreender as partes que desenvolveram.

A divisão inicial de tarefas serve para organizar o trabalho, mas não impede contribuições em outras partes do sistema.

Cada integrante deve:

- Implementar e testar suas funcionalidades.
- Manter suas alterações organizadas.
- Comunicar mudanças que afetem classes ou funcionalidades utilizadas por outros integrantes.
- Pedir ajuda quando houver dificuldades que possam comprometer a integração.
- Evitar modificar ou sobrescrever o trabalho de outra pessoa sem alinhamento prévio.

A divisão de responsabilidades não significa que cada integrante trabalhará isoladamente. O sistema deverá ser integrado continuamente.

## 5. Uso do Git e GitHub

### 5.1. Commits

Os commits devem ser frequentes, pequenos e representar alterações compreensíveis.

As mensagens devem descrever objetivamente o que foi feito. Exemplos:

- `feat: adiciona cadastro de pacientes`
- `feat: implementa prioridade na fila de atendimento`
- `fix: corrige atualização do status do atendimento`
- `test: adiciona testes para a classe Setor`
- `docs: atualiza instruções do README`

Cada integrante deve realizar commits de suas próprias alterações, evitando acumular todo o trabalho para o final.

### 5.2. Branches e integração

Sempre que possível, funcionalidades novas devem ser desenvolvidas em branches próprias, para facilitar a revisão e reduzir conflitos.

A branch principal deve permanecer em uma condição funcional sempre que viável.

Antes de integrar alterações, o responsável deve:

- Verificar se o código compila.
- Executar os testes disponíveis.
- Conferir se arquivos temporários ou artefatos de compilação não foram adicionados.
- Comunicar possíveis alterações incompatíveis com outras partes do sistema.

Conflitos do Git devem ser resolvidos com cuidado, preservando as contribuições dos envolvidos.

## 6. Testes e integração

Cada funcionalidade implementada deve ser testada antes de ser considerada concluída.

Sempre que possível, os testes devem verificar tanto o comportamento esperado quanto situações inválidas ou limites relevantes.

Ao modificar uma classe já utilizada por outras partes do sistema, o integrante deve verificar se a alteração afeta funcionalidades existentes.

A integração deve acontecer ao longo do desenvolvimento, e não apenas próximo à entrega final.

## 7. Uso de ferramentas de inteligência artificial

Ferramentas de inteligência artificial podem ser utilizadas como apoio para estudar, planejar, implementar, revisar e depurar o código.

Entretanto:

- O integrante responsável deve compreender o código que adicionar ao projeto.
- O código gerado deve ser revisado e testado antes de ser integrado.
- Soluções sugeridas por IA não devem ser aceitas sem avaliar sua adequação ao projeto.
- Dúvidas sobre conceitos ou implementações devem ser discutidas com o grupo.
- O uso dessas ferramentas deve respeitar as orientações da disciplina e do professor.

A responsabilidade pelo código enviado ao repositório continua sendo do integrante que realizou a alteração.

## 8. Comunicação e tomada de decisões

Como os integrantes possuem disponibilidades diferentes, a comunicação deve ser objetiva e priorizar o registro das decisões importantes.

- Mudanças que afetem outras classes devem ser comunicadas antes da integração, sempre que possível.
- Decisões relevantes sobre arquitetura, interfaces entre classes e funcionamento do sistema devem ser registradas no canal utilizado pelo grupo.
- Dúvidas não devem impedir o trabalho por tempo indeterminado: o integrante deve comunicar o problema e buscar uma solução com os demais.
- Alterações de escopo devem ser avaliadas considerando o tempo disponível e os requisitos da disciplina.

O objetivo é reduzir a necessidade de reuniões frequentes sem comprometer a colaboração.

## 9. Evolução do projeto

O sistema poderá evoluir conforme as necessidades identificadas durante a implementação.

O diagrama de classes e os requisitos iniciais são referências para o desenvolvimento, mas podem ser ajustados quando houver justificativa técnica.

Entre as funcionalidades previstas estão o gerenciamento de pacientes e atendimentos, a organização de filas, a classificação de prioridades, o encaminhamento para setores hospitalares e as simulações em lote com geração de relatórios.

As funcionalidades devem ser implementadas de forma incremental, priorizando uma base funcional antes de adicionar recursos mais complexos.



**Última atualização:** a preencher pelo grupo.
