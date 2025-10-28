#include <stdio.h>
//#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

void requester(const char *requestType)
{
    
}
void makePushToTheRepo(const char *branch)
{
    printf("Enviando conteúdo para a cadeira de %s...\n", branch);
    system("git add ../Topics/*");
    system("git commit -a -m 'Atualização de conteúdos'");
    char pushCommand[100];
    snprintf(pushCommand, sizeof(pushCommand), "git push origin %s", branch);
    system(pushCommand);
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
            makePushToTheRepo("P1");       
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