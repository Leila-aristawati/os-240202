// mmu.h
// Definisi untuk Memory Management Unit (MMU) pada arsitektur x86

#ifndef MMU_H
#define MMU_H

// Eflags register
#define FL_IF           0x00000200      // Interrupt Enable

// Control Register flags
#define CR0_PE          0x00000001      // Protection Enable
#define CR0_WP          0x00010000      // Write Protect
#define CR0_PG          0x80000000      // Paging

#define CR4_PSE         0x00000010      // Page size extension

// Segment selectors
#define SEG_KCODE 1  // kernel code
#define SEG_KDATA 2  // kernel data+stack
#define SEG_UCODE 3  // user code
#define SEG_UDATA 4  // user data+stack
#define SEG_TSS   5  // task state segment

#define NSEGS     6  // jumlah segmen

#ifndef __ASSEMBLER__

// Segment Descriptor
struct segdesc {
  uint lim_15_0 : 16;  // low bits segment limit
  uint base_15_0 : 16; // low bits segment base address
  uint base_23_16 : 8; // middle bits segment base address
  uint type : 4;       // segment type
  uint s : 1;          // 0 = system, 1 = application
  uint dpl : 2;        // descriptor privilege level
  uint p : 1;          // present
  uint lim_19_16 : 4;  // high bits segment limit
  uint avl : 1;        // available for software use
  uint rsv1 : 1;       // reserved
  uint db : 1;         // 0 = 16-bit, 1 = 32-bit segment
  uint g : 1;          // granularity (4K units)
  uint base_31_24 : 8; // high bits segment base address
};

#define SEG(type, base, lim, dpl) (struct segdesc)    \
{ ((lim) >> 12) & 0xffff, (uint)(base) & 0xffff,      \
  ((uint)(base) >> 16) & 0xff, type, 1, dpl, 1,       \
  (uint)(lim) >> 28, 0, 0, 1, 1, (uint)(base) >> 24 }

#define SEG16(type, base, lim, dpl) (struct segdesc)  \
{ (lim) & 0xffff, (uint)(base) & 0xffff,              \
  ((uint)(base) >> 16) & 0xff, type, 1, dpl, 1,       \
  (uint)(lim) >> 16, 0, 0, 1, 0, (uint)(base) >> 24 }

#endif

#define DPL_USER    0x3     // Descriptor Privilege Level untuk user

// Application segment type bits
#define STA_X       0x8     // Executable segment
#define STA_W       0x2     // Writeable (non-executable segments)
#define STA_R       0x2     // Readable (executable segments)

// System segment type bits
#define STS_T32A    0x9     // Available 32-bit TSS
#define STS_IG32    0xE     // 32-bit Interrupt Gate
#define STS_TG32    0xF     // 32-bit Trap Gate

// Virtual address layout:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/

#define PDXSHIFT        22
#define PTXSHIFT        12

#define PDX(va)         (((uint)(va) >> PDXSHIFT) & 0x3FF)
#define PTX(va)         (((uint)(va) >> PTXSHIFT) & 0x3FF)

#define PGADDR(d, t, o) ((uint)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// Number of entries
#define NPDENTRIES      1024
#define NPTENTRIES      1024

// Page size
#define PGSIZE          4096

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

// Page table/directory entry flags
#define PTE_P           0x001   // Present
#define PTE_W           0x002   // Writable
#define PTE_U           0x004   // User
#define PTE_PS          0x080   // Page Size (4MB pages)

// Flags mask and address extraction
#define PTE_ADDR(pte)   ((uint)(pte) & ~0xFFF)
#define PTE_FLAGS(pte)  ((uint)(pte) & 0xFFF)

// Custom flag untuk Copy-On-Write
#define PTE_COW         0x200   // contoh flag bit bebas untuk CoW

#ifndef __ASSEMBLER__

typedef uint pte_t;
typedef uint pde_t;

// Deklarasi fungsi untuk paging
pte_t* walkpgdir(pde_t *pgdir, const void *va, int alloc);
int mappages(pde_t *pgdir, void *va, uint size, uint pa, int perm);

// Task state segment
struct taskstate {
  uint link;
  uint esp0;
  ushort ss0;
  ushort padding1;
  uint *esp1;
  ushort ss1;
  ushort padding2;
  uint *esp2;
  ushort ss2;
  ushort padding3;
  void *cr3;
  uint *eip;
  uint eflags;
  uint eax;
  uint ecx;
  uint edx;
  uint ebx;
  uint *esp;
  uint *ebp;
  uint esi;
  uint edi;
  ushort es;
  ushort padding4;
  ushort cs;
  ushort padding5;
  ushort ss;
  ushort padding6;
  ushort ds;
  ushort padding7;
  ushort fs;
  ushort padding8;
  ushort gs;
  ushort padding9;
  ushort ldt;
  ushort padding10;
  ushort t;
  ushort iomb;
};

// Gate descriptors for interrupts/traps
struct gatedesc {
  uint off_15_0 : 16;
  uint cs : 16;
  uint args : 5;
  uint rsv1 : 3;
  uint type : 4;
  uint s : 1;
  uint dpl : 2;
  uint p : 1;
  uint off_31_16 : 16;
};

#define SETGATE(gate, istrap, sel, off, d)                \
{                                                         \
  (gate).off_15_0 = (uint)(off) & 0xffff;                \
  (gate).cs = (sel);                                      \
  (gate).args = 0;                                        \
  (gate).rsv1 = 0;                                        \
  (gate).type = (istrap) ? STS_TG32 : STS_IG32;          \
  (gate).s = 0;                                           \
  (gate).dpl = (d);                                       \
  (gate).p = 1;                                           \
  (gate).off_31_16 = (uint)(off) >> 16;                  \
}

#endif

#endif // MMU_H