#include <iostream>
using namespace std;

class noncopyable
{

private:
    noncopyable(const noncopyable &);
    noncopyable &operator=(const noncopyable &);

public:
    // noncopyable(const noncopyable &) = delete;
    // noncopyable &operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};
class A : noncopyable
{
public:
    A() = default;
    // A(const A &);
};