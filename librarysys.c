#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <synchapi.h>
#include "bookStruct.h"
#include "studentStruct.h"
#include "loansStruct.h"
#include "fileHandler.h"

//Display functions
void login();
void openBookManagementPage();
void openUserManagementPage();
void resetScreen();
void endScreen();
void pressAnyKey();
void mainScreen();

//Book functions
void borrowBook();
void donateBook();
void showLoanedBooks();
void bookSearch();
void addNewLoanEntry();
void removeBookQuantity();
void returnBook();
void searchBookByName(char [50]);
void displayAllBooksByAuthor(char [50]);
void searchBookByNameAndAuthor(char [50], char [50]);
void updateBookList();
void updateLoans();

//User functions
void verifyArguments(char *[], int);
void registerStudent(FILE *);
void addUser();
void updateStudentList();
void copyCurrentAuthStudent(char *[]);
int checkValidity(char [], char[]);

char *currentlyAuthStudentName;
int isLoggedIn = 0; // A bool for the current status of the student

char fUsersName[] = "users.txt"; // The name of the file which holds the users
char fBooksName[] = "books.txt";
char fLoansName[] = "borrows.txt";
char fTempName[] = "temporary.txt";

char bookWritingPattern[] = "%s, %s, %d\n"; // Name, name, number
char bookReadingPattern[] = "%[^,], %[^,], %d\n"; // ~ Name Name, Name Name, number

char userWritingPattern[] = "%s %s\n"; //Name Name
char userReadingPattern[] = "%[^ ] %[^\n]\n"; // ~ Name Name

char loansWritingPattern[] = "%s, %s, %s\n";
char loansReadingPattern[] = "%[^,], %[^,], %[^\n]\n"; // ~ Name Name, Name Name, Name Name

struct Student * students;
struct Loans * loans;
struct Book * books;

char wishedBook[50], bookAuthor[50];
int bookStock = 0, numberOfStudents = 0, numberOfBooks = 0, numberOfLoans = 0;

int main()
{
    login();
}

void login()
{
    resetScreen();

    printf("---------------------\n");
    printf("- Enter Credentials -\n");
    printf("---------------------\n");
    printf("Enter your surname and firstname under the following form:"
           "\n'login [FIRSTNAME] [SURNAME]'\nHit ENTER to confirm. \n");

    char studentName[50], wordSeparator[] = " ";
    char *words[10];
    int i = 0;

    gets(studentName);
    if(strcmp(studentName, "") == 0)
    {
        printf("Invalid format!\n");
        Sleep(1000);
        login();
    }

    //We break down our input arguments
    words[i] = strtok(studentName, wordSeparator);
    while(words[i] != NULL)
    {
        words[++i] = strtok(NULL, wordSeparator);
    }

    verifyArguments(words, i);

    copyCurrentAuthStudent(words);

    strcpy(studentName, strcat(words[1], words[2]));

    FILE *fUsers = openFile(fUsersName, "r+");
    //The file is empty
    if(checkEmptyFile(fUsers) == 1)
    {
        registerStudent(fUsers);
        return;
    }

    int foundUser = 0;
    char fname[50], lname[50];
    while(fscanf(fUsers, userReadingPattern, fname, lname) != EOF)
    {
        numberOfStudents++;
        if(strcmp(studentName, strcat(fname, lname)) == 0)
        {
            foundUser = 1;
        }
    }
    fclose(fUsers);

    isLoggedIn = foundUser;
    if(isLoggedIn)
    {
        updateStudentList();
        updateBookList();
        updateLoans();

        printf("\nSuccessfully logged in!\n");
        Sleep(2000);
        mainScreen();
    }
    else
    {
        printf("There has no user with the name '%s' found in the database, try again.", currentlyAuthStudentName);
        free(currentlyAuthStudentName);

        pressAnyKey();
        login();
    }
}

