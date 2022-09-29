#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/types.h"
#include "../headers/queue_process.h"
#include "../headers/allocation.h"
#include "../headers/color_code.h"

#define MAX_ARRAY_SIZE 32
int cpt_compactage = 0;


ListProcess loadFromFile(char *dest) {
    FILE *fp = fopen(dest, "r");
    if(!fp) {
        printf(RED);
        printf("File Not Found ! \n");
        exit(EXIT_FAILURE);
        }

    ListProcess list;
    list.head = NULL;
    Process p;
    char tmp[4]; 

    while( fscanf(fp, "%c%d%3s", &p.status, &p.size, tmp)) 
    {
        pushLastProcess(&list, p);
        if (tmp[2] != ',') break;
    }
    fclose(fp);

    return list;
}

Node* createNode(int debut, int fin, char etat) {
    Node* node = malloc(sizeof(Node));
    Bloc* bloc_ = malloc(sizeof(Bloc));

    bloc_->start = debut;
    bloc_->end = fin;
    bloc_->state = etat;

    node->bloc = bloc_;
    node->next = NULL;

    return node;
}

void initializeList(ListBloc* list, int ram_size) {
    Node* node = createNode(0, ram_size, 'f');

    list->head = node;
    list->nbr_proc = 0;
}

int allocAlgorithm(ListBloc* list, const Process *p, int choice) {
    Node *current = list->head, *previous = NULL, *tmp = NULL, *fit_prev = NULL;
    
    if (!current) exit(EXIT_FAILURE);

    switch(choice) {
     
        case 1:   
            while(current && (current->bloc->state == 'o' || (current->bloc->end - current->bloc->start) < p->size)) { 
                previous = current;    
                current = current->next;
           } break;
        
        case 2:
            while(current) {
                if((current->bloc->state == 'f') && ((current->bloc->end - current->bloc->start) >= p->size))  {
                    if (!tmp){
                        fit_prev = previous;
                        tmp = current;
                    }
                    else if((current->bloc->end - current->bloc->start) < (tmp->bloc->end - tmp->bloc->start)){
                        tmp = current;
                        fit_prev = previous;
                    }
                }
                previous = current;
                current = current->next;
            } 
            if (tmp) {
                current = tmp;
                previous = fit_prev;    
            }
            break;  
        case 3: 
            while(current) {
                if((current->bloc->state == 'f') && ((current->bloc->end - current->bloc->start) >= p->size))  {
                    if (!tmp){
                        fit_prev = previous;
                        tmp = current;
                    }
                    else if((current->bloc->end - current->bloc->start) > (tmp->bloc->end - tmp->bloc->start)){
                        tmp = current;
                        fit_prev = previous;
                    }
                }
                previous = current;
                current = current->next;
            } 
            if (tmp) {
                current = tmp;
                previous = fit_prev;    
            }
            break;

        default: 
            exit(EXIT_FAILURE);
    }

    if (current && current->bloc->state == 'f') {
        if (p->size == current->bloc->end - current->bloc->start) {
            current->bloc->state = 'o';
        }

        else {
            Node* new = createNode(current->bloc->start, current->bloc->start + p->size, 'o');
            current->bloc->start = new->bloc->end;
            
            if(previous == NULL){
                new->next = current;
                list->head = new;
            }
            else {
                new->next = current;
                previous->next = new;
            }
        }
        list->nbr_proc++;
        return 1;
    }   
    return 0;
}   

void Fusion(ListBloc* list){
    Node* current = list->head;
    if(!list->head) exit(EXIT_FAILURE);

    while(current) {
        if (current->next && current->bloc->state == 'f') {
            Node *tmp = NULL, *tofree;
            if(current->next) tmp = current->next;
            while(tmp && tmp->bloc->state == 'f') {
                current->bloc->end = tmp->bloc->end;
                current->next = tmp->next;
                tofree = tmp;
                tmp = tmp->next;
                free(tofree->bloc);
                free(tofree);
            }
        }
        current = current->next;
    }
    if(current == list->head) list->head = current;
}

