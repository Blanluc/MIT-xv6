// Simple grep.  Only supports ^ . * $ operators.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[1024];
int match(char*, char*);

void
sixfive(int fd)
{
    int n;
    char c;

    char nb_buf[64];
    int string_count=0;
    int string_valid=1;
    char special[]={'\r','\t','\n',' ','.',',','/','-','\0'};
    int beginning=1;

    // Read File (char by char)
    while((n = read(fd, &c, 1)) > 0){
        // if a number comes right after special char, ok
        // otherwise, invalid
        if (string_count==0 && !(c>=48 && c<=57)){ // if beginning of number and not number
            // printf("here\n");
            if (strchr(special,*(&c-1)+c)!=0){
                // printf("Yes\n");
                // printf("Special char is: %c %c\n",*(&c-1),c);
                string_valid=1;
            } else{
                // printf("NO\n");
                // printf("char is: %c %c\n",*(&c-1),c);
                string_valid=0;

            }
        }

        if ((c>=48 && c<=57 && string_valid)||(c>=48 && c<=57 && beginning)){ // if is withing range of digit ascii, add
            //printf("number added\n");
            string_valid=1;
            nb_buf[string_count++]=c;
        }else{
            
            if (string_count>0){
                //printf("number ended\n");
                if (atoi(nb_buf)%5==0 ||atoi(nb_buf)%6==0){
                    printf("%d\n",atoi(nb_buf));
                }
                for (int y=0;y<string_count;y++){
                    nb_buf[y]='\0';
                }
                string_count=0;
                string_valid=1;
            }
        }
        beginning=0;
    
    }
    // need this at the end
    if (string_count>0){
            //printf("number ended\n");
            if (atoi(nb_buf)%5==0 ||atoi(nb_buf)%6==0){
                printf("%d\n",atoi(nb_buf));
            }
            for (int y=0;y<string_count;y++){
                nb_buf[y]='\0';
            }
            string_count=0;
            string_valid=1;
        }
}

int
main(int argc, char *argv[])
{
  int fd, i;
  

  if(argc <= 1){
    fprintf(2, "usage: grep pattern [file ...]\n");
    exit(1);
  }


  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      printf("grep: cannot open %s\n", argv[i]);
      exit(1);
    }
    //printf("Staritng 6,5\n");
    sixfive(fd);
    close(fd);
  }
  exit(0);
}

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9, or
// https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
  if(re[0] == '^')
    return matchhere(re+1, text);
  do{  // must look at empty string
    if(matchhere(re, text))
      return 1;
  }while(*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[1] == '*')
    return matchstar(re[0], re+2, text);
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
  do{  // a * matches zero or more instances
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  return 0;
}

