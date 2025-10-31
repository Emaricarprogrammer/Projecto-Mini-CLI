#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "conteudo.h"

void inicializarListaConteudos(ListaConteudos *lista)
{
    lista->num_conteudos = 0;
}

void limparBufferConteudo()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void menuDisciplinas()
{
    printf("\n=== DISCIPLINAS DISPONIVEIS ===\n");
    printf("1. Programacao I\n");
    printf("2. Analise Matematica\n");
    printf("3. Logica Matematica\n");
    printf("4. Ingles Tecnico\n");
    printf("5. Metodologia de Investigacao Cientifica\n");
    printf("6. Comunicacao Escrita\n");
    printf("7. Introducao a Ciencias da Computacao\n");
    printf("Escolha uma disciplina: ");
}

const char *obterNomeDisciplina(int opcao)
{
    switch (opcao)
    {
    case 1:
        return "Programacao I";
    case 2:
        return "Analise Matematica";
    case 3:
        return "Logica Matematica";
    case 4:
        return "Ingles Tecnico";
    case 5:
        return "Metodologia de Investigacao Cientifica";
    case 6:
        return "Comunicacao Escrita";
    case 7:
        return "Introducao a Ciencias da Computacao";
    default:
        return "Outra";
    }
}

int copiarArquivoParaRepositorio(const char *caminho_origem, const char *nome_arquivo, const char *disciplina)
{
    char comando_copia[MAX_CAMINHO * 2];
    char pasta_disciplina[MAX_CAMINHO];

    // Criar estrutura de pastas
    snprintf(pasta_disciplina, sizeof(pasta_disciplina), "data\\conteudos\\%s", disciplina);

    // Criar pasta data/conteudos se nao existir
    system("mkdir data\\conteudos 2>nul");

    // Criar pasta da disciplina
    char comando_mkdir[MAX_CAMINHO];
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_disciplina);
    system(comando_mkdir);

    // Copiar arquivo para a pasta da disciplina
    snprintf(comando_copia, sizeof(comando_copia), "copy \"%s\" \"%s\\%s\"", caminho_origem, pasta_disciplina, nome_arquivo);

    printf("Copiando arquivo: %s\n", comando_copia);
    int resultado = system(comando_copia);

    if (resultado == 0)
    {
        printf("Arquivo copiado com sucesso para: %s\\%s\n", disciplina, nome_arquivo);
        return 0;
    }
    else
    {
        printf("Erro ao copiar arquivo: %s\n", nome_arquivo);
        return -1;
    }
}

int executarComandoGit(const char *comando)
{
    printf("Executando: %s\n", comando);
    int resultado = system(comando);

    if (resultado == 0)
    {
        printf("Comando executado com sucesso\n");
        return 0;
    }
    else
    {
        printf("Erro ao executar comando\n");
        return -1;
    }
}

int adicionarConteudo(ListaConteudos *lista, const char *caminho_local, const char *descricao, const char *disciplina)
{
    if (lista->num_conteudos >= MAX_CONTEUDOS)
    {
        printf("Lista de conteudos esta cheia! (Maximo: %d)\n", MAX_CONTEUDOS);
        return -1;
    }

    // Verificar se o arquivo existe
    FILE *arquivo = fopen(caminho_local, "r");
    if (arquivo == NULL)
    {
        printf("Erro: Arquivo nao encontrado: %s\n", caminho_local);
        return -2;
    }
    fclose(arquivo);

    // Extrair nome do arquivo do caminho
    const char *nome_arquivo = strrchr(caminho_local, '\\');
    if (nome_arquivo == NULL)
    {
        nome_arquivo = strrchr(caminho_local, '/');
    }
    if (nome_arquivo != NULL)
    {
        nome_arquivo++; // Pular a barra
    }
    else
    {
        nome_arquivo = caminho_local;
    }

    // Verificar se e PDF
    const char *extensao = strrchr(nome_arquivo, '.');
    if (extensao == NULL || (strcasecmp(extensao, ".pdf") != 0 && strcasecmp(extensao, ".txt") != 0))
    {
        printf("Aviso: O arquivo nao e PDF ou TXT. Continuando mesmo assim...\n");
    }

    Conteudo *novo_conteudo = &lista->conteudos[lista->num_conteudos];

    // Copiar arquivo para o repositorio local na pasta da disciplina
    if (copiarArquivoParaRepositorio(caminho_local, nome_arquivo, disciplina) != 0)
    {
        return -3;
    }

    // Preencher dados do conteudo
    strncpy(novo_conteudo->nome_arquivo, nome_arquivo, MAX_NOME_ARQUIVO - 1);
    novo_conteudo->nome_arquivo[MAX_NOME_ARQUIVO - 1] = '\0';

    strncpy(novo_conteudo->caminho_local, caminho_local, MAX_CAMINHO - 1);
    novo_conteudo->caminho_local[MAX_CAMINHO - 1] = '\0';

    strncpy(novo_conteudo->descricao, descricao, MAX_DESCRICAO - 1);
    novo_conteudo->descricao[MAX_DESCRICAO - 1] = '\0';

    strncpy(novo_conteudo->disciplina, disciplina, MAX_DISCIPLINA - 1);
    novo_conteudo->disciplina[MAX_DISCIPLINA - 1] = '\0';

    // Obter data atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(novo_conteudo->data_upload, sizeof(novo_conteudo->data_upload),
             "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    lista->num_conteudos++;
    printf("Conteudo adicionado com sucesso!\n");
    return 0;
}

