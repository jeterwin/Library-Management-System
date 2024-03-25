#include <stdio.h>

#include "fileHandler.h"

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
