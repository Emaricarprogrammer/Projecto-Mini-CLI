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

    snprintf(pasta_disciplina, sizeof(pasta_disciplina), "data\\conteudos\\%s", disciplina);

    system("mkdir data\\conteudos 2>nul");

    char comando_mkdir[MAX_CAMINHO];
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_disciplina);
    system(comando_mkdir);

    printf("Tentando copiar: %s\n", caminho_origem);

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

    const char *nome_arquivo = strrchr(caminho_local, '\\');
    if (nome_arquivo == NULL)
    {
        nome_arquivo = strrchr(caminho_local, '/');
    }
    if (nome_arquivo != NULL)
    {
        nome_arquivo++;
    }
    else
    {
        nome_arquivo = caminho_local;
    }

    printf("Arquivo detectado: %s\n", nome_arquivo);

    const char *extensao = strrchr(nome_arquivo, '.');
    if (extensao == NULL || (strcasecmp(extensao, ".pdf") != 0 && strcasecmp(extensao, ".txt") != 0))
    {
        printf("Aviso: O arquivo nao e PDF ou TXT. Continuando mesmo assim...\n");
    }

    Conteudo *novo_conteudo = &lista->conteudos[lista->num_conteudos];

    printf("Copiando arquivo para o repositorio...\n");
    if (copiarArquivoParaRepositorio(caminho_local, nome_arquivo, disciplina) != 0)
    {
        printf("Falha ao copiar arquivo. Tentando metodo alternativo...\n");

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

    strncpy(novo_conteudo->nome_arquivo, nome_arquivo, MAX_NOME_ARQUIVO - 1);
    novo_conteudo->nome_arquivo[MAX_NOME_ARQUIVO - 1] = '\0';

    strncpy(novo_conteudo->caminho_local, caminho_local, MAX_CAMINHO - 1);
    novo_conteudo->caminho_local[MAX_CAMINHO - 1] = '\0';

    strncpy(novo_conteudo->descricao, descricao, MAX_DESCRICAO - 1);
    novo_conteudo->descricao[MAX_DESCRICAO - 1] = '\0';

    strncpy(novo_conteudo->disciplina, disciplina, MAX_DISCIPLINA - 1);
    novo_conteudo->disciplina[MAX_DISCIPLINA - 1] = '\0';

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

    if (executarComandoGit("git status") != 0)
    {
        printf("Inicializando repositorio Git...\n");

        if (executarComandoGit("git init") != 0)
        {
            printf("Erro ao inicializar repositorio Git\n");
            return -1;
        }

        char comando_remote[MAX_CAMINHO];
        snprintf(comando_remote, sizeof(comando_remote),
                 "git remote add origin https://github.com/Emaricarprogrammer/Projecto-Mini-CLI.git");
        if (executarComandoGit(comando_remote) != 0)
        {
            printf("Erro ao adicionar remote origin\n");
            return -1;
        }
    }

    printf("Verificando branch arturDev929...\n");
    if (executarComandoGit("git branch -a | findstr arturDev929") != 0)
    {
        printf("Branch arturDev929 nao encontrada. Criando nova branch...\n");

        if (executarComandoGit("git checkout -b arturDev929") != 0)
        {
            printf("Erro ao criar branch arturDev929\n");
            return -1;
        }
    }
    else
    {
        printf("Mudando para branch arturDev929...\n");
        if (executarComandoGit("git checkout arturDev929") != 0)
        {
            printf("Erro ao mudar para branch arturDev929\n");
            return -1;
        }
    }

    if (executarComandoGit("git add .") != 0)
    {
        printf("Erro ao adicionar arquivos\n");
        return -1;
    }

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

    printf("Fazendo push para a branch arturDev929 no GitHub...\n");

    if (executarComandoGit("git push -u origin arturDev929") != 0)
    {
        printf("Tentando push com force...\n");
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

void listarConteudosParaDownload(const ListaConteudos *lista)
{
    if (lista->num_conteudos == 0)
    {
        printf("Nenhum conteudo disponivel para download.\n");
        return;
    }

    printf("\n=== CONTEUDOS DISPONIVEIS PARA DOWNLOAD ===\n");
    printf("%-3s %-20s %-25s %-30s %-15s\n", "ID", "Disciplina", "Arquivo", "Descricao", "Data");
    printf("-----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < lista->num_conteudos; i++)
    {
        const Conteudo *cont = &lista->conteudos[i];
        printf("%-3d %-20s %-25s %-30s %-15s\n",
               i + 1, cont->disciplina, cont->nome_arquivo, cont->descricao, cont->data_upload);
    }
}

int baixarConteudo(const ListaConteudos *lista, int id_conteudo, const char *pasta_destino)
{
    if (id_conteudo < 1 || id_conteudo > lista->num_conteudos)
    {
        printf("ID de conteudo invalido!\n");
        return -1;
    }

    const Conteudo *conteudo = &lista->conteudos[id_conteudo - 1];

    char caminho_origem[MAX_CAMINHO];
    snprintf(caminho_origem, sizeof(caminho_origem),
             "data\\conteudos\\%s\\%s",
             conteudo->disciplina,
             conteudo->nome_arquivo);

    FILE *arquivo = fopen(caminho_origem, "rb");
    if (arquivo == NULL)
    {
        printf("Erro: Arquivo nao encontrado no repositorio: %s\n", caminho_origem);
        printf("O conteudo pode nao ter sido copiado corretamente.\n");
        return -2;
    }
    fclose(arquivo);

    char comando_mkdir[MAX_CAMINHO];
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_destino);
    system(comando_mkdir);

    char caminho_destino[MAX_CAMINHO];
    snprintf(caminho_destino, sizeof(caminho_destino),
             "%s\\%s", pasta_destino, conteudo->nome_arquivo);

    char comando_copia[MAX_CAMINHO * 2];
    snprintf(comando_copia, sizeof(comando_copia),
             "copy \"%s\" \"%s\"", caminho_origem, caminho_destino);

    printf("Baixando: %s\n", conteudo->nome_arquivo);
    printf("De: %s\n", caminho_origem);
    printf("Para: %s\n", caminho_destino);

    int resultado = system(comando_copia);

    if (resultado == 0)
    {
        printf("Download concluido com sucesso!\n");
        printf("Arquivo salvo em: %s\n", caminho_destino);
        return 0;
    }
    else
    {
        printf("Erro ao baixar o arquivo.\n");

        printf("Tentando metodo alternativo...\n");
        snprintf(comando_copia, sizeof(comando_copia),
                 "xcopy \"%s\" \"%s\" /Y", caminho_origem, pasta_destino);
        resultado = system(comando_copia);

        if (resultado == 0)
        {
            printf("Download concluido com metodo alternativo!\n");
            return 0;
        }
        else
        {
            printf("Falha no download. Tente manualmente.\n");
            return -3;
        }
    }
}

int baixarConteudosPorDisciplina(const ListaConteudos *lista, const char *disciplina, const char *pasta_destino)
{
    int downloads_realizados = 0;

    char pasta_disciplina[MAX_CAMINHO];
    snprintf(pasta_disciplina, sizeof(pasta_disciplina), "%s\\%s", pasta_destino, disciplina);

    char comando_mkdir[MAX_CAMINHO];
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_disciplina);
    system(comando_mkdir);

    printf("\n=== BAIXANDO CONTEUDOS DE: %s ===\n", disciplina);

    for (int i = 0; i < lista->num_conteudos; i++)
    {
        const Conteudo *conteudo = &lista->conteudos[i];

        if (strcmp(conteudo->disciplina, disciplina) == 0)
        {
            printf("\n[%d/%d] %s...\n", i + 1, lista->num_conteudos, conteudo->nome_arquivo);

            if (baixarConteudo(lista, i + 1, pasta_disciplina) == 0)
            {
                downloads_realizados++;
            }
        }
    }

    printf("\nDownload de %s concluido: %d arquivos baixados\n", disciplina, downloads_realizados);
    return downloads_realizados;
}

int baixarConteudoDoGitHub(const char *disciplina, const char *nome_arquivo, const char *pasta_destino)
{
    printf("\n=== BAIXANDO DO GITHUB ===\n");

    char url_github[MAX_CAMINHO];
    snprintf(url_github, sizeof(url_github),
             "https://raw.githubusercontent.com/Emaricarprogrammer/Projecto-Mini-CLI/arturDev929/data/conteudos/%s/%s",
             disciplina, nome_arquivo);

    printf("URL: %s\n", url_github);

    char comando_mkdir[MAX_CAMINHO];
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_destino);
    system(comando_mkdir);

    char pasta_disciplina[MAX_CAMINHO];
    snprintf(pasta_disciplina, sizeof(pasta_disciplina), "%s\\%s", pasta_destino, disciplina);
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_disciplina);
    system(comando_mkdir);

    char caminho_destino[MAX_CAMINHO];
    snprintf(caminho_destino, sizeof(caminho_destino), "%s\\%s", pasta_disciplina, nome_arquivo);

    char comando_curl[MAX_CAMINHO * 2];
    snprintf(comando_curl, sizeof(comando_curl),
             "curl -L -o \"%s\" \"%s\"", caminho_destino, url_github);

    printf("Baixando: %s\n", nome_arquivo);
    printf("Disciplina: %s\n", disciplina);
    printf("Para: %s\n", caminho_destino);

    int resultado = system(comando_curl);

    if (resultado == 0)
    {
        FILE *arquivo = fopen(caminho_destino, "rb");
        if (arquivo != NULL)
        {
            fclose(arquivo);
            printf("Download concluido com sucesso!\n");
            printf("Arquivo salvo em: %s\n", caminho_destino);
            return 0;
        }
        else
        {
            printf("Erro: Arquivo nao foi baixado corretamente.\n");
            return -1;
        }
    }
    else
    {
        printf("Erro ao baixar com curl. Tentando com powershell...\n");

        char comando_ps[MAX_CAMINHO * 2];
        snprintf(comando_ps, sizeof(comando_ps),
                 "powershell -Command \"Invoke-WebRequest -Uri '%s' -OutFile '%s'\"",
                 url_github, caminho_destino);

        resultado = system(comando_ps);

        if (resultado == 0)
        {
            FILE *arquivo = fopen(caminho_destino, "rb");
            if (arquivo != NULL)
            {
                fclose(arquivo);
                printf("Download concluido com PowerShell!\n");
                printf("Arquivo salvo em: %s\n", caminho_destino);
                return 0;
            }
        }

        printf("Falha no download. Verifique:\n");
        printf("1. Conexao com internet\n");
        printf("2. Se o arquivo existe no GitHub\n");
        printf("3. URL: %s\n", url_github);
        return -2;
    }
}

