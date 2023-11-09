#pragma once

#pragma once

#include <stdint.h>

typedef uint16_t Elf32_Half;
typedef uint16_t Elf64_Half;

typedef uint32_t Elf32_Word;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf64_Word;
typedef int32_t Elf64_Sword;

typedef uint64_t Elf32_Xword;
typedef int64_t Elf32_Sxword;
typedef uint64_t Elf64_Xword;
typedef int64_t Elf64_Sxword;

typedef uint32_t Elf32_Addr;
typedef uint64_t Elf64_Addr;

typedef uint32_t Elf32_Off;
typedef uint64_t Elf64_Off;

typedef uint16_t Elf32_Section;
typedef uint16_t Elf64_Section;

typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;

#define EI_NIDENT (16)

typedef struct {
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
} Elf32_Ehdr;

typedef struct {
	unsigned char e_ident[EI_NIDENT];
	Elf64_Half e_type;
	Elf64_Half e_machine;
	Elf64_Word e_version;
	Elf64_Addr e_entry;
	Elf64_Off e_phoff;
	Elf64_Off e_shoff;
	Elf64_Word e_flags;
	Elf64_Half e_ehsize;
	Elf64_Half e_phentsize;
	Elf64_Half e_phnum;
	Elf64_Half e_shentsize;
	Elf64_Half e_shnum;
	Elf64_Half e_shstrndx;
} Elf64_Ehdr;

#define EI_MAG0 0
#define ELFMAG0 0x7f

#define EI_MAG1 1
#define ELFMAG1 'E'

#define EI_MAG2 2
#define ELFMAG2 'L'

#define EI_MAG3 3
#define ELFMAG3 'F'

#define ELFMAG "\177ELF"
#define SELFMAG 4

#define EI_CLASS 4
#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2
#define ELFCLASSNUM 3

#define EI_DATA 5
#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2
#define ELFDATANUM 3

#define EI_VERSION 6

#define EI_OSABI 7
#define ELFOSABI_NONE 0
#define ELFOSABI_SYSV 0
#define ELFOSABI_HPUX 1
#define ELFOSABI_NETBSD 2
#define ELFOSABI_LINUX 3
#define ELFOSABI_GNU 3
#define ELFOSABI_SOLARIS 6
#define ELFOSABI_AIX 7
#define ELFOSABI_IRIX 8
#define ELFOSABI_FREEBSD 9
#define ELFOSABI_TRU64 10
#define ELFOSABI_MODESTO 11
#define ELFOSABI_OPENBSD 12
#define ELFOSABI_ARM 97
#define ELFOSABI_STANDALONE 255

#define EI_ABIVERSION 8

#define EI_PAD 9

#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4
#define ET_NUM 5
#define ET_LOOS 0xfe00
#define ET_HIOS 0xfeff
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

#define EM_NONE 0
#define EM_M32 1
#define EM_SPARC 2
#define EM_386 3
#define EM_68K 4
#define EM_88K 5
#define EM_860 7
#define EM_MIPS 8
#define EM_S370 9
#define EM_MIPS_RS3_LE 10

#define EM_PARISC 15
#define EM_VPP500 17
#define EM_SPARC32PLUS 18
#define EM_960 19
#define EM_PPC 20
#define EM_PPC64 21
#define EM_S390 22

#define EM_V800 36
#define EM_FR20 37
#define EM_RH32 38
#define EM_RCE 39
#define EM_ARM 40
#define EM_FAKE_ALPHA 41
#define EM_SH 42
#define EM_SPARCV9 43
#define EM_TRICORE 44
#define EM_ARC 45
#define EM_H8_300 46
#define EM_H8_300H 47
#define EM_H8S 48
#define EM_H8_500 49
#define EM_IA_64 50
#define EM_MIPS_X 51
#define EM_COLDFIRE 52
#define EM_68HC12 53
#define EM_MMA 54
#define EM_PCP 55
#define EM_NCPU 56
#define EM_NDR1 57
#define EM_STARCORE 58
#define EM_ME16 59
#define EM_ST100 60
#define EM_TINYJ 61
#define EM_X86_64 62
#define EM_PDSP 63

#define EM_FX66 66
#define EM_ST9PLUS 67
#define EM_ST7 68
#define EM_68HC16 69
#define EM_68HC11 70
#define EM_68HC08 71
#define EM_68HC05 72
#define EM_SVX 73
#define EM_ST19 74
#define EM_VAX 75
#define EM_CRIS 76
#define EM_JAVELIN 77
#define EM_FIREPATH 78
#define EM_ZSP 79
#define EM_MMIX 80
#define EM_HUANY 81
#define EM_PRISM 82
#define EM_AVR 83
#define EM_FR30 84
#define EM_D10V 85
#define EM_D30V 86
#define EM_V850 87
#define EM_M32R 88
#define EM_MN10300 89
#define EM_MN10200 90
#define EM_PJ 91
#define EM_OPENRISC 92
#define EM_ARC_A5 93
#define EM_XTENSA 94
#define EM_AARCH64 183
#define EM_TILEPRO 188
#define EM_MICROBLAZE 189
#define EM_TILEGX 191
#define EM_NUM 192
#define EM_ALPHA 0x9026

#define EV_NONE 0
#define EV_CURRENT 1
#define EV_NUM 2

typedef struct {
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} Elf32_Shdr;

typedef struct {
	Elf64_Word sh_name;
	Elf64_Word sh_type;
	Elf64_Xword sh_flags;
	Elf64_Addr sh_addr;
	Elf64_Off sh_offset;
	Elf64_Xword sh_size;
	Elf64_Word sh_link;
	Elf64_Word sh_info;
	Elf64_Xword sh_addralign;
	Elf64_Xword sh_entsize;
} Elf64_Shdr;

