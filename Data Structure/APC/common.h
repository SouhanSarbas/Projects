#ifndef COMMON_H
#define COMMON_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Macrons */

#define SUCCESS 0
#define FAILURE -1



/* Type Definitions */

typedef enum
{
    success,
    failure
} Status;


typedef struct node
{
    struct node *next;
    struct node *prev;
    int data;
    
} Dlist;

typedef struct _operand
{
    char *num;
    int sign;
    int len;
} operand;


/* Function declarations */
int dl_insert_first(Dlist **head, Dlist **tail, int data);
int dl_insert_last(Dlist **head, Dlist **tail, int data);
int dl_delete_list(Dlist **head, Dlist **tail);
void print_list(Dlist *head);
int dl_delete_first(Dlist **head, Dlist **tail);



Status validate_CLA(char *argv[], operand *op1, operand *op2, char *operation);
Status validate_operand(char argv[], operand *op1);



Status convert_num_str_to_list(Dlist **head, Dlist **tail, char num[]);




Status add_elements(operand *op1, operand *op2, int *res_sign, 
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, 
	Dlist **Res_head, Dlist **Res_tail);

Status sub_elements(operand *op1, operand *op2, int *res_sign, 
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, 
	Dlist **Res_head, Dlist **Res_tail);

Status mul_elements(operand *op1, operand *op2, int *res_sign, 
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, 
	Dlist **Res_head, Dlist **Res_tail);

Status div_elements(operand *op1, operand *op2, int *res_sign, 
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, 
	Dlist **Res_head, Dlist **Res_tail);





Status sub_elements_and_get_sign(operand *op1, operand *op2, int *res_sign, 
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, 
	Dlist **Res_head, Dlist **Res_tail);

Status add_list_elements(Dlist **head1, Dlist **tail1, 
	Dlist **head2, Dlist **tail2, Dlist **Res_head, Dlist **Res_tail);

Status sub_list_elements(Dlist **head1, Dlist **tail1, 
	Dlist **head2, Dlist **tail2, Dlist **Res_head, Dlist **Res_tail);

Status check_h1_larger(Dlist *head1, Dlist *head2);


#endif
