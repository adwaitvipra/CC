#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

//=============================================================================
//				UTILITIES
//=============================================================================

void traverse (node *head)
{
	while (head)
	{
		/* print token */
		head = head->next;
	}
	printf ("\n");

	return ;
}

void reverse_traverse (node *head)
{
	if (head)
	{
		while (head->next)
			head = head->next;

		while (head)
		{	
			/* print token */
			head = head->prev;
		}
		printf ("\n");
	}

	return;
}

int count (node *head)
{
	int cnt;

	for (cnt = 0; head; cnt++)
		head = head->next;

	return cnt;
}

node *reverse (node *head)
{
	node *curr = NULL, *prev = NULL;

	if (head && head->next)
	{
		curr = head;
		while (curr)
		{
			prev = curr->prev;
			curr->prev = curr->next;
			curr->next = prev;
			curr = curr->prev;
		}
		head = prev->prev;
	}

	return head;
}

//=============================================================================
//				INSERTIONS
//=============================================================================

bool get_node (node **new_node)
{
	bool flag = false;

	if (new_node && ((*new_node) = (node *) malloc (sizeof (node))))
		flag = true;

	return flag;
}

node *push (node *head, struct token_t *token)
{
	node *new_node = NULL;

	if (get_node (&new_node))
	{
		new_node->token = token;
		new_node->prev = new_node->next = NULL;

		if (head)
		{
			new_node->next = head;
			head->prev = new_node;
		}

		head = new_node;
	}

	return head;
}

node *append (node *head, struct token_t *token)
{
	node *new_node = NULL,
	     *nptr = NULL;

	if (get_node (&new_node))
	{
		new_node->token = token;
		new_node->next = new_node->prev = NULL;

		if (!head)
			head = new_node;
		else
		{
			for (nptr = head; nptr->next; nptr = nptr->next)
				;
			nptr->next = new_node;
			new_node->prev = nptr;
		}
	}

	return head;
}

node *insert (node *head, int idx, struct token_t *token)
{
	int cnt;
	node *new_node = NULL,
	     *tmp = NULL;

	cnt = count (head);

	if (idx >= 0 || idx <= cnt)
	{
		if (!idx)
			head = push (head, token);
		else if (idx == cnt)
			head = append (head, token);
		else if (get_node (&new_node))
		{
			new_node->token = token;
			new_node->next = new_node->prev = NULL;

			tmp = head;
			for(int i = 1; i < idx; i++)
				tmp = tmp->next;

			new_node->prev = tmp;
			new_node->next = tmp->next;

			tmp->next->prev = new_node;
			tmp->next = new_node;

		}
	}

	return head;
}

//=============================================================================
//				DELETIONS
//=============================================================================

node *pop (node *head)
{
	node *nptr = NULL;

	if (head)
	{
		nptr = head;
		head = head->next;

		if (head) 
			head -> prev = NULL;

		/* free token */
		free (nptr);
	}

	return head;
}

node *Remove (node *head, int idx)
{
	int cnt ;
	node *nptr = NULL;

	cnt = count (head);
	if (idx >= 0 || idx < cnt)
	{
		if (!idx)
			head = pop (head);
		else
		{
			nptr = head;
			for (int i = 0; i < idx; i++)
				nptr = nptr->next;

			nptr->prev->next = nptr->next;

			if (nptr->next)
				nptr->next->prev = nptr->prev;

			/* free token */
			free(nptr);
		}
	}

	return head;
}

void delete (node *list)
{
	node *tptr = NULL;

	while (list)
	{
		tptr = list;
		list = list->next;

		/* free token */
		free (tptr);
	}

	return;
}
