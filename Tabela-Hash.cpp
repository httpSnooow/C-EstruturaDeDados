#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct aluno {
        int matricula;
        char nome[30];
        float N1, N2, N3, N4, MF;
    };
struct hash {
    int qtd, TABLE_SIZE;
    struct aluno **itens;
};
typedef struct hash Hash;


Hash* criaHash (int tamanho);
void liberaHash (Hash* ha);
int valorString (char *str);
int insereHash_SemColisao (Hash* ha, struct aluno al);
int buscaHash_Semcolisao (Hash* ha, int mat, struct aluno* al);

//  Calcula a posição na tabela usando o método da divisão
//  Complexidade: O(1)
//  Motivo: Realiza apenas operações
int chaveDivisao (int chave, int TABLE_SIZE)
{
    return (chave & 0x7FFFFFFF) % TABLE_SIZE;
}

//  Aloca memória e inicializa uma tabela hash
//  Complexidade: O(N) (N = TABLE_SIZE)
//  Motivo: Percorre o array de ponteiros para inicializar com NULL
Hash* criaHash (int TABLE_SIZE)
{
    Hash* ha = (Hash*) malloc (sizeof (Hash));
    if (ha != NULL) {
        int i;
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->itens = (struct aluno**) malloc (TABLE_SIZE * sizeof (struct aluno*));
        if (ha->itens == NULL)
        {
            free (ha);
            return NULL;
        }
        ha->qtd = 0;
        for (i = 0; i < ha->TABLE_SIZE; i++)
            ha->itens[i] = NULL;
    }
    return ha;
}

//  Libera toda a memória alocada para a tabela hash
//  Complexidade: O(N) (N = TABLE_SIZE)
//  Motivo: Percorre a tabela inteira para liberar cada elemento
void liberaHash (Hash* ha)
{
    if (ha != NULL)
    {
        int i;

        for (i = 0; i < ha->TABLE_SIZE; i++)
        {
            if (ha->itens[i] != NULL)
                free (ha->itens[i]);
        }
        free (ha->itens);
        free (ha);
    }
}

//  Insere um aluno na tabela (método sem tratamento de colisão)
//  Complexidade: O(1)
//  Motivo: Calcula a posição e insere diretamente no índice
int insereHash_SemColisao (Hash* ha, struct aluno al)
{
    if (ha == NULL || ha->qtd == ha->TABLE_SIZE)
        return 0;
    
    int chave = al.matricula;
    int pos = chaveDivisao (chave, ha->TABLE_SIZE);

    struct aluno* novo;
    
    novo = (struct aluno*) malloc (sizeof(struct aluno));
    
    if (novo == NULL)
        return 0;
        
    *novo = al;
    ha->itens[pos] = novo;
    ha->qtd++;
    return 1;
}

//  Busca um aluno pela matrícula (método sem tratamento de colisão)
//  Complexidade: O(1)
//  Motivo: Calcula a posição e verifica diretamente o índice
int buscaHash_Semcolisao (Hash* ha, int mat, struct aluno* al)
{
    if (ha == NULL)
        return 0;
        
    int pos = chaveDivisao (mat, ha->TABLE_SIZE);
    
    if (ha->itens[pos] == NULL)
        return 0;
        
    if(ha->itens[pos]->matricula != mat)
        return 0; 
        
    *al = *(ha->itens[pos]);
    return 1;
}

int main()
{
    int i;
    int table_size = 31; 
    setlocale(LC_ALL,"Portuguese");
    
    //a) Criar uma tabela hash com 31 posições - use a função criaHash()
    Hash* ha = criaHash(table_size);
    if(ha == NULL){
        printf("Erro na alocacao da tabela hash.\n");
        return 1;
    }

    //b) Após a criação, imprima a quantidade de posições livres e ocupadas da tabela hash.
    printf("Tabela hash CRIADA com %d posições: %d ocupada(s) e %d livre(s).\n", ha->TABLE_SIZE, ha->qtd, ha->TABLE_SIZE - ha->qtd);

    //c) Inserir 25 alunos na tabela hash criada na alternativa a) 
    printf ("\nInserindo 25 alunos na tabela hash...\n");
    
    struct aluno al_insere;
    for (i = 0; i < 25; i++) {
        al_insere.matricula = 2300 + i;
        sprintf(al_insere.nome, "Aluno %d", 2300 + i);
        al_insere.N1 = 0;
        al_insere.N2 = 0;
        al_insere.N3 = 0;
        al_insere.N4 = 0;
        al_insere.MF = 0;
        
        insereHash_SemColisao(ha, al_insere);
    }

    //d) Após a inserção, imprima a quantidade de posições livres e ocupadas da tabela hash.
    printf("Tabela hash com %d posições: %d ocupada(s) e %d livre(s).\n", ha->TABLE_SIZE, ha->qtd, ha->TABLE_SIZE - ha->qtd);

    //e) Atribua as notas para o aluno de matrícula 2305 e 2301 usando a tabela hash. 
    
    printf ("\nAtribuindo as notas e calculando a média...\n");
    
    int mat_update;
    int pos_update;
    
   
    mat_update = 2305;
    pos_update = chaveDivisao(mat_update, ha->TABLE_SIZE);
    
    if (ha->itens[pos_update] != NULL && ha->itens[pos_update]->matricula == mat_update) {
        ha->itens[pos_update]->N1 = 6;
        ha->itens[pos_update]->N2 = 9;
        ha->itens[pos_update]->N3 = 5;
        ha->itens[pos_update]->N4 = 8;
        ha->itens[pos_update]->MF = (6.0 + 9.0 + 5.0 + 8.0) / 4.0;
    } else {
        printf("\nErro ao tentar atualizar matricula %d", mat_update);
    }

   
    mat_update = 2301;
    pos_update = chaveDivisao(mat_update, ha->TABLE_SIZE);

    if (ha->itens[pos_update] != NULL && ha->itens[pos_update]->matricula == mat_update) {
        ha->itens[pos_update]->N1 = 9;
        ha->itens[pos_update]->N2 = 8;
        ha->itens[pos_update]->N3 = 9.5;
        ha->itens[pos_update]->N4 = 10;
        ha->itens[pos_update]->MF = (9.0 + 8.0 + 9.5 + 10.0) / 4.0;
    } else {
        printf("\nErro ao tentar atualizar matricula %d", mat_update);
    }


    // f) Busque as notas das matrícula 2301, 2302, 2305 e 2351.
    printf ("\nConsultando as notas...\n");
    
    struct aluno al_busca;
    int matriculas_busca[] = {2301, 2302, 2305, 2351}; 
    
    for (i = 0; i < 4; i++) {
        int mat_busca = matriculas_busca[i];
        if (buscaHash_Semcolisao(ha, mat_busca, &al_busca)) {
            int pos_busca = chaveDivisao(mat_busca, ha->TABLE_SIZE);
            printf("\nMatrícula %d na posição %d.\n", al_busca.matricula, pos_busca);
            printf("Notas: %.1f, %.1f, %.1f, %.1f e MF: %.1f\n",
                   al_busca.N1, al_busca.N2, al_busca.N3, al_busca.N4, al_busca.MF);
        } else {
            printf("\nMatrícula %d não encontrada na tabela hash.\n", mat_busca);
        }
    }

    // h) Desaloque a memória alocada para a tabela hash.
    liberaHash(ha);

    return 0;
}
