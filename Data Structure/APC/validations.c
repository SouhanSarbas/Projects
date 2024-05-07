/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */

#include "common.h"

Status validate_CLA(char *argv[],operand *op1, operand *op2, char *operation)
{
    if(validate_operand(argv[1],op1) == success)
    {
	if(validate_operand(argv[3],op2) == success)
	{
	    if(strstr("+-/x", argv[2]) != NULL)
	    {
	//	printf("--- %s\n", argv[2]);
		*operation = argv[2][0];
		return success;
	    }
	    else
	    {
		printf("ERROR: Invalid operation\n");
		return failure;
	    }
	}
	else
	{
	    printf("ERROR: Operand must be a valid number\n");
	    return failure;
	}

    }
    else
    {
	    printf("ERROR: Operand must be a valid number\n");
	    return failure;
    }
}
Status validate_operand(char argv[], operand *op1)
{
    int i = 0;
    if(argv[0] == '+')
    {
	op1->sign = 1;
	i++;
    }
    else if(argv[0] == '-')
    {
	op1->sign = -1;
	i++;
    }
    else if(argv[0] >= '0' && argv[0] <= '9')
    {
	op1 -> sign = 1;
    }
    else
    {
	return failure;
    }
int j = i;
    op1 -> num = &argv[i];
    if(strlen(op1 -> num) > 0)
    {
	while(argv[i] != '\0')
	{
	    if (argv[i] > '9' || argv[i] < '0')
	    {
		return failure;
	    }
	    i++;
	}

	while(argv[j] == '0')
	{
	    j++;
	}
	op1 -> len = i - j;
	if ((op1 -> len) > 0)
	{
	    op1 -> num = &argv[j];
	}
	else
	{
	    op1 -> num = &argv[j-1];
	}

	return success;
    }
    else
    {
	return failure;
    }

}










