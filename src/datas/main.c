#include <stdio.h>
//#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

void makePushToTheRepo(const char *branch, const char *commitMessage)
{
    char pushCommand[256];
    snprintf(pushCommand, sizeof(pushCommand), "git checkout -B %s", branch);
    if(system(pushCommand) != 0)
    {
        printf("Erro ao trocar para a branch %s\n", branch);
        return;
    }
    if(system("git add ../Topics/*") != 0)
    {
        printf("Erro ao adicionar arquivos.\n");
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
void downloadTopics(const char *courseName)
{
    printf("Conteúdos do curso %s foram baixados com sucesso!\n", courseName);
}
int main(void)
{
    int option;
    int branch;
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
            makePushToTheRepo("P1", "Novos conteudos");       
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