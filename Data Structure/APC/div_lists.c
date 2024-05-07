/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */

#include "common.h"

Status div_elements(operand *op1, operand *op2, int *res_sign,
	Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2,
	Dlist **Res_head, Dlist **Res_tail)
{
    //printf("%d .. %c\n", op2 -> len , (op2 -> num)[0]);
    if(((op2->len) <= 1) && ((op2->num) [0] == '0'))
    {
	printf("ERROR: Division by zero is not possible\n");
	return failure;
    }
    *res_sign = (op1 -> sign) /(op2 -> sign);

    int count = 0;

    Dlist *rem_h = NULL;
    Dlist *rem_t = NULL;
    Dlist *one_h = NULL;
    Dlist *one_t = NULL;
    Dlist *z_h = NULL;
    Dlist *z_t = NULL;
    Dlist *count_h = NULL;
    Dlist *count_t = NULL;

    if(dl_insert_first(&one_h, &one_t, 1) == FAILURE)
    {
	return failure;
    }
    if(dl_insert_first(&z_h, &z_t, 0) == FAILURE)
    {
	return failure;
    }


    if(dl_insert_first(&count_h, &count_t, 0) == FAILURE)
    {
	return failure;
    }

    while(check_h1_larger(*head1, *head2) == success)
    {
	if(sub_list_elements(head1,tail1,head2,tail2 ,&rem_h, &rem_t) == success)
	{
	    count++;

	    /*
	    if(add_list_elements(&count_h, &count_t, &one_h, &one_t, Res_head, Res_tail) == failure)
	    {
		return failure;
	    }

	    if(dl_delete_list(&count_h, &count_t) == FAILURE)
	    {
		return failure;
	    }

	    if(add_list_elements(Res_head, Res_tail, &z_h, &z_t, &count_h, &count_t) == failure)
	    {
		return failure;
	    }

	    if(dl_delete_list(Res_head, Res_tail) == FAILURE)
	    {
		return failure;
	    }
	    */


	}
	if(dl_delete_list(head1,tail1) == FAILURE)
	{
	    return failure;
	}

	if(add_list_elements(&rem_h,&rem_t, &z_h, &z_t, head1,tail1 ) == failure)
	{
	    return failure;
	}

	if(dl_delete_list(&rem_h, &rem_t) == FAILURE)
	{
	    return failure;
	}


    }
    printf("REMINDER: ");
    print_list(*head1);


    printf("RESULT: %c ", *res_sign > 0 ? '+' : '-');
   
    printf("%d\n", count);
   // print_list(count_h);
   
    return success;


}

Status check_h1_larger(Dlist *head1, Dlist *head2)
{
    int count1 = 0, count2 = 0;
    Dlist *temp1 = head1;
    Dlist *temp2 = head2;

    while(temp1 != NULL)
    {
	count1++;
	temp1 = temp1 -> next;
    }

    while(temp2 != NULL)
    {
	count2++;
	temp2 = temp2 -> next;
    }
    if(count1 > count2)
    {
	return success;
    }
    else if(count1 == count2)
    {
	temp1 = head1;
	temp2 = head2;
	while(temp2 != NULL)
	{
	    if((temp1 -> data) > (temp2 -> data))
	    {
		return success;
	    }
	    else if((temp1 -> data) < (temp2 -> data))
	    {
		return failure;
	    }
	    temp1 = temp1 -> next;
	    temp2 = temp2 -> next;
	}
	return success;

    }
    else
    {
	return failure;
    }
}








