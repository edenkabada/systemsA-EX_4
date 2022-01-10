#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX 99999
#define SIZE 255
#define NO_PATH -1
#define MAX_SIZE 99999999

typedef struct Edge {
    int mDest;
    int mWeight;
    struct Edge* mNext;
}Edge;

typedef struct Node {
    int mId;
    int mPriorityWeight;
    int mIsVisited;
    int mNumberOfEdges;
    Edge* mHeadEdges;
    struct Node* mNextNode;
}Node ;

typedef struct PQ {
    int mId;
    int mWeight;
    struct PQ* mNext;
}PQ;



typedef struct DirectedGraph {
    int mNumberOfNodes;
    Node* mHeadNodes;
} DirectedGraph;

enum Action {
    A_INIT_GRAPH,
    B_ADD_NODE,
    D_DELETE_NODE,
    S_SHORT_PATH,
    T_TSP,
    ERROR
};

enum Action getAction(char ch)
{
    if (ch == 'A')
    {
        return A_INIT_GRAPH;
    }
    if (ch == 'B')
    {
        return B_ADD_NODE;
    }
    if (ch == 'D')
    {
        return D_DELETE_NODE;
    }
    if (ch == 'S')
    {
        return S_SHORT_PATH;
    }
    if (ch == 'T')
    {
        return T_TSP;
    }
    return ERROR;
}

int convertCharNumberToInt(char ch)
{
    return ch - '0';
}

void freeEdge(Edge* head)
{
    Edge* tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->mNext;
        free(tmp);
    }
}

void freeGraph(DirectedGraph* directGraph) {
    Node* head = directGraph->mHeadNodes;
    Node* tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->mNextNode;
        freeEdge(tmp->mHeadEdges);
        free(tmp);
    }
}

DirectedGraph* initGraph(int nodesNumber) {
    DirectedGraph* directGraph = (DirectedGraph*)calloc(1,sizeof(DirectedGraph));
    return directGraph;
}

int shouldStop(char ch)
{
    if (ch != 'D' && ch != 'A' && ch != 'B' && ch != 'S' && ch != 'T' && ch != '\0')
    {
        return 0;
    }
    return 1;
}

Node* getNode(DirectedGraph* graph, int src) {

    Node* head = graph->mHeadNodes;
    while (head != NULL) {
        if (head->mId == src) {
            return head;
        }
        head = head->mNextNode;
    }
    return NULL;
}

Node* addNodeToGraph(DirectedGraph* graph, int src)
{
    Node* node = getNode(graph, src);
    if (node == NULL)
    {
        Node* add = (Node*)calloc(1, sizeof(Node));
        add->mId = src;
        add->mNextNode = graph->mHeadNodes;
        graph->mHeadNodes = add;
        graph->mNumberOfNodes++;
        return add;
    }
    else {
        freeEdge(node->mHeadEdges);
        node->mHeadEdges = NULL;
        return node;
    }
}

void addEdgeToNode(Node* node, Edge* edge)
{
    edge->mNext = node->mHeadEdges;
    node->mHeadEdges = edge;
}

void addAllNodesToGraph(DirectedGraph* graph, char* text, int* index)
{
    while (shouldStop(text[*index]) == 0)
    {
       if (text[*index] == 'n')
       {
           *index = *index + 2;         
            Node* addedNode = addNodeToGraph(graph, convertCharNumberToInt(text[*index]));
            *index = *index + 2;
            while (text[*index] != 'n' && shouldStop(text[*index]) == 0)
            {
                Edge* edge = (Edge*)calloc(1, sizeof(Edge));
                edge->mDest = convertCharNumberToInt(text[*index]);
                *index = *index + 2;
                edge->mWeight = convertCharNumberToInt(text[*index]);
                *index = *index + 2;
                addEdgeToNode(addedNode, edge);
            }
        }
    }
}

void printGraph(DirectedGraph* graph)
{

    Node* head = graph->mHeadNodes;
    while (head != NULL)
    {
        printf("Node %d :", head->mId);
        Edge* headEdge = head->mHeadEdges;
        while (headEdge != NULL)
        {
            printf("[ %d , w : %d]", headEdge->mDest, headEdge->mWeight);
            headEdge = headEdge->mNext;
        }
        printf("\n");
        head = head->mNextNode;
    }
}

void deleteNode(DirectedGraph* graph, int src)
{
    Node* temp = graph->mHeadNodes;
    if (temp != NULL) {
        if (temp->mId == src) {
            Node* nodeToDelete = graph->mHeadNodes;
            graph->mHeadNodes = graph->mHeadNodes->mNextNode;
            graph->mNumberOfNodes--;
            freeEdge(nodeToDelete->mHeadEdges);
            free(nodeToDelete);
        }
        else {
            while (temp->mNextNode != NULL) {
                if (temp->mNextNode->mId == src) {
                    Node* nodeToDelete = temp->mNextNode;
                    temp->mNextNode = temp->mNextNode->mNextNode;
                    graph->mNumberOfNodes--;
                    freeEdge(nodeToDelete->mHeadEdges);
                    free(nodeToDelete);
                    break;
                }
                temp = temp->mNextNode;
            }
        }
    }

}

