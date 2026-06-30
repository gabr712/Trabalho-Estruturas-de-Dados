#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

#define COR_RESET    "\033[0m"
#define COR_VERDE    "\033[32m"
#define COR_VERMELHO "\033[31m"
#define COR_AMARELO  "\033[33m"
#define COR_CIANO    "\033[1;36m"

typedef struct {
    int protocolo;
    char local[50];
    char tipo[30];
    char horario[20];
} Chamada;

typedef struct {
    Chamada dados[MAX];
    int topo;
} Pilha;

int totalAtendidas = 0;

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inicializar(Pilha *p) {
    p->topo = -1;
}

int vazia(Pilha *p) {
    return p->topo == -1;
}

int cheia(Pilha *p) {
    return p->topo == MAX - 1;
}

int protocolo_existe(Pilha *p, int protocolo) {
    for (int i = 0; i <= p->topo; i++) {
        if (p->dados[i].protocolo == protocolo) return 1;
    }
    return 0;
}

void push(Pilha *p, Chamada c) {
    if (cheia(p)) {
        printf(COR_AMARELO "Erro: a pilha esta cheia (limite de %d chamadas).\n" COR_RESET, MAX);
        return;
    }
    if (protocolo_existe(p, c.protocolo)) {
        printf(COR_VERMELHO "Erro: ja existe uma chamada com o protocolo %d.\n" COR_RESET, c.protocolo);
        return;
    }
    p->topo++;
    p->dados[p->topo] = c;
    printf(COR_VERDE "Chamada %d registrada com sucesso!\n" COR_RESET, c.protocolo);
}

int pop(Pilha *p) {
    if (vazia(p)) {
        printf(COR_AMARELO "Pilha vazia. Nenhuma chamada para atender.\n" COR_RESET);
        return 0;
    }
    Chamada c = p->dados[p->topo];
    p->topo--;
    totalAtendidas++;
    printf(COR_VERDE "\n--- Chamada Atendida (removida da pilha) ---\n" COR_RESET);
    printf("Protocolo : %d\n", c.protocolo);
    printf("Local     : %s\n", c.local);
    printf("Tipo      : %s\n", c.tipo);
    printf("Horario   : %s\n", c.horario);
    return 1;
}

void peek(Pilha *p) {
    if (vazia(p)) {
        printf(COR_AMARELO "Pilha vazia. Nenhuma chamada no topo.\n" COR_RESET);
        return;
    }
    Chamada *c = &p->dados[p->topo];
    printf(COR_CIANO "\n--- Proxima chamada a ser atendida (topo) ---\n" COR_RESET);
    printf("Protocolo : %d\n", c->protocolo);
    printf("Local     : %s\n", c->local);
    printf("Tipo      : %s\n", c->tipo);
    printf("Horario   : %s\n", c->horario);
}

void listar(Pilha *p) {
    if (vazia(p)) {
        printf(COR_AMARELO "Pilha vazia.\n" COR_RESET);
        return;
    }
    printf(COR_CIANO "\n%-10s %-30s %-20s %-10s\n" COR_RESET,
           "Protocolo", "Local", "Tipo", "Horario");
    printf("%-10s %-30s %-20s %-10s\n",
           "----------", "------------------------------",
           "--------------------", "----------");
    for (int i = p->topo; i >= 0; i--) {
        if (i == p->topo)
            printf("[TOPO] ");
        else
            printf("       ");
        printf("%-10d %-30s %-20s %-10s\n",
               p->dados[i].protocolo, p->dados[i].local, p->dados[i].tipo, p->dados[i].horario);
    }
    printf("\n");
}

void relatorio(Pilha *p) {
    printf(COR_CIANO "\n========== RELATORIO DE CHAMADAS ==========\n" COR_RESET);
    printf("Chamadas pendentes na pilha : %d\n", p->topo + 1);
    printf("Chamadas ja atendidas       : %d\n", totalAtendidas);
    if (!vazia(p)) {
        printf("Chamada mais antiga (base)  : protocolo %d, local \"%s\"\n",
               p->dados[0].protocolo, p->dados[0].local);
        printf("Chamada mais recente (topo) : protocolo %d, local \"%s\"\n",
               p->dados[p->topo].protocolo, p->dados[p->topo].local);
    }
    printf(COR_CIANO "============================================\n" COR_RESET);
}

