//
// Created by Erwin on 6/29/2024.
//

#ifndef LIBRARYSYSTEM_USERHANDLER_H
#define LIBRARYSYSTEM_USERHANDLER_H

static struct Student * students;

static int numberOfStudents = 0;

void copyCurrentAuthStudent(char *, char *, char *);

void registerStudent(char *);

void updateStudentList();



void copyCurrentAuthStudent(char *firstName, char *lastName, char * currentlyAuthStudentName)
{
    //We assign the last position + 1 of our first name the null terminator then change
    //the value of the last position to an empty space, allowing us to concatenate the
    //last name on top of the first name
    if(currentlyAuthStudentName != NULL)
    {
        free(currentlyAuthStudentName);
    }

    currentlyAuthStudentName = malloc(sizeof(firstName) + sizeof(lastName) + 1);
    strcpy(currentlyAuthStudentName, firstName);
    currentlyAuthStudentName[strlen(currentlyAuthStudentName) + 1] = '\0';
    currentlyAuthStudentName[strlen(currentlyAuthStudentName)] = ' ';
    strcat(currentlyAuthStudentName, lastName);
}

void registerStudent(char * currentlyAuthStudentName)
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
        registerStudent(currentlyAuthStudentName);
        return;
    }

    //Set the file back at the start of a line (we moved the position)
    writeStudentToFile(firstName, lastName);

    copyCurrentAuthStudent(firstName, lastName, currentlyAuthStudentName);

    printf("Successfully added in database!\nWelcome %s %s!\n", firstName, lastName);
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

#endif //LIBRARYSYSTEM_USERHANDLER_H
