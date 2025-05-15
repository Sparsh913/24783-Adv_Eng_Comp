#include <iostream>
#include "fssimplewindow.h"
#include "ysglfontdata.h"


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
		int height=1;
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

	bool UpdateHeight(Node *nodePtr)
	{
		int L=(nullptr!=nodePtr->left ? nodePtr->left->height : 0);
		int R=(nullptr!=nodePtr->right ? nodePtr->right->height : 0);
		int newHeight=std::max(L,R)+1;
		if(newHeight!=nodePtr->height)
		{
			nodePtr->height=newHeight;
			return true;
		}
		return false;
	}
	void UpdateHeightCascade(Node *nodePtr)
	{
		while(nullptr!=nodePtr)
		{
			if(true!=UpdateHeight(nodePtr))
			{
				break;
			}
			nodePtr=nodePtr->up;
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
	NodeHandle GetLeft(NodeHandle ndHd) const
	{
		return MakeHandle(ndHd.ptr->left);
	}
	NodeHandle GetRight(NodeHandle ndHd) const
	{
		return MakeHandle(ndHd.ptr->right);
	}

	int GetHeight(NodeHandle ndHd) const
	{
		if(nullptr!=ndHd.ptr)
		{
			return ndHd.ptr->height;
		}
		return 0;
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
		UpdateHeightCascade(newNode->up);
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
			UpdateHeightCascade(ptr->up);
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
			UpdateHeightCascade(subTree->up);
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
			auto UpOfRMOL=RMOL->up;
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
			UpdateHeightCascade(UpOfRMOL);


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

			UpdateHeight(RMOL);
			UpdateHeightCascade(RMOL->up);
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

		UpdateHeight(A);
		UpdateHeight(B);
		UpdateHeightCascade(B->up);
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


void DrawTreeNode(
	const BinaryTree <int,int> &tree,
	typename BinaryTree <int,int>::NodeHandle ndHd,
	int prevCx,int prevCy,int x0,int x1,int y,int yStep,
	typename BinaryTree <int,int>::NodeHandle highlight)
{
	if(ndHd!=nullptr)
	{
		if(highlight==ndHd)
		{
			glColor3f(0.5,0.5,0.5);
			glBegin(GL_QUADS);
			glVertex2i(x0,y);
			glVertex2i(x1,y);
			glVertex2i(x1,y+yStep);
			glVertex2i(x0,y+yStep);
			glEnd();
			glColor3f(0,0,0);
		}

		int cx=(x0+x1)/2;
		int cy=y+yStep/2;
		glBegin(GL_LINES);
		glVertex2i(prevCx,prevCy);
		glVertex2i(cx,cy);
		glEnd();

		char str[256];
		sprintf(str,"%d(%d)",tree.GetKey(ndHd),tree.GetHeight(ndHd));
		glRasterPos2i(cx,cy);
		YsGlDrawFontBitmap10x14(str);

		DrawTreeNode(tree,tree.GetLeft(ndHd),cx,cy,x0,cx,y+yStep,yStep,highlight);
		DrawTreeNode(tree,tree.GetRight(ndHd),cx,cy,cx,x1,y+yStep,yStep,highlight);
	}
}

void DrawTree(const BinaryTree <int,int> &tree,typename BinaryTree <int,int>::NodeHandle highlight)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	DrawTreeNode(tree,tree.GetRoot(),
	   wid/2,0,  // prevCx,prevCy
	   0,wid,  // x0,x1
	   0,40,    // y,yStep
	   highlight
	   );
}

typename BinaryTree <int,int>::NodeHandle PickNode(
	const BinaryTree <int,int> &tree,
	typename BinaryTree <int,int>::NodeHandle ndHd,
	int prevCx,int prevCy,int x0,int x1,int y,int yStep,
	int mx,int my)
{
	if(ndHd!=nullptr)
	{
		if(x0<=mx && mx<x1 && y<=my && my<y+yStep)
		{
			return ndHd;
		}

		int cx=(x0+x1)/2;
		int cy=y+yStep/2;

		auto left=PickNode(tree,tree.GetLeft(ndHd),cx,cy,x0,cx,y+yStep,yStep,mx,my);
		if(left!=nullptr)
		{
			return left;
		}
		auto right=PickNode(tree,tree.GetRight(ndHd),cx,cy,cx,x1,y+yStep,yStep,mx,my);
		if(right!=nullptr)
		{
			return right;
		}
	}
	ndHd.Nullify();
	return ndHd;
}

typename BinaryTree <int,int>::NodeHandle PickNode(const BinaryTree <int,int> &tree,int mx,int my)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	return PickNode(tree,tree.GetRoot(),
	   wid/2,0,  // prevCx,prevCy
	   0,wid,  // x0,x1
	   0,40,    // y,yStep
	   mx,my);
}

int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	srand(time(nullptr));

	BinaryTree <int,int> tree;

	for(int i=0; i<100; ++i)
	{
		auto r=rand()%100;
		tree.Insert(r,0);
	}

	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		int lb,mb,rb,mx,my;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);

		auto ndHd=PickNode(tree,mx,my);


		switch(key)
		{
		case FSKEY_L:
			tree.RotateLeft(ndHd);
			break;
		case FSKEY_DEL:
			tree.Delete(ndHd);
			break;
		}


		int wid,hei;
		FsGetWindowSize(wid,hei);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,wid,hei,0,-1,1);

		glViewport(0,0,wid,hei);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		DrawTree(tree,ndHd);
		FsSwapBuffers();
	}
}
