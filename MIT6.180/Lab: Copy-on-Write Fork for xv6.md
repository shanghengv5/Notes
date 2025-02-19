# Lab: Copy-on-Write Fork for xv6
## Implement copy-on-write fork(hard)
![alt text](image-41.png)
查找scause的值是多少为page fault

![alt text](image-42.png)

根据riscv-privilege 手册可得到，15为pagefault

![alt text](image-43.png)

可以看到pid为3时，引起了0xf的scause异常，据此，修改user_trap函数

注意还需要关注pa的引用问题，因为当cow的时候，一个物理地址会有两个引用，只有引用为0才能释放。
![alt text](image-44.png)
物理地址区域可得，又因为一页为4096