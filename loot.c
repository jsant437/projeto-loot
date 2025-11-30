#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------- STRUCTS --------------------

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// -------------------- VARIÁVEIS GLOBAIS --------------------

#define MAX_ITENS 10
Item mochilaVetor[MAX_ITENS];
int totalVetor = 0;

No* mochilaLista = NULL;

int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// -------------------- FUNÇÕES AUXILIARES --------------------

void trim(char* s) { size_t n = strlen(s); if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0'; }

// -------------------- VERSÃO COM VETOR --------------------

void inserirItemVetor() {
    if (totalVetor >= MAX_ITENS) { printf("Vetor cheio.\n"); return; }
    Item novo;
    printf("Nome: "); fgets(novo.nome, 30, stdin); trim(novo.nome);
    printf("Tipo: "); fgets(novo.tipo, 20, stdin); trim(novo.tipo);
    printf("Quantidade: "); scanf("%d", &novo.quantidade);
    getchar();
    mochilaVetor[totalVetor++] = novo;
    printf("Item inserido no vetor.\n");
}

void removerItemVetor() {
    char nome[30]; printf("Nome para remover: "); fgets(nome, 30, stdin); trim(nome);
    for (int i = 0; i < totalVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            for (int j = i; j < totalVetor - 1; j++) mochilaVetor[j] = mochilaVetor[j + 1];
            totalVetor--; printf("Removido do vetor.\n"); return;
        }
    }
    printf("Item não encontrado.\n");
}

void listarVetor() {
    printf("--- Mochila Vetor ---\n");
    for (int i = 0; i < totalVetor; i++) {
        printf("%s | %s | %d\n", mochilaVetor[i].nome, mochilaVetor[i].tipo, mochilaVetor[i].quantidade);
    }
}

int buscarSequencialVetor(const char* nome) {
    comparacoesSequencial = 0;
    for (int i = 0; i < totalVetor; i++) {
        comparacoesSequencial++;
        if (strcmp(mochilaVetor[i].nome, nome) == 0) return i;
    }
    return -1;
}

void ordenarVetor() {
    for (int i = 0; i < totalVetor - 1; i++) {
        for (int j = 0; j < totalVetor - 1 - i; j++) {
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j + 1].nome) > 0) {
                Item tmp = mochilaVetor[j];
                mochilaVetor[j] = mochilaVetor[j + 1];
                mochilaVetor[j + 1] = tmp;
            }
        }
    }
    printf("Vetor ordenado.\n");
}

int buscarBinariaVetor(const char* nome) {
    comparacoesBinaria = 0;
    int ini = 0, fim = totalVetor - 1;
    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        comparacoesBinaria++;
        int cmp = strcmp(nome, mochilaVetor[meio].nome);
        if (cmp == 0) return meio;
        if (cmp > 0) ini = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

// -------------------- LISTA ENCADEADA --------------------

void inserirItemLista() {
    No* novo = malloc(sizeof(No));
    printf("Nome: "); fgets(novo->dados.nome, 30, stdin); trim(novo->dados.nome);
    printf("Tipo: "); fgets(novo->dados.tipo, 20, stdin); trim(novo->dados.tipo);
    printf("Quantidade: "); scanf("%d", &novo->dados.quantidade);
    getchar();
    novo->proximo = mochilaLista;
    mochilaLista = novo;
    printf("Item inserido na lista.\n");
}

void removerItemLista() {
    char nome[30]; printf("Nome para remover: "); fgets(nome, 30, stdin); trim(nome);

    No* ant = NULL, * p = mochilaLista;
    while (p) {
        if (strcmp(p->dados.nome, nome) == 0) {
            if (ant) ant->proximo = p->proximo;
            else mochilaLista = p->proximo;
            free(p); printf("Removido da lista.\n"); return;
        }
        ant = p; p = p->proximo;
    }
    printf("Item não encontrado.\n");
}

void listarLista() {
    printf("--- Mochila Lista ---\n");
    for (No* p = mochilaLista; p; p = p->proximo) {
        printf("%s | %s | %d\n", p->dados.nome, p->dados.tipo, p->dados.quantidade);
    }
}

int buscarSequencialLista(const char* nome) {
    comparacoesSequencial = 0;
    for (No* p = mochilaLista; p; p = p->proximo) {
        comparacoesSequencial++;
        if (strcmp(p->dados.nome, nome) == 0) return 1;
    }
    return 0;
}

// -------------------- MENU --------------------

void menuVetor() {
    int op; char nome[30];
    do {
        printf("\n--- Vetor ---\n1 Inserir\n2 Remover\n3 Listar\n4 Buscar Seq.\n5 Ordenar\n6 Buscar Binária\n0 Voltar\n> ");
        scanf("%d", &op); getchar();
        if (op == 1) inserirItemVetor();
        else if (op == 2) removerItemVetor();
        else if (op == 3) listarVetor();
        else if (op == 4) {
            printf("Nome: "); fgets(nome, 30, stdin); trim(nome);
            int i = buscarSequencialVetor(nome);
            printf("Comparações: %d | Resultado: %d\n", comparacoesSequencial, i);
        }
        else if (op == 5) ordenarVetor();
        else if (op == 6) {
            printf("Nome: "); fgets(nome, 30, stdin); trim(nome);
            int i = buscarBinariaVetor(nome);
            printf("Comparações: %d | Resultado: %d\n", comparacoesBinaria, i);
        }
    } while (op != 0);
}

void menuLista() {
    int op; char nome[30];
    do {
        printf("\n--- Lista ---\n1 Inserir\n2 Remover\n3 Listar\n4 Buscar Seq.\n0 Voltar\n> ");
        scanf("%d", &op); getchar();
        if (op == 1) inserirItemLista();
        else if (op == 2) removerItemLista();
        else if (op == 3) listarLista();
        else if (op == 4) {
            printf("Nome: "); fgets(nome, 30, stdin); trim(nome);
            int ok = buscarSequencialLista(nome);
            printf("Comparações: %d | Encontrado: %d\n", comparacoesSequencial, ok);
        }
    } while (op != 0);
}

int main() {
    int op;
    do {
        printf("\n=== SISTEMA DE MOCHILA ===\n1 Vetor\n2 Lista Encadeada\n0 Sair\n> ");
        scanf("%d", &op); getchar();
        if (op == 1) menuVetor();
        else if (op == 2) menuLista();
    } while (op != 0);
    return 0;
}
