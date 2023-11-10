#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

#include "kettlingur.h"

struct ps2 ps2;

#include "shorthand.h"



void init() {
	ps2.ee.ram = aligned_alloc(4096, EE_RAM_SIZE);
	ps2.ee.scratch = aligned_alloc(4096, EE_SCRATCHPAD_SIZE);
	ps2.ee.bios = aligned_alloc(4096, BIOS_SIZE);
	ps2.ee.regs = aligned_alloc(4096, EE_REGS_SIZE);

	ps2.vu0.code = NULL;
	ps2.vu0.data = NULL;
	ps2.vu1.code = NULL;
	ps2.vu1.data = NULL;

	ps2.iop.ram = aligned_alloc(4096, IOP_IOP_RAM_SIZE);
	ps2.iop.scratch = aligned_alloc(4096, 4096); //TODO: how big is this?
	ps2.iop.bios = ps2.ee.bios;
	ps2.iop.regs = aligned_alloc(4096, IOP_REGS_SIZE);
	ps2.gs.regs = aligned_alloc(4096, GS_REGS_SIZE);

	//values from pcsx2
	pc32u = 0xbfc00000;
	ps2.ee.cop0.config._u32[0] = 0x440;
	ps2.ee.cop0.status._u32[0] = 0x70400004; // COP0 enabled | BEV = 1 | TS = 1
	ps2.ee.cop0.prid._u32[0] = 0x00002e20; // PRevID = Revision ID, same as R5900
	ps2.ee.cop1.fcr[0]._u32[0] = 0x00002e00; // fpu Revision..
	ps2.ee.cop1.fcr[31]._u32[0] = 0x01000001; // fpu Status/Control
}

void fini() {
	free(ps2.gs.regs);
	free(ps2.iop.regs);
	//no free for ps2.iop.bios because it is also ps2.ee.bios
	free(ps2.iop.scratch);
	free(ps2.iop.ram);
	free(ps2.ee.regs);
	free(ps2.ee.bios);
	free(ps2.ee.scratch);
	free(ps2.ee.ram);
}

void loadbios(char *path) {
	FILE *fp;
	char *abspath = realpath(path, NULL);
	fp = fopen(abspath, "r");
	dieif(fp == nullptr, "could not open %s", abspath);
	fseek(fp, 0L, SEEK_END);
	size_t size = (size_t) ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	assert(size == 1024 * 1024 * 4);
	fread(ps2.ee.bios, size, 1, fp);
	ps2.ee.pc._u32[0] = 0xbfc00000;
	free(abspath);
	fclose(fp);
}



void loadelf(char *path) {
	FILE *fp;
	char *abspath = realpath(path, NULL);
	fp = fopen(abspath, "r");
	dieif(fp == NULL, "could not open %s", abspath);
	fseek(fp, 0L, SEEK_END);
	size_t size = (size_t) ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	u8 *buffer = malloc(size);

	fread(buffer, size, 1, fp);

	Elf32_Ehdr *elf_header = (Elf32_Ehdr *) buffer;
	Elf32_Phdr *phdr = (Elf32_Phdr *) ((uint8_t *) buffer + elf_header->e_phoff);
	printf("entry %x\n", elf_header->e_entry);
	printf("num pheaders %x\n", elf_header->e_phnum);
	printf("phoff %x\n", elf_header->e_phoff);
	printf("prog offset %x\n", phdr->p_offset);
	printf("paddr %x\n", phdr->p_paddr);
	printf("vaddr %x\n", phdr->p_vaddr);
	printf("filesz %x\n", phdr->p_filesz);
	printf("memsz %x\n", phdr->p_memsz);

	memset((u8 *) ps2.ee.ram + phdr->p_vaddr, 0, phdr->p_memsz);
	memcpy((u8 *) ps2.ee.ram + phdr->p_vaddr, (uint8_t *) buffer + phdr->p_offset, phdr->p_filesz);

	ps2.ee.pc._u32[0] = elf_header->e_entry;
	free(buffer);
	free(abspath);
	fclose(fp);
}

u32 memread32(u32 addr);
int main(int argc, char **argv) {
	init();
	//loadbios("../bios/ps2.bin");
	loadelf("../thirdparty/ps2autotests/tests/cpu/ee/alu.elf");
	//loadelf("../elfs/ee/helloworld");
	char dis[1024];
	reg32 a, b;

	step(50000);
	fini();
}