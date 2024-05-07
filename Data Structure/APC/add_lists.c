/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */
#include "common.h"

Status add_elements(operand *op1, operand *op2, int *res_sign,
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
	Dlist **Res_head, Dlist **Res_tail)
{
    // addition
    if((op1->sign) == (op2->sign))
    {
	*res_sign = op1->sign;
	if(add_list_elements(head1, tail1, head2, tail2, Res_head, Res_tail) == success)
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
	if(op2->sign == -1)
	{
	    // h1 - h2
	    if( sub_elements_and_get_sign(op1,op2, res_sign,
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



Status add_list_elements(Dlist **head1, Dlist **tail1,
	Dlist **head2, Dlist **tail2, Dlist **Res_head, Dlist **Res_tail)
{

    int data, carry = 0;
    Dlist *num1 = *tail1;
    Dlist *num2 = *tail2;

    while(num1 != NULL && num2 != NULL)
    {
	data = ((num1 -> data) + (num2 -> data)) + carry;
	carry = data / 10;
	data = data % 10;
	if(dl_insert_first(Res_head,Res_tail,data) == FAILURE)
	{
	    return failure;
	}
	num1 = num1 -> prev;
	num2 = num2 -> prev;
    }
    while(num1 != NULL)
    {
	data = num1 -> data + carry;
	carry = data / 10;
	data = data % 10;
	if(dl_insert_first(Res_head,Res_tail,data) == FAILURE)
	{
	    return failure;
	}
	num1 = num1 -> prev;
    }
    while(num2 != NULL)
    {
	data = num2 -> data + carry;
	carry = data / 10;
	data = data % 10;
	if(dl_insert_first(Res_head,Res_tail,data) == FAILURE)
	{
	    return failure;
	}
	num2 = num2 -> prev;
    }
    if (carry > 0)
    {
	if(dl_insert_first(Res_head, Res_tail, carry) == FAILURE)
	{
	    return failure;
	}
    }
    return success;


}
