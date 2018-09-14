#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

void printdir(DIR * dptr){
    struct dirent *ds;
    struct stat stbuf;
    int count=0;
    
    while ((ds = readdir(dptr)) != 0){
        if (ds->d_name[0] != '.'){
            printf("%s ", ds->d_name);
            count++;
        }
        lstat(ds->d_name, &stbuf);
        if ((stbuf.st_mode & S_IFMT) == S_IFLNK){
            char path_to_link[1024] = {0};
            if (readlink(ds->d_name, path_to_link, sizeof(path_to_link)))
                printf("-> %s\t", path_to_link);
            
        }
    }
    closedir(dptr);
    if (count>0)
        printf("\b\n");
}

int main(int argc, char **argv){
    char *cwd = ".";
    DIR *dp = opendir(cwd);
    if (argc > 1){
        int i = 1;
        while (i<argc){
            if ((dp = opendir(argv[i]))!=NULL){
                if (argc>2)
                    printf("%s:\n", argv[i]);
                printdir(dp);
                i++;
                if (argc>2)
                    printf("\n");
  
            }
            else printf("ls: %s: No such file or directory\n", argv[i++]);
        }
    } else {
        printdir(dp);
    }
}