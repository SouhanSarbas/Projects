/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */



#include "common.h"



Status sub_elements(operand *op1, operand *op2, int *res_sign,
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
	Dlist **Res_head, Dlist **Res_tail)
{

    // subtraction
    if((op1->sign) != (op2->sign))
    {
	*res_sign = op1->sign;
	if(add_list_elements(head1, tail1, head2, tail2, 
		    Res_head, Res_tail) == success)
	{
	    return success;
	}
	else
	{
	    return failure;
	}
    }
    else
    {
	if((op1->sign) == 1)
	{
	    // h1 - h2
	    if( sub_elements_and_get_sign(op1, op2, res_sign,
			head1, tail1, head2, tail2,
			Res_head, Res_tail) == success)
	    {
		return success;
	    }
	    else
	    {
		return failure;
	    }
	}
	else
	{
	    // h2 - h1
	    if( sub_elements_and_get_sign(op2, op1, res_sign,
			head2, tail2, head1, tail1,
			Res_head, Res_tail) == success)
	    {
		return success;
	    }
	    else
	    {
		return failure;
	    }
	}
    }


}


Status sub_elements_and_get_sign(operand *op1, operand *op2, int *res_sign,
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
	Dlist **Res_head, Dlist **Res_tail)
{
    if ((op1->len) > (op2->len))
    {
	//printf("len 1=%d, 2=%d\n", op1 -> len , op2 -> len);
	if(sub_list_elements(head1,tail1, head2,tail2, Res_head,Res_tail) == success)
	{
	    *res_sign = 1;
	    return success;
	}
	else
	{
	    return failure;
	}
    }
    else if ((op1->len) < (op2->len))
    {
	//printf("len 1=%d, 2=%d\n", op1 -> len , op2 -> len);
	if(sub_list_elements(head2,tail2, head1,tail1, Res_head,Res_tail) == success)
	{
	    *res_sign = -1;
	    return success;
	}
	else
	{
	    return failure;
	}
    }
    else
    {
	//printf("len 1=%d, 2=%d\n", op1 -> len , op2 -> len);
	int i = 0;
	while((((op1->num)[i]) == ((op2 ->num)[i])) && (((op1 -> num) [i] ) != '\0'))
	{
	    i++;
	}
	if((op1->num[i]) > ((op2 -> num) [i]))
	{
	    if(sub_list_elements(head1,tail1, head2,tail2, Res_head,Res_tail) == success)
	    {
		*res_sign = 1;
		return success;
	    }
	    else
	    {
		return failure;
	    }
	}
	else
	{
	    if(sub_list_elements(head2,tail2, head1,tail1, Res_head,Res_tail) == success)
	    {
		*res_sign = -1;
		return success;
	    }
	    else
	    {
		return failure;
	    }
	}


    }

}


Status sub_list_elements(Dlist **head1, Dlist **tail1,
	Dlist **head2, Dlist **tail2, 
	Dlist **Res_head, Dlist **Res_tail)
{
    int data,borrow = 0;
    Dlist *num1 = *tail1;
    Dlist *num2 = *tail2;
    Dlist *temp = NULL;

    while(num2 != NULL)
    {
	//printf("b = %d, n1 = %d, n2 = %d\n",(borrow * 10) , (num1 -> data) , (num2 == NULL ? 0 : (num2 -> data)));

	if((num2 -> data) > (num1 -> data))
	{
	    temp = num1;
	    borrow = 1;
	    num1 = num1 -> prev;
	    while((num1 -> data) == 0)
	    {
		num1 = num1 -> prev;
	    }
	    (num1 -> data)--;
	    num1 = num1 -> next;
	    while(num1 != temp)
	    {
		num1 -> data = 9;
		num1 = num1 -> next;
	    }
	}

	data = ((borrow * 10) + (num1 -> data)) - (num2 == NULL ? 0 : (num2 -> data));

	borrow = 0;
	if(dl_insert_first(Res_head, Res_tail, data) == FAILURE)
	{
	    return failure;
	}


	num1 = num1 -> prev;
	num2 = num2 -> prev;
	
    }

    while(num1 != NULL)
    {
	data = num1 -> data;
	if(dl_insert_first(Res_head, Res_tail, data) == FAILURE)
	{
	    return failure;
	}
	num1 = num1 -> prev;
    }
    
    while(((*Res_head) != NULL) && (((*Res_head) -> data) == 0))
    {
	    dl_delete_first(Res_head, Res_tail);
    }    
    if(*Res_head == NULL)
    {
	if(dl_insert_first(Res_head, Res_tail, 0) == FAILURE)
	{
	    return failure;
	}

    }

    return success;

}



