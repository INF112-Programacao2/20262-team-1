# Guia didático para continuar o backend C++

Este documento explica como ligar o frontend ao núcleo C++ existente. O
frontend já possui a camada HTTP, mas **não há servidor HTTP nem biblioteca
JSON neste repositório**. Portanto, as respostas descritas aqui são contratos
para a próxima etapa, não funcionalidades já implementadas.

## 1. Visão geral da arquitetura

```text
frontend/index.html
  -> js/auth.js e js/main.js                 seleção de perfil
frontend/pages/*/*.html
  -> js/auth.js                              proteção da página
  -> js/app.js                               layout, telas e eventos
     -> js/components.js                    modais e notificações
     -> js/api/services/*-service.js        operação do domínio
        -> js/api/api.js                     cliente HTTP único
           -> api-config.js / endpoints.js  configuração e rotas

include/*.h + src/*.cpp                     regras de domínio C++ existentes
backend/API_CONTRACT.md                     contrato que o servidor deve cumprir
```

### Frontend

- `frontend/index.html`: tela de escolha de perfil, sem senha real.
- `frontend/pages/`: uma página HTML por perfil e funcionalidade.
- `frontend/css/global.css`: identidade visual, layout, tabelas e formulários.
- `frontend/css/components.css`: aparência dos modais e toasts.
- `frontend/js/main.js`: renderiza os cinco cartões de perfil.
- `frontend/js/auth.js`: grava `vidacare-role` no `sessionStorage`, redireciona
  ao dashboard e impede acesso sem perfil.
- `frontend/js/app.js`: monta sidebar/header, usa dados locais somente para
  visualização e conecta eventos aos serviços da API.
- `frontend/js/components.js`: cria modal, valida formulários, controla
  carregamento, ESC, clique externo e mensagens.
- `frontend/js/api/api-config.js`: única origem de `API_BASE_URL`.
- `frontend/js/api/api.js`: implementa GET, POST, PUT, DELETE, JSON, timeout e
  erros.
- `frontend/js/api/services/`: funções com nomes do domínio; nenhum HTML faz
  `fetch` diretamente.

Cada página inclui `auth.js` e `app.js`. O `app.js` importa
`components.js` como módulo. Os serviços também são importados sob demanda
quando o usuário confirma uma operação.

### Navegação e perfis

Ao clicar em um cartão, `VidaCareAuth.select(role)` grava o perfil e navega
para `pages/<perfil>/dashboard.html`. Cada página chama `require()`. Se o
perfil não estiver na sessão, a página volta à seleção. O menu é escolhido a
partir do mapa `menus` em `app.js`; assim, o perfil acompanhante não recebe
links administrativos, por exemplo.

Isto **não é autenticação**: é somente uma seleção local para demonstração.
Quando houver login real, substitua a seleção por uma resposta do backend e
preserve a mesma ideia de sessão/permissão.

### Dados fictícios

`app.js` mantém um pequeno conjunto local de pacientes para desenhar tabelas.
Eles não são resposta HTTP. A interface informa essa condição em detalhes e
não transforma uma falha de rede em sucesso local. Operações de escrita
aguardam a resposta do backend; portanto, sem servidor, o modal permanece
aberto com “Backend indisponível”.

## 2. Como os modais funcionam

`openModal({ title, content, onConfirm })` em `components.js`:

1. cria um `div.modal-backdrop` e um `section.modal`;
2. insere o formulário ou conteúdo fornecido;
3. valida `form.reportValidity()` antes de confirmar;
4. mostra “Aguardando servidor...” enquanto a promessa está pendente;
5. chama `onConfirm` e só fecha quando ele termina com sucesso;
6. mantém o modal aberto em erro;
7. permite fechar por X, cancelar, ESC ou clique no fundo.

Visualização local pode fechar imediatamente. Cadastro, edição, exclusão,
alocação, prescrição e execução chamam um serviço e só fecham depois de
`success: true`.

## 3. Caminho completo: editar usuário

