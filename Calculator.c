#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
int main(){
    char inputStr[1000];
    char calStr[1000];
    int j=0;
    float num1;
    float num2;
    
    fgets(inputStr,sizeof(inputStr),stdin);  // INPUT //

                    // Formatting and verifying input string // 
    //----------------------------------------------------------------------//
    
    inputStr[strlen(inputStr)-1]='\0';
    for(int i=0;i<strlen(inputStr);i++){    
        if(inputStr[i]==' '){
            continue;
        }
        else if(!isdigit(inputStr[i]) && inputStr[i]!='+' && inputStr[i]!='-' && inputStr[i]!='/' && inputStr[i]!='*' && inputStr[i]!='.' ){
            printf("Invalid String");
            return 0;
        }
        else{
            calStr[j]=inputStr[i];
            j++;
        }
    }
    calStr[j]='\0';
    // calStr[j]='\0';
    // calStr[strlen(calStr)-2]='\0';
    //Basic calc operation logic//
    //----------------------------------------------------------------------//
    
    char strNum1[100];
    char op;
    
    int i=0;
    int idx_num1=0;
    
    //First num
    while(i<strlen(calStr) && isdigit(calStr[i])){
        strNum1[idx_num1]=calStr[i];
        i++;
        idx_num1++;
    }
    strNum1[idx_num1]='\0';
    
    num1=atof(strNum1);
    while(i<strlen(calStr)){
        op=calStr[i];
        i++;
        // Second num
        char strNum2[100];
        int idx_num2=0;
        while(i<strlen(calStr) && isdigit(calStr[i])){
            strNum2[idx_num2]=calStr[i];
            idx_num2++;
            i++;
        }
        strNum2[idx_num2]='\0';
        
        //Arithmetic operation
        num2=atof(strNum2);
        switch(op){
            case '+':
            num1+=num2;
            break;
            case '-':
            num1-=num2;
            break;
            case '*':
            num1*=num2;
            break;
            case '/':
            num1/=num2;
            break;
        }
    }
    printf("%lf",num1);

}