void exibirConteudos(const ListaConteudos *lista)
{
    if (lista->num_conteudos == 0)
    {
        printf("Nenhum conteudo adicionado.\n");
        return;
    }

    printf("\n=== CONTEUDOS ADICIONADOS (%d) ===\n", lista->num_conteudos);
    printf("%-20s %-25s %-30s %-15s\n", "Disciplina", "Arquivo", "Descricao", "Data");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < lista->num_conteudos; i++)
    {
        const Conteudo *cont = &lista->conteudos[i];
        printf("%-20s %-25s %-30s %-15s\n",
               cont->disciplina, cont->nome_arquivo, cont->descricao, cont->data_upload);
    }
}

int fazerPushGitHub(const ListaConteudos *lista, const char *mensagem_commit)
{
    printf("\n=== FAZENDO PUSH PARA O GITHUB ===\n");

    // Verificar se e um repositorio git
    if (executarComandoGit("git status") != 0)
    {
        printf("Inicializando repositorio Git...\n");

        // Inicializar repositorio git
        if (executarComandoGit("git init") != 0)
        {
            printf("Erro ao inicializar repositorio Git\n");
            return -1;
        }

        // Adicionar remote origin
        char comando_remote[MAX_CAMINHO];
        snprintf(comando_remote, sizeof(comando_remote),
                 "git remote add origin https://github.com/Emaricarprogrammer/Projecto-Mini-CLI.git");
        if (executarComandoGit(comando_remote) != 0)
        {
            printf("Erro ao adicionar remote origin\n");
            return -1;
        }
    }

    // Adicionar todos os arquivos
    if (executarComandoGit("git add .") != 0)
    {
        printf("Erro ao adicionar arquivos\n");
        return -1;
    }

    // Fazer commit
    char comando_commit[MAX_CAMINHO + 100];
    if (strlen(mensagem_commit) == 0)
    {
        snprintf(comando_commit, sizeof(comando_commit),
                 "git commit -m \"Adicao de conteudos educacionais - %d arquivos\"",
                 lista->num_conteudos);
    }
    else
    {
        snprintf(comando_commit, sizeof(comando_commit),
                 "git commit -m \"%s\"", mensagem_commit);
    }

    if (executarComandoGit(comando_commit) != 0)
    {
        printf("Erro ao fazer commit\n");
        return -1;
    }

    // Fazer push para o GitHub
    printf("Fazendo push para o GitHub...\n");
    if (executarComandoGit("git push -u origin main") != 0)
    {
        // Tentar com master se main falhar
        if (executarComandoGit("git push -u origin master") != 0)
        {
            printf("Erro ao fazer push para o GitHub\n");
            printf("Verifique suas credenciais Git\n");
            return -1;
        }
    }

    printf("\n✅ Push realizado com sucesso para o GitHub!\n");
    printf("Repositorio: https://github.com/Emaricarprogrammer/Projecto-Mini-CLI.git\n");
    return 0;
}