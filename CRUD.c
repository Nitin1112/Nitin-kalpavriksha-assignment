#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 300

typedef struct
{
    char name[30];
    char email[50];
    char designation[20];
    int salary;
} User;

void getData(User *user)
{
    printf("Name: ");
    fgets(user->name, sizeof(user->name), stdin);
    user->name[strcspn(user->name, "\n")] = 0;

    printf("Email: ");
    fgets(user->email, sizeof(user->email), stdin);
    user->email[strcspn(user->email, "\n")] = 0;

    printf("Designation: ");
    fgets(user->designation, sizeof(user->designation), stdin);
    user->designation[strcspn(user->designation, "\n")] = 0;

    printf("Salary: ");
    scanf("%d", &user->salary);
    getchar(); 
}

void createUser()
{
    FILE *f = fopen("user.txt", "a");
    if (!f)
    {
        printf("Couldn't open file\n");
        return;
    }

    User user;
    printf("\nENTER USER DATA\n\n");
    getData(&user);

    fprintf(f, "{\n");
    fprintf(f, "\"name\": \"%s\",\n", user.name);
    fprintf(f, "\"email\": \"%s\",\n", user.email);
    fprintf(f, "\"designation\": \"%s\",\n", user.designation);
    fprintf(f, "\"salary\": %d\n", user.salary);
    fprintf(f, "}\n\n");

    fclose(f);
    printf("User created successfully.\n");
}

void readUser()
{
    FILE *f = fopen("user.txt", "r");
    if (!f)
    {
        printf("Couldn't open file\n");
        return;
    }

    char line[LINE_LENGTH];
    printf("\nUSER DATA:\n\n");
    while (fgets(line, sizeof(line), f))
    {
        printf("%s", line);
    }

    fclose(f);
}

void searchAndUpdate(char *searchName, FILE *temp, FILE *f, int *found)
{
    char line[LINE_LENGTH];
    User user;
    char buffer[5000] = "";

    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, "\"name\":") != NULL)
        {
            char currentName[30];
            sscanf(line, " \"name\": \"%[^\"]", currentName);

            if (strcmp(currentName, searchName) == 0)
            {
                *found = 1;

                while (fgets(line, sizeof(line), f))
                {
                    if (strchr(line, '}'))
                        break;
                }

                printf("\nEnter new data for %s\n", searchName);
                getData(&user);

                fprintf(temp, "{\n");
                fprintf(temp, "\"name\": \"%s\",\n", user.name);
                fprintf(temp, "\"email\": \"%s\",\n", user.email);
                fprintf(temp, "\"designation\": \"%s\",\n", user.designation);
                fprintf(temp, "\"salary\": %d\n", user.salary);
                fprintf(temp, "}\n\n");
            }
            else
            {
                fputs(line, temp);
            }
        }
        else
        {
            fputs(line, temp);
        }
    }
}

void updateUser()
{
    FILE *f = fopen("user.txt", "r");
    if (!f)
    {
        printf("Couldn't open file\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp)
    {
        fclose(f);
        return;
    }

    char searchName[30];
    printf("Enter name of user to update: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    int found = 0;
    searchAndUpdate(searchName, temp, f, &found);

    fclose(f);
    fclose(temp);

    remove("user.txt");
    rename("temp.txt", "user.txt");

    if (found)
        printf("User updated successfully.\n");
    else
        printf("User not found.\n");
}

void deleteUser()
{
    FILE *f = fopen("user.txt", "r");
    if (!f)
    {
        printf("Couldn't open file\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp)
    {
        fclose(f);
        return;
    }

    char searchName[30];
    printf("Enter name of user to delete: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    char line[LINE_LENGTH];
    int found = 0;
    int skipBlock = 0;

    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, "\"name\":") != NULL)
        {
            char currentName[30];
            sscanf(line, " \"name\": \"%[^\"]", currentName);

            if (strcmp(currentName, searchName) == 0)
            {
                found = 1;
                skipBlock = 1;
                continue;
            }
        }

        if (skipBlock && strchr(line, '}'))
        {
            skipBlock = 0;
            continue;
        }

        if (!skipBlock)
            fputs(line, temp);
    }

    fclose(f);
    fclose(temp);
    remove("user.txt");
    rename("temp.txt", "user.txt");

    if (found)
        printf("User deleted successfully.\n");
    else
        printf("User not found.\n");
}

int main()
{
    int option;
    while (1)
    {
        printf("\n=== USER MANAGEMENT ===\n");
        printf("1. Create User\n");
        printf("2. Read All Users\n");
        printf("3. Update a User\n");
        printf("4. Delete a User\n");
        printf("5. Exit\n");
        printf("Enter an option: ");
        scanf("%d", &option);
        getchar();

        switch (option)
        {
        case 1:
            createUser();
            break;
        case 2:
            readUser();
            break;
        case 3:
            updateUser();
            break;
        case 4:
            deleteUser();
            break;
        case 5:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid Option\n");
        }
    }
    return 0;
}
