#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <synchapi.h>

#include "fileHandler.c"
#include "utilityClass.c"
#include "displayHandler.c"

//Display functions
void login();
void openBookManagementPage();
void openUserManagementPage();
void resetScreen();
void mainScreen();

//Book functions
void borrowBook();
void donateBook();
void showLoanedBooks();
void bookSearch();
void returnBook();
void searchBookByName(char [50]);
void searchBookByNameAndAuthor(char [50], char [50]);
void updateBookList();
void updateLoans();

//User functions
void updateStudentList();
void copyCurrentAuthStudent(char *, char *);

void registerStudent();

void findReturnedBook();

static char *currentlyAuthStudentName; // A char to hold the logged in student name
static int isLoggedIn = 0; // A bool for the current status of the student

//Structs for better management
static struct Student * students;
static struct Loans * loans;
static struct Book * books;

//Variables to store the wished book and author name for faster searching
static char wishedBook[50], wishedAuthor[50];
static int bookStock = 0, numberOfStudents = 0, numberOfBooks = 0, numberOfLoans = 0;

int main()
{
    login();
}

void login()
{
    resetScreen();
    welcomeUser();

    char studentName[50], wordSeparator[] = " ";
    char * words[3];
    int currentWordCount = 0;

    gets(studentName);
    if(strcmp(studentName, "") == 0)
    {
        errorMessage("Invalid format!");
        login();
        return;
    }

    //We break down our input arguments
    words[currentWordCount] = strtok(studentName, wordSeparator);
    while(words[currentWordCount] != NULL)
    {
        words[++currentWordCount] = strtok(NULL, wordSeparator);
    }

    //We didn't input a string under the form of "login [FNAME] [LNAME]"
    if(!verifyArguments(words, currentWordCount))
    {
        login();
        return;
    }

    copyCurrentAuthStudent(words[1], words[2]);

    strcpy(studentName, strcat(words[1], words[2]));
    //The file is empty
    if(checkEmptyFile() == 1)
    {
        registerStudent();

        updateStudentList();
        updateLoans();
        updateBookList();

        Sleep(2000);

        mainScreen();
        return;
    }

    char firstName[50], lastName[50];
    isLoggedIn = findStudent(firstName, lastName, studentName);

    if(isLoggedIn)
    {
        updateStudentList();
        updateBookList();
        updateLoans();

        displayLoggedIn();
        Sleep(2000);
        mainScreen();
    }
    else
    {
        noUserFoundMessage(currentlyAuthStudentName);

        pressAnyKey();
        login();
    }
}

void registerStudent()
{
    FILE * fUsers = openFile(fUsersName, "a+");
    char firstName[50], lastName[50];

    fflush(fUsers);
    printf("Looks like the library's database is empty.\n");
    printf("Enter your first name below:\n");
    gets(firstName);

    printf("Enter your last name below:\n");
    gets(lastName);

    //Invalid input found
    if(checkValidity(firstName, lastName) == 1)
    {
        errorMessage("You did not provide a first or a last name!");
        Sleep(2000);
        registerStudent();
        return;
    }

    //Set the file back at the start of a line (we moved the position)
    writeStudentToFile(firstName, lastName);

    copyCurrentAuthStudent(firstName, lastName);

    printf("Successfully added in database!\nWelcome %s %s!\n", firstName, lastName);
}

void copyCurrentAuthStudent(char *firstName, char *lastName)
{
    //We assign the last position + 1 of our first name the null terminator then change
    //the value of the last position to an empty space, allowing us to concatenate the
    //last name on top of the first name
    free(currentlyAuthStudentName);

    currentlyAuthStudentName = malloc(sizeof(firstName) + sizeof(lastName) + 1);
    strcpy(currentlyAuthStudentName, firstName);
    currentlyAuthStudentName[strlen(currentlyAuthStudentName) + 1] = '\0';
    currentlyAuthStudentName[strlen(currentlyAuthStudentName)] = ' ';
    strcat(currentlyAuthStudentName, lastName);
}

