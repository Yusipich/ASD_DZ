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