#define SHN_UNDEF 0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC 0xff00
#define SHN_BEFORE 0xff00

#define SHN_AFTER 0xff01

#define SHN_HIPROC 0xff1f
#define SHN_LOOS 0xff20
#define SHN_HIOS 0xff3f
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
#define SHN_XINDEX 0xffff
#define SHN_HIRESERVE 0xffff

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_INIT_ARRAY 14
#define SHT_FINI_ARRAY 15
#define SHT_PREINIT_ARRAY 16
#define SHT_GROUP 17
#define SHT_SYMTAB_SHNDX 18
#define SHT_NUM 19
#define SHT_LOOS 0x60000000
#define SHT_GNU_ATTRIBUTES 0x6ffffff5
#define SHT_GNU_HASH 0x6ffffff6
#define SHT_GNU_LIBLIST 0x6ffffff7
#define SHT_CHECKSUM 0x6ffffff8
#define SHT_LOSUNW 0x6ffffffa
#define SHT_SUNW_move 0x6ffffffa
#define SHT_SUNW_COMDAT 0x6ffffffb
#define SHT_SUNW_syminfo 0x6ffffffc
#define SHT_GNU_verdef 0x6ffffffd
#define SHT_GNU_verneed 0x6ffffffe
#define SHT_GNU_versym 0x6fffffff
#define SHT_HISUNW 0x6fffffff
#define SHT_HIOS 0x6fffffff
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0x8fffffff

#define SHF_WRITE (1 << 0)
#define SHF_ALLOC (1 << 1)
#define SHF_EXECINSTR (1 << 2)
#define SHF_MERGE (1 << 4)
#define SHF_STRINGS (1 << 5)
#define SHF_INFO_LINK (1 << 6)
#define SHF_LINK_ORDER (1 << 7)
#define SHF_OS_NONCONFORMING (1 << 8)

#define SHF_GROUP (1 << 9)
#define SHF_TLS (1 << 10)
#define SHF_MASKOS 0x0ff00000
#define SHF_MASKPROC 0xf0000000
#define SHF_ORDERED (1 << 30)
#define SHF_EXCLUDE (1 << 31)

#define GRP_COMDAT 0x1

typedef struct {
	Elf32_Word st_name;
	Elf32_Addr st_value;
	Elf32_Word st_size;
	unsigned char st_info;
	unsigned char st_other;
	Elf32_Section st_shndx;
} Elf32_Sym;

typedef struct {
	Elf64_Word st_name;
	unsigned char st_info;
	unsigned char st_other;
	Elf64_Section st_shndx;
	Elf64_Addr st_value;
	Elf64_Xword st_size;
} Elf64_Sym;

typedef struct {
	Elf32_Half si_boundto;
	Elf32_Half si_flags;
} Elf32_Syminfo;

typedef struct {
	Elf64_Half si_boundto;
	Elf64_Half si_flags;
} Elf64_Syminfo;

#define SYMINFO_BT_SELF 0xffff
#define SYMINFO_BT_PARENT 0xfffe
#define SYMINFO_BT_LOWRESERVE 0xff00

#define SYMINFO_FLG_DIRECT 0x0001
#define SYMINFO_FLG_PASSTHRU 0x0002
#define SYMINFO_FLG_COPY 0x0004
#define SYMINFO_FLG_LAZYLOAD 0x0008

#define SYMINFO_NONE 0
#define SYMINFO_CURRENT 1
#define SYMINFO_NUM 2

#define ELF32_ST_BIND(val) (((unsigned char)(val)) >> 4)
#define ELF32_ST_TYPE(val) ((val)&0xf)
#define ELF32_ST_INFO(bind, type) (((bind) << 4) + ((type)&0xf))

#define ELF64_ST_BIND(val) ELF32_ST_BIND(val)
#define ELF64_ST_TYPE(val) ELF32_ST_TYPE(val)
#define ELF64_ST_INFO(bind, type) ELF32_ST_INFO((bind), (type))

#define STB_LOCAL 0
#define STB_GLOBAL 1
#define STB_WEAK 2
#define STB_NUM 3
#define STB_LOOS 10
#define STB_GNU_UNIQUE 10
#define STB_HIOS 12
#define STB_LOPROC 13
#define STB_HIPROC 15

#define STT_NOTYPE 0
#define STT_OBJECT 1
#define STT_FUNC 2
#define STT_SECTION 3
#define STT_FILE 4
#define STT_COMMON 5
#define STT_TLS 6
#define STT_NUM 7
#define STT_LOOS 10
#define STT_GNU_IFUNC 10
#define STT_HIOS 12
#define STT_LOPROC 13
#define STT_HIPROC 15

#define STN_UNDEF 0

#define ELF32_ST_VISIBILITY(o) ((o)&0x03)
#define ELF64_ST_VISIBILITY(o) ELF32_ST_VISIBILITY(o)

#define STV_DEFAULT 0
#define STV_INTERNAL 1
#define STV_HIDDEN 2
#define STV_PROTECTED 3

typedef struct {
	Elf32_Addr r_offset;
	Elf32_Word r_info;
} Elf32_Rel;

typedef struct {
	Elf64_Addr r_offset;
	Elf64_Xword r_info;
} Elf64_Rel;

typedef struct {
	Elf32_Addr r_offset;
	Elf32_Word r_info;
	Elf32_Sword r_addend;
} Elf32_Rela;

typedef struct {
	Elf64_Addr r_offset;
	Elf64_Xword r_info;
	Elf64_Sxword r_addend;
} Elf64_Rela;

#define ELF32_R_SYM(val) ((val) >> 8)
#define ELF32_R_TYPE(val) ((val)&0xff)
#define ELF32_R_INFO(sym, type) (((sym) << 8) + ((type)&0xff))