void copyCurrentAuthStudent(char *words[3])
{
    //We assign the last position + 1 of our first name the null terminator then change
    //the value of the last position to an empty space, allowing us to concatenate the
    //last name on top of the first name
    currentlyAuthStudentName = malloc(sizeof(words[1]) + sizeof(words[2]) + 1);
    strcpy(currentlyAuthStudentName, words[1]);
    currentlyAuthStudentName[strlen(currentlyAuthStudentName) + 1] = '\0';
    currentlyAuthStudentName[strlen(currentlyAuthStudentName)] = ' ';
    strcat(currentlyAuthStudentName, words[2]);
}

void updateBookList()
{
    numberOfBooks = 0;

    FILE * fBooks = openFile(fBooksName, "r");
    char _bookName[50], _authorName[50];
    int quantity;

    while(fscanf(fBooks, bookReadingPattern, _bookName, _authorName, &quantity) != EOF)
    {
        numberOfBooks++;
    }
    fclose(fBooks);

    books = malloc(numberOfBooks * sizeof(struct Book));

    fBooks = openFile(fBooksName, "r");
    fflush(fBooks);
    int i = 0;
    while(fscanf(fBooks, bookReadingPattern, _bookName, _authorName, &quantity) != EOF)
    {
        strcpy(books[i].bookName, _bookName);
        strcpy(books[i].authorName, _authorName);
        books[i].copiesAvailable = quantity;
        i++;
    }

    fclose(fBooks);
}

void updateLoans()
{
    numberOfLoans = 0;

    FILE * fLoans = openFile(fLoansName, "r");
    char _borrowerName[50], _bookName[50], _authorName[50];

    while(fscanf(fLoans, loansReadingPattern, _borrowerName, _bookName, _authorName) != EOF)
    {
        numberOfLoans++;
    }

    fclose(fLoans);

    loans = malloc(numberOfLoans * sizeof(struct Loans));

    fLoans = openFile(fLoansName, "r");;

    int i = 0;
    while(fscanf(fLoans, loansReadingPattern, _borrowerName, _bookName, _authorName) != EOF)
    {
        strcpy(loans[i].borrowerName, _borrowerName);
        strcpy(loans[i].bookName, _bookName);
        strcpy(loans[i].authorName, _authorName);
        i++;
    }

    fclose(fLoans);
}

void updateStudentList()
{
    numberOfStudents = 0;
    char _firstName[50], _lastName[50];
    FILE *fUsers = openFile(fUsersName, "r");

    while(fscanf(fUsers, userReadingPattern, _firstName, _lastName) != EOF)
    {
        numberOfStudents++;
    }

    fclose(fUsers);
    students = malloc(numberOfStudents * sizeof(struct Student));

    fUsers = openFile(fUsersName, "r");

    int i = 0;
    while(fscanf(fUsers, userReadingPattern, _firstName, _lastName) != EOF)
    {
        strcpy(students[i].firstName, _firstName);
        strcpy(students[i].lastName, _lastName);
        i++;
    }

    fclose(fUsers);
}

void registerStudent(FILE * fUsers)
{
    char firstName[50], lastName[50];

    printf("Looks like the library's database is empty.\n");
    printf("Enter your first name below:\n");
    gets(firstName);

    printf("Enter your last name below:\n");
    gets(lastName);

    if(strcmp(firstName, "") == 0 || strcmp(lastName, "") == 0)
    {
        printf("You did not provide a first or a last name!\n");
        Sleep(2000);
        registerStudent(fUsers);
    }

    fseek(fUsers, 0, SEEK_SET);
    fprintf(fUsers, userWritingPattern, firstName, lastName);
    fclose(fUsers);

    printf("Successfully added in database!\nWelcome %s %s!\n", firstName, lastName);

    updateStudentList();
    updateLoans();
    updateBookList();

    Sleep(2000);

    mainScreen();
}

void verifyArguments(char *words[], int length)
{
    //We check to see if the first argument of our login string is "login"
    //and if the length of our arguments are 3, respecting the "login [FNAME] [LNAME]" pattern.
    if(strcmp(words[0], "login") != 0 || length != 3)
    {
        printf("\nInvalid format!\n");
        Sleep(1000);
        login();
    }
}

