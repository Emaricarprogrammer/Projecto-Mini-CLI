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

    do
    {
        printf("=================================");
        printf("\n\tGestão da Turma");
        printf("\n=================================\n");
        printf("\n 1 - Enviar conteúdos");
        printf("\n 2 - Baixar conteúdos");
        printf("\n 3 - Gerar grupos aleatórios");
        printf("\n 4 - Eleger delegado(a)");
        printf("\n 5 - Sair\n");
        printf("Sua opção: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            printf("=================================");
            printf("\n Cadeiras disponiveis ");
            printf("\n=================================\n");
            printf("\n 1- Programação 1");
            printf("\n 2- Comunicação escrita");
            printf("\n 3- Análise matemática");
            printf("\n 4- Lógica matemática");
            printf("\n 5- Inglês técnico");
            printf("\n 6- Mic");
            printf("\n 7- ICC");
            printf("\n 8 - Sair");

            printf("\nSua opção: ");
            scanf("%d", &branch);

            if (branch == 1)
            {
                printf("Digite o nome do ficheiro a enviar (teste.txt): ");
                scanf("%s", relativePath);

                snprintf(finalPath, sizeof(finalPath), "../Topics/P1/%s", relativePath);

                if (access(finalPath, F_OK) != 0)
                {
                    printf("❌ Ficheiro não encontrado: %s\n", finalPath);
                }
                else
                {
                    makePushToTheRepo("P1", "Novos conteudos", finalPath);
                }
            }
            else if (branch != 8)
            {
                printf("Opção inválida!\n");
            }
            break;

        case 2:
            printf("=================================");
            printf("\n Cadeiras disponiveis ");
            printf("\n=================================\n");
            printf("\n 1- Programação 1");
            printf("\n 2- Comunicação escrita");
            printf("\n 3- Análise matemática");
            printf("\n 4- Lógica matemática");
            printf("\n 5- Inglês técnico");
            printf("\n 6- Mic");
            printf("\n 7- ICC");
            printf("\n 8 - Sair\n");

            printf("\nSua opção: ");
            scanf("%d", &branch);

            if (branch == 1)
            {
                system("clear");
                downloadTopics("P1");
            }
            else if (branch != 8)
            {
                printf("Opção inválida!\n");
            }
            break;

        case 3:
            printf("Informe a quantidade de elementos para cada grupo: ");
            scanf("%d", &groupSize);
            generateRandomGroups("../../alunos.txt", groupSize);
            break;

        case 4:
            printf("Delegado(a) eleito(a) com sucesso!\n");
            break;

        case 5:
            printf("Saindo do programa...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }

        sleep(1);
        system("clear");

    } while (option != 5);

    return 0;
}
