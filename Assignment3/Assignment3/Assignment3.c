#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _node
{
	char data;
	struct _node *left;
	struct _node *right;
}Node;

Node *root = NULL;
int GetHeight(Node *node)
{
	int height = 0;
	if (node != NULL)
		height = 1 + max(GetHeight(node->left), GetHeight(node->right));

	return height;
}
Node* InsertNode(Node *node, int data)
{
	if (!node)
	{
		node = (Node*)malloc(sizeof(Node));
		node->data = data;
		node->left = node->right = NULL;
	}
	else if (data < node->data)
	{
		node->left = InsertNode(node->left, data);
		node = ReBalance(node);

	}
	else if (data > node->data)
	{
		node->right = InsertNode(node->right, data);
		node = ReBalance(node);
	}
	else
	{
		printf("중복");
		exit(1);
	}

	return node;
}

Node* rotateLL(Node *node)
{
	Node *child = node->left;
	node->left = child->right;
	child->right = node;
	return child;
}
Node* rotateRR(Node *node)
{
	Node *child = node->right;
	node->right = child->left;
	child->left = node;
	return child;
}
Node* rotateLR(Node *node)
{
	Node *child = node->left;
	node->left = rotateRR(child);
	return rotateLL(node);
}

Node* rotateRL(Node *node)
{
	Node *child = node->right;
	node->right = rotateLL(child);
	return rotateRR(node);
}
int GetHeight_Diff(Node *node)
{
	int leftHeight;
	int rightHeight;

	if (node == NULL)  return 0;
	else
	{
		leftHeight = GetHeight(node->left);
		rightHeight = GetHeight(node->right);

		return leftHeight - rightHeight;
	}
}
Node* ReBalance(Node *node)
{
	int heightDiff = GetHeight_Diff(node);
	if (heightDiff >= 2)
	{
		if (GetHeight_Diff(node->left) >= 1)
		{
			printf("[LL회전]\n");
			node = rotateLL(node);
		}
		else
		{
			printf("[LR회전]\n");
			node = rotateLR(node);
		}
	}

	else if (heightDiff <= -2)
	{
		if (GetHeight_Diff(node->right) <= -1)
		{
			printf("[RR회전]\n");
			node = rotateRR(node);
		}
		else
		{
			printf("[RL회전]\n");
			node = rotateRL(node);
		}
	}

	return node;
}

void tree_print(Node *root)   //전위 순회로 구현
{
	if (root != NULL) {
		printf("%d ", root->data);
		tree_print(root->left);
		tree_print(root->right);
	}
}

int main()
{
	root = InsertNode(root, "DECEMBER");
	root = InsertNode(root, "JANUARY");
	root = InsertNode(root, "APRIL");
	root = InsertNode(root, "MARCH");
	root = InsertNode(root, "JULY");
	root = InsertNode(root, "AUGUST");
	root = InsertNode(root, "OCTOBER");
	root = InsertNode(root, "FEBRUARY");
	root = InsertNode(root, "NOVEMBER");
	root = InsertNode(root, "MAY");
	root = InsertNode(root, "JUNE");


	printf("\n# Prefix 순회 #\n");
	tree_print(root);


	return 0;
}
