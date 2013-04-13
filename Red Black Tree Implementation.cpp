/*
 * main.cpp
 *
 *  Created on: 13/03/2013
 *      Author: Chandan Reddy Akiti
 */

#include <iostream>
#include <string>
#include <stack>
using namespace std;

// ADT of a Node in tree
class TreeNode{
public:
  TreeNode* p;	// parent node pointer
	int key;
	TreeNode* left;
	TreeNode* right;
	string color;
public:
	TreeNode(TreeNode* _p,TreeNode* _left,TreeNode* _right){
		p = _p;
		key = 0;
		left=_left;
		right = _right;
		color = "BLACK";
	}
};

// ADT of a RED-BLACK tree
class RB_Tree{
public:
	TreeNode* root;
	TreeNode* NIL;		// sentinal node

// Local functions for class RB_Tree
private:
	void RB_left_rotate(TreeNode*);
	void RB_right_rotate(TreeNode*);
	void RB_insert_fixup(TreeNode*);
	void RB_delete_fixup(TreeNode*);
	void RB_transplant(TreeNode*,TreeNode*);
	TreeNode* RB_minSub(TreeNode*);\

// Global functions on Tree of class RB_Tree	
public:
	RB_Tree(){		// Constructor for class RB_Tree :: NIL is circularly linked to itself.
		NIL = new TreeNode(NULL,NULL,NULL);
		NIL->p = NIL;
		NIL->right = NIL;
		NIL->left = NIL;
		root = NIL;
	}
	void RB_insert(TreeNode*);
	void RB_delete(int);
	TreeNode* RB_search(int);
	TreeNode* RB_minimum();
	TreeNode* RB_maximum();
	TreeNode* RB_left_successor(int);
	TreeNode* RB_right_successor(int);
	TreeNode* RB_predecessor(int);
};

/*
 * 			y				  x
 *		   / \				 /
 *		  @   x		-->		y
 *			 /			   / \
 *			$			  @	  $
 */
void RB_Tree::RB_left_rotate(TreeNode* x){
	TreeNode* y = x->right;
	x->right = y->left;
	if (y->left!=NIL)
		y->left->p = x;
	y->p = x->p;
	if (x->p==NIL)
		root = y;
	else if (x==(x->p)->left)
		(x->p)->left = y;
	else
		(x->p)->right = y;
	y->left = x;
	x->p = y;
	return;
}

/*
 * 			x			y
 *		   /		   / \
 *		  y		--->  @   x
 *		 / \			 /
 *		@	$			$
 */
void RB_Tree::RB_right_rotate(TreeNode* y){
	TreeNode* x = y->left;
	y->left = x->right;
	if (x->right!=NIL)
		x->right->p = y;
	x->p = y->p;
	if (y->p==NIL)
		this->root = x;
	else if (y==y->p->right)
		y->p->right = x;
	else
		y->p->left = x;
	x->right = y;
	y->p = x;
	return;
}

/*
 *	Inserting element following with insertion algorithm of BST and color::BLACK
 */
void RB_Tree::RB_insert(TreeNode* z){
	if (root==NIL){
		root = z;
		NIL->left = root;
		NIL->right = root;
                z->p = NIL;
		z->left = NIL;
		z->right = NIL;
	}
	else {
		TreeNode* x=root;
		while ( 1 ) {
			if (x->key<z->key && x->right!=NIL){
				x = x->right;
			}
			else if (x->key>=z->key && x->left!=NIL){
				x = x->left;
			}
			else
				break;
		}
		if (x->key<z->key){
			x->right = z;
			z->p = x;
		}
		else{
			x->left = z;
			z->p = x;
		}
		z->right = NIL;
		z->left = NIL;
		z->color = "RED";
	}
	RB_insert_fixup(z);
	return;
}

/*
 *	Insertion fix-up to refrain the properties of RED-BLACK trees
 */
