#include <stdio.h>
#include <stdlib.h>
#include "../headers/types.h"
#include "../headers/queue_process.h"

void pushProcessToQueue(ListProcess *list, Process p) {
    Node_p *current = list->head, *previous = NULL;
    Node_p* new = malloc(sizeof(Node_p));

    new->process = p;
    new->next = NULL;

    while(current && current->process.size < p.size) {
        previous = current;
        current = current->next;
    }

    if (!previous) {
        new->next = list->head;
        list->head = new;
    }

    else if (!current) {
        previous->next = new;
    }

    else if (current->process.size < p.size) {
        previous->next = new;
        new->next = current;
    }
}

void popProcess(ListProcess* list) {
    if(list->head) {;
        Node_p *current = list->head;

        list->head = current->next;
        free(current);    
    }
}

void freeListProcess(ListProcess* list) {
    Node_p* current = list->head;

    while(list->head) {
        current = list->head;
        list->head = list->head->next;
        free(current);
    }
}

void pushLastProcess(ListProcess *list, Process p) {
    Node_p* new = malloc(sizeof(Node_p));
    Node_p* current = list->head;

    new->process = p;
    new->next = NULL;

    if(!list->head) {
        list->head = new;
        return;
    }

    while(current->next) 
        current = current->next;

    current->next = new;    
}
