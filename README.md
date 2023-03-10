# OS_HW

### 实验三 Linux 进程管理

#### 1. 实现一个模拟 shell（[HW03_01](/HW03_01/)）

编写三个不同的程序 cmd1.c、cmd2.c 及 cmd3.c，每个程序功能自定，分别编译成可执行文件 cmd1、cmd2 及 cmd3。然后再编写一个程序，模拟shell程序的功能：根据用户的输入内容创建子进程并去执行相应的程序，等待子程序退出后再接受下一条命令，若接收到 exit，则父进程退出。

#### 2. 实现一个管道通信程序（[HW03_02](/HW03_02/)）

由父进程创建一个管道，然后再创建三个子进程，由这三个子进程利用管道与父进程通信。要求能试验阻塞型读写过程中的各种情况，测试管道的默认大小。

#### 3. 利用 Linux 的消息队列通信机制实现三个线程间的通信（[HW03_03](/HW03_03/)）

创建三个线程：sender1、sender2 和 receiver，三个线程的功能如下：

（1）sender1 线程运行 sender1() 函数，创建一个消息队列，等待用户输入数据，将数据发送给 receiver 线程，直到用户输入“exit”，此时向 receiver 线程发送“end1”并等待receiver 线程的应答，将应答消息显示在终端屏幕上，结束线程运行。

（2）sender2 线程运行 sender2() 函数，共享 sender1 线程创建的消息队列，其他功能与sender1线程一致。

（3）receiver 线程运行 receiver() 函数，打印 sender1 和 sender2 线程通过消息队列发送的信息，当两个线程都结束时，删除消息队列，结束线程运行。

选择合适的信号量实现三个线程之间的同步与互斥。

#### 4. 利用 Linux 的共享内存通信机制实现两个进程间的通信（[HW03_04](/HW03_04/)）

编写程序 sender，创建共享内存，等待用户通过终端输入一串字符，再将字符通过共享内存发送给 receiver，然后等待 receiver 的应答，将应答消息显示在屏幕上，最后删除共享内存。

### 实验四 简单文件系统的实现（[HW04](/HW04/)）

在内存中开辟一个虚拟磁盘空间作为文件存储分区，在其上实现一个简单的基于多级目录的单用户单任务系统中的文件系统。在退出该文件系统的使用时，应将虚拟磁盘上的内容以一个文件的方式保存到磁盘上，以便下次可以再将它恢复到内存的虚拟磁盘中。

文件的物理结构可以采用显示链接或其他结构；空闲磁盘空间的管理可采用 FAT 表、位示图或其他办法；文件目录结构采用多级目录结构，为简单起见可以不使用索引节点；要求提供以下操作命令：

mkdir：用于创建子目录。

rmdir：用于删除子目录。

ls：用于显示目录中的内容。

cd：用于更改当前目录。

create：用于创建文件。

open：用于打开文件

close：用于关闭文件。

write：用于写文件。

read：用于读文件。

rm：用于删除文件。

exitsys：用于退出文件系统。
