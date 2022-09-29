#ifndef TYPES_H
#define TYPES_H

typedef struct {
    int size;
    char status; 
}Process;

typedef struct Node_p {
    Process process;
    struct Node_p *next;
}Node_p;

typedef struct ListProcess {
    Node_p *head;
}ListProcess;

typedef struct Bloc {
    char state; 
    int start;
    int end;
}Bloc;

typedef struct Node {
    Bloc *bloc;
    struct Node *next;
}Node;

typedef struct ListBloc {
    Node* head;
    int nbr_proc;
}ListBloc;

#endif
