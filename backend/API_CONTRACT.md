# Contrato da API HTTP do VidaCare

Este contrato descreve o que o frontend já chama e o que ainda deve ser
implementado no servidor C++. As classes e métodos indicados como existentes
foram conferidos em `include/`. Quando não há método equivalente, o texto diz
explicitamente **extensão sugerida**.

## Regras gerais

- Base URL: `http://localhost:8080`, alterável em
  `frontend/js/api/api-config.js`.
- JSON UTF-8 em requisições com corpo.
- Sucesso somente com status HTTP 2xx **e** `"success": true`.
- O frontend trata timeout, erro de conexão e JSON inválido como
  `BACKEND_UNAVAILABLE`/erro de API.
- Não há autenticação HTTP implementada ainda; a seleção de perfil é
  demonstrativa.
- `id` deve ser o identificador persistente do domínio, não o nome exibido.

### Envelope de resposta

```json
{
  "success": true,
  "message": "Operação concluída.",
  "data": {}
}
```

```json
{
  "success": false,
  "message": "CPF já cadastrado.",
  "code": "VALIDATION_ERROR",
  "data": null
}
```

Use `400` para validação, `401/403` para acesso, `404` para inexistente,
`409` para conflito e `500` para falha interna.

## 1. Usuários

### Consultar usuários

- **Frontend:** preparado em `usuarios-service.js::listarUsuarios`.
- **Endpoint:** `GET /api/usuarios`
- **Recebe:** filtros opcionais, por exemplo `?tipo=MEDICO`.
- **Responde:**

```json
{"success":true,"message":"Usuários consultados.","data":{"usuarios":[{"id":1,"nome":"Maria Silva","cpf":"00000000000","tipo":"MEDICO","status":"ATIVO"}]}}
```

- **C++ existente:** `Usuario` fornece getters; `ControleAcesso` valida
  algumas permissões.
- **Extensão sugerida:** coleção/repositório de `Usuario*` e busca por ID.

### Cadastrar usuário

- **Frontend:** `usuarios-service.js::cadastrarUsuario`.
- **Endpoint:** `POST /api/usuarios`
- **JSON:**

```json
{"action":"cadastrar_usuario","data":{"nome":"Maria Silva","cpf":"00000000000","idade":35,"tipo":"MEDICO","email":"maria@hospital.local"}}
```

- **Resposta:**

```json
{"success":true,"message":"Usuário cadastrado com sucesso.","data":{"id":10}}
```

- **C++ existente:** construtor de `Usuario` é base; `ProfissionalSaude`,
  `Atendente`, `Administrador` e `Acompanhante` são tipos concretos.
- **Extensão sugerida:** factory/coleção para escolher o subtipo e persistir.

### Editar, excluir e alterar estado

- **Frontend:** `editarUsuario`, `excluirUsuario`,
  `alterarStatusUsuario` em `usuarios-service.js`.
- **Endpoints usados atualmente pelo frontend:** `POST /api/usuarios` para
  edição/status via `action`, e `DELETE /api/usuarios/{id}` para exclusão.
  `PUT /api/usuarios/{id}` e `/status` são uma possível normalização REST,
  mas exigem alteração coordenada no service e no contrato.
- **Exemplo PUT:**

```json
{"action":"editar_usuario","data":{"nome":"Maria Souza","tipo":"MEDICO","email":"maria@hospital.local"}}
```

- **Exemplo status:**

```json
{"action":"alterar_status_usuario","data":{"status":"INATIVO"}}
```

- **C++ existente:** `Usuario::setNome`, `Usuario::setIdade`.
- **Extensão sugerida:** setters/registro necessários para CPF, tipo, e-mail,
  status e exclusão; não invente esses métodos dentro das classes atuais sem
  decidir primeiro o modelo de dados.

## 2. Pacientes

### Consultar

- `GET /api/pacientes` — `pacientes-service.js::listarPacientes`.
- Resposta: `data.pacientes` com `id`, `nome`, `cpf`, `idade`, prioridade e
  status atual.
- C++: `Paciente::getNome`, `getCpf`, `getIdade`, `getPrioridade`,
  `consultarStatusAtendimento`.
