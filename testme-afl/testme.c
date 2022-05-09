#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fuzzme(char *buf)
{
  if(buf[0] == 'b')
    if(buf[1] == 'u')
      if(buf[2] == 'g') {
        abort(); // Defect: Sends a SIGABRT signal
      }
  return 0;
}

int main(int argc, char *argv[])
{
  FILE *f;
  char buf[12];

  if(argc != 2){
    fprintf(stderr, "Must supply a text file\n");
    return -1;
  }
  f = fopen(argv[1], "r");
  if(f == NULL){
    fprintf(stderr, "Could not open %s\n", argv[1]);
    return -1;
  }
  if(fgets(buf, sizeof(buf), f) == NULL){
    fprintf(stderr, "Could not read from %s\n", argv[1]);
    return -1;
  }
  fuzzme(buf);
  return 0;
}
