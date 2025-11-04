#ifndef DELEGADO_H
#define DELEGADO_H

#include "alunos.h"

#define MAX_CANDIDATOS 20
#define MAX_VOTOS 100

typedef struct
{
    Aluno candidato;
    int votos;
    char tipo[15]; // "Delegado" ou "Subdelegado"
} Candidato;

typedef struct
{
    Candidato candidatos[MAX_CANDIDATOS];
    int num_candidatos;
    int votos_total;
    int votos_brancos;
    int eleicao_realizada;
    Aluno delegado_eleito;
    Aluno subdelegados_eleitos[2];
    int num_subdelegados_eleitos;
} Eleicao;

// Funcoes para eleicao
void inicializarEleicao(Eleicao *eleicao);
int adicionarCandidato(Eleicao *eleicao, ListaAlunos *lista, int numero_aluno, const char *tipo);
void listarCandidatos(const Eleicao *eleicao);
void realizarEleicaoAutomatica(Eleicao *eleicao, ListaAlunos *lista);
void apurarResultados(const Eleicao *eleicao);
int enviarEmailVencedorReal(const Aluno *vencedor, const char *tipo_cargo);
void salvarResultadosEleicao(const Eleicao *eleicao, const char *filename);
void carregarResultadosEleicao(Eleicao *eleicao, const char *filename);
void exibirResultadosEleicao(const Eleicao *eleicao);

#endif