#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

// Stack implementation
typedef struct Node
{
    char info;
    struct Node *next;
} node;

typedef struct
{
    node *top;
} Stack;

Stack *create_s()
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

int empty_s(Stack *s)
{
    return s->top == NULL;
}

void push(Stack *s, char x)
{
    node *new = (node *)malloc(sizeof(node));
    new->info = x;
    new->next = s->top;
    s->top = new;
}

char pop(Stack *s)
{
    // if the stack is empty
    if (s->top == NULL)
    {
        return '\0';
    }
    node *aux = s->top;
    char x = aux->info;
    s->top = aux->next;
    free(aux);
    return x;
}

void cleanStack(Stack *s)
{
    char aux = pop(s);
    push(s, aux);
    while (aux != 'Z')
    {
        pop(s);
        aux = pop(s);
        push(s, aux);
    }
}

void FreeStack(Stack *s)
{
    node *current = s->top;
    node *nextNode;

    while (current != NULL)
    {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    s->top = NULL;
}

// PDA(Graph) implementation
typedef struct Transition
{
    char push[MAX];
    char pop;
    char consume;
    int Qdestiny;
    struct Transition *next;
} transition;

typedef struct States
{
    transition *T;
    int *terminal_state;
} states;

states *init(int statesQTY)
{
    int i;
    states *q = (states *)malloc(statesQTY * sizeof(states));
    // initialize the adjacency list
    for (i = 0; i < statesQTY; i++)
    {
        q[i].T = NULL;
        q[i].terminal_state = (int *)malloc(sizeof(int));
        *(q[i].terminal_state) = 0;
    }

    return q;
}

// goes from state i to j and consumes c(edge ​​weight) pop T and push
states *NewTransition(states *q, int i, int j, char T, char *X, char c)
{
    transition *newtransition = (transition *)malloc(sizeof(transition));
    newtransition->consume = c;
    newtransition->pop = T;
    strcpy(newtransition->push, X);
    newtransition->Qdestiny = j;
    newtransition->next = NULL;
    if (q[i].T == NULL)
    {
        q[i].T = newtransition;
    }
    else
    {
        transition *aux = q[i].T;
        while (aux->next != NULL)
        {
            aux = aux->next;
        }
        aux->next = newtransition;
    }
    return q;
}
states *definesTerminals(states *q)
{
    int F = 0, i = 0;
    scanf("%d", &F);
    for (i = 0; i < F; i++)
    {
        int qf = -1;
        scanf("%d", &qf);
        *(q[qf].terminal_state) = 1;
    }
    return q;
}

void FreeTransitions(transition *T)
{
    transition *current = T;
    transition *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

int backtracking(states *pda, char *word, Stack *s, int estado, int accept)
{
    // stopping point (PDA accepted)
    char aux = pop(s);
    push(s, aux);
    if ((word[0] == '\0' || word == NULL) && *pda[estado].terminal_state == 1)
    {
        accept = 1;
        return accept;
    }

    // If it is not final state
    else
    {

        // copy word and stack (to use in recurtion)
        int length = strlen(word) + 1;
        char *wordCopy = (char *)malloc(length * sizeof(char));
        strcpy(wordCopy, word);
        Stack *stackCopy = create_s();
        Stack *AuxStack = create_s();
        while (s->top != NULL)
        {
            aux = pop(s);
            push(AuxStack, aux);
        }
        while (AuxStack->top != NULL)
        {
            aux = pop(AuxStack);
            push(s, aux);
            push(stackCopy, aux);
        }
        FreeStack(AuxStack);
        // creates a copy of the possible paths from that state
        transition *path = pda[estado].T;

        while (path != NULL) // test all paths
        {
            // case 1
            if ((path->consume != '&') && (word[0] == path->consume) && (accept != 1) && (path->pop == '&'))
            {
                for (int i = 0; wordCopy[i] != '\0'; i++)
                {
                    wordCopy[i] = wordCopy[i + 1];
                }
                // If you need to push something
                if ((path->push[0] != '&'))
                {
                    for (int i = strlen(path->push) - 1; i >= 0; i--)
                    {
                        push(stackCopy, path->push[i]);
                    }
                }
                // After consuming letters and push/pop whatever is necessary, the status advances
                accept = backtracking(pda, wordCopy, stackCopy, path->Qdestiny, accept);
            }
            aux = pop(stackCopy);
            push(stackCopy, aux);
            // case 2
            if ((path->consume != '&') && (word[0] == path->consume) && (accept != 1) && ((aux == path->pop)))
            {
                pop(stackCopy);
                for (int i = 0; wordCopy[i] != '\0'; i++)
                {
                    wordCopy[i] = wordCopy[i + 1];
                }
                // If you need to push something
                if ((path->push[0] != '&'))
                {
                    for (int i = strlen(path->push) - 1; i >= 0; i--)
                    {
                        push(stackCopy, path->push[i]);
                    }
                }
                // After consuming letters and push/pop whatever is necessary, the status advances
                accept = backtracking(pda, wordCopy, stackCopy, path->Qdestiny, accept);
            }

            // case 3: costless transition goes to the next state
            if ((path->consume == '&') && (path->pop == '&') && (path->push[0] == '&') && (accept != 1))
            {
                accept = backtracking(pda, wordCopy, stackCopy, path->Qdestiny, accept);
            }

            // case 4:transition to final state
            aux = pop(stackCopy);
            push(stackCopy, aux);
            if ((path->consume == '&') && (aux == 'Z'))
            {
                accept = backtracking(pda, wordCopy, stackCopy, path->Qdestiny, accept);
            }

            // while it is not accepted, try the next path
            if (accept != 1)
            {
                path = path->next;
                AuxStack = create_s();
                FreeStack(stackCopy);
                stackCopy = create_s();
                while (s->top != NULL)
                {
                    aux = pop(s);
                    push(AuxStack, aux);
                }
                while (AuxStack->top != NULL)
                {
                    aux = pop(AuxStack);
                    push(s, aux);
                    push(stackCopy, aux);
                }
                FreeStack(AuxStack);
                length = strlen(word) + 1;
                wordCopy = (char *)realloc(wordCopy, length * sizeof(char));
                strcpy(wordCopy, word);
            }

            // if the word is accept returns to the beginning
            else if (accept == 1)
            {
                return accept;
            }
        }
        // If there is no path to follow or none of them, it is possible to return to the previous call
        if (path == NULL)
        {
            return accept;
        }
    }
    return accept;
}

void FreeStates(states *q, int statesQTY)
{
    int i;
    for (i = 0; i < statesQTY; i++)
    {
        FreeTransitions(q[i].T); // Releases all transitions from state i
    }
    free(q); // Free the state array
}

int main()
{
    int Q = 0, T = 0;
    scanf("%d %d", &Q, &T);
    int i;
    states *states = init(Q);
    // creates the graph (adjacency list)
    for (i = 0; i < T; i++)
    {
        char c = '\0', desT = '\0', empX[MAX];
        int qI = 0, qF = 0;
        scanf("%d %c %c %s %d", &qI, &c, &desT, empX, &qF);
        NewTransition(states, qI, qF, desT, empX, c);
    }
    definesTerminals(states);
    Stack *s = create_s();
    push(s, 'Z');
    char control = '\0';
    char word[101];
    while (control != '*')
    {
        printf("\n\n");
        scanf("%100s", word);
        control = word[0];
        int accept = 0;
        if (control != '*')
        {
            accept = backtracking(states, word, s, 0, accept);
            if (accept == 1)
            {
                printf("\n%s: Yes", word);
            }
            else if (accept == 0)
            {
                printf("\n%s: Not", word);
            }
            cleanStack(s);
        }
    }
    FreeStack(s);
    FreeStates(states, Q);
    return 0;
}