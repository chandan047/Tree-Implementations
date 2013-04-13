/*
 * main.cpp
 *
 *  Created on: 01/03/2013
 *      Author: Chandan Akiti
 */

#include <iostream>
#include <cstdio>
#include <stack>
using namespace std;


class TreeNode{
public:
    int data;
	bool visited;
	TreeNode *left; // left child and right child pointer 
	TreeNode *right;
public:
	TreeNode();
};

TreeNode::TreeNode(){
	data = 0;
	visited = false;
	left = NULL;
	right = NULL;
}

// adt of binary tree
class BinaryTree{
public:
	TreeNode* root;
public:
	void in_order_traversal();
	void visit_refresh();
	void post_order_traversal();
	void pre_order_traversal();
	void construct_BST();
};

void BinaryTree::in_order_traversal() {
	stack<TreeNode*> s;
	s.push(root);
	while (!s.empty()) {
		TreeNode *top = s.top();
		if (top != NULL) {
			if (!top->visited) {
				s.push(top->left);
			}
			else {
				cout << top->data << " ";
				s.pop();
				s.push(top->right);
			}
		}
		else {
			s.pop();
			if (!s.empty())
				s.top()->visited = true;
		}
	}
	cout<<'\n';
}

void BinaryTree::visit_refresh() {
	if (!root) return;
	stack<TreeNode*> s;
	s.push(root);
	while (!s.empty()) {
		TreeNode *curr = s.top();
		curr->visited = false;
		s.pop();
		if (curr->right!=NULL){
			s.push(curr->right);
		}
		if (curr->left!=NULL){
			s.push(curr->left);
		}
	}
}

void BinaryTree::post_order_traversal() {
	if (!root) return;
	stack<TreeNode*> s;
	s.push(root);
	TreeNode *prev = NULL;
	while ( !s.empty() ) {
		TreeNode *curr = s.top();
	    if ( !prev || prev->left == curr || prev->right == curr) {
	    	if (curr->left)
	    		s.push(curr->left);
	    	else if (curr->right)
	    		s.push(curr->right);
	    }
	    else if (curr->left == prev) {
	    	if (curr->right)
	    		s.push(curr->right);
	    }
	    else {
	    	cout << curr->data << " ";
	    	s.pop();
	    }
	    prev = curr;
	}
	cout<<'\n';
}

void BinaryTree::pre_order_traversal(){
	if (!root) return;
	stack<TreeNode*> s;
	s.push(root);
	while (!s.empty()) {
		TreeNode *curr = s.top();
		cout<<curr->data<<" ";
		s.pop();
		if (curr->right!=NULL){
			s.push(curr->right);
		}
		if (curr->left!=NULL){
			s.push(curr->left);
		}
	}
	cout<<'\n';
}

void BinaryTree::construct_BST(){
	TreeNode *node=root,*prevnode;
	cout<<"Number of data(>0) for BST : \n";
	int i,_data;
	cin>>i;
	cout<<"Enter the data:/n/t";
	cin>>_data;
	root = new TreeNode();
	root->data = _data;
	i--;
	node = root;

	while (i-->0){
		cin>>_data;
		while (node!=NULL){
			prevnode = node;
			if (node->data<_data)
				node = node->right;
			else
				node = node->left;
		}
		node = prevnode;
		if (node->data<_data){
			node->right = new TreeNode();
			node = node->right;
			node->data = _data;
		}
		else {
			node->left = new TreeNode();
			node = node->left;
			node->data = _data;
		}
		node = root;
	}
	return;
}


int main() {
	BinaryTree B;
	B.construct_BST();
	B.in_order_traversal();
	B.visit_refresh();
	B.post_order_traversal();
	B.pre_order_traversal();
	return 0;
}
