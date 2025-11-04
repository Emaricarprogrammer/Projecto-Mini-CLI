#ifndef CONTEUDO_H
#define CONTEUDO_H

#define MAX_CAMINHO 500
#define MAX_NOME_ARQUIVO 100
#define MAX_DESCRICAO 200
#define MAX_DISCIPLINA 100
#define MAX_CONTEUDOS 50

typedef struct
{
    char nome_arquivo[MAX_NOME_ARQUIVO];
    char caminho_local[MAX_CAMINHO];
    char descricao[MAX_DESCRICAO];
    char disciplina[MAX_DISCIPLINA];
    char data_upload[20];
} Conteudo;

typedef struct
{
    Conteudo conteudos[MAX_CONTEUDOS];
    int num_conteudos;
} ListaConteudos;

void inicializarListaConteudos(ListaConteudos *lista);
int adicionarConteudo(ListaConteudos *lista, const char *caminho_local, const char *descricao, const char *disciplina);
void exibirConteudos(const ListaConteudos *lista);
int fazerPushGitHub(const ListaConteudos *lista, const char *mensagem_commit);
int copiarArquivoParaRepositorio(const char *caminho_origem, const char *nome_arquivo, const char *disciplina);
int executarComandoGit(const char *comando);
void limparBufferConteudo();
void menuDisciplinas();
const char *obterNomeDisciplina(int opcao);

void listarConteudosParaDownload(const ListaConteudos *lista);
int baixarConteudo(const ListaConteudos *lista, int id_conteudo, const char *pasta_destino);
int baixarConteudosPorDisciplina(const ListaConteudos *lista, const char *disciplina, const char *pasta_destino);

int baixarConteudoDoGitHub(const char *disciplina, const char *nome_arquivo, const char *pasta_destino);
int listarConteudosDoGitHub();
int baixarTodosConteudosGitHub(const char *pasta_destino);
int baixarDisciplinaGitHub(const char *disciplina, const char *pasta_destino);
void listarDisciplinasGitHub();

#endif