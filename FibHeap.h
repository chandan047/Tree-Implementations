/* 
 * File:   FibHeap.h
 * Author: chandanakiti
 *
 * Created on 15 April 2013, 3:59 AM
 */

#ifndef FIBONACCI_H
#define	FIBONACCI_H

#define MAX_DEGREE 20
/////////////////////////////////////////////////

typedef struct HeapNode{
    HeapNode *Parent;
    HeapNode *Left;
    HeapNode *Right;
    HeapNode *Child[MAX_DEGREE];
    int Degree;
    bool mark;
    int key;
} HeapNode;

class FibHeap
{
public:
    HeapNode* RootHeap;
    HeapNode* MaxHeap;
    
    FibHeap();
    ~FibHeap();
    void Insert(int hpInfo);
    void ExtractMax();
    FibHeap UniteHeap(FibHeap F);
    void IncreaseKey(HeapNode* x, int k);
    void DumpHeap();
    
private:
    
    HeapNode* NewNode(int hpInfo);
    void AddToRootHeap(HeapNode* pNode);
    void Consolidate();
    void AttachTree(HeapNode* pFromNode,HeapNode* pToNode);
    void LevelUp(HeapNode* pNode);
    void RemoveNode(HeapNode* pNode);
    void AssignMaxNode();           // find a maximum node
    void DumpTree(HeapNode* pNode);
    void DeleteTree(HeapNode* pNode);
    void Cut(HeapNode*,HeapNode*);
    void CascadingCut(HeapNode*);
    
};

#endif	/* FIBONACCI_H */
