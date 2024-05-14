#pragma once

#include <doctest.h>
#include "Core/Header/Include/Singleton.h"

using namespace LD;

class Foo : public Singleton<Foo>
{
public:
    Foo()
    {
        sInstanceCount++;
        x = 0;
    }

    ~Foo()
    {
        sInstanceCount--;
    }

    static int sInstanceCount;

    inline int GetX()
    {
        return x;
    }

    inline void SetX(int x_)
    {
        x = x_;
    }

private:
    int x;
};

int Foo::sInstanceCount;

TEST_CASE("Test Singleton")
{
    CHECK(Foo::sInstanceCount == 0);

    Foo& foo1 = Foo::GetSingleton();
    CHECK(Foo::sInstanceCount == 1);

    foo1.SetX(1337);

    Foo& foo2 = Foo::GetSingleton();
    CHECK(Foo::sInstanceCount == 1);
    CHECK(foo2.GetX() == 1337);

    Foo::DeleteSingleton();
    CHECK(Foo::sInstanceCount == 0);

    foo1 = Foo::GetSingleton();
    CHECK(Foo::sInstanceCount == 1);
    CHECK(foo1.GetX() == 0);
}