#ifndef LIBRARY_MANAGEMENT_SYSTEM_UTILITYHEADER_H
#define LIBRARY_MANAGEMENT_SYSTEM_UTILITYHEADER_H

#include <stdio.h>

//Library functions
void displayAllBooksByAuthor(char authorName[], struct Book * books, int numberOfBooks);

void displayAllBooks();

int findBook(struct Book *, int, char[], char[]);

void writeBooksToFile(struct Book *, int);

//We will update our entry inside the books file and decrease the available quantity by 1
int removeBookQuantity(struct Book *, int, char[], char[]);

//Loan functions

int searchForLoan(int, struct Loans *, char [],
                  char [], char []);

//We are inserting a new entry in our text with loans for every book that is borrowed by each student
void addNewLoanEntry(char [], char[], char []);

//User functions
void writeStudentToFile(char[], char[]);

int checkValidity(char str1[], char str2[]);

int verifyArguments(char *words[], int length);

int findStudent(char firstName[], char lastName[], char studentName[]);

int addUser(int numberOfStudents, struct Student * students);

int findStudentDuplicate(struct Student *, int, char[], char[]);


#endif //LIBRARY_MANAGEMENT_SYSTEM_UTILITYHEADER_H