void mainScreen()
{
    resetScreen();

    printf("--------------------------\n");
    printf("--      Welcome to      --\n");
    printf("--  The Library System! --\n");
    printf("--------------------------\n\n");
    printf("Type the index of the system that you want to access, then press ENTER!\n");
    printf("> 1. User Management Page\n");
    printf("> 2. Library Management Page\n");
    printf("> 3. Quit App\n");

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
            printf("You must input a number between 1-3!\n");
            Sleep(2000);
            mainScreen();
    }
}

void addUser()
{
    resetScreen();

    char firstName[50], lastName[50];
    int foundDuplicate = 0;
    printf("Enter the first name of the new student which you wish to add in the database:\n");
    gets(firstName);

    printf("Enter the student's last name:\n");
    gets(lastName);

    if(checkValidity(firstName, lastName) == 1)
    {
        printf("You must enter a first name and a last name!\n");
        Sleep(3000);
        mainScreen();
    }
    //Check for duplicates
    for(int i = 0; i < numberOfStudents; i++)
    {
        //We found a duplicate
        if(strcmp(firstName, students[i].firstName) == 0 && strcmp(lastName, students[i].lastName) == 0)
        {
            printf("\nThere already exists an user with the same name!\n");
            Sleep(3000);
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

        updateStudentList();

        printf("Successfully added a new entry in the database!\n");

        Sleep(2000);
    }

    openUserManagementPage();
}

void resetScreen()
{
    system("cls");
    fflush(stdin);
}

void bookSearch()
{
    resetScreen();

    FILE * fBooks = openFile(fBooksName, "r");
    int foundBooks = 0;
    char bookName[50], authorName[50];

    printf("Enter the name of the book you would like to search:\n");
    gets(bookName);

    printf("Enter an author name:\n");
    gets(authorName);

    fflush(stdin);

    //We shall conduct searches based on book name or author name, or both. If the author name is
    //empty, then we will only search a book by its name, if we are searching by book names
    //then we shall get returned a list with all books written by that author.

    //We provided a book name and author name
    if(strcmp(authorName, "\0") != 0 && strcmp(bookName, "\0") != 0)
    {
        for(int i = 0; i <= numberOfBooks; i++)
        {
            int foundBookName = strcmp(books[i].bookName, bookName);
            int foundAuthorName = strcmp(books[i].authorName, authorName);
            if(foundBookName == 0 && foundAuthorName == 0)
            {
                foundBooks = 1;
                books[i].copiesAvailable == 0 ?
                printf("The book is out of stock, try searching for another one.\n") :
                printf("The book is in stock, there are %d copies available.\n", books[i].copiesAvailable);

                break;
            }
        }
        //We didn't find any books
        if(foundBooks == 0)
        {
            printf("There are no books named '%s' written by '%s' available in the library.\n",
                   bookName, authorName);
        }
    }
    //We provided a book name
    else if(strcmp(bookName, "\0") != 0)
    {
        for(int i = 0; i <= numberOfBooks; i++)
        {
            int found = strcmp(books[i].bookName, bookName);
            if(found == 0)
            {
                foundBooks = 1;
                books[i].copiesAvailable == 0 ?
                printf("The book is out of stock, try searching for another one.\n") :
                printf("The book is in stock and it is written by"
                       " '%s', there are %d copies available.\n",
                       books[i].authorName, books[i].copiesAvailable);

                break;
            }
        }

        if(foundBooks == 0)
        {
            printf("There are no books named '%s' available in the library.\n", bookName);
        }

    }
        //We provided an author name
    else if(strcmp(authorName, "\0") != 0)
    {
        printf("These are all the books available written by '%s':\n\n", authorName);

        for(int i = 0; i <= numberOfBooks; i++)
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
            printf("\nWrite down the book you're interested in and go borrow it!\n");
        }

    }
    else
    {
        printf("You should at least provide a book name or an author name!\n");
    }
    fclose(fBooks);

    pressAnyKey();
    openBookManagementPage();
}

