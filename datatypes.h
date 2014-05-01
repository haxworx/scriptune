#ifndef __data_types_h__
#define __data_types_h__

typedef struct chapter_t chapter_t;
struct chapter_t {
	int max_verse;
	chapter_t *next;
};

typedef struct book_t book_t;
struct book_t {
	char *title;
	char *alias;
	chapter_t *chapters;
	int max_chapter;
};

book_t **books;

chapter_t *node_alloc(void);
void node_add(chapter_t *root_node, int max_verse);
void nodes_free(chapter_t *root_node);
int find_max_verse(chapter_t *root_node, int chapter);
void book_data_free(void);
#endif
