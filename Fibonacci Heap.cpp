
#include <cstdlib>
#include <iostream>
#include <cstring>
using namespace std;

#define MAX_DEGREE 20
/////////////////////////////////////////////////
typedef struct {
    int key;
} stHeapInfo;

typedef struct stHeapNode{
    stHeapNode *pParent;
    stHeapNode *pLeft;
    stHeapNode *pRight;
    stHeapNode *ppChild[MAX_DEGREE];
    int nchild;
    int nDegree;
    stHeapInfo hpInfo;
} stHeapNode;

class FibHeap
{
public:
    FibHeap();
    ~FibHeap();
    void Insert(stHeapInfo hpInfo);
    void ExtractMax(stHeapInfo* hpInfo);
    void DumpHeap();
    FibHeap UniteHeap(FibHeap F);
    
private:
    stHeapNode* m_pRootHeap;
    stHeapNode* m_pMaxHeap;
    
    stHeapNode* m_fnNewNode(stHeapInfo hpInfo);
    void m_fnAddToRootHeap(stHeapNode* pNode);
    void m_fnConsolidate();
    void m_fnAttachTree(stHeapNode* pFromNode,stHeapNode* pToNode);
    void m_fnLevelUp(stHeapNode* pNode);
    void m_fnRemoveNode(stHeapNode* pNode);
    void m_fnAssignMinNode();           // find a minimum node
    void m_fnDumpTree(stHeapNode* pNode);
    void m_fnDeleteTree(stHeapNode* pNode);
    
};

FibHeap::FibHeap(){
    m_pRootHeap = NULL;
    m_pMaxHeap = NULL;
}

FibHeap::~FibHeap(){
    stHeapNode* pNode = m_pRootHeap;
    while (pNode != NULL){
            stHeapNode* pNextNode = pNode->pRight;
            m_fnDeleteTree(pNode);
            pNode = pNextNode;
    }
}

void FibHeap::Insert(stHeapInfo hpInfo){
    stHeapNode* pNewNode = m_fnNewNode(hpInfo);
    m_fnAddToRootHeap(pNewNode);
}

void FibHeap::ExtractMax(stHeapInfo* hpInfo){
    if(m_pMaxHeap==NULL)
        return;
    *hpInfo = m_pMaxHeap->hpInfo;
    
    //Move children into root
    m_fnLevelUp(m_pMaxHeap);
    
    //Remove the minimum node
    m_fnRemoveNode(m_pMaxHeap);
    m_pMaxHeap = NULL;
    
    //Re-assign a new one for m_MinHeap
    m_fnAssignMinNode();

    //Consolidate the heap trees
    m_fnConsolidate();
}

void FibHeap::DumpHeap(){
    stHeapNode* pNode = m_pRootHeap;
    while(pNode != NULL){
        cout<<"--------------------\n";
        if(pNode==m_pMaxHeap)
            cout<<"*";
        m_fnDumpTree(pNode);
        pNode = pNode->pRight;
    }
}

void FibHeap::m_fnAssignMinNode(){
    stHeapNode* pCheckNode = m_pRootHeap;
    while( pCheckNode != NULL ){
        if( m_pMaxHeap==NULL || m_pMaxHeap->hpInfo.key<pCheckNode->hpInfo.key )
            m_pMaxHeap = pCheckNode;
        pCheckNode = pCheckNode->pRight;
    }
}

void FibHeap::m_fnRemoveNode(stHeapNode* pNode){
    if(pNode == NULL)
        return;
    
    if(pNode == m_pRootHeap)
        m_pRootHeap = m_pRootHeap->pRight;
    if(pNode->pLeft)
        pNode->pLeft->pRight = pNode->pRight;
    if(pNode->pRight)
        pNode->pRight->pLeft = pNode->pLeft;
    delete pNode;
}

void FibHeap::m_fnLevelUp(stHeapNode* pNode){
    for(int i=0;i<pNode->nchild;i++){
        stHeapNode* pChildNode = pNode->ppChild[i];
        m_fnAddToRootHeap(pChildNode);
        pChildNode->pParent = NULL;
        pNode->ppChild[i] = NULL;
    }
    pNode->nchild = 0;
}

