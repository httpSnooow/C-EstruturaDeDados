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

typedef struct fila {
    int inicio, final, qtd;
    struct aluno dados[MAX];
} Fila;

// Cria uma fila vazia, aloca na memória e inicializa inicio=0, final=0, qtd=0
// Complexidade: O(1)
// Motivo: Apenas aloca memória e inicializa três inteiros, sem loops.
Fila* cria_Fila() {
    Fila *fi = (Fila*) malloc(sizeof(Fila)); 
    if (fi != NULL) {
        fi->inicio = 0;
        fi->final  = 0;
        fi->qtd    = 0;
    }
    return fi;
}

// Libera a memória alocada para a fila
// Complexidade: O(1)
// Motivo: Chama free() sem percorrer elementos.
void libera_Fila(Fila* fi) {
    free(fi);
}

// Retorna o número de elementos na fila, ou -1 se ponteiro inválido
// Complexidade: O(1)
// Motivo: Acesso direto ao campo qtd, sem loops.
int tamanho_Fila(Fila* fi) {
    if (fi == NULL)
        return -1;
    return fi->qtd;
}

// Verifica se a fila está cheia
// Complexidade: O(1)
// Motivo: Compara qtd com constante MAX.
int Fila_cheia(Fila* fi) {
    if (fi == NULL)
        return -1;
    return (fi->qtd == MAX);
}

// Verifica se a fila está vazia
// Complexidade: O(1)
// Motivo: Compara qtd com zero.
int Fila_vazia(Fila* fi) {
    if (fi == NULL)
        return -1;
    return (fi->qtd == 0);
}

// Insere um aluno no final da fila; retorna 1 em sucesso, 0 caso contrário
// Complexidade: O(1)
// Motivo: Atribuição direta no vetor, ajuste de índice modular e incremento de contador.
int insere_Fila(Fila* fi, struct aluno al) {
    if (fi == NULL || fi->qtd == MAX)
        return 0;
    fi->dados[fi->final] = al;  // Insere o aluno na posição final
    fi->final = (fi->final + 1) % MAX; // Atualiza o índice final, com retorno ao início se necessário
    fi->qtd++;  // Aumenta a quantidade de elementos na fila
    return 1;
}

// Remove o aluno do início da fila; retorna 1 em sucesso, 0 caso contrário
// Complexidade: O(1)
// Motivo: Ajuste de índice e diminui o contador.
int remove_Fila(Fila* fi) {
    if (fi == NULL || fi->qtd == 0)
        return 0;
    fi->inicio = (fi->inicio + 1) % MAX; // Move o índice para o próximo elemento
    fi->qtd--;
    return 1;
}

// Consulta sem remover o aluno no início da fila; retorna 1 em sucesso, 0 caso contrário
// Complexidade: O(1)
// Motivo: Cópia direta de um elemento do vetor.
int consulta_Fila(Fila* fi, struct aluno *al) {
    if (fi == NULL || Fila_vazia(fi))
        return 0;
    *al = fi->dados[fi->inicio]; // Copia o aluno da posição inicial da fila para o ponteiro
    return 1;
}

// Imprime o estado atual da fila 
// Complexidade: O(n)

void imprime_Fila(Fila* fi) {
    printf("Fila (qtd = %d):\n", fi->qtd);
    int i, pos;
    for (i = 0, pos = fi->inicio; i < fi->qtd; i++) {
        struct aluno *a = &fi->dados[pos];
        printf("  [%d] Mat: %d  Nome: %-10s  Notas: %.1f, %.1f, %.1f\n",i+1, a->matricula, a->nome, a->n1, a->n2, a->n3);
        pos = (pos + 1) % MAX;
    }
    printf("\n");
}

int main() 
{
    setlocale(LC_ALL, "Portuguese");
    Fila *fi = cria_Fila();

    printf("Fila criada. \nFila vazia? %s\n\n",
           Fila_vazia(fi) == 1 ? "Sim" : "Não");

    struct aluno a1 = {101, "Guilherme", 8.5, 7.0, 9.0};
    struct aluno a2 = {103, "João",      6.0, 8.0, 7.5};
    struct aluno a3 = {102, "Pedro",     9.0, 9.5, 8.0};

    printf("Inserindo Guilherme, João e Pedro:\n");
    insere_Fila(fi, a1);
    insere_Fila(fi, a2);
    insere_Fila(fi, a3);
    imprime_Fila(fi);

    printf("Tamanho: %d   \nFila cheia? %s\n\n",
           tamanho_Fila(fi),
           Fila_cheia(fi) == 1 ? "Sim" : "Não");

    struct aluno consulta;
    if (consulta_Fila(fi, &consulta)) {
        printf("Consulta frente: %s (mat %d)\n\n",
               consulta.nome, consulta.matricula);
    }

    printf("Removendo um elemento: \n");
    remove_Fila(fi);
    imprime_Fila(fi);

    printf("Liberando fila...\n");
    libera_Fila(fi);
    printf("Fila liberada.\n");

    return 0;
}
