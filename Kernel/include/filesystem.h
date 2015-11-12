#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_FS_CHILDS 16

typedef struct device device_t;
typedef struct directory directory_t;

typedef struct {
	char 			*name;
	uint32_t 		start;
	uint32_t 		size;
} file_t;

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

file_t* fs_open(const char *path);
int32_t fs_read(file_t *file, char* buf, uint32_t size);
int32_t fs_write(file_t *file, const char* data, uint32_t size);
void fs_close(file_t *file);

void fs_test();

#endif