void showLoanedBooks()
{
    resetScreen();

    FILE * fLoans = openFile(fLoansName, "r");

    char _bookName[50], _authorName[50], _borrowerName[50];
    int found = 1, booksBorrowed = 0;

    while(fscanf(fLoans, loansReadingPattern, _borrowerName, _bookName, _authorName) != EOF)
    {
        found = strcmp(_borrowerName, currentlyAuthStudentName);

        //For the first book we would like to send a different message than with the rest
        if(found == 0 && booksBorrowed == 0)
        {
            printf("These are all the books you have loaned so far!\n\n");
            printf("Book Name: %s\nAuthor Name: %s\n", _bookName, _authorName);
            booksBorrowed++;
        }
        else if(found == 0)
        {
            printf("Book Name: %s\nAuthor Name: %s\n", _bookName, _authorName);
            booksBorrowed++;
        }
    }
    fclose(fLoans);

    if(booksBorrowed == 0)
    {
        printf("You have not borrowed any books!\n");
    }

    pressAnyKey();
    mainScreen();
}

void donateBook()
{
    resetScreen();

    char bookName[50], authorName[50];
    int bookQuantity, foundBookName = 1, foundAuthorName = 1;

    printf("Enter the name of the book you wish to donate:\n");
    gets(bookName);

    printf("Enter the author of the book:\n");
    gets(authorName);

    if(checkValidity(bookName, authorName) == 1)
    {
        printf("Not a valid book name!");
        Sleep(2000);
        openBookManagementPage();
    }

    printf("Enter the amount of books you would like to donate:\n");
    scanf("%d", &bookQuantity);

    for(int i = 0; i < numberOfBooks; i++)
    {
        foundBookName = strcmp(books[i].bookName, bookName);
        foundAuthorName = strcmp(books[i].authorName, authorName);

        if(foundBookName == 0 && foundAuthorName == 0)
        {
            books[i].copiesAvailable += bookQuantity;
            break;
        }
    }

    FILE * fBook = openFile(fBooksName, "w");
    //There was no duplicate book found in our library
    for(int i = 0; i < numberOfBooks; i++)
    {
        fprintf(fBook, bookWritingPattern,
                books[i].bookName, books[i].authorName, books[i].copiesAvailable);
    }

    if((foundBookName == foundAuthorName && foundBookName != 0) ||
            (foundBookName != foundAuthorName))
    {
        fprintf(fBook, bookWritingPattern,
                bookName,authorName, bookQuantity);
    }

    fclose(fBook);

    updateBookList();

    printf("\nSuccessfully added %s to the library's database!\n", bookName);
    printf("Would you like to add another book? [Y/N]\n");

    fflush(stdin);

    char choice;
    scanf("%c", &choice);

    if(choice == 'y' || choice == 'Y')
    {
        donateBook();
    }
    else
    {
        openBookManagementPage();
    }
}

int checkValidity(char str1[], char str2[])
{
    return (strcmp(str1, "") == 0) || (strcmp(str2, "") == 0);
}

void borrowBook()
{
    resetScreen();

    char bookName[50], authorName[50];

    printf("Enter the name of the book you would like to borrow:\n");
    gets(bookName);

    printf("Enter the author of the book:\n");
    gets(authorName);

    fflush(stdin);

    //We shall conduct searches based on book name or author name, or both. If the author name is
    //empty, then we will only search a book by its name, if we are searching by book names
    //then we shall get returned a list with all books written by that author.

    //We provided a book name and author name
    if(strcmp(bookName, "\0") != 0 && strcmp(authorName, "\0") != 0)
    {
        searchBookByNameAndAuthor(bookName, authorName);
    }
    //We provided a book name
    else if(strcmp(bookName, "\0") != 0)
    {
        searchBookByName(bookName);
    }
    //We provided an author name
    else if(strcmp(authorName, "\0") != 0)
    {
        displayAllBooksByAuthor(authorName);
    }
    else
    {
        printf("You should at least provide a book name or an author name!\n");
    }

    pressAnyKey();
    openBookManagementPage();
}

