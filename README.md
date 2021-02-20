# BookStore设计文档
## Class
### `User`类
    char password[31] = "";
    char username[31] = "";
    int privilege = -1;
    char userid[31] = "";
    int select = -1;
记录用户信息;
### `Book`类 
    char ISBN[20] = "" ;
    char name[60] = "" ;
    char author[60] = "" ;
    char keyword[60] = "" ;
    double price = 0;
    int quantity = 0;
记录书籍信息;
### `Transcation`类 
记录buy与import的对应信息;
### `Action`类与`Log`类: 
记录员工用户的相关操作信息;

## 文件组成：
### `User.dat`
储存用户信息;
### `Book.dat`
储存书籍信息;
### `userid.dat`/`isbn.dat`/`name.dat`/`author.dat`/`keyword.dat`
使用块状链表储存信息，便于索引;
### `action.dat`/`log.dat` 
储存员工操作信息;

## CPP组成：
### `main.cpp`: 
程序运行主体部分;
实现读取原始命令信息和异常抛出的捕获;
### `Blocklist.cpp`:
块状链表;
addnode ， deletenode来实现更改；
`merge`：将个数较少的block合并
`split`：将个数较多的block分离
保障索引的速度；
通过Blocklist类的函数实现快速的索引;
### `Program.cpp`:
实现与程序运行相关的函数：初始化 , run 和 quit等;
### `Book.cpp`:
实现书籍管理：import ，buy ， modify， show等
### `User.cpp`:
实现用户的账户管理：useradd ， login ，logout等 
### `Store.cpp`:
实现财政和书店整体管理：showfinance ， log等
