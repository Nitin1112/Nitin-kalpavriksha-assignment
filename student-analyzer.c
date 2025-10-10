#include<stdio.h>
#define NAME_LENGTH 30

void studentPerformanceAnalayzer();
int menu();

int menu(){
    int option;
    while(1){
        int option;
        printf("\n====STUDENT PERFORMANCE ANALYZER====\n\n");
        printf("1. Enter Student-Performance-Analayzer\n");
        printf("2. Exit\n");
        printf("Enter an option: ");
        scanf("%d",&option);
        switch(option){
            case 1:
            studentPerformanceAnalayzer();
            break;
            case 2:
            printf("Exiting...");
            return 1;
            break;
            default:
            printf("Invalid option");
        }
    }
}

void studentPerformanceAnalayzer(){
    int roll,mark1,mark2,mark3;
    char name[NAME_LENGTH];
    printf("Enter Student details\n");
    printf("Roll-no/Name/Mark1/Mark2/Mark3 : ");
    scanf("%d/%[^/]s/%d/%d/%d",&roll,name,&mark1,&mark2,&mark3);
    printf("Roll-No: %d",roll);
    printf("Name: %s",name);
    printf("Mark1: %d",mark1);
    printf("Mark2: %d",mark2);
    printf("Mark3: %d",mark3);
}

int main(){
    return menu();
}