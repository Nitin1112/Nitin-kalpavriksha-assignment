#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50
#define HASH_SIZE 1009

typedef struct PCB
{
    char process_name[NAME_LEN];
    int process_id;
    int cpu_burst;
    int remaining_cpu;
    int io_start_time;
    int io_duration;
    int remaining_io;
    int io_started;
    int executed_cpu;
    int completion_time;
    int state;
    struct PCB *next;
} PCB;

typedef struct QueueNode
{
    PCB *process;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *head;
    QueueNode *tail;
    int size;
} Queue;

typedef struct KillEvent
{
    int target_pid;
    int kill_time;
    int used;
    struct KillEvent *next;
} KillEvent;

PCB *hash_table[HASH_SIZE];
Queue ready_queue, waiting_queue, terminated_queue;
KillEvent *kill_list = NULL;
PCB *current_running = NULL;
int total_processes = 0;

unsigned computeHash(int process_id)
{
    return process_id % HASH_SIZE;
}

void hashmapInsert(PCB *process)
{
    unsigned h = computeHash(process->process_id);
    process->next = hash_table[h];
    hash_table[h] = process;
}

PCB *hashmapGet(int process_id)
{
    unsigned h = computeHash(process_id);
    PCB *curr = hash_table[h];
    while (curr)
    {
        if (curr->process_id == process_id)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

void hashmapRemove(int process_id)
{
    unsigned h = computeHash(process_id);
    PCB *curr = hash_table[h], *prev = NULL;
    while (curr)
    {
        if (curr->process_id == process_id)
        {
            if (prev)
                prev->next = curr->next;
            else
                hash_table[h] = curr->next;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void queueInit(Queue *queue)
{
    queue->head = queue->tail = NULL;
    queue->size = 0;
}

void queuePush(Queue *queue, PCB *process)
{
    QueueNode *node = malloc(sizeof(QueueNode));
    node->process = process;
    node->next = NULL;
    if (!queue->tail)
        queue->head = queue->tail = node;
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
}

PCB *queuePop(Queue *queue)
{
    if (!queue->head)
        return NULL;
    QueueNode *node = queue->head;
    PCB *process = node->process;
    queue->head = node->next;
    if (!queue->head)
        queue->tail = NULL;
    free(node);
    queue->size--;
    return process;
}

int queueRemoveByPid(Queue *queue, int pid)
{
    QueueNode *curr = queue->head, *prev = NULL;
    while (curr)
    {
        if (curr->process->process_id == pid)
        {
            if (prev)
                prev->next = curr->next;
            else
                queue->head = curr->next;
            if (curr == queue->tail)
                queue->tail = prev;
            free(curr);
            queue->size--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

void addKillEvent(int pid, int time)
{
    KillEvent *k = malloc(sizeof(KillEvent));
    k->target_pid = pid;
    k->kill_time = time;
    k->used = 0;
    k->next = kill_list;
    kill_list = k;
}

int terminatedCount()
{
    return terminated_queue.size;
}

void readInput()
{
    char line[200];
    while (fgets(line, sizeof(line), stdin))
    {
        if (strncmp(line, "kill", 4) == 0)
        {
            int pid, time;
            sscanf(line + 4, "%d %d", &pid, &time);
            addKillEvent(pid, time);
        }
        else
        {
            char pname[NAME_LEN], str1[20], str2[20];
            int pid, cpu;
            int r = sscanf(line, "%s %d %d %s %s", pname, &pid, &cpu, str1, str2);
            if (r >= 3)
            {
                PCB *p = calloc(1, sizeof(PCB));
                strcpy(p->process_name, pname);
                p->process_id = pid;
                p->cpu_burst = cpu;
                p->remaining_cpu = cpu;
                if (r >= 5 && strcmp(str1, "-") != 0)
                {
                    p->io_start_time = atoi(str1);
                    p->io_duration = atoi(str2);
                }
                else
                {
                    p->io_start_time = -1;
                    p->io_duration = 0;
                }
                hashmapInsert(p);
                queuePush(&ready_queue, p);
                total_processes++;
            }
        }
    }
}

void applyKillEvents(int tick)
{
    KillEvent *k = kill_list;
    while (k)
    {
        if (!k->used && k->kill_time == tick)
        {
            PCB *p = hashmapGet(k->target_pid);
            if (p)
            {
                if (current_running && current_running->process_id == p->process_id)
                {
                    current_running->state = 2;
                    current_running->completion_time = tick;
                    hashmapRemove(p->process_id);
                    queuePush(&terminated_queue, current_running);
                    current_running = NULL;
                }
                else if (queueRemoveByPid(&ready_queue, p->process_id))
                {
                    p->state = 2;
                    p->completion_time = tick;
                    hashmapRemove(p->process_id);
                    queuePush(&terminated_queue, p);
                }
                else if (queueRemoveByPid(&waiting_queue, p->process_id))
                {
                    p->state = 2;
                    p->completion_time = tick;
                    hashmapRemove(p->process_id);
                    queuePush(&terminated_queue, p);
                }
                else
                {
                    p->state = 2;
                    p->completion_time = tick;
                    hashmapRemove(p->process_id);
                    queuePush(&terminated_queue, p);
                }
            }
            k->used = 1;
        }
        k = k->next;
    }
}

void simulate()
{
    int tick = 0;
    while (terminatedCount() < total_processes)
    {
        applyKillEvents(tick);

        Queue updated_waiting_queue;
        queueInit(&updated_waiting_queue);
        QueueNode *curr = waiting_queue.head;

        while (curr)
        {
            PCB *p = curr->process;
            QueueNode *next = curr->next;

            if (p->io_started == 0)
            {
                p->io_started = 1;
                queuePush(&updated_waiting_queue, p);
            }
            else
            {
                if (p->remaining_io > 0)
                    p->remaining_io--;
                if (p->remaining_io == 0)
                {
                    p->state = 0;
                    p->io_started = 0;
                    queuePush(&ready_queue, p);
                }
                else
                {
                    queuePush(&updated_waiting_queue, p);
                }
            }
            free(curr);
            curr = next;
        }

        waiting_queue = updated_waiting_queue;

        if (!current_running)
        {
            current_running = queuePop(&ready_queue);
            if (current_running)
                current_running->state = 1;
        }

        if (current_running)
        {
            current_running->executed_cpu++;
            current_running->remaining_cpu--;
        }

        if (current_running)
        {
            if (current_running->remaining_cpu == 0)
            {
                current_running->state = 3;
                current_running->completion_time = tick + 1;
                hashmapRemove(current_running->process_id);
                queuePush(&terminated_queue, current_running);
                current_running = NULL;
            }
            else if (current_running->io_start_time >= 0 &&
                     current_running->executed_cpu == current_running->io_start_time)
            {
                current_running->state = 4;
                current_running->remaining_io = current_running->io_duration;
                current_running->io_started = 0;
                queuePush(&waiting_queue, current_running);
                current_running = NULL;
            }
        }

        tick++;
    }
}

void printResults()
{
    printf("PID\tName\tCPU\tIO\tTurnaround\tWaiting\n");
    QueueNode *curr = terminated_queue.head;
    while (curr)
    {
        PCB *p = curr->process;
        if (p->state == 2)
        {
            printf("%d\t%s\t%d\t%d\t-\t\t-\n",
                   p->process_id, p->process_name, p->cpu_burst, p->io_duration);
        }
        else
        {
            int turnaround = p->completion_time;
            int waiting = turnaround - p->cpu_burst;
            if (waiting < 0)
                waiting = 0;

            printf("%d\t%s\t%d\t%d\t%d\t\t%d\n",
                   p->process_id, p->process_name, p->cpu_burst,
                   p->io_duration, turnaround, waiting);
        }
        curr = curr->next;
    }
}

int main()
{
    queueInit(&ready_queue);
    queueInit(&waiting_queue);
    queueInit(&terminated_queue);
    readInput();
    simulate();
    printResults();
    return 0;
}
