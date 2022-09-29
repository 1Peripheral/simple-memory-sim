#include <stdio.h>
#include <stdlib.h>
#include "../headers/allocation.h"
#include "../headers/queue_process.h"
#include "../headers/types.h"
#include "../headers/color_code.h"

void clr() {
    system("cls || clear");
}

int menu() {
    int choix;
    system("cls || clear");
    printf(CYAN);
    printf("\n\t|   MENU   |\n\n\n");
    printf(YELLOW);
    printf("  1/- Data \n\n");
    printf("  2/- Algorithm : First Fit \n\n");
    printf("  3/- Algorithm : Best Fit \n\n");
    printf("  4/- Algorithm : Worst Fit \n\n");
    printf("  0/- Quit  \n\n");
    printf(GREEN);

    printf("\n   Choice "); 
    printf(RESET);
    scanf("%d", &choix);
    return choix;
}

void main() {
    ListBloc listBloc;
    ListProcess listProcess, queueProcess;
    int ramsize=0, choix;
    char directory[128];
    listProcess.head = NULL;
    queueProcess.head = NULL;

    do {
        choix = menu();
        printf(YELLOW);

        switch(choix) {
            case 0: clr();
                    break;

            case 1: clr();
                    printf("  Enter the memory size (KB) : "); scanf("%d", &ramsize);
                    printf("  Enter the file path : "); scanf("%s", directory);
                    listProcess = loadFromFile(directory);
                    if(!checkCompatibility(&listProcess, ramsize)) {
                        printf(RED);
                        printf(" Memory size given is not enough !\n\n");
                        system("pause");
                        break;
                    }
                    printf("\n\n");
                    printf(GREEN);
                    printf(" File Loaded .\n ");
                    printf(RESET);
                    system("pause");
                    break;

            case 2: clr();
                    if(ramsize == 0){
                        printf(RED);
                        printf(" Memory size and data file are not known !\n");
                        break;
                    }
                    initializeList(&listBloc, ramsize);
                    listProcess = loadFromFile(directory);
                    printAlgo(&listBloc, &listProcess, &queueProcess, choix-1);
                    freeListBloc(&listBloc);
                    printf("\n\n");
                    printf(RESET);
                    system("pause");
                    break;

            case 3: clr();
                    if(ramsize == 0){
                        printf(RED);
                        printf(" Memory size and data file are not known !\n");
                        printf("\n\n");
                        system("pause");
                        break;
                    }
                    initializeList(&listBloc, ramsize);
                    listProcess = loadFromFile(directory);
                    printAlgo(&listBloc, &listProcess, &queueProcess, choix-1);
                    freeListBloc(&listBloc);
                    printf("\n\n");
                    printf(RESET);
                    system("pause");
                    break;

            case 4: clr();
                    if(ramsize == 0){
                        printf(RED);
                        printf(" Memory size and data file are not known !\n");
                        printf("\n\n");
                        system("pause");
                        break;
                    }
                    initializeList(&listBloc, ramsize);
                    listProcess = loadFromFile(directory);
                    printAlgo(&listBloc, &listProcess, &queueProcess, choix-1);
                    freeListBloc(&listBloc);
                    printf("\n\n");
                    printf(RESET);
                    system("pause");
                    break;    
        }
    }while(choix);

    
    freeListProcess(&listProcess);
    freeListProcess(&queueProcess);
    freeListBloc(&listBloc);
}
