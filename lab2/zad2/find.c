#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

const char format[] = "%Y-%m-%d %H:%M:%S";

void print_from_stat(const char *filename, const struct stat *statptr)
{
    char file_type[64] = "undefined";

    if (S_ISREG(statptr->st_mode))
        strcpy(file_type, "file");
    else if (S_ISDIR(statptr->st_mode))
        strcpy(file_type, "dir");
    else if (S_ISLNK(statptr->st_mode))
        strcpy(file_type, "slink");
    else if (S_ISCHR(statptr->st_mode))
        strcpy(file_type, "char dev");
    else if (S_ISBLK(statptr->st_mode))
        strcpy(file_type, "block dev");
    else if (S_ISFIFO(statptr->st_mode))
        strcpy(file_type, "fifo");
    else if (S_ISSOCK(statptr->st_mode))
        strcpy(file_type, "socket");

    struct tm tm_modif_time;
    localtime_r(&statptr->st_mtime, &tm_modif_time);
    char modif_time_str[255];
    strftime(modif_time_str, 255, format, &tm_modif_time);

    struct tm tm_access_time;
    localtime_r(&statptr->st_atime, &tm_access_time);
    char access_time_str[255];
    strftime(access_time_str, 255, format, &tm_access_time);

    printf("%s || type: %s, size: %ld, modification time: %s, access time: %s\n",
           filename, file_type, statptr->st_size, modif_time_str, access_time_str);
}

void maxdepth(char *root_path, int depth)
{
    if (depth == 0)
        return;
    if (root_path == NULL)
        return;
    DIR *dir = opendir(root_path);

    if (dir == NULL)
    {
        fprintf(stderr, "error open dir: %s\n", strerror(errno));
        exit(-1);
    }

    struct dirent *file;

    char new_path[256];
    while ((file = readdir(dir)) != NULL)
    {
        strcpy(new_path, root_path);
        strcat(new_path, "/");
        strcat(new_path, file->d_name);

        struct stat sb;

        if (lstat(new_path, &sb) < 0)
        {
            fprintf(stderr, "unable to lstat file %s: %s\n", new_path, strerror(errno));
            exit(-1);
        }

        if (S_ISDIR(sb.st_mode))
        {
            if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
            {
                continue;
            }

            maxdepth(new_path, depth - 1);
        }

        print_from_stat(new_path, &sb);
    }
    closedir(dir);
}

void print_files_handler(char *root_path, char *command, char mode, int count)
{
    if (strcmp(command, "-maxdepth") == 0)
    {
        maxdepth(root_path, count);
    }
}

int main()
{
    char tab[5] = {'a'};
    print_files_handler("../../", "-maxdepth", tab[0], 2);
}