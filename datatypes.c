#include "stdinc.h"
#include "datatypes.h"

chapter_t *node_alloc(void)
{
	chapter_t *tmp = malloc(sizeof(chapter_t));
	if(tmp == NULL)
		sc_error("malloc()");

	tmp->next = NULL;
	
	return tmp;
}


void node_add(chapter_t *root_node, int max_verse)
{
	chapter_t *cursor = root_node;

	while(cursor->next != NULL)	
		cursor=cursor->next;

	if(cursor->next == NULL) {
		cursor->next = node_alloc();
		cursor=cursor->next;
		cursor->max_verse = max_verse;
	} else
		sc_error("Undefined error");
		
}

int find_max_verse(chapter_t *root_node, int chapter)
{
	chapter_t *cursor = root_node->next;
	int i = 0;

	while(i < chapter-1) {
		cursor=cursor->next;
		i++;
	}

	return cursor->max_verse;
}

void book_data_free(void)
{
	extern int book_count;
	extern book_t **books;
	int i;

	for(i = 0; i < book_count; i++) {
		nodes_free(books[i]->chapters);
		if (books[i]->alias)
			free(books[i]->alias);
		free(books[i]->title);		
		free(books[i]);
	}

	free(books);
}



void nodes_free(chapter_t *root_node)
{	
	chapter_t *cursor = root_node;
	chapter_t *forward;

	while(cursor) {
		forward = cursor->next;
		free(cursor);
		cursor=forward;
	}
}
