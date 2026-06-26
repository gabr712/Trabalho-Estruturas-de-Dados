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
    int id;
    char titulo[50];
    char artista[50];
    int duracao;
    char genero[30];
} Musica;

typedef struct {
    Musica dados[MAX];
    int quantidade;
} Playlist;

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inicializar(Playlist *p) {
    p->quantidade = 0;
}

int vazia(Playlist *p) {
    return p->quantidade == 0;
}

int cheia(Playlist *p) {
    return p->quantidade == MAX;
}

int buscar_indice(Playlist *p, int id) {
    for (int i = 0; i < p->quantidade; i++) {
        if (p->dados[i].id == id) return i;
    }
    return -1;
}

void inserir(Playlist *p, Musica m) {
    if (cheia(p)) {
        printf(COR_AMARELO "A playlist esta cheia (limite de %d musicas atingido).\n" COR_RESET, MAX);
        return;
    }
    if (buscar_indice(p, m.id) != -1) {
        printf(COR_VERMELHO "Erro: ja existe uma musica com o ID %d.\n" COR_RESET, m.id);
        return;
    }
    p->dados[p->quantidade] = m;
    p->quantidade++;
    printf(COR_VERDE "Musica \"%s\" adicionada a playlist com sucesso!\n" COR_RESET, m.titulo);
}

void buscar(Playlist *p, int id) {
    int idx = buscar_indice(p, id);
    if (idx == -1) {
        printf(COR_VERMELHO "Musica com ID %d nao encontrada.\n" COR_RESET, id);
        return;
    }
    Musica *m = &p->dados[idx];
    printf(COR_CIANO "\n--- Musica encontrada ---\n" COR_RESET);
    printf("ID      : %d\n", m->id);
    printf("Titulo  : %s\n", m->titulo);
    printf("Artista : %s\n", m->artista);
    printf("Duracao : %d segundos\n", m->duracao);
    printf("Genero  : %s\n", m->genero);
}

void editar(Playlist *p, int id) {
    int idx = buscar_indice(p, id);
    if (idx == -1) {
        printf(COR_VERMELHO "Musica com ID %d nao encontrada.\n" COR_RESET, id);
        return;
    }

    Musica *m = &p->dados[idx];
    char buffer[50];

    printf("Editando musica ID %d (Enter para manter o valor atual):\n", id);

    printf("Titulo atual [%s]: ", m->titulo);
    fgets(buffer, 50, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) strcpy(m->titulo, buffer);

    printf("Artista atual [%s]: ", m->artista);
    fgets(buffer, 50, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) strcpy(m->artista, buffer);

    printf("Duracao atual [%d] segundos: ", m->duracao);
    fgets(buffer, 50, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        int nova = atoi(buffer);
        if (nova > 0) {
            m->duracao = nova;
        } else {
            printf(COR_AMARELO "Valor invalido, duracao mantida em %d segundos.\n" COR_RESET, m->duracao);
        }
    }

    printf("Genero atual [%s]: ", m->genero);
    fgets(buffer, 30, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) strcpy(m->genero, buffer);

    printf(COR_VERDE "Musica atualizada com sucesso!\n" COR_RESET);
}

void excluir(Playlist *p, int id) {
    if (vazia(p)) {
        printf(COR_AMARELO "A playlist esta vazia, nao ha o que excluir.\n" COR_RESET);
        return;
    }
    int idx = buscar_indice(p, id);
    if (idx == -1) {
        printf(COR_VERMELHO "Musica com ID %d nao encontrada.\n" COR_RESET, id);
        return;
    }
    char tituloRemovido[50];
    strcpy(tituloRemovido, p->dados[idx].titulo);
    for (int i = idx; i < p->quantidade - 1; i++) {
        p->dados[i] = p->dados[i + 1];
    }
    p->quantidade--;
    printf(COR_VERDE "Musica \"%s\" (ID %d) removida da playlist.\n" COR_RESET, tituloRemovido, id);
}