void RB_Tree::RB_insert_fixup(TreeNode* z){
	TreeNode* y=NULL;
	while (z->p->color=="RED"){
		if (z->p==z->p->p->left){
			y = z->p->p->right;
			if (y->color=="RED"){
				z->p->color = "BLACK";
				y->color = "BLACK";
				z->p->p->color = "RED";
				z = z->p->p;
			}
			else{
				if (z==z->p->right) {
					z = z->p;
					RB_left_rotate(z);
				}
				z->p->color = "BLACK";
				z->p->p->color = "RED";
				RB_right_rotate(z->p->p);
			}
		}
		else {
			y = z->p->p->left;
			if (y->color=="RED"){
				z->p->color = "BLACK";
				y->color = "BLACK";
				z->p->p->color = "RED";
				z = z->p->p;
			}
			else{
				if (z==z->p->left) {
					z = z->p;
					RB_right_rotate(z);
				}
				z->p->color = "BLACK";
				z->p->p->color = "RED";
				RB_left_rotate(z->p->p);
			}
		}
	}
	root->color = "BLACK";
	return;
}

// Transplanting Node 'u' with Node 'v'
void RB_Tree::RB_transplant(TreeNode* u,TreeNode* v){
	if(u->p==NIL)
		this->root = v;
	else if (u==u->p->left)
		u->p->left = v;
	else
		u->p->right = v;
	v->p = u->p;
	return;
}

// Returns the left-most::minimum node in the right sub-tree of Node 'z'
TreeNode* RB_Tree::RB_minSub(TreeNode* z){
	TreeNode* y = z;
	while (y->left!=NIL) {
		y = y->left;
	}
	return y;
}

// Delete the Node having integer key 'n'
void RB_Tree::RB_delete(int n){
	TreeNode* y = RB_search(n),*x=NULL;
        TreeNode* z=y;
        if (y!=NIL || y!=NULL){
            string y_original_color = y->color;
            if (z->left==NIL){
                    x = z->right;
                    RB_transplant(z,z->right);
            }
            else if (z->right==NIL){
                    x = z->left;
                    RB_transplant(z,z->left);
            }
            else{
                    y = RB_minSub(z->right);
                    y_original_color = y->color;
                    x = y->right;
                    if (y->p==z)
                            x->p = y;
                    else{
                            RB_transplant(y,y->right);
                            y->right = z->right;
                            y->right->p = y;
                    }
                    RB_transplant(z,y);
                    y->left = z->left;
                    y->left->p = y;
                    y->color = z->color;
            }

            if (y_original_color=="BLACK")
                    RB_delete_fixup(x);
            cout<<"deleted"<<endl;
        }
        else
            cout<<"<.....incomplete deletion.....>\n";
	return;
}


/*
 *	Deletion fix-up to refrain the properties of RED-BLACK trees
 */
