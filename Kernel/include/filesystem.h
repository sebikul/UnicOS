#include "types.h"

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

typedef struct device device_t;
typedef struct directory directory_t;

struct file{
	char* 			name;
	void* 			start;
	uint64_t 		size;
};

struct directory {
	char 			*name;
	file_t 			*leaves[MAX_FS_CHILDS];
	directory_t 	*childs[MAX_FS_CHILDS];
} ;

struct device {
	directory_t 	*rootdir;
	char 			*name;
};

void fs_mount(device_t *dev, const char* mountpoint);
void fs_unmount(device_t *dev);

file_t* fs_open(const char *path, uint64_t flags);
int32_t fs_read(file_t *file, char* buf, uint32_t size, uint32_t offset);
int32_t fs_write(file_t *file, const char* data, uint32_t size, uint32_t offset);
void fs_close(file_t *file);

void fs_test();

#endif