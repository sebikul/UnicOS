#include <stdint.h>
#include "filesystem.h"
#include "string.h"
#include "kernel.h"
#include "mem.h"

static device_t *rootdevice = NULL;

typedef struct {
	device_t *dev;
	char* path;
} mount;

static mount mountinfo[MAX_FS_CHILDS] = {{0}};

static directory_t* fs_traverse(const char* path) {

	directory_t *dir;
	char *pos = path;

	kdebug("Traversing directory: ");
	_kdebug(path);
	kdebug_nl();

	if (rootdevice == NULL) {
		return NULL;
	}

	dir = rootdevice->rootdir;
	pos++;

	//Pide recorrer /
	if (*pos == '\0') {
		kdebug("Traversing /\n");
		return dir;
	}

	while (TRUE) {

		uint32_t i;
		char* curr = pos;

		kdebug("Current directory: ");
		_kdebug(curr);
		kdebug_nl();

		while (*pos != '\0' && *pos != '/') {
			pos++;
		}

		for (i = 0; i < MAX_FS_CHILDS; i++) {

			if (dir->childs[i] != NULL) {
				kdebug("Comparando: ");
				_kdebug(dir->childs[i]->name);
				_kdebug(" con ");
				_kdebug(curr);
				_kdebug(" cant. de caracteres: ");
				kdebug_base(pos - curr, 10);
				kdebug_nl();
			}

			if (dir->childs[i] != NULL && strncmp(dir->childs[i]->name, curr, pos - curr) == 0) {
				kdebug("Encontrado!\n");
				dir = dir->childs[i];
				break;
			}
		}

		if (i == MAX_FS_CHILDS) {
			kdebug("Directory not found\n");
			//TODO errno not found
			break;
		}

		if (*pos == '\0') {
			kdebug("Directory found!\n");
			//El nombre coincide y llegamos al final de la cadena
			return dir;
		}
		pos++; //Salteamos la barra
	}

	return NULL;
}

void fs_mount(device_t *dev, const char* mountpoint) {

	uint32_t i;

	if (rootdevice == NULL && strcmp(mountpoint, "/") != 0) {
		//TODO errno
		return;
	}

	kdebug("Mounting device with name: ");
	_kdebug(dev->name);
	_kdebug(" at ");
	_kdebug(mountpoint);
	kdebug_nl();

	if (rootdevice == NULL) {
		kdebug("Null root device, mounting as root.\n");
		rootdevice = dev;
	} else {

		for (i = 0; i < MAX_FS_CHILDS; i++) {
			if (mountinfo[i].dev == dev) {
				kdebug("El dispositivo ya se encuentra montado!\n");
				return;
			}
		}

		directory_t *dir = fs_traverse(mountpoint);

		for (i = 0; i < MAX_FS_CHILDS; i++) {
			if (dir->childs[i] == NULL) {
				dir->childs[i] = dev->rootdir;
				kdebug("Mounting device!\n");
				break;
			}
		}

		if (i == MAX_FS_CHILDS) {
			//TODO errno not found
			return;
		}
	}

	//Guardamos la informacion del volumen
	for (i = 0; i < MAX_FS_CHILDS; i++) {
		if (mountinfo[i].dev == NULL) {
			mountinfo[i].dev = dev;
			mountinfo[i].path = mountpoint;
			break;
		}
	}
}

void fs_unmount(device_t *dev) {

	char* mountpoint;
	int32_t mount;
	int32_t child;

	for (mount = 0; mount < MAX_FS_CHILDS; mount++) {
		if (mountinfo[mount].dev == dev) {
			mountpoint = mountinfo[mount].path;
			break;
		}
	}

	kdebug("Unmounting device with name: ");
	_kdebug(dev->name);
	_kdebug(" at ");
	_kdebug(mountpoint);
	kdebug_nl();

	directory_t *dir = fs_traverse(mountpoint);

	for (child = 0; child < MAX_FS_CHILDS; child++) {
		if (dir->childs[child] == dev->rootdir) {
			mountinfo[mount].dev = NULL;
			mountinfo[mount].path = NULL;
			dir->childs[child] = NULL;
			break;
		}
	}
}

static void dumpdir(directory_t *dir, int level) {

	for (uint32_t i = 0; i < MAX_FS_CHILDS; i++) {
		if (dir->childs[i] != NULL) {
			int j = level;

			kdebug("");

			while (j > 0) {
				_kdebug("    ");
				video_write_string(video_current_console(), "    ");
				j--;
			}

			_kdebug("|-");
			video_write_string(video_current_console(), "|-");

			if (dir->device != dir->childs[i]->device) {
				video_write_string(video_current_console(), "!!");
				_kdebug("!!");
			}

			video_write_string(video_current_console(), dir->childs[i]->name);
			_kdebug(dir->childs[i]->name);

			if (dir->device != dir->childs[i]->device) {

				video_write_string(video_current_console(), " (");
				video_write_string(video_current_console(), dir->childs[i]->device->name);
				video_write_string(video_current_console(), " )");

				_kdebug(" (");
				_kdebug(dir->childs[i]->device->name);
				_kdebug(" )");
			}

			kdebug_nl();
			video_write_nl(video_current_console());

			dumpdir(dir->childs[i], level + 1);
		}
	}

	for (uint32_t i = 0; i < MAX_FS_CHILDS; i++) {
		if (dir->leaves[i] != NULL) {
			int j = level;
			kdebug("");

			while (j > 0) {
				_kdebug("    ");
				video_write_string(video_current_console(), "    ");
				j--;
			}

			_kdebug("|~");
			video_write_string(video_current_console(), "|~");

			_kdebug(dir->leaves[i]->name);
			video_write_string(video_current_console(), dir->leaves[i]->name);

			kdebug_nl();
			video_write_nl(video_current_console());
		}
	}
}

