#include <synchapi.h>
#include <stdio.h>

#include "displayHandler.h"
#include "studentStruct.h"

void welcomeUser()
{
    printf("---------------------\n");
    printf("- Enter Credentials -\n");
    printf("---------------------\n");
    printf("Enter your surname and firstname under the following form:\n"
           "\n'login [FIRSTNAME] [SURNAME]'\nHit ENTER to confirm.\n\n");
}

void displayMainPage()
{
    printf("--------------------------\n");
    printf("--      Welcome to      --\n");
    printf("--  The Library System! --\n");
    printf("--------------------------\n\n");
    printf("Type the index of the system that you want to access, then press ENTER!\n");
    printf("> 1. User Management Page\n");
    printf("> 2. Library Management Page\n");
    printf("> 3. Quit App\n");
}

void noUserFoundMessage(char * studentName)
{
    printf("There is no user with the name '%s' found in the database, try again.", studentName);
}


void displayAllBooksByAuthor(char authorName[], struct Book * books, int numberOfBooks)
{
    printf("These are all the books available written by '%s' available in the library:\n\n", authorName);

    int foundBooks = 0;

    for(int i = 0; i < numberOfBooks; i++)
    {
        int found = strcmp(authorName, books[i].authorName);

        if(found == 0)
        {
            foundBooks++;
            printf("Book Name: %s\n", books[i].bookName);
        }
    }

    if(foundBooks != 0)
    {
        printf("\nWrite down the book you're interested in and try searching it again!\n");
    }
}

void pressAnyKey()
{
    fflush(stdin);

    printf("\nPress any key to continue.\n");
    char ch;
    scanf("%c", &ch);
}

void displayLoggedIn()
{
    printf("\nSuccessfully logged in!\n");
}

void errorMessage(char * message)
{
    printf("%s\n", message);
    Sleep(2000);
}

void displayUserManagementPage()
{
    printf("-------------------\n");
    printf("- User Management -\n");
    printf("-------------------\n\n");
    printf("> 1. Add User\n");
    printf("> 2. Return to main menu\n");
}

void displayBookManagementPage()
{
    printf("-------------------\n");
    printf("- Book Management -\n");
    printf("-------------------\n\n");
    printf("> 1. Borrow a Book\n");
    printf("> 2. Donate a Book\n");
    printf("> 3. Return a Book\n");
    printf("> 4. Show Borrowed Books\n");
    printf("> 5. Search For a Book\n");
    printf("> 6. Return to the Main Menu\n");
}

void endScreen()
{
    printf("Have a nice day!");
    Sleep(2000);
    exit(0);
}