void listar(Playlist *p) {
    if (vazia(p)) {
        printf(COR_AMARELO "A playlist esta vazia.\n" COR_RESET);
        return;
    }
    printf(COR_CIANO "\n%-5s %-30s %-25s %-10s %-15s\n" COR_RESET,
           "ID", "Titulo", "Artista", "Duracao(s)", "Genero");
    printf("%-5s %-30s %-25s %-10s %-15s\n",
           "-----", "------------------------------",
           "-------------------------", "----------", "---------------");
    for (int i = 0; i < p->quantidade; i++) {
        Musica *m = &p->dados[i];
        printf("%-5d %-30s %-25s %-10d %-15s\n",
               m->id, m->titulo, m->artista, m->duracao, m->genero);
    }
    printf("\n");
}

void relatorio(Playlist *p) {
    if (vazia(p)) {
        printf(COR_AMARELO "\nNenhuma musica cadastrada ainda. Adicione algumas para ver o relatorio!\n" COR_RESET);
        return;
    }

    int duracaoTotal = 0;
    int maisLonga = p->dados[0].duracao;
    int maisCurta = p->dados[0].duracao;
    char tituloMaisLonga[50];
    char tituloMaisCurta[50];
    strcpy(tituloMaisLonga, p->dados[0].titulo);
    strcpy(tituloMaisCurta, p->dados[0].titulo);

    for (int i = 0; i < p->quantidade; i++) {
        int d = p->dados[i].duracao;
        duracaoTotal += d;
        if (d > maisLonga) {
            maisLonga = d;
            strcpy(tituloMaisLonga, p->dados[i].titulo);
        }
        if (d < maisCurta) {
            maisCurta = d;
            strcpy(tituloMaisCurta, p->dados[i].titulo);
        }
    }

    double media = (double) duracaoTotal / p->quantidade;

    printf(COR_CIANO "\n========== RELATORIO DA PLAYLIST ==========\n" COR_RESET);
    printf("Total de musicas        : %d\n", p->quantidade);
    printf("Duracao total           : %d min %d seg\n", duracaoTotal / 60, duracaoTotal % 60);
    printf("Duracao media por musica: %.1f segundos\n", media);
    printf("Musica mais longa       : %s (%d s)\n", tituloMaisLonga, maisLonga);
    printf("Musica mais curta       : %s (%d s)\n", tituloMaisCurta, maisCurta);
    printf(COR_CIANO "=============================================\n" COR_RESET);
}

void salvar_csv(Playlist *p, const char *arquivo) {
    FILE *fp = fopen(arquivo, "w");
    if (fp == NULL) {
        printf(COR_VERMELHO "Erro ao abrir arquivo para escrita.\n" COR_RESET);
        return;
    }
    fprintf(fp, "id;titulo;artista;duracao;genero\n");
    for (int i = 0; i < p->quantidade; i++) {
        Musica *m = &p->dados[i];
        fprintf(fp, "%d;%s;%s;%d;%s\n", m->id, m->titulo, m->artista, m->duracao, m->genero);
    }
    fclose(fp);
    printf(COR_VERDE "Playlist salva em '%s'.\n" COR_RESET, arquivo);
}

void carregar_csv(Playlist *p, const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (fp == NULL) {
        printf(COR_AMARELO "Arquivo '%s' nao encontrado. Iniciando com playlist vazia.\n" COR_RESET, arquivo);
        return;
    }

    char linha[200];
    fgets(linha, sizeof(linha), fp);

    int carregadas = 0;
    while (fgets(linha, sizeof(linha), fp)) {
        linha[strcspn(linha, "\n")] = '\0';

        Musica m;
        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        m.id = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(m.titulo, token, 49);
        m.titulo[49] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(m.artista, token, 49);
        m.artista[49] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        m.duracao = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(m.genero, token, 29);
        m.genero[29] = '\0';

        if (buscar_indice(p, m.id) == -1 && !cheia(p)) {
            p->dados[p->quantidade] = m;
            p->quantidade++;
            carregadas++;
        }
    }

    fclose(fp);
    printf(COR_VERDE "%d musica(s) carregada(s) de '%s'.\n" COR_RESET, carregadas, arquivo);
}

