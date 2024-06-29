#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <synchapi.h>

#include "fileHandler.h"
#include "utilityHeader.h"
#include "displayHandler.h"
#include "bookHandler.h"
#include "userHandler.h"

//Display functions
void login();
void openBookManagementPage();
void openUserManagementPage();
void resetScreen();
void mainScreen();

static char *currentlyAuthStudentName; // A char to hold the logged in student name
static int isLoggedIn = 0; // A bool for the current status of the student

//Structs for better management

//Variables to store the wished book and author name for faster searching
static char wishedAuthor[50];

int main(int argc, char *argv[])
{
    login(argc, argv);
}

void login(int argc, char *argv[])
{
    resetScreen();
    welcomeUser();

    char studentName[50], firstName[50], lastName[50];
    char wordSeparator[] = " ";
    char *words[3];
    int currentWordCount = 0;

    if (argc == 4)
    {
        strcpy(firstName, argv[2]);
        strcpy(lastName, argv[3]);

        strcpy(studentName, firstName);
        strcat(studentName, lastName);

        copyCurrentAuthStudent(firstName, lastName, currentlyAuthStudentName);
    }
    else
    {
        gets(studentName);
        if (strcmp(studentName, "") == 0)
        {
            errorMessage("Invalid format!");
            login(argc, argv);
            return;
        }

        // We break down our input arguments
        words[currentWordCount] = strtok(studentName, wordSeparator);
        while (words[currentWordCount] != NULL)
        {
            words[++currentWordCount] = strtok(NULL, wordSeparator);
        }

        // We didn't input a string under the form of "login [FNAME] [LNAME]"
        if (!verifyArguments(words, currentWordCount))
        {
            login(argc, argv);
            return;
        }

        strcpy(firstName, words[1]);
        strcpy(lastName, words[2]);

        strcpy(studentName, firstName);
        strcat(studentName, lastName);


        copyCurrentAuthStudent(firstName, lastName, currentlyAuthStudentName);
    }

    //The file is empty
    if(checkEmptyFile() == 1)
    {
        registerStudent(currentlyAuthStudentName);

        updateStudentList();
        updateLoansStruct();
        updateBookList();

        Sleep(2000);

        mainScreen();
        return;
    }

    isLoggedIn = findStudent(firstName, lastName, studentName);

    if(isLoggedIn)
    {
        updateStudentList();
        updateBookList();
        updateLoansStruct();

        displayLoggedIn();
        Sleep(2000);
        mainScreen();
    }
    else
    {
        noUserFoundMessage(currentlyAuthStudentName);

        pressAnyKey();
        login(argc, argv);
    }
}


void mainScreen()
{
    resetScreen();
    displayMainPage();

    int choice = 0;
    scanf("%d", &choice);

    fflush(stdin);

    switch(choice)
    {
        case 1:
            openUserManagementPage();
            break;
        case 2:
            openBookManagementPage();
            break;
        case 3:
            endScreen();
            break;
        default:
            errorMessage("You must input a number between 1-3 !");
            mainScreen();
    }
}

void openUserManagementPage()
{
    resetScreen();

    displayUserManagementPage();

    int choice;
    scanf("%d", &choice);

    fflush(stdin);

    switch(choice)
    {
        case 1:
            resetScreen();

            //We didn't add a user successfully
            if(addUser(numberOfStudents, students) == -1)
            {
                mainScreen();
            }
            else
            {
                updateStudentList();
                openUserManagementPage();
            }

            break;
        case 2:
            mainScreen();
            break;
        default:
            errorMessage("You must introduce a number between 1-2 !");
            openUserManagementPage();
    }
}

void openBookManagementPage()
{
    resetScreen();
    displayBookManagementPage();

    int choice;
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            borrowBook(currentlyAuthStudentName);
            break;
        case 2:
            donateBook();
            break;
        case 3:
            returnBook(currentlyAuthStudentName);
            break;
        case 4:
            showLoanedBooks(currentlyAuthStudentName);
            mainScreen();
            break;
        case 5:
            bookSearch();
            break;
        case 6:
            mainScreen();
            break;
        default:
            errorMessage("You must introduce a number between 1-6 !");
            openUserManagementPage();
    }
    openBookManagementPage();
}