static char* fs_basedir(const char* path) {

	uint32_t len = strlen(path);
	char* base = malloc(len + 1);

	strcpy(base, path);

	kdebug("Parsing basedir of: ");
	_kdebug(base);
	kdebug_nl();

	char* aux = base + len;

	while (*aux != '/' && aux > base) {
		aux--;
	}

	if (aux == base) {
		base[1] = '\0';
	} else {
		*aux = '\0';
	}

	kdebug("Returning: ");
	_kdebug(base);
	kdebug_nl();

	return base;
}

static char* fs_filename(const char* path) {

	char* name = path + strlen(path);

	while (*name != '/' && name != path) {
		name--;
	}

	return name + 1;
}

directory_t* fs_mkdir(const char* path) {

	uint32_t i = 0;

	if (rootdevice == NULL) {
		return NULL;
	}

	char* parent = fs_basedir(path);

	directory_t *parentdir = fs_traverse(parent);
	char* dirname = fs_filename(path);

	for (; i < MAX_FS_CHILDS; i++) {

		if (parentdir->childs[i] == NULL) {
			kdebug("Creating firectory: ");
			_kdebug(dirname);
			kdebug_nl();

			directory_t* dir = malloc(sizeof(directory_t));
			dir->device = parentdir->device;

			dir->name = malloc(strlen(dirname) + 1);
			strcpy(dir->name, dirname);

			for (uint32_t j = 0; j < MAX_FS_CHILDS; j++) {
				dir->leaves[j] = NULL;
				dir->childs[j] = NULL;
			}

			parentdir->childs[i] = dir;

			return dir;
		}
	}

	if (i == MAX_FS_CHILDS) {
		//TODO no space
		return NULL;
	}

	return NULL;
}

file_t* fs_open(const char *path, uint64_t flags) {

	uint32_t i = 0;

	if (rootdevice == NULL) {
		return NULL;
	}

	char* parent = fs_basedir(path);

	directory_t *dir = fs_traverse(parent);

	if (dir == NULL) {
		//todo errno
		kdebug("File not found!\n");
		return NULL;
	}

	kdebug("Searching for file in: ");
	_kdebug(parent);
	kdebug_nl();

	free(parent);

	char* filename = fs_filename(path);

	if (flags & O_CREAT) {

		for (; i < MAX_FS_CHILDS; i++) {

			if (dir->leaves[i] == NULL) {
				kdebug("Creating file: ");
				_kdebug(filename);
				_kdebug(" in directory.");
				kdebug_nl();

				file_t* file = malloc(sizeof(file_t));
				file->device = dir->device;

				file->name = malloc(strlen(filename) + 1);
				strcpy(file->name, filename);

				file->size = 0;
				file->start = NULL;

				dir->leaves[i] = file;

				return dir->leaves[i];
			}
		}

		if (i == MAX_FS_CHILDS) {
			//TODO no space
			return NULL;
		}

	} else {
		for (; i < MAX_FS_CHILDS; i++) {

			kdebug("Looking for: ");
			_kdebug(filename);
			_kdebug(" in directory.");
			kdebug_nl();

			if (strcmp(dir->leaves[i]->name, filename) == 0) {
				kdebug("Archivo encontrado!\n");
				return dir->leaves[i];
			}
		}


		if (i == MAX_FS_CHILDS) {
			//TODO not found
			return NULL;
		}
	}

	return NULL;
}

directory_t* fs_opendir(const char* path) {

	if (rootdevice == NULL) {
		return NULL;
	}

	char* tmp = path;

	tmp += strlen(path) - 1;

	if (*tmp == '/') {
		*tmp = '\0';
	}

	return fs_traverse(path);
}

int32_t fs_read(file_t *file, char* buf, uint32_t size, uint32_t offset) {

	return file->device->funcs.read(file, buf, size, offset);
}
int32_t fs_write(file_t *file, const char* data, uint32_t size, uint32_t offset) {

	return file->device->funcs.write(file, data, size, offset);
}

void fs_dump() {
	kdebug("Dumping filesystem!\n");

	dumpdir(rootdevice->rootdir, 0);
}

void fs_init() {
	directory_t* rootdir = malloc(sizeof(directory_t));

	device_t* rootdev = ramfs_init(rootdir, "rootfs");

	rootdir->name = "root";
	rootdir->device = rootdev;

	for (uint32_t j = 0; j < MAX_FS_CHILDS; j++) {
		rootdir->leaves[j] = NULL;
		rootdir->childs[j] = NULL;
	}

	fs_mount(rootdev, "/");
}

