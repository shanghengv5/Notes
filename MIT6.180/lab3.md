# Lab3
page tables

## prepare

read xv6 chapter3

## Inspect a user-process page table (easy)

### For every page table entry in the print_pgtbl output, explain what it logically contains and what its permission bits are. Figure 3.4 in the xv6 book might be helpful, although note that the figure might have a slightly different set of pages than process that's being inspected here. Note that xv6 doesn't place the virtual pages consecutively in physical memory.

![alt text](image-39.png)
PTE >> 10 获得PPN， << 12 留出offset位，得到pa。
PTE & 0x3ff,0x3ff是10位的mask,用于获得Flags

## Speed up system calls (easy)

仿造着TRAPFRAME添加USYSCALL代码
```c
void
proc_freepagetable(pagetable_t pagetable, uint64 sz)
{
  uvmunmap(pagetable, TRAMPOLINE, 1, 0);
  uvmunmap(pagetable, TRAPFRAME, 1, 0);
#ifdef LAB_PGTBL
  uvmunmap(pagetable, USYSCALL, 1, 0);    
#endif  
  uvmfree(pagetable, sz);
}

...
#ifdef LAB_PGTBL
  if(mappages(pagetable, USYSCALL, PGSIZE,
              (uint64)(p->usyscall), PTE_U | PTE_R ) < 0){
    uvmunmap(pagetable, TRAMPOLINE, 1, 0);
    uvmunmap(pagetable, TRAPFRAME, 1, 0);
    uvmfree(pagetable, 0);
    return 0;
  }
#endif

...
#ifdef LAB_PGTBL
  if((p->usyscall = (struct usyscall *)kalloc()) == 0){
      freeproc(p);
      release(&p->lock);
      return 0;
  }
  p->usyscall->pid = p->pid;
#endif

...
#ifdef LAB_PGTBL
  struct usyscall *usyscall;
#endif
```

## Print a page table (easy)
```c
void
vmprint(pagetable_t pagetable) {
  // your code here
  printf("page table %p\n", pagetable);
  pteprint(pagetable, 1);

}

void pteprint(pagetable_t pagetable, uint64 depth) {
  for(int i = 0; i < 512; i++){
    pte_t pte = pagetable[i];
    if(pte & PTE_V) {
      uint64 child = PTE2PA(pte);
      for (int i = 0; i < depth; i++) {
        printf(" ..");
      }
      uint64 va = i * PGSIZE;
      printf("%p: pte %p pa %p\n", (void *)va, (pte_t *)pte, (pte_t *)child);
      if ((pte & (PTE_R|PTE_W|PTE_X)) == 0) {
        pteprint((pagetable_t)child, depth+1);
      }
    }
  }
}
```
### For every leaf page in the vmprint output, explain what it logically contains and what its permission bits are, and how it relates to the output of the earlier print_pgtbl() exercise above. Figure 3.4 in the xv6 book might be helpful, although note that the figure might have a slightly different set of pages than the process that's being inspected here.
可以使用PTE_FLAGS获取对应的权限位

## Use superpages (moderate)/(hard)

定义好对应的superKem以及SUPER_PG标志位。
设置一个合理的superpg数量，即可
```
#define PTE_SUPER (1L << 5) // SUPER

#ifdef LAB_PGTBL
void superfree(void *pa)
{
  struct run *r;

  if (((uint64)pa % SUPERPGSIZE) != 0 || (char *)pa < end || (uint64)pa >= PHYSTOP)
    panic("superfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, SUPERPGSIZE);

  r = (struct run *)pa;

  acquire(&superKmem.lock);
  r->next = superKmem.freelist;
  superKmem.freelist = r;
  release(&superKmem.lock);
}

// Allocate one super 2MB page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
superalloc(void)
{
  struct run *r;

  acquire(&superKmem.lock);
  r = superKmem.freelist;
  if (r)
    superKmem.freelist = r->next;
  release(&superKmem.lock);
  if (r)
    memset((char *)r, 5, SUPERPGSIZE); // fill with junk
  return (void *)r;
}
#endif

```

### READ pgtbltest.c
### add superalloc and superfree
### 参考资料

[A Comprehensive Analysis of Superpage
Management Mechanisms and Policies](https://www.usenix.org/system/files/atc20-zhu-weixi_0.pdf)

[Redit about page table](https://www.reddit.com/r/RISCV/comments/v895do/what_are_super_pages_wrt_page_tables/)