- Coleção global e filtros por CPF são extensão de infraestrutura.

### Cadastrar

- `POST /api/pacientes` — `cadastrarPaciente`.
- JSON:

```json
{"action":"cadastrar_paciente","data":{"nome":"Ana Costa","cpf":"45678912300","idade":42,"telefone":"31999990000","email":"ana@local"}}
```

- Resposta:

```json
{"success":true,"message":"Paciente cadastrado.","data":{"id":21}}
```

- C++: `Paciente(int id, const std::string&, const std::string&, int)`.
- Espaço reservado: inserir o objeto no agregado responsável.

### Editar/excluir

- `POST /api/pacientes` com `"action":"editar_paciente"` — `editarPaciente`.
  O formato mantém o padrão de ações usado pelo service atual; uma rota
  `PUT /api/pacientes/{id}` é uma possível extensão REST.
- `DELETE /api/pacientes/{id}` — `excluirPaciente`.
- C++ existente: `Usuario::setNome`, `setIdade`; não há setter de CPF,
  telefone/e-mail nem método de exclusão. Esses campos/ações são extensões
  sugeridas e devem respeitar atendimentos históricos.

## 3. Atendimentos

### Consultar e atualizar status

- `GET /api/atendimentos` — `atendimentos-service.js::listarAtendimentos`.
- `PUT /api/atendimentos/{id}` — `atualizarStatusAtendimento`.
- JSON:

```json
{"action":"atualizar_status_atendimento","data":{"status":"EM_ATENDIMENTO","observacao":"Triagem iniciada"}}
```

- Resposta: `data.atendimento` com status atualizado.
- C++: `Atendimento::inicializar`, `atualizarStatus`, `finalizar`,
  `verificarPendencias`; `ProfissionalSaude::realizarAtendimento`.
- O handler deve rejeitar finalização quando `Atendimento::finalizar()`
  retornar `false`.

## 4. Filas

### Consultar fila

- `GET /api/filas?setor=clinica-medica` —
  `filas-service.js::consultarFila`.
- Resposta: `data.fila`, posição e tempo estimado.
- C++: `FilaAtendimento::getFila`, `getTamanho`,
  `estimarTempoDeFila`, `visualizarFila`.

### Chamar próximo

- `POST /api/filas/proximo` — `filas-service.js::chamarProximo`.
- JSON:

```json
{"action":"chamar_proximo","data":{"setor_id":3,"atendente_id":7}}
```

- C++: `Atendente::chamarProximo`, `FilaAtendimento::chamarProximo`.
- Resposta: atendimento chamado ou erro de fila vazia.

### Inserir paciente

- `POST /api/filas` — extensão de service/rota ainda não exposta pelo
  frontend atual.
- C++: `Atendente::encaminharParaFila`,
  `FilaAtendimento::adicionarAtendimento`.

## 5. Setores

- `GET /api/setores` — consultar setores; C++ `Hospital::buscarSetor`,
  `Setor::getId/getNome/getDescricao/getCapacidade`.
- `GET /api/setores/ocupacao` — `Setor::calcularLotacao`.
- `POST /api/setores` e `DELETE /api/setores/{id}` — operações administrativas
  futuras; C++ `Administrador::adicionarSetor` e
  `Administrador::removerSetor`/`Hospital::adicionarSetor`.
- Resposta de ocupação:

```json
{"success":true,"message":"Ocupação consultada.","data":{"setor_id":3,"ocupacao":76,"capacidade":40}}
```

## 6. Leitos

- `GET /api/leitos` — `leitos-service.js::listarLeitos`; C++ usa
  `Setor::consultarLeitosDisponiveis` e `Hospital/Setor::buscarLeito`.
- `POST /api/leitos/alocacoes` — `alocarPaciente`.
- JSON:

```json
{"action":"alocar_paciente","data":{"leito_id":102,"paciente_id":21}}
```

- C++: `Leito::alocarPaciente`; validar antes com
  `Leito::verificarDisponibilidade`.
