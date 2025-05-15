#include <stdlib.h>
#include <time.h>
#include <iostream>


class Node
{
public:
	Node *left=nullptr,*right=nullptr,*up=nullptr;
	int data;
};

void Print(Node *ptr)
{
	if(nullptr!=ptr)
	{
		Print(ptr->left);
		std::cout << ptr->data << "\n";
		Print(ptr->right);
	}
}

Node *FindFirst(Node *root)
{
	if(nullptr==root)
	{
		return nullptr;
	}
	while(nullptr!=root->left)
	{
		root=root->left;
	}
	return root;
}

Node *FindNext(Node *ptr)
{
	if(nullptr==ptr)
	{
		return nullptr;
	}
	else if(nullptr!=ptr->right)
	{
		return FindFirst(ptr->right);
	}
	else
	{
		while(nullptr!=ptr)
		{
			auto up=ptr->up;
			if(nullptr!=up && up->left==ptr)
			{
				return up;
			}
			else
			{
				ptr=ptr->up;
			}
		}
	}
	return nullptr;
}

int main(void)
{
	srand(time(nullptr));

	Node *root=nullptr;

	for(int i=0; i<20; ++i)
	{
		auto r=rand()%20;

		auto newNode=new Node;
		newNode->data=r;
		if(nullptr==root)
		{
			root=newNode;
		}
		else
		{
			Node *ptr=root;
			for(;;)
			{
				if(newNode->data<ptr->data)
				{
					if(nullptr!=ptr->left)
					{
						ptr=ptr->left;
					}
					else
					{
						ptr->left=newNode;
						newNode->up=ptr;
						break;
					}
				}
				else
				{
					if(nullptr!=ptr->right)
					{
						ptr=ptr->right;
					}
					else
					{
						ptr->right=newNode;
						newNode->up=ptr;
						break;
					}
				}
			}
		}
	}

	int ctr=0;
	// Print(root);
	for(auto ptr=FindFirst(root); nullptr!=ptr; ptr=FindNext(ptr))
	{
		std::cout << ctr << " " << ptr->data << "\n";
		++ctr;
	}

	return 0;
}