#define ELF64_R_SYM(i) ((i) >> 32)
#define ELF64_R_TYPE(i) ((i)&0xffffffff)
#define ELF64_R_INFO(sym, type) ((((Elf64_Xword)(sym)) << 32) + (type))

typedef struct {
	Elf32_Word p_type;
	Elf32_Off p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
} Elf32_Phdr;

typedef struct {
	Elf64_Word p_type;
	Elf64_Word p_flags;
	Elf64_Off p_offset;
	Elf64_Addr p_vaddr;
	Elf64_Addr p_paddr;
	Elf64_Xword p_filesz;
	Elf64_Xword p_memsz;
	Elf64_Xword p_align;
} Elf64_Phdr;

#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6
#define PT_TLS 7
#define PT_NUM 8
#define PT_LOOS 0x60000000
#define PT_GNU_EH_FRAME 0x6474e550
#define PT_GNU_STACK 0x6474e551
#define PT_GNU_RELRO 0x6474e552
#define PT_LOSUNW 0x6ffffffa
#define PT_SUNWBSS 0x6ffffffa
#define PT_SUNWSTACK 0x6ffffffb
#define PT_HISUNW 0x6fffffff
#define PT_HIOS 0x6fffffff
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7fffffff

#define PN_XNUM 0xffff

#define PF_X (1 << 0)
#define PF_W (1 << 1)
#define PF_R (1 << 2)
#define PF_MASKOS 0x0ff00000
#define PF_MASKPROC 0xf0000000

#define NT_PRSTATUS 1
#define NT_FPREGSET 2
#define NT_PRPSINFO 3
#define NT_PRXREG 4
#define NT_TASKSTRUCT 4
#define NT_PLATFORM 5
#define NT_AUXV 6
#define NT_GWINDOWS 7
#define NT_ASRS 8
#define NT_PSTATUS 10
#define NT_PSINFO 13
#define NT_PRCRED 14
#define NT_UTSNAME 15
#define NT_LWPSTATUS 16
#define NT_LWPSINFO 17
#define NT_PRFPXREG 20
#define NT_SIGINFO 0x53494749
#define NT_FILE 0x46494c45
#define NT_PRXFPREG 0x46e62b7f
#define NT_PPC_VMX 0x100
#define NT_PPC_SPE 0x101
#define NT_PPC_VSX 0x102
#define NT_386_TLS 0x200
#define NT_386_IOPERM 0x201
#define NT_X86_XSTATE 0x202
#define NT_S390_HIGH_GPRS 0x300
#define NT_S390_TIMER 0x301
#define NT_S390_TODCMP 0x302
#define NT_S390_TODPREG 0x303
#define NT_S390_CTRS 0x304
#define NT_S390_PREFIX 0x305
#define NT_S390_LAST_BREAK 0x306
#define NT_S390_SYSTEM_CALL 0x307
#define NT_S390_TDB 0x308
#define NT_ARM_VFP 0x400
#define NT_ARM_TLS 0x401
#define NT_ARM_HW_BREAK 0x402
#define NT_ARM_HW_WATCH 0x403
#define NT_METAG_CBUF 0x500
#define NT_METAG_RPIPE 0x501
#define NT_METAG_TLS 0x502
#define NT_VERSION 1

typedef struct {
	Elf32_Sword d_tag;
	union {
		Elf32_Word d_val;
		Elf32_Addr d_ptr;
	} d_un;
} Elf32_Dyn;

typedef struct {
	Elf64_Sxword d_tag;
	union {
		Elf64_Xword d_val;
		Elf64_Addr d_ptr;
	} d_un;
} Elf64_Dyn;

#define DT_NULL 0
#define DT_NEEDED 1
#define DT_PLTRELSZ 2
#define DT_PLTGOT 3
#define DT_HASH 4
#define DT_STRTAB 5
#define DT_SYMTAB 6
#define DT_RELA 7
#define DT_RELASZ 8
#define DT_RELAENT 9
#define DT_STRSZ 10
#define DT_SYMENT 11
#define DT_INIT 12
#define DT_FINI 13
#define DT_SONAME 14
#define DT_RPATH 15
#define DT_SYMBOLIC 16
#define DT_REL 17
#define DT_RELSZ 18
#define DT_RELENT 19
#define DT_PLTREL 20
#define DT_DEBUG 21
#define DT_TEXTREL 22
#define DT_JMPREL 23
#define DT_BIND_NOW 24
#define DT_INIT_ARRAY 25
#define DT_FINI_ARRAY 26
#define DT_INIT_ARRAYSZ 27
#define DT_FINI_ARRAYSZ 28
#define DT_RUNPATH 29
#define DT_FLAGS 30
#define DT_ENCODING 32
#define DT_PREINIT_ARRAY 32
#define DT_PREINIT_ARRAYSZ 33
#define DT_NUM 34
#define DT_LOOS 0x6000000d
#define DT_HIOS 0x6ffff000
#define DT_LOPROC 0x70000000
#define DT_HIPROC 0x7fffffff
#define DT_PROCNUM DT_MIPS_NUM

#define DT_VALRNGLO 0x6ffffd00
#define DT_GNU_PRELINKED 0x6ffffdf5
#define DT_GNU_CONFLICTSZ 0x6ffffdf6
#define DT_GNU_LIBLISTSZ 0x6ffffdf7
#define DT_CHECKSUM 0x6ffffdf8
#define DT_PLTPADSZ 0x6ffffdf9
#define DT_MOVEENT 0x6ffffdfa
#define DT_MOVESZ 0x6ffffdfb
#define DT_FEATURE_1 0x6ffffdfc
#define DT_POSFLAG_1 0x6ffffdfd

#define DT_SYMINSZ 0x6ffffdfe
#define DT_SYMINENT 0x6ffffdff
#define DT_VALRNGHI 0x6ffffdff
#define DT_VALTAGIDX(tag) (DT_VALRNGHI - (tag))
#define DT_VALNUM 12

