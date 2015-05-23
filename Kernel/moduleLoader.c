#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <video.h>

static void loadModule(uint8_t ** module, void * targetModuleAddress);
static uint32_t readUint32(uint8_t ** address);

void loadModules(void * payloadStart, void ** targetModuleAddress)
{
	int i;
	uint8_t * currentModule = (uint8_t*)payloadStart;
	uint32_t moduleCount = readUint32(&currentModule);

	for (i = 0; i < moduleCount; i++)
		loadModule(&currentModule, targetModuleAddress[i]);
}

static void loadModule(uint8_t ** module, void * targetModuleAddress)
{
	uint32_t moduleSize = readUint32(module);

	video_write_string("  Will copy module at 0x");
	video_write_hex((uint64_t)*module);
	video_write_string(" to 0x");
	video_write_hex((uint64_t)targetModuleAddress);
	video_write_string(" (");
	video_write_hex(moduleSize);
	video_write_string(" bytes)");
	video_write_nl();

	memcpy(targetModuleAddress, *module, moduleSize);
	*module += moduleSize;

	video_write_pline("  [Done]");
}

static uint32_t readUint32(uint8_t ** address)
{
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
