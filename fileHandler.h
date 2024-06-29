#ifndef LIBRARYSYSTEM_FILEHANDLER_H
#define LIBRARYSYSTEM_FILEHANDLER_H

#include <stdio.h>

static char fUsersName[] = "users.txt"; // The name of the file which holds the users
static char fBooksName[] = "books.txt";
static char fLoansName[] = "borrows.txt";

static char bookWritingPattern[] = "%s, %s, %d\n"; // Name, name, number
static char bookReadingPattern[] = "%[^,], %[^,], %d\n"; // ~ ,mkName Name, Name Name, number

static char userWritingPattern[] = "%s %s\n"; //Name Name
static char userReadingPattern[] = "%[^ ] %[^\n]\n"; // ~ Name Name

static char loansWritingPattern[] = "%s, %s, %s\n";
static char loansReadingPattern[] = "%[^,], %[^,], %[^\n]\n"; // ~ Name Name, Name Name, Name Name

FILE * openFile(char[], char []);

int checkEmptyFile();

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

int checkEmptyFile()
{
    FILE *fUsers = openFile(fUsersName, "r+");

    fseek(fUsers, 0, SEEK_END); // goto end of file
    if (ftell(fUsers) == 0)
    {
        fclose(fUsers);
        return 1;
    }
    fclose(fUsers);
    return 0;
}

#endif //LIBRARYSYSTEM_FILEHANDLER_H
