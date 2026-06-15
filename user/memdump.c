#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data);

int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Example 1:\n");
    int a[2] = { 61810, 2025 };
    memdump("ii", (char*) a);
    
    printf("Example 2:\n");
    memdump("S", "a string");

    printf("Example 3:\n");
    char *s = "another"; // s is a pointer, initialized to point to a string constant
    memdump("s", (char *) &s);

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;
    
    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");
    
    printf("Example 4:\n");
    memdump("pihcS", (char*) &example);
    
    printf("Example 5:\n");
    memdump("sccccc", (char*) &example);
  } else if(argc == 2){
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while(n < sizeof(data)){
      int nn = read(0, data + n, sizeof(data) - n);
      if(nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data)
{
  //printf("%s\n","heey");
  // Your code here.
  char *ptr=data;
  for(int i = 0; fmt[i]; i++){
    if (fmt[i]=='i'){ // print the next 4 bytes of the data as a 32-bit integer, in decimal
      printf("%d\n",(*(uint32 *)ptr)); // interesting
      ptr+=sizeof(uint32); // 4 bytes
    }
    if (fmt[i]=='p'){ // print the next 8 bytes of the data as a 64-bit integer, in hex.
      printf("%lx\n",(*(uint64 *)ptr));
      ptr+=sizeof(uint64); // 8 bytes
    }
    if (fmt[i]=='h'){ // print the next 2 bytes of the data as a 16-bit integer, in decimal.
      printf("%d\n",(*(uint16 *)ptr));
      ptr+=sizeof(uint16); // 2 bytes
    }
    if (fmt[i]=='c'){ // print the next 1 byte of the data as an 8-bit ASCII character.
      printf("%c\n",(uint8)(*ptr));
      //ptr+=sizeof(uint8);
      ptr+=sizeof(char); // 1 byte
    }
    if (fmt[i]=='s'){ // the next 8 bytes of the data contain a 64-bit pointer to a C string; print the string.
      char *strptr = *(char **)ptr;
        while(*strptr != '\0'){
          printf("%c", *strptr); // im so fakin guuud
          strptr++;
          
        }
        ptr+=sizeof(char *); // 8 bytes
        printf("\n");
    }
    if (fmt[i]=='S'){ // the rest of the data contains the bytes of a null-terminated C string; print the string.
      printf("%s\n",ptr);
      ptr+=sizeof(char);
    }
    
  }

}