#define DT_ADDRRNGLO 0x6ffffe00
#define DT_GNU_HASH 0x6ffffef5
#define DT_TLSDESC_PLT 0x6ffffef6
#define DT_TLSDESC_GOT 0x6ffffef7
#define DT_GNU_CONFLICT 0x6ffffef8
#define DT_GNU_LIBLIST 0x6ffffef9
#define DT_CONFIG 0x6ffffefa
#define DT_DEPAUDIT 0x6ffffefb
#define DT_AUDIT 0x6ffffefc
#define DT_PLTPAD 0x6ffffefd
#define DT_MOVETAB 0x6ffffefe
#define DT_SYMINFO 0x6ffffeff
#define DT_ADDRRNGHI 0x6ffffeff
#define DT_ADDRTAGIDX(tag) (DT_ADDRRNGHI - (tag))
#define DT_ADDRNUM 11

#define DT_VERSYM 0x6ffffff0

#define DT_RELACOUNT 0x6ffffff9
#define DT_RELCOUNT 0x6ffffffa

#define DT_FLAGS_1 0x6ffffffb
#define DT_VERDEF 0x6ffffffc

#define DT_VERDEFNUM 0x6ffffffd
#define DT_VERNEED 0x6ffffffe

#define DT_VERNEEDNUM 0x6fffffff
#define DT_VERSIONTAGIDX(tag) (DT_VERNEEDNUM - (tag))
#define DT_VERSIONTAGNUM 16

#define DT_AUXILIARY 0x7ffffffd
#define DT_FILTER 0x7fffffff
#define DT_EXTRATAGIDX(tag) ((Elf32_Word) - ((Elf32_Sword)(tag) << 1 >> 1) - 1)
#define DT_EXTRANUM 3

#define DF_ORIGIN 0x00000001
#define DF_SYMBOLIC 0x00000002
#define DF_TEXTREL 0x00000004
#define DF_BIND_NOW 0x00000008
#define DF_STATIC_TLS 0x00000010

#define DF_1_NOW 0x00000001
#define DF_1_GLOBAL 0x00000002
#define DF_1_GROUP 0x00000004
#define DF_1_NODELETE 0x00000008
#define DF_1_LOADFLTR 0x00000010
#define DF_1_INITFIRST 0x00000020
#define DF_1_NOOPEN 0x00000040
#define DF_1_ORIGIN 0x00000080
#define DF_1_DIRECT 0x00000100
#define DF_1_TRANS 0x00000200
#define DF_1_INTERPOSE 0x00000400
#define DF_1_NODEFLIB 0x00000800
#define DF_1_NODUMP 0x00001000
#define DF_1_CONFALT 0x00002000
#define DF_1_ENDFILTEE 0x00004000
#define DF_1_DISPRELDNE 0x00008000
#define DF_1_DISPRELPND 0x00010000
#define DF_1_NODIRECT 0x00020000
#define DF_1_IGNMULDEF 0x00040000
#define DF_1_NOKSYMS 0x00080000
#define DF_1_NOHDR 0x00100000
#define DF_1_EDITED 0x00200000
#define DF_1_NORELOC 0x00400000
#define DF_1_SYMINTPOSE 0x00800000
#define DF_1_GLOBAUDIT 0x01000000
#define DF_1_SINGLETON 0x02000000

#define DTF_1_PARINIT 0x00000001
#define DTF_1_CONFEXP 0x00000002

#define DF_P1_LAZYLOAD 0x00000001
#define DF_P1_GROUPPERM 0x00000002

typedef struct {
	Elf32_Half vd_version;
	Elf32_Half vd_flags;
	Elf32_Half vd_ndx;
	Elf32_Half vd_cnt;
	Elf32_Word vd_hash;
	Elf32_Word vd_aux;
	Elf32_Word vd_next;
} Elf32_Verdef;

typedef struct {
	Elf64_Half vd_version;
	Elf64_Half vd_flags;
	Elf64_Half vd_ndx;
	Elf64_Half vd_cnt;
	Elf64_Word vd_hash;
	Elf64_Word vd_aux;
	Elf64_Word vd_next;
} Elf64_Verdef;

#define VER_DEF_NONE 0
#define VER_DEF_CURRENT 1
#define VER_DEF_NUM 2

#define VER_FLG_BASE 0x1
#define VER_FLG_WEAK 0x2

#define VER_NDX_LOCAL 0
#define VER_NDX_GLOBAL 1
#define VER_NDX_LORESERVE 0xff00
#define VER_NDX_ELIMINATE 0xff01

typedef struct {
	Elf32_Word vda_name;
	Elf32_Word vda_next;
} Elf32_Verdaux;

typedef struct {
	Elf64_Word vda_name;
	Elf64_Word vda_next;
} Elf64_Verdaux;

typedef struct {
	Elf32_Half vn_version;
	Elf32_Half vn_cnt;
	Elf32_Word vn_file;
	Elf32_Word vn_aux;
	Elf32_Word vn_next;
} Elf32_Verneed;

typedef struct {
	Elf64_Half vn_version;
	Elf64_Half vn_cnt;
	Elf64_Word vn_file;
	Elf64_Word vn_aux;
	Elf64_Word vn_next;
} Elf64_Verneed;

#define VER_NEED_NONE 0
#define VER_NEED_CURRENT 1
#define VER_NEED_NUM 2

typedef struct {
	Elf32_Word vna_hash;
	Elf32_Half vna_flags;
	Elf32_Half vna_other;
	Elf32_Word vna_name;
	Elf32_Word vna_next;
} Elf32_Vernaux;

typedef struct {
	Elf64_Word vna_hash;
	Elf64_Half vna_flags;
	Elf64_Half vna_other;
	Elf64_Word vna_name;
	Elf64_Word vna_next;
} Elf64_Vernaux;

