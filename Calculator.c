#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1000

void performCalculation();
void handleInput(const char *s, char *calStrPointer);
char *firstPass(const char *string);
double secondPass(const char *tempStr);

int main()
{
    while (1)
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

    char *firstPassResult = firstPass(calStr);
    double value = secondPass(firstPassResult);

    printf("Result: %.10g\n", value);
}

void handleInput(const char *s, char *calStrPointer)
{
    int j = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == ' ')
            continue;

        if (!isdigit((unsigned char)s[i]) && s[i] != '+' &&
            s[i] != '-' && s[i] != '/' && s[i] != '*')
        {
            printf("Invalid character detected: %c\n", s[i]);
            calStrPointer[0] = '\0';
            return;
        }

        calStrPointer[j++] = s[i];
    }
    calStrPointer[j] = '\0';
}

char* firstPass(const char* string) {
    static char tempStr[MAX_LENGTH];
    tempStr[0] = '\0';
    int i = 0;

    while (i < strlen(string)) {
        char numStr[100] = "";
        int idx = 0;
        double num1 = 0, num2 = 0;
        char op = 0;

        int sign1 = 1;
        if (string[i] == '-') { sign1 = -1; i++; }
        else if (string[i] == '+') { i++; }

        while (i < strlen(string) && (isdigit((unsigned char)string[i]) || string[i] == '.')) {
            numStr[idx++] = string[i++];
        }
        numStr[idx] = '\0';
        num1 = strtod(numStr, NULL) * sign1;

        while (i < strlen(string) && (string[i] == '*' || string[i] == '/')) {
            op = string[i++];

            int sign2 = 1;
            if (string[i] == '-') { sign2 = -1; i++; }
            else if (string[i] == '+') { i++; }

            idx = 0;
            while (i < strlen(string) && (isdigit((unsigned char)string[i]) || string[i] == '.')) {
                numStr[idx++] = string[i++];
            }
            numStr[idx] = '\0';
            num2 = strtod(numStr, NULL) * sign2;

            if (op == '*') num1 *= num2;
            else if (op == '/') {
                if (num2 == 0) { printf("Division by zero!\n"); return ""; }
                num1 /= num2;
            }
        }

        char temp[50];
        sprintf(temp, "%.10g", num1);
        strcat(tempStr, temp);

        if (i < strlen(string) && (string[i] == '+' || string[i] == '-')) {
            int len = strlen(tempStr);
            tempStr[len] = string[i++];
            tempStr[len + 1] = '\0';
        }
    }

    return tempStr;
}


double secondPass(const char *tempStr)
{
    double finalResult = 0;
    int i = 0;
    int sign = 1;

    while (i < strlen(tempStr))
    {
        char numStr[100] = "";
        int idx = 0;

        if (tempStr[i] == '+')
        {
            sign = 1;
            i++;
        }
        else if (tempStr[i] == '-')
        {
            sign = -1;
            i++;
        }

        while (i < strlen(tempStr) && (isdigit((unsigned char)tempStr[i]) || tempStr[i] == '.'))
        {
            numStr[idx++] = tempStr[i++];
        }
        numStr[idx] = '\0';

        finalResult += sign * strtod(numStr, NULL);
    }

    return finalResult;
}
