#include <stdio.h>
#include <string.h>
#include <synchapi.h>

#include "loansStruct.h"
#include "bookStruct.h"
#include "studentStruct.h"

#include "fileHandler.h"
#include "displayHandler.h"
#include "utilityHeader.h"

int checkValidity(char str1[], char str2[])
{
    return (strcmp(str1, "") == 0) || (strcmp(str2, "") == 0);
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

int findBook(struct Book * books, int numberOfBooks, char wishedBook[], char bookAuthor[])
{
    int found;

    for(int i = 0; i < numberOfBooks; i++)
    {
        found = strcmp(books[i].bookName, wishedBook) != 0 && strcmp(books[i].authorName, bookAuthor) != 0;

        if(found == 0)
        {
            books[i].copiesAvailable -= 1;
            return 1;
        }
    }

    return 0;
}

//We will update our entry inside the books file and decrease the available quantity by 1
int removeBookQuantity(struct Book * books, int numberOfBooks, char wishedBook[], char wishedAuthor[])
{
    if(findBook(books, numberOfBooks, wishedBook, wishedAuthor) == 0)
    {
        errorMessage("Something went wrong!");
        pressAnyKey();
        return 0;
    }

    writeBooksToFile(books, numberOfBooks);

    printf("\nWould you want to loan another book? [Y/N]\n");
    char choice;
    scanf("%c", &choice);
    if(choice == 'y' || choice == 'Y')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//We are inserting a new entry in our text with loans for every book that is borrowed by each student
void addNewLoanEntry(char currentlyAuthStudentName[], char wishedBook[], char wishedAuthor[])
{
    FILE * fLoans = openFile(fLoansName, "a+");

    fprintf(fLoans, loansWritingPattern, currentlyAuthStudentName, wishedBook, wishedAuthor);
    fclose(fLoans);
}

void writeStudentToFile(char firstName[], char lastName[])
{
    FILE * fUsers = openFile(fUsersName, "a+");

    fprintf(fUsers, userWritingPattern, firstName, lastName);
    fclose(fUsers);
}

void writeBooksToFile(struct Book * books, int numberOfBooks)
{
    if(books == NULL || numberOfBooks == 0) { return; }

    //We need to open our original books file in write mode in order to clear the file from its content
    FILE * fBooks = openFile(fBooksName, "w");
    fclose(fBooks);

    //We open our original file in append mode and copy everything from our book structs
    fBooks = openFile(fBooksName, "a");

    for(int i = 0; i < numberOfBooks; i++)
    {
        fprintf(fBooks, bookWritingPattern,
                books[i].bookName, books[i].authorName, books[i].copiesAvailable);
    }

    fclose(fBooks);
}

int verifyArguments(char * words[], int length)
{
    //We check to see if the first argument of our login string is "login"
    //and if the length of our arguments are 3, respecting the "login [FNAME] [LNAME]" pattern.
    if(words == NULL || strcmp(words[0], "login") != 0 || length != 3)
    {
        errorMessage("Invalid format!");
        return 0;
    }
    return 1;
}

int findStudent(char firstName[], char lastName[], char studentName[])
{
    FILE * fUsers = openFile(fUsersName, "r");

    while(fscanf(fUsers, userReadingPattern, firstName, lastName) != EOF)
    {
        //We are comparing our student name with the first name and last name concatenated from the file
        if(strcmp(studentName, strcat(firstName, lastName)) == 0)
        {
            fclose(fUsers);
            return 1;
        }
    }

    fclose(fUsers);
    return 0;
}

int checkForDuplicates(struct Student * students, int numberOfStudents, char firstName[], char lastName[])
{
    for(int i = 0; i < numberOfStudents; i++)
    {
        //We found a duplicate
        if(strcmp(firstName, students[i].firstName) == 0 && strcmp(lastName, students[i].lastName) == 0)
        {
            errorMessage("There already exists an user with the same name!");
            return 1;
        }
    }

    return 0;
}

int addUser(int numberOfStudents, struct Student * students)
{
    char firstName[50], lastName[50];
    printf("Enter the first name of the new student which you wish to add in the database:\n");
    gets(firstName);

    printf("Enter the student's last name:\n");
    gets(lastName);

    if(checkValidity(firstName, lastName) == 1)
    {
        errorMessage("You must enter a first name and a last name!");
        return -1;
    }
    //Check for duplicates
    if(checkForDuplicates(students, numberOfStudents, firstName, lastName) == 1)
    {
        errorMessage("Found an user with the same name!\n");
        return -1;
    }
    printf("hemlo");
    //If we didn't find a duplicate
    writeStudentToFile(firstName, lastName);

    printf("Successfully added a new entry in the database!\n");

    Sleep(2000);
    return 1;
}

