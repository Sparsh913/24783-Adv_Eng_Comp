#include <stdlib.h>
#include <time.h>
#include <iostream>


template <class KeyType,class ValueType>
class BinaryTree
{
private:
	BinaryTree(const BinaryTree <KeyType,ValueType> &);
	BinaryTree <KeyType,ValueType> &operator=(BinaryTree <KeyType,ValueType> &);

	class Node
	{
	public:
		Node *left=nullptr,*right=nullptr,*up=nullptr;
		KeyType key;
		ValueType value;
	};

public:
	class NodeHandle
	{
	friend BinaryTree <KeyType,ValueType>;
	private:
		Node *ptr=nullptr;
	public:
		inline void Nullify(void)
		{
			ptr=nullptr;
		}
		inline bool IsNull(void) const
		{
			return ptr==nullptr;
		}
		inline bool IsNotNull(void) const
		{
			return ptr!=nullptr;
		}
		inline bool operator==(NodeHandle hd) const
		{
			return hd.ptr==ptr;
		}
		inline bool operator!=(NodeHandle hd) const
		{
			return hd.ptr!=ptr;
		}
		// The following two operators allow:
		// auto rootHd=tree.GetRoot();
		// if(rootHd==nullptr) ...
		inline bool operator==(std::nullptr_t) const
		{
			return ptr==nullptr;
		}
		inline bool operator!=(std::nullptr_t) const
		{
			return ptr!=nullptr;
		}
	};

private:
	Node *root=nullptr;

	static NodeHandle MakeHandle(Node *ptr)
	{
		NodeHandle hd;
		hd.ptr=ptr;
		return hd;
	}

	void DeleteNodes(Node *ptr)
	{
		if(nullptr!=ptr)
		{
			DeleteNodes(ptr->left);
			DeleteNodes(ptr->right);
			delete ptr;
		}
	}
public:
	BinaryTree(){}
	~BinaryTree()
	{
		CleanUp();
	}
	void CleanUp(void)
	{
		DeleteNodes(root);
		root=nullptr;
	}

	NodeHandle GetRoot(void) const
	{
		return MakeHandle(root);
	}

	NodeHandle Insert(const KeyType &key,const ValueType &value)
	{
		auto newNode=new Node;
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
				if(newNode->key<ptr->key)
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
		return MakeHandle(newNode);
	}

	void Delete(NodeHandle ndHd)
	{
		auto ptr=ndHd.ptr;
		if(nullptr==ptr)
		{
			return;
		}
		else if(nullptr==ptr->right && nullptr==ptr->left)  // Easiest case
		{
			if(nullptr==ptr->up)
			{
				root=nullptr;
			}
			else
			{
				if(ptr->up->right==ptr)
				{
					ptr->up->right=nullptr;
				}
				else
				{
					ptr->up->left=nullptr;
				}
			}
			delete ptr;
			return;
		}
		else if(nullptr==ptr->right || nullptr==ptr->left) // The second easiest case
		{
			auto subTree=(nullptr!=ptr->right ? ptr->right : ptr->left);

			if(nullptr==ptr->up)
			{
				root=subTree;
			}
			else
			{
				if(ptr->up->right==ptr)
				{
					ptr->up->right=subTree;
				}
				else
				{
					ptr->up->left=subTree;
				}
			}
			subTree->up=ptr->up;
			delete ptr;
			return;
		}
		else
		{
			// At this time I know both ptr->left & ptr->right are non-nullptr.
			// First find the right most of left.

			Node *RMOL=ptr->left; // I know non-nullptr.
			while(nullptr!=RMOL->right)
			{
				RMOL=RMOL->right;
			}


			// RMOL is Right Most of the Left Sub-tree.
			// RMOL has ony up to 1 sub-tree, which is left.
			// RMOL is guaranteed to have one-level up.
			// RMOL is NOT guaranteed to be the right of one-level up.

			// Detach RMOL from the tree.
			if(nullptr!=RMOL->left)
			{
				RMOL->left->up=RMOL->up;
			}
			if(RMOL->up->right==RMOL)
			{
				RMOL->up->right=RMOL->left;
			}
			else
			{
				RMOL->up->left=RMOL->left;
			}


			// Put RMOL in place for ptr.
			RMOL->up=ptr->up;
			if(nullptr==ptr->up)
			{
				root=RMOL;
			}
			else if(ptr->up->right==ptr)
			{
				ptr->up->right=RMOL;
			}
			else
			{
				ptr->up->left=RMOL;
			}

			RMOL->right=ptr->right;
			if(nullptr!=RMOL->right)
			{
				RMOL->right->up=RMOL;
			}

			RMOL->left=ptr->left;
			if(nullptr!=RMOL->left)
			{
				RMOL->left->up=RMOL;
			}
		}
	}

	void RotateLeft(NodeHandle ndHd)
	{
		auto A=ndHd.ptr;
		if(nullptr==A)
		{
			return;
		}

		auto B=A->right;
		if(nullptr==B)
		{
			return;
		}

		if(nullptr==A->up)
		{
			root=B;
		}
		else if(A->up->right==A)
		{
			A->up->right=B;
		}
		else
		{
			A->up->left=B;
		}
		B->up=A->up;


		auto C=B->left;
		A->right=C;
		if(nullptr!=C)
		{
			C->up=A;
		}


		B->left=A;
		A->up=B;
	}

	NodeHandle FindFirst(NodeHandle rootHd) const
	{
		auto root=rootHd.ptr;
		if(nullptr==root)
		{
			return rootHd;
		}
		while(nullptr!=root->left)
		{
			root=root->left;
		}
		return MakeHandle(root);
	}

	NodeHandle FindFirst(void) const
	{
		return FindFirst(GetRoot());
	}

	NodeHandle FindNext(NodeHandle hd) const
	{
		auto ptr=hd.ptr;
		if(nullptr==ptr)
		{
			return hd;
		}
		else if(nullptr!=ptr->right)
		{
			return FindFirst(MakeHandle(ptr->right));
		}
		else
		{
			while(nullptr!=ptr)
			{
				auto up=ptr->up;
				if(nullptr!=up && up->left==ptr)
				{
					hd.ptr=up;
					return hd;
				}
				else
				{
					ptr=ptr->up;
				}
			}
		}
		return MakeHandle(nullptr);
	}


	inline const KeyType &GetKey(NodeHandle hd) const
	{
		return hd.ptr->key;
	}
	inline const ValueType &GetValue(NodeHandle hd) const
	{
		return hd.ptr->key;
	}
	inline ValueType &GetValue(NodeHandle hd)
	{
		return hd.ptr->key;
	}
};


int main(void)
{
	srand(time(nullptr));

	BinaryTree <int,int> tree;

	for(int i=0; i<20; ++i)
	{
		auto r=rand()%20;
		tree.Insert(r,0);
	}

	for(auto hd=tree.FindFirst(); hd!=nullptr; hd=tree.FindNext(hd))
	{
		std::cout << tree.GetKey(hd) <<  " ";
	}
	std::cout << "\n";


	int ctr=0;
	for(auto hd=tree.FindFirst(); hd!=nullptr && ctr<20; )
	{
		auto nextHd=tree.FindNext(hd);
		if(0==tree.GetKey(hd)%2)
		{
			tree.Delete(hd);

			for(auto hd=tree.FindFirst(); hd!=nullptr; hd=tree.FindNext(hd))
			{
				std::cout << tree.GetKey(hd) <<  " ";
			}
			std::cout << "\n";
		}
		hd=nextHd;
		++ctr;
	}


	return 0;
}
