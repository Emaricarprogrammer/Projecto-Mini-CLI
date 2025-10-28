#ifndef ALUNOS_H
#define ALUNOS_H

#define MAX_NOME 100
#define MAX_EMAIL 100
#define MAX_CONTACTO 20
#define MAX_ALUNOS 100

typedef struct
{
    char nome[MAX_NOME];
    char numero_estudante[20];
    char email[MAX_EMAIL];
    char contacto[MAX_CONTACTO];
} Aluno;

typedef struct
{
    Aluno alunos[MAX_ALUNOS];
    int quantidade;
} ListaAlunos;

// Funcoes para manipulacao da lista
void inicializarLista(ListaAlunos *lista);
int adicionarAluno(ListaAlunos *lista, const char *nome, const char *numero_estudante, const char *email, const char *contacto);
void exibirAlunos(const ListaAlunos *lista);
int buscarAluno(const ListaAlunos *lista, const char *numero_estudante);
int removerAluno(ListaAlunos *lista, const char *numero_estudante);
int salvarLista(const ListaAlunos *lista, const char *filename);
int carregarLista(ListaAlunos *lista, const char *filename);
void limparBuffer();

#endif