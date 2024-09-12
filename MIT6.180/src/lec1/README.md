# LECTURE1
阅读handout中的chapter1.

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