void updateBookList()
{
    free(books);
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
    free(loans);
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
    free(students);

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
        errorMessage("You should at least provide a book name or an author name!");
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
            printf("Book Name: %s\nAuthor Name: %s\n\n", _bookName, _authorName);
            booksBorrowed++;
        }
        else if(found == 0)
        {
            printf("Book Name: %s\nAuthor Name: %s\n\n", _bookName, _authorName);
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
        errorMessage("Not a valid book name!");
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

    printf("\nSuccessfully added '%s' to the library's database!\n", bookName);
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
        displayAllBooksByAuthor(authorName, books, numberOfBooks);
    }
    else
    {
        errorMessage("You should at least provide a book name or an author name!");
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

            if(books[i].copiesAvailable <= 0)
            {
                printf("The book is out of stock, try searching for another one.\n");
            }
            else
            {
                strcpy(wishedBook, bookName);
                strcpy(wishedAuthor, authorName);
                bookStock = books[i].copiesAvailable;

                printf("You have successfully loaned the book!");

                Sleep(2000);

                addNewLoanEntry(currentlyAuthStudentName, wishedBook, wishedAuthor);

                updateLoans();
                //There was an error or we don't want to borrow another book
                if(removeBookQuantity(books, numberOfBooks, wishedBook, wishedAuthor) == 0)
                {
                    openBookManagementPage();
                }
                else
                {
                    borrowBook();
                }
            }
            break;
        }
    }
    //We didn't find any books written with the specified arguments
    if(foundBooks == 0)
    {
        printf("There are no books named '%s' written by '%s' available in the library.\n",
               bookName, authorName);
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
            if(books[i].copiesAvailable <= 0)
            {
                printf("The book is currently out of stock, try searching for another one.\n");

                pressAnyKey();
                openBookManagementPage();
                return;
            }

            strcpy(wishedBook, books[i].bookName);
            strcpy(wishedAuthor, books[i].authorName);
            bookStock = books[i].copiesAvailable;

            foundBooks++;

            printf("There is a book named '%s' written by '%s' available.\n",
                   books[i].bookName, books[i].authorName);
            printf("Is this the book you were looking for? [Y/N]\n");

            char ch;
            scanf("%c", &ch);
            if(ch == 'y' || ch == 'Y')
            {
                printf("You have successfully loaned the book!");

                Sleep(3000);

                addNewLoanEntry(currentlyAuthStudentName, wishedBook, wishedAuthor);

                updateLoans();
                if(removeBookQuantity(books, numberOfBooks, wishedBook, wishedAuthor) == 0)
                {
                    openBookManagementPage();
                }
                else
                {
                    borrowBook();
                }
            }
            else
            {
                printf("\nPlease mention the author of the book as well.\n");
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

            int result = addUser(numberOfStudents, students);
            //We didn't add an user successfully
            if(result == -1)
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
            errorMessage("You must introduce a number between 1-6 !");
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

    strcpy(wishedBook, bookName);
    strcpy(wishedAuthor, authorName);

    if(checkValidity(wishedBook, wishedAuthor) == 1)
    {
        errorMessage("You must introduce the name of the book AND the author's name!\n");
        openBookManagementPage();
        return;
    }
    //We suppose we didn't find any matches so we initialize them with 1 (strcmp returns 0 on match)
    int foundUser = 1, foundBook = 1, foundAuthor = 1, returnedBook = 0;

    for(int i = 0; i < numberOfLoans; i++)
    {
        foundUser = strcmp(loans[i].borrowerName, currentlyAuthStudentName);
        foundBook = strcmp(loans[i].bookName, wishedBook);
        foundAuthor = strcmp(loans[i].authorName, wishedAuthor);

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
        findReturnedBook();

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

void findReturnedBook()
{
    for(int i = 0; i < numberOfBooks; i++)
    {
        int foundBook = strcmp(books[i].bookName, wishedBook);
        int foundAuthor = strcmp(books[i].authorName, wishedAuthor);

        if(foundBook == 0 && foundAuthor == 0)
        {
            books[i].copiesAvailable += 1;
            break;
        }
    }

    FILE * fBooks = openFile(fBooksName, "w");

    for(int i = 0; i < numberOfBooks; i++)
    {
        fprintf(fBooks, bookWritingPattern, books[i].bookName,
                books[i].authorName, books[i].copiesAvailable);
    }

    fclose(fBooks);

    updateBookList();
}