#define VER_FLG_WEAK 0x2

typedef struct {
	uint32_t a_type;
	union {
		uint32_t a_val;
	} a_un;
} Elf32_auxv_t;

typedef struct {
	uint64_t a_type;
	union {
		uint64_t a_val;
	} a_un;
} Elf64_auxv_t;

#define AT_NULL 0
#define AT_IGNORE 1
#define AT_EXECFD 2
#define AT_PHDR 3
#define AT_PHENT 4
#define AT_PHNUM 5
#define AT_PAGESZ 6
#define AT_BASE 7
#define AT_FLAGS 8
#define AT_ENTRY 9
#define AT_NOTELF 10
#define AT_UID 11
#define AT_EUID 12
#define AT_GID 13
#define AT_EGID 14
#define AT_CLKTCK 17

#define AT_PLATFORM 15
#define AT_HWCAP 16

#define AT_FPUCW 18

#define AT_DCACHEBSIZE 19
#define AT_ICACHEBSIZE 20
#define AT_UCACHEBSIZE 21

#define AT_IGNOREPPC 22

#define AT_SECURE 23

#define AT_BASE_PLATFORM 24

#define AT_RANDOM 25

#define AT_HWCAP2 26

#define AT_EXECFN 31

#define AT_SYSINFO 32
#define AT_SYSINFO_EHDR 33

#define AT_L1I_CACHESHAPE 34
#define AT_L1D_CACHESHAPE 35
#define AT_L2_CACHESHAPE 36
#define AT_L3_CACHESHAPE 37

typedef struct {
	Elf32_Word n_namesz;
	Elf32_Word n_descsz;
	Elf32_Word n_type;
} Elf32_Nhdr;

typedef struct {
	Elf64_Word n_namesz;
	Elf64_Word n_descsz;
	Elf64_Word n_type;
} Elf64_Nhdr;

#define ELF_NOTE_SOLARIS "SUNW Solaris"

#define ELF_NOTE_GNU "GNU"

#define ELF_NOTE_PAGESIZE_HINT 1

#define NT_GNU_ABI_TAG 1
#define ELF_NOTE_ABI NT_GNU_ABI_TAG

#define ELF_NOTE_OS_LINUX 0
#define ELF_NOTE_OS_GNU 1
#define ELF_NOTE_OS_SOLARIS2 2
#define ELF_NOTE_OS_FREEBSD 3

#define NT_GNU_BUILD_ID 3
#define NT_GNU_GOLD_VERSION 4

typedef struct {
	Elf32_Xword m_value;
	Elf32_Word m_info;
	Elf32_Word m_poffset;
	Elf32_Half m_repeat;
	Elf32_Half m_stride;
} Elf32_Move;

typedef struct {
	Elf64_Xword m_value;
	Elf64_Xword m_info;
	Elf64_Xword m_poffset;
	Elf64_Half m_repeat;
	Elf64_Half m_stride;
} Elf64_Move;

#define ELF32_M_SYM(info) ((info) >> 8)
#define ELF32_M_SIZE(info) ((unsigned char)(info))
#define ELF32_M_INFO(sym, size) (((sym) << 8) + (unsigned char)(size))

#define ELF64_M_SYM(info) ELF32_M_SYM(info)
#define ELF64_M_SIZE(info) ELF32_M_SIZE(info)
#define ELF64_M_INFO(sym, size) ELF32_M_INFO(sym, size)

#define EF_CPU32 0x00810000

#define R_68K_NONE 0
#define R_68K_32 1
#define R_68K_16 2
#define R_68K_8 3
#define R_68K_PC32 4
#define R_68K_PC16 5
#define R_68K_PC8 6
#define R_68K_GOT32 7
#define R_68K_GOT16 8
#define R_68K_GOT8 9
#define R_68K_GOT32O 10
#define R_68K_GOT16O 11
#define R_68K_GOT8O 12
#define R_68K_PLT32 13
#define R_68K_PLT16 14
#define R_68K_PLT8 15
#define R_68K_PLT32O 16
#define R_68K_PLT16O 17
#define R_68K_PLT8O 18
#define R_68K_COPY 19
#define R_68K_GLOB_DAT 20
#define R_68K_JMP_SLOT 21
#define R_68K_RELATIVE 22
#define R_68K_NUM 23

#define R_386_NONE 0
#define R_386_32 1
#define R_386_PC32 2
#define R_386_GOT32 3
#define R_386_PLT32 4
#define R_386_COPY 5
#define R_386_GLOB_DAT 6
#define R_386_JMP_SLOT 7
#define R_386_RELATIVE 8
#define R_386_GOTOFF 9
#define R_386_GOTPC 10
#define R_386_32PLT 11
#define R_386_TLS_TPOFF 14
#define R_386_TLS_IE 15
#define R_386_TLS_GOTIE 16
#define R_386_TLS_LE 17
#define R_386_TLS_GD 18
#define R_386_TLS_LDM 19
#define R_386_16 20
#define R_386_PC16 21
#define R_386_8 22
#define R_386_PC8 23
#define R_386_TLS_GD_32 24
#define R_386_TLS_GD_PUSH 25
#define R_386_TLS_GD_CALL 26
#define R_386_TLS_GD_POP 27
#define R_386_TLS_LDM_32 28
#define R_386_TLS_LDM_PUSH 29
#define R_386_TLS_LDM_CALL 30
#define R_386_TLS_LDM_POP 31
#define R_386_TLS_LDO_32 32
#define R_386_TLS_IE_32 33
#define R_386_TLS_LE_32 34
#define R_386_TLS_DTPMOD32 35
#define R_386_TLS_DTPOFF32 36
#define R_386_TLS_TPOFF32 37
#define R_386_SIZE32 38
#define R_386_TLS_GOTDESC 39
#define R_386_TLS_DESC_CALL 40
#define R_386_TLS_DESC 41
#define R_386_IRELATIVE 42
#define R_386_NUM 43

