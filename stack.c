#include <stdlib.h>

struct node {
    struct node *n;
    int v;
    char *s;
};

typedef struct stack {
    struct node *top;
    size_t size;
} stack;

stack sb;
stack sl;

void stack_init(stack *s) {
    s->top = NULL;
    s->size = 0;
}

void push(stack *s, int v) {
    if(s->top == NULL) {
        s->top = malloc(sizeof(struct node));
        s->top->v = v;
        s->top->n = NULL;
        s->size++;
    } else {
        struct node *n = malloc(sizeof(struct node));
        struct node *top = s->top;
        n->n = top;
        n->v = v;
        s->top = n;
        s->size++;
    }
}

int pop(stack *s) {
    if(s->size <= 0 || s->top == NULL) {
        return -1;
    }

    int tv = s->top->v;
    s->top = s->top->n;
    s->size--;
    return tv;
}
