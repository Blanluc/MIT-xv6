#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"
// Helper function to identify valid chars


int
main(int argc, char *argv[])
{
  // TODO: CONFUSING, REVISE
  // Your code here.
  for (int i = 0; i < 128; i++) {
        char *ptr = sbrk(4096);
        for (int j = 0; j < 4096 - 16; j++) {
            if (memcmp(ptr + j, "This may help.", 14) == 0) {
              if (*(ptr + j)!='\0'){
                write(1, ptr + j + 16, 8);
                write(1, "\n", 1);}
                exit(0);
            }
        }
  }
  
  

  exit(1);
}