void searchBookByNameAndAuthor(char bookName[50], char authorName[50])
{
    int foundBooks = 0;

    for(int i = 0; i < numberOfBooks; i++)
    {
        int foundBookName = strcmp(books[i].bookName, bookName);
        int foundAuthorName = strcmp(books[i].authorName, authorName);

        //We found a book
        if(foundBookName == 0 && foundAuthorName == 0)
        {
            foundBooks++;
            strcpy(wishedBook, books[i].bookName);
            strcpy(bookAuthor, books[i].authorName);
            bookStock = books[i].copiesAvailable;
            if(bookStock == 0)
            {
                printf("The book is out of stock, try searching for another one.\n");
            }
            else
            {
                printf("The book is in stock, there are %d available left, you have"
                       " successfully loaned the book!", bookStock);

                Sleep(2000);

                addNewLoanEntry();

                removeBookQuantity();
            }
            break;
        }
    }
    //We didn't find any books
    if(foundBooks == 0)
    {
        printf("There are no books named '%s' written by '%s' available in the library.\n",
               bookName, authorName);
    }

    pressAnyKey();
    openBookManagementPage();
}

void displayAllBooksByAuthor(char authorName[50])
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

    pressAnyKey();
    openBookManagementPage();
}

void searchBookByName(char bookName[50])
{
    int foundBooks = 0;

    for(int i = 0; i < numberOfBooks; i++)
    {
        int found = strcmp(bookName, books[i].bookName);

        if(found == 0)
        {
            foundBooks++;
            strcpy(wishedBook, books[i].bookName);
            strcpy(bookAuthor, books[i].authorName);
            bookStock = books[i].copiesAvailable;

            printf("There is a book named '%s' written by '%s' available.\n",
                   books[i].bookName, books[i].authorName);
            printf("Is this the book you were looking for? [Y/N]\n");

            char ch;
            scanf("%c", &ch);
            if(ch == 'y' || ch == 'Y')
            {
                if(bookStock == 0)
                {
                    printf("The book is currently out of stock, try searching for another one.\n");

                    pressAnyKey();
                    openBookManagementPage();
                }
                else
                {
                    printf("The book is in stock, there are %d available left, you have"
                           " successfully loaned the book!", bookStock);

                    Sleep(3000);

                    addNewLoanEntry();

                    removeBookQuantity();
                }
                break;
            }
            else
            {
                printf("\nPlease mention the author of the book as well.\n");
                break;
            }
        }
    }

    if(foundBooks == 0)
    {
        printf("There are no books named '%s' available in the library.\n", bookName);
    }

    pressAnyKey();
    openBookManagementPage();
}

void pressAnyKey()
{
    fflush(stdin);

    printf("\nPress any key to continue.\n");
    char ch;
    scanf("%c", &ch);
}

//We will update our entry inside the books file and decrease the available quantity by 1
void removeBookQuantity()
{
    fflush(stdin);

    FILE * fTemp = openFile(fTempName, "r+");

    int found = 1;

    for(int i = 0; i < numberOfBooks; i++)
    {
        found = strcmp(books[i].bookName, wishedBook) != 0 && strcmp(books[i].authorName, bookAuthor) != 0;

        if(found != 0)
        {
            fprintf(fTemp, "%s, %s, %d\n",
                    books[i].bookName, books[i].authorName, books[i].copiesAvailable);
        }
        else
        {
            bookStock--;
            fprintf(fTemp, bookWritingPattern,
                    books[i].bookName, books[i].authorName, bookStock);
        }
    }

    fclose(fTemp);
    //We need to open our original books file in write mode in order to clear the file from its content
    FILE * fBooks = openFile(fBooksName, "w");
    fclose(fBooks);

    //We open our original file in append mode and copy everything from our temp file
    fBooks = openFile(fBooksName, "a");
    fTemp = openFile(fTempName, "r");

    for(int i = 0; i < numberOfBooks; i++)
    {
        fprintf(fBooks, bookWritingPattern,
                books[i].bookName, books[i].authorName, books[i].copiesAvailable);
    }

    fclose(fBooks);
    fclose(fTemp);

    //We open the temp file in write mode to clear it of its content so its prepared for its next book
    fTemp = openFile(fTempName, "w");
    fclose(fTemp);
    //We free the memory allocated to the concatenated book name pointer

    printf("\nWould you want to loan another book? [Y/N]\n");
    char choice;
    scanf("%c", &choice);
    if(choice == 'y' || choice == 'Y')
    {
        borrowBook();
    }
    else
    {
        openBookManagementPage();
    }
}

