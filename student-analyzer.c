#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NAME_LENGTH 30
#define SUBJECT_LENGTH 3
#define TOTAL_SUBJECTS 3

#define GRADE_A_LIMIT 85
#define GRADE_B_LIMIT 70
#define GRADE_C_LIMIT 50
#define GRADE_D_LIMIT 35

typedef struct
{
    int rollNumber;
    char name[NAME_LENGTH];
    int marks[SUBJECT_LENGTH];
} Student;

typedef enum Grade
{
    A = 'A',
    B = 'B',
    C = 'C',
    D = 'D',
    F = 'F',
} Grade;

typedef enum GradeStars
{
    STARS_A = 5,
    STARS_B = 4,
    STARS_C = 3,
    STARS_D = 2,
    STARS_E = 1,
    STARS_F = 0
} GradeStars;

double calculateAverageMark(int totalMarks)
{
    return (double)(totalMarks) / (TOTAL_SUBJECTS * 1.0);
}

Grade getGrade(double average)
{
    if (average >= GRADE_A_LIMIT)
    {
        return A;
    }
    else if (average >= GRADE_B_LIMIT)
    {
        return B;
    }
    else if (average >= GRADE_C_LIMIT)
    {
        return C;
    }
    else if (average >= GRADE_D_LIMIT)
    {
        return D;
    }
    else
    {
        return F;
    }
}

void analyzePerformance(Grade g)
{
    int stars = 0;
    switch (g)
    {
        case A:
            stars = STARS_A;
            break;
        case B:
            stars = STARS_B;
            break;
        case C:
            stars = STARS_C;
            break;
        case D:
            stars = STARS_D;
            break;
        case F:
            stars = STARS_F;
            break;
    }
    for (int i = 0; i < stars; i++)
        printf("*");
}

int calculateTotalMarks(Student *student)
{
    int totalMarks = 0;
    for (int index = 0; index < SUBJECT_LENGTH; index++)
    {
        totalMarks += student->marks[index];
    }
    return totalMarks;
}

void output(Student *s)
{
    printf("Roll: %d\n", s->rollNumber);
    printf("Name: %s\n", s->name);
    int totalMarks = calculateTotalMarks(s);
    printf("Total: %d\n", totalMarks);
    double averageMark = calculateAverageMark(totalMarks);
    Grade grade = getGrade(averageMark);
    printf("Average: %f\n", averageMark);
    printf("Grade: %c\n", grade);
    if (grade != F)
    {
        printf("Performance:");
        analyzePerformance(grade);
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

bool areMarksInRange(int m)
{
    if (m < 0 || m > 100)
        return false;
    return true;
}

bool handleInput(Student *students, Student *s, int count)
{
    for (int index = 0; index < count; index++)
    {
        if (students[index].rollNumber == s->rollNumber)
        {
            printf("You entered the same Roll Number again !\n");
            return false;
        }
    }

    for (int index = 0; index < SUBJECT_LENGTH; index++)
    {
        if (!areMarksInRange(s->marks[index]))
        {
            printf("Enter valid mark range\n");
            return false;
        }
    }
    return true;
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
        scanf("%d %s %d %d %d", &s.rollNumber, s.name, &s.marks[0], &s.marks[1], &s.marks[2]);
        if (!handleInput(students, &s, index))
        {
            printf("Re-enter !!\n");
            continue;
        }
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
