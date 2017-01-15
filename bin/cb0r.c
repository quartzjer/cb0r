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

size_t describe(cb0r_t in, char *out, uint32_t skip)
{
  size_t outlen = 0;
  cb0r_s res = {0,};
  cb0r(in->start,in->end,skip,&res);
  switch(res.type)
  {
    case CB0R_INT: {
      outlen = sprintf(out,"%llu",res.value);
    } break;
    case CB0R_NEG: {
      outlen = sprintf(out,"-%llu",res.value+1);
    } break;
    case CB0R_BYTE: if(res.count != CB0R_STREAM) {
      break;
    };
    case CB0R_UTF8: if(res.count != CB0R_STREAM) {
      outlen = sprintf(out,"\"%.*s\"",(int)res.length,res.start);
      break;
    };
    case CB0R_ARRAY:
    case CB0R_MAP: {
      outlen += sprintf(out+outlen,(res.type==CB0R_MAP)?"{":"[");
      // streaming, need to find end first
      if(res.count == CB0R_STREAM)
      {
        res.count = 0;
        cb0r(res.start,res.end,CB0R_STREAM,&res);
      }
      for(uint32_t i=0;i<res.count;i++)
      {
        if(i) outlen += sprintf(out+outlen,",");
        outlen += describe(&res,out+outlen,i);
        if(res.type != CB0R_MAP) continue;
        outlen += sprintf(out+outlen,":");
        outlen += describe(&res,out+outlen,++i);
      }
      outlen += sprintf(out+outlen,(res.type==CB0R_MAP)?"}":"]");
    } break;
    case CB0R_TAG: {
      outlen += sprintf(out+outlen,"TAG[%llu]",res.value);
    } break;
    case CB0R_SIMPLE: {
      outlen += sprintf(out+outlen,"SIMPLE[%llu]",res.value);
    } break;
    case CB0R_FALSE: {
      outlen = sprintf(out,"false");
    } break;
    case CB0R_TRUE: {
      outlen = sprintf(out,"true");
    } break;
    case CB0R_NULL: {
      outlen = sprintf(out,"null");
    } break;
    case CB0R_UNDEF: {
      outlen = sprintf(out,"undefined");
    } break;
    case CB0R_FLOAT: {
      outlen = sprintf(out,"FLOAT[%llu]",res.length);
    } break;
    default: {
      if(res.type > CB0R_TAGS && res.type < CB0R_SIMPLES) outlen = sprintf(out,"TAGS[%u]",res.type);
      else if(res.type >= CB0R_ERR) outlen = sprintf(out,"ERR![%u]",res.type);
      else outlen = sprintf(out,"TODO[%u]",res.type);
    } break;

  }
  return outlen;
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

  char *out = malloc(len*4);
  memset(out,0,len*4);
  describe(&(cb0r_s){.type = CB0R_DATA, .start = bin, .end = bin+len, .length = len}, out, 0);
  printf("%s\n",out);

  free(bin);
  free(out);
  return 0;
}

