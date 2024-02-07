##### 前言：

```markdown
本文介绍一下常用的gcc和make工具。
有学习成本，初次接触完全掌握其中的概念并能够熟练使用预估需要4个小时的学习成本。
建议遇到不懂的名词和概念，直接Chat-gpt4搜索
```

## **编译C语言程序**

```markdown
原理：
1.将文件编译成可链接文件
2.将所需可链接文件链接成一个可执行文件
```

#### 方法一 直接编译链接成一个可执行文件

```markdown
编译单个文件
1.可将单个C语言代码文件直接、编译、链接成一个可执行文件
gcc -o output    a.c

编译多个文件
1.将多个C语言代码文件直接编译、链接成一个可执行文件
gcc -o output    a.c b.c 
```

#### 方法二 将文件先编译成可链接文件，再链接成可执行文件

```markdown
1.将多个文件先编译成可链接文件
2.将多个可链接文件链接到一起，成为一个可执行文件。（将不同文件的代码段、文本段整合到一个文件的代码段里）

单个文件
linux下为.o文件，一个.o文件，包括文本段、代码段、堆栈段符号表等等
gcc -c  a.c                -c选项指定生成可链接文件,且不进行链接
gcc -o  output  a.o        将.o文件链接生成可执行文件

多个文件
gcc   -c fun1.c  -o fun1.o
gcc   -c fun2.c  -o fun2.o       使用-o选项可以自己命名生成的.o文件，否则与.c文件同名

gcc   -o output  fun1.o fun2.o   将多个.o文件链接成一个可执行文件
```

#### 方法三 将文件先编译成函数库文件，编译主程序时链接函数库

```markdown
1.生成静态链接库
①将文件编译成目标文件
gcc   -c fun1.c    -o fun1.o
gcc   -c fun2.c    -o fun2.o
②将目标文件打包成一个静态链接库
ar  rcs libmylib.a    fun1.o  fun2.o
                         打包静态链接库命令
③生成可执行文件时链接静态库
gcc   main.c    -L  -lmylib    -o  ouput       
                         -L选项指定链接器在当前目录中查找库文件 -lmylib指定链接到名为libmylib.a的库


2.动态共享库
①将文件编译为位置无关的目标文件
gcc   -c -fPIC  mylib.c     -o  mylib.o
                         -fPIC选项表示生成位置无关的代码
②创建动态链接库
gcc   -shared    -o  libmylib.so    mylib.o
                         使用mylib.o目标文件生成一个命令libmylib.so的动态链接库
③编译时使用动态链接库
gcc   main.c     -o output     -L  -lmylib
                         -L选项可以跟路径  -L"/var/lib/library"，在路径下寻找库
```



## 自动化编译工具 Make程序

#### 介绍

```markdown
属于第三方应用，使用前应安装sudo apt install make
make程序会自动寻找当前目录下的makefile文件，根据makefile文件里定义的规则执行makefile文件里定义的命令。
```

#### 经常使用的makefile语法规则

```makefile
#makefile语法规则

#以下是一个实例

CC = gcc                  #定义了一个变量，变量名cc，值是gcc；约定俗称一般使用CC表示编译器，可自行设定
CFLAGS = -g -Wall         #属于gcc的编译选项 -g debug版本，-Wall编译时启动全部告警信息，跟使用的编译器有关
SRCS = *.c                #支持通配符
OBJS = $(SRCS:.c=.o)       #通配符的替换，将*.c变为*.o

#all用来在make时可以携带的参数，定义了all，可使用make all，就会执行myprogram（作为一个target）定义时指定的命令规则
all: myprogram           
build: myoutput          #bulid和all一样，可使用make build

#目标规则
#target表示要构建的目标
#dependency1 dependencey2 表示构建target依赖的文件
#command1 command2 表示构建target的命令
target: dependency1 dependency2
    command1
    command2

myprogram: main.c util.c   #构建myprogram需要当前目录下有main.c和util.c文件
    $(CC)  $(CFLAGS) main.c util.c  -o myprogram

myprogram: main.c util.c
    $(CC)  $(CFLAGS) $(SRCS)  -o myprogram

#伪目标
#通常用来描述一些没有文件依赖和不产生实际文件的构建任务。通常是清理操作
.PHONY: clean
clean:
    rm -f $(OBJS)
    
distclean:
    rm -f 

```

#### makefile变量	

```makefile
#变量可以替换，显得makefile很复杂，这里详细探究一下内里

#定义变量
NAME = myprogram
SOURCES := file1.c  file2.c

#引用变量 $(variable) 或 ${variable}
complile:
    gcc $(SOURCES) -o $(NAME)
    
#变量赋值
SOURCES = file3.c
SOURCES += file4.c

#自动化变量
    #    $@：表示target
    #    $<：表示规则中的第一个依赖  dependence1
    #    $^：表示规则中的所有依赖文件列表 dependence1 dependence2 ...

    target: dependency1 dependency2
        command1
        command2
    #eg.
    myprogram: main.c util.c
        gcc main.c util.c  -o myprogram

    #使用自动化变量写成
    myprogram: main.c util.c
        gcc  $^  -o $@      #  $^表示依赖列表， main.c util.c   $@表示target  myprogram


#模式变量/模式规则
#用于指定文件名之间的模式匹配，以及根据匹配的模式执行相应的命令
%.<target_extension>: %.<source_extension>
     <command>

%.o: %.c
    gcc -c $< -o $@  #将每一个.c文件都编译一个对应同名的.o文件
    
%: %.c
    gcc $< -o $@     #将每一个.c文件编译成对应同名的无后缀的可执行文件

%: %.o
    gcc $< -o $@     #将每一个.o文件编译成对应同名的无后缀的可执行文件
```

#### 通配符

```makefile
#makefile支持通配符，遇到通配符不会的可以直接搜索吧

SOURCES = file1.c   file2.c   file3.c
OBJECTS = $(patsubst %.c, %, $(SOURCES))    #OBJECTS为 file1  file2  file3
```

#### 最常用的语句

```makefile
#wildcard是函数， *是通配符， 这个语句的意思，在当前目录下（不包括子文件）所有.c文件
SRCS = $(wildcard *.c)
```

