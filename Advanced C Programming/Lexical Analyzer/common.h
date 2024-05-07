#ifndef COMMON_H
#define COMMON_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* datatype */
typedef enum
{
    idle,
    comment_s,
    comment_m,
    preproc,
    next_line,
    identifier,
    key,
    string,
    numeric,
    numend,
    hexa,
    bina,
    octal,
    error
} State;

#define SUCCESS 0
#define FAILURE -1

char opers_R[20] = { ' ', '\n',
    '+','-','/','*', '<','>','=', ',','?',':',';','[',']','{','}','(',')','%'};

char *keywords[32] = {
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while"
};

#endif
