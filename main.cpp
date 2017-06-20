#include <stdio.h>
#include <malloc.h>

typedef struct Node* NodePtr;
struct Node {
	int val;
	NodePtr left, right, parent;
	char color;
};

NodePtr node_alloc_nil()
{
	NodePtr self = (NodePtr)malloc(sizeof(struct Node));
	self->color = 'b';
	return self;
}
NodePtr NIL = node_alloc_nil();
NodePtr node_alloc(int newval) {
	NodePtr self = (NodePtr)malloc(sizeof(struct Node));
	self->val = newval;
	self->left = NIL;
	self->right = NIL;
	self->parent = NULL;
	self->color = 'b';
	return self;
}

typedef struct BST* BSTPtr;
struct BST {
	NodePtr root;
};

BSTPtr bst_alloc() {
	BSTPtr self = (BSTPtr)malloc(sizeof(struct BST));
	self->root = NULL;
	return self;
}

void bst_print(BSTPtr self, NodePtr tree, int level) {
	if (tree->right != NIL)
		bst_print(self, tree->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("    ");
	printf("%d%c\n", tree->val, tree->color);
	if (tree->left != NIL)
		bst_print(self, tree->left, level + 1);
}

void bst_inorder(BSTPtr self, NodePtr tree) {
	if (tree == NIL)
		return;
	else {
		bst_inorder(self, tree->left);
		printf("%d %c\n", tree->val, (tree->color) - 32);
		bst_inorder(self, tree->right);
	}
}

void bst_inorder_count(BSTPtr self, NodePtr tree, int* nodenum, int* blacknodenum) {
	if (tree == NIL)
		return;
	else {
		bst_inorder_count(self, tree->left, nodenum, blacknodenum);
		nodenum[0]++;
		if (tree->color == 'b') blacknodenum[0]++;
		bst_inorder_count(self, tree->right, nodenum, blacknodenum);
	}
}

void bst_blackheight(BSTPtr self, int* bh) {
	NodePtr n = self->root;
	for (;;)
	{
		if (n == NIL) return;
		else
		{
			n = n->right;
			if (n->color == 'b')bh[0]++;
		}
	}
}

NodePtr bst_search(BSTPtr self, int n)
{
	NodePtr currentNode = self->root;
	if (self->root == NULL) return NULL;
	for (;;)
	{
		if (currentNode == NIL || n == currentNode->val)
			return currentNode;
		if (n < currentNode->val)
		{
			currentNode = currentNode->left;
		}
		else
		{
			currentNode = currentNode->right;
		}
	}
}


// subtree의 Minimum을 찾는 함수
NodePtr bst_min(NodePtr self)
{
	NodePtr currentNode = self;
	while (currentNode->left != NIL)
	{
		currentNode = currentNode->left;
	}
	return currentNode;
}

NodePtr bst_max(NodePtr self)
{
	NodePtr currentNode = self;
	while (currentNode->right != NIL)
	{
		currentNode = currentNode->right;
	}
	return currentNode;
}

NodePtr bst_successor(NodePtr x)
{
	NodePtr y;
	if (x->right != NIL)
		return bst_min(x->right);
	y = x->parent;
	while (y != NIL && x == y->right)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

NodePtr bst_predecessor(NodePtr x)
{
	NodePtr y;
	if (x->left != NIL)
		return bst_max(x->left);
	y = x->parent;
	while (y != NIL && x == y->left)
	{
		x = y;
		y = y->parent;
	}
	return y;
}


void rotate_left(BSTPtr T, NodePtr x)
{
	NodePtr y = x->right;
	x->right = y->left;
	if (y->left != NIL)
	{
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == NULL)
		T->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}
void rotate_right(BSTPtr T, NodePtr x)
{
	NodePtr y = x->left;
	x->left = y->right;
	if (y->right != NIL) y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL) T->root = y;
	else if (x == x->parent->right) x->parent->right = y;
	else x->parent->left = y;
	y->right = x;
	x->parent = y;
}

void transplant(BSTPtr T, NodePtr u, NodePtr v)
{
	if (u->parent == NULL) T->root = v;
	else if (u == u->parent->left) u->parent->left = v;
	else u->parent->right = v;
	if (v != NULL) v->parent = u->parent;
}
void rb_insert_fixup(BSTPtr T, NodePtr z);
void rb_insert(BSTPtr T, NodePtr z)
{
	NodePtr x, y = NIL;
	if (T->root == NULL)
	{
		T->root = z;
		z->color = 'b';
		return;
	}
	x = T->root;
	while (x != NIL)
	{
		y = x;
		if (z->val < x->val) x = x->left;
		else x = x->right;
	}
	z->parent = y;
	if (y == NIL)
	{
		T->root = z;
		z->color = 'b';
		return;
	}
	else if (z->val < y->val) y->left = z;
	else y->right = z;
	z->left = NIL;
	z->right = NIL;
	z->color = 'r';
	rb_insert_fixup(T, z);
}
void rb_insert_fixup(BSTPtr T, NodePtr z)
{
	NodePtr y;
	if (z->parent == T->root)return;
	while (z->parent->color == 'r')
	{
		if (z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right;
			if (y->color == 'r')
			{
				z->parent->color = 'b';
				y->color = 'b';
				z->parent->parent->color = 'r';
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->right)
				{
					z = z->parent;
					rotate_left(T, z);
				}
				z->parent->color = 'b';
				z->parent->parent->color = 'r';
				rotate_right(T, z->parent->parent);
			}
		}
		else
		{
			y = z->parent->parent->left;
			if (y->color == 'r')
			{
				z->parent->color = 'b';
				y->color = 'b';
				z->parent->parent->color = 'r';
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->left)
				{
					z = z->parent;
					rotate_right(T, z);
				}
				z->parent->color = 'b';
				z->parent->parent->color = 'r';
				rotate_left(T, z->parent->parent);
			}
		}
		if (z->parent == NULL) break;
	}
	T->root->color = 'b';
}
void rb_delete_fixup(BSTPtr T, NodePtr x);

