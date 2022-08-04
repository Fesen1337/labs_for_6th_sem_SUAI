#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT_BLOCKS 1000
#define COUNT_INODE 50
#define FILENAME_SIZE 12
#define BLOCK_SIZE 992

FILE *f;

struct inode
{
    int first_data_block;
    int count_blocks;
    char filename[FILENAME_SIZE];
};

struct superblock
{
    int count_free_blocks;
    int count_free_inode; 
    char blocks_map[COUNT_BLOCKS];
    struct inode inodes[COUNT_INODE];
};

struct block
{
    int size;
    int next;
    char data[BLOCK_SIZE];
};

#endif