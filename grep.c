#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 1000

int main(int argc, char *argv[]) {
    int word_len;
    char str[MAXSTRLEN], *word;
    FILE *fd;
    int key = ((argc==4)&&(strcmp(*++argv, "-v")==0));
    
    if ((argc==3) || key){
        word_len = strlen(*++argv);
        char word[word_len];
        strcpy(word, *argv);
        if ((fd = fopen(*++argv, "r")) == NULL) {
            printf("can't open %s\n", *argv);
            exit(1);
        } else {
            while (fgets(str, MAXSTRLEN, fd)) {
                char * i = strstr(str, word);
                if ((i==0) == key) {
                    printf("%s", str);
                }
            }
        }
    } else {
        printf("use pattern: grep [-v] word_to_find file_to_search\n");
    }
    return 0;
}