void exibir_menu() {
    printf(COR_CIANO "\n========== PLAYLIST DE MUSICAS ==========\n" COR_RESET);
    printf("1. Inserir musica\n");
    printf("2. Buscar musica por ID\n");
    printf("3. Editar musica\n");
    printf("4. Excluir musica\n");
    printf("5. Listar todas as musicas\n");
    printf("6. Salvar CSV\n");
    printf("7. Carregar CSV\n");
    printf("8. Relatorio / Estatisticas\n");
    printf("0. Sair\n");
    printf(COR_CIANO "=========================================\n" COR_RESET);
    printf("Escolha: ");
}

int main() {
    Playlist playlist;
    const char *arquivo = "playlist.csv";
    int opcao;

    inicializar(&playlist);
    carregar_csv(&playlist, arquivo);

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
                if (cheia(&playlist)) {
                    printf(COR_AMARELO "Erro: playlist cheia.\n" COR_RESET);
                    break;
                }
                Musica m;
                printf("ID: ");
                scanf("%d", &m.id);
                limpar_buffer();

                if (m.id <= 0) {
                    printf(COR_VERMELHO "ID deve ser maior que zero.\n" COR_RESET);
                    break;
                }

                printf("Titulo: ");
                fgets(m.titulo, 50, stdin);
                m.titulo[strcspn(m.titulo, "\n")] = '\0';
                if (strlen(m.titulo) == 0) {
                    printf(COR_VERMELHO "Titulo nao pode ser vazio.\n" COR_RESET);
                    break;
                }

                printf("Artista: ");
                fgets(m.artista, 50, stdin);
                m.artista[strcspn(m.artista, "\n")] = '\0';
                if (strlen(m.artista) == 0) {
                    printf(COR_VERMELHO "Artista nao pode ser vazio.\n" COR_RESET);
                    break;
                }

                printf("Duracao (segundos): ");
                scanf("%d", &m.duracao);
                limpar_buffer();

                if (m.duracao <= 0) {
                    printf(COR_VERMELHO "Duracao deve ser maior que zero.\n" COR_RESET);
                    break;
                }

                printf("Genero: ");
                fgets(m.genero, 30, stdin);
                m.genero[strcspn(m.genero, "\n")] = '\0';
                if (strlen(m.genero) == 0) {
                    printf(COR_VERMELHO "Genero nao pode ser vazio.\n" COR_RESET);
                    break;
                }

                inserir(&playlist, m);
                break;
            }

            case 2: {
                int id;
                printf("ID para buscar: ");
                scanf("%d", &id);
                limpar_buffer();
                buscar(&playlist, id);
                break;
            }

            case 3: {
                int id;
                printf("ID da musica para editar: ");
                scanf("%d", &id);
                limpar_buffer();
                editar(&playlist, id);
                break;
            }

            case 4: {
                int id;
                printf("ID da musica para excluir: ");
                scanf("%d", &id);
                limpar_buffer();
                excluir(&playlist, id);
                break;
            }

            case 5:
                listar(&playlist);
                break;

            case 6:
                salvar_csv(&playlist, arquivo);
                break;

            case 7:
                carregar_csv(&playlist, arquivo);
                break;

            case 8:
                relatorio(&playlist);
                break;

            case 0:
                salvar_csv(&playlist, arquivo);
                printf("Encerrando...\n");
                break;

            default:
                printf(COR_VERMELHO "Opcao invalida.\n" COR_RESET);
        }

    } while (opcao != 0);

    return 0;
}
