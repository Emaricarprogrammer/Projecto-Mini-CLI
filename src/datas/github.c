#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/github.h"
#include <unistd.h>

void makePushToTheRepo(const char *branch, const char *commitMessage, const char *filePath)
{
    char pushCommand[256];
    snprintf(pushCommand, sizeof(pushCommand), "git checkout %s 2>/dev/null || git checkout -b %s", branch, branch);
    if(system(pushCommand) != 0)
    {
        printf("Erro ao trocar/criar para a branch %s\n", branch);
        return;
    }

    if(filePath != NULL && strlen(filePath) > 0)
    {
        snprintf(pushCommand, sizeof(pushCommand), "git add %s", filePath);
    }
    else
    {
        snprintf(pushCommand, sizeof(pushCommand), "git add .");
    }
    if(system(pushCommand) != 0)
    {
        printf("❌ Erro ao adicionar arquivos (%s)\n", filePath ? filePath : ".");
        return;
    }
    if(system(pushCommand) != 0)
    {
        printf("Erro ao atualizar a branch %s\n", branch);
        return;
    }

    snprintf(pushCommand, sizeof(pushCommand), "git commit -a -m '%s'", commitMessage);
    if(system(pushCommand) != 0)
    {
        printf("Erro ao commitar os conteudos.\n");
        return;
    }
    printf("Enviando conteúdo para a cadeira de %s...\n", branch);
    snprintf(pushCommand, sizeof(pushCommand), "git push origin %s", branch);
    if(system(pushCommand) != 0)
        {
            printf("Erro ao enviar os conteudos para a branch %s. \n", branch);
            return;   
        }
        printf("Conteúdo enviado com sucesso para a cadeira de %s!\n", branch);
}

void downloadTopics(const char *branch)
{
    char command[512];

    printf("Baixando conteúdos da cadeira de %s (somente Topics/%s)...\n", branch, branch);

    snprintf(command, sizeof(command),
    "git clone --no-checkout --depth 1 --branch %s https://github.com/Emaricarprogrammer/Projecto-Mini-CLI.git tmp_repo", branch);
    
    if (system(command) != 0)
    {
        printf("Erro ao clonar repositório.\n");
        return;
    }
    snprintf(command, sizeof(command),
        "cd tmp_repo && git sparse-checkout init --cone && git sparse-checkout set src/Topics/%s && git checkout", branch);
    if (system(command) != 0)
    {
        printf("❌ Erro ao configurar sparse checkout.\n");
        return;
    }
    snprintf(command, sizeof(command),
        "mkdir -p ./Topics && cp -r tmp_repo/src/Topics/%s ./Topics/ && rm -rf tmp_repo", branch);
    if (system(command) != 0)
    {
        printf("⚠️  Falha ao mover pasta para raiz do projeto.\n");
        return;
    }
    printf("Conteúdo de Topics/P1 baixado com sucesso!\n");
}
