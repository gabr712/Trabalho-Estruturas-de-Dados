#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX 100

#define COR_RESET    "\033[0m"
#define COR_VERDE    "\033[32m"
#define COR_VERMELHO "\033[31m"
#define COR_AMARELO  "\033[33m"
#define COR_CIANO    "\033[1;36m"

typedef struct {
    int numero;
    char tipo;
    char horario[10];
} Senha;

typedef struct {
    Senha dados[MAX];
    int inicio;
    int quantidade;
} FilaNormal;

typedef struct {
    Senha dados[MAX];
    int inicio;
    int fim;
    int quantidade;
} FilaPreferencial;

int proximaSenha = 1;
int atendidosNormal = 0;
int atendidosPreferencial = 0;

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pegar_horario_atual(char *destino, int tamanho) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    snprintf(destino, tamanho, "%02d:%02d", t->tm_hour, t->tm_min);
}

void inicializar_fila_normal(FilaNormal *f) {
    f->inicio = 0;
    f->quantidade = 0;
}

int normal_vazia(FilaNormal *f) {
    return f->quantidade == 0;
}

int normal_cheia(FilaNormal *f) {
    return (f->inicio + f->quantidade) >= MAX;
}

void enfileirar_normal(FilaNormal *f, Senha s) {
    if (normal_cheia(f)) {
        printf(COR_AMARELO "Erro: a fila normal esta cheia (sem espaco contiguo disponivel).\n" COR_RESET);
        return;
    }
    f->dados[f->inicio + f->quantidade] = s;
    f->quantidade++;
    printf(COR_VERDE "Senha N%03d cadastrada na fila normal.\n" COR_RESET, s.numero);
}

int desenfileirar_normal(FilaNormal *f, Senha *removida) {
    if (normal_vazia(f)) {
        return 0;
    }
    *removida = f->dados[f->inicio];
    f->inicio++;
    f->quantidade--;
    return 1;
}

void listar_normal(FilaNormal *f) {
    if (normal_vazia(f)) {
        printf(COR_AMARELO "\nA fila normal esta vazia.\n" COR_RESET);
        return;
    }
    printf(COR_CIANO "\n--- FILA NORMAL (frente -> fim) ---\n" COR_RESET);
    printf("%-8s %-6s %-10s\n", "Senha", "Tipo", "Horario");
    for (int i = 0; i < f->quantidade; i++) {
        Senha s = f->dados[f->inicio + i];
        printf("N%-7d %-6c %-10s\n", s.numero, s.tipo, s.horario);
    }
    printf("------------------------------------\n");
}

