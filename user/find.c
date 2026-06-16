#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// Use recursion to allow find to descend into sub-directories.

// Note that == does not compare strings as in Python. Use strcmp() instead.

char*
fmtname(char *path) // i think it converts to name
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  buf[sizeof(buf)-1] = '\0';
  return buf;
}

// scan everything
// start with "."
// read names and find type
// if dir => recursion

void
find(char *path,char *name)
{
  char buf[512], *p;
  int fd; // file descriptor
  struct dirent de; // directory entry
  struct stat st;
  //char *path =".";
  char *name_var;

  // we start at "."

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // This is the part where i should compare
  switch(st.type){
  case T_DEVICE:
  case T_FILE: // if we gave the path to a file
    printf("%s %d %d %d\n", fmtname(path), st.type, st.ino, (int) st.size);
// if (strcmp(fmtname(path),name)){
//     printf("found it\n");
// } else{
//     printf("fail, compared %s and %s\n",fmtname(path),name);
// }
    break;

  case T_DIR:
   // scan dir
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    
    while(read(fd, &de, sizeof(de)) == sizeof(de)){ // go through stuff in dir
        // since we scan everything, we need recursion
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, (int) st.size);
      name_var = fmtname(buf); // careful, name_var points to the static buffer used by fmtname
      for (int i=0;name_var[i];i++){
        if (name_var[i]==' '){
            name_var[i]='\0';
            //printf("i = %d\n",i);
        } else{
            
        }
      }
      
      if (strcmp(name_var,name)==0){
            printf("%s/%s\n",path,name);
        } else{
           
        }

        // call recur
        //printf(" name var %s\n",name_var);
        if (strcmp(name_var,".")!=0 && strcmp(name_var,"..")!=0 && st.type==T_DIR){
            //printf(" yes \n");
            
            //*p++ = *name_var; // buf already contains the full path
            //printf(" moving to %s\n",buf);
            find(buf,name);

        } else{
            //printf("no\n");
        }
        
        
        
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  //int i;

  if(argc <= 2){
    fprintf(2, "usage: find [file ...]\n");
    //ls("."); 
    exit(0);
  } 
  if(argc == 2){
    find(".",argv[1]); // i dont think i need to pass "."
    exit(0);
  } 
  if(argc == 3){
    find(argv[1],argv[2]); // i dont think i need to pass "."
    exit(0);
  } 
  
//   for(i=1; i<argc; i++)
//     ls(argv[i]);
  
}
