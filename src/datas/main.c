#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/github.h"
#include "../includes/generate-groups.h"
#include <unistd.h>
#include <time.h>

int main(void)
{
    int option;
    int branch;
    char relativePath[256];
    char finalPath[512];
    int groupSize;

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
        printf("\n 2- Comunicação escrita");
        printf("\n 3- Análise matemática\n");
        printf("\n 4- Lógica matemática\n");
        printf("\n 5- INglês técnico\n");
        printf("\n 6- Mic\n");
        printf("\n 7- ICC\n");
        printf("\n 8 - Sair\n");

        printf("Sua opção:");
        scanf("%d", &branch);

        if(branch == 1)
        {
            printf("=================================");
            printf("Digite o nome do ficheiro a enviar (teste.txt):");
            scanf("%s", relativePath);

            snprintf(finalPath, sizeof(finalPath), "../Topics/P1/%s", relativePath);
            if(access(finalPath, F_OK) != 0)
            {
                printf("❌ Ficheiro não encontrado no directorio: %s\n", finalPath);
                return 1;
            }
            makePushToTheRepo("P1", "Novos conteudos", finalPath);       
        }
        else
        {
            printf("Opção inválida!\n");
        }
    }
    else if (option == 2)
    {
        printf("=================================");
        printf("\n Cadeiras disponiveis ");
        printf("\n=================================\n");
        printf("\n 1- Programação 1");
        printf("\n 2- Comunicação escrita");
        printf("\n 3- Análise matemática\n");
        printf("\n 4- Lógica matemática\n");
        printf("\n 5- INglês técnico\n");
        printf("\n 6- Mic\n");
        printf("\n 7- ICC\n");
        printf("\n 8 - Sair\n");

        printf("\nSua opção:\n");
        scanf("%d", &branch);
        if(branch == 1)
        {
            system("clear");
            printf("=================================");
            downloadTopics("P1");
        }
        else
        {
            printf("Opção inválida!\n");
        }
    }
    else if (option == 3)
    {
        system("clear");
        printf("=================================\n");
        printf("Informe a quantidade de elementos para cada grupo: ");
        scanf("%d", &groupSize);
        generateRandomGroups("../../alunos.txt", groupSize);
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