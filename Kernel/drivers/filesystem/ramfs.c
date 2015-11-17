#include <stdint.h>
#include "filesystem.h"
#include "string.h"
#include "kernel.h"
#include "mem.h"

static int32_t ramfs_read(file_t *file, char* buf, uint32_t size, uint32_t offset) {

	uint32_t i = 0;

	char* data = file->start;

	kdebug("Reading ");
	kdebug_base(size, 10);
	_kdebug(" bytes from file starting at ");
	kdebug_base(offset, 10);
	kdebug_nl();

	data += offset;

	for (; i < size && i < file->size; i++) {
		buf[i] = data[i];
	}

	kdebug("Leido: ");
	_kdebug(buf);
	kdebug_nl();

	return i;
}

static int32_t ramfs_write(file_t *file, const char* data, uint32_t size, uint32_t offset) {
	uint32_t i = 0;

	if (file->start == NULL || (size + offset) > file->size) {
		kdebug("File size exceded for writting, reallocating!\n");
		void* newstart = malloc(size + offset);
		int newsize = size + offset;

		if (file->start != NULL) {
			kdebug("Copying old data!\n");
			memcpy(newstart, file->start, file->size);
			free(file->start);
		}

		file->start = newstart;
		file->size = newsize;
	}

	char* buf = file->start;

	buf += offset;

	for (; i < size && i < file->size; i++) {
		buf[i] = data[i];
	}

	return i;
}

device_t* ramfs_init(directory_t *rootdir, char *name) {

	device_t* dev = malloc(sizeof(device_t));

	dev->name = name;
	dev->rootdir = rootdir;
	dev->funcs.read = ramfs_read;
	dev->funcs.write = ramfs_write;

	return dev;
}
