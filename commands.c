#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GLOBAL_TREG_COUNT 8
#define GETTYPE(x) x == 0 ? 's' : 't'

int TREG = 0;
int BRANCH_COUNT = 0;
int LOOP_COUNT = 0;

struct node {
    struct node *n;
    int v;
};

typedef struct stack {
    struct node *top;
    size_t size;
} stack;

extern FILE *program;
extern stack sb;
extern stack sl;

int gettemp() {
    if(TREG + 1 >= GLOBAL_TREG_COUNT) {
        TREG = 0;
    }

    return TREG++;
}

void settempn(int pos, int val) {
    fprintf(program, "li $t%d, %d\n", pos, val);
}

void settemp(int pos1, int pos2) {
    fprintf(program, "move $t%d, $t%d\n", pos1, pos2);
}

int cmd_sum(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "add $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "add $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_sub(int v1, int v2,int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "sub $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "sub $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_mul(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "mul $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "mul $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_div(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "div $%c%d, $%c%d\n", GETTYPE(i1), v1, GETTYPE(i2), v2);
    fprintf(program, "mflo $t%d\n", result);
    sprintf(b, "div $%c%d, $%c%d\nmflo $t%d\n", GETTYPE(i1), v1, GETTYPE(i2), v2, result);
    return result;
}

int cmd_mod(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "rem $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "rem $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_and(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "and $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "and $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_or(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "or $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "or $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_nor(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "nor $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "nor $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_seq(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "seq $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "seq $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_lst(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "slt $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "slt $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_grt(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "sgt $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "sgt $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_sne(int v1, int v2, int i1, int i2, char *b) {
    int result = gettemp();
    fprintf(program, "sne $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    sprintf(b, "sne $t%d, $%c%d, $%c%d\n", result, GETTYPE(i1), v1, GETTYPE(i2), v2);
    return result;
}

int cmd_not(int v1, int i1, char *b) {
    int result = gettemp();
    fprintf(program, "not $t%d, $%c%d\n", result, GETTYPE(i1), v1);
    sprintf(b, "not $t%d, $%c%d\n", result, GETTYPE(i1), v1);
    return result;
}

void cmd_print(int temp, int num) {
    fprintf(program, "move $a0, $%c%d\n", GETTYPE(num), temp);
    fprintf(program, "li $v0, 1\nsyscall\n");
    fprintf(program, "li $a0, 10\n");
    fprintf(program, "li $v0, 11\nsyscall\n");
}

void cmd_read(int temp) {
    fprintf(program, "li $v0, 5\nsyscall\n");
    fprintf(program, "move $s%d, $v0\n", temp);
}

void cmd_if_equ(int t1, int t2, int i1, int i2) {
    push(&sb, BRANCH_COUNT);
    fprintf(program, "bne $%c%d, $%c%d, B%d\n", GETTYPE(i1), t1, GETTYPE(i2), t2, BRANCH_COUNT++);
}

char* cmd_if_nequ(int t1, int t2, int i1, int i2, int ZERO) {
    push(&sb, BRANCH_COUNT);
    char buffer[50];
    if(!ZERO) {
        int b = BRANCH_COUNT;
        fprintf(program, "beq $%c%d, $%c%d, B%d\n", GETTYPE(i1), t1, GETTYPE(i2), t2, BRANCH_COUNT++);
        sprintf(buffer, "beq $%c%d, $%c%d, B%d\n", GETTYPE(i1), t1, GETTYPE(i2), t2, b);
    } else {
        int b = BRANCH_COUNT;
        fprintf(program, "beq $%c%d, $zero, B%d\n", GETTYPE(i1), t1, BRANCH_COUNT++);
        sprintf(buffer, "beq $%c%d, $zero, B%d\n", GETTYPE(i1), t1, b);
    }

    return strdup(buffer);
}

void cmd_if_end(int isnum) {
    fprintf(program, "B%d:\n", pop(&sb));
}

void cmd_while_nequ(int t1) {
    push(&sl, LOOP_COUNT);
    fprintf(program, "L%d:\n", LOOP_COUNT++);
}

void cmd_while_end(int isnum) {
    fprintf(program, "j L%d\n", pop(&sl));
}
