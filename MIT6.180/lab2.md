# Lab2

## Abstracting physical resources

## User mode,supervisor mode, and system calls

## Operating system organization

### Kernel organiztion

![alt text](image-2.png)

* monolithic kernel 性能高，内核一个模块出问题会影响整个系统，拓展性差，交互简单，因为共用一个内存空间。
* microkernel 灵活可靠，性能相对差，因为需要频繁的切换上下文

## 2.4Code:xv6 organization

![alt text](image-3.png)

## 2.5Process overview

* trampoline 跳板，4096byte，以实现函数调用和控制流的转移。
* trapframe 陷阱帧
    * 指令计数器(Programe Counter PC) 指向当前正在执行的指令的地址
    * 栈指针(Stack Pointer SP) 指向当前线程的栈顶位置。
    * 寄存器状态
    * 中断或者其他异常类型
通过trampoline和trapframe实现转换进入内核和返回。
trampoline实现内核的进入和返回，trapframe保存了进城的状态和信息。

### import pieces

```c
strcut proc {
    kstack // kernel stack
    state // indicates whether the processes is allocated, ready to run, currently running on a CPU, wait for I/O, exiting
    pagetable // xv6 casusing paging hardware to use a pagetable when executing that  process in a user space, also serves as the record of addresses of the physical pages allocated to store the process's memory.
}
//...
```

* kernel state
* page table
* kernel stack(p->kstack): each process has two stack, kernel stack and process stack
* run state

### Summary

A address space to give a process illusion of its own memory
A thread to give the process the illusion of its own CPU

## 2.6 the first process and system call
概述一个系统如何启动
![alt text](image-4.png)

## 2.7 Security Model

系统必须假设用户层代码会尽全力破坏内核或者其他进城。如果用户层代码访问地址超出了允许的地址空间，那么就可能访问到指令集,即使这不是为用户层设计的。
他可能试图读或者写内核的控制寄存器。也可能直接访问设备硬件，甚至传递一些参数导致系统崩溃。

## 2.8 Real World

## lab

```zsh
# 窗口1
make qemu-gdb
# 窗口2
gdb-multiarch
# 加载.gdbinit
source .gdbinit
```

### Looking at the backtrace output, which function called syscall?
![alt text](image-5.png)

### What is the value of p->trapframe->a7 and what does that value represent? (Hint: look user/initcode.S, the first user program xv6 starts.)
![alt text](image-7.png)

### What was the previous mode that the CPU was in?
![alt text](image-8.png)
![alt text](image-9.png)

可以知道ssp在第8位，0010,0010后八位中，ssp=0

### Write down the assembly instruction the kernel is panicing at. Which register corresponds to the variable num?

![alt text](image-10.png)
执行

![alt text](image-11.png)

将地址0x0的特殊寄存器0加载到a3 寄存器中。

###  Why does the kernel crash? Hint: look at figure 3-3 in the text; is address 0 mapped in the kernel address space? Is that confirmed by the value in scause above? (See description of scause in RISC-V privileged instructions)

#### Figure3

* Sv39 only use 39bits of a 64-bit virtual address
* PTEs 2的27次方 to find a PTE and make the 56-bit physical address by 44-bit from PPN and whose bottom 12 bits are copied from the original virtual address.
* PPN 44-bit physical page number(PPN)

![alt text](image-12.png)

* TLB(Translation Look-aside Buffer),用户缓存虚拟地址与物理地址的映射关系，因为使用了三层PTEs结构，内存需要加载三层的PTEs是一个潜在的性能隐患。

![alt text](image-13.png)
RISC-V address translation details

* satp(Supervisor Address traslation and protection register)
    * Page Table Base Address:页表的基地址
    * Address Translation and Protection Mode:地址转换和保护模式，比如页表模式和分页模式
    * Address Space Protection:地址空间保护策略，比如访问权限控制，内存区域的可读执行属性

![alt text](image-14.png)

* CLINT(Core Local Interupt),处理器用于内部中断使用，管理处理器核心之间的中断和定时器事件。
* PLIC(Platform Level Interupt Controller),用于外部设备与处理器的中断分发控制器，PLIC会根据不同优先级发出对应不同设备的中断信号，用以管理设备的运行与中止。
* UART(Universal Asynchronous Receiver Transmitter),用于与外部设备的串行通信。
* VIRTIO disk,用于虚拟化存储方案，一般用于虚拟机或者虚拟容器
* Guard page,用于内存的保护和检测，目的是防止错误的读取和写入超出范围的数据。

##### Kernel address space

* PHYSTOP 定义了物理地址的顶值
* KERNBASE (kernel base address),内核虚拟地址的基准起始地址。

The kernel gets at RAM and memory-mapped device registers using "direct mapping" that is mapping the resources at virtual addressses that are equal to the physical address.

There are a couple of kernel virtual addresses that aren't direct-mapped:
* The trampoline page. It is mapped at the top of virtual address space;
* The kernel stack pages. Each process has its own kernel stack,which is mapped high so that below it xv6 can leave an unmapped guard page.The guard page's PTE is invalid, so that if the kernel overflows a kernel stack, it will likely cause an exception and the kernel will panic.

##### Code:creating an address space

* sfence(store fence),存储屏障，用于保障存储指令在乱序中能够保持一致，保证等待之前的更新操作必须完成。

##### Physical memory allocation

##### Code:Physical memory allocator

* PGROUNDUP 定义了对齐的大小，通常是4KB

#### Process address space