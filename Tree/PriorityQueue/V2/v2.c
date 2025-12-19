#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 10
#define EMPTYLAST -1
// #define EMPTY (int)(pow(2, 32) - 1)

typedef int element, index;

typedef struct{
    element Elements[MAX];
    index lastIdx;
} SET;

void init(SET*);
void insert(SET*, element);
void heapifyUp(SET*);
void heapifyDown(SET*, index);
void heap(SET*);
int deletemin(SET*);
void sort(SET*);
void display(SET);


// Pre, In, and Post Orders
void preOrder(SET, index);
void inOrder(SET, index);
void postOrder(SET, index);


int main(){
    SET A;
    init(&A);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    insert(&A, 5);
    insert(&A, 15);
    insert(&A, 7);
    insert(&A, 3);
    insert(&A, 20);
    insert(&A, 10);
    insert(&A, 11);
    display(A);
    heap(&A);
    sort(&A);
    display(A);


    printf("\nPreOrder: ");
    printf("\nInOrder: ");
    inOrder(A, 0);

    printf("\nPostOrder: ");
    postOrder(A, 0);
    return 0;
}


void init(SET* A){
    A->lastIdx = EMPTYLAST;
}

void insert(SET *A, element elem){
    if(A->lastIdx + 1 < MAX){
        A->Elements[++A->lastIdx] = elem;
    }
}

void heapifyUp(SET* A){
    index node = A->lastIdx;

    while(node > 0 && A->Elements[node] < A->Elements[(node-1)/2]){
        element temp = A->Elements[(node-1)/2];
        A->Elements[(node-1)/2] = A->Elements[node];
        A->Elements[node] = temp;

        node = (node-1)/2;
    }
}

void heapifyDown(SET* A, index root){
    bool satisfied = false;
    while(!satisfied){
        index smallest = root;
        index lc = root * 2 + 1;
        index rc = root * 2 + 2;

        if(lc <= A->lastIdx && A->Elements[lc] < A->Elements[smallest]) smallest = lc;
        if(rc <= A->lastIdx && A->Elements[rc] < A->Elements[smallest]) smallest = rc;

        if(root != smallest){
            element temp = A->Elements[root];
            A->Elements[root] = A->Elements[smallest];
            A->Elements[smallest] = temp;
            root = smallest;
        }else{
            satisfied = true;
        }
    }
}

void heap(SET* A){
    index trav = (A->lastIdx-1)/2;
    for(; trav >= 0; trav--){
        heapifyDown(A, trav);
    }
}

int deletemin(SET* A){
    element temp = A->Elements[0];
    A->Elements[0] = A->Elements[A->lastIdx];
    A->Elements[A->lastIdx--] = temp;

    heapifyDown(A, 0);

    return temp;
}

void sort(SET* A){
    element temp_element[MAX];
    index tempIdx = A->lastIdx;
    for(int trav = 0; trav <= tempIdx; trav++){
        temp_element[trav] = deletemin(A);
        printf("Element [%d]: %d\n", trav, temp_element[trav]);
    }
    
    A->lastIdx = tempIdx;
    for(int i = 0; i <= A->lastIdx; i++){
        A->Elements[i] = temp_element[i];
    }
}

void display(SET A){
    printf("\n");
    for(int trav = 0; trav <= A.lastIdx; trav++){
        printf("%d ", A.Elements[trav]);
    }
    printf("\n");
}


void preOrder(SET A, index head){
    printf("%d ", A.Elements[head]);
    for(int trav = 1; trav <= 2; trav++){
        if(head * 2 + trav <= A.lastIdx){
            preOrder(A, head * 2 + trav);
        }
    }
}


void inOrder(SET A, index head){
    index leftChild = head * 2 + 1;
    if(leftChild > A.lastIdx){
        printf("%d ", A.Elements[head]); // leaf node
    }else{
        inOrder(A, leftChild);
        printf("%d ", A.Elements[head]);
        /*
        Since the structure is similar to binary tree (which means only 2 children)
        traversing is not necessary. So, I will just directly check its right child.
        */
       index rightChild = head * 2 + 2;
        if(rightChild <= A.lastIdx){
            inOrder(A, rightChild);
        }
    }
}

void postOrder(SET A, index head){
    index leftChild = head * 2 + 1;
    for(int trav = 1; trav <= 2; trav++){
        if(head * 2 + trav <= A.lastIdx){
            postOrder(A, head * 2 + trav);
        }
    }
    printf("%d ", A.Elements[head]);
}

// void preOrder(SET A){
//     int pointA = 0, pointB = 0;
//     index parent = 0;
//     index leftChild, rightChild;
//     SET B;
//     init(&B);
//     B.Elements[++B.lastIdx] = A.Elements[B.lastIdx];

//     if(A.lastIdx != EMPTYLAST){
//         do{
//             leftChild = parent * 2 + 1;
//             rightChild = parent * 2 + 2;
//             pointB = (leftChild <= A.lastIdx)? +1: -1;
//             if(pointB > pointA){
//                 B.Elements[++B.lastIdx] = A.Elements[leftChild];
//                 parent = leftChild;
//             }else if(pointB < pointA){
//                 parent = (parent-1)/2;
//                 rightChild = parent * 2 + 2;
//                 if(rightChild <= A.lastIdx){
//                     B.Elements[++B.lastIdx] = A.Elements[rightChild];
//                     pointA = pointB;
//                     pointB++;
//                 }
//             }
//             pointA = pointB;
//         }while(pointA != pointB || pointA != 0);
//     }
// }