//We are inserting a new entry in our text with loans for every book that is borrowed by each student
void addNewLoanEntry()
{
    FILE * fLoans = openFile(fLoansName, "a+");

    fprintf(fLoans, loansWritingPattern, currentlyAuthStudentName, wishedBook, bookAuthor);
    fclose(fLoans);
}

void openUserManagementPage()
{
    resetScreen();

    printf("-------------------\n");
    printf("- User Management -\n");
    printf("-------------------\n\n");
    printf("> 1. Add User\n");
    printf("> 2. Return to main menu\n");

    int choice;
    scanf("%d", &choice);

    fflush(stdin);

    switch(choice)
    {
        case 1:
            addUser();
            break;
        case 2:
            mainScreen();
            break;
        default:
            printf("You must introduce a number between 1-2!\n");
            Sleep(2000);
            openUserManagementPage();
    }
}

void openBookManagementPage()
{
    resetScreen();

    printf("-------------------\n");
    printf("- Book Management -\n");
    printf("-------------------\n\n");
    printf("> 1. Borrow a Book\n");
    printf("> 2. Donate a Book\n");
    printf("> 3. Return a Book\n");
    printf("> 4. Show Borrowed Books\n");
    printf("> 5. Search For a Book\n");
    printf("> 6. Return to the Main Menu\n");

    int choice;
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            borrowBook();
            break;
        case 2:
            donateBook();
            break;
        case 3:
            returnBook();
            break;
        case 4:
            showLoanedBooks();
            break;
        case 5:
            bookSearch();
            break;
        case 6:
            mainScreen();
            break;
        default:
            printf("You must introduce a number between 1-6!\n");
            Sleep(2000);
            openUserManagementPage();
    }
}

void returnBook()
{
    resetScreen();

    char bookName[50], authorName[50];
    printf("Enter the name of the book you would like to return:\n");
    gets(bookName);

    printf("Enter the book's author:\n");
    gets(authorName);

    int foundUser = 1, foundBook = 1, foundAuthor = 1, returnedBook = 0;

    for(int i = 0; i < numberOfLoans; i++)
    {
        foundUser = strcmp(loans[i].borrowerName, currentlyAuthStudentName);
        foundBook = strcmp(loans[i].bookName, bookName);
        foundAuthor = strcmp(loans[i].authorName, authorName);

        if(foundUser == 0 && foundBook == foundUser && foundAuthor == foundUser && returnedBook == 0)
        {
            strcpy(loans[i].authorName, "");
            strcpy(loans[i].borrowerName, "");
            strcpy(loans[i].bookName, "");

            printf("Successfully returned the book!\n");
            returnedBook = 1;
        }
    }

    if(returnedBook)
    {
        FILE * fLoans = openFile(fLoansName, "w");

        for(int i = 0; i < numberOfLoans; i++)
        {
            //We found the erased book
            if(strcmp(loans[i].bookName, "") == 0)
            {
                continue;
            }
            fprintf(fLoans, loansWritingPattern, loans[i].borrowerName, loans[i].bookName,
                    loans[i].authorName);
        }
        fclose(fLoans);

        updateLoans();

        printf("Would you like to return another book? [Y/N]\n");

        char ch;
        scanf("%c", &ch);
        if(ch == 'y' || ch == 'Y')
        {
            returnBook();
        }
        else
        {
            openBookManagementPage();
        }
    }
    else
    {
        printf("You didn't borrow a book named '%s', written by '%s'.\n", bookName, authorName);

        pressAnyKey();
        openBookManagementPage();
    }
}

void endScreen()
{
    printf("Bye bye.");
    Sleep(1000);
    exit(0);
}
