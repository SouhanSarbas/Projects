/*
 * AUTHOR : SOUHAN SARBAS C H
 * DATE : 6-5-2024
 * DESCRIPTION : Arbitrary Precision Calculator
 *             [ Arithmetic operations on larger numbers ]
 */

#include "common.h"

int main(int argc, char *argv[])
{
    operand op1,op2;
    char operation;
    int res_sign;

    /* list for first operand*/
    Dlist *head1 = NULL;
    Dlist *tail1 = NULL;
    
    /* list for second operand */
    Dlist *head2 = NULL;
    Dlist *tail2 = NULL;
    
    /* list to store the result */
    Dlist *Res_head = NULL;
    Dlist *Res_tail = NULL;

    /* Check whether  no. of arguments passed is valid */
    if(argc < 4)
    {
	printf("ERROR: Command line arguments missing\n");
	printf("Usage: %s operand1 operator operand2\n", argv[0]);
	printf("Sample : %s 10001 + 1110\n", argv[0]);
	return FAILURE;
    }

    /* Validate the arguments passed */
    if(validate_CLA(argv, &op1, &op2, &operation) == success)
    {
	/* Convert numbers in string to lists */
	if(convert_num_str_to_list(&head1, &tail1, op1.num) == failure)
	{
	    printf("ERROR: Converting number to linked list failed\n");
	    return FAILURE;
	}

	if(convert_num_str_to_list(&head2, &tail2, op2.num) == failure)
	{
	    printf("ERROR: Converting number to linked list failed\n");
	    return FAILURE;
	}

	/* DO ARITHMETIC OPERATIONS */
	switch(operation)
	{
	    case '+' : // addition
		printf(">> %c %s [%c] %c %s\n", op1.sign > 0 ? '+' : '-', op1.num , operation, op2.sign > 0 ? '+' : '-', op2.num);
		if(add_elements(&op1, &op2, &res_sign,
			    &head1, &tail1, &head2, &tail2, 
			    &Res_head, &Res_tail) == success)
		{
		    printf("%c", res_sign > 0 ? '+' : '-');
		    print_list(Res_head);

		}
		else
		{

		    printf("ERROR: Addition failed\n");
		    return FAILURE;
		}
		break;

	    case '-' : // subtraction
		printf(">> %c %s [%c] %c %s\n", op1.sign > 0 ? '+' : '-', op1.num , operation, op2.sign > 0 ? '+' : '-', op2.num);
		if(sub_elements(&op1, &op2, &res_sign,
			    &head1, &tail1, &head2, &tail2, 
			    &Res_head, &Res_tail) == success)
		{
		    printf("%c", res_sign > 0 ? '+' : '-');
		    print_list(Res_head);

		}
		else
		{

		    printf("ERROR: Subtraction failed\n");
		    return FAILURE;
		}

		break;
	    case 'x' : // Multiplication
		printf(">> %c %s [%c] %c %s\n", op1.sign > 0 ? '+' : '-', op1.num , operation, op2.sign > 0 ? '+' : '-', op2.num);
		if(mul_elements(&op1, &op2, &res_sign,
			    &head1, &tail1, &head2, &tail2, 
			    &Res_head, &Res_tail) == success)
		{
		    printf("%c", res_sign > 0 ? '+' : '-');
		    print_list(Res_head);
		}
		else
		{
		    printf("ERROR: Multiplication failed\n");
		    return FAILURE;
		}

		break;
	    case '/' : // Division
		printf(">> %c %s [%c] %c %s\n", op1.sign > 0 ? '+' : '-', op1.num , operation, op2.sign > 0 ? '+' : '-', op2.num);
		if(div_elements(&op1, &op2, &res_sign,
			    &head1, &tail1, &head2, &tail2, 
			    &Res_head, &Res_tail) == success)
		{
		    printf("\n");
		}
		else
		{
		    printf("ERROR: Division failed\n");
		    return FAILURE;
		}
		break;
	    default :
		{
		printf(">> %c %s [%c] %c %s\n", op1.sign > 0 ? '+' : '-', op1.num , operation, op2.sign > 0 ? '+' : '-', op2.num);
		    printf("ERROR: Invalid operation\n");
		    return FAILURE;
		}

	}

    }
    else // Error handling
    {
	printf("Usage: %s operand1 operator operand2\n", argv[0]);
	printf("Sample : %s 10001 + 1110\n", argv[0]);
    }


    return SUCCESS;
}
