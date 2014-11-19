#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
 

void fileType(int dtype) {
    if(dtype == DT_UNKNOWN)
        printf("\tType: Type is unknown\n");
    if(dtype == DT_REG)
        printf("\tType: Regular file\n");
    if(dtype == DT_DIR)
        printf("\tType: Directory\n");
    if(dtype == DT_FIFO)
        printf("\tType: named pipe, or FIFO\n");
    if(dtype == DT_SOCK)
        printf("\tType: local-domain socket\n");
    if(dtype == DT_CHR)
        printf("\tType: Character device\n");
    if(dtype == DT_BLK)
        printf("\tType: Block device\n");
    if(dtype == DT_LNK)
        printf("\tType: Symbolic link\n");
}

int main()
{
    DIR *d;
    char path[128];
    struct dirent *dir;

    printf("\n Please input directory path (e.g. \"/usr\").\n To get the list of files in current directory type \".\" (without the quotation marks).\n\t");
    scanf("%s", path);

    d = opendir(path);

    if (d) {
        while ((dir = readdir(d)) != NULL)
        {
            struct stat st;

            if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;

            printf("[%s]\n", dir->d_name); 
            // printf("\tType: %d\n", dir->d_type); 
            fileType(dir->d_type);
            printf("\tUserID | GroupID of owner: %hu | %hu\n", st.st_uid, st.st_gid);
            printf("\tFile Size: %8ld bytes\n", st.st_size);
            printf("\tPreferred I/O block size: %ld bytes\n", (long) st.st_blksize);
            printf("\tBlocks allocated: %lld\n",(long long) st.st_blocks);
            printf("\tLast status change: %s", ctime(&st.st_ctime));
            printf("\tLast file access: %s", ctime(&st.st_atime));
            printf("\tLast file modification: %s", ctime(&st.st_mtime));

        }
        closedir(d);
    } else {
        perror("opendir");
        return -1;
    }

    return 0;
}