/*
 * main.cpp
 *    	IMPLEMENTATION OF LEFTIEST MAX HEAP
 *	
 *  Created on: 20/03/2013
 *      Author: chandanakiti
 */


#include <iostream>
#include <stack>
#include <cstdlib>
#include <cmath>
using namespace std;

// ADT of a Node in tree
class TreeNode{
	public:
		TreeNode* p;	// parent node pointer
		int key;
		int npl;		// Null Path Length
		int height;		// height for Node to farthest leaf
		TreeNode* left;
		TreeNode* right;
	public:
		TreeNode(TreeNode* _p,TreeNode* _left,TreeNode* _right){
			p = _p;
			key = 0;
			npl = 0;
			height = -1;
			left=_left;
			right = _right;
		}
};

// ADT of Leftiest Max Heap
class Leftiest_Heap{
	public:
		TreeNode* root;		// root of the Leftiest Heap
		TreeNode* NIL;		// Sentinal node of Leftiest Heap
	private:				// local functions on Leftiest Heap
		int height(TreeNode*);
		void LH_npl_update(TreeNode*);
		void LH_fixup(TreeNode*);
	public:					// global functions on Leftiest Heap
		Leftiest_Heap();
		void LH_construct();
		void LH_merge(TreeNode*);
		void LH_insert(int);
		void LH_delMax();
		void print(TreeNode*);
};

Leftiest_Heap::Leftiest_Heap()		// constructor of Leftiest Heap
{
	NIL = new TreeNode(NULL,NULL,NULL);
	root = NIL;
	NIL->left = NIL;
	NIL->right = NIL;
	NIL->key = -1000;
	NIL->npl = -1;
}

/*
 *	If : right child's Null Path Length(npl) > left child's npl 
 *		then : swap left and right childs 
 */
void Leftiest_Heap::LH_fixup(TreeNode* p)
{
	if (p->right->npl > p->left->npl){
		TreeNode* tmp = p->left;
		p->left = p->right;
		p->right = tmp;
	}
	return;
}

/*
 *	updated npl of TreeNode p = 1 + min(left child`s npl, right child`s npl) 
 */
void Leftiest_Heap::LH_npl_update(TreeNode* p)
{
	int rgt_buffer = p->right->npl;
	int lft_buffer = p->left->npl;
	if (rgt_buffer>lft_buffer)
		p->npl = lft_buffer+1;
	else
		p->npl = rgt_buffer+1;
	return;
}

/*
 *	Merging (this)tree with a tree having Root : _root
 */
void Leftiest_Heap::LH_merge(TreeNode* _root)
{
	if(!_root)		// If there is no tree to be merged
		return;
	else if (root==NIL) {	// if current tree is NIL
		root = _root;
		return;
	}
	else{
		TreeNode* node1=root,*node2=_root;
		if (node1->key <= node2->key)
			root = node2;
		stack<TreeNode*> s;
		while (node1!=NIL && node2!=NIL) {
			if(node1->key <= node2->key){
				s.push(node2);
				node2 = node2->right;
			}
			else {
				s.push(node1);
				node1 = node1->right;
			}
		}
		if (node1==NIL){
			TreeNode* tmp = node1;
			node1 = node2;
			node2 = tmp;
		}
		while (!s.empty()) {
			node2 = s.top();	s.pop();
			node2->right = node1;
			node1->p = node2;
			LH_fixup(node2);
			LH_npl_update(node2);
			node1 = node2;
		}
	}
	return;
}

/*
 *	Make a TreeNode with key 'n' : This can be considered as a tree with a single TreeNode
 *	Merge the current tree with the new single-node Tree
 */
void Leftiest_Heap::LH_insert(int n)
{
	TreeNode* node = new TreeNode(NULL,NIL,NIL);
	node->key = n;
	this->LH_merge(node);
	return;
}

/*
 *	Maximum key lies with root of Leftiest Max Heap 
 *	merge left and right sub-trees of root
 *	delete the previous root
 */
void Leftiest_Heap::LH_delMax() 
{
	TreeNode *node = root->right , *tmp = root;
	root = root->left;
	LH_merge(node);
	delete(tmp);
	return;
}

/*
 *	returns the distance between the node and its farthest child
 */
int Leftiest_Heap::height(TreeNode* node)
{
	int h;
	if (node==NIL)
		h = 0;
	else if (height(node->right) >= height(node->left))
		h = 1+height(node->right);
	else
		h = 1+height(node->left);
	return h;
}

/*
 *	print the tree respective to levels from root
 *		2-stack implementation : 
 *			every 'for' loop extracts children of previous level in order into stack 's'
 *			buffer stack used to hold nodes in reverse order while extracting children from previous nodes in 's'
 */
void Leftiest_Heap::print(TreeNode* p)
{
	cout<<"\nThe Tree is : ";

	int h = height(p);
	
	if (h==0){
		cout<<"Empty\n\n";
		return;
	}
	else{
		cout<<"(height = "<<h<<")"<<endl;
		stack<TreeNode*> s;
		stack<TreeNode*> buff;
		s.push(p);
		cout<<1<<'.'<<p->key<<endl<<endl;
		for (int i=1; i<h; i++){
			while (!s.empty()){
				TreeNode* tmp = s.top();	s.pop();
				buff.push(tmp->right);
				buff.push(tmp->left);
			}
			int j=1;
			while (!buff.empty()){
				TreeNode* tmp = buff.top();	buff.pop();
				cout<<j<<'.';
				if (tmp!=NIL)
					cout<<tmp->key<<"\t";
				else
					cout<<"\t";
				s.push(tmp);
				j++;
			}
			cout<<endl<<endl;
		}
		return;
	}
}


int main()
{
	Leftiest_Heap *L=NULL;
	L = new Leftiest_Heap();
	
	int n=0;	// choice
	while (n!=3) 
	{
		std::cout << "1.Insert element \n" ;
		std::cout << "2.Delete maximum element \n" ;
		std::cout << "3.Exit \n";
		std::cin >> n;
		switch(n)
		{
			case 1:		// insert element
				int p;
				cin>>p;
				L->LH_insert(p);
				L->print(L->root);
				break;
			case 2:		// delete max(root)
				L->LH_delMax();
				L->print(L->root);
				break;
			case 3:		// stop the program
				cout<<"<....exiting the program....>\n";
				break;
			default:
				cout<<"<....wrong option....>\n";
				break;
		}
	}
	
	return 0;
}
