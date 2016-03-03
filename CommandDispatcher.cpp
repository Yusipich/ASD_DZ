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