void rb_delete(BSTPtr T, NodePtr z)
{
	NodePtr x, y = z;
	if (z == T->root && T->root->left == NIL && T->root->right == NIL)
	{
		T->root = NULL;
		return;
	}
	char y_origin_color = y->color;
	if (z->left == NIL)
	{
		x = z->right;
		transplant(T, z, z->right);
	}
	else if (z->right == NIL)
	{
		x = z->left;
		transplant(T, z, z->left);
	}
	else
	{
		y = bst_min(z->right);
		y_origin_color = y->color;
		x = y->right;
		if (y->parent == z)x->parent = y;
		else
		{
			transplant(T, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		transplant(T, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if (y_origin_color == 'b')
		rb_delete_fixup(T, x);
}
void rb_delete_fixup(BSTPtr T, NodePtr x)
{
	NodePtr w;
	while (x->color == 'b')
	{
		if (x == T->root) break;
		if (x == x->parent->left)
		{
			w = x->parent->right;
			if (w->color == 'r')
			{
				w->color = 'b';
				x->parent->color = 'r';
				rotate_left(T, x->parent);
				w = x->parent->right;
			}
			if (w->left->color == 'b' && w->right->color == 'b')
			{
				w->color = 'r';
				x = x->parent;
			}
			else if (w->right->color == 'b')
			{
				w->left->color = 'b';
				w->color = 'r';
				rotate_right(T, w);
				w = x->parent->right;
			}
			else
			{
				w->color = x->parent->color;
				x->parent->color = 'b';
				w->right->color = 'b';
				rotate_left(T, x->parent);
				x = T->root;
			}
		}
		else
		{
			w = x->parent->left;
			if (w->color == 'r')
			{
				w->color = 'b';
				x->parent->color = 'r';
				rotate_right(T, x->parent);
				w = x->parent->left;
			}
			if (w->right->color == 'b' && w->left->color == 'b')
			{
				w->color = 'r';
				x = x->parent;
			}
			else if (w->left->color == 'b')
			{
				w->right->color = 'b';
				w->color = 'r';
				rotate_left(T, w);
				w = x->parent->left;
			}
			else
			{
				w->color = x->parent->color;
				x->parent->color = 'b';
				w->left->color = 'b';
				rotate_right(T, x->parent);
				x = T->root;
			}
		}
	}
	x->color = 'b';
}

void main()
{
	BSTPtr bst = bst_alloc();
	FILE *input, *search, *output;
	int a = 0;
	NodePtr a_del, a_search, a_pre, a_suc;
	fopen_s(&input, "input.txt", "r");
	fopen_s(&output, "output.txt", "w");
	fopen_s(&search, "search.txt", "r");
	for (;;) // input
	{
		fscanf_s(input, "%d\n", &a);
		if (a > 0)
		{
			rb_insert(bst, node_alloc(a));
			//bst_print(bst, bst->root, 0);
			//printf("\n------------------------------------------------------------------------\n");
		}
		else if (a < 0)
		{
			a_del = bst_search(bst, -a);
			if (a_del == NIL || a_del == NULL) continue;			
			else rb_delete(bst, a_del);				
			//bst_print(bst, bst->root, 0);
			//printf("\n------------------------------------------------------------------------\n");
		}
		else break;
	}
	while (fscanf_s(search, "%d\n", &a) != EOF) // search
	{
		a_search = bst_search(bst, a);
		if (a_search != NIL)
		{
			if (a_search == bst_min(bst->root))
			{
				fprintf_s(output,"NIL ");
			}
			else
			{
				a_pre = bst_predecessor(a_search);
				fprintf_s(output, "%d ", a_pre->val);
			}
			fprintf_s(output, "%d ", a);
			if (a_search == bst_max(bst->root))
			{
				fprintf_s(output, "NIL\n");
			}
			else
			{
				a_suc = bst_successor(a_search);
				fprintf_s(output, "%d\n", a_suc->val);
			}
		}
		else
		{
			rb_insert(bst, node_alloc(a));
			a_search = bst_search(bst, a);
			if (a_search == bst_min(bst->root))
			{
				fprintf_s(output, "NIL ");
			}
			else
			{
				a_pre = bst_predecessor(a_search);
				fprintf_s(output, "%d ", a_pre->val);
			}
			fprintf_s(output, "NIL ", a);
			if (a_search == bst_max(bst->root))
			{
				fprintf_s(output, "NIL\n");
			}
			else
			{
				a_suc = bst_successor(a_search);
				fprintf_s(output, "%d\n", a_suc->val);
			}
			rb_delete(bst, a_search);
		}
	}
	fclose(input);
	fclose(output);
	fclose(search);
}
