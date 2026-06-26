# Relatório do Trabalho Final — Estruturas de Dados

**Disciplina:** Estruturas de Dados  
**Curso:** Engenharia da Computação  
**Instituição:** UniJorge  
**Integrantes:** David Silva Souza, Gabriel Ribeiro, Gabriel Silva dos Santos, Cauãm dos Santos

---

## Trabalho A — Playlist de Músicas

**Estrutura utilizada:** Lista simples com vetor de struct (`Playlist.dados[MAX]`) e variável de controle `quantidade`.

A lista segue a ordem de cadastro. A busca, edição e exclusão são feitas por ID. Na exclusão, os elementos à direita são deslocados para manter a continuidade do vetor. O ID é validado como único no momento da inserção.

**Extras implementados:**
- Cores ANSI no terminal
- Relatório com duração total, média, música mais longa e mais curta

---

## Trabalho B — Chamadas de Emergência

**Estrutura utilizada:** Pilha simples com vetor de struct (`Pilha.dados[MAX]`) e variável de controle `topo`.

A lógica LIFO é respeitada: a última chamada registrada é a primeira atendida. O protocolo é validado como único. O peek permite consultar o topo sem remover. Um contador global `totalAtendidas` registra quantas chamadas foram atendidas na sessão.

**Extras implementados:**
- Cores ANSI no terminal
- Relatório com pendentes, atendidas, chamada mais antiga (base) e mais recente (topo)

---

## Trabalho C — Atendimento Bancário

**Estruturas utilizadas:**
- `FilaNormal`: fila simples com vetor, `inicio` e `quantidade`
- `FilaPreferencial`: fila circular com vetor, `inicio`, `fim` e `quantidade`

A regra de atendimento usa um array de 3 posições `{'N','N','P'}` com índice rotativo, garantindo a proporção 2:1. Se a fila chamada estiver vazia, o atendimento cai automaticamente para a outra. O horário de cada senha é capturado do sistema no momento da geração. O carregamento do CSV restaura a variável `proximaSenha` com base no maior número encontrado.

**Extras implementados:**
- Cores ANSI no terminal
- Horário real do sistema por senha
- Relatório com senhas aguardando por tipo e total atendido por tipo

---

## Considerações Gerais

Os três programas atendem integralmente aos requisitos obrigatórios definidos no enunciado: uso de struct, vetores de struct, menu interativo, CRUD completo (onde aplicável), tratamento de estrutura vazia e cheia, validação de entradas, persistência em CSV e carregamento automático.

Como extras, todos os programas incluem cores ANSI e relatório de estatísticas, funcionalidades que somam pontos extras conforme o enunciado.
