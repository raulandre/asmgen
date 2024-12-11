%{
#include "parser.tab.h"
%}

%option noyywrap
%option yylineno

%%

[\n\r\t ]+ {;}
"var" { return VAR; }
"endi" { return ENDI; }
"endw" { return ENDW; }
"end" { return END; }
"print" { return PRINT; }
"while" { return WHILE; }
"do" { return DO; }
"if" { return IF; }
"&&" { return AND; }
"||" { return OR; }
"!" { return NOT; }
[a-zA-Z_][a-zA-Z_\-0-9]* { yylval.no.s = strdup(yytext); return ID; }
[0-9]+ { yylval.no.n = atoi(yytext); return NUM; }
[;] { return SEMI; }
[,] { return COMMA; }
"==" { return EQU; }
"!=" { return NEQU; }
">" { return GRT; }
"<" { return LST; }
"=" { return ATTRIB; }
"%" { return MOD; }
"+" { return SUM; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }
"^" { return POW; }
"(" { return OB; }
")" { return CB; }
. { fprintf(stderr, "Unrecognized token: '%s'", yytext); }

%%
