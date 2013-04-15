#include <cstdlib>
#include <iostream>
#include <cstring>
using namespace std;

#define MAX_DEGREE 20
/////////////////////////////////////////////////
typedef struct {
    int key;
} HeapInfo;

typedef struct HeapNode{
    HeapNode *Parent;
    HeapNode *Left;
    HeapNode *Right;
    HeapNode *Child[MAX_DEGREE];
    int child;
    int Degree;
    HeapInfo hpInfo;
} HeapNode;

class FibHeap
{
public:
    FibHeap();
    ~FibHeap();
    void Insert(HeapInfo hpInfo);
    void ExtractMax(HeapInfo* hpInfo);
    void DumpHeap();
    FibHeap UniteHeap(FibHeap F);
    
private:
    HeapNode* RootHeap;
    HeapNode* MaxHeap;
    
    HeapNode* NewNode(HeapInfo hpInfo);
    void AddToRootHeap(HeapNode* pNode);
    void Consolidate();
    void AttachTree(HeapNode* pFromNode,HeapNode* pToNode);
    void LevelUp(HeapNode* pNode);
    void RemoveNode(HeapNode* pNode);
    void AssignMinNode();           // find a minimum node
    void DumpTree(HeapNode* pNode);
    void DeleteTree(HeapNode* pNode);
    
};

FibHeap::FibHeap(){
    RootHeap = NULL;
    MaxHeap = NULL;
}

FibHeap::~FibHeap(){
    HeapNode* Node = RootHeap;
    while (Node != NULL){
            HeapNode* pNextNode = Node->Right;
            DeleteTree(Node);
            Node = pNextNode;
    }
}

void FibHeap::Insert(HeapInfo hpInfo){
    HeapNode* newNode = NewNode(hpInfo);
    AddToRootHeap(newNode);
}

void FibHeap::ExtractMax(HeapInfo* hpInfo){
    if(MaxHeap==NULL)
        return;
    *hpInfo = MaxHeap->hpInfo;
    
    //Move children into root
    LevelUp(MaxHeap);
    
    //Remove the minimum node
    RemoveNode(MaxHeap);
    MaxHeap = NULL;
    
    //Re-assign a new one for m_MinHeap
    AssignMinNode();

    //Consolidate the heap trees
    Consolidate();
}

void FibHeap::DumpHeap(){
    HeapNode* Node = RootHeap;
    while(Node != NULL){
        cout<<"--------------------\n";
        if(Node==MaxHeap)
            cout<<"*";
        DumpTree(Node);
        Node = Node->Right;
    }
}

void FibHeap::AssignMinNode(){
    HeapNode* CheckNode = RootHeap;
    while( CheckNode != NULL ){
        if( MaxHeap==NULL || MaxHeap->hpInfo.key<CheckNode->hpInfo.key )
            MaxHeap = CheckNode;
        CheckNode = CheckNode->Right;
    }
}

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

void FibHeap::LevelUp(HeapNode* Node){
    for(int i=0;i<Node->child;i++){
        HeapNode* pChildNode = Node->Child[i];
        AddToRootHeap(pChildNode);
        pChildNode->Parent = NULL;
        Node->Child[i] = NULL;
    }
    Node->child = 0;
}

void FibHeap::AddToRootHeap(HeapNode* Node){
    if(RootHeap==NULL){
        RootHeap = Node;
        MaxHeap = Node;
        return;
    }
    
    Node->Right = RootHeap;
    RootHeap->Left = Node;
    RootHeap = Node;
    if( MaxHeap->hpInfo.key < RootHeap->hpInfo.key )
        MaxHeap = RootHeap;
}

HeapNode* FibHeap::NewNode(HeapInfo hpInfo){
    HeapNode* Node = new HeapNode;
    Node->Left = NULL;
    Node->Right = NULL;
    Node->Parent = NULL;
    Node->hpInfo = hpInfo;
    Node->Degree = 0;
    Node->child = 0;
    
    memset(Node->Child,0,sizeof(Node->Child));
    
    return Node;
}

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
            if (PreNode->hpInfo.key < Node->hpInfo.key)
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
    ToNode->Child[ToNode->child] = FromNode;
    ToNode->child++;
    ToNode->Degree++;
    FromNode->Parent = ToNode;
    
    return;
}

void FibHeap::DumpTree(HeapNode* Node){
    cout<<Node->hpInfo.key<<endl;
    for(int i=0;i<Node->child;i++){
        HeapNode* pChildNode = Node->Child[i];
        cout<<pChildNode->hpInfo.key<<endl;
    }
    cout<<endl;
    for(int i=0;i<Node->child;i++){
        HeapNode* pChildNode = Node->Child[i];
        DumpTree(pChildNode);
    }
}

void FibHeap::DeleteTree(HeapNode* Node){
    if(Node==NULL)
        return;
    
    for(int i=0;i<Node->child;i++){
        HeapNode* pChildNode = Node->Child[i];
        DeleteTree(pChildNode);
    }
    
    delete Node;
}

FibHeap FibHeap::UniteHeap(FibHeap F){
    FibHeap H;
    HeapNode* pMax = MaxHeap;
    H.MaxHeap = pMax;
    H.RootHeap = RootHeap;
    
}


////////////////////////////////////////////////////////////////////////////////////////////////
//    main files

void test1()
{
        printf("[Test1] \n");
        FibHeap fiboHeap;
        HeapInfo heapInfo;

        heapInfo.key = 1;
        fiboHeap.Insert(heapInfo);

        heapInfo.key = 3;
        fiboHeap.Insert(heapInfo);

        heapInfo.key = 2;
        fiboHeap.Insert(heapInfo);

        heapInfo.key = 5;
        fiboHeap.Insert(heapInfo);

        heapInfo.key = 9;
        fiboHeap.Insert(heapInfo);

        fiboHeap.DumpHeap();

        // It should be 1
        fiboHeap.ExtractMax(&heapInfo);
        printf("Extract max = %d\n", heapInfo.key);
        fiboHeap.DumpHeap();
       
        // It should be 2
        fiboHeap.ExtractMax(&heapInfo);
        printf("Extract max = %d\n", heapInfo.key);
        fiboHeap.DumpHeap();
 
        // It should be 3
        fiboHeap.ExtractMax(&heapInfo);
        printf("Extract max = %d\n", heapInfo.key);
        fiboHeap.DumpHeap();
        
        // It should be 5
        fiboHeap.ExtractMax(&heapInfo);
        printf("Extract max = %d\n", heapInfo.key);
        fiboHeap.DumpHeap();
 
        // It should be 9
        fiboHeap.ExtractMax(&heapInfo);
        printf("Extract max = %d\n", heapInfo.key);
        fiboHeap.DumpHeap();

        // Error handling
        fiboHeap.ExtractMax(&heapInfo);
        fiboHeap.DumpHeap();
        printf("[Test1] Complete!\n\n");
}

void test2()
{
        printf("[Test2] \n");
        FibHeap fiboHeap;
        HeapInfo heapInfo;
        int maxTest = 10;

        srand(0);
        for (int i = 0; i < maxTest; i++)
        {
                int nR = rand() % 100;
                heapInfo.key = nR;

                fiboHeap.Insert(heapInfo);
        }
        fiboHeap.DumpHeap();
        for (int i = 0; i < maxTest; i++)
        {
                fiboHeap.ExtractMax(&heapInfo);
                printf("Extract max = %d\n", heapInfo.key);
        fiboHeap.DumpHeap();
        }
        printf("[Test2] Complete!\n");
}


/*
 * 
 */
int main(int argc, char** argv) {
    test1();
    test2();
    
    return 0;
}
