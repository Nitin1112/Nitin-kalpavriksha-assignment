#include <stdio.h>
#define NAME_LENGTH 30

typedef struct
{
    int roll;
    char name[NAME_LENGTH];
    int mark1;
    int mark2;
    int mark3;
    int total;
    double average;
    char grade;
} Student;

void analyzePerformance(char grade);
double averageMark(int* total);
char getGrade(double average);
int menu();
void output(Student *s);
void printDetails(Student *students, int n);
void printStar(int n);
void recursiveRollList(Student *students, int n, int i);
void studentPerformanceAnalayzer();
int totalMarks(int m1, int m2, int m3);

int menu()
{
    int option;
    while (1)
    {
        int option;
        printf("\n====STUDENT PERFORMANCE ANALYZER====\n\n");
        printf("1. Enter Student-Performance-Analayzer\n");
        printf("2. Exit\n");
        printf("Enter an option: ");
        scanf("%d", &option);
        switch (option)
        {
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

int totalMarks(int m1, int m2, int m3)
{
    return m1 + m2 + m3;
}

double averageMark(int *total)
{
    return (double)(*total) / 3.0;
}

void printStar(int n)
{
    for (int i = 0; i < n; i++)
        printf("*");
}

void analyzePerformance(char grade)
{
    switch (grade)
    {
    case 'A':
        printStar(5);
        break;
    case 'B':
        printStar(4);
        break;
    case 'C':
        printStar(3);
        break;
    case 'D':
        printStar(2);
        break;
    }
}

void output(Student *s)
{
    printf("Roll-No: %d\n", s->roll);
    printf("Name: %s\n", s->name);
    printf("Total: %d\n", s->total);
    printf("Average: %f\n", s->average);
    printf("Grade: %c\n", s->grade);
    if (s->grade != 'F')
    {
        printf("Performance: ");
        analyzePerformance(s->grade);
    }
    printf("\n\n");
}

char getGrade(double average)
{
    if (average >= 85)
    {
        return 'A';
    }
    else if (average >= 70)
    {
        return 'B';
    }
    if (average >= 50)
    {
        return 'C';
    }
    if (average >= 35)
    {
        return 'D';
    }
    if (average < 35)
    {
        return 'F';
    }
}

void printDetails(Student *students, int n)
{
    for (int i = 0; i < n; i++)
    {
        output(&students[i]);
    }
}

void recursiveRollList(Student *students, int n, int i)
{
    if (i < n)
    {
        printf("%d ", students[i++].roll);
        recursiveRollList(students, n, i);
    }
}

void studentPerformanceAnalayzer()
{
    int noOfStudents;
    int i = 0;

    printf("Enter number of students:");
    scanf("%d", &noOfStudents);

    int n = noOfStudents;
    Student students[n];

    printf("Enter Student details\n");
    printf("(Roll-no Name Mark1 Mark2 Mark3):\n");

    while (i < n)
    {
        Student s;
        scanf("%d %s %d %d %d", &s.roll, s.name, &s.mark1, &s.mark2, &s.mark3);

        s.total = totalMarks(s.mark1, s.mark2, s.mark3);
        s.average = averageMark(&s.total);
        s.grade = getGrade(s.average);

        students[i++] = s;
    }

    printDetails(students, n);
    printf("List of Roll Numbers: ");
    recursiveRollList(students, n, 0);
}

int main()
{
    return menu();
}