- `PUT /api/leitos/{id}` — `alterarStatusLeito`.
- JSON: `{"action":"alterar_status_leito","data":{"status":"BLOQUEADO"}}`.
- C++: `Leito::bloquear` ou `Leito::liberar`.

## 7. Recursos hospitalares

- `GET /api/recursos` — `recursos-service.js::listarRecursos`; C++
  `Setor::consultarRecursos`.
- `PUT /api/recursos/{id}` — `atualizarDisponibilidade`.
- Equipamento: `verificarDisponibilidade`, `reservar`, `liberar`,
  `iniciarManutencao`, `finalizarManutencao`.
- Material: `retirar(double)` e `reabastecer(double)`.
- Resposta deve incluir o estado após o método, nunca apenas “requisição
  recebida”.

## 8. Prescrições

- `GET /api/prescricoes?paciente_id=21` — `consultarPrescricoes`.
- `POST /api/prescricoes` — `prescricoes-service.js::emitirPrescricao`.
- JSON:

```json
{"action":"emitir_prescricao","data":{"paciente_id":21,"medico_id":8,"itens":[{"medicamento_id":4,"dosagem":"750 mg","quantidade":10,"modo_uso":"1 a cada 8 horas"}]}}
```

- C++: `ReceitaMedica::adicionarMedicamento`, `emitir`,
  `gerarTexto`; `Paciente::adicionarReceita`.
- Após `emitir()`, respeite `foiEmitida()` e não permita edição.

## 9. Procedimentos

- `GET /api/procedimentos` — `listarProcedimentos`.
- `POST /api/procedimentos` — `registrarProcedimento`.
- `PUT /api/procedimentos/{id}` — `confirmarExecucao`.
- JSON:

```json
{"action":"registrar_procedimento","data":{"atendimento_id":30,"procedimento_id":5,"observacao":"Executado sem intercorrências"}}
```

- C++: `Atendimento::adicionarProcedimento`,
  `Procedimento::executar`, `foiExecutado`, `setResponsavel`.

## 10. Relatórios

- `GET /api/relatorios?tipo=ocupacao` — `consultarRelatorio`.
- `GET /api/relatorios?tipo=filas` — relatório de filas.
- `POST /api/relatorios/exportacoes` — `exportarRelatorio`.
- JSON:

```json
{"action":"exportar_relatorio","data":{"tipo":"ocupacao","formato":"json"}}
```

- C++: `Relatorio::gerarRelatorioOcupacao`,
  `gerarRelatorioFilas`, `gerar`, `exportar`.
- O frontend ainda não depende de um arquivo exportado; o backend deve
  devolver metadados ou conteúdo conforme decisão futura.

## 11. Simulações

- `GET /api/relatorios?tipo=simulacao` é a chamada provisória usada pelo
  frontend para consultar o resultado.
- `POST /api/simulacoes` é a **extensão recomendada** para receber cenário,
  quantidade e disponibilidade sem misturar consulta de relatório com
  alteração de estado.
- C++ relacionado: `GerenciadorFluxo::monitorarSobrecargas`,
  `detectarGargalos`, `recomendarProximoSetor` e `setLimitesFila`.
- Não altere objetos reais em uma simulação sem definir cópia/rollback.

## 12. Auditoria

- `GET /api/auditoria` é a extensão de service ainda não usada pela tela.
- `POST /api/auditoria` pode ser interno ao servidor, não necessariamente
  exposto ao navegador.
- C++: `RegistroAuditoria::registrarDecisao`,
  `consultarHistorico`, `getUltimoTimestamp`.
- Registre operações realmente concluídas, não tentativas que falharam.

## 13. Notificações e acompanhante

Estas telas ainda exibem dados locais. Quando forem integradas:

- `GET /api/pacientes/{id}/acompanhamento`;
- `GET /api/notificacoes?paciente_id={id}`.

Métodos C++ relacionados: `Paciente::acompanharAtendimento`,
`consultarStatusAtendimento`, `visualizarEquipeResponsavel`,
`Acompanhante::getPaciente` e `ServicoNotificacao`.

Essas rotas são extensões sugeridas, pois não existem nos services atuais.
