#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define COMMANDLINE_LENGTH 200
#define COMMAND_LENGTH 20
#define MAX_BLOCK_SIZE 100
#define SIZE 50
#define BLOCK_SIZE 512
#define NUMBER_OF_BLOCK 64

typedef struct FileNode FileNode;
typedef struct FreeBlock FreeBlock;

struct FileNode
{
    char name[SIZE];
    int blockcount;
    int size;
    int blocks[MAX_BLOCK_SIZE];
    bool isDirectory;
    FileNode *next;
    FileNode *prev;
    FileNode *child;
    FileNode *parent;
};

struct FreeBlock
{
    int index;
    FreeBlock *next;
    FreeBlock *prev;
};

char virtualDisk[NUMBER_OF_BLOCK][BLOCK_SIZE];
int usedBlocks = 0;

void getCommand(char *command, char *command_line)
{
    int i = 0;
    while (command_line[i] != ' ' && command_line[i] != '\0')
    {
        command[i] = command_line[i];
        i++;
    }
    command[i] = '\0';
}

void freeBlockMemory(FreeBlock **head)
{
    FreeBlock *temp;
    while (*head != NULL)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

void freeFileNodes(FileNode **root)
{
    FileNode *temp;
    while (*root != NULL)
    {
        temp = *root;
        *root = (*root)->next;
        free(temp);
    }
}

FileNode *isDirectoryPresent(FileNode *cwd, char *name)
{
    if (cwd->child == NULL)
        return NULL;
    FileNode *temp = cwd->child;
    do
    {
        if (strcmp(temp->name, name) == 0 && temp->isDirectory)
            return temp;
        temp = temp->next;
    } while (temp != cwd->child);
    return NULL;
}

bool initializeBlocks(FreeBlock **head)
{
    *head = (FreeBlock *)malloc(sizeof(FreeBlock));
    if (*head == NULL)
        return false;
    (*head)->index = 0;
    (*head)->prev = NULL;
    FreeBlock *temp = *head;
    for (int i = 1; i < NUMBER_OF_BLOCK; i++)
    {
        FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
        newBlock->index = i;
        newBlock->prev = temp;
        temp->next = newBlock;
        temp = newBlock;
    }
    temp->next = NULL;
    return true;
}

void createDirectory(FileNode *cwd, char *command_line)
{
    FileNode *newNode = (FileNode *)malloc(sizeof(FileNode));
    newNode->child = NULL;
    newNode->isDirectory = true;
    strcpy(newNode->name, command_line);
    newNode->parent = cwd;
    if (cwd->child == NULL)
    {
        cwd->child = newNode;
        newNode->prev = newNode;
        newNode->next = newNode;
        printf("Directory '%s' created successfully.\n", command_line);
        return;
    }
    FileNode *temp = cwd->child;
    while (temp->next != cwd->child)
        temp = temp->next;
    temp->next = newNode;
    newNode->prev = temp;
    newNode->next = cwd->child;
    cwd->child->prev = newNode;
    printf("Directory '%s' created successfully.\n", command_line);
}

void removeDirectory(FileNode **cwd, char *command_line)
{
    FileNode *toRemoveNode = isDirectoryPresent(*cwd, command_line);
    if (!toRemoveNode)
    {
        printf("Directory not found.\n");
        return;
    }
    if (toRemoveNode->child != NULL)
    {
        printf("Directory not empty. Remove files first.\n");
        return;
    }
    if (toRemoveNode->next == toRemoveNode)
        (*cwd)->child = NULL;
    else
    {
        toRemoveNode->prev->next = toRemoveNode->next;
        toRemoveNode->next->prev = toRemoveNode->prev;
        if ((*cwd)->child == toRemoveNode)
            (*cwd)->child = toRemoveNode->next;
    }
    free(toRemoveNode);
    printf("Directory removed successfully.\n");
}

void createFile(FileNode *cwd, char *name)
{
    FileNode *newNode = (FileNode *)malloc(sizeof(FileNode));
    newNode->isDirectory = false;
    strcpy(newNode->name, name);
    newNode->size = 0;
    newNode->blockcount = 0;
    newNode->parent = cwd;
    if (cwd->child == NULL)
    {
        cwd->child = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    }
    else
    {
        FileNode *temp = cwd->child;
        while (temp->next != cwd->child)
            temp = temp->next;
        temp->next = newNode;
        newNode->prev = temp;
        newNode->next = cwd->child;
        cwd->child->prev = newNode;
    }
    printf("File '%s' created successfully.\n", name);
}

void writeFile(FileNode *cwd, char *name, char *content, FreeBlock **head)
{
    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("File not found.\n");
        return;
    }
    do
    {
        if (!temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            int len = strlen(content);
            int blocksNeeded = (len + BLOCK_SIZE - 1) / BLOCK_SIZE;
            if (blocksNeeded > NUMBER_OF_BLOCK - usedBlocks)
            {
                printf("Not enough space on disk.\n");
                return;
            }
            temp->size = len;
            temp->blockcount = blocksNeeded;
            for (int i = 0; i < blocksNeeded; i++)
            {
                if (*head == NULL)
                {
                    printf("No free blocks available.\n");
                    return;
                }
                temp->blocks[i] = (*head)->index;
                FreeBlock *t = *head;
                *head = (*head)->next;
                if (*head)
                    (*head)->prev = NULL;
                free(t);
                usedBlocks++;
            }
            for (int i = 0; i < len; i++)
            {
                int blockIndex = temp->blocks[i / BLOCK_SIZE];
                virtualDisk[blockIndex][i % BLOCK_SIZE] = content[i];
            }
            printf("Data written successfully (size=%d bytes).\n", len);
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);
    printf("File not found.\n");
}

void readFile(FileNode *cwd, char *name)
{
    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("File not found.\n");
        return;
    }
    do
    {
        if (!temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            if (temp->size == 0)
            {
                printf("(empty)\n");
                return;
            }
            for (int i = 0; i < temp->size; i++)
            {
                int blockIndex = temp->blocks[i / BLOCK_SIZE];
                printf("%c",virtualDisk[blockIndex][i % BLOCK_SIZE]);
            }
            printf("\n");
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);
    printf("File not found.\n");
}

void deleteFile(FileNode *cwd, char *name, FreeBlock **head)
{
    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("File not found.\n");
        return;
    }
    do
    {
        if (!temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            for (int i = 0; i < temp->blockcount; i++)
            {
                FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
                newBlock->index = temp->blocks[i];
                newBlock->next = *head;
                newBlock->prev = NULL;
                if (*head)
                    (*head)->prev = newBlock;
                *head = newBlock;
                usedBlocks--;
            }
            if (temp->next == temp)
                cwd->child = NULL;
            else
            {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                if (cwd->child == temp)
                    cwd->child = temp->next;
            }
            free(temp);
            printf("File deleted successfully.\n");
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);
    printf("File not found.\n");
}

void listFilesAndFolders(FileNode *cwd)
{
    if (cwd->child == NULL)
    {
        printf("(empty)\n");
        return;
    }
    FileNode *temp = cwd->child;
    do
    {
        printf("%s%s\n", temp->name, temp->isDirectory ? "/" : "");
        temp = temp->next;
    } while (temp != cwd->child);
}

void changeDirectory(FileNode **cwd, char *absolute_path, char *args)
{
    if (strcmp(args, "..") == 0)
    {
        if ((*cwd)->parent != NULL)
        {
            *cwd = (*cwd)->parent;
            char *p = strrchr(absolute_path, '/');
            if (p && p != absolute_path)
                *p = '\0';
        }
        printf("Moved to %s\n", absolute_path);
        return;
    }
    FileNode *next = isDirectoryPresent(*cwd, args);
    if (!next)
    {
        printf("Directory not found.\n");
        return;
    }
    *cwd = next;
    strcat(absolute_path, "/");
    strcat(absolute_path, args);
    printf("Moved to %s\n", absolute_path);
}

void displayDiskUsage()
{
    int freeBlocks = NUMBER_OF_BLOCK - usedBlocks;
    float percent = ((float)usedBlocks / NUMBER_OF_BLOCK) * 100;
    printf("Total Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\nDisk Usage: %.2f%%\n",
           NUMBER_OF_BLOCK, usedBlocks, freeBlocks, percent);
}

void vfs()
{
    FileNode *root = (FileNode *)malloc(sizeof(FileNode));
    FreeBlock *headFreeBlock;
    if (!initializeBlocks(&headFreeBlock) || root == NULL)
        return;
    strcpy(root->name, "/");
    root->parent = NULL;
    root->child = NULL;
    root->next = root;
    root->prev = root;
    FileNode *currentDirectory = root;
    char absolute_path[COMMANDLINE_LENGTH] = "";
    strcat(absolute_path, root->name);

    while (1)
    {
        printf("\n%s >", absolute_path);
        char command_line[COMMANDLINE_LENGTH];
        fgets(command_line, COMMANDLINE_LENGTH, stdin);
        command_line[strlen(command_line) - 1] = 0;
        char command[COMMAND_LENGTH];
        getCommand(command, command_line);
        char *args = NULL;
        if (strchr(command_line, ' ') != NULL)
            args = strchr(command_line, ' ') + 1;

        if (strcmp(command, "mkdir") == 0)
        {
            if (args == NULL)
            {
                printf("Incomplete command\n");
                continue;
            }
            createDirectory(currentDirectory, args);
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            if (args == NULL)
            {
                printf("Incomplete command\n");
                continue;
            }
            removeDirectory(&currentDirectory, args);
        }
        else if (strcmp(command, "create") == 0)
        {
            if (args == NULL)
            {
                printf("Incomplete command\n");
                continue;
            }
            createFile(currentDirectory, args);
        }
        else if (strcmp(command, "write") == 0)
        {
            if(args == NULL )
            {
                printf("Incomplete command\n");
                continue;
            }
            char filename[SIZE], content[BLOCK_SIZE * 10];
            char *firstQuote = strchr(args, '"');
            char *lastQuote = strrchr(args, '"');
            int nameLen = firstQuote - args - 1;
            
            if (nameLen==0|| !firstQuote || !lastQuote || lastQuote == firstQuote)
            {
                printf("Incomplete command\n");
                continue;
            }
            strncpy(filename, args, nameLen);
            filename[nameLen] = '\0';
            int contentLen = lastQuote - firstQuote - 1;
            strncpy(content, firstQuote + 1, contentLen);
            content[contentLen] = '\0';

            writeFile(currentDirectory, filename, content, &headFreeBlock);
        }
        else if (strcmp(command, "read") == 0)
        {
            if (args == NULL)
            {
                printf("Incomplete command\n");
                continue;
            }
            readFile(currentDirectory, args);
        }
        else if (strcmp(command, "delete") == 0)
        {
            if (args == NULL)
            {
                printf("Incomplete command\n");
                continue;
            }
            deleteFile(currentDirectory, args, &headFreeBlock);
        }
        else if (strcmp(command, "ls") == 0)
            listFilesAndFolders(currentDirectory);
        else if (strcmp(command, "cd") == 0)
        {
            if (args == NULL)
            {
                printf("Incomplete command\n");
                continue;
            }
            changeDirectory(&currentDirectory, absolute_path, args);
        }
        else if (strcmp(command, "pwd") == 0)
            printf("%s\n", absolute_path);
        else if (strcmp(command, "df") == 0)
            displayDiskUsage();
        else if (strcmp(command, "exit") == 0)
        {
            freeFileNodes(&root);
            freeBlockMemory(&headFreeBlock);
            printf("Memory released. Exiting program...\n");
            break;
        }
        else
            printf("Enter valid command !!\n");
    }
}

int main()
{
    printf("$ ./vfs\nCompact VFS - ready. Type 'exit' to quit.\n");
    vfs();
    return 0;
}
