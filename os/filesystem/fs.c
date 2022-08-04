#include "fs.h"

struct superblock sb;

void read_sb()
{
    fseek(f, 0, SEEK_SET);
    fread(&sb, sizeof(struct superblock), 1, f);
}

void write_sb()
{
    fseek(f, 0, SEEK_SET);
    fwrite(&sb, sizeof(struct superblock), 1, f);
}

void init(char filesystem[])
{
    f = fopen(filesystem, "r+b");
    fseek(f, 0, SEEK_SET);
}

void format()
{
    read_sb();
    sb.count_free_blocks = COUNT_BLOCKS;
    sb.count_free_inode = COUNT_INODE;

    for(int i = 0; i < COUNT_BLOCKS; i++)
    {
        sb.blocks_map[i] = '0';
    }

    for(int i = 0; i < COUNT_INODE; i++)
    {
        sb.inodes[i].count_blocks = -1;
    }
    write_sb();
}

int exist_file(char filename[])
{
    
    int flag = -1;
    
    for(int i = 0; i < COUNT_INODE; i++)
    {
        if(sb.inodes[i].count_blocks == -1)
            continue;
        
        if(strcmp(filename, sb.inodes[i].filename) == 0)
        {
            flag = i;    
        }
        if(flag > -1)
            return flag;
    }
    return -1;
}

void get_block(int id_block, struct block* cur_block)
{
    fseek(f, 0, SEEK_SET);
    fseek(f, sizeof(struct superblock) + (id_block)*sizeof(struct block), SEEK_SET);
    fread(cur_block, sizeof(struct block), 1, f);
}

void write_block(int id_block, struct block* cur_block)
{
    fseek(f, 0, SEEK_SET);
    fseek(f, sizeof(struct superblock) + id_block*sizeof(struct block), SEEK_SET);
    fwrite(cur_block, sizeof(struct block), 1, f);
}

int init_block()
{
    struct block cur_block;
    cur_block.size = 0;
    cur_block.next = -1;
    sb.count_free_blocks--;

    for(int i =0 ; i < COUNT_BLOCKS; i++)
    {
        if(sb.blocks_map[i] == '0')
        {
            
            sb.blocks_map[i] = '1';
            write_block(i, &cur_block);
            return i;
        }
    }
}

void create_file(char filename[])
{
    read_sb();
    if(exist_file(filename) > -1)
    {
        printf("ERROR: file is already created\n");
        return;
    }
    if(sb.count_free_blocks <= 0 || sb.count_free_inode <= 0)
    {
        printf("Fs is fully\n");
        return;
    }
    struct inode *cur_inode;
    for(int i = 0; i < COUNT_INODE; i++)
    {
        if(sb.inodes[i].count_blocks == -1)
        {
            cur_inode = &sb.inodes[i];
            break;
        }
    }
    sb.count_free_inode--;
    strcpy(cur_inode->filename, filename);
    cur_inode->count_blocks = 1;
    cur_inode->first_data_block = init_block();

    
    write_sb();
}

void add_to_file(char filename[], char buf[])
{
    read_sb();
    
    
    int id_inode = exist_file(filename);
    if(id_inode == -1)
    {
        printf("ERROR: No file in fs\n");
        return;
    }
    
    struct inode *cur_inode = &sb.inodes[id_inode];
    struct block cur_block;

    get_block(cur_inode->first_data_block, &cur_block);

    

    int id = 0;
    int cur_id_block = cur_inode->first_data_block;
    int next_id_block = cur_block.next;
    
    while(next_id_block != -1)
    {
        cur_id_block = next_id_block;
        get_block(cur_id_block, &cur_block);
        next_id_block = cur_block.next;
    }

    while(id < strlen(buf))
    {
        while(id < strlen(buf) && cur_block.size < BLOCK_SIZE)
        {
            cur_block.data[cur_block.size] = buf[id];
            id++;
            cur_block.size++;
        }
        if(cur_block.size >= BLOCK_SIZE)
        {
            next_id_block = init_block();
            cur_block.next = next_id_block;
            cur_inode->count_blocks++;
            write_block(cur_id_block, &cur_block);
            cur_id_block = next_id_block;
            get_block(cur_id_block, &cur_block);
        }
    }
    write_block(cur_id_block, &cur_block);
    write_sb();
}

