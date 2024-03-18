#ifndef LIBRARYSYSTEM_FILEHANDLER_H
#define LIBRARYSYSTEM_FILEHANDLER_H

FILE * openFile(char[], char []);

FILE * openFile(char fileName[10], char mode[5])
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

int checkEmptyFile(FILE *);

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
#endif //LIBRARYSYSTEM_FILEHANDLER_H