1. A tabela de `app.js` cria o botão `data-action="edit-user"`.
2. O listener abre `openModal()` com `userForm(nome, true)`.
3. O formulário recebe nome, CPF, tipo, e-mail e status.
4. O usuário altera os campos e clica em **Salvar**.
5. `components.js` coleta `Object.fromEntries(new FormData(form))`.
6. `app.js` importa `usuarios-service.js` e chama `editarUsuario(dados)`.
7. O serviço monta `PUT /api/usuarios/{id}` (ou o formato de ação definido
   pelo contrato) e envia JSON por `api.put()`.
8. O servidor C++ roteia a URL, desserializa JSON e executa a regra.
9. O domínio atualiza o objeto correspondente e valida permissões.
10. O servidor devolve `{ success, message, data }`.
11. `api.js` rejeita HTTP não-2xx ou `success: false`.
12. Em sucesso, `app.js` fecha o modal e mostra toast; em erro, o modal fica
    aberto e os dados permanecem editáveis.

Responsabilidades: `components.js` cuida da interface; `app.js` coordena a
página; `usuarios-service.js` conhece o endpoint; `api.js` conhece HTTP; o
backend C++ conhece regra de negócio. O frontend não deve decidir se um
usuário pode ser removido.

## 4. Estado atual do C++ e pontos de extensão

Os cabeçalhos existentes são a fonte da verdade:

| Domínio | Classes/métodos existentes | Observação |
|---|---|---|
| Usuário | `Usuario::getNome`, `getCpf`, `setNome`, `setIdade`; `Funcionario` | Não existe cadastro/lista global de usuários. CRUD exige uma extensão de armazenamento/serviço. |
| Permissão | `ControleAcesso::podeGerenciarSetores`, `podeGerenciarFila`, `podeRealizarAtendimento`, `podeVisualizarProprioAtendimento` | Pode ser usado pelo roteador antes da regra. |
| Paciente | `Paciente` constructor, `adicionarAtendimento`, `getAtendimentoAtual`, `consultarStatusAtendimento`, `adicionarReceita` | Não há identificador global ou cadastro em `Hospital`; o servidor precisará organizar a coleção. |
| Fila | `FilaAtendimento::adicionarAtendimento`, `chamarProximo`, `reorganizarFila`, `estimarTempoDeFila` | Pronto para consulta/chamada após localizar a fila correta. |
| Atendimento | `inicializar`, `atualizarStatus`, `finalizar`, `adicionarProcedimento`, `executarProcedimentos` | O método `finalizar()` pode recusar pendências. |
| Profissional | `ProfissionalSaude::realizarAtendimento` | Médico/enfermeiro entra na equipe e inicia atendimento. |
| Setor | `calcularLotacao`, `consultarLeitosDisponiveis`, `consultarRecursos`, `buscarLeito` | Administrador usa `adicionarSetor`/`removerSetor` em `Hospital`. |
| Leito | `alocarPaciente`, `bloquear`, `liberar`, `verificarDisponibilidade` | O servidor deve localizar o objeto e traduzir erros para HTTP. |
| Recursos | `Equipamento::reservar/liberar/iniciarManutencao`; `Material::retirar/reabastecer` | Não há serviço global de recursos. |
| Receita | `ReceitaMedica::adicionarMedicamento`, `emitir`, `gerarTexto` | Receita emitida não deve ser alterada. |
| Procedimento | `executar`, `foiExecutado`, `setResponsavel` | Pode ser associado a `Atendimento`. |
| Relatório | `Relatorio::gerarRelatorioOcupacao`, `gerarRelatorioFilas`, `exportar` | O endpoint deve adaptar o resultado para JSON. |
| Auditoria | `RegistroAuditoria::registrarDecisao`, `consultarHistorico` | Registrar operações confirmadas pelo domínio. |
| Notificação | `ServicoNotificacao::enviarNotificacao`, `chamarPaciente`, `alertarSobrecarga` | Útil após mudanças de fila/setor. |

