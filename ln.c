#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int key_stat(char *str, char *substr){
    return strcmp(str,substr)==0;
}

void error(){
    printf("ln: no such file or directory\n");
    exit(-1);
}

int cantopen(char *file_to_open){
    int fd_s = open(file_to_open, O_RDONLY);
    close(fd_s);
    return fd_s==-1;
}

void take_name(char *path_to_file, char name[20]){
    char *sptr;
    int i = 0;
    if ((sptr = strrchr(path_to_file, '/')) != NULL){
        while (*++sptr != '\0') {
            name[i++] = *sptr;
        }
    } else {
            printf("ln: %s: file exists\n", name);
            exit(-1);
    }
    if (cantopen(path_to_file)==0){
        printf("ln: %s: file exists\n", name);
        exit(-1);
    }
}

int is_dir(char *name){
    DIR *dp = opendir(name);
    if (dp != NULL) {
        closedir(dp);
        return 1;
    }
    else return 0;
}

void make_path(char *name, char * path_to_dir){
    int len = strlen(path_to_dir);
    strcpy(path_to_dir, name);
    if (path_to_dir[len-1] != '/')
        strcat(path_to_dir, "/");
}

void make_target_name(char * dir_name, char *target_name, char *source_name){
    strcpy(target_name, dir_name);
    char name[20] = {};
    char *sptr;
    int i = 0;
    if ((sptr = strrchr(source_name, '/')) != NULL){
        while (*++sptr != '\0') {
            name[i++] = *sptr;
        }
        strcat(target_name, name);
    } else strcat(target_name, source_name);
}

void pattern(){
    printf("use one of the patterns:\nln [-s] source_file [target_file]\n");
    printf("ln [-s] source_file ... target_dir\n");
    exit (-1);
}

int main(int argc, char **argv){
    char *key = "-s";
    char target_file[128];
    int fd_s, fd_t;
    
    if (argc == 1){
        pattern();
    }

    /* symlink */
    if (key_stat(argv[1], key)){
        switch (argc) {
            case 2: pattern(); break;
            case 3:
                if (cantopen(argv[2])){
                    error();
                } else {
                    take_name(argv[2], target_file);
                    symlink(argv[2], target_file);
                } break;
            case 4:
                if (is_dir(argv[3])){
                    char path_to_dir[1024];
                    make_path(argv[3], path_to_dir);
                    if (cantopen(path_to_dir) ){
                        error();
                    } else {
                        char path_to_target[1024];
                        make_target_name(path_to_dir, path_to_target, argv[2]);
                        symlink(argv[2], path_to_target);
                    }
                }
                else
                    symlink(argv[2], argv[3]);
                break;
            default:
                if (is_dir(argv[--argc])){
                    char path_to_dir[1024];
                    make_path(argv[argc], path_to_dir);
                    if (cantopen(path_to_dir) ){
                        error();
                    } else {
                        char path_to_target[1024];
                        while (--argc>1) {
                            make_target_name(path_to_dir, path_to_target, argv[argc]);
                            symlink(argv[argc], path_to_target);
                        }
                    }
                } else error();
                break;
        }
    }
    /* hardlink */
    else {
        switch (argc) {
            case 2:
                if (cantopen(argv[1])){
                    error();
                } else if (is_dir(argv[1])) {
                    printf("ln: %s is a directory\n", argv[1]);
                    exit(-1);
                } else {
                    take_name(argv[1], target_file);
                    link(argv[1], target_file);
                } break;
            case 3:
                if (cantopen(argv[1])){
                    error();
                } else if (is_dir(argv[1])) {
                    printf("ln: %s is a directory\n", argv[1]);
                    exit(-1);
                } else {
                    if (is_dir(argv[2])){
                        char path_to_dir[1024];
                        make_path(argv[2], path_to_dir);
                        if (cantopen(path_to_dir) ){
                            error();
                        } else {
                            char path_to_target[1024];
                            make_target_name(path_to_dir, path_to_target, argv[1]);
                            link(argv[1], path_to_target);
                        }
                    }
                    else link(argv[1], argv[2]);
                } break;
            default:
                if (is_dir(argv[--argc])){
                    char path_to_dir[1024];
                    make_path(argv[argc], path_to_dir);
                    if (cantopen(path_to_dir) ){
                        error();
                    } else {
                        char path_to_target[1024];
                        while (--argc>0) {
                            if (is_dir(argv[argc])){
                                printf("ln: %s is a directory\n", argv[1]);
                                exit(-1);
                            }
                            make_target_name(path_to_dir, path_to_target, argv[argc]);
                            link(argv[argc], path_to_target);
                        }
                    }
                } else error();
                break;
        }
    }
    return 0;
}