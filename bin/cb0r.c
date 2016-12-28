#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "../src/cb0r.h"

uint8_t *load(char *file, int32_t *len)
{
  uint8_t *buf;
  struct stat fs;
  FILE *fd;
  
  fd = fopen(file,"rb");
  if(!fd)
  {
    printf("fopen error %s: %s\n",file,strerror(errno));
    return NULL;
  }
  if(fstat(fileno(fd),&fs) < 0)
  {
    fclose(fd);
    printf("fstat error %s: %s\n",file,strerror(errno));
    return NULL;
  }
  
  if(!(buf = malloc((size_t)fs.st_size)))
  {
    fclose(fd);
    printf("OOM: %lld\n",fs.st_size);
    return NULL;
  }
  *len = fread(buf,1,(size_t)fs.st_size,fd);
  fclose(fd);
  if(*len != (size_t)fs.st_size)
  {
    free(buf);
    printf("fread %d != %lld for %s: %s\n",*len,fs.st_size,file,strerror(errno));
    return NULL;
  }

  return buf;
}

int main(int argc, char **argv)
{
  if(argc != 2)
  {
    printf("Usage: cb0r file.cbor\n");
    return -1;
  }

  int32_t len = 0;
  uint8_t *bin = load(argv[1],&len);
  if(!bin) return -1;

  cb0r_s res = {0,};
  uint8_t *end = cb0r(bin,bin+len,0,&res);

  printf("type %d len %ld\n",res.type,end-bin);

  free(bin);
  return 0;
}

