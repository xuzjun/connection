#ifndef _TREE_H_
#define _TREE_H_

typedef struct tree_s {
	size_t size;
	int    *cmpar(void *a, void *b);
	node_t *root;
} tree_t;

typedef struct node_s node_t;
struct node_s {
	node_t *p;
	node_t *l;
	node_t *r;
	void   *k;
}

tree_t *init();
node_t *minimum(tree_t *t);
node_t *maximum(tree_t *t);
size_t size(tree_t *t);
/*
 * return the tree
 */
tree_t *insert(tree_t *t, node_t *k);
tree_t *delete(tree_t *t, node_t *n);
void inorder_traversal(tree_t *t, void print(node_t *n));
// void *predecessor(tree_t *t, node_t *n);
// void *successor(tree_t *t, node_t *n);
void destory(tree_t *t);

#endif
