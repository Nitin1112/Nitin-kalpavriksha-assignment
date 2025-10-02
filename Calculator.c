#include<stdio.h>
#include<string.h>
int main(){
    char inputStr[1000];
    char calStr[1000];
    int j=0;
    fgets(inputStr,sizeof(inputStr),stdin);
    for(int i=0;i<strlen(inputStr);i++){
        if(inputStr[i]==' '){
            continue;
        }
        else{
            calStr[j]=inputStr[i];
            j++;
        }
    }
}