#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define STRING_LENGTH 100
#define ACTION_LENGTH 20
#define VALUE_LENGTH 20

typedef struct QueueNode{
    int key;
    char value[VALUE_LENGTH];
    struct QueueNode *prev;
    struct QueueNode *next;
}QueueNode;

typedef struct Queue{
    QueueNode *front;
    QueueNode *rear;
}Queue;

Queue *queue = NULL;
QueueNode **hashMap = NULL;
int capacity = 0;
int size = 0;

void findAndStoreAction(char *input,char *action){
    int i=0;
    char temp[STRING_LENGTH];
    strcpy(temp,input);
    while(temp[i]!=' ' && temp[i]!='\0'){
        i++;
    }
    temp[i]='\0';
    strcpy(action,temp);
}

char* getArgs(char* input){
    int i=0;
    char *c=input;
    while(input[i]!='\0'){
        if(input[i]==' ')
            break;
        c++;
        i++;
    }
    return c+1;
}

int stringToInt(char* token){
    int num=0;
    int idx=0;
    while(token[idx]!='\0'){
        num = num*10 + (token[idx]-'0');
        idx++;
    }
    return num;
}

bool checkArgs(char* args,int *key,char* value){
    char* t=strtok(args," ");
    int i=0;
    while(t!=NULL){
        if(i==0){
            *key=stringToInt(t);
        }
        else if(i==1){
            strcpy(value,t);
        }
        else{
            printf("Invalid input\n");
            return false;
        }
        i++;
        t=strtok(NULL," ");
    }
    if(i < 2){
        printf("Invalid input\n");
        return false;
    }
    return true;
}

int hash(int key){
    return key % capacity;
}

void moveToRear(QueueNode *node){
    if(queue->rear == node) return;

    if(node->prev != NULL)
        node->prev->next = node->next;
    else
        queue->front = node->next;

    if(node->next != NULL)
        node->next->prev = node->prev;

    node->next = NULL;
    node->prev = queue->rear;
    queue->rear->next = node;
    queue->rear = node;
}

QueueNode* addCache(int key,char *value){
    QueueNode *newNode = malloc(sizeof(QueueNode));
    if(!newNode) return NULL;

    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->prev = NULL;
    newNode->next = NULL;

    if(queue->front == NULL){
        queue->front = newNode;
        queue->rear = newNode;
        return newNode;
    }

    queue->rear->next = newNode;
    newNode->prev = queue->rear;
    queue->rear = newNode;

    return newNode;
}

void removeFromHashMap(QueueNode *node){
    int index = hash(node->key);

    for(int i=0;i<capacity;i++){
        int idx = (index + i) % capacity;
        if(hashMap[idx] == node){
            hashMap[idx] = NULL;

            int j = (idx + 1) % capacity;
            while(hashMap[j] != NULL){
                QueueNode *temp = hashMap[j];
                hashMap[j] = NULL;

                int newIndex = hash(temp->key);
                for(int k=0;k<capacity;k++){
                    int ni = (newIndex + k) % capacity;
                    if(hashMap[ni] == NULL){
                        hashMap[ni] = temp;
                        break;
                    }
                }
                j = (j + 1) % capacity;
            }
            return;
        }
    }
}

void removeLRU(){
    QueueNode *remove = queue->front;

    if(remove == NULL) return;

    if(queue->front == queue->rear){
        queue->front = NULL;
        queue->rear = NULL;
    } else {
        queue->front = remove->next;
        queue->front->prev = NULL;
    }

    removeFromHashMap(remove);

    free(remove);
    size--;
}

char* get(char* args){
    int key = stringToInt(args);
    int index = hash(key);

    for(int i=0; i<capacity; i++){
        int idx = (index + i) % capacity;
        if(hashMap[idx] != NULL && hashMap[idx]->key == key){
            moveToRear(hashMap[idx]);
            return hashMap[idx]->value;
        }
    }
    return NULL;
}

void put(int key,char* value){
    for(int i=0;i<capacity;i++){
        if(hashMap[i] != NULL && hashMap[i]->key == key){
            strcpy(hashMap[i]->value, value);
            moveToRear(hashMap[i]);
            return;
        }
    }

    if(size == capacity){
        removeLRU();
    }

    QueueNode *node = addCache(key,value);
    size++;

    int index = hash(key);
    for(int i=0;i<capacity;i++){
        int idx = (index + i) % capacity;
        if(hashMap[idx] == NULL){
            hashMap[idx] = node;
            return;
        }
    }
}

void createCache(char* args){
    capacity = stringToInt(args);
    queue = malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    hashMap = calloc(capacity, sizeof(QueueNode*));
}

void LRU(){
    char input[STRING_LENGTH];
    char action[ACTION_LENGTH]="";
    char *args;

    while(1){
        fgets(input,STRING_LENGTH,stdin);
        input[strcspn(input,"\n")]='\0';

        findAndStoreAction(input,action);
        if(strlen(action)==0){
            printf("Invalid input\n");
            continue;
        }

        if(strcmp(action,"get")==0){
            args=getArgs(input);
            if(*args=='\0'){
                printf("Invalid input\n");
                continue;
            }
            char *result = get(args);
            if(result) printf("%s\n", result);
            else printf("NULL\n");
        }
        else if(strcmp(action,"put")==0){
            args=getArgs(input);
            if(*args=='\0'){
                printf("Invalid input\n");
                continue;
            }
            int key=0;
            char value[VALUE_LENGTH];
            if(!checkArgs(args,&key,value)) continue;
            put(key,value);
        }
        else if(strcmp(action,"createCache")==0){
            args=getArgs(input);
            if(*args=='\0'){
                printf("Invalid input\n");
                continue;
            }
            createCache(args);
        }
        else if(strcmp(action,"exit")==0){
            return;
        }
        else{
            printf("Invalid command!\n");
        }
    }
}

int main(){
    LRU();
    return 0;
}