// Return a value to check if the memory has been modified or not.
int checkAllocation(ListBloc* list, ListProcess* queuep, ListProcess* p, int choice) {
    if(p->head == NULL) exit(EXIT_FAILURE);

    if (p->head->process.status == '+') {
        if( allocAlgorithm(list,  &p->head->process, choice) )
            printProcessStatus(&p->head->process);

        else {
            if(Compactage(list)){
                printf(RED);
                printf("\tCompaction Done(N %d) !\n", cpt_compactage);
            }
            if(allocAlgorithm(list, &p->head->process, choice))
                printProcessStatus(&p->head->process); 
            else {  
                printf(CYAN);
                printf("P(%d Ko) Has Been Queued .\n\n", p->head->process.size);
                printf(YELLOW);
                pushProcessToQueue(queuep, p->head->process);
            }
        }
    
        popProcess(p);
    }

    else {
        Node* current = list->head;
        while(current && ( (current->bloc->end - current->bloc->start) != p->head->process.size  || current->bloc->state == 'f'))
            current = current->next;

        if (current && (current->bloc->end - current->bloc->start) == p->head->process.size && (current->bloc->state = 'o')) {
            current->bloc->state = 'f'; 
            printProcessStatus(&p->head->process);
            popProcess(p);
            Fusion(list);
        }

        else if (p->head->next) {
            do{
                pushLastProcess(p, p->head->process);
                popProcess(p);
            }while(p->head && p->head->process.status == '+');
            return 0;
            
        }

        if (queuep->head){
            if(allocAlgorithm(list, &queuep->head->process, choice)){
                printProcessStatus(&queuep->head->process);
                popProcess(queuep);     
            }
        }
    }
    return 1;
}

int checkCompatibility(ListProcess* listp, int ram_size) {
    if (!listp->head) exit(EXIT_FAILURE);

    Node_p* current = listp->head;

    while(current) {
        if(current->process.size > ram_size) return 0;
        current = current->next;
    }

    return 1;
}

int Compactage(ListBloc* list) {
    if(!list->head) exit(EXIT_FAILURE);

    Node *current = list->head, *previous = NULL, *toFree;
    int totalSize = 0, saveStart = 0, compa_effectuer = 0, cpt_occu=0;

    while(current) {
        if(current->bloc->state == 'f') {
            toFree = current;
            totalSize += (current->bloc->end - current->bloc->start);
            if (previous == NULL) {
                list->head = current = list->head->next;
            }
            else {
                previous->next = current->next;
                current = current->next;
            }
            free(toFree->bloc);
            free(toFree);
            cpt_occu++;
            continue;
        }
        else {
            current->bloc->end = (current->bloc->end - current->bloc->start) + saveStart; 
            current->bloc->start = saveStart;
            saveStart = current->bloc->end;
        }
        previous = current;
        current = current->next;
    }

    if(previous && totalSize > 0) {
        Node* new = createNode(saveStart, saveStart+totalSize, 'f');
        if(cpt_occu >=2 ){
            compa_effectuer = 1;
            cpt_compactage++; 
        }  
        new->next = NULL;
        previous->next = new;
    }
    return compa_effectuer;
}

void  printListBloc(ListBloc* list) {
   Node* current = list->head;

    printf("   STATE   |   START   |   END   |  SIZE     |\n");
    while(current){
        printf("%6c     |%11d|%9d|%9d  |\n", 
                    toupper(current->bloc->state),
                    current->bloc->start,
                    current->bloc->end,
                    (current->bloc->end-current->bloc->start));
        current = current->next;
   } 
   printf("----------------------------------------------\n\n\n\n");
}

void freeListBloc(ListBloc* list) {
    Node* tmp;

    while(list->head) {
        tmp = list->head;
        list->head = list->head->next;
        free(tmp->bloc);
        free(tmp);        
    }
    list->nbr_proc = 0;
}

void printProcessStatus(Process *p) {
    printf(RESET);
    if(p->status == '-'){
        printf(GREEN);
        printf("\tFreed  ");
        printf(RESET);
    }

    else{
        printf(BLUE);
        printf("\tLoading ");
        printf(RESET);
    }
    printf(" P(%d Ko) .\n", p->size);
    printf(YELLOW);
}

void printAlgo(ListBloc* listBloc, ListProcess* listProc, ListProcess* queueProc, int choice){
    printf(RESET);
    printf("\n\tInitial State Of The Memory .\n");
    printListBloc(listBloc);
    while(listProc->head) {
        if(checkAllocation(listBloc, queueProc, listProc, choice)){
            printListBloc(listBloc);
        }
        printf(RESET);
    }
    cpt_compactage = 0;
}

