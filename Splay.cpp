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