int listarConteudosDoGitHub()
{
    printf("\n=== LISTANDO CONTEUDOS DO GITHUB ===\n");

    printf("Repositorio: https://github.com/Emaricarprogrammer/Projecto-Mini-CLI/tree/arturDev929/data/conteudos\n");
    printf("Estrutura esperada:\n");

    const char *disciplinas[] = {
        "Programacao I", "Analise Matematica", "Logica Matematica",
        "Ingles Tecnico", "Metodologia de Investigacao Cientifica",
        "Comunicacao Escrita", "Introducao a Ciencias da Computacao"};

    printf("\n=== ESTRUTURA DE PASTAS NO GITHUB ===\n");

    for (int i = 0; i < 7; i++)
    {
        printf("\n--- %s ---\n", disciplinas[i]);
        printf("URL: https://github.com/Emaricarprogrammer/Projecto-Mini-CLI/tree/arturDev929/data/conteudos/%s\n", disciplinas[i]);
        printf("Arquivos disponiveis nesta pasta:\n");
        printf("  (Para ver os arquivos, visite o link acima)\n");
    }

    printf("\n=== INSTRUCOES PARA DOWNLOAD ===\n");
    printf("1. Use a opcao 2 para baixar arquivo especifico\n");
    printf("2. Digite exatamente o nome do arquivo como aparece no GitHub\n");
    printf("3. Exemplo: se o arquivo for 'lista_exercicios.pdf', digite exatamente isso\n");

    return 1;
}