int* createCities(char* text,int* index,int *sizeCityInput) {
    *sizeCityInput = convertCharNumberToInt(text[*index]);
    int* result = (int*)calloc(6, sizeof(int));
    *index = *index + 2;
    for (int i = 0; i < *sizeCityInput; i++)
    {
        result[i] = convertCharNumberToInt(text[*index]);
        *index = *index + 2;
    }
    return result;
}

void swap(int* arr, int a, int b)
{
    int tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
}



void printArray(int* a, int len,int* index, int** permutation2d)
{
   
    size_t i = 0;
    for (i = 0; i < len; i++) {
        permutation2d[*index][i] = a[i];
    }
    *index = *index + 1;
}


void permute(int* arr, int start, int end,int *index,int** permutation2d)
{
    int i;
    if (start == end) 
    {
        printArray(arr, end,index, permutation2d);
        return;
    }
    permute(arr, start + 1, end, index, permutation2d);
   
    for (i = start + 1; i < end; i++)
    {
        if (arr[start] == arr[i]) continue;
        swap(arr, start, i);
        permute(arr, start + 1, end, index, permutation2d);
        swap(arr, start, i);

    }
}


int** getPermutationEmptyArray(int size,int* newSize)
{
    *newSize = 1;
    for (int i = 2; i <= size; i++)
    {
        *newSize *= i;
    }
    int** result = (int**)calloc(*newSize, sizeof(int*));
    for (int i = 0; i < *newSize; i++)
    {
        result[i] = (int*)calloc(size, sizeof(int));
    }
    return result;
}


int getWeigthOfEdge(DirectedGraph* graph, int src, int dest)
{
    Node* node = getNode(graph, src);
    if (node != NULL) {
        Edge* edge = node->mHeadEdges;
        while (edge != NULL)
        {
            if (edge->mDest == dest)
            {
                return edge->mWeight;
            }
            edge = edge->mNext;
        }
    }
    return NO_PATH;

}


int calculatePath(DirectedGraph* graph, int* cities, int size)
{
    int totalWeight = 0;
    for (int i = 1; i < size; i++)
    {
        int weight = getWeigthOfEdge(graph, cities[i - 1], cities[i]);
        if (weight == NO_PATH)
        {
            totalWeight = NO_PATH;
            break;

        }
        else {
            totalWeight += weight;
        }
    }
    return totalWeight;

}