void inicializar_fila_preferencial(FilaPreferencial *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

int preferencial_vazia(FilaPreferencial *f) {
    return f->quantidade == 0;
}

int preferencial_cheia(FilaPreferencial *f) {
    return f->quantidade == MAX;
}

void enfileirar_preferencial(FilaPreferencial *f, Senha s) {
    if (preferencial_cheia(f)) {
        printf(COR_AMARELO "Erro: a fila preferencial esta cheia.\n" COR_RESET);
        return;
    }
    f->dados[f->fim] = s;
    f->fim = (f->fim + 1) % MAX;
    f->quantidade++;
    printf(COR_VERDE "Senha P%03d cadastrada na fila preferencial.\n" COR_RESET, s.numero);
}

int desenfileirar_preferencial(FilaPreferencial *f, Senha *removida) {
    if (preferencial_vazia(f)) {
        return 0;
    }
    *removida = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->quantidade--;
    return 1;
}

void listar_preferencial(FilaPreferencial *f) {
    if (preferencial_vazia(f)) {
        printf(COR_AMARELO "\nA fila preferencial esta vazia.\n" COR_RESET);
        return;
    }
    printf(COR_CIANO "\n--- FILA PREFERENCIAL (frente -> fim) ---\n" COR_RESET);
    printf("%-8s %-6s %-10s\n", "Senha", "Tipo", "Horario");
    for (int i = 0; i < f->quantidade; i++) {
        int idx = (f->inicio + i) % MAX;
        Senha s = f->dados[idx];
        printf("P%-7d %-6c %-10s\n", s.numero, s.tipo, s.horario);
    }
    printf("-------------------------------------------\n");
}

void cadastrar_senha(FilaNormal *fn, FilaPreferencial *fp) {
    char tipo;
    printf("Tipo da senha (N = normal, P = preferencial): ");
    scanf(" %c", &tipo);
    limpar_buffer();

    tipo = (char) toupper((unsigned char) tipo);
    if (tipo != 'N' && tipo != 'P') {
        printf(COR_VERMELHO "Erro: tipo invalido. Use N ou P.\n" COR_RESET);
        return;
    }

    Senha s;
    s.numero = proximaSenha++;
    s.tipo = tipo;
    pegar_horario_atual(s.horario, sizeof(s.horario));

    if (tipo == 'N') {
        enfileirar_normal(fn, s);
    } else {
        enfileirar_preferencial(fp, s);
    }
}

void atender(FilaNormal *fn, FilaPreferencial *fp) {
    static const char ordem[3] = {'N', 'N', 'P'};
    static int posicao = 0;

    if (normal_vazia(fn) && preferencial_vazia(fp)) {
        printf(COR_AMARELO "\nNao ha nenhuma senha aguardando atendimento.\n" COR_RESET);
        return;
    }

    char vez = ordem[posicao];
    posicao = (posicao + 1) % 3;

    Senha atendida;
    int sucesso = 0;
    char filaUsada;

    if (vez == 'N') {
        sucesso = desenfileirar_normal(fn, &atendida);
        filaUsada = 'N';
        if (!sucesso) {
            sucesso = desenfileirar_preferencial(fp, &atendida);
            filaUsada = 'P';
        }
    } else {
        sucesso = desenfileirar_preferencial(fp, &atendida);
        filaUsada = 'P';
        if (!sucesso) {
            sucesso = desenfileirar_normal(fn, &atendida);
            filaUsada = 'N';
        }
    }

    if (sucesso) {
        if (filaUsada == 'N') atendidosNormal++;
        else atendidosPreferencial++;

        printf(COR_VERDE "\n--- SENHA ATENDIDA ---\n" COR_RESET);
        printf("Senha    : %c%03d\n", filaUsada, atendida.numero);
        printf("Tipo     : %s\n", filaUsada == 'N' ? "Normal" : "Preferencial");
        printf("Horario chegada : %s\n", atendida.horario);
    } else {
        printf(COR_AMARELO "\nNao foi possivel atender (ambas as filas vazias).\n" COR_RESET);
    }
}

void relatorio(FilaNormal *fn, FilaPreferencial *fp) {
    int totalAtendidas = atendidosNormal + atendidosPreferencial;
    printf(COR_CIANO "\n========== RELATORIO DO ATENDIMENTO ==========\n" COR_RESET);
    printf("Senhas aguardando (normal)     : %d\n", fn->quantidade);
    printf("Senhas aguardando (preferencial): %d\n", fp->quantidade);
    printf("Total ja atendidas              : %d\n", totalAtendidas);
    printf("  - Normais atendidas           : %d\n", atendidosNormal);
    printf("  - Preferenciais atendidas     : %d\n", atendidosPreferencial);
    printf("Proxima senha a ser gerada      : %03d\n", proximaSenha);
    printf(COR_CIANO "===============================================\n" COR_RESET);
}

void salvar_csv(FilaNormal *fn, FilaPreferencial *fp, const char *arquivo) {
    FILE *fpArq = fopen(arquivo, "w");
    if (fpArq == NULL) {
        printf(COR_VERMELHO "Erro ao abrir arquivo para escrita.\n" COR_RESET);
        return;
    }

    fprintf(fpArq, "numero;tipo;horario\n");

    for (int i = 0; i < fn->quantidade; i++) {
        Senha s = fn->dados[fn->inicio + i];
        fprintf(fpArq, "%d;%c;%s\n", s.numero, s.tipo, s.horario);
    }
    for (int i = 0; i < fp->quantidade; i++) {
        int idx = (fp->inicio + i) % MAX;
        Senha s = fp->dados[idx];
        fprintf(fpArq, "%d;%c;%s\n", s.numero, s.tipo, s.horario);
    }

    fclose(fpArq);
    printf(COR_VERDE "Filas salvas em '%s'.\n" COR_RESET, arquivo);
}

void carregar_csv(FilaNormal *fn, FilaPreferencial *fp, const char *arquivo) {
    FILE *fpArq = fopen(arquivo, "r");
    if (fpArq == NULL) {
        printf(COR_AMARELO "Arquivo '%s' nao encontrado. Iniciando com filas vazias.\n" COR_RESET, arquivo);
        return;
    }

    char linha[100];
    fgets(linha, sizeof(linha), fpArq);

    int carregadas = 0;
    int maiorSenha = 0;

    while (fgets(linha, sizeof(linha), fpArq)) {
        linha[strcspn(linha, "\n")] = '\0';
        linha[strcspn(linha, "\r")] = '\0';

        Senha s;
        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        s.numero = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        s.tipo = token[0];

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(s.horario, token, sizeof(s.horario) - 1);
        s.horario[sizeof(s.horario) - 1] = '\0';

        if (s.tipo == 'N') {
            enfileirar_normal(fn, s);
        } else if (s.tipo == 'P') {
            enfileirar_preferencial(fp, s);
        }

        if (s.numero > maiorSenha) {
            maiorSenha = s.numero;
        }
        carregadas++;
    }

    fclose(fpArq);
    proximaSenha = maiorSenha + 1;
    printf(COR_VERDE "%d senha(s) carregada(s) de '%s'.\n" COR_RESET, carregadas, arquivo);
}

void exibir_menu() {
    printf(COR_CIANO "\n========== ATENDIMENTO BANCARIO ==========\n" COR_RESET);
    printf("1. Cadastrar senha (gerar nova)\n");
    printf("2. Atender proxima senha\n");
    printf("3. Listar fila normal\n");
    printf("4. Listar fila preferencial\n");
    printf("5. Salvar CSV\n");
    printf("6. Carregar CSV\n");
    printf("7. Relatorio / Estatisticas\n");
    printf("0. Sair\n");
    printf(COR_CIANO "===========================================\n" COR_RESET);
    printf("Escolha: ");
}

int main() {
    FilaNormal filaNormal;
    FilaPreferencial filaPreferencial;
    const char *arquivo = "senhas.csv";
    int opcao;

    inicializar_fila_normal(&filaNormal);
    inicializar_fila_preferencial(&filaPreferencial);

    carregar_csv(&filaNormal, &filaPreferencial, arquivo);

    do {
        exibir_menu();
        if (scanf("%d", &opcao) != 1) {
            printf(COR_VERMELHO "Opcao invalida! Digite apenas numeros.\n" COR_RESET);
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1:
                cadastrar_senha(&filaNormal, &filaPreferencial);
                break;

            case 2:
                atender(&filaNormal, &filaPreferencial);
                break;

            case 3:
                listar_normal(&filaNormal);
                break;

            case 4:
                listar_preferencial(&filaPreferencial);
                break;

            case 5:
                salvar_csv(&filaNormal, &filaPreferencial, arquivo);
                break;

            case 6:
                inicializar_fila_normal(&filaNormal);
                inicializar_fila_preferencial(&filaPreferencial);
                carregar_csv(&filaNormal, &filaPreferencial, arquivo);
                break;

            case 7:
                relatorio(&filaNormal, &filaPreferencial);
                break;

            case 0:
                salvar_csv(&filaNormal, &filaPreferencial, arquivo);
                printf("Encerrando...\n");
                break;

            default:
                printf(COR_VERMELHO "Opcao invalida.\n" COR_RESET);
        }

    } while (opcao != 0);

    return 0;
}
