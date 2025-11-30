#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20

/* Estrutura que representa um componente da torre de resgate */
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

/* ---------- Protótipos das funções (modularização) ---------- */

/* Ordenações: cada função ordena o array in-place e retorna o número de comparações via ponteiro */
long long bubbleSortNome(Componente arr[], int n);        /* ordena por nome (string) */
long long insertionSortTipo(Componente arr[], int n);    /* ordena por tipo (string) */
long long selectionSortPrioridade(Componente arr[], int n); /* ordena por prioridade (int) */

/* Busca binária por nome — só faz sentido se já estiver ordenado por nome */
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long long *comparacoes);

/* Exibição */
void mostrarComponentes(Componente arr[], int n);

/* Medidor de tempo genérico para um algoritmo de ordenação */
double medirTempo(void (*algoritmo)(Componente*, int, long long*), Componente arr[], int n, long long *comparacoes);

/* Função auxiliar para copiar array (evitar reordenar original durante testes) */
void copiarArray(Componente destino[], Componente fonte[], int n);

/* Função para ler string com fgets e remover \n */
void lerString(char *buf, int tamanho);

/* Função para trocar elementos */
void trocar(Componente *a, Componente *b);

/* Função principal: menu e fluxo */
int main(void) {
    Componente componentes[MAX_COMPONENTES];
    int total = 0;

    printf("=== Módulo de Priorização e Montagem da Torre de Fuga ===\n\n");

    /* Entrada de quantos componentes (validar) */
    while (1) {
        char buffer[16];
        printf("Quantos componentes deseja cadastrar? (1-%d): ", MAX_COMPONENTES);
        if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
        total = atoi(buffer);
        if (total >= 1 && total <= MAX_COMPONENTES) break;
        printf("Valor inválido. Tente novamente.\n");
    }

    /* Cadastro dos componentes */
    for (int i = 0; i < total; ++i) {
        printf("\nComponente %d\n", i + 1);

        printf("Nome: ");
        lerString(componentes[i].nome, sizeof(componentes[i].nome));

        printf("Tipo (ex: controle, suporte, propulsao): ");
        lerString(componentes[i].tipo, sizeof(componentes[i].tipo));

        while (1) {
            char buf[16];
            printf("Prioridade (1-10): ");
            if (!fgets(buf, sizeof(buf), stdin)) return 0;
            componentes[i].prioridade = atoi(buf);
            if (componentes[i].prioridade >= 1 && componentes[i].prioridade <= 10) break;
            printf("Prioridade inválida. Tente novamente.\n");
        }
    }

    /* Menu interativo */
    Componente trabalho[MAX_COMPONENTES]; /* cópia para operações */
    int ordenadoPorNome = 0; /* flag para permitir busca binária apenas se ordenado por nome */

    while (1) {
        printf("\n---------- MENU ----------\n");
        printf("1) Mostrar componentes (ordem de cadastro)\n");
        printf("2) Ordenar por NOME (Bubble Sort)\n");
        printf("3) Ordenar por TIPO (Insertion Sort)\n");
        printf("4) Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("5) Busca Binária por NOME (requer ordenação por nome)\n");
        printf("6) Mostrar componentes atuais\n");
        printf("0) Sair\n");
        printf("--------------------------\n");
        printf("Escolha uma opção: ");

        char escolhaBuf[8];
        if (!fgets(escolhaBuf, sizeof(escolhaBuf), stdin)) break;
        int opc = atoi(escolhaBuf);

        if (opc == 0) {
            printf("Saindo...\n");
            break;
        }

        if (opc == 1) {
            printf("\n--- Componentes (cadastro) ---\n");
            mostrarComponentes(componentes, total);
            continue;
        }

        if (opc >= 2 && opc <= 4) {
            /* Prepara cópia para ordenar, para não alterar array original se o usuário quiser comparar */
            copiarArray(trabalho, componentes, total);

            long long comparacoes = 0;
            clock_t inicio, fim;
            double tempoSegundos = 0.0;

            if (opc == 2) {
                /* Bubble sort por nome (string) */
                inicio = clock();
                comparacoes = bubbleSortNome(trabalho, total);
                fim = clock();
                tempoSegundos = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                ordenadoPorNome = 1;
                printf("\nResultado: Bubble Sort por NOME concluído.\n");
            } else if (opc == 3) {
                /* Insertion sort por tipo (string) */
                inicio = clock();
                comparacoes = insertionSortTipo(trabalho, total);
                fim = clock();
                tempoSegundos = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                ordenadoPorNome = 0;
                printf("\nResultado: Insertion Sort por TIPO concluído.\n");
            } else { /* opc == 4 */
                /* Selection sort por prioridade (int) */
                inicio = clock();
                comparacoes = selectionSortPrioridade(trabalho, total);
                fim = clock();
                tempoSegundos = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                ordenadoPorNome = 0;
                printf("\nResultado: Selection Sort por PRIORIDADE concluído.\n");
            }

            /* Exibir métricas e array ordenado */
            printf("Comparações realizadas: %lld\n", comparacoes);
            printf("Tempo (clock): %.6f segundos\n", tempoSegundos);

            printf("\n--- Componentes ordenados ---\n");
            mostrarComponentes(trabalho, total);

            /* Pergunta se deseja sobrescrever o array principal com o ordenado atual */
            printf("\nDeseja aplicar essa ordenação como estado atual do jogo? (s/n): ");
            char yn[8];
            if (!fgets(yn, sizeof(yn), stdin)) break;
            if (yn[0] == 's' || yn[0] == 'S') {
                copiarArray(componentes, trabalho, total);
                printf("Estado atual atualizado.\n");
            } else {
                printf("Estado atual mantido (cadastro original preservado).\n");
            }

            continue;
        }

        if (opc == 5) {
            if (!ordenadoPorNome) {
                printf("Busca binária exige que os componentes estejam ordenados por NOME.\n");
                printf("Use a opção 2 primeiro para ordenar por nome.\n");
                continue;
            }

            char chave[30];
            printf("Digite o nome do componente a buscar (busca binária): ");
            lerString(chave, sizeof(chave));

            long long compsBusca = 0;
            int idx = buscaBinariaPorNome(componentes, total, chave, &compsBusca);

            printf("Comparações feitas pela busca binária: %lld\n", compsBusca);
            if (idx >= 0) {
                printf("Componente encontrado no índice %d:\n", idx);
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                       componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
            } else {
                printf("Componente '%s' não encontrado.\n", chave);
            }

            continue;
        }

        if (opc == 6) {
            printf("\n--- Componentes (estado atual) ---\n");
            mostrarComponentes(componentes, total);
            continue;
        }

        printf("Opção inválida. Tente novamente.\n");
    }

    return 0;
}

