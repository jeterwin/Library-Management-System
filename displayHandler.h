#ifndef LIBRARYSYSTEM_DISPLAYHANDLER_H
#define LIBRARYSYSTEM_DISPLAYHANDLER_H

#include "bookStruct.h"

void welcomeUser();

void displayMainPage();

void noUserFoundMessage(char * studentName);

void displayAllBooksByAuthor(char authorName[], struct Book * books, int numberOfBooks);

void pressAnyKey();

void displayLoggedIn();

void errorMessage(char * message);

void displayUserManagementPage();

void displayBookManagementPage();

void endScreen();

#endif //LIBRARYSYSTEM_DISPLAYHANDLER_H
