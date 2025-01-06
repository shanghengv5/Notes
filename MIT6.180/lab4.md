# Lab: traps
RISC-V assembly (easy)
## Which registers contain arguments to functions? For example, which register holds 13 in main's call to printf?
a0 cotain x argument to g function
a2 hold 13
## Where is the call to function f in the assembly code for main? Where is the call to g? (Hint: the compiler may inline functions.)
function f
26:	45b1                	li	a1,12
function g
14:	250d                	addiw	a0,a0,3 
## At what address is the function printf located?
pc is x1 shif left 12bit => x1000
x1000+x6bc=x16bc
## What value is in the register ra just after the jalr to printf in main?
x1000+4=x1004
## What is the output? Here's an ASCII table that maps bytes to characters.The output depends on that fact that the RISC-V is little-endian. If the RISC-V were instead big-endian what would you set i to in order to yield the same output? Would you need to change 57616 to a different value?

```
	unsigned int i = 0x00646c72;
	printf("H%x Wo%s", 57616, (char *) &i);
```

He110, World

## 