/* ================= Implementações das funções ================= */

/* Ler string com fgets e remover newline */
void lerString(char *buf, int tamanho) {
    if (!fgets(buf, tamanho, stdin)) {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\n")] = '\0';
}

/* Mostrar componentes formatados */
void mostrarComponentes(Componente arr[], int n) {
    printf("\nÍndice | Nome                           | Tipo               | Prioridade\n");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        printf("%5d  | %-30s | %-18s | %9d\n",
               i, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
    printf("\n");
}

/* Troca dois componentes */
void trocar(Componente *a, Componente *b) {
    Componente tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Cópia de array de componentes */
void copiarArray(Componente destino[], Componente fonte[], int n) {
    for (int i = 0; i < n; ++i) destino[i] = fonte[i];
}

/* ----------- Bubble Sort por nome (strings) -----------
   Retorna número de comparações realizadas.
   Observação: conta comparações de strings usando strcmp (cada strcmp é 1 comparação lógica aqui). */
long long bubbleSortNome(Componente arr[], int n) {
    long long comps = 0;
    int trocou;
    for (int i = 0; i < n - 1; ++i) {
        trocou = 0;
        for (int j = 0; j < n - 1 - i; ++j) {
            comps++; /* comparação entre arr[j].nome e arr[j+1].nome */
            if (strcmp(arr[j].nome, arr[j + 1].nome) > 0) {
                trocar(&arr[j], &arr[j + 1]);
                trocou = 1;
            }
        }
        if (!trocou) break; /* já está ordenado */
    }
    return comps;
}

/* ----------- Insertion Sort por tipo (strings) -----------
   Retorna número de comparações realizadas (conta as comparações de string). */
long long insertionSortTipo(Componente arr[], int n) {
    long long comps = 0;
    for (int i = 1; i < n; ++i) {
        Componente chave = arr[i];
        int j = i - 1;
        /* enquanto j >= 0 e arr[j].tipo > chave.tipo */
        while (j >= 0) {
            comps++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = chave;
    }
    return comps;
}

/* ----------- Selection Sort por prioridade (int) -----------
   Retorna número de comparações realizadas (conta comparações de prioridade). */
long long selectionSortPrioridade(Componente arr[], int n) {
    long long comps = 0;
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            comps++;
            if (arr[j].prioridade < arr[minIdx].prioridade) {
                minIdx = j;
            }
        }
        if (minIdx != i) trocar(&arr[i], &arr[minIdx]);
    }
    return comps;
}

/* ----------- Busca binária por nome -----------
   Retorna índice encontrado ou -1.
   Também retorna o número de comparações realizadas via ponteiro comparacoes. */
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long long *comparacoes) {
    int left = 0, right = n - 1;
    *comparacoes = 0;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        (*comparacoes)++;
        int cmp = strcmp(chave, arr[mid].nome);
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return -1;
}