int baixarTodosConteudosGitHub(const char *pasta_destino)
{
    printf("\n=== BAIXAR TODOS OS CONTEUDOS DO GITHUB ===\n");
    printf("AVISO: Esta funcao requer que voce saiba os nomes exatos dos arquivos.\n");
    printf("Use primeiro a opcao 1 para ver a estrutura, depois a opcao 2 para baixar arquivos especificos.\n");

    char comando_mkdir[MAX_CAMINHO];
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_destino);
    system(comando_mkdir);

    printf("Pasta de destino criada: %s\n", pasta_destino);
    printf("Agora use a opcao 2 para baixar arquivos especificos.\n");

    return 0;
}

int baixarDisciplinaGitHub(const char *disciplina, const char *pasta_destino)
{
    printf("\n=== BAIXAR DISCIPLINA: %s ===\n", disciplina);
    printf("AVISO: Esta funcao requer que voce saiba os nomes dos arquivos.\n");

    char comando_mkdir[MAX_CAMINHO];
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_destino);
    system(comando_mkdir);

    char pasta_disciplina[MAX_CAMINHO];
    snprintf(pasta_disciplina, sizeof(pasta_disciplina), "%s\\%s", pasta_destino, disciplina);
    snprintf(comando_mkdir, sizeof(comando_mkdir), "mkdir \"%s\" 2>nul", pasta_disciplina);
    system(comando_mkdir);

    printf("Pasta criada: %s\n", pasta_disciplina);
    printf("Para baixar arquivos, use a opcao 2 e especifique:\n");
    printf("- Disciplina: %s\n", disciplina);
    printf("- Nome do arquivo (exato como aparece no GitHub)\n");

    return 0;
}

void listarDisciplinasGitHub()
{
    printf("\n=== DISCIPLINAS DISPONIVEIS NO GITHUB ===\n");

    const char *disciplinas[] = {
        "Programacao I",
        "Analise Matematica",
        "Logica Matematica",
        "Ingles Tecnico",
        "Metodologia de Investigacao Cientifica",
        "Comunicacao Escrita",
        "Introducao a Ciencias da Computacao"};

    for (int i = 0; i < 7; i++)
    {
        printf("%d. %s\n", i + 1, disciplinas[i]);
        printf("   URL: https://github.com/Emaricarprogrammer/Projecto-Mini-CLI/tree/arturDev929/data/conteudos/%s\n", disciplinas[i]);
    }
}