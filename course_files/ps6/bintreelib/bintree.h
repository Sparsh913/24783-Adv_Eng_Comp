#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> // #include <cstdlib> ->  std::srand(std::time(NULL));
#include <time.h>



template <class Key,class Value>
class BinaryTree
{
private:
	class Node
	{
	friend class BinaryTree <Key,Value>;

	private:
		Node *up=nullptr,*left=nullptr,*right=nullptr;
	public:
		Key key;
		Value value;
		int height=1;
	};

public:
	class NodeHandle
	{
	friend class BinaryTree <Key,Value>;
	private:
		Node *ptr=nullptr;
	public:
		bool IsNotNull(void) const
		{
			return nullptr!=ptr;
		}
		bool IsNull(void) const
		{
			return nullptr==ptr;
		}
		bool operator==(NodeHandle incoming) const
		{
			return incoming.ptr==ptr;
		}
		bool operator!=(NodeHandle incoming) const
		{
			return incoming.ptr!=ptr;
		}
	};

private:
	Node *root=nullptr;
	size_t len=0;

public:
	inline BinaryTree(){}
	~BinaryTree()
	{
		CleanUp();
	}
	void CleanUp(void)
	{
		Delete(root);
		root=nullptr;
		len=0;
	}
private:
	void Delete(Node *ptr)
	{
		if(nullptr!=ptr)
		{
			Delete(ptr->right);
			Delete(ptr->left);
			delete ptr;
		}
	}

	NodeHandle PtrToHandle(Node *ptr) const
	{
		NodeHandle hd;
		hd.ptr=ptr;
		return hd;
	}

public:
	NodeHandle Root(void) const
	{
		return PtrToHandle(root);
	}
	NodeHandle LeftOf(NodeHandle hd) const
	{
		if(nullptr==hd.ptr)
		{
			return PtrToHandle(nullptr);
		}
		return PtrToHandle(hd.ptr->left);
	}
	NodeHandle RightOf(NodeHandle hd) const
	{
		if(nullptr==hd.ptr)
		{
			return PtrToHandle(nullptr);
		}
		return PtrToHandle(hd.ptr->right);
	}

	NodeHandle LeftMostOf(NodeHandle hd) const
	{
		if(nullptr==hd.ptr)
		{
			return PtrToHandle(nullptr);
		}
		while(nullptr!=hd.ptr->left)
		{
			hd.ptr=hd.ptr->left;
		}
		return hd;
	}

private:
	Node *LeftMostOf(Node *ptr) const
	{
		if(nullptr==ptr)
		{
			return nullptr;
		}
		while(nullptr!=ptr->left)
		{
			ptr=ptr->left;
		}
		return ptr;
	}
	Node *RightMostOf(Node *ptr) const
	{
		if(nullptr==ptr)
		{
			return nullptr;
		}
		while(nullptr!=ptr->right)
		{
			ptr=ptr->right;
		}
		return ptr;
	}

public:
	NodeHandle FindFirst(void) const
	{
		return PtrToHandle(LeftMostOf(root));
	}
	NodeHandle FindNext(NodeHandle hd) const
	{
		if(nullptr==hd.ptr)
		{
			return hd;
		}

		auto ptr=hd.ptr;
		if(nullptr!=ptr->right)
		{
			return PtrToHandle(LeftMostOf(ptr->right));
		}
		else
		{
			for(;;)
			{
				auto up=ptr->up;
				if(nullptr==up)
				{
					return PtrToHandle(nullptr);
				}
				else if(up->left==ptr)
				{
					return PtrToHandle(up);
				}
				else
				{
					ptr=ptr->up;
				}
			}
		}
		return PtrToHandle(nullptr);
	}

private:
	void UpdateHeightCascade(Node *ptr)
	{
		bool first=true;
		while(nullptr!=ptr)
		{
			auto hL=(nullptr!=ptr->left ? ptr->left->height : 0);
			auto hR=(nullptr!=ptr->right ? ptr->right->height : 0);

			auto newHeight=std::max(hL,hR)+1;
			if(newHeight==ptr->height && true!=first)
			{
				break;
			}
			ptr->height=newHeight;
			first=false;

			ptr=ptr->up;
		}
	}

public:
	NodeHandle Insert(const Key &key,const Value &value)
	{
		auto *newNode=new Node;
		newNode->key=key;
		newNode->value=value;

		if(nullptr==root)
		{
			root=newNode;
		}
		else
		{
			auto *ptr=root;
			for(;;)
			{
				if(ptr->key<=newNode->key)
				{
					if(nullptr==ptr->right)
					{
						ptr->right=newNode;
						newNode->up=ptr;
						break;
					}
					else
					{
						ptr=ptr->right;
					}
				}
				else
				{
					if(nullptr==ptr->left)
					{
						ptr->left=newNode;
						newNode->up=ptr;
						break;
					}
					else
					{
						ptr=ptr->left;
					}
				}
			}
		}
		UpdateHeightCascade(newNode);
		++len;
		return PtrToHandle(newNode);
	}

