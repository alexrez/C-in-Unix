#include <stdio.h>

int main(int argc,char* argv[]){
  int i;
  for(i=1; argc-->1; i++)
    argc==1 ? printf("%s\n", argv[i]) : printf("%s ", argv[i]);
}
