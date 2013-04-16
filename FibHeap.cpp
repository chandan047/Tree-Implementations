#include "FibHeap.h"
#include <iostream>
#include <cstring>
using namespace std;

// Constrctor
FibHeap::FibHeap(){
    RootHeap = NULL;
    MaxHeap = NULL;
}

// Destructor
FibHeap::~FibHeap(){
    HeapNode* Node = RootHeap;
    while (Node != NULL){
            HeapNode* pNextNode = Node->Right;
            DeleteTree(Node);
            Node = pNextNode;
    }
}

/*
 *  Insert element into root list.
 */
void FibHeap::Insert(int hpInfo){
    HeapNode* newNode = NewNode(hpInfo);
    AddToRootHeap(newNode);
}

/*
 *  Move all children of MaxNode to Root List
 *  Remove MaxNode from Root List
 *  Assignment new MaxNode
 *  Consolidate the Heap to Fib Heap
 */
void FibHeap::ExtractMax(){
    if(MaxHeap==NULL)
        return;
    
    LevelUp(MaxHeap);
    
    RemoveNode(MaxHeap);
    MaxHeap = NULL;
    
    AssignMaxNode();

    Consolidate();
}

// Printing The Fib Heap
void FibHeap::DumpHeap(){
    HeapNode* Node = RootHeap;
    while(Node != NULL){
        cout<<"----------------\nRoot : ";
        if(Node==MaxHeap)
            cout<<"*";
        DumpTree(Node);
        Node = Node->Right;
    }
}

// Function to update MaxNode
void FibHeap::AssignMaxNode(){
    HeapNode* CheckNode = RootHeap;
    while( CheckNode != NULL ){
        if( MaxHeap==NULL || MaxHeap->key<CheckNode->key )
            MaxHeap = CheckNode;
        CheckNode = CheckNode->Right;
    }
}

// Function to remove Node from Root List
void FibHeap::RemoveNode(HeapNode* Node){
    if(Node == NULL)
        return;
    
    if(Node == RootHeap)
        RootHeap = RootHeap->Right;
    if(Node->Left)
        Node->Left->Right = Node->Right;
    if(Node->Right)
        Node->Right->Left = Node->Left;
    delete Node;
}

// Function to move children of Node to Root List
void FibHeap::LevelUp(HeapNode* Node){
    for(int i=0;i<MAX_DEGREE;i++){
        HeapNode* ChildNode = Node->Child[i];
        if(ChildNode==NULL)
            continue;
        AddToRootHeap(ChildNode);
        ChildNode->Parent = NULL;
        Node->Child[i] = NULL;
    }
    Node->Degree = 0;
}

// Adds Node left to Root and Node is new Root
void FibHeap::AddToRootHeap(HeapNode* Node){
    if(RootHeap==NULL){
        RootHeap = Node;
        MaxHeap = Node;
        return;
    }
    
    Node->Right = RootHeap;
    RootHeap->Left = Node;
    RootHeap = Node;
    if( MaxHeap->key < RootHeap->key )
        MaxHeap = RootHeap;
}

// Returns new HeapNode with hpInfo
HeapNode* FibHeap::NewNode(int hpInfo){
    HeapNode* Node = new HeapNode;
    Node->Left = NULL;
    Node->Right = NULL;
    Node->Parent = NULL;
    Node->key = hpInfo;
    Node->Degree = 0;
    Node->mark = true;
    
    memset(Node->Child,0,sizeof(Node->Child));
    
    return Node;
}

// Consolidates the current Heap to Fibonacci Heap
void FibHeap::Consolidate(){
    HeapNode* DegreeNode[MAX_DEGREE];
    HeapNode* Node = RootHeap;
    
    memset(DegreeNode,0,sizeof(DegreeNode));
    while( Node !=NULL ){
        if( DegreeNode[Node->Degree]==NULL ){
            DegreeNode[Node->Degree] = Node;
            Node = Node->Right;
        }
        else{
            HeapNode* PreNode = DegreeNode[Node->Degree];
            if (PreNode->key < Node->key)
                AttachTree(PreNode,Node);
            else
                AttachTree(Node,PreNode);
            
            // Reset the search
            memset(DegreeNode,0,sizeof(DegreeNode));
            Node = RootHeap;
        }
    }
    
}

void FibHeap::AttachTree(HeapNode* FromNode, HeapNode* ToNode){
    if( FromNode==RootHeap )
        RootHeap = RootHeap->Right;
    
    // Break the link of FromNode
    if(FromNode->Left)
        FromNode->Left->Right = FromNode->Right;
    if (FromNode->Right)
        FromNode->Right->Left = FromNode->Left;
    FromNode->Left = NULL;
    FromNode->Right = NULL;

    // Move the FromNode under the ToNode
    for(int i=0;i<MAX_DEGREE;i++)
        if( ToNode->Child[i]==NULL ){
            ToNode->Child[i] = FromNode;
            break;
        }
    ToNode->Degree++;
    FromNode->Parent = ToNode;
    
    return;
}

void FibHeap::DumpTree(HeapNode* Node){
    cout<<Node->key<<endl;
    for(int i=0;i<MAX_DEGREE;i++){
        HeapNode* ChildNode = Node->Child[i];
        if(ChildNode!=NULL)
            cout<<ChildNode->key<<endl;
    }
    cout<<endl;
    for(int i=0;i<MAX_DEGREE;i++){
        HeapNode* ChildNode = Node->Child[i];
        if(ChildNode!=NULL)
            DumpTree(ChildNode);
    }
}

void FibHeap::DeleteTree(HeapNode* Node){
    if(Node==NULL)
        return;
    
    for(int i=0;i<MAX_DEGREE;i++){
        HeapNode* ChildNode = Node->Child[i];
        if(ChildNode!=NULL)
            DeleteTree(ChildNode);
    }
    
    delete Node;
}

// Melds 'this' FibHeap with 'F'
FibHeap FibHeap::UniteHeap(FibHeap F){
    FibHeap H;
    HeapNode* pMax = MaxHeap;
    H.MaxHeap = pMax;
    H.RootHeap = RootHeap;
    HeapNode* firstEnd = H.RootHeap->Left;
    firstEnd->Right = F.RootHeap;
    F.RootHeap->Left = firstEnd;
    if ( H.MaxHeap->key < F.MaxHeap->key)
        H.MaxHeap = F.MaxHeap;
    return H;
}

// Cuts Node x from its parent y and puts in Root List
void FibHeap::Cut(HeapNode *x, HeapNode *y){
    int i=0;
    for (i=0;i<MAX_DEGREE;i++)
        if ( y->Child[i] == x )
            break;
    y->Child[i] = NULL;
    y->Degree--;
    AddToRootHeap(x);
    x->Parent = NULL;
    x->mark = false;
}


void FibHeap::CascadingCut(HeapNode* y){
    HeapNode* z = y->Parent;
    if(z!=NULL){
        if(y->mark==false)
            y->mark = true;
        else{
            Cut(y,z);
            CascadingCut(z);
        }
    }
    return;
}

// Increments Key of Node x to k and re-arranges Heap
void FibHeap::IncreaseKey(HeapNode* x, int k){
    if( k < x->key )
        return;
    x->key = k;
    HeapNode* y = x->Parent;
    if (y!=NULL && x->key>y->key ){
        Cut(x,y);
        CascadingCut(y);
    }
    if(x->key>MaxHeap->key)
        MaxHeap = x;
    return;
}
