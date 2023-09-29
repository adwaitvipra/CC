#ifndef LIST_H
#define LIST_H 1

typedef struct node
{
	struct token_t *token;
	struct node *prev;
	struct node *next;
} node;

void traverse (node *);
void reverse_traverse (node *);
int count (node *);
node *reverse (node *);

bool get_node (node **);
node *push (node *, struct token_t *);
node *append (node *, struct token_t *);
node *insert (node *, int, struct token_t *);

node *pop (node *);
node *Remove (node *, int);
void delete (node *);

#endif /* LIST_H */
