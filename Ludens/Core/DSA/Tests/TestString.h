#pragma once

#include <doctest.h>
#include "Core/DSA/Include/String.h"

using namespace LD;

static const char sBlob[] = "this string is so long it should be heap allocated";
static const size_t sBlobLen = sizeof(sBlob) - 1;
static const size_t sBlobCap = NextPowerOf2(sBlobLen); // assumes implementation

TEST_CASE("String Copy")
{
    // construct from cstr
    String s1 = "Hello";

    CHECK(s1.Size() == 5);
    CHECK(s1.ByteSize() == 5);
    CHECK(s1 == "Hello");

    // copy construct
    String s2(s1);
    s1[0] = 'h';

    CHECK(s2.Size() == 5);
    CHECK(s2.ByteSize() == 5);
    CHECK(s2 == "Hello");
    CHECK(s1 == "hello");

    // default construct
    String s3;
    CHECK(s3.IsEmpty());
    CHECK(s3.ByteSize() == 0);

    // copy assign
    s3 = s2;
    s2[4] = '.';

    CHECK(s3.Size() == 5);
    CHECK(s3 == "Hello");
}

TEST_CASE("String Move")
{
    String s1("bar");
    String s2(sBlob);

    // move construct from local buffer
    String s3 = s1 + s1;
    CHECK(s3 == "barbar");
    CHECK(s1 == "bar");

    // move construct from heap buffer
    String s4 = s2 + s1;
    CHECK(s4.Size() == s2.Size() + s1.Size());
    CHECK(s1 == "bar");

    // move assign from local buffer
    s3 = s1 + s1;
    CHECK(s3 == "barbar");
    CHECK(s1 == "bar");

    // move assign from heap buffer
    s4 = s2 + s1;
    CHECK(s4.Size() == s2.Size() + s1.Size());
    CHECK(s1 == "bar");
}

TEST_CASE("String operator+")
{
    String s1("hello"), s2(", world");
    String s3 = s1 + s2;
    CHECK(s1 == "hello");
    CHECK(s2 == ", world");
    CHECK(s3 == "hello, world");

    s3 = s1 + ", " + "world";
    CHECK(s3 == "hello, world");
}

TEST_CASE("String operator+=")
{
    String s1("bar");
    String s2("barbar");

    // this is safe as we create a temporary rvalue and assign back to s1
    s1 += s1;
    CHECK(s1 == s2);

    s2 += "bar";
    CHECK(s2 == "barbarbar");
}