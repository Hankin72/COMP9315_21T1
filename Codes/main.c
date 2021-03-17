// c 程序主要包括以下部分：
// 预处理器指令
// 函数
// 变量
// 语句&表达式
// 注释


#include <stdio.h>
// 程序的第一行 #include <stdio.h> 是预处理器指令，告诉 C 编译器在实际编译之前要包含 stdio.h 文件。
// 下一行 int main() 是主函数，程序从这里开始执行。
// 下一行 /*...*/ 将会被编译器忽略，这里放置程序的注释内容。它们被称为程序的注释。
//下一行 printf(...) 是 C 中另一个可用的函数，会在屏幕上显示消息 "Hello, World!"。
//下一行 return 0; 终止 main() 函数，并返回值 0。



//  C 的令牌（Token）
// c程序有各种令牌组成，令牌可以是关键字，标识符，常量，或者一个符号，
// 分号，是语句结束符， 也就是说，每个语句必须以分号结束，它表明一个逻辑实体的结束

int main(){
    /*
     * 我的第一个c程序
     */
    printf("Hello, world! \n");


    int n[]={3,2,1,4,7,6,5,8,0,9};

    int largest,i,num;

    num= sizeof(n)/sizeof(n[0]);

    printf("%d",n);



    return 0;
}

// 标识符
// c标识符是用来标识 变量， 函数， 或者任何其他用户自定义项目的名称。
//  一个标识符以字母A-Z 或者 a-z 或下划线_ 开始， 后跟零个或多个字母，下划线和数字（0-9）

// C 标识符内不允许出现标点字符，比如 @、$ 和 % 。 C 是区分大小写的编程语言。因此，在c中，manpower和Manpower是两个不同标识符号，

// 关键字
// auto	声明自动变量   break	跳出当前循环  case	开关语句分支   char	声明字符型变量或函数返回值类型
// const	定义常量，如果一个变量被 const 修饰，那么它的值就不能再被改变
// continue	结束当前循环，开始下一轮循环
// default	开关语句中的"其它"分支
// do	循环语句的循环体
// double	声明双精度浮点型变量或函数返回值类型
// else	条件语句否定分支（与 if 连用
// enum	声明枚举类型
//extern	声明变量或函数是在其它文件或本文件的其他位置定义
// float	声明浮点型变量或函数返回值类型
// for	一种循环语句
// //goto	无条件跳转语句
// if	条件语句
//int	声明整型变量或函数
//long	声明长整型变量或函数返回值类型

//register	声明寄存器变量

//return	子程序返回语句（可以带参数，也可不带参数）

//short	声明短整型变量或函数

//signed	声明有符号类型变量或函数

//sizeof	计算数据类型或变量长度（即所占字节数）
//static	声明静态变量

//struct	声明结构体类型

//switch	用于开关语句

//typedef	用以给数据类型取别名

//unsigned	声明无符号类型变量或函数

//union	声明共用体类型

//void	声明函数无返回值或无参数，声明无类型指针

//volatile	说明变量在程序执行中可被隐含地改变

//while	循环语句的循环条件