void addCity(int* cities, DirectedGraph* graph, int index)
{
    Node* head = graph->mHeadNodes;
    while (head != NULL)
    {
        int found = 0;
        int key = head->mId;
        for (int i = 0; i < index; i++)
        {
            if (cities[i] == key) {
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            cities[index] = key;
            break;
        }
        head = head->mNextNode;
    }

}
//A 4 n 0 2 5 3 3 n 2 0 4 1 1 n 1 3 7 0 2 n 3 T 3 2 1 3 B 5 0 4 2 1 B 2 1 3 5 1 T 3 2 1 3 D 2 T 2 3 0

int sizeOfAllNodesInGraph(DirectedGraph* graph) {
    int res = 0;
    Node* temp = graph->mHeadNodes;
    while (temp != NULL)
    {
        res++;
        temp = temp->mNextNode;
    }

    return res;

}


int* getAllGraphNodes(DirectedGraph* graph,int size)
{
    int* result = (int*)calloc(size, sizeof(int));
    int index = 0;
    Node* temp = graph->mHeadNodes;
    while (temp != NULL)
    {
        result[index++] = temp->mId;
        temp = temp->mNextNode;
    }

    return result;
}

int isArrContainsCities(int* citiesInput, int sizeCityInput,int* checkArr, int  untilIndex) {
   
    for (int i = 0; i < sizeCityInput; i++)
    {
        int isContain = 0;
        for (int j = 0; j < untilIndex; j++)
        {
            if (citiesInput[i] == checkArr[j]) {

                isContain = 1;
                break;
            }

        }
        if (isContain == 0) {
            return 0;
        }

    }

    return 1;
}



int performTsp(DirectedGraph* graph, int* citiesInput, int sizeCityInput) {
   // printGraph(graph);
    int minPath = MAX_SIZE;
    int sizeOfNodes = sizeOfAllNodesInGraph(graph);
    int* graphArr = getAllGraphNodes(graph, sizeOfNodes);
    int sizePermutaion2D = 0;
    int** permutation2D = getPermutationEmptyArray(sizeOfNodes, &sizePermutaion2D);
    int index = 0;
    permute(citiesInput, 0, sizeOfNodes, &index, permutation2D);
    for (int i = sizeCityInput; i <= sizeOfNodes; i++) {
        for (int j = 0; j < sizePermutaion2D; j++)
        {

            int* checkArr = permutation2D[j];
            if (isArrContainsCities(citiesInput, sizeCityInput, checkArr, i) == 1) {
                int result = calculatePath(graph, checkArr, i);
                if (result != NO_PATH && result < minPath) {
                    minPath = result;
                }        
            }
        }  
    }
    for (int i = 0; i < sizePermutaion2D; i++)
    {
        free(permutation2D[i]);
    }
    free(permutation2D);
    free(graphArr);
    return minPath == MAX_SIZE ? NO_PATH : minPath;

  
}


void setPriority(Node* node, int p)
{
    node->mPriorityWeight = p;
}

void setVisited(Node* node)
{
    node->mIsVisited = 1;
}


int isEmpty(PQ** head) {
    return (*head) == NULL;
}


PQ* newNode(int src, int p) {
    PQ* temp = (PQ*)malloc(sizeof(PQ));
    temp->mId = src;
    temp->mWeight = p;
    temp->mNext = NULL;
    return temp;
}

int pop(PQ** head) {
    PQ* temp = *head;
    (*head) = (*head)->mNext;
    int node = temp->mId;
    free(temp);
    return node;
}
void push(PQ** head, int d, int p) {
    if (isEmpty(head))
    {
        *head = newNode(d,p);
        return;
    }


    PQ* start = (*head);
    PQ* temp = newNode(d, p);
    if ((*head)->mWeight > p) {
        temp->mNext = *head;
        (*head) = temp;
    }
    else {
        while (start->mNext != NULL && start->mNext->mWeight < p) {
            start = start->mNext;
        }
        temp->mNext = start->mNext;
        start->mNext = temp;
    }
}



void dijkstra(DirectedGraph* graph, int srcNode, int destNode) {
    Node* ptr = getNode(graph,srcNode);
    setPriority(ptr, 0);
    setVisited(ptr);
    PQ* pq = newNode(ptr->mId, 0);
    while (!isEmpty(&pq)) {
        ptr = getNode(graph, pop(&pq));
        setVisited(ptr);
        if (ptr->mHeadEdges== NULL) {
            continue;
        }
        else {
            Edge* edgePtr = ptr->mHeadEdges;
            while (edgePtr != NULL) {
                Node* n = getNode(graph, edgePtr->mDest);
                int res = ptr->mPriorityWeight + edgePtr->mWeight;
                if (res < n->mPriorityWeight) {
                    n->mPriorityWeight = res;
                }
                if (n->mIsVisited == 0)
                {
                    push(&pq, n->mId, n->mPriorityWeight);
                }

                edgePtr = edgePtr->mNext;
            }
            if (ptr->mId == destNode)
            {
                while (!isEmpty(&pq)) {
                    pop(&pq);
                }
                printf("Dijsktra shortest path: %d \n", ptr->mPriorityWeight);
                return;
            }
        
        }
    }
    while (!isEmpty(&pq)) {
        pop(&pq);
    }

    printf("Dijsktra shortest path: %d \n", -1);

}


void setAllPriorityNodesWeightToMax(DirectedGraph* directed) {

    Node* temp = directed->mHeadNodes;
    while (temp != NULL)
    {
        temp->mPriorityWeight = MAX;
        temp->mIsVisited = 0;
        temp = temp->mNextNode;
    }
}

int main() {
    DirectedGraph* directedGraph = NULL;
    char textInput[SIZE] = { 0 };
    fgets(textInput, SIZE, stdin);
    int i = 0;
    while (i < strlen(textInput)) {     
        enum Action action= getAction(textInput[i]);
        i += 2;
        switch (action)
        {
            case A_INIT_GRAPH:
            {

                if (directedGraph != NULL)
                {
                    freeGraph(directedGraph);
                    free(directedGraph);
                }
                directedGraph = initGraph(convertCharNumberToInt(textInput[i]));
                i += 2;
                addAllNodesToGraph(directedGraph, textInput, &i);
                break;
            }
            case B_ADD_NODE:
            {

                Node* addedNode = addNodeToGraph(directedGraph, convertCharNumberToInt(textInput[i]));
                i += 2;
                while (shouldStop(textInput[i]) == 0)
                {
                    Edge* edge = (Edge*)calloc(1, sizeof(Edge));
                    edge->mDest = convertCharNumberToInt(textInput[i]);
                    i += 2;
                    edge->mWeight = convertCharNumberToInt(textInput[i]);
                    i += 2;
                    addEdgeToNode(addedNode, edge);
                }
                break;
            }
            case D_DELETE_NODE:
            {  
              deleteNode(directedGraph, convertCharNumberToInt(textInput[i]));
              i += 2;
              break;
            }
            case S_SHORT_PATH:
            {
                setAllPriorityNodesWeightToMax(directedGraph);
                int srcNode = convertCharNumberToInt(textInput[i]);
                i += 2;
                int destNode = convertCharNumberToInt(textInput[i]);
                dijkstra(directedGraph, srcNode, destNode);
                break;
            }
            case T_TSP:
            {
                //printGraph(directedGraph);
                int sizeCityInput = 0;
                int* citiesInput = createCities(textInput, &i, &sizeCityInput);
                int path = performTsp(directedGraph, citiesInput, sizeCityInput);
                printf("TSP shortest path: %d \n", path);
                free(citiesInput);
                break;
            }
            default: {
                i += 2;
            }
        }
    }
   
    if (directedGraph != NULL)
    {
        freeGraph(directedGraph);
        free(directedGraph);
    }
}