# Preparação do backend C++

O diretório `backend/` contém somente a documentação da futura API HTTP do
Sistema de Gestão Hospitalar. A implementação do servidor, regras de negócio,
persistência e testes de integração serão responsabilidade da etapa posterior
em C++.

O frontend usa `http://localhost:8080` por padrão, configurado em
`frontend/js/api/api-config.js`. A camada de serviços já separa as operações por
domínio e aguarda confirmação do servidor antes de fechar os modais.

Consulte [API_CONTRACT.md](API_CONTRACT.md) para os endpoints e formatos JSON.
