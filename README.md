# Trabalho Final — Estruturas de Dados em C

Repositório do trabalho final da disciplina de **Estruturas de Dados**, curso de **Engenharia da Computação** — **UniJorge**.

---

## 👥 Integrantes

| Nome Completo            | Curso                    |
| ------------------------ | ------------------------ |
| David Silva Souza        | Engenharia da Computação |
| Gabriel Ribeiro          | Engenharia da Computação |
| Gabriel Silva dos Santos | Engenharia da Computação |
| Cauãm dos Santos         | Engenharia da Computação |

**Turma:** Engenharia da Computação

---

## 📚 Temas Escolhidos

| Trabalho   | Tema                            | Estrutura de Dados           |
| ---------- | -------------------------------- | ----------------------------- |
| Trabalho A | **A1 — Playlist de Músicas**     | Lista Simples (vetor)         |
| Trabalho B | **B2 — Chamadas de Emergência**  | Pilha Simples (vetor)         |
| Trabalho C | **C1 — Atendimento Bancário**    | Fila Simples + Fila Circular  |

---

## 📂 Estrutura do Repositório
Trabalho-Estruturas-de-Dados/
├── README.md
├── relatorio.md
├── Trabalho_A/

│   ├── trabalho_a.c
│   └── playlist.csv

├── Trabalho_B/
│   ├── trabalho_b.c
│   └── chamadas.csv

└── Trabalho_C/
├── trabalho_c.c
└── senhas.csv
---

## 📝 Descrição dos Programas

### Trabalho A — Playlist de Músicas (`Trabalho_A/trabalho_a.c`)

Sistema de gerenciamento de playlist musical usando **lista simples com vetor de struct**.

Cada música armazena: ID, título, artista, duração (segundos) e gênero.

Funcionalidades implementadas:

- Inserir música com validação de ID único
- Buscar música por ID
- Editar campos de uma música (mantendo o ID)
- Excluir música por ID com deslocamento do vetor
- Listar todas as músicas em tabela formatada
- Relatório com total de músicas, duração total, média, música mais longa e mais curta
- Salvar e carregar dados em arquivo CSV (`playlist.csv`)

---

### Trabalho B — Chamadas de Emergência (`Trabalho_B/trabalho_b.c`)

Sistema de registro e atendimento de chamadas de emergência usando **pilha simples com vetor de struct**, seguindo a lógica LIFO.

Cada chamada armazena: protocolo, local, tipo da ocorrência e horário.

Funcionalidades implementadas:

- Registrar chamada (push) com validação de protocolo único
- Atender próxima chamada (pop) — última registrada é a primeira atendida
- Consultar chamada no topo (peek) sem remover
- Listar todas as chamadas da pilha com indicação do topo
- Relatório com chamadas pendentes, já atendidas, mais antiga e mais recente
- Salvar e carregar dados em arquivo CSV (`chamadas.csv`)

---

### Trabalho C — Atendimento Bancário (`Trabalho_C/trabalho_c.c`)

Sistema de senhas para atendimento bancário com **duas filas simultâneas**: uma fila linear simples para senhas normais e uma **fila circular** para senhas preferenciais.

Cada senha armazena: número, tipo (N/P) e horário de chegada (capturado automaticamente do sistema).

Funcionalidades implementadas:

- Gerar senha normal (fila simples) ou preferencial (fila circular)
- Atendimento com regra de prioridade: 2 normais para cada 1 preferencial
- Se uma fila estiver vazia, o atendimento continua pela outra
- Listar fila normal e fila preferencial separadamente
- Relatório com senhas aguardando, já atendidas por tipo e próximo número
- Salvar e carregar dados em arquivo CSV (`senhas.csv`)

---

## ⚙️ Como Compilar

> Pré-requisito: GCC instalado. No Windows, use MinGW ou WSL.
Trabalho A — Playlist de Músicas
gcc Trabalho_A/trabalho_a.c -o Trabalho_A/trabalho_a
Trabalho B — Chamadas de Emergência
gcc Trabalho_B/trabalho_b.c -o Trabalho_B/trabalho_b
Trabalho C — Atendimento Bancário
gcc Trabalho_C/trabalho_c.c -o Trabalho_C/trabalho_c
---

## ▶️ Como Executar
Trabalho A
./Trabalho_A/trabalho_a
Trabalho B
./Trabalho_B/trabalho_b
Trabalho C
./Trabalho_C/trabalho_c
Os três programas podem ser avaliados de forma **completamente independente**.

---

## 🗒️ Observações sobre o Funcionamento

- Ao sair de qualquer programa (opção `0`), os dados são **salvos automaticamente** no arquivo CSV correspondente.
- Na próxima execução, os dados são **carregados automaticamente** do CSV.
- Todos os programas usam **cores ANSI** no terminal para melhor legibilidade (verde = sucesso, vermelho = erro, amarelo = aviso, ciano = títulos). Em terminais sem suporte a cores (ex: alguns ambientes Windows), os códigos de cor aparecerão como texto, mas o programa funciona normalmente.
- O programa de Atendimento Bancário captura o **horário real do sistema** no momento de geração de cada senha.

---

## ⚠️ Dificuldades Encontradas

- Implementação da fila circular com controle de `inicio`, `fim` e `quantidade` sem confundir os índices ao percorrer para listagem e salvamento em CSV.
- Gerenciamento do buffer de entrada (`limpar_buffer()`) para evitar leituras erradas ao misturar `scanf` e `fgets` no mesmo programa.
- Garantia de que o carregamento do CSV não insere duplicatas e respeita os limites de tamanho dos campos da struct.
