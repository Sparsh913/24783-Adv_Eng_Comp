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
	bool autoRebalancing = false;
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

		// Perform AVL rebalancing if enabled
		if (autoRebalancing)
		{
			Rebalance(PtrToHandle(newNode));
		}
		return PtrToHandle(newNode);
	}

	void Delete(NodeHandle hd)
	{
		auto toDel=hd.ptr;
		Node* startRebalance = nullptr;

		// Easiest case
		if(nullptr==toDel->left && nullptr==toDel->right)
		{
			if(nullptr!=toDel->up)
			{
				startRebalance = toDel->up;
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
				startRebalance = toDel->up;
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
			startRebalance = toDel->left;
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
			startRebalance = RMOL->up;

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

		// Perform AVL rebalancing if enabled
		if (autoRebalancing && startRebalance != nullptr)
		{
			Rebalance(PtrToHandle(startRebalance));
		}
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

	void RotateRight(NodeHandle hd)
	{
		auto A = hd.ptr;
		if (nullptr == A)
		{
			return;
		}

		auto B = A->left;
		if (nullptr == B)
		{
			return;
		}

		if (nullptr != A->up)
		{
			if (A->up->left == A)
			{
				A->up->left = B;
			}
			else
			{
				A->up->right = B;
			}
		}
		else
		{
			root = B;
		}
		B->up = A->up;

		A->left = B->right;
		if (B->right != nullptr)
		{
			B->right->up = A;
		}

		B->right = A;
		A->up = B;

		UpdateHeightCascade(A);
		UpdateHeightCascade(B);
	}

	public:
		// Tree balancing functions
		void TreeToVine();
		void VineToTree();
		void Rebalance(NodeHandle ndHd);

	private:
		// Helper functions for tree balancing
		void Compression(size_t count);
		size_t log2(size_t n);

};

template <class Key, class Value>
void BinaryTree<Key, Value>::TreeToVine()
{
    // Create a pseudo-root to simplify the algorithm
    NodeHandle pseudoRoot = Root();
    if (pseudoRoot.IsNull())
        return;
    
    NodeHandle vineTail = pseudoRoot;
    NodeHandle remainder = RightOf(vineTail);
    
    while (remainder.IsNotNull())
    {
        if (LeftOf(remainder).IsNull())
        {
            // Move vineTail down one
            vineTail = remainder;
            remainder = RightOf(remainder);
        }
        else
        {
            // Perform a rotation
            NodeHandle tempNode = LeftOf(remainder);
            RotateRight(remainder);
            remainder = tempNode;
        }
    }
}

template <class Key, class Value>
void BinaryTree<Key, Value>::VineToTree()
{
    size_t n = len;
    if (n <= 1)
        return;
    
    // Calculate the number of nodes in the deepest level of the full tree
    size_t leafCount = n + 1 - (1 << static_cast<size_t>(log2(n + 1)));
    
    // Perform the initial compression to create leaves
    Compression(leafCount);
    n = n - leafCount;
    
    // Perform compressions until the tree is balanced
    while (n > 1)
    {
        Compression(n / 2);
        n = n / 2;
    }
}

template <class Key, class Value>
void BinaryTree<Key, Value>::Compression(size_t count)
{
    NodeHandle scanner = Root();
    
    for (size_t i = 0; i < count; ++i)
    {
        NodeHandle child = RightOf(scanner);
        if (child.IsNull())
            break;
        
        // Perform the rotation to compress the vine
        RotateLeft(scanner);
        scanner = RightOf(scanner);
    }
}

// Helper method to calculate log2
template <class Key, class Value>
size_t BinaryTree<Key, Value>::log2(size_t n)
{
    size_t result = 0;
    while (n > 1)
    {
        n >>= 1;
        ++result;
    }
    return result;
}

template <class Key, class Value>
void BinaryTree<Key, Value>::Rebalance(NodeHandle ndHd)
{
    Node* node = ndHd.ptr;
    
    while (node != nullptr)
    {
        // Update height (already done in UpdateHeightCascade)
        
        // Calculate balance factor
        int balanceFactor = 0;
        if (node->right != nullptr)
            balanceFactor += node->right->height;
        if (node->left != nullptr)
            balanceFactor -= node->left->height;
        
        // Check if node is unbalanced (balance factor > 1 or < -1)
        if (balanceFactor > 1) // Right subtree is heavier
        {
            // Check if we need a Right-Left rotation
            int rightChildBalanceFactor = 0;
            if (node->right->right != nullptr)
                rightChildBalanceFactor += node->right->right->height;
            if (node->right->left != nullptr)
                rightChildBalanceFactor -= node->right->left->height;
            
            if (rightChildBalanceFactor < 0) // Right-Left case
            {
                // Right rotation on right child
                RotateRight(PtrToHandle(node->right));
            }
            
            // Left rotation on current node
            RotateLeft(PtrToHandle(node));
        }
        else if (balanceFactor < -1) // Left subtree is heavier
        {
            // Check if we need a Left-Right rotation
            int leftChildBalanceFactor = 0;
            if (node->left->right != nullptr)
                leftChildBalanceFactor += node->left->right->height;
            if (node->left->left != nullptr)
                leftChildBalanceFactor -= node->left->left->height;
            
            if (leftChildBalanceFactor > 0) // Left-Right case
            {
                // Left rotation on left child
                RotateLeft(PtrToHandle(node->left));
            }
            
            // Right rotation on current node
            RotateRight(PtrToHandle(node));
        }
        
        // Move up to parent
        node = node->up;
    }
}