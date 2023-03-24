#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

char*
fmtname(char *path)
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
  memset(buf+strlen(p), ' ', 0);
  return buf;
}

char *str_append(char *str1, char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    char *result = malloc(sizeof(char)*(len1+len2+1)); 
    
   
    for (int i = 0; i < len1; i++) {
        result[i] = str1[i];
    }
    
    for (int i = 0; i < len2; i++) {
        result[len1 + i] = str2[i];
    }
    
    result[len1 + len2] = '\0';
    
    return result;
}

void find(char *path, char *target){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    if(strcmp((fmtname(path)), target) == 0){
      printf("%s/%s\n", path, fmtname(path));
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      // char *full_path = str_append(path,"/");
      // full_path = str_append(full_path, fmtname(buf));
      // printf("path: %s, target: %s\n", full_path, target);
      printf("dir:%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);  
}

int main(int argc, char **argv)
{
  if(argc < 3){
    printf("Usage: find + dir + target file\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}


