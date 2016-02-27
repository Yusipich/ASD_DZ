		CommandDispatcher.cpp
		

#include "CommandDispatcher.h"

using namespace std;

CommandDispatcher::CommandDispatcher(SplayTree* splay, ofstream * f) {
	this->tree = splay;
	this->out = f;
	this->log.open("splay.log");
}


CommandDispatcher::~CommandDispatcher() {
	this->log.close();
}

void CommandDispatcher::execute(string cmd) {
	if (cmd.empty()) {
		return;
	}

	string command;
	string params;

	int separatorPos = cmd.find(" ");
	if (separatorPos != string::npos) {
		command = cmd.substr(0, separatorPos);
		params = cmd.substr(cmd.find(" ") + 1);
	} else {
		command = cmd;
		params = "";
	}
	
	log << "command: \"" << command.c_str() << "\"; params: \"" << params << "\"\n";

	if (command == "maxSize") {
		this->maxSize(params);
	} else if (command == "add") {
		this->add(params);
	} else if (command == "del") {
		this->del(params);
	} else if (command == "min") {
		this->min(params);
	} else if (command == "max") {
		this->max(params);
	} else if (command == "find") {
		this->find(params);
	} else if (command == "print") {
		this->print(params);
	}  else {
		log << " - Unknown command\n";
	}
	
}

void CommandDispatcher::maxSize(string params) {
	this->tree->setMaxSize(stoi(params));
	log << " - set\n";
};

void CommandDispatcher::add(string params) {
	string key;
	int separatorPos;
	do {
		separatorPos = params.find(" ");
		if (separatorPos != string::npos) {
			key = params.substr(0, separatorPos);
			params = params.substr(separatorPos + 1);
		} else {
			key = params;
			params = "";
		}

		this->tree->insert(stoi(key));

		log << " - added: " << key << "\n";
	} while (!params.empty());
	
};

void CommandDispatcher::del(string params) {
	string key;
	int separatorPos;
	do {
		separatorPos = params.find(" ");
		if (separatorPos != string::npos) {
			key = params.substr(0, separatorPos);
			params = params.substr(separatorPos + 1);
		} else {
			key = params;
			params = "";
		}

		this->tree->remove(stoi(key));

		log << " - removed: " << key << "\n";
	} while (!params.empty());
};

void CommandDispatcher::find(string params) {
	int depth = this->tree->find(stoi(params));

	log << " -";
	if (depth == -1) {
		log << " not";
	}
	
	log << " found. depth: " << depth << "\n";
};

void CommandDispatcher::min(string params) {
	log << " - min: " << this->tree->min() << "\n";
};

void CommandDispatcher::max(string params) {
	log << " - min: " << this->tree->max() << "\n";
};

int CommandDispatcher::realPrint(TreeNode* node, int indent, bool isRight) {
	int newIndent;
	string idt(indent, ' ');

	*out << (isRight ? "" : idt) << node->key << " - ";
	
	idt = string(indent + to_string(node->key).size() + 3, ' ');
	
	if (node->right) {
		newIndent = this->realPrint(node->right, idt.size(), true);
	} else {
		*out << "NULL";
	}
	*out << "\n";
	
	if (node->left) {
		newIndent = this->realPrint(node->left, idt.size(), false);
	} else {
		*out << idt << "NULL";
	}
	
	return 0;
}

void CommandDispatcher::print(string params) {
	*out << "---- ----\n";
	realPrint(this->tree->getRoot(), 0, true);
	*out << "\n";
	log << " - printed\n";
};

		splay.cpp

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "SplayTree.h"
#include "CommandDispatcher.h"

int main(int argc, char *argv[])
{

	if (argc < 3) {
		cout << "\nUsage: tree.exe <input file> <output file>\n";
		return 1;
	}

	SplayTree* cache;
	CommandDispatcher* dispatcher;
	
	ifstream F;
	ofstream O;

	F.open(argv[1], ios::in); // Входные данные
	if (!F.is_open()) {
		cout << "Error opening file \"" << argv[1] << "\"";
		return 2;
	}

	O.open(argv[2]); // Файл результатов
	if (!O.is_open()) {
		cout << "Error opening file \"" << argv[2] << "\"";
		return 2;
	}

	cache = new SplayTree();
	dispatcher = new CommandDispatcher(cache, &O);

	string cmd;
	while (!F.eof())
	{
		getline(F, cmd);
		dispatcher->execute(cmd);
	}

	F.close();
	O.close();

	system("pause");

	return 0;
}

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

		TreeNode.cpp
		
#include "TreeNode.h"

TreeNode::TreeNode(TreeNode* parent)
{
	left = right = NULL;
	this->parent = parent;

	key = 0;
}


TreeNode::~TreeNode()
{
	if (left)
		delete left;
	
	if (right)
		delete right;
}
