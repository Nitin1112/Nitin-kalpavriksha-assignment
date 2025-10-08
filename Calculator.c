#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1000

void performCalculation();
void handleInput(const char *s, char *calStrPointer);
char *handleAddAndSub(const char *string);
double handleDivisionAndMultiplication(const char *tempStr);
int checkCharacters(char c);
int menu();

double extractNumber(const char *str, int *i);
double processMultiplicationAndDivision(const char *string, int *i, double firstNum);
void appendNumberWithOperator(char *dest, double number, char op);
void appendNumber(char *dest, double number);
double extractSignedNumber(const char *tempStr, int *i, int *sign);

int menu(){
    while(1)
    {
        printf("\n=== Simple Calculator ===\n");
        printf("1. Perform calculation\n");
        printf("2. Exit\n");
        printf("Enter an option: ");

        int option;
        scanf("%d", &option);
        getchar();

        switch (option)
        {
        case 1:
            performCalculation();
            break;
        case 2:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid option.\n");
            break;
        }
    }
}

int main()
{
    
    return menu();
}

void performCalculation()
{
    char inputStr[MAX_LENGTH];
    char calStr[MAX_LENGTH];

    printf("\nEnter an expression: ");
    fgets(inputStr, sizeof(inputStr), stdin);

    int len = strlen(inputStr);
    if (len > 0 && inputStr[len - 1] == '\n')
    {
        inputStr[len - 1] = '\0';
    }

    handleInput(inputStr, calStr);

    char *Result1 = handleAddAndSub(calStr);
    double value = handleDivisionAndMultiplication(Result1);

    printf("Result: %.10g\n", value);
}

int checkCharacters(char c)
{
    if (!isdigit((unsigned char)c) && c != '+' && c != '-' && c != '/' && c != '*')
    {
        return 1;
    }
    else
        return 0;
}

void handleInput(const char *s, char *calStrPointer)
{
    int j = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == ' ')
            continue;

        if (checkCharacters(s[i]))
        {
            printf("Invalid character detected: %c\n", s[i]);
            calStrPointer[0] = '\0';
            return;
        }

        calStrPointer[j++] = s[i];
    }
    calStrPointer[j] = '\0';
}

double extractNumber(const char *str, int *i)
{
    char numStr[100] = "";
    int idx = 0;
    while (isdigit((unsigned char)str[*i]) || str[*i] == '.')
    {
        numStr[idx++] = str[(*i)++];
    }
    numStr[idx] = '\0';
    return strtod(numStr, NULL);
}

double processMultiplicationAndDivision(const char *string, int *i, double firstNum)
{
    while (*i < strlen(string) && (string[*i] == '*' || string[*i] == '/'))
    {
        char op = string[(*i)++];
        int sign2 = 1;
        if (string[*i] == '-') { sign2 = -1; (*i)++; }
        else if (string[*i] == '+') { (*i)++; }

        double num2 = extractNumber(string, i) * sign2;

        if (op == '*')
            firstNum *= num2;
        else if (op == '/')
        {
            if (num2 == 0)
            {
                printf("Division by zero!\n");
                return 0;
            }
            firstNum /= num2;
        }
    }
    return firstNum;
}

void appendNumber(char *dest, double number)
{
    char temp[50];
    sprintf(temp, "%.10g", number);
    strcat(dest, temp);
}

void appendNumberWithOperator(char *dest, double number, char op)
{
    appendNumber(dest, number);
    int len = strlen(dest);
    dest[len] = op;
    dest[len + 1] = '\0';
}

char *handleAddAndSub(const char *string)
{
    static char tempStr[MAX_LENGTH];
    tempStr[0] = '\0';
    int i = 0;

    while (i < strlen(string))
    {
        int sign1 = 1;
        if (string[i] == '-') { sign1 = -1; i++; }
        else if (string[i] == '+') { i++; }

        double num1 = extractNumber(string, &i) * sign1;
        num1 = processMultiplicationAndDivision(string, &i, num1);

        if (i < strlen(string) && (string[i] == '+' || string[i] == '-'))
        {
            appendNumberWithOperator(tempStr, num1, string[i]);
            i++;
        }
        else
        {
            appendNumber(tempStr, num1);
        }
    }

    return tempStr;
}

double extractSignedNumber(const char *tempStr, int *i, int *sign)
{
    *sign = 1;
    if (tempStr[*i] == '+') { *sign = 1; (*i)++; }
    else if (tempStr[*i] == '-') { *sign = -1; (*i)++; }

    char numStr[100] = "";
    int idx = 0;
    while (*i < strlen(tempStr) && (isdigit((unsigned char)tempStr[*i]) || tempStr[*i] == '.'))
    {
        numStr[idx++] = tempStr[(*i)++];
    }
    numStr[idx] = '\0';

    return strtod(numStr, NULL);
}

double handleDivisionAndMultiplication(const char *tempStr)
{
    double finalResult = 0;
    int i = 0;

    while (i < strlen(tempStr))
    {
        int sign;
        double number = extractSignedNumber(tempStr, &i, &sign);
        finalResult += sign * number;
    }

    return finalResult;
}
