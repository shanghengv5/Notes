# Lab1
阅读handout中的chapter1.
[Lab: Xv6 and Unix utilities](https://pdos.csail.mit.edu/6.1810/2024/labs/util.html)

## 安装环境
[lab tools](https://pdos.csail.mit.edu/6.1810/2024/tools.html)

镜像安装ubuntu24.04
```
ubuntu:
    image: ubuntu:24.04
    volumes:
      - $DOCUMENT_PATH:/home/Documents
    networks:
      backend-activity:
        ipv4_address: $BACK_UBUNTU_IP
    container_name: ubuntu
    stdin_open: true
    tty: true
```

## Lab util: Unix utilities

### sleep(easy)
```c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
  if ( argc < 2) {
    fprintf(2, "Pls send a number\n");
    exit(1);
  }
  sleep(atoi(argv[2]));
  fprintf(1, "(nothing happens for a little while)\n");
  exit(0);
}
```

```bash
./grade-lab-util sleep
```

开始的练手小作业，注意去MAKEFILE增加编译即可

### pingpong
