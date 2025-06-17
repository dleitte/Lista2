// Sophia Garcia e Maria Eduarda Souza

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 4000

typedef struct ListaInterna {
    float valor;
    struct ListaInterna* prox;
} ListaInterna;

typedef struct ListaExterna {
    int chave;
    ListaInterna* sublista;
    struct ListaExterna* ant;
    struct ListaExterna* prox;
} ListaExterna;

ListaExterna* novoNoLE(int chave) {
    ListaExterna* no = (ListaExterna*)malloc(sizeof(ListaExterna));
    no->chave = chave;
    no->sublista = NULL;
    no->ant = NULL;
    no->prox = NULL;
    return no;
}

ListaInterna* novoNoLI(float valor) {
    ListaInterna* no = (ListaInterna*)malloc(sizeof(ListaInterna));
    no->valor = valor;
    no->prox = NULL;
    return no;
}

void inserirOrdenadoLE(ListaExterna** inicio, int chave) {
    ListaExterna* novo = novoNoLE(chave);
    if (*inicio == NULL || chave < (*inicio)->chave) {
        novo->prox = *inicio;
        if (*inicio) (*inicio)->ant = novo;
        *inicio = novo;
        return;
    }

    ListaExterna* atual = *inicio;
    while (atual->prox && atual->prox->chave < chave)
        atual = atual->prox;

    novo->prox = atual->prox;
    if (atual->prox) atual->prox->ant = novo;
    atual->prox = novo;
    novo->ant = atual;
}

void inserirLI(ListaInterna** inicio, float valor) {
    ListaInterna* novo = novoNoLI(valor);
    if (!*inicio) {
        *inicio = novo;
        novo->prox = novo;
        return;
    }

    ListaInterna* atual = *inicio;
    while (atual->prox != *inicio)
        atual = atual->prox;
    atual->prox = novo;
    novo->prox = *inicio;
}

void associar(ListaExterna* le, ListaInterna* li) {
    if (!li) return;
    ListaInterna* ini = li;
    do {
        ListaExterna* atualLE = le;
        while (atualLE) {
            if (fabs(li->valor - atualLE->chave) < 1.0) {
                ListaInterna* novo = novoNoLI(li->valor);
                novo->prox = atualLE->sublista;
                atualLE->sublista = novo;
                break;
            }
            atualLE = atualLE->prox;
        }
        li = li->prox;
    } while (li != ini);
}

void imprimir(ListaExterna* le, FILE* fp_out) {
    fprintf(fp_out, "[");
    ListaExterna* atual = le;
    while (atual) {
        fprintf(fp_out, "%d(", atual->chave);

        ListaInterna* sub = atual->sublista;
        ListaInterna* ordenada = NULL;
        while (sub) {
            ListaInterna* no = novoNoLI(sub->valor);
            if (!ordenada || no->valor > ordenada->valor) {
                no->prox = ordenada;
                ordenada = no;
            } else {
                ListaInterna* temp = ordenada;
                while (temp->prox && temp->prox->valor > no->valor)
                    temp = temp->prox;
                no->prox = temp->prox;
                temp->prox = no;
            }
            sub = sub->prox;
        }

        ListaInterna* ptr = ordenada;
        while (ptr) {
            fprintf(fp_out, "%.2f", ptr->valor);
            if (ptr->prox) fprintf(fp_out, "−>");
            ptr = ptr->prox;
        }
        fprintf(fp_out, ")");

        if (atual->prox) fprintf(fp_out, "->");
        atual = atual->prox;

        while (ordenada) {
            ListaInterna* temp = ordenada;
            ordenada = ordenada->prox;
            free(temp);
        }
    }
    fprintf(fp_out, "]\n");
}

void liberarTudo(ListaExterna* le) {
    while (le) {
        ListaExterna* tempLE = le;
        le = le->prox;

        ListaInterna* li = tempLE->sublista;
        while (li) {
            ListaInterna* tempLI = li;
            li = li->prox;
            free(tempLI);
        }
        free(tempLE);
    }
}

int main() {
    FILE* fp_in = fopen("L1Q3.in", "r");
    FILE* fp_out = fopen("L1Q3.out", "w");

    if (!fp_in || !fp_out) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    char linha[MAX_LINE];

    while (fgets(linha, sizeof(linha), fp_in)) {
        ListaExterna* le = NULL;
        ListaInterna* li = NULL;

        char* token = strtok(linha, " \n");
        int modo = 0;

        while (token) {
            if (strcmp(token, "LE") == 0) {
                modo = 0;
            } else if (strcmp(token, "LI") == 0) {
                modo = 1;
            } else {
                if (modo == 0) {
                    inserirOrdenadoLE(&le, atoi(token));
                } else {
                    inserirLI(&li, atof(token));
                }
            }
            token = strtok(NULL, " \n");
        }

        associar(le, li);
        imprimir(le, fp_out);
        liberarTudo(le);
    }

    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
