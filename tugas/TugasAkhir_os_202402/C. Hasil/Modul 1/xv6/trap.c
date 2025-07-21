#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

extern pte_t* walkpgdir(pde_t *pgdir, const void *va, int alloc);
extern int ref_count[];          // array refcount halaman fisik
extern void incref(char *pa);
extern void decref(char *pa);

void
trap(struct trapframe *tf)
{
  struct proc *p = myproc();
  pte_t *pte;
  uint va, pa;

  if(tf->trapno == T_PGFLT){
    if(p == 0){
      cprintf("trap: page fault but no current process\n");
      panic("trap");
    }
    va = rcr2();
    pte = walkpgdir(p->pgdir, (void*)va, 0);
    if(pte == 0 || (*pte & PTE_P) == 0){
      cprintf("trap: page fault, no PTE\n");
      p->killed = 1;
    } else if(*pte & PTE_COW){ 
      pa = PTE_ADDR(*pte);
      char *mem = (char*)P2V(pa);
      if(ref_count[pa >> 12] > 1){
        char *newmem = kalloc();
        if(newmem == 0){
          cprintf("trap: out of memory in COW\n");
          p->killed = 1;
        } else {
          memmove(newmem, mem, PGSIZE);
          decref((char*)pa);          // Kurangi refcount page lama
          *pte = V2P(newmem) | PTE_P | PTE_U | PTE_W;
          incref(newmem);             // Naikkan refcount page baru
          lcr3(V2P(p->pgdir));        // Flush TLB
        }
      } else {
        // Refcount == 1, cukup set flag tulis dan hapus COW
        *pte |= PTE_W;
        *pte &= ~PTE_COW;
        lcr3(V2P(p->pgdir));        // Flush TLB
      }
    } else {
      cprintf("trap: page fault, not COW\n");
      p->killed = 1;
    }
    return;
  }

  switch(tf->trapno){
    case T_IRQ0 + IRQ_TIMER:
      if(cpuid() == 0){
        acquire(&tickslock);
        ticks++;
        wakeup(&ticks);
        release(&tickslock);
      }
      lapiceoi();
      break;

    case T_IRQ0 + IRQ_IDE:
      ideintr();
      lapiceoi();
      break;

    case T_IRQ0 + IRQ_KBD:
      kbdintr();
      lapiceoi();
      break;

    case T_IRQ0 + IRQ_COM1:
      uartintr();
      lapiceoi();
      break;

    case T_IRQ0 + 7:
    case T_IRQ0 + IRQ_SPURIOUS:
      cprintf("cpu%d: spurious interrupt at %x:%x\n",
              cpuid(), tf->cs, tf->eip);
      lapiceoi();
      break;

    case T_SYSCALL:
      if(p->killed)
        exit();
      p->tf = tf;
      syscall();
      if(p->killed)
        exit();
      return;

    default:
      if(p == 0 || (tf->cs & 3) == 0){
        // In kernel, panic
        cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
                tf->trapno, cpuid(), tf->eip, rcr2());
        panic("trap");
      }
      // User process misbehaved, kill it
      cprintf("pid %d %s: trap %d err %d on cpu %d "
              "eip 0x%x addr 0x%x--kill proc\n",
              p->pid, p->name, tf->trapno, tf->err, cpuid(), tf->eip, rcr2());
      p->killed = 1;
  }

  if(p && p->killed && (tf->cs & 3) == DPL_USER)
    exit();

  // Force process to give up CPU on clock tick.
  if(p && p->state == RUNNING && tf->trapno == T_IRQ0 + IRQ_TIMER)
    yield();

  if(p && p->killed && (tf->cs & 3) == DPL_USER)
    exit();
}