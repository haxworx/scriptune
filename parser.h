#ifndef __parser_h__
#define __parser_h__

void check_for_book(char *path);
void read_layout(void);
void parse_text_random(const char *path, int b_idx, int chapter, int verse_s, int verse_e);
int check_ranges(char *book, int chapter, int verse_s, int verse_e);
void show_books(void);

#endif