#define STT_SPARC_REGISTER 13

#define EF_SPARCV9_MM 3
#define EF_SPARCV9_TSO 0
#define EF_SPARCV9_PSO 1
#define EF_SPARCV9_RMO 2
#define EF_SPARC_LEDATA 0x800000
#define EF_SPARC_EXT_MASK 0xFFFF00
#define EF_SPARC_32PLUS 0x000100
#define EF_SPARC_SUN_US1 0x000200
#define EF_SPARC_HAL_R1 0x000400
#define EF_SPARC_SUN_US3 0x000800

#define R_SPARC_NONE 0
#define R_SPARC_8 1
#define R_SPARC_16 2
#define R_SPARC_32 3
#define R_SPARC_DISP8 4
#define R_SPARC_DISP16 5
#define R_SPARC_DISP32 6
#define R_SPARC_WDISP30 7
#define R_SPARC_WDISP22 8
#define R_SPARC_HI22 9
#define R_SPARC_22 10
#define R_SPARC_13 11
#define R_SPARC_LO10 12
#define R_SPARC_GOT10 13
#define R_SPARC_GOT13 14
#define R_SPARC_GOT22 15
#define R_SPARC_PC10 16
#define R_SPARC_PC22 17
#define R_SPARC_WPLT30 18
#define R_SPARC_COPY 19
#define R_SPARC_GLOB_DAT 20
#define R_SPARC_JMP_SLOT 21
#define R_SPARC_RELATIVE 22
#define R_SPARC_UA32 23

#define R_SPARC_PLT32 24
#define R_SPARC_HIPLT22 25
#define R_SPARC_LOPLT10 26
#define R_SPARC_PCPLT32 27
#define R_SPARC_PCPLT22 28
#define R_SPARC_PCPLT10 29
#define R_SPARC_10 30
#define R_SPARC_11 31
#define R_SPARC_64 32
#define R_SPARC_OLO10 33
#define R_SPARC_HH22 34
#define R_SPARC_HM10 35
#define R_SPARC_LM22 36
#define R_SPARC_PC_HH22 37
#define R_SPARC_PC_HM10 38
#define R_SPARC_PC_LM22 39
#define R_SPARC_WDISP16 40
#define R_SPARC_WDISP19 41
#define R_SPARC_GLOB_JMP 42
#define R_SPARC_7 43
#define R_SPARC_5 44
#define R_SPARC_6 45
#define R_SPARC_DISP64 46
#define R_SPARC_PLT64 47
#define R_SPARC_HIX22 48
#define R_SPARC_LOX10 49
#define R_SPARC_H44 50
#define R_SPARC_M44 51
#define R_SPARC_L44 52
#define R_SPARC_REGISTER 53
#define R_SPARC_UA64 54
#define R_SPARC_UA16 55
#define R_SPARC_TLS_GD_HI22 56
#define R_SPARC_TLS_GD_LO10 57
#define R_SPARC_TLS_GD_ADD 58
#define R_SPARC_TLS_GD_CALL 59
#define R_SPARC_TLS_LDM_HI22 60
#define R_SPARC_TLS_LDM_LO10 61
#define R_SPARC_TLS_LDM_ADD 62
#define R_SPARC_TLS_LDM_CALL 63
#define R_SPARC_TLS_LDO_HIX22 64
#define R_SPARC_TLS_LDO_LOX10 65
#define R_SPARC_TLS_LDO_ADD 66
#define R_SPARC_TLS_IE_HI22 67
#define R_SPARC_TLS_IE_LO10 68
#define R_SPARC_TLS_IE_LD 69
#define R_SPARC_TLS_IE_LDX 70
#define R_SPARC_TLS_IE_ADD 71
#define R_SPARC_TLS_LE_HIX22 72
#define R_SPARC_TLS_LE_LOX10 73
#define R_SPARC_TLS_DTPMOD32 74
#define R_SPARC_TLS_DTPMOD64 75
#define R_SPARC_TLS_DTPOFF32 76
#define R_SPARC_TLS_DTPOFF64 77
#define R_SPARC_TLS_TPOFF32 78
#define R_SPARC_TLS_TPOFF64 79
#define R_SPARC_GOTDATA_HIX22 80
#define R_SPARC_GOTDATA_LOX10 81
#define R_SPARC_GOTDATA_OP_HIX22 82
#define R_SPARC_GOTDATA_OP_LOX10 83
#define R_SPARC_GOTDATA_OP 84
#define R_SPARC_H34 85
#define R_SPARC_SIZE32 86
#define R_SPARC_SIZE64 87
#define R_SPARC_GNU_VTINHERIT 250
#define R_SPARC_GNU_VTENTRY 251
#define R_SPARC_REV32 252

#define R_SPARC_NUM 253

#define DT_SPARC_REGISTER 0x70000001
#define DT_SPARC_NUM 2

#define EF_MIPS_NOREORDER 1
#define EF_MIPS_PIC 2
#define EF_MIPS_CPIC 4
#define EF_MIPS_XGOT 8
#define EF_MIPS_64BIT_WHIRL 16
#define EF_MIPS_ABI2 32
#define EF_MIPS_ABI_ON32 64
#define EF_MIPS_ARCH 0xf0000000

#define EF_MIPS_ARCH_1 0x00000000
#define EF_MIPS_ARCH_2 0x10000000
#define EF_MIPS_ARCH_3 0x20000000
#define EF_MIPS_ARCH_4 0x30000000
#define EF_MIPS_ARCH_5 0x40000000
#define EF_MIPS_ARCH_32 0x50000000
#define EF_MIPS_ARCH_64 0x60000000
#define EF_MIPS_ARCH_32R2 0x70000000
#define EF_MIPS_ARCH_64R2 0x80000000