Qualquer “repository”, roteador HTTP, cadastro global ou método de busca por
ID citado no contrato é **sugestão de infraestrutura**, não uma classe que já
existe. Crie-a somente quando a necessidade estiver clara.

## 5. Como iniciar o servidor

O `Makefile` comprova somente C++17, `g++`, `-Wall -Wextra` e os testes. Não há
dependência HTTP/JSON instalada. Não adicionei uma biblioteca automaticamente:
escolha uma biblioteca compatível com o ambiente da equipe (por exemplo,
uma biblioteca HTTP e `nlohmann/json`) e registre a decisão no README/Makefile.

Uma sequência segura é:

```text
1. escolher e instalar a biblioteca HTTP/JSON;
2. criar um executável de servidor separado do programa interativo;
3. configurar porta 8080, ou alterar api-config.js;
4. criar roteamento para os endpoints do API_CONTRACT.md;
5. desserializar JSON e validar campos;
6. localizar objetos do domínio;
7. chamar os métodos existentes;
8. serializar a resposta padrão;
9. testar com curl antes de abrir o frontend.
```

O frontend pode ser servido por um servidor estático local. Abrir HTML
diretamente por `file://` pode limitar módulos ES e CORS; prefira um servidor
estático durante a integração.

## 6. Padrão de um handler C++ (pseudocódigo)

O código abaixo é deliberadamente adaptador: `request`, `json` e `response`
dependem da biblioteca escolhida e não são classes existentes no repositório.

```cpp
Response cadastrarPaciente(const Request& request) {
    try {
        Json body = parseJson(request.body);
        validarCampos(body["data"], {"nome", "cpf", "idade"});

        // IMPLEMENTAR: criar/localizar a coleção de pacientes.
        // Paciente paciente(nextId(), body["data"]["nome"],
        //                   body["data"]["cpf"], body["data"]["idade"]);

        // IMPLEMENTAR: inserir no agregado escolhido pelo projeto.
        return jsonResponse(201, {
            {"success", true},
            {"message", "Paciente cadastrado com sucesso."},
            {"data", {{"id", /* id atribuído */}}}
        });
    } catch (const ValidationError& error) {
        return jsonResponse(400, errorResponse("VALIDATION_ERROR", error.what()));
    } catch (const std::exception& error) {
        return jsonResponse(500, errorResponse("INTERNAL_ERROR", error.what()));
    }
}
```

Nunca retorne sucesso antes de o método do domínio terminar. Nunca exponha
ponteiros, stack traces ou detalhes internos na resposta pública.

## 7. Sete exemplos de implementação

Os exemplos abaixo mostram o fluxo do handler sem escolher uma biblioteca.
`parseJson`, `jsonResponse`, `Request` e `Response` são nomes didáticos do
adaptador HTTP que você escolher.

### 7.1 Consultar usuários

```text
GET /api/usuarios
-> parse filtros
-> consultar a coleção/repositório de Usuario (extensão sugerida)
-> para cada Usuario, usar getId(), getNome(), getCpf(), getIdade()
-> devolver data.usuarios e status 200
```

O projeto não possui uma coleção global nem método `listarUsuarios`; esse é o
primeiro ponto a criar no servidor, sem alterar `Usuario` além do necessário.

### 7.2 Cadastrar usuário

```text
POST /api/usuarios
{"action":"cadastrar_usuario","data":{"nome":"Maria","cpf":"123","idade":35,"tipo":"MEDICO"}}
-> validar campos e CPF
-> escolher o tipo concreto (ProfissionalSaude, Atendente, Administrador ou Acompanhante)
-> construir o objeto com seu construtor existente
-> inserir na coleção sugerida
-> {"success":true,"message":"...","data":{"id":10}}
```

Não há factory ou armazenamento de usuários no repositório atual; ambos são
infraestrutura sugerida, não métodos já disponíveis.

### 7.3 Editar usuário

```text
POST /api/usuarios
{"action":"editar_usuario","data":{"id":10,"nome":"Maria Souza","idade":36}}
-> localizar Usuario id=10
-> ControleAcesso valida o solicitante
-> chamar setNome() e/ou setIdade()
-> atualizar os campos adicionais somente se a extensão tiver sido definida
-> responder success:true ou 400/404/403
```

