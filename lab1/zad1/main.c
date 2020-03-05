#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char **table = NULL;
unsigned int initializated = 0;
unsigned int table_size = 0;
char *tmp_file = NULL;
// 1.
int init_table(unsigned int size)
{
    if (initializated == 1)
    {
        fprintf(stderr, "already initializated");
        return -1;
    }

    table = calloc(size, sizeof(char *));
    table_size = size;
    initializated = 1;
    tmp_file = "tmp.txt";
    return 0;
}
// 3.
int diff_files(char *file1, char *file2)
{
    char buf[256];
    snprintf(buf, sizeof buf, "diff %s %s > %s", file1, file2, tmp_file);
    int find_status = system(buf);
    if (find_status != 0)
        return find_status;

    return 0;
}

int get_file_size(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    rewind(fp);
    return sz;
}

int load_file(char **buffer, char *file_path)
{
    FILE *f = fopen(file_path, "rb");
    if (!f)
    {
        fprintf(stderr, "Problem with reading file");
        return -1;
    }
    long file_size = get_file_size(f);
    *buffer = calloc(file_size, sizeof(char));
    fread(*buffer, sizeof(char), file_size, f);
    fclose(f);
    return 0;
}

int file_to_array(char *file_name)
{
    for (int i = 0; i < table_size; i++)
    {
        if (table[i] == NULL)
        {
            int load_status = load_file(&table[i], file_name);
            if (load_status < 0)
            {
                fprintf(stderr, "can't load into array\n");
                return load_status;
            }
            return i;
        }
    }
    fprintf(stderr, "can't load into array, array is full\n");
    return -1;
}
// 4.
int tmp_to_array()
{
    return file_to_array(tmp_file);
}

char *get_block(int idx)
{
    return table[idx];
}
// 6.
int delete_block(int idx)
{
    free(table[idx]);
    table[idx] = NULL;
    return 0;
}

void delete_array()
{
    for (int i = 0; i < table_size; ++i)
    {
        delete_block(i);
    }
    free(table);
}
// 5.
int get_operations_count(int idx)
{
    char *content = get_block(idx);
    char *p;
    int counter = 0;
    p = content;
    while (*p != '\0')
    {
        if (*p == '\n' && (*(++p) > '0' && *(p) < '9'))
            counter++;
        p++;
    }

    return counter + 1;
}
// 7.
void delete_operation_from_block(int block_idx, int operation_idx)
{
    char **content = &table[block_idx];

    char *p = *content;
    size_t len = 0;
    int counter = 0;
    int exist = 0;
    char *first = NULL;
    int line_count = 0;
    int old_counter = 0;
    while (*p != '\0')
    {

        if (*p == '\n' && (*(++p) > '0' && *(p) < '9'))
            counter++;
        if (counter == operation_idx)
        {

            if (exist == 0)
            {
                exist = 1;
                first = p;
                first;
            }

            len++;
            old_counter = counter;
        }

        p++;
        if (old_counter == counter)
        {
            if (*p == '\n')
            {
                len++;
            }
        }
        old_counter = -1;
    }
    if (counter + 1 <= operation_idx)
    {
        fprintf(stderr, "Index doesn't exist\n");
        return;
    }

    memmove(first, first + len, p - first + len);
}

int main()
{

    init_table(10);
    diff_files("a.txt", "b.txt");
    tmp_to_array();

    get_operations_count(0);
    // delete_operation_from_block(0, 2);
    for (int i = 0; i < 10; ++i)
    {
        int idx = i;
        printf("idx: %d\n", idx);
        printf("block: %s\n", get_block(idx));
        delete_block(idx);
    }
    return 0;
}