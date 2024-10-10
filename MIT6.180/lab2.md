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