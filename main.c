#include "stdinc.h"
#include "parser.h"
#include "datatypes.h"

#define TYPE_RANDOM 1
#define TYPE_SEARCH 2
#define TYPE_BOOKLS 3

#ifndef PREFIX
#define PREFIX "/usr/local"
#endif

int book_count;
const char *book_path;
const char *layout_path;

int main(int argc, char **argv)
{
	int i;
	int type = TYPE_RANDOM;
	char *book = "kjv.book";	
	char *path;
	int b_idx;
	char *book_s;
	int chapter;
	int verse_s;
	int verse_e;
	int args = 0;

	for(i = 1; i < argc; i++) {

		if(!strcmp(argv[i], "-h"))
			show_help();		

		if(!strcmp(argv[i], "-r")) {
			type = TYPE_RANDOM; 
		}

		if(!strcmp(argv[i], "-l")) {
			type = TYPE_BOOKLS;
			break;
		}
		
		if((!strcmp(argv[i], "-b")) && (i < argc -1)) {
			i++;
			book = argv[i];
		}

		if((!strcmp(argv[i], "-s")) && (i <= argc-4)) {
			
			args = i;			
			book_s = argv[++i]; 
			chapter = atoi(argv[++i]);
			verse_s = atoi(argv[++i]);
			args+=i;
			if(args < argc)
				verse_e = atoi(argv[++i]);
			else		
				verse_e = verse_s;			

			type = TYPE_SEARCH;
		}	
	}
	
	path = make_full_path(PREFIX, book);
	check_for_book(path);

	read_layout();

	switch(type) {
		case TYPE_RANDOM:
			parse_text_random(path, -1, 0, 0, 0);
		break;

		case TYPE_SEARCH:
			b_idx = check_ranges(book_s, chapter, verse_s, verse_e);
			parse_text_random(path, b_idx, chapter, verse_s, verse_e);
		break;

		case TYPE_BOOKLS:
			show_books();
		break;
	}

	book_data_free();

	return EXIT_SUCCESS;
}
