#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Players_data.h"

#define TEAM_NAME_LENGTH 12
#define NAME_LENGTH 20
#define ROLE_LENGTH 15

typedef struct PlayerNode{
    int id;
    char name[NAME_LENGTH];
    char team[TEAM_NAME_LENGTH];
    char role[ROLE_LENGTH];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;

    struct PlayerNode *next;
} PlayerNode;

typedef struct Team{
    char name[TEAM_NAME_LENGTH];
    int playerCount;
    float avg_batting_strike_rate;

    PlayerNode *players;          
    PlayerNode *batsmanHead;      
    PlayerNode *bowlerHead;       
    PlayerNode *allrounderHead;   
} Team;

typedef struct TempNode {
    PlayerNode *player;       
    struct TempNode *next;
} TempNode;


int findTeamId(int idx, Team *teams){
    for(int id = 1; id < 11; id++){
        if(strcmp(teams[id].name, players[idx].team) == 0){
            return id;
        }
    }
    return 0;
}

float calculatePerformanceIndex(PlayerNode *p){
    if(strcmp(p->role,"Batsman")==0){
        return (p->battingAverage * p->strikeRate) / 100.0;
    }
    else if(strcmp(p->role,"Bowler")==0){
        return (p->wickets * 2) + (100 - p->economyRate);
    }
    else{
        return ((p->battingAverage * p->strikeRate)/100.0) + (p->wickets * 2);
    }
}

int rolePriority(char *role){
    if(strcmp(role,"Batsman")==0) return 1;
    if(strcmp(role,"Bowler")==0) return 2;
    return 3; 
}

void updateRoleHeads(Team *t){
    t->batsmanHead = NULL;
    t->bowlerHead = NULL;
    t->allrounderHead = NULL;

    PlayerNode *cur = t->players;

    while(cur != NULL){
        if(strcmp(cur->role,"Batsman")==0 && t->batsmanHead == NULL)
            t->batsmanHead = cur;
        else if(strcmp(cur->role,"Bowler")==0 && t->bowlerHead == NULL)
            t->bowlerHead = cur;
        else if(strcmp(cur->role,"All-rounder")==0 && t->allrounderHead == NULL)
            t->allrounderHead = cur;

        cur = cur->next;
    }
}

void insertSorted(Team *t, PlayerNode *newNode){

    PlayerNode **head = &t->players;

    if (*head == NULL ||
        rolePriority(newNode->role) < rolePriority((*head)->role) ||
        (rolePriority(newNode->role) == rolePriority((*head)->role) &&
         newNode->performanceIndex > (*head)->performanceIndex)) {

        newNode->next = *head;
        *head = newNode;
        updateRoleHeads(t);
        return;
    }

    PlayerNode *curr = *head;

    while(curr->next != NULL){
        int cr = rolePriority(curr->next->role);
        int nr = rolePriority(newNode->role);

        if(nr < cr) break;

        if(nr == cr &&
           newNode->performanceIndex > curr->next->performanceIndex)
            break;

        curr = curr->next;
    }

    newNode->next = curr->next;
    curr->next = newNode;

    updateRoleHeads(t);
}

void initializeTeamName(Team* teams){
    strcpy(teams[1].name,"India");
    strcpy(teams[2].name,"Australia");
    strcpy(teams[3].name,"England");
    strcpy(teams[4].name,"Pakistan");
    strcpy(teams[5].name,"New Zealand");
    strcpy(teams[6].name,"South Africa");
    strcpy(teams[7].name,"Bangladesh");
    strcpy(teams[8].name,"Sri Lanka");
    strcpy(teams[9].name,"Afghanistan");
    strcpy(teams[10].name,"West Indies");
}

void initializePlayers(Team *t, Player p){
    PlayerNode *newNode = malloc(sizeof(PlayerNode));

    newNode->id = p.id;
    strcpy(newNode->name, p.name);
    strcpy(newNode->team, p.team);
    strcpy(newNode->role, p.role);
    newNode->totalRuns = p.totalRuns;
    newNode->battingAverage = p.battingAverage;
    newNode->strikeRate = p.strikeRate;
    newNode->wickets = p.wickets;
    newNode->economyRate = p.economyRate;

    newNode->performanceIndex = calculatePerformanceIndex(newNode);
    newNode->next = NULL;

    insertSorted(t, newNode);
}

