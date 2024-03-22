#ifndef LIBRARYSYSTEM_FILEHANDLER_H
#define LIBRARYSYSTEM_FILEHANDLER_H

#include <stdio.h>

FILE * openFile(char[], char []);

int checkEmptyFile(FILE * file);

void registerStudent(FILE * fUsers, char userWritingPattern[]);

int checkValidity(char str1[], char str2[]);

int addUser(char fUsersName[], char userWritingPattern[], int numberOfStudents, struct Student * students);

int findStudent(FILE * fUsers, char userReadingPattern[], char firstName[], char lastName[],
                char studentName[]);

int verifyArguments(char *words[], int length);

#endif //LIBRARYSYSTEM_FILEHANDLER_H