void fs_test() {

	kdebug("Testing filesystem\n");

	fs_init();

	fs_mkdir("/dir1");
	fs_mkdir("/dir1/dir2");
	fs_mkdir("/dir1/dir2/dir3");
	fs_mkdir("/dir1/dir4");
	fs_mkdir("/dir2");
	fs_mkdir("/dir2/dir5");

	file_t* file1 = fs_open("/file1", O_CREAT);
	fs_open("/dir2/file2", O_CREAT);
	fs_open("/dir2/dir5/file3", O_CREAT);

	fs_dump();

	char data[] = "hola que tal";
	char response[50];

	int w =	fs_write(file1, data, strlen(data) + 1, 0);

	kdebug("Escrito en el archivo: '");
	_kdebug(data);
	_kdebug("' tamano: ");
	kdebug_base(w, 10);
	kdebug_nl();

	int r = fs_read(file1, response, sizeof(response), 0);

	kdebug("Leido del archivo: '");
	_kdebug(response);
	_kdebug("' tamano: ");
	kdebug_base(r, 10);
	kdebug_nl();

	directory_t* newdir = malloc(sizeof(directory_t));

	device_t* newdev = ramfs_init(newdir, "otro mount");
	newdir->name = "newroot";
	newdir->device = newdev;

	for (uint32_t j = 0; j < MAX_FS_CHILDS; j++) {
		newdir->leaves[j] = NULL;
		newdir->childs[j] = NULL;
	}

	fs_mount(newdev, "/dir2/dir5");

	fs_dump();

	w =	fs_write(file1, data, strlen(data) + 1, w - 1);

	kdebug("Escrito en el archivo: '");
	_kdebug(data);
	_kdebug("' tamano: ");
	kdebug_base(w, 10);
	kdebug_nl();

	r = fs_read(file1, response, 50, 0);

	kdebug("Leido del archivo: '");
	_kdebug(response);
	_kdebug("' tamano: ");
	kdebug_base(r, 10);
	kdebug_nl();

	directory_t* dir = fs_opendir("/dir2/dir5");

	if (dir != NULL) {
		kdebug("Directory found!\n");
	} else {
		kdebug("Directory NOT found!\n");
	}

	fs_mkdir("/dir2/dir5/newroot/holaaa");

	// dirpool[0].name = "root";

	// dirpool[0].childs[0] = &(dirpool[1]);
	// dirpool[0].childs[1] = &(dirpool[2]);
	// dirpool[0].childs[2] = &(dirpool[5]);

	// dirpool[1].childs[0] = &(dirpool[3]);
	// dirpool[3].childs[0] = &(dirpool[4]);

	// dirpool[1].name = "dir1";
	// dirpool[2].name = "dir2";
	// dirpool[3].name = "dir3";
	// dirpool[4].name = "dir4";
	// dirpool[5].name = "dir5";

	// dirpool[0].leaves[0] = &(filepool[0]);
	// dirpool[4].leaves[0] = &(filepool[1]);
	// dirpool[4].leaves[1] = &(filepool[2]);
	// dirpool[3].leaves[0] = &(filepool[3]);


	// filepool[0].name = "file1";
	// filepool[1].name = "file2";
	// filepool[2].name = "file3";
	// filepool[3].name = "file4";
	// filepool[3].device = newdev;

	// rootdev = ramfs_init(&(dirpool[0]), "rootfs");

	// fs_mount(rootdev, "/");

	// dirpool[6].name = "dir6";
	// dirpool[7].name = "dir7";

	// dirpool[6].childs[0] = &(dirpool[7]);
	// dirpool[6].leaves[0] = &(filepool[5]);
	// filepool[5].name = "file5";

	// newdev = ramfs_init(&(dirpool[6]), "New Device");

	// dumpfs();

	// fs_mount(newdev, "/dir1/dir3");
	// fs_mount(newdev, "/dir1/dir4");

	// dumpfs();

	// file_t *file1 = fs_open("/file1", 0);
	// file1->start = malloc(50);
	// file1->size = 50;
	// file1->device = rootdev;

	// if (file1 == &filepool[0]) {
	// 	kdebug("Archivo corresponde\n");
	// }

	// file_t *file2 = fs_open("/dir1/dir3/file4", 0);

	// if (file2 == &filepool[3]) {
	// 	kdebug("Archivo corresponde\n");
	// }

	// char data[] = "hola que tal";
	// char response[50];

	// int w =	fs_write(file1, data, strlen(data) + 1, 0);

	// kdebug("Escrito en el archivo: '");
	// _kdebug(data);
	// _kdebug("' tamano: ");
	// kdebug_base(w, 10);
	// kdebug_nl();

	// int r = fs_read(file1, response, sizeof(response), 0);

	// kdebug("Leido del archivo: '");
	// _kdebug(response);
	// _kdebug("' tamano: ");
	// kdebug_base(r, 10);
	// kdebug_nl();

	// fs_unmount(&newdev);

	// dumpfs();

}
