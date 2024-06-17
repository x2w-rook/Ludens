#pragma once

#include <cstring>
#include <cstddef>
#include "Core/DSA/Include/View.h"
#include "Core/Math/Include/Bits.h"
#include "Core/Math/Include/Hash.h"

namespace LD
{

/// generic template string with configurable SSO,
/// provides local storage of a fixed amount of chars before heap allocation.
template <typename TChar, size_t TLocalCapacity>
class TString
{
public:
    static size_t LocalCapacity()
    {
        return TLocalCapacity;
    }

    /// minimum initialization
    TString() : mBase(mLocal), mSize(0), mCapacity(TLocalCapacity)
    {
    }

    TString(size_t size) : mBase(mLocal), mSize(size)
    {
        GrowCapacity(size);
    }

    /// construct from null terminated c string
    /// @note TString is never guaranteed to be null terminated.
    TString(const char* cstr) : mBase(mLocal), mSize(0), mCapacity(TLocalCapacity)
    {
        size_t size = strlen(cstr);

        if (size > mCapacity)
            GrowCapacity(size);

        mSize = size;
        for (size_t i = 0; i < mSize; i++)
            mBase[i] = (TChar)cstr[i];
    }

    TString(const TString& other) : mBase(mLocal), mSize(0), mCapacity(TLocalCapacity)
    {
        if (other.mCapacity > mCapacity)
            GrowCapacity(other.mCapacity);

        mSize = other.mSize;
        for (size_t i = 0; i < mSize; i++)
            mBase[i] = other.mBase[i];
    }

    TString(TString&& other) noexcept : mSize(other.mSize), mCapacity(other.mCapacity)
    {
        if (other.mBase == other.mLocal)
        {
            mBase = mLocal;
            for (size_t i = 0; i < mSize; i++)
                mBase[i] = other.mBase[i];
        }
        else
            mBase = other.mBase;

        other.mSize = 0;
        other.mBase = other.mLocal;
        other.mCapacity = TLocalCapacity;
    }

    ~TString()
    {
        if (mBase != mLocal)
            delete[] mBase;
    }

    TString& operator=(const TString& other)
    {
        if (other.mCapacity > mCapacity)
            GrowCapacity(other.mCapacity);

        mSize = other.mSize;
        for (size_t i = 0; i < mSize; i++)
            mBase[i] = other.mBase[i];

        return *this;
    }

    TString& operator=(TString&& other) noexcept
    {
        mSize = other.mSize;
        mCapacity = other.mCapacity;

        if (other.mBase == other.mLocal)
        {
            mBase = mLocal;
            for (size_t i = 0; i < mSize; i++)
                mBase[i] = other.mBase[i];
        }
        else
            mBase = other.mBase;

        other.mSize = 0;
        other.mBase = other.mLocal;
        other.mCapacity = TLocalCapacity;
        return *this;
    }

    inline size_t Capacity() const
    {
        return mCapacity;
    }

    /// number of chars
    inline size_t Size() const
    {
        return mSize;
    }

    inline void Resize(size_t size)
    {
        if (size > mCapacity)
            GrowCapacity(size);

        mSize = size;
    }

    inline void Clear()
    {
        mSize = 0;
    }

    /// get a string view that is valid until the next
    /// non-const member function is called
    inline View<TChar> GetView() const
    {
        return View<TChar> { mSize, mBase };
    }

    /// number of bytes if viewed as a byte-stream
    inline size_t ByteSize() const
    {
        return sizeof(TChar) * mSize;
    }

    inline bool IsEmpty() const
    {
        return mSize == 0;
    }

    const TChar& operator[](int idx) const
    {
        return mBase[idx];
    }

    TChar& operator[](int idx)
    {
        return mBase[idx];
    }

    bool operator==(const TString& other) const
    {
        if (mSize != other.mSize)
            return false;

        for (size_t i = 0; i < mSize; i++)
        {
            if (mBase[i] != other.mBase[i])
                return false;
        }

        return true;
    }

    bool operator!=(const TString& other) const
    {
        return !(*this == other);
    }

    /// equality test against null terminated c string
    bool operator==(const char* cstr) const
    {
        if (mSize != strlen(cstr))
            return false;

        for (size_t i = 0; i < mSize; i++)
        {
            if (mBase[i] != (TChar)cstr[i])
                return false;
        }

        return true;
    }

    bool operator!=(const char* cstr) const
    {
        return !(*this == cstr);
    }

    /// create string, appending the contents of another string
    TString operator+(const TString& other) const
    {
        size_t newSize = mSize + other.mSize;
        TString value(newSize);

        for (size_t i = 0; i < mSize; i++)
            value.mBase[i] = mBase[i];

        for (size_t i = 0; i < other.mSize; i++)
            value.mBase[mSize + i] = other.mBase[i];

        return value;
    }

    TString& operator+=(const TString& other)
    {
        return *this = *this + other;
    }

    /// create string, appending the contents of a null terminated c string
    TString operator+(const char* cstr) const
    {
        size_t len = strlen(cstr);
        size_t newSize = mSize + len;
        TString value(newSize);

        for (size_t i = 0; i < mSize; i++)
            value.mBase[i] = mBase[i];

        for (size_t i = 0; i < len; i++)
            value.mBase[mSize + i] = (TChar)cstr[i];

        return value;
    }

    TString& operator+=(const char* cstr)
    {
        return *this = *this + cstr;
    }

private:
    void GrowCapacity(size_t capacity)
    {
        mCapacity = NextPowerOf2(capacity);

        if (mCapacity <= TLocalCapacity)
            return;

        TChar* newBase = new TChar[mCapacity];
        for (size_t i = 0; i < mSize; i++)
            newBase[i] = mBase[i];

        if (mBase != mLocal)
            delete[] mBase;

        mBase = newBase;
    }

    TChar* mBase;
    TChar mLocal[TLocalCapacity];
    size_t mSize;
    size_t mCapacity;
};

using String = TString<char, 24>;

/// pre-hashed string, optimized for fast equality test.
template <typename TChar, typename THash>
class TStringHash
{
public:

    /// @warn empty string will assume a hash of zero
    TStringHash() : mHash(0)
    {
    }

    explicit TStringHash(const TChar* str, size_t len)
    {
        mHash = THash{}(str, len);
    }

    /// construct from null terminated c string
    explicit TStringHash(const char* cstr)
    {
        mHash = THash{}(cstr, strlen(cstr));
    }

    /// assign new hash from string
    TStringHash& operator=(const View<TChar>& view)
    {
        mHash = THash{}(view.Data(), view.Size());
        return *this;
    }

    /// assign new hash from null terminated c string 
    TStringHash& operator=(const char* cstr)
    {
        mHash = THash{}(cstr, strlen(cstr));
        return *this;
    }

    bool operator==(const TStringHash& other) const
    {
        return mHash == other.mHash;
    }

    bool operator!=(const TStringHash& other) const
    {
        return !(*this == other);
    }

private:
    size_t mHash;
};

using StringHash = TStringHash<char, DJB2<char>>;

} // namespace LD