void FibHeap::m_fnAddToRootHeap(stHeapNode* pNode){
    if(m_pRootHeap==NULL){
        m_pRootHeap = pNode;
        m_pMaxHeap = pNode;
        return;
    }
    
    pNode->pRight = m_pRootHeap;
    m_pRootHeap->pLeft = pNode;
    m_pRootHeap = pNode;
    if( m_pMaxHeap->hpInfo.key < m_pRootHeap->hpInfo.key )
        m_pMaxHeap = m_pRootHeap;
}

stHeapNode* FibHeap::m_fnNewNode(stHeapInfo hpInfo){
    stHeapNode* pNode = new stHeapNode;
    pNode->pLeft = NULL;
    pNode->pRight = NULL;
    pNode->pParent = NULL;
    pNode->hpInfo = hpInfo;
    pNode->nDegree = 0;
    pNode->nchild = 0;
    
    memset(pNode->ppChild,0,sizeof(pNode->ppChild));
    
    return pNode;
}

void FibHeap::m_fnConsolidate(){
    stHeapNode* ppDegreeNode[MAX_DEGREE];
    stHeapNode* pNode = m_pRootHeap;
    
    memset(ppDegreeNode,0,sizeof(ppDegreeNode));
    while( pNode !=NULL ){
        if( ppDegreeNode[pNode->nDegree]==NULL ){
            ppDegreeNode[pNode->nDegree] = pNode;
            pNode = pNode->pRight;
        }
        else{
            stHeapNode* pPreNode = ppDegreeNode[pNode->nDegree];
            if (pPreNode->hpInfo.key < pNode->hpInfo.key)
                m_fnAttachTree(pPreNode,pNode);
            else
                m_fnAttachTree(pNode,pPreNode);
            
            // Reset the search
            memset(ppDegreeNode,0,sizeof(ppDegreeNode));
            pNode = m_pRootHeap;
        }
    }
    
}

void FibHeap::m_fnAttachTree(stHeapNode* pFromNode, stHeapNode* pToNode){
    if( pFromNode==m_pRootHeap )
        m_pRootHeap = m_pRootHeap->pRight;
    
    // Break the link of pFromNode
    if(pFromNode->pLeft)
        pFromNode->pLeft->pRight = pFromNode->pRight;
    if (pFromNode->pRight)
        pFromNode->pRight->pLeft = pFromNode->pLeft;
    pFromNode->pLeft = NULL;
    pFromNode->pRight = NULL;

    // Move the pFromNode under the pToNode
    pToNode->ppChild[pToNode->nchild] = pFromNode;
    pToNode->nchild++;
    pToNode->nDegree++;
    pFromNode->pParent = pToNode;
    
    return;
}

void FibHeap::m_fnDumpTree(stHeapNode* pNode){
    cout<<pNode->hpInfo.key<<endl;
    for(int i=0;i<pNode->nchild;i++){
        stHeapNode* pChildNode = pNode->ppChild[i];
        cout<<pChildNode->hpInfo.key<<endl;
    }
    cout<<endl;
    for(int i=0;i<pNode->nchild;i++){
        stHeapNode* pChildNode = pNode->ppChild[i];
        m_fnDumpTree(pChildNode);
    }
}

void FibHeap::m_fnDeleteTree(stHeapNode* pNode){
    if(pNode==NULL)
        return;
    
    for(int i=0;i<pNode->nchild;i++){
        stHeapNode* pChildNode = pNode->ppChild[i];
        m_fnDeleteTree(pChildNode);
    }
    
    delete pNode;
}

FibHeap FibHeap::UniteHeap(FibHeap F){
    FibHeap H;
    stHeapNode* pMax = m_pMaxHeap;
    H.m_pMaxHeap = pMax;
    H.m_pRootHeap = m_pRootHeap;
    
}


////////////////////////////////////////////////////////////////////////////////////////////////
//    main files

void test1()
{
        printf("[Test1] \n");
        FibHeap fiboHeap;
        stHeapInfo heapInfo;

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
        stHeapInfo heapInfo;
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
