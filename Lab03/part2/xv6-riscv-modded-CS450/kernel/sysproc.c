// Francisco Barba Cuellar #A20121767
// Deimantas Gilys #A20434583
// Nathan Cook #A20458336
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

int sys_whereIs(void)
{
  //added
  char va;

  if(argstr(0, &va, sizeof(char*)) < 0)
    return -1;

  struct proc* p = myproc();

  pte_t* pte = walk(p->pagetable, (uint64)va, 0);

  if(pte == 0 || (*pte & PTE_V) == 0)
    return -1;

  return PTE2PA(*pte);
}

int sys_iswritable(void)
{
  char va;
  struct proc* p = myproc();
  if(argstr(0, &va, sizeof(char *)) < 0)
    return -1;
  
  pte_t* pte = walk(p->pagetable, (uint64)va, 0);
  if(pte == 0)
    return -1;
  
  if((*pte & PTE_W) == 0)
    return 0;
  else
    return 1;
}

int sys_notwritable(void)
{
  char va;
  struct proc* p = myproc();
  if(argstr(0, &va, sizeof(char *)) < 0)
    return -1;
  
  pte_t* pte = walk(p->pagetable, (uint64)va, 0);
  if(pte == 0)
    return -1;
  
  if((*pte & PTE_W) == 0)
    return -1;

  *pte &= ~PTE_W;
  return 0;
}

int sys_yeswritable(void)
{
  char va;
  struct proc* p = myproc();
  if(argstr(0, &va, sizeof(char *)) < 0)
    return -1;
  
  pte_t* pte = walk(p->pagetable, (uint64)va, 0);
  if(pte == 0)
    return -1;
  
  if((*pte & PTE_W) != 0)
    return -1;

  *pte |= PTE_W;
  return 0;
}


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