	void Delete(NodeHandle hd)
	{
		auto toDel=hd.ptr;

		// Easiest case
		if(nullptr==toDel->left && nullptr==toDel->right)
		{
			if(nullptr!=toDel->up)
			{
				if(toDel->up->right==toDel)
				{
					toDel->up->right=nullptr;
				}
				else
				{
					toDel->up->left=nullptr;
				}
				UpdateHeightCascade(toDel->up);
			}
			else
			{
				CleanUp();
				return;
			}
		}
		else if(nullptr==toDel->left || nullptr==toDel->right)
		{
			Node *below=nullptr;
			if(nullptr!=toDel->left)
			{
				below=toDel->left;
			}
			else
			{
				below=toDel->right;
			}

			if(nullptr!=toDel->up)
			{
				if(toDel->up->right==toDel)
				{
					toDel->up->right=below;
				}
				else
				{
					toDel->up->left=below;
				}
			}
			else // if(nullptr==toDel->up)
			{
				root=below;
			}
			below->up=toDel->up;
			UpdateHeightCascade(below->up);
		}
		else if(nullptr==toDel->left->right)
		{
			toDel->left->right=toDel->right;
			toDel->right->up=toDel->left;

			if(toDel->up!=nullptr)
			{
				if(toDel->up->right==toDel)
				{
					toDel->up->right=toDel->left;
				}
				else
				{
					toDel->up->left=toDel->left;
				}
			}
			else
			{
				root=toDel->left;
			}
			toDel->left->up=toDel->up;
			UpdateHeightCascade(toDel->left);
		}
		else
		{
			auto RMOL=RightMostOf(toDel->left);
			auto startUpdate=RMOL->up;

			RMOL->up->right=RMOL->left;
			if(nullptr!=RMOL->left)
			{
				RMOL->left->up=RMOL->up;
			}

			RMOL->left=toDel->left;
			RMOL->left->up=RMOL;
			RMOL->right=toDel->right;
			RMOL->right->up=RMOL;

			if(toDel->up!=nullptr)
			{
				if(toDel->up->right==toDel)
				{
					toDel->up->right=RMOL;
				}
				else
				{
					toDel->up->left=RMOL;
				}
			}
			else
			{
				root=RMOL;
			}
			RMOL->up=toDel->up;
			UpdateHeightCascade(startUpdate);
			UpdateHeightCascade(RMOL);  // Sorry, I forgot this in class.  There is a possibility that the update from startUpdate may stop before reaching RMOL.
		}
		delete toDel;
	}

	const Key &GetKey(NodeHandle hd) const
	{
		return hd.ptr->key;
	}

	int GetHeight(NodeHandle hd) const
	{
		return hd.ptr->height;
	}

	Value &GetValue(NodeHandle hd)
	{
		return hd.ptr->value;
	}
	const Value &GetValue(NodeHandle hd) const
	{
		return hd.ptr->value;
	}

	void RotateLeft(NodeHandle hd)
	{
		auto A=hd.ptr;
		if(nullptr==A)
		{
			return;
		}

		auto B=A->right;
		if(nullptr==B)
		{
			return;
		}

		if(nullptr!=A->up)
		{
			if(A->up->right==A)
			{
				A->up->right=B;
			}
			else
			{
				A->up->left=B;
			}
		}
		else
		{
			root=B;
		}
		B->up=A->up;

		A->right=B->left;
		if(B->left!=nullptr)
		{
			B->left->up=A;
		}

		B->left=A;
		A->up=B;

		UpdateHeightCascade(A);
		UpdateHeightCascade(B);
	}
};
