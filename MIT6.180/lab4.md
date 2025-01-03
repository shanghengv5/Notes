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