#define E_MIPS_ARCH_1 0x00000000
#define E_MIPS_ARCH_2 0x10000000
#define E_MIPS_ARCH_3 0x20000000
#define E_MIPS_ARCH_4 0x30000000
#define E_MIPS_ARCH_5 0x40000000
#define E_MIPS_ARCH_32 0x50000000
#define E_MIPS_ARCH_64 0x60000000

#define SHN_MIPS_ACOMMON 0xff00
#define SHN_MIPS_TEXT 0xff01
#define SHN_MIPS_DATA 0xff02
#define SHN_MIPS_SCOMMON 0xff03
#define SHN_MIPS_SUNDEFINED 0xff04

#define SHT_MIPS_LIBLIST 0x70000000
#define SHT_MIPS_MSYM 0x70000001
#define SHT_MIPS_CONFLICT 0x70000002
#define SHT_MIPS_GPTAB 0x70000003
#define SHT_MIPS_UCODE 0x70000004
#define SHT_MIPS_DEBUG 0x70000005
#define SHT_MIPS_REGINFO 0x70000006
#define SHT_MIPS_PACKAGE 0x70000007
#define SHT_MIPS_PACKSYM 0x70000008
#define SHT_MIPS_RELD 0x70000009
#define SHT_MIPS_IFACE 0x7000000b
#define SHT_MIPS_CONTENT 0x7000000c
#define SHT_MIPS_OPTIONS 0x7000000d
#define SHT_MIPS_SHDR 0x70000010
#define SHT_MIPS_FDESC 0x70000011
#define SHT_MIPS_EXTSYM 0x70000012
#define SHT_MIPS_DENSE 0x70000013
#define SHT_MIPS_PDESC 0x70000014
#define SHT_MIPS_LOCSYM 0x70000015
#define SHT_MIPS_AUXSYM 0x70000016
#define SHT_MIPS_OPTSYM 0x70000017
#define SHT_MIPS_LOCSTR 0x70000018
#define SHT_MIPS_LINE 0x70000019
#define SHT_MIPS_RFDESC 0x7000001a
#define SHT_MIPS_DELTASYM 0x7000001b
#define SHT_MIPS_DELTAINST 0x7000001c
#define SHT_MIPS_DELTACLASS 0x7000001d
#define SHT_MIPS_DWARF 0x7000001e
#define SHT_MIPS_DELTADECL 0x7000001f
#define SHT_MIPS_SYMBOL_LIB 0x70000020
#define SHT_MIPS_EVENTS 0x70000021
#define SHT_MIPS_TRANSLATE 0x70000022
#define SHT_MIPS_PIXIE 0x70000023
#define SHT_MIPS_XLATE 0x70000024
#define SHT_MIPS_XLATE_DEBUG 0x70000025
#define SHT_MIPS_WHIRL 0x70000026
#define SHT_MIPS_EH_REGION 0x70000027
#define SHT_MIPS_XLATE_OLD 0x70000028
#define SHT_MIPS_PDR_EXCEPTION 0x70000029

#define SHF_MIPS_GPREL 0x10000000
#define SHF_MIPS_MERGE 0x20000000
#define SHF_MIPS_ADDR 0x40000000
#define SHF_MIPS_STRINGS 0x80000000
#define SHF_MIPS_NOSTRIP 0x08000000
#define SHF_MIPS_LOCAL 0x04000000
#define SHF_MIPS_NAMES 0x02000000
#define SHF_MIPS_NODUPE 0x01000000

#define STO_MIPS_DEFAULT 0x0
#define STO_MIPS_INTERNAL 0x1
#define STO_MIPS_HIDDEN 0x2
#define STO_MIPS_PROTECTED 0x3
#define STO_MIPS_PLT 0x8
#define STO_MIPS_SC_ALIGN_UNUSED 0xff

#define STB_MIPS_SPLIT_COMMON 13

typedef union {
	struct {
		Elf32_Word gt_current_g_value;
		Elf32_Word gt_unused;
	} gt_header;
	struct {
		Elf32_Word gt_g_value;
		Elf32_Word gt_bytes;
	} gt_entry;
} Elf32_gptab;

typedef struct {
	Elf32_Word ri_gprmask;
	Elf32_Word ri_cprmask[4];
	Elf32_Sword ri_gp_value;
} Elf32_RegInfo;

typedef struct {
	unsigned char kind;

	unsigned char size;
	Elf32_Section section;

	Elf32_Word info;
} Elf_Options;

#define ODK_NULL 0
#define ODK_REGINFO 1
#define ODK_EXCEPTIONS 2
#define ODK_PAD 3
#define ODK_HWPATCH 4
#define ODK_FILL 5
#define ODK_TAGS 6
#define ODK_HWAND 7
#define ODK_HWOR 8

#define OEX_FPU_MIN 0x1f
#define OEX_FPU_MAX 0x1f00
#define OEX_PAGE0 0x10000
#define OEX_SMM 0x20000
#define OEX_FPDBUG 0x40000
#define OEX_PRECISEFP OEX_FPDBUG
#define OEX_DISMISS 0x80000

#define OEX_FPU_INVAL 0x10
#define OEX_FPU_DIV0 0x08
#define OEX_FPU_OFLO 0x04
#define OEX_FPU_UFLO 0x02
#define OEX_FPU_INEX 0x01

#define OHW_R4KEOP 0x1
#define OHW_R8KPFETCH 0x2
#define OHW_R5KEOP 0x4
#define OHW_R5KCVTL 0x8

#define OPAD_PREFIX 0x1
#define OPAD_POSTFIX 0x2
#define OPAD_SYMBOL 0x4

typedef struct {
	Elf32_Word hwp_flags1;
	Elf32_Word hwp_flags2;
} Elf_Options_Hw;

