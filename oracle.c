#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int **rand_seq(int count, int total)
{
   FILE *f;
   char buf[256];
   unsigned int seed = 0;
   if (count <= 0) return NULL;
   int **seq = malloc(count * sizeof(unsigned int *));

   for (int i = 0; i < count; i++)
     {
        seed = 0;
        f = fopen("/dev/urandom", "r");
        if (!f)
          return NULL;
        fread(buf, 1, sizeof(buf), f);
        for (int j = 0; j < sizeof(buf); j++)
          seed += (unsigned int) buf[j];
        fclose(f);
        srand(seed);
        seq[i] = malloc(sizeof(unsigned int));
        *seq[i] = (unsigned int) (total * (rand() / (RAND_MAX + 1.0)));
     }
  
   return seq; 
}

static void
_trim(char *word)
{
   char *p = word;

   while (*p++)
     {
        if (p[0] == '\r' || p[0] == '\n')
          {
             p[0] = '\0';
             return;
          }
     }
}

int main(int argc, char **argv)
{
   const char *book;
   char *pos;
   struct stat st;
   int **seq;
   int i, count;

   if (argc != 3)
     {
        printf("Usage ./oracle [book] [count]\n");
        exit(2);
     }

   book = argv[1];
   if (stat(book, &st) < 0)
     exit(1);
  
   count = atoi(argv[2]);
   if (count <= 0) exit(1 << 3);

   int total_bytes = st.st_size;
 
   seq = rand_seq(count, total_bytes);

   int fd = open(book, 0444);
   if (fd == -1)
     {
        fprintf(stderr, "ERR: could not open '%s'\n", book);
        exit(1 << 2);
     }

   char *map = (char *) mmap(NULL, total_bytes, PROT_READ, MAP_SHARED, fd, 0);
   close(fd);

   i = 0;

   while (i < count)
     {
        int byte = 0;
        while (byte < total_bytes)
          {
             pos = map + byte++;
             if (byte == *seq[i])
               {
                  char *end, *start = pos;
                  while (*start++ != ' ');
                  end = strchr(start, ' ');
                  size_t len = end - start;
                  char *word = malloc(len);
                  memcpy(word, start, len);
                  word[len] = '\0';
                  _trim(word);
                  printf(" %s ", word); 
                  free(word);
                  free(seq[i]);
                  i++;
                  break;
               }
          } 
     }

   free(seq);
   printf("\n");

   return EXIT_SUCCESS;
}
