#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX 100
#define TAM_NOME 50
#define TAM_MUSICA 50
#define QTD_MUSICAS 3

typedef struct Amigo {
    int id;
    char nome[TAM_NOME];
    char musicas[QTD_MUSICAS][TAM_MUSICA];
} Amigo;

typedef struct Lista {
    int tamanho;
    struct Amigo amigos[MAX];
} Lista;

void inicializaLista(Lista *l) {
    if (l != NULL)
        l->tamanho = 0;
}

int insereAmigo(Lista *l, Amigo a) {
    if (l->tamanho == MAX) {
        return 0;
    }
    l->amigos[l->tamanho] = a;
    l->tamanho++;
    return 1;
}

void imprimeLista(Lista *l) {
    int i, j;
    for (i = 0; i < l->tamanho; i++) {
        struct Amigo *a = &l->amigos[i];
        printf("ID: %d\n", a->id);
        printf("Nome: %s\n", a->nome);
        printf("Músicas preferidas:\n");
        for (j = 0; j < QTD_MUSICAS; j++) {
            printf("  %d. %s\n", j + 1, a->musicas[j]);
        }
        printf("-----------------------\n");
    }
}

int verificaOrdemID(Lista *l) {
    int i;
    for (i = 0; i < l->tamanho - 1; i++) {
        if (l->amigos[i].id > l->amigos[i + 1].id)
            return 0;
    }
    return 1;
}

int buscaRecursivaPorID(Lista *l, int id, int index) {
    if (index >= l->tamanho)
        return -1;
    if (l->amigos[index].id == id)
        return index;
    return buscaRecursivaPorID(l, id, index + 1);
}

void imprimeAmigo(Amigo a) {
    int i;
    printf("ID: %d\n", a.id);
    printf("Nome: %s\n", a.nome);
    printf("Músicas:\n");
    for (i = 0; i < QTD_MUSICAS; i++) {
        printf("  %d. %s\n", i + 1, a.musicas[i]);
    }
    printf("-------------------------\n");
}

int buscaBinariaRecursiva(Lista *l, int id, int inicio, int fim) {
    if (inicio > fim)
        return -1;

    int meio = inicio + (fim - inicio) / 2;

    if (l->amigos[meio].id == id)
        return meio;
    else if (id < l->amigos[meio].id)
        return buscaBinariaRecursiva(l, id, inicio, meio - 1);
    else
        return buscaBinariaRecursiva(l, id, meio + 1, fim);
}

void troca(Amigo *a, Amigo *b) {
    Amigo temp = *a;
    *a = *b;
    *b = temp;
}

int particiona(Lista *l, int esq, int dir) {
    Amigo pivo = l->amigos[dir];
    int i = (esq - 1);
    int j;

    for (j = esq; j <= dir - 1; j++) {
        if (l->amigos[j].id < pivo.id) {
            i++;
            troca(&l->amigos[i], &l->amigos[j]);
        }
    }
    troca(&l->amigos[i + 1], &l->amigos[dir]);
    return (i + 1);
}

void quicksort_recursivo(Lista *l, int esq, int dir) {
    if (esq < dir) {
        int pivo_idx = particiona(l, esq, dir);
        quicksort_recursivo(l, esq, pivo_idx - 1);
        quicksort_recursivo(l, pivo_idx + 1, dir);
    }
}

void ordenarPorID(Lista *l) {
    if (l->tamanho > 1) {
        quicksort_recursivo(l, 0, l->tamanho - 1);
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    Lista l;
    
    inicializaLista(&l);

    Amigo a1 = {20, "Alice", {"Bohemian Rhapsody", "Imagine", "Hotel California"}};
    insereAmigo(&l, a1);
    Amigo a2 = {19, "Bruno", {"Shape of You", "Blinding Lights", "Levitating"}};
    insereAmigo(&l, a2);
    Amigo a3 = {18, "Carla", {"Hey Jude", "Smells Like Teen Spirit", "Wonderwall"}};
    insereAmigo(&l, a3);
    Amigo a4 = {17, "Daniel", {"Garota de Ipanema", "Aquarela do Brasil", "Chega de Saudade"}};
    insereAmigo(&l, a4);
    Amigo a5 = {16, "Elisa", {"Asa Branca", "Romaria", "Tocando em Frente"}};
    insereAmigo(&l, a5);

    printf("=== Lista de Amigos - Ordem de Inserção ===\n");
    imprimeLista(&l);

    if (verificaOrdemID(&l) == 0)
        printf("\nA lista de Amigos não está em ordem crescente de ID.\n");
    else
        printf("\nA lista de Amigos está em ordem crescente de ID.\n");

    int idBusca;
    printf("Digite o ID a buscar:\n");
    scanf("%d", &idBusca);
    int pos = buscaRecursivaPorID(&l, idBusca, 0);

    if (pos == -1)
        printf("Amigo com ID %d não encontrado.\n", idBusca);
    else {
        printf("Amigo encontrado:\n");
        imprimeAmigo(l.amigos[pos]);
    }

    printf("************************\n");
    ordenarPorID(&l);
    
    printf("=== Lista de Amigos - Ordem crescente de ID ===\n");
    imprimeLista(&l);

    printf("************************\n");
    if (verificaOrdemID(&l) == 1) {
        pos = buscaBinariaRecursiva(&l, idBusca, 0, l.tamanho - 1);
        if (pos == -1) {
            printf("Amigo com ID %d não encontrado (Busca Binária).\n", idBusca);
        } else {
            printf("Amigo encontrado (Busca Binária):\n");
            imprimeAmigo(l.amigos[pos]);
        }
    }

    return 0;
}
