#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/github.h"
#include <unistd.h>

void makePushToTheRepo(const char *branch, const char *commitMessage, const char *filePath)
{
    char command[1024];

    if (access(filePath, F_OK) != 0)
    {
        printf("❌ Ficheiro não encontrado: %s\n", filePath);
        return;
    }

    // Criar ou mudar para a branch
    snprintf(command, sizeof(command), "git checkout -B %s", branch);
    system(command);

    // Criar pasta dentro do repo
    snprintf(command, sizeof(command), "mkdir -p src/datas/assets/uploads/%s", branch);
    system(command);

    // Nome do ficheiro
    char *filename = strrchr(filePath, '/');
    filename = filename ? filename + 1 : (char *)filePath;

    // Copiar ficheiro
    snprintf(command, sizeof(command),
             "cp \"%s\" src/datas/assets/uploads/%s/%s",
             filePath, branch, filename);
    system(command);

    // Git add
    snprintf(command, sizeof(command),
             "git add src/datas/assets/uploads/%s/%s",
             branch, filename);
    system(command);

    // Commit
    snprintf(command, sizeof(command), "git commit -m \"%s\"", commitMessage);
    system(command);

    // Push (criando branch remota)
    snprintf(command, sizeof(command), "git push -u origin %s", branch);
    system(command);

    printf("✅ Conteúdo enviado com sucesso para a branch %s!\n", branch);
}



void downloadTopics(const char *branch)
{
    char command[1024];

    // Pegar diretório HOME do utilizador (cross-platform)
    const char *home =
    #ifdef _WIN32
        getenv("USERPROFILE");
    #else
        getenv("HOME");
    #endif

    if (!home)
    {
        printf("❌ Não foi possível localizar o diretório do utilizador.\n");
        return;
    }

    printf("🔄 Baixando conteúdos da cadeira de %s (somente Topics/%s)...\n", branch, branch);

    // Clonar a branch específica sem checkout completo
    snprintf(command, sizeof(command),
             "git clone --no-checkout --depth 1 --branch %s https://github.com/Emaricarprogrammer/Projecto-Mini-CLI.git tmp_repo", branch);
    if (system(command) != 0)
    {
        printf("❌ Erro ao clonar repositório.\n");
        return;
    }

    // Configurar sparse-checkout para trazer apenas a pasta Topics da branch
    snprintf(command, sizeof(command),
             "cd tmp_repo && git sparse-checkout init --cone && git sparse-checkout set src/Topics/%s && git checkout", branch);
    if (system(command) != 0)
    {
        printf("❌ Erro ao configurar sparse checkout.\n");
        system("rm -rf tmp_repo"); // Limpar mesmo em caso de erro
        return;
    }

    // Criar pasta destino dentro do HOME
    snprintf(command, sizeof(command),
             "mkdir -p \"%s/GestaoTurma/Topics\"", home);
    system(command);

    // Copiar os arquivos da pasta Topics da branch para a pasta destino
    snprintf(command, sizeof(command),
             "cp -r tmp_repo/src/Topics/%s \"%s/GestaoTurma/Topics/\"", branch, home);
    if (system(command) != 0)
    {
        printf("⚠️ Falha ao copiar os tópicos para \"%s/GestaoTurma/Topics/\"\n", home);
        system("rm -rf tmp_repo"); // Limpar tmp_repo
        return;
    }

    // Limpar a pasta temporária
    snprintf(command, sizeof(command), "rm -rf tmp_repo");
    system(command);

    printf("✅ Conteúdo de Topics/%s baixado com sucesso em: %s/GestaoTurma/Topics/\n", branch, home);
}
