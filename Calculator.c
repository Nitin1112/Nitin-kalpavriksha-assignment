#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int main(){
    char inputStr[1000];
    char calStr[1000];
    int j = 0;

    fgets(inputStr, sizeof(inputStr), stdin);  // INPUT //

                    // Formatting and verifying input string 
    //----------------------------------------------------------------------
    inputStr[strlen(inputStr) - 1] = '\0';
    for (int i = 0; i < strlen(inputStr); i++) {
        if (inputStr[i] == ' ') {
            continue;
        }
        else if (!isdigit(inputStr[i]) && inputStr[i] != '+' && inputStr[i] != '-' && inputStr[i] != '/' && inputStr[i] != '*') {
            printf("Invalid String");
            return 0;
        }
        else {
            calStr[j] = inputStr[i];
            j++;
        }
    }
    calStr[j] = '\0';

                    // Handling * and / operators
    //----------------------------------------------------------------------
    int i = 0;
    char tempStr[1000] = "";
    while (i < strlen(calStr)) {
        char num1Str[100] = "";
        char num2Str[100] = "";
        int idx1 = 0, idx2 = 0;
        int num1, num2;
        char op;

        // Extract first number
        while (i < strlen(calStr) && isdigit(calStr[i])) {
            num1Str[idx1++] = calStr[i++];
        }
        num1Str[idx1] = '\0';
        num1 = atoi(num1Str);

        if (i >= strlen(calStr)) {
            char temp[50];
            sprintf(temp, "%d", num1);
            strcat(tempStr, temp);
            break;
        }

        op = calStr[i++];

        // process all upcoming * and /
        while (op == '*' || op == '/') {
            // Extract second number
            idx2 = 0;
            while (i < strlen(calStr) && isdigit(calStr[i])) {
                num2Str[idx2++] = calStr[i++];
            }
            num2Str[idx2] = '\0';
            num2 = atoi(num2Str);

            if (op == '*') {
                num1 *= num2;
            }
            else if (op == '/') {
                if (num2 == 0) {
                    printf("Division by Zero occured");
                    return 0;
                }
                num1 /= num2;
            }

            // Checking if there is another * or /
            if (i < strlen(calStr)) {
                op = calStr[i++];
            } else {
                break;
            }
        }

        // Storing the result
        char temp[50];
        sprintf(temp, "%d", num1);
        strcat(tempStr, temp);

        if (op == '+' || op == '-') {
            int len = strlen(tempStr);
            tempStr[len] = op;
            tempStr[len + 1] = '\0';
        }
    }

                        // Handling + and - operators
    //----------------------------------------------------------------------
    int finalResult = 0;
    i = 0;
    int sign = 1;
    while (i < strlen(tempStr)) {
        char numStr[100] = "";
        int idx = 0;

        if (tempStr[i] == '+') {
            sign = 1;
            i++;
        }
        else if (tempStr[i] == '-') {
            sign = -1;
            i++;
        }

        while (i < strlen(tempStr) && isdigit(tempStr[i])) {
            numStr[idx++] = tempStr[i++];
        }
        numStr[idx] = '\0';

        finalResult += sign * atoi(numStr);
    }

    printf("%d", finalResult);
    return 0;
}