#define OHWA0_R4KEOP_CHECKED 0x00000001
#define OHWA1_R4KEOP_CLEAN 0x00000002

#define R_MIPS_NONE 0
#define R_MIPS_16 1
#define R_MIPS_32 2
#define R_MIPS_REL32 3
#define R_MIPS_26 4
#define R_MIPS_HI16 5
#define R_MIPS_LO16 6
#define R_MIPS_GPREL16 7
#define R_MIPS_LITERAL 8
#define R_MIPS_GOT16 9
#define R_MIPS_PC16 10
#define R_MIPS_CALL16 11
#define R_MIPS_GPREL32 12

#define R_MIPS_SHIFT5 16
#define R_MIPS_SHIFT6 17
#define R_MIPS_64 18
#define R_MIPS_GOT_DISP 19
#define R_MIPS_GOT_PAGE 20
#define R_MIPS_GOT_OFST 21
#define R_MIPS_GOT_HI16 22
#define R_MIPS_GOT_LO16 23
#define R_MIPS_SUB 24
#define R_MIPS_INSERT_A 25
#define R_MIPS_INSERT_B 26
#define R_MIPS_DELETE 27
#define R_MIPS_HIGHER 28
#define R_MIPS_HIGHEST 29
#define R_MIPS_CALL_HI16 30
#define R_MIPS_CALL_LO16 31
#define R_MIPS_SCN_DISP 32
#define R_MIPS_REL16 33
#define R_MIPS_ADD_IMMEDIATE 34
#define R_MIPS_PJUMP 35
#define R_MIPS_RELGOT 36
#define R_MIPS_JALR 37
#define R_MIPS_TLS_DTPMOD32 38
#define R_MIPS_TLS_DTPREL32 39
#define R_MIPS_TLS_DTPMOD64 40
#define R_MIPS_TLS_DTPREL64 41
#define R_MIPS_TLS_GD 42
#define R_MIPS_TLS_LDM 43
#define R_MIPS_TLS_DTPREL_HI16 44
#define R_MIPS_TLS_DTPREL_LO16 45
#define R_MIPS_TLS_GOTTPREL 46
#define R_MIPS_TLS_TPREL32 47
#define R_MIPS_TLS_TPREL64 48
#define R_MIPS_TLS_TPREL_HI16 49
#define R_MIPS_TLS_TPREL_LO16 50
#define R_MIPS_GLOB_DAT 51
#define R_MIPS_COPY 126
#define R_MIPS_JUMP_SLOT 127

#define R_MIPS_NUM 128

#define PT_MIPS_REGINFO 0x70000000
#define PT_MIPS_RTPROC 0x70000001
#define PT_MIPS_OPTIONS 0x70000002

#define PF_MIPS_LOCAL 0x10000000

#define DT_MIPS_RLD_VERSION 0x70000001
#define DT_MIPS_TIME_STAMP 0x70000002
#define DT_MIPS_ICHECKSUM 0x70000003
#define DT_MIPS_IVERSION 0x70000004
#define DT_MIPS_FLAGS 0x70000005
#define DT_MIPS_BASE_ADDRESS 0x70000006
#define DT_MIPS_MSYM 0x70000007
#define DT_MIPS_CONFLICT 0x70000008
#define DT_MIPS_LIBLIST 0x70000009
#define DT_MIPS_LOCAL_GOTNO 0x7000000a
#define DT_MIPS_CONFLICTNO 0x7000000b
#define DT_MIPS_LIBLISTNO 0x70000010
#define DT_MIPS_SYMTABNO 0x70000011
#define DT_MIPS_UNREFEXTNO 0x70000012
#define DT_MIPS_GOTSYM 0x70000013
#define DT_MIPS_HIPAGENO 0x70000014
#define DT_MIPS_RLD_MAP 0x70000016
#define DT_MIPS_DELTA_CLASS 0x70000017
#define DT_MIPS_DELTA_CLASS_NO 0x70000018

#define DT_MIPS_DELTA_INSTANCE 0x70000019
#define DT_MIPS_DELTA_INSTANCE_NO 0x7000001a

#define DT_MIPS_DELTA_RELOC 0x7000001b
#define DT_MIPS_DELTA_RELOC_NO 0x7000001c

#define DT_MIPS_DELTA_SYM 0x7000001d

#define DT_MIPS_DELTA_SYM_NO 0x7000001e

#define DT_MIPS_DELTA_CLASSSYM 0x70000020

#define DT_MIPS_DELTA_CLASSSYM_NO 0x70000021

#define DT_MIPS_CXX_FLAGS 0x70000022
#define DT_MIPS_PIXIE_INIT 0x70000023
#define DT_MIPS_SYMBOL_LIB 0x70000024
#define DT_MIPS_LOCALPAGE_GOTIDX 0x70000025
#define DT_MIPS_LOCAL_GOTIDX 0x70000026
#define DT_MIPS_HIDDEN_GOTIDX 0x70000027
#define DT_MIPS_PROTECTED_GOTIDX 0x70000028
#define DT_MIPS_OPTIONS 0x70000029
#define DT_MIPS_INTERFACE 0x7000002a
#define DT_MIPS_DYNSTR_ALIGN 0x7000002b
#define DT_MIPS_INTERFACE_SIZE 0x7000002c
#define DT_MIPS_RLD_TEXT_RESOLVE_ADDR 0x7000002d

#define DT_MIPS_PERF_SUFFIX 0x7000002e

#define DT_MIPS_COMPACT_SIZE 0x7000002f
#define DT_MIPS_GP_VALUE 0x70000030
#define DT_MIPS_AUX_DYNAMIC 0x70000031

#define DT_MIPS_PLTGOT 0x70000032

#define DT_MIPS_RWPLT 0x70000034
#define DT_MIPS_NUM 0x35
