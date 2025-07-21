#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"

#define MAXPHYS_PAGES (PHYSTOP / PGSIZE)
static int page_refcount[MAXPHYS_PAGES];

// Helper: Convert physical address to refcount index
static int
pa2refindex(uint pa) {
  return (pa - KERNBASE) / PGSIZE;
}

void
refcount_init() {
  int i;
  for(i = 0; i < MAXPHYS_PAGES; i++)
    page_refcount[i] = 0;
}

void
increfcount(uint pa) {
  int idx = pa2refindex(pa);
  if(idx < 0 || idx >= MAXPHYS_PAGES)
    panic("increfcount: invalid pa");
  page_refcount[idx]++;
}

void
decrefcount(uint pa) {
  int idx = pa2refindex(pa);
  if(idx < 0 || idx >= MAXPHYS_PAGES)
    panic("decrefcount: invalid pa");
  if(page_refcount[idx] <= 0)
    panic("decrefcount: refcount already zero");
  page_refcount[idx]--;
}

int
getrefcount(uint pa) {
  int idx = pa2refindex(pa);
  if(idx < 0 || idx >= MAXPHYS_PAGES)
    panic("getrefcount: invalid pa");
  return page_refcount[idx];
}

// Copy-on-write version of uvmunmap/allocuvm fokus di cowuvm
pde_t*
cowuvm(pde_t *pgdir, uint sz)
{
  pde_t *d;
  pte_t *p;
  uint i, pa, flags;

  if((d = setupkvm()) == 0)
    return 0;

  for(i = 0; i < sz; i += PGSIZE){
    p = walkpgdir(pgdir, (void*)i, 0);
    if(p == 0)
      panic("cowuvm: walkpgdir returned 0");

    if(!(*p & PTE_P))
      panic("cowuvm: page not present");

    pa = PTE_ADDR(*p);
    flags = PTE_FLAGS(*p);

    // Tandai halaman lama jadi read-only dan set flag COW
    *p = pa | (flags & ~PTE_W) | PTE_COW;

    // Increment refcount halaman fisik
    increfcount(pa);

    // Map ke page directory baru dengan proteksi sama (read-only)
    if(mappages(d, (void*)i, PGSIZE, pa, flags & ~PTE_W) < 0){
      freevm(d);
      return 0;
    }
  }
  return d;
}

// Fungsi mappages yang mendukung PTE_COW dan proteksi read-only
int
mappages(pde_t *pgdir, void *va, uint size, uint pa, int perm)
{
  char *a, *last;
  pte_t *pte;

  a = (char*)PGROUNDDOWN((uint)va);
  last = (char*)PGROUNDDOWN(((uint)va) + size - 1);
  for(;;){
    pte = walkpgdir(pgdir, a, 1);
    if(pte == 0)
      return -1;
    if(*pte & PTE_P)
      panic("mappages: remap");

    *pte = pa | perm | PTE_P;
    if(a == last)
      break;
    a += PGSIZE;
  }
  return 0;
}