//
// Created by Erwin on 6/29/2024.
//

#ifndef LIBRARYSYSTEM_BOOKHANDLER_H
#define LIBRARYSYSTEM_BOOKHANDLER_H

static struct Book * books;
static struct Loans * loans;

//Variables to store the wished book and author name for faster searching
static char wishedAuthor[50], wishedBook[50];
static int numberOfBooks = 0, numberOfLoans = 0, bookStock = 0;

void borrowBook();
void donateBook();
void bookSearch();
void returnBook();

void findReturnedBook();

void updateBookList();
void updateLoansStruct();

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

void updateLoansStruct()
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

void bookSearch()
{
    resetScreen();

    FILE * fBooks = openFile(fBooksName, "r");

    printf("Would you like to:\n1.Search for a specific book.\n2.See what books are available"
           " in the library.\n\n");

    int choice;
    scanf("%d", &choice);

    if(choice == 1)
    {
        fflush(stdin);

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
    }
    else
    {
        displayAllBooks(numberOfBooks, books);
    }

    pressAnyKey();
    //openBookManagementPage();
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
        return;
       // openBookManagementPage();
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
        return;
        //openBookManagementPage();
    }
}

int searchBookByName(char bookName[50], char * currentlyAuthStudentName)
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
                return 0;
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

                if(removeBookQuantity(books, numberOfBooks, wishedBook, wishedAuthor) == 0)
                {
                    //openBookManagementPage();
                    return 1;
                }
                else
                {
                    //borrowBook();
                    return 2;
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
}

int searchBookByNameAndAuthor(char bookName[50], char authorName[50], char * currentlyAuthStudentName)
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
                //There was an error or we don't want to borrow another book
                if(removeBookQuantity(books, numberOfBooks, wishedBook, wishedAuthor) == 0)
                {
                    //openBookManagementPage();
                    return 1;
                }
                else
                {
                    //borrowBook();
                    return 2;
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
}

void borrowBook(char * currentlyAuthStudentName)
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
        int result = searchBookByNameAndAuthor(bookName, authorName, currentlyAuthStudentName);
        if(result == 1)
        {
            return;
            //openBookManagementPage();
        }
        else if(result == 2)
        {
            borrowBook(currentlyAuthStudentName);
        }

    }
        //We provided a book name
    else if(strcmp(bookName, "\0") != 0)
    {
        int result = searchBookByName(bookName, currentlyAuthStudentName);
        if(result == 1)
        {
            return;
            //openBookManagementPage();
        }
        else if(result == 2)
        {
            borrowBook(currentlyAuthStudentName);
        }
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

    updateLoansStruct();

    pressAnyKey();
    return;
    //openBookManagementPage();
}

void returnBook(char * currentlyAuthStudentName)
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
        //openBookManagementPage();
        return;
    }
    //We suppose we didn't find any matches so we initialize them with 1 (strcmp returns 0 on match)
    if(searchForLoan(numberOfLoans, loans, currentlyAuthStudentName, wishedBook, wishedAuthor))
    {
        findReturnedBook();
        writeLoansToFile(numberOfLoans, loans);
        updateLoansStruct();

        printf("Would you like to return another book? [Y/N]\n");

        char ch;
        scanf("%c", &ch);
        if(ch == 'y' || ch == 'Y')
        {
            returnBook(currentlyAuthStudentName);
        }
        else
        {
            return;
            //return 1;
            //openBookManagementPage();
        }
    }
    else
    {
        printf("You didn't borrow a book named '%s', written by '%s'.\n", bookName, authorName);

        pressAnyKey();
        return;
        //return 1;
        //openBookManagementPage();
    }
}


#endif //LIBRARYSYSTEM_BOOKHANDLER_H
