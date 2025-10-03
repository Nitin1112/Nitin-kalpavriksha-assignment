#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
    char name[30];
    char email[50];
    char designation[20];
    int salary;
} User;


void createUser(){
        FILE* f = fopen("user.txt", "a");
    User user;
    if (f == NULL) {
        printf("Couldn't open file\n");
        return;
    }

    printf("\nENTER USER DATA\n\n");

    printf("Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = 0;

    printf("Email: ");
    fgets(user.email, sizeof(user.email), stdin);
    user.email[strcspn(user.email, "\n")] = 0;

    printf("Designation: ");
    fgets(user.designation, sizeof(user.designation), stdin);
    user.designation[strcspn(user.designation, "\n")] = 0;

    printf("Salary: ");
    scanf("%d", &user.salary);
    getchar();

    fprintf(f, "{\n");
    fprintf(f, "\"name\": \"%s\",\n", user.name);
    fprintf(f, "\"email\": \"%s\",\n", user.email);
    fprintf(f, "\"designation\": \"%s\",\n", user.designation);
    fprintf(f, "\"salary\": %d\n", user.salary);
    fprintf(f, "}\n\n");

    fclose(f);
    printf("User created successfully.\n");
}

void readUser(){
    FILE* f = fopen("user.txt", "r");
    if (f == NULL) {
        printf("Couldn't open file\n");
        return;
    }

    char line[200];
    printf("\nUSER DATA:\n\n");
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }

    fclose(f);
}

void updateUser(){
    FILE* f = fopen("user.txt", "r");
    if (f == NULL) {
        printf("Couldn't open file\n");
        return;
    }

    FILE* temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fclose(f);
        return;
    }

    char searchName[30];
    printf("Enter name of user to update: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    char line[300];
    int found = 0;
    User user;

    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "\"name\":") != NULL) {
            char currentName[30];
            sscanf(line, "\"name\": \"%[^\"]", currentName);

            if (strcmp(currentName, searchName) == 0) {
                found = 1;
                for (int i = 0; i < 4; i++) {
                    fgets(line, sizeof(line), f);
                }

                printf("\nEnter new data for %s\n", searchName);

                printf("Name: ");
                fgets(user.name, sizeof(user.name), stdin);
                user.name[strcspn(user.name, "\n")] = 0;

                printf("Email: ");
                fgets(user.email, sizeof(user.email), stdin);
                user.email[strcspn(user.email, "\n")] = 0;

                printf("Designation: ");
                fgets(user.designation, sizeof(user.designation), stdin);
                user.designation[strcspn(user.designation, "\n")] = 0;

                printf("Salary: ");
                scanf("%d", &user.salary);
                getchar();

                fprintf(temp, "{\n");
                fprintf(temp, "\"name\": \"%s\",\n", user.name);
                fprintf(temp, "\"email\": \"%s\",\n", user.email);
                fprintf(temp, "\"designation\": \"%s\",\n", user.designation);
                fprintf(temp, "\"salary\": %d\n", user.salary);
                fprintf(temp, "}\n\n");
            } else {
                fputs(line, temp);
                for (int i = 0; i < 4; i++) {
                    fgets(line, sizeof(line), f);
                    fputs(line, temp);
                }
            }
        } else {
            fputs(line, temp);
        }
    }

    fclose(f);
    fclose(temp);

    remove("user.txt");
    rename("temp.txt", "user.txt");

    if (found) {
        printf("User updated successfully.\n");
    } else {
        printf("User not found.\n");
    }
}

void deleteUser(){
     FILE* f = fopen("user.txt", "r");
    if (f == NULL) {
        printf("Couldn't open file\n");
        return;
    }

    FILE* temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fclose(f);
        return;
    }

    char searchName[30];
    printf("Enter name of user to delete: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    char line[300];
    int found = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "\"name\":") != NULL) {
            char currentName[30];
            sscanf(line, "\"name\": \"%[^\"]", currentName);

            if (strcmp(currentName, searchName) == 0) {
                found = 1;
                for (int i = 0; i < 4; i++) {
                    fgets(line, sizeof(line), f);
                }
            } else {
                fputs(line, temp);
                for (int i = 0; i < 4; i++) {
                    fgets(line, sizeof(line), f);
                    fputs(line, temp);
                }
            }
        } else {
            fputs(line, temp);
        }
    }

    fclose(f);
    fclose(temp);

    remove("user.txt");
    rename("temp.txt", "user.txt");

    if (found) {
        printf("User deleted successfully.\n");
    } else {
        printf("User not found.\n");
    }
}

int main(){
    int option;
    while(1){
        printf("1. Create User\n");
        printf("2. Read All Users\n");
        printf("3. Update a User\n");
        printf("4. Delete a User\n");
        printf("5. Exit\n");
        printf("Enter an option: ");
        scanf("%d",&option);
        getchar();
        switch(option){
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
            return 0;
            break;
        }   
    }
    return 0;
}