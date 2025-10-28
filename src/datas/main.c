#include <stdio.h>
//#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

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

void downloadTopics(const char *branchName)
{
    printf("Conteúdos do curso %s foram baixados com sucesso!\n", branchName);
}
int main(void)
{
    int option;
    int branch;
    char relativePath[100];
    printf("=================================");
    printf("\n\tGestão da Turma");
    printf("\n=================================\n");
    printf("\n 1 - Enviar conteúdos");
    printf("\n 2 - Baixar de conteúdos");
    printf("\n 3 - Gerar grupos aleatórios");
    printf("\n 4 - Eleger delegado(a)");
    printf("\n 5 - Sair\n");
    printf("sua opção:");
    scanf("%d", &option);

    if (option == 1)
    {
        printf("=================================");
        printf("\n Cadeiras disponiveis ");
        printf("\n=================================\n");
        printf("\n 1- Programação 1");
        printf("\n 2 - Sair\n");

        printf("Sua opção:");
        scanf("%d", &branch);

        if(branch == 1)
        {
            printf("Digite o caminho relativo do ficheiro a enviar (../Topics/P1/teste.txt):");
            scanf("%s", relativePath);

            makePushToTheRepo("P1", "Novos conteudos", relativePath);       
        }
        else
        {
            printf("Opção inválida!\n");
        }
    }
    else if (option == 2)
    {
        char courseName[50];
        printf("Digite o nome do curso:");
        scanf("%s", courseName);
        downloadTopics(courseName);
    }
    else if (option == 3)
    {
        printf("Grupos aleatórios gerados com sucesso!\n");
    }
    else if (option == 4)
    {
        printf("Delegado(a) eleito(a) com sucesso!\n");
    }
    else if (option == 5)
    {
        printf("Saindo do programa...\n");
    }
    else
    {
        printf("Opção inválida!\n");
    }
    return 0;
}