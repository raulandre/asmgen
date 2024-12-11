%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ERROR(x) fprintf(stderr, "Error: %s.", x)
extern int yylineno;
FILE *program = NULL;

void yyerror(char *s) { fprintf(stderr, "Error on line %d: %s\n", yylineno, s); }
void end() {
    if(program != NULL) {
        fprintf(program, "li $v0, 10\nsyscall\n");
        fprintf(program, "\n");
        fclose(program);
    }
}

struct node {
    struct node *n;
    int v;
};

typedef struct stack {
    struct node *top;
    size_t size;
} stack;

extern stack sb;
%}

%union {
  struct no {
    int isnum;
    int pos;
    char *s;
    int n;
    char cmd[60];
  } no;
}

%type<no> ID
%type<no> NUM
%type<no> EXP
%type<no> IF_HEAD
%type<no> WHILE_HEAD
%left SUM SUB
%right MUL DIV POW
%token VAR ID NUM SEMI COMMA ATTRIB SUM SUB MUL DIV POW OB CB PRINT EQU DO IF NEQU AND OR NOT WHILE MOD END GRT LST ENDI ENDW
%start S

%%

S : VAR VARIABLES SEMI STATEMENTS END { end();  YYACCEPT; }
  | STATEMENTS END { end(); YYACCEPT; } ;

VARIABLES : %empty
          | ID ATTRIB NUM { declare($1.s, $3.n); }
          | ID ATTRIB NUM COMMA VARIABLES { declare($1.s, $3.n); } ;

EXP : NUM { $$.pos = gettemp(), $$.isnum = 1; settempn($$.pos, $1.n); }
    | ID { $$.pos = getp($1.s); $$.isnum = 0; free($1.s); }
    | EXP SUM EXP { $$.pos = cmd_sum($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1; }
    | EXP SUB EXP { $$.pos = cmd_sub($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1;}
    | EXP MUL EXP { $$.pos = cmd_mul($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1;}
    | EXP DIV EXP { $$.pos = cmd_div($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1;}
    | EXP MOD EXP { $$.pos = cmd_mod($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1;}
    | EXP AND EXP { $$.pos = cmd_and($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1;}
    | EXP OR EXP { $$.pos = cmd_or($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1;}
    | EXP LST EXP { $$.pos = cmd_lst($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1; }
    | EXP GRT EXP { $$.pos = cmd_grt($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1; }
    | EXP EQU EXP { $$.pos = cmd_seq($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1;  }
    | EXP NEQU EXP { $$.pos = cmd_sne($1.pos, $3.pos, $1.isnum, $3.isnum, $$.cmd); $$.isnum = 1;  }
    | NOT EXP { $$.pos = cmd_not($2.pos, $2.isnum, $$.cmd); }
    | OB EXP CB { $$.pos = gettemp(); $$.isnum = $2.isnum; settemp($$.pos, $2.pos, $$.cmd); }
    ;

STATEMENTS : %empty
           | STATEMENT SEMI STATEMENTS ;

STATEMENT : %empty
          | ID ATTRIB EXP { set($1.s, $3.pos); free($1.s); }
          | PRINT EXP { cmd_print($2.pos, $2.isnum); }
          | IF_HEAD DO STATEMENTS ENDI {  cmd_if_end($1.isnum); }
          | WHILE_HEAD DO STATEMENTS ENDW { cmd_while_end($1.pos); cmd_if_end($1.isnum); }

IF_HEAD: IF OB EXP CB { $$.isnum = $3.isnum; $$.pos = $3.pos; cmd_if_nequ($3.pos, 0, $3.isnum, 0, 1); };
WHILE_HEAD: WHILE OB EXP CB { $$.pos = $3.pos; $$.isnum = $3.isnum; char* cmd = cmd_if_nequ($3.pos, 0, $3.isnum,0 , 1); cmd_while_nequ($3.pos); fprintf(program, "%s%s", $3.cmd, cmd); free(cmd); }

%%

int main() {
    stack_init(&sb);
    program = fopen("out.mips", "w");
    if(program == NULL) {
        ERROR("failed to create output file");
        exit(EXIT_FAILURE);
    }

    fprintf(program, ".text\nmain:\n");
    yyparse();
}
