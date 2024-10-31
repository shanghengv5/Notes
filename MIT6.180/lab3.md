# Lab3
page tables

## prepare

read xv6 chapter3

## Inspect a user-process page table (easy)

### For every page table entry in the print_pgtbl output, explain what it logically contains and what its permission bits are. Figure 3.4 in the xv6 book might be helpful, although note that the figure might have a slightly different set of pages than process that's being inspected here. Note that xv6 doesn't place the virtual pages consecutively in physical memory.

![alt text](image-39.png)
PTE >> 10 获得PPN， << 12 留出offset位，得到pa。
PTE & 0x3ff,0x3ff是10位的mask,用于获得Flags

## 