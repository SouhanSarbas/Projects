/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */

#include "common.h"

/* Function to insert at first */

int dl_insert_first(Dlist **head, Dlist **tail, int data)
{
    /* create the new node */
    Dlist *newl = malloc(sizeof(Dlist));
    if(newl == NULL)
    {
	return FAILURE;
    }
    newl -> data = data;
    newl -> prev = NULL;
    newl -> next = NULL;

    /* When list is empty */
    if(*head == NULL)
    {
	*head = newl;
	*tail = newl;
	return SUCCESS;
    }

    /* When not empty */
    newl -> next = *head;
    (*head) -> prev = newl;
    *head = newl;
    return SUCCESS;

}



/* Function to insert at last in double linked list */

int dl_insert_last(Dlist **head, Dlist **tail, int data)
{
    /* create the new node */
    Dlist *newl = malloc(sizeof(Dlist));
    if(newl == NULL)
    {
	return FAILURE;
    }
    newl -> data = data;
    newl -> prev = NULL;
    newl -> next = NULL;

    /* When list is empty */
    if(*head == NULL)
    {
	*head = newl;
	*tail = newl;
	return SUCCESS;    
    }

    /* When not empty */

    newl -> prev = *tail;
    (*tail) -> next = newl;
    *tail = newl;
    return SUCCESS;

}



/* Function to delete whole double linked list */
int dl_delete_list(Dlist **head, Dlist **tail)
{
    /* Chwck whether list is empty */
    if(*head == NULL)
    {
	return FAILURE;
    }

    /* delete each element */
    while(*head != *tail)
    {
	*head = (*head) -> next;
	free((*head) -> prev);
	(*head) -> prev = NULL;
    }

    free(*head);
    *head = NULL;
    *tail = NULL;

    return SUCCESS;


}




void print_list(Dlist *head)
{
    /* Cheking the list is empty or not */
    if (head == NULL)
    {
	printf("INFO : List is empty\n");
    }
    else
    {
	printf(" ");
	while (head)		
	{
	    /* Printing the list */
	    printf("%d", head -> data);

	    /* Travering in forward direction */
	    head = head -> next;
	    if (head)
		printf(" ");
	}
	printf("\n");
    }
}

int dl_delete_first(Dlist **head, Dlist **tail)
{
    /* Check whether list is empty */
    if(*head == NULL)
    {
	return FAILURE;
    }

    /* Single element list */

    if(*head == *tail)
    {
	free(*head);
	*head = NULL;
	*tail = NULL;
	return SUCCESS;
    }

    /* Multiple element list */

    *head = (*head) -> next;
    free((*head) -> prev);
    (*head) -> prev = NULL;
    return SUCCESS;

}



