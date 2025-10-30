#ifndef GRUPOS_H
#define GRUPOS_H

#include "alunos.h"

#define MAX_GRUPOS 20
#define MAX_DISCIPLINA 100
#define MAX_TEMA 100
#define MAX_ELEMENTOS_POR_GRUPO 10

typedef struct
{
    int id_grupo;
    int numero_grupo;
    char disciplina[MAX_DISCIPLINA];
    char tema[MAX_TEMA];
    Aluno elementos[MAX_ELEMENTOS_POR_GRUPO];
    int num_elementos;
    int indice_lider;
} Grupo;

typedef struct
{
    Grupo grupos[MAX_GRUPOS];
    int num_grupos;
    int proximo_id;
} ListaGrupos;

// Funcoes para manipulacao de grupos
void inicializarListaGrupos(ListaGrupos *lista);
int gerarGruposAutomaticos(ListaGrupos *lista, const ListaAlunos *alunos, int elementos_por_grupo, const char *disciplina, const char *tema);
void exibirGrupos(const ListaGrupos *lista);
int salvarGrupos(const ListaGrupos *lista, const char *filename);
int carregarGrupos(ListaGrupos *lista, const char *filename);
int enviarEmailReal(const char *para, const char *assunto, const char *mensagem);
void enviarEmailGrupos(const ListaGrupos *lista);
void enviarEmailGrupoEspecifico(const ListaGrupos *lista, int id_grupo);
void escolherLiderGrupo(ListaGrupos *lista, int id_grupo);

#endif