/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :
 
 */
#include "common.h"

/* Function to convert the number in string to double linked list */
Status convert_num_str_to_list(Dlist **head, Dlist **tail, char num[])
{
    int i = 0;
    int n;
    while(num[i] != '\0')
    {
	n = (int)(num[i] - '0');
	if(dl_insert_last(head, tail, n) == FAILURE)
	{
	    return failure;
	}
	i++;
    }
    return success;
}