void read_file(char filename[])
{
    read_sb();
    

    int id_inode = exist_file(filename);
    if(id_inode == -1)
    {
        printf("ERROR: No file in fs\n");
    }

    struct inode *cur_inode = &sb.inodes[id_inode];
    struct block cur_block;

    get_block(cur_inode->first_data_block, &cur_block);

    int id = 0;
    int cur_id_block = cur_inode->first_data_block;
    int next_id_block = cur_block.next;

    
    do{
        next_id_block = cur_block.next;
        for(id = 0; id < cur_block.size; id++)
        {
            printf("%c", cur_block.data[id]);
        }
        cur_id_block = next_id_block;
        get_block(cur_id_block, &cur_block);
        
    }while(next_id_block >= 0);

}

void clear_file(char filename[])
{
    read_sb();
    
    
    int id_inode = exist_file(filename);
    if(id_inode == -1)
    {
        printf("ERROR: No file in fs\n");
        return;
    }
    
    struct inode *cur_inode = &sb.inodes[id_inode];
    cur_inode->count_blocks = 1;

    struct block cur_block;

    get_block(cur_inode->first_data_block, &cur_block);

    int id = 0;
    int cur_id_block = cur_inode->first_data_block;
    int next_id_block = cur_block.next;
    cur_block.size = 0;
    cur_block.next = -1;
    write_block(cur_id_block, &cur_block);
    cur_id_block = next_id_block;
    


    while(cur_id_block != -1)
    {
        get_block(cur_id_block, &cur_block);
        next_id_block = cur_block.next;
        cur_block.size = 0;
        sb.blocks_map[cur_id_block] = '0';
        cur_block.next = -1;
        sb.count_free_blocks++;
        write_block(cur_id_block, &cur_block);
        cur_id_block = next_id_block;

    }
    write_sb();
}

void rm_file(char filename[])
{
    read_sb();

    
    int id_inode = exist_file(filename);
    if(id_inode == -1)
    {
        printf("ERROR: No file in fs\n");
        return;
    }
    clear_file(filename);

    sb.inodes[id_inode].count_blocks = -1;
    sb.count_free_blocks++;
    sb.count_free_inode++;

    int cur_id_block = sb.inodes[id_inode].first_data_block;
    sb.blocks_map[cur_id_block] = '0';
    struct block cur_block;

    get_block(cur_id_block, &cur_block);
    cur_block.size = 0;
    cur_block.next = -1;
    sb.inodes[id_inode].first_data_block = -1;

    write_block(cur_id_block, &cur_block);
    write_sb();
}

void write_to_file(char filename[], char buf[])
{
    read_sb();

    int id_inode = exist_file(filename);
    if(id_inode == -1)
    {
        printf("ERROR: No file in fs\n");
        return;
    }

    clear_file(filename);

    add_to_file(filename, buf);
    
    write_sb();
}

int get_file_size(char filename[])
{
    read_sb();
    int res = 0;
    int id_inode = exist_file(filename);
    struct inode *cur_inode = &sb.inodes[id_inode];
    res = (cur_inode->count_blocks - 1)*BLOCK_SIZE;
    struct block cur_block;
    get_block(cur_inode->first_data_block, &cur_block);
    int cur_id_block = cur_inode->first_data_block;

    
    while(cur_block.next >= 0)
    {
        get_block(cur_id_block, &cur_block);
        cur_id_block = cur_block.next;

    }
    res += cur_block.size;
    write_sb();
    return res;
}

void get_file_list()
{
    read_sb();
    int id = 0;
    for(int i = 0; i < COUNT_INODE; i++)
    {
        if(sb.inodes[i].count_blocks > -1)
        {
            id++;
            int size = get_file_size(sb.inodes[i].filename);
            printf("%2d) %12s %d Bytes\n",id, sb.inodes[i].filename, size);
        }
    }
    if(id == 0)
        printf("LOG: there empty\n");
}