void loadInitialData(Team* teams){
    for(int i=1;i<11;i++){
        teams[i].players = NULL;
        teams[i].batsmanHead = NULL;
        teams[i].bowlerHead  = NULL;
        teams[i].allrounderHead = NULL;
        teams[i].playerCount = 0;
        teams[i].avg_batting_strike_rate = 0;
    }

    initializeTeamName(teams);

    for(int i=0;i<playerCount;i++){
        int tid = findTeamId(i, teams);
        initializePlayers(&teams[tid], players[i]);
        teams[tid].playerCount++;
    }
        for(int tid = 1; tid <= 10; tid++){
        float sumSR = 0;
        int count = 0;
        PlayerNode *p = teams[tid].players;

        while(p != NULL){
            sumSR += p->strikeRate;
            count++;
            p = p->next;
        }

        if(count > 0)
            teams[tid].avg_batting_strike_rate = sumSR / count;
    }
}

void addPlayers(Team *teams){
    int tid;
    printf("Enter Team ID: ");
    scanf("%d",&tid);
    getchar();

    PlayerNode *n = malloc(sizeof(PlayerNode));

    printf("Player ID: ");
    scanf("%d",&n->id); getchar();

    printf("Name: ");
    fgets(n->name, NAME_LENGTH, stdin);
    n->name[strcspn(n->name,"\n")] = 0;

    strcpy(n->team, teams[tid].name);

    int ro;
    printf("Role (1-Batsman,2-Bowler,3-All-rounder): ");
    scanf("%d",&ro);

    if(ro==1) strcpy(n->role,"Batsman");
    else if(ro==2) strcpy(n->role,"Bowler");
    else strcpy(n->role,"All-rounder");

    printf("Total Runs: ");
    scanf("%d",&n->totalRuns);
    printf("Batting Average: ");
    scanf("%f",&n->battingAverage);
    printf("Strike Rate: ");
    scanf("%f",&n->strikeRate);
    printf("Wickets: ");
    scanf("%d",&n->wickets);
    printf("Economy Rate: ");
    scanf("%f",&n->economyRate);

    n->performanceIndex = calculatePerformanceIndex(n);
    n->next = NULL;

    insertSorted(&teams[tid], n);
    teams[tid].playerCount++;

    float sumSR = 0;
    int count = 0;
    PlayerNode *ptr = teams[tid].players;

    while(ptr){
        sumSR += ptr->strikeRate;
        count++;
        ptr = ptr->next;
    }

    teams[tid].avg_batting_strike_rate = sumSR / count;

    printf("Player Added Successfully!\n");
}

void displayPlayersBySpecificTeam(Team* teams){
    int id;
    printf("Enter Team Id: ");
    scanf("%d",&id);

    PlayerNode *p = teams[id].players;

    printf("Players of Team %s\n",teams[id].name);
    printf("====================================================================================\n");
    printf("ID  Name                 Role           Runs  Avg   SR    Wkts  ER   Perf.Index\n");
    printf("====================================================================================\n");

    float totSR=0;
    int count=0;

    while(p){
        printf("%-3d %-20s %-15s %-5d %-5.1f %-5.1f %-4d %-4.1f %-6.2f\n",
            p->id, p->name, p->role, p->totalRuns,
            p->battingAverage, p->strikeRate, p->wickets,
            p->economyRate, p->performanceIndex);
        p = p->next;
    }

    printf("Average Batting Strike Rate: %.2f\n",
        teams[id].avg_batting_strike_rate);

    printf("Total Players: %d\n",teams[id].playerCount);
}

void displayTeamsByAvgBattingStrikeRate(Team *teams){
    bool used[11]={0};

    printf("\nTeams Sorted by Avg Batting SR\n");
    printf("===============================================\n");

    for(int k=1;k<11;k++){
        float mx=-1;
        int tid=0;

        for(int i=1;i<11;i++){
            if(!used[i] && teams[i].avg_batting_strike_rate >= mx){
                mx = teams[i].avg_batting_strike_rate;
                tid = i;
            }
        }

        used[tid]=1;
        printf("%-2d %-12s %.2f\n",
            tid, teams[tid].name,
            teams[tid].avg_batting_strike_rate);
    }
}

