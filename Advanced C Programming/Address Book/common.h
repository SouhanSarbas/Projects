#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* Datatypes */

typedef struct _Filedata
{
    char *fname;
    char *ftemp;
    FILE *fptr;
    int size;
} BookInfo;
typedef struct _Contact
{
    char name[24];
    char mob[16];
    char email[24];
    char location[24];
} ContactInfo;

typedef enum 
{
    success,
    failure = -1,
    file_empty = -2
} Status;


/* Function declaration */

// function to open the csv file and get address book details
Status open_and_validate_csv(BookInfo *filedata);
// function to print all the contacts in the address book
void print_all(BookInfo *filedata);

int read_address_book_size(BookInfo *filedata);

int convert_to_int(char cptr[], int size);

void print_menu(void);

void search_contact(BookInfo *filedata);

int search_by_name(BookInfo *filedata, char *str);

int search_by_mob(BookInfo *filedata);

int search_by_email(BookInfo *filedata);

int search_by_loc(BookInfo *filedata);

Status add_contact(BookInfo *filedata);

Status edit_contact(BookInfo *filedata);

Status edit_SL(BookInfo *filedata, int n);

void change_to_str(char str[], int num);

Status delete_contact(BookInfo *filedata);

Status delete_SL(BookInfo *filedata, int n);

#endif
