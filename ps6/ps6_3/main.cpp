#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> 
#include <time.h>
#include <cstdio>

#include "fssimplewindow.h"
#include "ysglfontdata.h"

#include "bintree.h"

const int DY=40;

// Variable to track the last inserted node
typename BinaryTree<int,int>::NodeHandle lastInsertedNode;

void RenderTreeNode(
   const BinaryTree<int,int> &tree,
   typename BinaryTree<int,int>::NodeHandle ndHd,
   int x0,int x1,int y0,int prevX,int prevY,
   typename BinaryTree<int,int>::NodeHandle highlight)
{
	if(true==ndHd.IsNull())
	{
		return;
	}

	if(ndHd==highlight)
	{
		glColor3f(0.5,0.5,0.5);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x0,y0);
		glVertex2i(x1,y0);
		glVertex2i(x1,y0+DY);
		glVertex2i(x0,y0+DY);
		glEnd();
		glColor3f(0,0,0);
	}
	
	// Highlight the last inserted node with light blue color
	if(ndHd==lastInsertedNode)
	{
		glColor3f(0,1,1); // Light blue (R=0, G=255, B=255)
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x0,y0);
		glVertex2i(x1,y0);
		glVertex2i(x1,y0+DY);
		glVertex2i(x0,y0+DY);
		glEnd();
		glColor3f(0,0,0);
	}

	int cx=(x0+x1)/2;
	int cy=y0+DY/2;

	char str[256];
	sprintf(str,"%d(%d)",tree.GetKey(ndHd),tree.GetHeight(ndHd));
	glRasterPos2i(cx,cy);
	YsGlDrawFontBitmap10x14(str);

	glBegin(GL_LINES);
	glVertex2i(prevX,prevY);
	glVertex2i(cx,cy);
	glEnd();

	RenderTreeNode(tree,tree.LeftOf(ndHd),x0,cx,y0+DY,cx,cy,highlight);
	RenderTreeNode(tree,tree.RightOf(ndHd),cx,x1,y0+DY,cx,cy,highlight);
}

void RenderTree(const BinaryTree<int,int> &tree,typename BinaryTree<int,int>::NodeHandle highlight)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);
	RenderTreeNode(tree,tree.Root(),0,wid,0,wid/2,0,highlight);
}

typename BinaryTree<int,int>::NodeHandle FindPickedTreeNode(
   const BinaryTree<int,int> &tree,
   typename BinaryTree<int,int>::NodeHandle ndHd,
   int x0,int x1,int y0,int mx,int my)
{
	if(true==ndHd.IsNull())
	{
		BinaryTree<int,int>::NodeHandle nullHd;
		return nullHd;
	}

	if(x0<=mx && mx<x1 && y0<=my && my<y0+DY)
	{
		return ndHd;
	}

	int cx=(x0+x1)/2;
	auto found=FindPickedTreeNode(tree,tree.LeftOf(ndHd),x0,cx,y0+DY,mx,my);
	if(true==found.IsNotNull())
	{
		return found;
	}
	return FindPickedTreeNode(tree,tree.RightOf(ndHd),cx,x1,y0+DY,mx,my);
}

typename BinaryTree<int,int>::NodeHandle PickedNdHd(
	BinaryTree<int,int> &tree,
	int mx,int my)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);
	auto ndHd=FindPickedTreeNode(tree,tree.Root(),0,wid,0,mx,my);
	return ndHd;
}

// Recursive function to insert nodes in a way that creates a perfectly balanced BST
void insertBalanced(BinaryTree<int,int> &tree, int start, int end)
{
    if (start > end)
        return;
        
    // Find the middle element
    int mid = (start + end) / 2;
    
    // Insert the middle element
    std::cout << "Inserting: " << mid << std::endl;
    lastInsertedNode = tree.Insert(mid, 0);
    
    // Recursively insert left subtree
    insertBalanced(tree, start, mid-1);
    
    // Recursively insert right subtree
    insertBalanced(tree, mid+1, end);
}

int main(void)
{
	BinaryTree<int,int> tree;
	
	// Turn off auto-rebalancing
	tree.autoRebalancing = false;

	// Instead of random insertion, use our balanced insertion algorithm
    insertBalanced(tree, 1, 127);

	// Print the keys in order
	std::cout << "In-order traversal:" << std::endl;
	for(auto hd=tree.FindFirst(); true==hd.IsNotNull(); hd=tree.FindNext(hd))
	{
		std::cout << "+ " << tree.GetKey(hd) << std::endl;
	}

	FsOpenWindow(0,0,1024,800,1);

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
		auto pickedNdHd=PickedNdHd(tree,mx,my);
		if(true==pickedNdHd.IsNotNull())
		{
			std::cout << tree.GetKey(pickedNdHd) << std::endl;
		}

		if(FSKEY_DEL==key && true==pickedNdHd.IsNotNull())
		{
			tree.Delete(pickedNdHd);
		}
		if(FSKEY_L==key && true==pickedNdHd.IsNotNull())
		{
			tree.RotateLeft(pickedNdHd);
		}

		int wid,hei;
		FsGetWindowSize(wid,hei);
		glViewport(0,0,wid,hei);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,wid,hei,0,-1,1);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		RenderTree(tree,pickedNdHd);
		FsSwapBuffers();
	}

	return 0;
}