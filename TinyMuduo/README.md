# DAY01

今天写几个基础类，和网络库功能无关的

## noncopyable类
其他类继承noncopyable类之后，可以正常的构造和析构，但是不能拷贝构造和赋值。
两种方式：
1. 让拷贝构造和赋值函数都delete掉 (推荐)
2. 拷贝构造和赋值函数 声明为private，并且不实现

## Log日志类
实现一个简易的异步日志

## Timestamp类
时间类