void topkPlayersbyRole(Team *teams){
    int tid, ro, k;
    printf("Enter Team ID: ");
    scanf("%d",&tid);
    printf("Enter Role (1-Batsman 2-Bowler 3-All-rounder): ");
    scanf("%d",&ro);
    printf("Enter K: ");
    scanf("%d",&k);

    PlayerNode *start = NULL;

    if(ro==1) start = teams[tid].batsmanHead;
    else if(ro==2) start = teams[tid].bowlerHead;
    else start = teams[tid].allrounderHead;

    printf("\nTop %d players of %s\n",k,teams[tid].name);
    printf("====================================================================================\n");
    printf("ID  Name                 Role           Runs  Avg   SR    Wkts  ER   Perf.Index\n");
    printf("====================================================================================\n");

    int count=0;
    PlayerNode *p = start;

    while(p && count<k){
        printf("%-3d %-20s %-15s %-5d %-5.1f %-5.1f %-4d %-4.1f %-6.2f\n",
            p->id, p->name, p->role, p->totalRuns,
            p->battingAverage, p->strikeRate, p->wickets,
            p->economyRate, p->performanceIndex);

        count++;
        p = p->next;
    }
}

void insertSortedTemp(TempNode **head, PlayerNode *p)
{
    TempNode *newNode = malloc(sizeof(TempNode));
    newNode->player = p;
    newNode->next = NULL;

    if (*head == NULL || 
        p->performanceIndex > (*head)->player->performanceIndex)
    {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    TempNode *cur = *head;
    while(cur->next != NULL &&
          cur->next->player->performanceIndex >= p->performanceIndex)
    {
        cur = cur->next;
    }

    newNode->next = cur->next;
    cur->next = newNode;
}

void displayAllPlayersBySpecificRole(Team teams[])
{
    int roleOption;
    printf("Enter Role to Display (1-Batsman 2-Bowler 3-All-rounder): ");
    scanf("%d", &roleOption);

    char selectedRole[20];
    if(roleOption == 1) strcpy(selectedRole, "Batsman");
    else if(roleOption == 2) strcpy(selectedRole, "Bowler");
    else strcpy(selectedRole, "All-rounder");

    TempNode *head = NULL;  

    for(int tid = 1; tid <= 10; tid++)
    {
        PlayerNode *p = teams[tid].players;

        while(p != NULL)
        {
            if(strcmp(p->role, selectedRole) == 0)
            {
                insertSortedTemp(&head, p);  
            }
            p = p->next;
        }
    }

    if(head == NULL)
    {
        printf("No players found for role: %s\n", selectedRole);
        return;
    }

    printf("\nPlayers Across ALL Teams (Role = %s)\n", selectedRole);
    printf("==============================================================================================\n");
    printf("ID   Name                 Team          Role           Runs  Avg   SR    Wkts  ER   Perf.Index\n");
    printf("==============================================================================================\n");

    TempNode *cur = head;
    while(cur != NULL)
    {
        PlayerNode *p = cur->player;

        printf("%-4d %-20s %-12s %-15s %-5d %-5.1f %-5.1f %-4d %-4.1f %-6.2f\n",
            p->id, p->name, p->team, p->role,
            p->totalRuns, p->battingAverage, p->strikeRate,
            p->wickets, p->economyRate, p->performanceIndex);

        cur = cur->next;
    }
    while(head != NULL)
    {
        TempNode *temp = head;
        head = head->next;
        free(temp);
    }
}

void performance_analyzer(){
    Team teams[11];
    loadInitialData(teams);

    while(1){
        printf("\n================ ICC ODI Player Performance Analyzer ================\n");
        printf("1. Add Player\n");
        printf("2. Display Team Players\n");
        printf("3. Display Teams by AVG Bat SR\n");
        printf("4. Top K Players by Role\n");
        printf("5. Display All Players of Role Across Teams\n");
        printf("6. Exit\n");
        printf("Enter choice: ");

        int c;
        scanf("%d",&c);

        switch(c){
            case 1: addPlayers(teams); break;
            case 2: displayPlayersBySpecificTeam(teams); break;
            case 3: displayTeamsByAvgBattingStrikeRate(teams); break;
            case 4: topkPlayersbyRole(teams); break;
            case 5: displayAllPlayersBySpecificRole(teams); break;
            case 6: return;
            default: printf("Invalid Choice\n");
        }
    }
}

int main(){
    performance_analyzer();
    return 0;
}
