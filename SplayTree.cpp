
		SplayTree.cpp
		
#include "SplayTree.h"

SplayTree::SplayTree() {
	root = NULL;
	size = 0;
}


SplayTree::~SplayTree() {
	delete root;
}

TreeNode* SplayTree::getRoot() {
	return
		this->root;
}

void SplayTree::setMaxSize(int newMaxSize) {
	maxSize = newMaxSize;

	while (maxSize < size) {
		removeLeastUsed();
	}
}

void SplayTree::removeLeastUsed() {

}

void SplayTree::insert(int key) {
	
	if (size == maxSize) {
		removeLeastUsed();
	}
	
	TreeNode *temp = root;
	TreeNode *p = 0;

	while (temp) {
		p = temp;

		if (temp->key < key) {
			temp = temp->right;
		} else {
			temp = temp->left;
		}
	}

	temp = new TreeNode(p);
	temp->key = key;

	if (!p) {
		root = temp;
	} else if (p->key < temp->key) {
		p->right = temp;
	} else {
		p->left = temp;
	}

	splay(temp);

	size++;
}

void SplayTree::remove(int key) {
	TreeNode* temp = search(key);

	splay(temp);

	TreeNode* right = temp->right;
	root = temp->left;
	root->parent = NULL;

	temp->left = NULL;
	temp->right = NULL;
	delete temp;

	TreeNode* tmp = root;
	while (tmp->right) {
		tmp = tmp->right;
	}

	splay(tmp);

	tmp->right = right;
	right->parent = tmp;

}

int SplayTree::find(int key) {
	TreeNode *temp = root;
	TreeNode* last;
	int depth = 0;

	while (temp) {
		last = temp;
		if (temp->key < key) {
			temp = temp->right;
		} else if (key < temp->key) {
			temp = temp->left;
		} else {
			splay(temp);
			return depth;
		}
		depth++;
	}
	
	splay(last);
	return -1;
}

TreeNode* SplayTree::search(int key) {
	TreeNode *temp = root;

	while (temp) {
		if (temp->key < key) {
			temp = temp->right;
		} else if (key < temp->key) {
			temp = temp->left;
		} else {
			return temp;
		}
	}

	return NULL;
}

int SplayTree::min() {
	TreeNode* tmp = root;

	while (tmp->left) {
		tmp = tmp->left;
	}

	if (tmp) {
		splay(tmp);

		return tmp->key;
	}

	return -1;
}

int SplayTree::max() {
	TreeNode* tmp = root;

	while (tmp->right) {
		tmp = tmp->right;
	}

	if (tmp) {
		splay(tmp);

		return tmp->key;
	}

	return -1;
}

void SplayTree::leftRotate(TreeNode *node) {
	TreeNode *temp = node->right;
	if (temp) {
		node->right = temp->left;
		if (temp->left) {
			temp->left->parent = node;
		}

		temp->parent = node->parent;
	}

	if (!node->parent) {
		root = temp;
	} else if (node == node->parent->left) {
		node->parent->left = temp;
	} else {
		node->parent->right = temp;
	}

	if (temp) {
		temp->left = node;
	}

	node->parent = temp;
}

void SplayTree::rightRotate(TreeNode *node) {
	TreeNode *temp = node->left;
	if (temp) {
		node->left = temp->right;
		if (temp->right) temp->right->parent = node;
		temp->parent = node->parent;
	}

	if (!node->parent) {
		root = temp;
	} else if (node == node->parent->left) {
		node->parent->left = temp;
	} else {
		node->parent->right = temp;
	}

	if (temp) {
		temp->right = node;
	}

	node->parent = temp;
}

void SplayTree::zig(TreeNode* node) {
	if (node->parent->left == node) {
		rightRotate(node->parent);
	} else {
		leftRotate(node->parent);
	}
}

void SplayTree::zigZig(TreeNode* node, int mode) {
	if (mode == ZIG_LEFT) {
		rightRotate(node->parent->parent);
		rightRotate(node->parent);
	} else {
		leftRotate(node->parent->parent);
		leftRotate(node->parent);
	}
}

void SplayTree::zigZag(TreeNode* node, int mode) {
	if (mode == ZIG_LEFT) {
		rightRotate(node->parent);
		leftRotate(node->parent);
	} else {
		leftRotate(node->parent);
		rightRotate(node->parent);
	}
}

void SplayTree::splay(TreeNode* node) {
	while (node != this->root) {

		if (node->parent == this->root) {
			zig(node);
		} else {

			if ((node == node->parent->left)
				&& (node->parent == node->parent->parent->left)) { // оба левые дети

				zigZig(node, ZIG_LEFT);
			} else if ((node == node->parent->right)
				&& (node->parent == node->parent->parent->right)) { //  оба правые дети then

				zigZig(node, ZIG_RIGHT);
			} else if ((node == node->parent->left)
				&& (node->parent == node->parent->parent->right)) { // node — левый ребенок, а node->parent — правый ребенок then

				zigZag(node, ZIG_LEFT);
			} else if ((node == node->parent->right)
				&& (node->parent == node->parent->parent->left)) { // node — правый ребенок, а node->parent — левый ребенок then

				zigZag(node, ZIG_RIGHT);
			}
		}
	}
}
