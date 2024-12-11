#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR(x) fprintf(stderr, "Error: %s.", x)
#define GLOBAL_SREG_COUNT 8
#define GLOBAL_TREG_COUNT 8

extern FILE *program;
int SREG = 0;

typedef struct {
    char *name;
    int pos;
    int value;
} variable;

variable variables[GLOBAL_SREG_COUNT]; //0-7

int find(char *name) {
    for(int i = 0; i < GLOBAL_SREG_COUNT; i++) {
        if(strcmp(name, variables[i].name) == 0) {
            return i;
        }
    }

    return -1;
}

void declare(char *name, int value) {
    if(SREG >= GLOBAL_SREG_COUNT) {
        ERROR("too many variables");
        fclose(program);
        exit(EXIT_FAILURE);
    }

    variable v;
    v.name = name;
    v.value = value;
    v.pos = SREG;

    fprintf(program, "li $s%d, %d\n", v.pos, v.value);
    variables[SREG++] = v;
}

void getv(int temp, char *str) {
    char *name = strdup(str);
    int pos = find(name);
    free(name);
    if(pos != -1) {
        fprintf(program, "move $t%d, $s%d\n", temp, variables[pos].pos);
        return;
    }

    ERROR("attempted to use undeclared variable");
    exit(EXIT_FAILURE);
}

int getp(char *str) {
    char *name = strdup(str);
    int pos = find(name);
    free(name);
    if(pos != -1) {
        return variables[pos].pos;
    }

    return -1;
}

void set(char *str, int temp) {
    char *name = strdup(str);
    int pos = find(name);
    free(name);
    if(pos != -1) {
        fprintf(program, "move $s%d, $t%d\n", variables[pos].pos, temp);
    } else {
        ERROR("attempted to use undeclared variable");
        exit(EXIT_FAILURE);
    }
}