O botão é tratado por `usuarios-service.js::editarUsuario`; a decisão de
permissão e a atualização são exclusivamente do backend.

### 7.4 Cadastrar paciente

```text
POST /api/pacientes
{"action":"cadastrar_paciente","data":{"nome":"Ana","cpf":"456","idade":42}}
-> validar dados
-> Paciente paciente(id, nome, cpf, idade)
-> inserir no agregado de pacientes (extensão sugerida)
-> responder com o id criado
```

O construtor existente é `Paciente(int, string, string, int)`. Telefone e
e-mail aparecem no formulário, mas não existem em `Paciente.h`; não os perca
silenciosamente: adicione campos/métodos somente após decidir essa extensão.

### 7.5 Consultar fila

```text
GET /api/filas?setor=3
-> localizar FilaAtendimento do setor
-> obter getFila(), getTamanho()
-> para cada Atendimento usar getId(), getIdPaciente(),
   getPrioridade() e estimarTempoDeFila(atendimento)
-> responder data.fila
```

`FilaAtendimento` já fornece os métodos necessários. A associação setor ->
fila é uma responsabilidade do agregado/servidor, não está exposta em uma
classe de repositório atual.

### 7.6 Chamar o próximo paciente

```text
POST /api/filas/proximo
{"action":"chamar_proximo","data":{"setor_id":3,"atendente_id":7}}
-> localizar Atendente e FilaAtendimento
-> ControleAcesso::podeGerenciarFila(usuario)
-> Atendente::chamarProximo(fila)
-> se houver atendimento, devolver seu id; se nullptr, 409/FILA_VAZIA
-> opcionalmente ServicoNotificacao::chamarPaciente(paciente)
```

Não devolva sucesso para uma fila vazia. O `nullptr` de
`Atendente::chamarProximo` é uma condição de negócio a ser convertida em
resposta HTTP.

### 7.7 Atualizar status de atendimento

```text
PUT /api/atendimentos/30
{"action":"atualizar_status_atendimento","data":{"status":"FINALIZADO"}}
-> localizar Atendimento
-> ControleAcesso::podeRealizarAtendimento(usuario)
-> para início: Atendimento::inicializar()
-> para status intermediário: Atendimento::atualizarStatus(novoStatus)
-> para finalização: verificarPendencias() e depois finalizar()
-> responder o status obtido por getStatus()
```

Se `finalizar()` retornar `false`, responda `409` com
`PENDENCIAS_ATIVAS`. O frontend manterá o modal aberto.

## 8. Como testar

Antes do frontend, teste cada handler com `curl` ou a ferramenta equivalente:

```bash
curl -i http://localhost:8080/api/usuarios
curl -i -X POST http://localhost:8080/api/usuarios ^
  -H "Content-Type: application/json" ^
  -d "{\"action\":\"cadastrar_usuario\",\"data\":{\"nome\":\"Maria Silva\",\"cpf\":\"00000000000\",\"tipo\":\"MEDICO\"}}"
```

No Windows, adapte a quebra de linha ao shell utilizado. Depois abra o
frontend por servidor estático, selecione o perfil e observe a aba Network do
navegador. `200/201 + success:true` fecha o modal; timeout, CORS ou erro HTTP
mantém o modal aberto.

## 9. Diagnóstico

- **Backend indisponível:** confirme processo e porta em `api-config.js`.
- **CORS:** sirva frontend e backend com origem permitida; não remova o
  tratamento de erro do frontend.
- **400:** confira nomes dos campos e validações do contrato.
- **401/403:** confira o usuário/contexto e `ControleAcesso`.
- **404:** compare a rota do service com `API_CONTRACT.md`.
- **500:** veja logs do servidor; não transforme a falha em dados locais.
- **JSON inválido:** corrija serialização do servidor.
- **Modal não fecha:** somente uma resposta com `success:true` pode chamar
  `context.close()`.
