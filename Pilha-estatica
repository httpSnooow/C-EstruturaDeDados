#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX 100

struct aluno {
    int matricula;
    char nome[30];
    float n1, n2, n3;
};

typedef struct pilha {
    int qtd;
    struct aluno dados[MAX];
} Pilha;

// Cria uma pilha vazia
// Complexidade: O(1)
// Motivo: Aloca memória e inicializa um inteiro.
Pilha* cria_Pilha() {
    Pilha* pi = (Pilha*) malloc(sizeof(Pilha));
    if (pi != NULL)
        pi->qtd = 0;
    return pi;
}

// Libera a memória alocada para a pilha
// Complexidade: O(1)
// Motivo: Apenas chama free.
void libera_Pilha(Pilha* pi) {
    free(pi);
}

// Retorna o número de elementos na pilha
// Complexidade: O(1)
// Motivo: Acesso direto ao campo qtd.
int tamanho_Pilha(Pilha* pi) {
    if (pi == NULL)
        return -1;
    return pi->qtd;
}

// Verifica se a pilha está cheia
// Complexidade: O(1)
// Motivo: Compara qtd com MAX.
int Pilha_cheia(Pilha* pi) {
    if (pi == NULL)
        return -1;
    return (pi->qtd == MAX);
}

// Verifica se a pilha está vazia
// Complexidade: O(1)
// Motivo: Compara qtd com 0.
int Pilha_vazia(Pilha* pi) {
    if (pi == NULL)
        return -1;
    return (pi->qtd == 0);
}

// Insere um aluno no topo da pilha
// Complexidade: O(1)
// Motivo: Inserção direta no vetor e incremento do contador.
int insere_Pilha(Pilha* pi, struct aluno al) {
    if (pi == NULL || pi->qtd == MAX)
        return 0;
    pi->dados[pi->qtd] = al;
    pi->qtd++;
    return 1;
}

// Remove o aluno do topo da pilha
// Complexidade: O(1)
// Motivo: Apenas diminui o contador.
int remove_Pilha(Pilha* pi) {
    if (pi == NULL || pi->qtd == 0)
        return 0;
    pi->qtd--;
    return 1;
}

// Consulta o aluno no topo da pilha
// Complexidade: O(1)
// Motivo: Acesso direto ao último elemento.
int acessa_topo_Pilha(Pilha* pi, struct aluno *al) {
    if (pi == NULL || pi->qtd == 0)
        return 0;
    *al = pi->dados[pi->qtd - 1];
    return 1;
}

// Imprime todos os elementos da pilha do topo até o fundo
// Complexidade: O(n)
// Motivo: É necessário percorrer todos os n elementos da pilha com um laço for.

void imprime_Pilha(Pilha* pi) {
    if (pi == NULL || pi->qtd == 0) {
        printf("Pilha vazia.\n");
        return;
    }

    printf("Pilha (tamanho = %d):\n", pi->qtd);
    for (int i = pi->qtd - 1; i >= 0; i--) {
        struct aluno *a = &pi->dados[i];
        printf("  [%d] Mat: %d  Nome: %-10s  Notas: %.1f, %.1f, %.1f\n",
               i + 1, a->matricula, a->nome, a->n1, a->n2, a->n3);
    }
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    Pilha *pi = cria_Pilha();
    printf("Pilha criada.\nPilha vazia? %s\n\n",
           Pilha_vazia(pi) == 1 ? "Sim" : "Não");

    struct aluno a1 = {101, "Guilherme", 8.5, 7.0, 9.0};
    struct aluno a2 = {103, "João",      6.0, 8.0, 7.5};
    struct aluno a3 = {102, "Pedro",     9.0, 9.5, 8.0};

    printf("Inserindo Guilherme, João e Pedro:\n");
    insere_Pilha(pi, a1);
    insere_Pilha(pi, a2);
    insere_Pilha(pi, a3);
    imprime_Pilha(pi);

    printf("Tamanho da pilha: %d   \nPilha cheia? %s\n\n",
           tamanho_Pilha(pi),
           Pilha_cheia(pi) == 1 ? "Sim" : "Não");

    struct aluno topo;
    if (acessa_topo_Pilha(pi, &topo)) {
        printf("Aluno no topo da pilha: %s (mat: %d)\n\n",
               topo.nome, topo.matricula);
    }

    printf("Removendo um elemento do topo da pilha.\n");
    remove_Pilha(pi);
    imprime_Pilha(pi);

    printf("Liberando pilha...\n");
    libera_Pilha(pi);
    printf("Pilha liberada.\n");

    return 0;
}
