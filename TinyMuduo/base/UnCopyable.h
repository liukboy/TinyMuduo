#pragma once

/*
如果一个类不想被拷贝以及赋值，那么就继承UnCopyable
*/
class UnCopyable
{
public:
    UnCopyable(const UnCopyable &) = delete;
    UnCopyable &operator=(const UnCopyable &) = delete;

protected:
    UnCopyable() = default;
    ~UnCopyable() = default;
};