#include <stdio.h>
#include <string.h>
#include <synchapi.h>

#include "displayHandler.c"

int findStudent(FILE * fUsers, char userReadingPattern[], char firstName[], char lastName[],
                char studentName[])
{
    while(fscanf(fUsers, userReadingPattern, firstName, lastName) != EOF)
    {
        //We are comparing our student name with the first name and last name concatenated from the file
        if(strcmp(studentName, strcat(firstName, lastName)) == 0)
        {
            return 1;
        }
    }

    return 0;
}
FILE * openFile(char fileName[], char mode[])
{
    FILE * file = fopen(fileName, mode);
    if(file == NULL)
    {
    //If the file does not exist open it in append mode in order to create it at run-time
    file = fopen(fileName, "a");
    openFile(fileName, mode);
    }
    return file;
}

int checkEmptyFile(FILE * file)
{
    fseek(file, 0, SEEK_END); // goto end of file
    if (ftell(file) == 0)
    {
        return 1;
    }
    fseek(file, 0, SEEK_SET); // goto begin of file
    return 0;
}

int verifyArguments(char *words[], int length)
{
    //We check to see if the first argument of our login string is "login"
    //and if the length of our arguments are 3, respecting the "login [FNAME] [LNAME]" pattern.
    if(strcmp(words[0], "login") != 0 || length != 3)
    {
        errorMessage("Invalid format!");
        return 0;
    }
    return 1;
}

int checkValidity(char str1[], char str2[])
{
    return (strcmp(str1, "") == 0) || (strcmp(str2, "") == 0);
}

int addUser(char fUsersName[], char userWritingPattern[], int numberOfStudents, struct Student * students)
{
    char firstName[50], lastName[50];
    int foundDuplicate = 0;
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
    for(int i = 0; i < numberOfStudents; i++)
    {
        //We found a duplicate
        if(strcmp(firstName, students[i].firstName) == 0 && strcmp(lastName, students[i].lastName) == 0)
        {
            errorMessage("There already exists an user with the same name!");
            foundDuplicate = 1;
            break;
        }
    }

    //If we didn't find a duplicate
    if(!foundDuplicate)
    {
        FILE * fUsers = openFile(fUsersName, "a");

        fprintf(fUsers, userWritingPattern, firstName, lastName);

        fclose(fUsers);

        printf("Successfully added a new entry in the database!\n");

        Sleep(2000);
        return 1;
    }
}