void RB_Tree::RB_delete_fixup(TreeNode* x){
	TreeNode* w=NULL;
	while (x!=root && x->color=="BLACK") {
		if (x==x->p->left){
			w = x->p->right;
			if (w->color=="RED") {
				w->color = "BLACK";
				x->p->color = "RED";
				RB_left_rotate(x->p);
				w = x->p->right;
			}
			if (w->left->color=="BLACK" && w->right->color=="BLACK")
				w->color = "RED" , x = x->p;
			else{
				if (w->right->color=="BLACK") {
					w->left->color = "BLACK";
					w->color = "RED";
					RB_right_rotate(w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = "BLACK";
				w->right->color = "BLACK";
				RB_left_rotate(x->p);
				x = root;
			}
		}
		else{
			if (x==x->p->right){
				w = x->p->left;
				if (w->color=="RED") {
					w->color = "BLACK";
					x->p->color = "RED";
					RB_right_rotate(x->p);
					w = x->p->left;
				}
				if (w->right->color=="BLACK" && w->left->color=="BLACK")
					w->color = "RED" , x = x->p;
				else{
					if (w->left->color=="BLACK") {
						w->right->color = "BLACK";
						w->color = "RED";
						RB_left_rotate(w);
						w = x->p->left;
					}
					w->color = x->p->color;
					x->p->color = "BLACK";
					w->left->color = "BLACK";
					RB_right_rotate(x->p);
					x = root;
				}
			}
		}
	}
	x->color = "BLACK";
}

// Search function for integer key 'n' in tree
TreeNode* RB_Tree::RB_search(int n){
	if (root==NIL){
	        cout<<"<.....Tree is empty.....>\n";
	        return NIL;
	}
	else {
		TreeNode* x = root;
		while (1) {
			if (x==NIL){
				cout<<"<.....Key not found error....>\n";
				return NULL;
			}
			else if (x->key==n){
                            cout<<"<.....key found.....>\n"<<" color = "<<x->color<<endl;
				return x;
			}
			else if (x->key > n)
				x = x->left;
			else if (x->key < n)
				x = x->right;
		}
	}
}

// returns leftmost::minimum key Node 
TreeNode* RB_Tree::RB_minimum(){
	TreeNode* x=root;
	if(x!=NIL){
		while (x->left!=NIL) {
			x = x->left;
		}
		cout<<"<.....MINIMUM FOUND.....>"<<endl;
		cout<<"\tMinimum = "<<x->key<<" color = "<<x->color;
		return x;
	}
	else{
		cout<<"<.....Tree is Empty.....>\n";
		return NIL;
	}
}
// returns rightmost::maximum key Node
TreeNode* RB_Tree::RB_maximum(){
	TreeNode* x=root;
	if(x!=NIL){
		while (x->right!=NIL) {
			x = x->right;
		}
		cout<<"<.....MAXIMUM FOUND.....>"<<endl;
		cout<<"\tMaximum = "<<x->key<<" color = "<<x->color;
		return x;
	}
	else{
		cout<<"<.....Tree is Empty.....>\n";
		return NIL;
	}
}

// returns the Node predecessor to Node having key 'n'
TreeNode* RB_Tree::RB_predecessor(int n) {
	TreeNode* x = RB_search(n);
	if(x->p!=NIL)
		cout<<"Predecessor = "<<x->p->key<<" color = "<<x->p->color<<endl;
	else
		cout<<"<.....Predecessor NOT found.....>"<<endl;
	return x->p;
}

// returns the Node left successing Node having key 'n'
TreeNode* RB_Tree::RB_left_successor(int n) {
	TreeNode* x = RB_search(n);
	if(x->left!=NIL)
		cout<<"left Successor = "<<x->left->key<<" color = "<<x->left->color<<endl;
	else
		cout<<"<.....left Successor NOT found.....>"<<endl;
	return x->left;
}

// returns the Node right successing Node having key 'n'
TreeNode* RB_Tree::RB_right_successor(int n) {
	TreeNode* x = RB_search(n);
	if(x->right!=NIL)
		cout<<"right Successor = "<<x->right->key<<" color = "<<x->right->color<<endl;
	else
		cout<<"<.....right Successor NOT found.....>"<<endl;
	return x->right;
}

// display the choice of operation on tree
void display_choice(){
	cout
                        <<"\n\t1 . Search key\n"
			<<"\t2 . Find Minimum\n"
			<<"\t3 . Find Maximum\n"
			<<"\t4 . Get Successor\n"
			<<"\t5 . Get Predecessor\n"
			<<"\t6 . Insert\n"
			<<"\t7 . Delete\n"
                        <<"\t8 . Terminate Program\n";
	cout<<"Enter your choice : ";
	return;
}

// MAIN
int main() {
	int ch=0;	// choice
	// initializing tree
	RB_Tree* tree=new RB_Tree();

	while(ch<8) {

        display_choice();
		cin>>ch;

		if(ch==1){
			cout<<"Enter the key\n";
			int n;
			cin>>n;
			tree->RB_search(n);
		}
		else if(ch==2)
			tree->RB_minimum();
		else if(ch==3)
			tree->RB_maximum();
		else if(ch==4){
            cout<<"Enter the key : ";
			int n;
			cin>>n;
			tree->RB_left_successor(n);
			tree->RB_right_successor(n);
		}
		else if(ch==5){
            cout<<"Enter the key : ";
			int n;
			cin>>n;
			tree->RB_predecessor(n);
		}
		else if(ch==6){
            cout<<"Enter the key : ";
			int n;
			cin>>n;
			TreeNode* x = new TreeNode(NULL,NULL,NULL);
			x->key = n;
			tree->RB_insert(x);
		}
		else if(ch==7){
            cout<<"Enter the key : ";
			int n;
			cin>>n;
			tree->RB_delete(n);
		}
	}
	return 0;
}


