#include "stdinc.h"
#include "datatypes.h"
#include <ctype.h>
#include <time.h>
#include <sys/mman.h>


void check_verses(int b_idx, int chapter, int verse_s, int verse_e)
{
	extern book_t **books;
	int i = 1;
	chapter_t *cursor = books[b_idx]->chapters->next;

	if(verse_s > verse_e)
		sc_error("Starting verse is greater than ending verse!");
	
	for(; i != chapter; cursor = cursor->next)
		i++;
	
	if(verse_e > cursor->max_verse+1) 
		sc_error("Ending verse does not exist!");
}

void check_chapter(int b_idx, int chapter)
{
	extern book_t **books;

	if(chapter <= 0 || chapter > books[b_idx]->max_chapter)
		sc_error("Chapter selected does not exist");
}

int check_book(char *book)
{
	int i;
	extern book_t **books;
	extern int book_count;


	for(i=0; i < book_count; i++) {
		if(!strcasecmp(book, books[i]->alias))
			return i;
	}

	sc_error("Book selected does not exist");

	return -1; /* shut up compiler */
}

void show_books(void)
{
	int i;
	int x;
	extern book_t **books;
	extern int book_count;

	for(i=0; i < book_count; i++) {
		printf("%s ", books[i]->title);
		x = strlen(books[i]->title);

		for(; x < 25; x++) 
			printf(" ");		
		printf("key=%s\n", books[i]->alias);
	}

}

int check_ranges(char *book, int chapter, int verse_s, int verse_e)
{
	int b_idx;

	b_idx = check_book(book);
	check_chapter(b_idx, chapter);	
	check_verses(b_idx, chapter, verse_s, verse_e);

	return b_idx;
}

void parse_text_random(const char *path, int b_idx, int chapter, int verse_s, int verse_e)
{
	char *map;
	char *ptr;
	int fd;
	int i;
	int c_count = 0;
	int b = 0; /* Book */
	int c = 0; /* Chapter */
	int v = 0; /* Verse */
	int b_w = b_idx;
	int c_w = chapter;
	int v_w = verse_s;
	int v_e = verse_e;
	extern book_t **books;
	extern int book_count;
	struct stat fstats;

	fd = open(path, 0444);
	if(fd < 0)
		sc_error_fmt("Unable to open %s for reading.", path);

	if(stat(path, &fstats)<0)
		sc_error("stat()");

	srand(time(NULL));

	if(b_w < 0) 
		b_w = (int) (book_count * (rand() / (RAND_MAX + 1.0)));

	if(!c_w)
		c_w = 1 + (int) (books[b_w]->max_chapter * (rand() / (RAND_MAX + 1.0)));
	
	if(!v_w)
		v_w = 1 + (int) (find_max_verse(books[b_w]->chapters, c_w) 
		* (rand() / (RAND_MAX + 1.0)));
	if(!v_e)
		v_e = v_w;

	map = (char *) mmap(NULL, fstats.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if(map == MAP_FAILED)
		sc_error("mmap()");	
	
	ptr = map;	
	
	printf("\n ");	

	for(i=0; i < fstats.st_size; i++) {

		if(*ptr == '@') {
			b++;
			c = 0;
			v = 0;
		}

		else if(*ptr == '^') {
			c++;
			v = 0;
		}
		
		else if(*ptr == '~') 
			v++;
		
		
		if(b == b_w+1 && c == c_w && v >= v_w && v <= v_e) {
			c_count++;
			if(*ptr != '~' && *ptr != '^') {

				if(c_count >= 65 && *ptr == ' ') {
					printf("  \n");
					c_count = 0;
				} 

				if(*ptr == '\n')
					putchar(' ');
				else			
					putchar(*ptr);
			} 
		}

		ptr++;
	}

	if(v_w == v_e)
		printf("\n\n %s %d:%d\n\n", books[b_w]->title,c_w,v_w);
	else
		printf("\n\n %s %d:%d-%d\n\n", books[b_w]->title,c_w,v_w, v_e);

        munmap(map, fstats.st_size);
	close(fd);
}

void read_layout(void)
{
	FILE *fp;
	char *p;
	char *ptr;
	char *sptr;
	char buf[512];
	int num = 0;
	int tag_open = 0;	
	extern const char *layout_path;
	extern book_t **books;
	extern int book_count;

	fp = fopen(layout_path, "r");
	if(!fp)
		sc_error_fmt("Unable to open layout file %s", layout_path);

	while((p = fgets(buf, sizeof(buf), fp)) != NULL) {

		if(*buf == '#')
			continue;

		if(strstr(buf, "<book title=")) {
			ptr = strtok(buf, "\"");
			if(!ptr)
				sc_error("Broken layout file");
			ptr++;
			ptr = strtok(NULL, "\"");
			if(!ptr)
				sc_error("Broken layout file");
			
			books = realloc(books, (1 + num) * sizeof(book_t));
			if(!books)
				sc_error("realloc()");

			books[num] = calloc(1, sizeof(book_t));
			if(books[num] == NULL)
				sc_error("malloc()");

			books[num]->title = strdup(ptr);
			books[num]->chapters = node_alloc();
			books[num]->max_chapter = 0;

			ptr += strlen(ptr)+1;

			if((ptr = strstr(ptr, "alias="))) {
				ptr = strtok(ptr, "\"");
				if(ptr) {
					ptr=strtok(NULL, "\"");
					books[num]->alias = strdup(ptr);
				}				
			} 
	
			if(!books[num]->alias)
				books[num]->alias = strdup(books[num]->title);

			tag_open = 1;

			continue;
		}

		if(tag_open == 1) {

			if(strstr(buf, "</book>")) {
				tag_open = 0;
				num++;
				continue;	
			}
		
			sptr=strtok(buf, ",");
			
			while(sptr) {
				books[num]->max_chapter++;
				node_add(books[num]->chapters, atoi(sptr));
				sptr = strtok(NULL, ",");
			}		
		}
	}

	book_count = num;
	fclose(fp);
}

void check_for_book(char *path)
{
	struct stat fstats;
	static char tbuf[1024];
	char *tmp;
	extern const char *book_path;
	extern const char *layout_path;	

	if(stat(path, &fstats) < 0)
		sc_error("Invalid path to book");
	
	if(!strstr(path, ".book\0"))
		sc_error("Invalid book extension - not '.book'");

	strncpy(tbuf, path, sizeof(tbuf)-1);
	
	tmp = strrchr(tbuf, '.');
	
	*tmp = '\0';

	strncat(tbuf, ".layout", sizeof(tbuf)-1);

	if(stat(tbuf, &fstats) < 0)
		sc_error("No accompanying book layout");

	book_path = path;
	layout_path = tbuf;
}
