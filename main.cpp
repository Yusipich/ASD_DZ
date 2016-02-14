
#include <iostream>
#include <fstream>

using namespace std;

class TreeNode
{
	friend class Tree;
public:
	TreeNode(const int &);
	int get_data();
protected:
	TreeNode* left;
	TreeNode* right;
	TreeNode* parent;
	int data;
	int age;
};

TreeNode::TreeNode(const int &a)
{
	data = a;
	left = right = NULL;
}

int TreeNode::get_data()
{
	return data;
}

class Tree
{
public:
	Tree();
	int insert_node(const int &);
	TreeNode delete_node(TreeNode*);
	int preorder_walk(TreeNode*, int *m, int i);
	TreeNode* extract_old(TreeNode* x);
	TreeNode* find_min(TreeNode*);
	TreeNode* find_node(TreeNode*, const int &);
	TreeNode* find_successor(const int &);
	TreeNode *get_root();
	void p_age(TreeNode*);
	void inorder_age(TreeNode*);
	int maxsize;
	int size;
private:
	TreeNode *root;
};

Tree::Tree()
{
	root = NULL;
}

int Tree::insert_node(const int &x)  
{
	TreeNode* n = new TreeNode(x);
	TreeNode* ptr;
	TreeNode* ptr1;

	n->parent = n->left = n->right = 0;
	n->age = 0;
	if (root == NULL)
	{
		root = n;
		n->age = 1;
		return 0;
	}

	ptr = root;
	while (ptr != 0)
	{
		ptr1 = ptr;
		if (x == ptr->get_data())
		{
			ptr->age = 0;
			p_age(root);
			return 0;
		}
		else if (x < ptr->get_data())
			ptr = ptr->left;
		else
			ptr = ptr->right;
	}
	n->parent = ptr1;
	if (x < ptr1->get_data())
		ptr1->left = n;
	else
		ptr1->right = n;
	if (size >= maxsize)
	{
		delete_node(extract_old(root));
	}
	size++;
	p_age(root);
	return 0;
}

TreeNode Tree::delete_node(TreeNode *z) 
{
	TreeNode* y;
	TreeNode* x;
	z->age = 0;
	if (z->left == 0 || z->right == 0)
		y = z;
	else
		y = find_successor(z->get_data());
	if (y->left != 0)
		x = y->left;
	else
		x = y->right;
	if (x != 0)
		x->parent = y->parent;

	if (y->parent == 0)
		root = x;
	else
	{
		if (y == (y->parent)->left)
			(y->parent)->left = x;
		else
			(y->parent)->right = x;
	}
	if (y != z)
		z->data = y->get_data();
	size--;
	return *y;
}

TreeNode* Tree::extract_old(TreeNode* x)
{
	TreeNode *h1, *h2;
	h1 = h2 = x;
	if (x->right != 0)
	{
		h1 = x->right;
		extract_old(h1);

	}
	if (x->left != 0)
	{
		h2 = x->left;
		extract_old(h2);
	}
	if (h1->age >= x->age)
		if (h2->age >= h1->age)
			return h2;
		else
			return h1;
	else if (h2->age >= x->age)
		return h2;
	else
		return x;
}

TreeNode* Tree::find_min(TreeNode* x)
{
	while (x->left != 0)
		x = x->left;
	return x;
}

TreeNode* Tree::find_successor(const int & val) 
{
	TreeNode* x = find_node(root, val);
	TreeNode* y;
	if (x == 0)
		return 0;
	if (x->right != 0)
		return find_min(x->right);
	y = x->parent;
	while (y != 0 && x == y->right)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

TreeNode* Tree::find_node(TreeNode* n, const int & val)
{
	if (n == 0 || val == n->get_data())
		return n;
	if (val > n->get_data())
		return find_node(n->right, val);
	else
		return find_node(n->left, val);
}

int Tree::preorder_walk(TreeNode* n, int *m, int i)
{
	if (n != 0)
	{
		m[i] = n->get_data();
		cout << n->get_data() << ' ';
		i = preorder_walk(n->left, m, i + 1);
		i = preorder_walk(n->right, m, i + 1);
		return i;
	}
	return i - 1;
}


TreeNode* Tree::get_root()
{
	return root;
}

void Tree::p_age(TreeNode* n)
{
	if (n != 0)
	{
		p_age(n->left);
		p_age(n->right);
		n->age++;
	}
}

int main(int argc, char *argv[])
{
	
	if (argc < 3) {
		cout << "\nUsage: tree.exe <input file> <output file>\n";
		return 1;
	}
	
	Tree cache;
	fstream F;
	ofstream O;
	int a = 0;

	cache.size = 1;
	
	F.open(argv[1], ios::in); 
	if (!F.is_open()) {
		cout << "Error opening file \"" << argv[1] << "\"";
		return 2;
	}
	
	F >> cache.maxsize;

	while (!F.eof())
	{
		F >> a;
		cache.insert_node(a);
	}

	F.close();
	O.open(argv[2]); 
	if (!O.is_open()) {
		cout << "Error opening file \"" << argv[2] << "\"";
		return 2;
	}

	int *mas = new int[cache.size];
	cache.preorder_walk(cache.get_root(), mas, 0);
	for (int i = 0; i < cache.size; i++)
		O << mas[i] << ' ';

	O.close();
	delete mas;
	
	cout << "\n\n";
	system("pause");
	
	return 0;
}
