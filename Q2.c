// Sophia Garcia e Maria Eduarda Souza

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define MAX_LINE 3001
#define MAX_NAMES_STACK 100
#define MAX_NAME_LEN 50 
#define MAX_OUTPUT_BUFFER_LEN (MAX_NAMES_STACK * (MAX_NAME_LEN + 6 + 1) + MAX_NAMES_STACK * 10) 

typedef struct {
    char nomes[MAX_NAMES_STACK][MAX_NAME_LEN];
    int top;
} Pilha;

void init_pilha(Pilha *p);
int is_empty(Pilha *p);
int is_full(Pilha *p);
void push_interno(Pilha *p, const char *nome);
char* pop_interno(Pilha *p); 

int comparar_nomes(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

void init_pilha(Pilha *p) {
    p->top = -1;
}

int is_empty(Pilha *p) {
    return (p->top == -1);
}

int is_full(Pilha *p) {
    return (p->top == MAX_NAMES_STACK - 1);
}

void push_interno(Pilha *p, const char *nome) {
    if (is_full(p)) {
        return; 
    }
    p->top++;
    strcpy(p->nomes[p->top], nome);
}

char* pop_interno(Pilha *p) {
    if (is_empty(p)) {
        return NULL;
    }
    return p->nomes[p->top--];
}

void add_op_to_buffer(char *buffer, const char *op, int *first_op_ptr) {
    if (!(*first_op_ptr)) {
        strcat(buffer, " ");
    }
    strcat(buffer, op);
    *first_op_ptr = 0;
}

void process_line(char *linha, FILE *fp_out) {
    Pilha p;
    init_pilha(&p);

    char temp_nomes_pop[MAX_NAMES_STACK][MAX_NAME_LEN];
    int temp_pop_count;

    char output_buffer[MAX_OUTPUT_BUFFER_LEN];
    output_buffer[0] = '\0';
    
    int first_op = 1;

    char *token = strtok(linha, " ");
    while (token != NULL) {
        char novo_nome[MAX_NAME_LEN];
        strcpy(novo_nome, token);

        temp_pop_count = 0;

        while (!is_empty(&p) && strcmp(p.nomes[p.top], novo_nome) > 0) {
            strcpy(temp_nomes_pop[temp_pop_count], pop_interno(&p));
            temp_pop_count++;
        }

        if (temp_pop_count > 0) {
            char pop_op_str[20];
            sprintf(pop_op_str, "%dx-pop", temp_pop_count);
            add_op_to_buffer(output_buffer, pop_op_str, &first_op);
        }

        push_interno(&p, novo_nome);
        char push_op_str[MAX_NAME_LEN + 6];
        sprintf(push_op_str, "push-%s", novo_nome);
        add_op_to_buffer(output_buffer, push_op_str, &first_op);
      
        for (int i = temp_pop_count - 1; i >= 0; i--) {
            push_interno(&p, temp_nomes_pop[i]); 
            sprintf(push_op_str, "push-%s", temp_nomes_pop[i]);
            add_op_to_buffer(output_buffer, push_op_str, &first_op);
        }
        
        token = strtok(NULL, " ");
    }

    fprintf(fp_out, "%s\n", output_buffer);
}


int main() {
    FILE *fp_in = fopen("L1Q2.in", "r");
    FILE *fp_out = fopen("L1Q2.out", "w");

    if (!fp_in || !fp_out) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    char linha[MAX_LINE]; 

    while (fgets(linha, sizeof(linha), fp_in)) {
        size_t len = strlen(linha);
        if (len > 0 && linha[len - 1] == '\n') {
            linha[len - 1] = '\0';
        }
        
        process_line(linha, fp_out);
    }

    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
