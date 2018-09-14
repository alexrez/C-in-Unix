#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXSTRLEN 1000

void printsym(char c){
    if (isprint(c))
        printf("%4c", c);
    else
        switch (c) {
            case '\t': printf("  \\t"); break;
            case '\n': printf("  \\n"); break;
            default: printf(" %0*o", 3, (unsigned char)c); break;
        }
}

int main(int argc, char *argv[]) {
    char symb;
    FILE *fd;
    int key = ((argc==3)&&(strcmp(*++argv, "-b")==0));
    
    if ((argc==2) || key){
        if ((fd = fopen(*++argv, "r")) == NULL) {
            printf("can't open %s\n", *argv);
            printf("use pattern: od [-b] file_to_output\n");
            exit(1);
        } else {
            int count=0;
            int newline;
            while ((symb=fgetc(fd))!=EOF) {
                newline = (((++count)%10) == 1);
                if (newline)
                    printf("\n%0*d ", 7, count);
                key ? printf(" %0*o", 3, (unsigned char)symb) : printsym(symb);
            }
            printf("\n%0*d\n", 7, count);
        }
    } else {
        printf("use pattern: od [-b] file_to_output\n");
    }
    return 0;
}