#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct aluno {
    int matricula;
    char nome[30];
    float n1, n2, n3;
};

typedef struct Elemento {
    struct aluno dados;
    struct Elemento *prox;
    struct Elemento *ant;
} Elem;

struct EstruturaInternaLista {
    Elem *sentinela;
    int qtd;
};

typedef struct EstruturaInternaLista* Lista;

Lista cria_lista();
void libera_lista(Lista li);
int insere_lista_final(Lista li, struct aluno al);
int insere_lista_inicio(Lista li, struct aluno al);
int remove_lista_inicio(Lista li);
int remove_lista_final(Lista li);
int tamanho_lista(Lista li);
int lista_vazia(Lista li);
int lista_cheia(Lista li);
int busca_lista_mat(Lista li, int mat, struct aluno *al);
int busca_lista_pos(Lista li, int pos, struct aluno *al);

Lista cria_lista() {
    Lista li = (struct EstruturaInternaLista*) malloc(sizeof(struct EstruturaInternaLista));
    if (li == NULL) {
        return NULL;
    }

    li->sentinela = (Elem*) malloc(sizeof(Elem));
    if (li->sentinela == NULL) {
        free(li);
        return NULL;
    }

    li->sentinela->prox = li->sentinela;
    li->sentinela->ant = li->sentinela;
    li->qtd = 0;

    return li;
}

