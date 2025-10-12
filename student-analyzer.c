#include <stdio.h>
#include <string.h>

#define NAME_LENGTH 30
#define TOTAL_SUBJECTS 3

#define GRADE_A_LIMIT 85
#define GRADE_B_LIMIT 70
#define GRADE_C_LIMIT 50
#define GRADE_D_LIMIT 35

typedef struct
{
    int rollNumber;
    char name[NAME_LENGTH];
    int science;
    int math;
    int social;
    int totalMarks;
} Student;

typedef enum performance
{
    A = 5,
    B = 4,
    C = 3,
    D = 2,
} performance;

double calculateAverageMark(int totalMarks)
{
    return (double)(totalMarks) / (TOTAL_SUBJECTS * 1.0);
}

char getGrade(double average)
{
    if (average >= GRADE_A_LIMIT)
    {
        return 'A';
    }
    else if (average >= GRADE_B_LIMIT)
    {
        return 'B';
    }
    else if (average >= GRADE_C_LIMIT)
    {
        return 'C';
    }
    else if (average >= GRADE_D_LIMIT)
    {
        return 'D';
    }
    else
    {
        return 'F';
    }
}

void analyzePerformance(performance p)
{
    for (int i = 0; i < p; i++)
        printf("*");
}

int calculateTotalMarks(Student *student)
{
    return student->science + student->math + student->social;
}

void output(Student *s)
{
    printf("Roll: %d\n", s->rollNumber);
    printf("Name: %s\n", s->name);
    printf("Total: %d\n", s->totalMarks);
    double average = calculateAverageMark(s->totalMarks);
    printf("Average: %f\n", average);
    printf("Grade: %c\n", getGrade(average));
    if (getGrade(average) != 'F')
    {
        printf("Performance:");
        switch (getGrade(average))
        {
            case 'A':
                analyzePerformance(A);
                break;
            case 'B':
                analyzePerformance(B);
                break;
            case 'C':
                analyzePerformance(C);
                break;
            case 'D':
                analyzePerformance(D);
                break;
        }
        printf("\n");
    }
    printf("\n");
}

void printDetails(Student *students, int n)
{
    for (int i = 0; i < n; i++)
    {
        output(&students[i]);
    }
}

void printAllRollNumbers(Student *students, int n, int i)
{
    if (i >= n)
    {
        return;
    }
    printf("%d ", students[i++].rollNumber);
    printAllRollNumbers(students, n, i);
}

int checkMarkRange(int m)
{
    if (m < 0 || m > 100)
        return 1;
    return 0;
}

int handleInput(Student *students, Student *s, int count)
{
    for (int index = 0; index < count; index++)
    {
        if (students[index].rollNumber == s->rollNumber)
        {
            printf("You entered the same Roll Number again !");
            return 0;
        }
    }

    if (checkMarkRange(s->math) || checkMarkRange(s->science) || checkMarkRange(s->social))
    {
        printf("Enter valid mark range !");
        return 0;
    }
    return 1;
}

void sortByRollNumber(Student *students, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (students[j].rollNumber > students[j + 1].rollNumber)
            {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

void studentPerformanceAnalyzer()
{
    int numberOfStudents;
    int index = 0;

    printf("Enter number of students:");
    scanf("%d", &numberOfStudents);

    int n = numberOfStudents;
    Student students[n];

    while (index < n)
    {
        Student s;
        scanf("%d %s %d %d %d", &s.rollNumber, s.name, &s.science, &s.math, &s.social);
        if (!handleInput(students, &s, index))
        {
            return;
        }
        s.totalMarks = calculateTotalMarks(&s);
        students[index++] = s;
    }

    sortByRollNumber(students, n);

    printDetails(students, n);
    printf("List of Roll Numbers(via recursion):");
    printAllRollNumbers(students, n, 0);
    printf("\n");
}

int main()
{
    while (1)
    {
        studentPerformanceAnalyzer();
    }
    return 0;
}
