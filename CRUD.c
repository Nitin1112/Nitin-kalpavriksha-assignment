#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 300

typedef struct
{
    int id;
    char name[30];
    int age;
} User;

int getNextId()
{
    FILE *f = fopen("user.txt", "r");
    if (!f)
        return 1;
    char line[LINE_LENGTH];
    int lastId = 0, id;
    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, "\"id\":") != NULL)
        {
            sscanf(line, " \"id\": %d", &id);
            if (id > lastId)
                lastId = id;
        }
    }
    fclose(f);
    return lastId + 1;
}

void getData(User *user)
{
    printf("Name: ");
    fgets(user->name, sizeof(user->name), stdin);
    user->name[strcspn(user->name, "\n")] = 0;
    printf("Age: ");
    scanf("%d", &user->age);
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
    user.id = getNextId();
    getData(&user);

    fprintf(f, "{\n");
    fprintf(f, "\"id\": %d,\n", user.id);
    fprintf(f, "\"name\": \"%s\",\n", user.name);
    fprintf(f, "\"age\": %d\n", user.age);
    fprintf(f, "}\n\n");

    fclose(f);
    printf("User created successfully. (ID: %d)\n", user.id);
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

void searchAndUpdate(int searchId, FILE *temp, FILE *f, int *found)
{
    char line[LINE_LENGTH];
    User user;

    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, "\"id\":") != NULL)
        {
            int currentId;
            sscanf(line, " \"id\": %d", &currentId);

            if (currentId == searchId)
            {
                *found = 1;
                while (fgets(line, sizeof(line), f))
                {
                    if (strchr(line, '}'))
                        break;
                }

                printf("\nEnter new data for ID %d\n", searchId);
                getData(&user);
                user.id = searchId;

                fprintf(temp, "{\n");
                fprintf(temp, "\"id\": %d,\n", user.id);
                fprintf(temp, "\"name\": \"%s\",\n", user.name);
                fprintf(temp, "\"age\": %d\n", user.age);
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

    int searchId;
    printf("Enter ID of user to update: ");
    scanf("%d", &searchId);
    getchar();

    int found = 0;
    searchAndUpdate(searchId, temp, f, &found);

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

    int searchId;
    printf("Enter ID of user to delete: ");
    scanf("%d", &searchId);
    getchar();

    char line[LINE_LENGTH];
    int found = 0;
    int skipBlock = 0;

    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, "\"id\":") != NULL)
        {
            int currentId;
            sscanf(line, " \"id\": %d", &currentId);

            if (currentId == searchId)
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