void salvar_csv(Pilha *p, const char *arquivo) {
    FILE *fp = fopen(arquivo, "w");
    if (fp == NULL) {
        printf(COR_VERMELHO "Erro ao abrir arquivo para escrita.\n" COR_RESET);
        return;
    }
    fprintf(fp, "protocolo;local;tipo;horario\n");
    for (int i = 0; i <= p->topo; i++) {
        fprintf(fp, "%d;%s;%s;%s\n",
                p->dados[i].protocolo, p->dados[i].local, p->dados[i].tipo, p->dados[i].horario);
    }
    fclose(fp);
    printf(COR_VERDE "Pilha salva em '%s'.\n" COR_RESET, arquivo);
}

void carregar_csv(Pilha *p, const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (fp == NULL) {
        printf(COR_AMARELO "Arquivo '%s' nao encontrado. Iniciando com pilha vazia.\n" COR_RESET, arquivo);
        return;
    }

    char linha[200];
    fgets(linha, sizeof(linha), fp);

    int carregadas = 0;
    while (fgets(linha, sizeof(linha), fp)) {
        linha[strcspn(linha, "\n")] = '\0';

        Chamada c;
        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        c.protocolo = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(c.local, token, 49);
        c.local[49] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(c.tipo, token, 29);
        c.tipo[29] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(c.horario, token, 19);
        c.horario[19] = '\0';

        if (!protocolo_existe(p, c.protocolo) && !cheia(p)) {
            p->topo++;
            p->dados[p->topo] = c;
            carregadas++;
        }
    }

    fclose(fp);
    printf(COR_VERDE "%d chamada(s) carregada(s) de '%s'.\n" COR_RESET, carregadas, arquivo);
}

void exibir_menu() {
    printf(COR_CIANO "\n====== CHAMADAS DE EMERGENCIA ======\n" COR_RESET);
    printf("1. Registrar chamada (Push)\n");
    printf("2. Atender proxima chamada (Pop)\n");
    printf("3. Ver proxima chamada (Peek)\n");
    printf("4. Listar todas as chamadas\n");
    printf("5. Salvar CSV\n");
    printf("6. Carregar CSV\n");
    printf("7. Relatorio / Estatisticas\n");
    printf("0. Sair\n");
    printf(COR_CIANO "====================================\n" COR_RESET);
    printf("Escolha: ");
}

int main() {
    Pilha pilha;
    const char *arquivo = "chamadas.csv";
    int opcao;

    inicializar(&pilha);
    carregar_csv(&pilha, arquivo);

    do {
        exibir_menu();
        if (scanf("%d", &opcao) != 1) {
            printf(COR_VERMELHO "Opcao invalida! Digite apenas numeros.\n" COR_RESET);
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1: {
                if (cheia(&pilha)) {
                    printf(COR_AMARELO "Erro: pilha cheia.\n" COR_RESET);
                    break;
                }
                Chamada c;
                printf("Protocolo: ");
                scanf("%d", &c.protocolo);
                limpar_buffer();

                if (c.protocolo <= 0) {
                    printf(COR_VERMELHO "Protocolo deve ser maior que zero.\n" COR_RESET);
                    break;
                }

                printf("Local: ");
                fgets(c.local, 50, stdin);
                c.local[strcspn(c.local, "\n")] = '\0';
                if (strlen(c.local) == 0) {
                    printf(COR_VERMELHO "Local nao pode ser vazio.\n" COR_RESET);
                    break;
                }

                printf("Tipo da ocorrencia: ");
                fgets(c.tipo, 30, stdin);
                c.tipo[strcspn(c.tipo, "\n")] = '\0';
                if (strlen(c.tipo) == 0) {
                    printf(COR_VERMELHO "Tipo de ocorrencia nao pode ser vazio.\n" COR_RESET);
                    break;
                }

                printf("Horario (ex: 14:30): ");
                fgets(c.horario, 20, stdin);
                c.horario[strcspn(c.horario, "\n")] = '\0';
                if (strlen(c.horario) == 0) {
                    printf(COR_VERMELHO "Horario nao pode ser vazio.\n" COR_RESET);
                    break;
                }

                push(&pilha, c);
                break;
            }

            case 2:
                pop(&pilha);
                break;

            case 3:
                peek(&pilha);
                break;

            case 4:
                listar(&pilha);
                break;

            case 5:
                salvar_csv(&pilha, arquivo);
                break;

            case 6:
                inicializar(&pilha);
                carregar_csv(&pilha, arquivo);
                break;

            case 7:
                relatorio(&pilha);
                break;

            case 0:
                salvar_csv(&pilha, arquivo);
                printf("Encerrando...\n");
                break;

            default:
                printf(COR_VERMELHO "Opcao invalida.\n" COR_RESET);
        }

    } while (opcao != 0);

    return 0;
}
