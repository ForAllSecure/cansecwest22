#include <stdint.h>
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

int LLVMFuzzerTestOneInput(char* data, size_t size)
{
    fuzzme(data);
    return 0;
}
