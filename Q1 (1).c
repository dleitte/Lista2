// Sophia Garcia e Maria Eduarda Souza

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 3001
#define MAX_LISTAS 100
#define MAX_ELEMENTOS 100

typedef struct {
    int elementos[MAX_ELEMENTOS];
    int quantidade;
    int soma;
    int posicao;
} Lista;

int comparar_elementos(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int comparar_soma(const void *a, const void *b) {
    const Lista *la = (const Lista *)a;
    const Lista *lb = (const Lista *)b;
    if (la->soma != lb->soma)
        return lb->soma - la->soma; 
    return lb->posicao - la->posicao; 
}

void process_line(char *linha, FILE *fp_out, int linha_index) {
    Lista listas[MAX_LISTAS];
    int total_listas = 0;
    int ordem = 0;

    char *token = strtok(linha, " ");
    while (token) {
        if (strcmp(token, "start") == 0) {
            Lista l;
            l.quantidade = 0;
            l.soma = 0;
            l.posicao = ordem++;

            token = strtok(NULL, " ");
            while (token && strcmp(token, "start") != 0) {
                int num;
                sscanf(token, "%d", &num);
                l.elementos[l.quantidade++] = num;
                l.soma += num;
                token = strtok(NULL, " ");
            }

            qsort(l.elementos, l.quantidade, sizeof(int), comparar_elementos);
            listas[total_listas++] = l;
        } else {
            token = strtok(NULL, " ");
        }
    }

    int manter[MAX_LISTAS] = {0};
    for (int i = total_listas - 1; i >= 0; i--) {
        int j, duplicado = 0;
        for (j = i + 1; j < total_listas; j++) {
            if (listas[i].soma == listas[j].soma) {
                duplicado = 1;
                break;
            }
        }
        if (!duplicado)
            manter[i] = 1;
    }

    Lista finais[MAX_LISTAS];
    int total_finais = 0;
    for (int i = 0; i < total_listas; i++) {
        if (manter[i])
            finais[total_finais++] = listas[i];
    }

    qsort(finais, total_finais, sizeof(Lista), comparar_soma);

    for (int i = 0; i < total_finais; i++) {
        if (i > 0) fprintf(fp_out, " ");
        fprintf(fp_out, "start");
        for (int j = 0; j < finais[i].quantidade; j++) {
            fprintf(fp_out, " %d", finais[i].elementos[j]);
        }
    }
    fprintf(fp_out, "\n");
}

int main() {
    FILE *fp_in = fopen("L1Q1.in", "r");
    FILE *fp_out = fopen("L1Q1.out", "w");

    if (!fp_in || !fp_out) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    char linha[MAX_LINE];
    int linha_index = 0;

    while (fgets(linha, sizeof(linha), fp_in)) {
        size_t len = strlen(linha);
        if (len > 0 && linha[len - 1] == '\n') {
            linha[len - 1] = '\0';
        }

        process_line(linha, fp_out, linha_index++);
    }

    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
