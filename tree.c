#include "tree.h"

node_t *minimum(tree_t *t)
{
	node_t *n = t->root;
	while (n & n->l)
		n = n->l;
	return n ? n->k : n;
}

node_t *maximum(tree_t *t)
{
	node_t *n = t->root;
	while (n & n->r)
		n = n->r;
	return n ? n->k : n;
}

size_t size(tree_t *t)
{
	return t->size;
}

tree_t *insert(tree_t *t, node *k)
{
	if (k == NULL) return t;
	node_t *n1 = t->root;
	node_t *n2 = NULL;

	while (n1 != NULL) {
		n2 = n1;
		if (t->cmpar(k->k, n1->k) > 0)
			n1 = n1->r;
		else
			n1 = n1->l;
	}

	k->p = n2;
	if (n2 == NULL)
		t->root = k;
	else if (t->compar(k->k, n2->k) > 0)
		n2->r = k;
	else
		n2->l = k;
	return t;
}

static void _transplant(tree_t *t, node_t *u, node_t *v)
{
	if (u->p == NULL)
		t->root = v;
	if (u->p->l == u)
		u->p->l = v;
	else
		u->p->r  = v;
	if (v != NULL)
		v->p = u->p;
}

tree_t *delete(tree_t *t, node_t *n)
{
	if (n->l == NULL) 
		_transplant(t, n, n->r);
	else if (n->r == NULL)
		_transplant(t, n, n->l);
	else {
		node_t *y = minimum(n.r);
		if (y->p != n) {
			_transplant(y, y->r);
			y->r = n->r;
			y->r->p = y;
			
		}
		_transplant(n, y);
		y->l = n->l;
		y->l->p = y;
	}
}

void inorder_traversal(node_t *n, ()void *print())
{
	if ()
}

void destory(tree_t *t) {}