void libera_lista(Lista li) {
    if (li == NULL) {
        return;
    }

    if (li->sentinela != NULL) {
        Elem *atual = li->sentinela->prox;
        while (atual != li->sentinela) {
            Elem *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
        free(li->sentinela);
    }
    free(li);
}

int insere_lista_final(Lista li, struct aluno al) {
    if (li == NULL || li->sentinela == NULL) {
        return 0;
    }

    Elem *novo_no = (Elem*) malloc(sizeof(Elem));
    if (novo_no == NULL) {
        return 0;
    }
    novo_no->dados = al;

    novo_no->prox = li->sentinela;
    novo_no->ant = li->sentinela->ant;
    li->sentinela->ant->prox = novo_no;
    li->sentinela->ant = novo_no;

    li->qtd++;
    return 1;
}

int insere_lista_inicio(Lista li, struct aluno al) {
    if (li == NULL || li->sentinela == NULL) {
        return 0;
    }

    Elem *novo_no = (Elem*) malloc(sizeof(Elem));
    if (novo_no == NULL) {
        return 0;
    }
    novo_no->dados = al;

    novo_no->prox = li->sentinela->prox;
    novo_no->ant = li->sentinela;
    li->sentinela->prox->ant = novo_no;
    li->sentinela->prox = novo_no;

    li->qtd++;
    return 1;
}

int remove_lista_inicio(Lista li) {
    if (li == NULL || li->sentinela == NULL || li->qtd == 0) {
        return 0;
    }

    Elem *no_remover = li->sentinela->prox;

    li->sentinela->prox = no_remover->prox;
    no_remover->prox->ant = li->sentinela;

    free(no_remover);
    li->qtd--;
    return 1;
}

int remove_lista_final(Lista li) {
    if (li == NULL || li->sentinela == NULL || li->qtd == 0) {
        return 0;
    }

    Elem *no_remover = li->sentinela->ant;

    li->sentinela->ant = no_remover->ant;
    no_remover->ant->prox = li->sentinela;

    free(no_remover);
    li->qtd--;
    return 1;
}

int tamanho_lista(Lista li) {
    if (li == NULL) {
        return 0;
    }
    return li->qtd;
}

int lista_vazia(Lista li) {
    if (li == NULL || li->qtd == 0) {
        return 1;
    }
    return 0;
}

int lista_cheia(Lista li) {
    if (li == NULL) return 1;
    return 0;
}

int busca_lista_mat(Lista li, int mat, struct aluno *al) {
    if (li == NULL || li->sentinela == NULL || al == NULL || li->qtd == 0) {
        return 0;
    }

    Elem *atual = li->sentinela->prox;
    while (atual != li->sentinela) {
        if (atual->dados.matricula == mat) {
            *al = atual->dados;
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

int busca_lista_pos(Lista li, int pos, struct aluno *al) {
    if (li == NULL || li->sentinela == NULL || al == NULL) {
        return 0;
    }
    if (pos <= 0 || pos > li->qtd) {
        return 0;
    }

    Elem *atual = li->sentinela->prox;
    for (int i = 1; i < pos; i++) {
        atual = atual->prox;
    }
    *al = atual->dados;
    return 1;
}

void print_aluno(struct aluno al) {
    printf("Mat: %d, Nome: %s, N1: %.1f, N2: %.1f, N3: %.1f\n",
           al.matricula, al.nome, al.n1, al.n2, al.n3);
}

void print_lista(Lista li_param) {
    if (li_param == NULL) {
        printf("ERRO: Lista eh NULL (ponteiro para descritor eh nulo).\n");
        return;
    }
    if (lista_vazia(li_param)) {
        printf("Lista Vazia. (Tamanho: %d)\n", tamanho_lista(li_param));
        return;
    }
    printf("Conteudo da Lista (Tamanho: %d):\n", tamanho_lista(li_param));
    Elem* atual = li_param->sentinela->prox;
    int pos_print = 1;
    while(atual != li_param->sentinela) {
        printf("  Pos %d: ", pos_print++);
        print_aluno(atual->dados);
        atual = atual->prox;
    }
    printf("\n");
}

int main() {
    Lista lista;
    struct aluno temp_aluno;

    printf("## Teste 1: cria_lista ##\n");
    lista = cria_lista();
    if (lista == NULL) {
        printf("Falha ao criar a lista. Encerrando.\n");
        return 1;
    }
    printf("Lista criada com sucesso.\n");
    print_lista(lista);
    printf("Lista vazia? %s\n", lista_vazia(lista) ? "Sim" : "Nao");
    printf("Lista cheia? %s\n", lista_cheia(lista) ? "Sim (se lista fosse NULL)" : "Nao");
    printf("Tamanho da lista: %d\n", tamanho_lista(lista));
    printf("\n\n");

    printf("## Teste 2: Insercoes ##\n");
    struct aluno a1 = {101, "Ana", 7.0f, 8.0f, 9.0f};
    struct aluno a2 = {102, "Beto", 6.0f, 6.5f, 7.0f};
    struct aluno a3 = {103, "Carla", 8.5f, 9.0f, 9.5f};
    struct aluno a4 = {104, "Daniel", 5.0f, 5.5f, 6.0f};

    printf("Inserindo no inicio (a1):\n");
    insere_lista_inicio(lista, a1);
    print_lista(lista);

    printf("Inserindo no final (a2):\n");
    insere_lista_final(lista, a2);
    print_lista(lista);

    printf("Inserindo no inicio (a3):\n");
    insere_lista_inicio(lista, a3);
    print_lista(lista);

    printf("Inserindo no final (a4):\n");
    insere_lista_final(lista, a4);
    print_lista(lista);

    printf("Lista vazia? %s\n", lista_vazia(lista) ? "Sim" : "Nao");
    printf("Lista cheia? %s\n", lista_cheia(lista) ? "Sim" : "Nao");
    printf("Tamanho da lista: %d\n", tamanho_lista(lista));
    printf("\n\n");

    printf("## Teste 3: Buscas ##\n");
    printf("Buscando por matricula 101 (existe - Ana):\n");
    if (busca_lista_mat(lista, 101, &temp_aluno)) {
        printf("Encontrado: "); print_aluno(temp_aluno);
    } else {
        printf("Nao encontrado.\n");
    }

    printf("Buscando por matricula 999 (nao existe):\n");
    if (busca_lista_mat(lista, 999, &temp_aluno)) {
        printf("Encontrado: "); print_aluno(temp_aluno);
    } else {
        printf("Nao encontrado.\n");
    }

    printf("Buscando por posicao 2 (existe - deve ser Ana {101}):\n");
    if (busca_lista_pos(lista, 2, &temp_aluno)) {
        printf("Encontrado: "); print_aluno(temp_aluno);
    } else {
        printf("Nao encontrado ou posicao invalida.\n");
    }

    printf("Buscando por posicao 4 (existe - deve ser Daniel {104}):\n");
    if (busca_lista_pos(lista, 4, &temp_aluno)) {
        printf("Encontrado: "); print_aluno(temp_aluno);
    } else {
        printf("Nao encontrado ou posicao invalida.\n");
    }

    printf("Buscando por posicao 0 (invalida):\n");
    if (busca_lista_pos(lista, 0, &temp_aluno)) {
        printf("Encontrado: "); print_aluno(temp_aluno);
    } else {
        printf("Nao encontrado ou posicao invalida.\n");
    }

    printf("Buscando por posicao 5 (invalida - fora dos limites):\n");
    if (busca_lista_pos(lista, 5, &temp_aluno)) {
        printf("Encontrado: "); print_aluno(temp_aluno);
    } else {
        printf("Nao encontrado ou posicao invalida.\n");
    }
    printf("\n\n");

    printf("## Teste 4: Remocoes ##\n");
    print_lista(lista);

    printf("Removendo do inicio (deve ser Carla {103}):\n");
    if(remove_lista_inicio(lista)) printf("Remocao bem sucedida.\n"); else printf("Falha na remocao.\n");
    print_lista(lista);

    printf("Removendo do final (deve ser Daniel {104}):\n");
    if(remove_lista_final(lista)) printf("Remocao bem sucedida.\n"); else printf("Falha na remocao.\n");
    print_lista(lista);

    printf("Removendo do inicio (deve ser Ana {101}):\n");
    if(remove_lista_inicio(lista)) printf("Remocao bem sucedida.\n"); else printf("Falha na remocao.\n");
    print_lista(lista);

    printf("Removendo do final (deve ser Beto {102} - ultimo elemento):\n");
    if(remove_lista_final(lista)) printf("Remocao bem sucedida.\n"); else printf("Falha na remocao.\n");
    print_lista(lista);

    printf("Lista vazia? %s\n", lista_vazia(lista) ? "Sim" : "Nao");
    printf("Tamanho da lista: %d\n", tamanho_lista(lista));
    printf("\n\n");

    printf("## Teste 5: Operacoes em lista vazia apos remocoes ##\n");
    printf("Tentando remover do inicio de lista vazia:\n");
    if(remove_lista_inicio(lista)) printf("Remocao bem sucedida (ERRO ESPERADO).\n"); else printf("Falha na remocao (correto).\n");

    printf("Tentando remover do final de lista vazia:\n");
    if(remove_lista_final(lista)) printf("Remocao bem sucedida (ERRO ESPERADO).\n"); else printf("Falha na remocao (correto).\n");

    printf("Tentando buscar por matricula 101 em lista vazia:\n");
    if (busca_lista_mat(lista, 101, &temp_aluno)) {
        printf("Encontrado (ERRO ESPERADO): "); print_aluno(temp_aluno);
    } else {
        printf("Nao encontrado (correto).\n");
    }

    printf("Tentando buscar por posicao 1 em lista vazia:\n");
    if (busca_lista_pos(lista, 1, &temp_aluno)) {
        printf("Encontrado (ERRO ESPERADO): "); print_aluno(temp_aluno);
    } else {
        printf("Nao encontrado ou posicao invalida (correto).\n");
    }
    printf("\n\n");

    printf("## Teste 6: libera_lista ##\n");
    printf("Liberando a lista...\n");
    libera_lista(lista);
    lista = NULL;
    printf("Lista liberada.\n");

    printf("Tentando liberar uma lista NULL (deve ser seguro):\n");
    libera_lista(NULL);
    printf("libera_lista(NULL) executado sem falhas.\n");

    printf("\nTodos os testes concluidos.\n");

    return 0;
}
