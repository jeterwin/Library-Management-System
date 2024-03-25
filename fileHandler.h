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

#endif //LIBRARYSYSTEM_FILEHANDLER_H
