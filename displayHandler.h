#ifndef LIBRARYSYSTEM_DISPLAYHANDLER_H
#define LIBRARYSYSTEM_DISPLAYHANDLER_H

void welcomeUser();

void displayMainPage();

void noUserFoundMessage();

void pressAnyKey();

void displayLoggedIn();

void errorMessage(char * message);

void displayUserManagementPage();

void showLoanedBooks();

void displayBookManagementPage();

void endScreen();

void resetScreen();

void welcomeUser()
{
    printf("---------------------\n");
    printf("- Enter Credentials -\n");
    printf("---------------------\n");
    printf("Enter your surname and firstname under the following form:\n"
           "\n'login [FIRSTNAME] [SURNAME]'\nHit ENTER to confirm.\n\n");
}

void resetScreen()
{
    system("cls");
    fflush(stdin);
}

void displayMainPage()
{
    printf("--------------------------\n");
    printf("--      Welcome to      --\n");
    printf("--  The Library System! --\n");
    printf("--------------------------\n\n");
    printf("Type the index of the system that you want to access, then press ENTER!\n");
    printf("> 1. User Management Page\n");
    printf("> 2. Library Management Page\n");
    printf("> 3. Quit App\n");
}

void noUserFoundMessage()
{
    printf("There is no user with that name found in the database, try again.");
}

void pressAnyKey()
{
    fflush(stdin);

    printf("\nPress any key to continue.\n");
    char ch;
    scanf("%c", &ch);
}

void displayLoggedIn()
{
    printf("\nSuccessfully logged in!\n");
}

void errorMessage(char * message)
{
    printf("%s\n", message);
    Sleep(2000);
}

void displayUserManagementPage()
{
    printf("-------------------\n");
    printf("- User Management -\n");
    printf("-------------------\n\n");
    printf("> 1. Add User\n");
    printf("> 2. Return to main menu\n");
}

void displayBookManagementPage()
{
    printf("-------------------\n");
    printf("- Book Management -\n");
    printf("-------------------\n\n");
    printf("> 1. Borrow a Book\n");
    printf("> 2. Donate a Book\n");
    printf("> 3. Return a Book\n");
    printf("> 4. Show Borrowed Books\n");
    printf("> 5. Search For a Book\n");
    printf("> 6. Return to the Main Menu\n");
}

void endScreen()
{
    printf("Have a nice day!");
    Sleep(2000);
    exit(0);
}

void showLoanedBooks(char * currentlyAuthStudentName)
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

}

#endif //LIBRARYSYSTEM_DISPLAYHANDLER_H
