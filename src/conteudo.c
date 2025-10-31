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

    // Tentar copiar usando diferentes metodos
    printf("Tentando copiar: %s\n", caminho_origem);

    // Metodo 1: comando COPY normal
    snprintf(comando_copia, sizeof(comando_copia), "copy \"%s\" \"%s\\%s\"", caminho_origem, pasta_disciplina, nome_arquivo);
    printf("Comando: %s\n", comando_copia);

    int resultado = system(comando_copia);

    if (resultado == 0)
    {
        printf("Arquivo copiado com sucesso para: %s\\%s\n", disciplina, nome_arquivo);
        return 0;
    }
    else
    {
        printf("Erro ao copiar arquivo com COPY. Tentando XCOPY...\n");

        // Metodo 2: Tentar com XCOPY (mais robusto)
        snprintf(comando_copia, sizeof(comando_copia), "xcopy \"%s\" \"%s\\\" /Y /I", caminho_origem, pasta_disciplina);
        resultado = system(comando_copia);

        if (resultado == 0)
        {
            printf("Arquivo copiado com XCOPY para: %s\\%s\n", disciplina, nome_arquivo);
            return 0;
        }
        else
        {
            printf("Erro grave: Nao foi possivel copiar o arquivo: %s\n", nome_arquivo);
            printf("Dica: Copie manualmente o arquivo para: %s\\%s\n", pasta_disciplina, nome_arquivo);
            return -1;
        }
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

    // Verificar se o arquivo existe - modo binario para melhor compatibilidade
    FILE *arquivo = fopen(caminho_local, "rb");
    if (arquivo == NULL)
    {
        printf("Erro: Arquivo nao encontrado ou inacessivel: %s\n", caminho_local);
        printf("Possiveis causas:\n");
        printf("- Unidade A: nao existe ou nao esta conectada\n");
        printf("- Caminho incorreto\n");
        printf("- Arquivo nao existe\n");
        printf("- Permissoes insuficientes\n");
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

    printf("Arquivo detectado: %s\n", nome_arquivo);

    // Verificar se e PDF
    const char *extensao = strrchr(nome_arquivo, '.');
    if (extensao == NULL || (strcasecmp(extensao, ".pdf") != 0 && strcasecmp(extensao, ".txt") != 0))
    {
        printf("Aviso: O arquivo nao e PDF ou TXT. Continuando mesmo assim...\n");
    }

    Conteudo *novo_conteudo = &lista->conteudos[lista->num_conteudos];

    // Tentar copiar arquivo para o repositorio local na pasta da disciplina
    printf("Copiando arquivo para o repositorio...\n");
    if (copiarArquivoParaRepositorio(caminho_local, nome_arquivo, disciplina) != 0)
    {
        printf("Falha ao copiar arquivo. Tentando metodo alternativo...\n");

        // Metodo alternativo: criar arquivo vazio e tentar novamente
        char caminho_destino[MAX_CAMINHO];
        snprintf(caminho_destino, sizeof(caminho_destino), "data\\conteudos\\%s\\%s", disciplina, nome_arquivo);

        FILE *dest = fopen(caminho_destino, "wb");
        if (dest != NULL)
        {
            fclose(dest);
            printf("Estrutura criada. Agora copie manualmente o arquivo.\n");
        }

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
    printf("Localizacao: data\\conteudos\\%s\\%s\n", disciplina, nome_arquivo);
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
    printf("Branch de destino: arturDev929\n");
    printf("Repositorio: https://github.com/Emaricarprogrammer/Projecto-Mini-CLI.git\n");

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

    // Verificar se a branch arturDev929 existe
    printf("Verificando branch arturDev929...\n");
    if (executarComandoGit("git branch -a | findstr arturDev929") != 0)
    {
        printf("Branch arturDev929 nao encontrada. Criando nova branch...\n");

        // Criar e mudar para a branch arturDev929
        if (executarComandoGit("git checkout -b arturDev929") != 0)
        {
            printf("Erro ao criar branch arturDev929\n");
            return -1;
        }
    }
    else
    {
        // Mudar para a branch arturDev929
        printf("Mudando para branch arturDev929...\n");
        if (executarComandoGit("git checkout arturDev929") != 0)
        {
            printf("Erro ao mudar para branch arturDev929\n");
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
                 "git commit -m \"Adicao de conteudos educacionais - %d arquivos [arturDev929]\"",
                 lista->num_conteudos);
    }
    else
    {
        snprintf(comando_commit, sizeof(comando_commit),
                 "git commit -m \"%s [arturDev929]\"", mensagem_commit);
    }

    if (executarComandoGit(comando_commit) != 0)
    {
        printf("Erro ao fazer commit\n");
        return -1;
    }

    // Fazer push para a branch arturDev929 no GitHub
    printf("Fazendo push para a branch arturDev929 no GitHub...\n");

    // Primeiro tentar push normal
    if (executarComandoGit("git push -u origin arturDev929") != 0)
    {
        printf("Tentando push com force...\n");
        // Se falhar, tentar com --force (cuidado: sobrescreve o remote)
        if (executarComandoGit("git push -u origin arturDev929 --force") != 0)
        {
            printf("Erro ao fazer push para o GitHub\n");
            printf("Verifique suas credenciais Git e permissoes\n");
            return -1;
        }
    }

    printf("\nPush realizado com sucesso para o GitHub!\n");
    printf("Branch: arturDev929\n");
    printf("Repositorio: https://github.com/Emaricarprogrammer/Projecto-Mini-CLI.git\n");
    printf("URL direta: https://github.com/Emaricarprogrammer/Projecto-Mini-CLI/tree/arturDev929\n");

    return 0;
}

// Funcao para obter a pasta Documentos do usuario
char *obterPastaDocumentos()
{
    static char documentos[MAX_CAMINHO];

    // Tentar obter via variavel de ambiente USERPROFILE
    char *userprofile = getenv("USERPROFILE");
    if (userprofile != NULL)
    {
        snprintf(documentos, sizeof(documentos), "%s\\Documents", userprofile);
        return documentos;
    }

    // Fallback para caminho padrao
    strcpy(documentos, "C:\\Users\\Usuario\\Documents");
    return documentos;
}

// Funcao para baixar um conteudo especifico
int baixarConteudo(const char *disciplina, const char *nome_arquivo, const char *pasta_destino)
{
    char caminho_origem[MAX_CAMINHO];
    char comando_copia[MAX_CAMINHO * 2];

    // Construir caminho de origem
    snprintf(caminho_origem, sizeof(caminho_origem), "data\\conteudos\\%s\\%s", disciplina, nome_arquivo);

    printf("Origem: %s\n", caminho_origem);
    printf("Destino: %s\n", pasta_destino);

    // Verificar se arquivo de origem existe
    FILE *arquivo = fopen(caminho_origem, "rb");
    if (arquivo == NULL)
    {
        printf("Erro: Arquivo nao encontrado: %s\n", caminho_origem);
        return -1;
    }
    fclose(arquivo);

    // Criar pasta de destino se nao existir
    char comando_mkdir[MAX_CAMINHO];
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_destino);
    system(comando_mkdir);

    // Copiar arquivo
    snprintf(comando_copia, sizeof(comando_copia), "copy \"%s\" \"%s\\%s\"", caminho_origem, pasta_destino, nome_arquivo);

    printf("Executando: %s\n", comando_copia);
    int resultado = system(comando_copia);

    if (resultado == 0)
    {
        printf("Download concluido: %s\\%s\n", pasta_destino, nome_arquivo);
        return 0;
    }
    else
    {
        printf("Erro no download do arquivo: %s\n", nome_arquivo);
        return -1;
    }
}

// Menu para download de conteudos
void menuDownloadConteudos(const ListaConteudos *lista)
{
    printf("\n=== DOWNLOAD DE CONTEUDOS ===\n");

    if (lista->num_conteudos == 0)
    {
        printf("Nenhum conteudo disponivel para download.\n");
        return;
    }

    // Obter pasta Documentos
    char *pasta_documentos = obterPastaDocumentos();
    char pasta_destino[MAX_CAMINHO];
    snprintf(pasta_destino, sizeof(pasta_destino), "%s\\ConteudosEscolares", pasta_documentos);

    printf("Pasta de destino: %s\n", pasta_destino);

    int opcao;
    do
    {
        printf("\n--- CONTEUDOS DISPONIVEIS ---\n");
        exibirConteudos(lista);

        printf("\nOpcoes de Download:\n");
        printf("1. Baixar conteudo especifico\n");
        printf("2. Baixar todos os conteudos\n");
        printf("3. Baixar por disciplina\n");
        printf("0. Voltar ao menu anterior\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferConteudo();

        switch (opcao)
        {
        case 1:
        {
            // Baixar conteudo especifico
            int indice;
            printf("Digite o numero do conteudo para baixar: ");
            scanf("%d", &indice);
            limparBufferConteudo();

            if (indice >= 1 && indice <= lista->num_conteudos)
            {
                const Conteudo *cont = &lista->conteudos[indice - 1];
                printf("Baixando: %s - %s\n", cont->disciplina, cont->nome_arquivo);

                if (baixarConteudo(cont->disciplina, cont->nome_arquivo, pasta_destino) == 0)
                {
                    printf("Download realizado com sucesso!\n");
                }
            }
            else
            {
                printf("Numero de conteudo invalido!\n");
            }
            break;
        }

        case 2:
        {
            // Baixar todos os conteudos
            printf("Iniciando download de todos os conteudos...\n");
            int sucessos = 0;

            for (int i = 0; i < lista->num_conteudos; i++)
            {
                const Conteudo *cont = &lista->conteudos[i];
                printf("Baixando %d/%d: %s... ", i + 1, lista->num_conteudos, cont->nome_arquivo);

                if (baixarConteudo(cont->disciplina, cont->nome_arquivo, pasta_destino) == 0)
                {
                    sucessos++;
                    printf("OK\n");
                }
                else
                {
                    printf("FALHA\n");
                }
            }

            printf("Download concluido: %d/%d arquivos baixados com sucesso.\n", sucessos, lista->num_conteudos);
            break;
        }

        case 3:
        {
            // Baixar por disciplina
            char disciplina[MAX_DISCIPLINA];
            printf("Digite o nome da disciplina: ");
            fgets(disciplina, MAX_DISCIPLINA, stdin);
            disciplina[strcspn(disciplina, "\n")] = 0;

            printf("Buscando conteudos da disciplina: %s\n", disciplina);
            int encontrados = 0;
            int sucessos = 0;

            for (int i = 0; i < lista->num_conteudos; i++)
            {
                const Conteudo *cont = &lista->conteudos[i];
                if (strcmp(cont->disciplina, disciplina) == 0)
                {
                    encontrados++;
                    printf("Baixando: %s... ", cont->nome_arquivo);

                    if (baixarConteudo(cont->disciplina, cont->nome_arquivo, pasta_destino) == 0)
                    {
                        sucessos++;
                        printf("OK\n");
                    }
                    else
                    {
                        printf("FALHA\n");
                    }
                }
            }

            if (encontrados == 0)
            {
                printf("Nenhum conteudo encontrado para a disciplina: %s\n", disciplina);
            }
            else
            {
                printf("Download concluido: %d/%d arquivos baixados com sucesso.\n", sucessos, encontrados);
            }
            break;
        }

        case 0:
            printf("Voltando ao menu anterior...\n");
            break;

        default:
            printf("Opcao invalida!\n");
            break;
        }

        if (opcao != 0)
        {
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (opcao != 0);
}