/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */

#include "common.h"

Status mul_elements(operand *op1, operand *op2, int *res_sign,
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
	Dlist **Res_head, Dlist **Res_tail)
{
    /* sign of the product */
    *res_sign = op1->sign * op2->sign;

    int data, carry = 0;
    Dlist *num1 = *tail1;
    Dlist *num2 = *head2;

    /* list to store intermediate sum */
    Dlist *hres = NULL;
    Dlist *tres = NULL;

    /* list to store cumulative sum */
    Dlist *th = NULL;
    Dlist *tt = NULL;
    
    /* initialize the result list with zero data */
    if(dl_insert_first(Res_head,Res_tail, 0) == FAILURE)
    {
	return failure;
    }

    /* Multiplication */
    while(num2 != NULL)
    {
	carry = 0;
	num1 = *tail1; // reset num1 pointer to right most digit of list head1
	while(num1 != NULL)
	{
	    /* multiply the digits */
	    data = (num1 -> data) * (num2 -> data);
	    /* add the carry */
	    data = data + carry;
	    /* Find the carry */
	    carry = data / 10;
	    /* Find the digit */
	    data = data % 10;

	    /* Store the digit */
	    if(dl_insert_first(&hres, &tres, data) == FAILURE)
	    {
		return failure;
	    }

	    num1 = num1 -> prev; // next digit in  head1 list to the left
	}
	/* Add carry digit at left */
	if(carry > 0)
	{
	    if(dl_insert_first(&hres, &tres, carry) == FAILURE)
	    {
		return failure;
	    }
	}
	num2 = num2 -> next;

	/* Add 0 if its not at the one's position */
/*	if (num2 != NULL)
	{
	    if(dl_insert_last(&hres, &tres, 0) == FAILURE)
	    {
		return failure;
	    }
	}
*/


	/* Add cumulative sum in Res_head list  with intermediate sum got at hres list 
	   and store in th list */
	if(add_list_elements(Res_head, Res_tail, &hres, &tres, &th, &tt) == failure)
	{
	    return failure;
	}
/*
	print_list(*Res_head);
	print_list(hres);
	print_list(th);
	*/
	/* reset the hres list data to zero */
	if(dl_delete_list(&hres, &tres) == FAILURE)
	{
	    return failure;
	}
	if(dl_insert_first(&hres, &tres, 0) == FAILURE)
	{
	    return failure;
	}
	/* reset the Res_head list */
	if(dl_delete_list(Res_head, Res_tail) == FAILURE)
	{
	    return failure;
	}

	/* Store the cumulative sum in Res_head list */
	if(add_list_elements(&th, &tt, &hres, &tres, Res_head, Res_tail) == failure)
	{
	    return failure;
	}

	/* reset the hres list and th list */
	if(dl_delete_list(&hres, &tres) == FAILURE)
	{
	    return failure;
	}
	if(dl_delete_list(&th, &tt) == FAILURE)
	{
	    return failure;
	}
	/* Add 0 if its not at the one's position */
	if (num2 != NULL)
	{
	    if(dl_insert_last(Res_head, Res_tail, 0) == FAILURE)
	    {
		return failure;
	    }
	}

    }
    return success;
}


