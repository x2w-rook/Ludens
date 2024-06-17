#include <iostream>
#include "Core/DSA/Include/String.h"
#include "Core/OS/Include/Time.h"

using namespace LD;

// compare string hash equality test
static void BenchStringHash()
{
    const size_t N = 10000000;
    const size_t longStrLen = 2 * String::LocalCapacity();
    double timeStartup;
    double timeShortStr;
    double timeShortStrHash;
    double timeLongStr;
    double timeLongStrHash;

    String alphaNum("0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz");

    auto generateStr = [&](String& str, size_t len)
    {
        str.Resize(len);
        for (size_t i = 0; i < len; i++)
            str[i] = alphaNum[rand() % alphaNum.Size()];
    };

    String* longStr = new String[N];
    String* shortStr = new String[N];
    StringHash* longStrHash = new StringHash[N];
    StringHash* shortStrHash = new StringHash[N];
    size_t ctr = 0;

    {
        ScopeTimer timer(&timeStartup);

        for (size_t i = 0; i < N; i++)
        {
            generateStr(shortStr[i], String::LocalCapacity());
            shortStrHash[i] = shortStr[i].GetView();

            generateStr(longStr[i], longStrLen);
            longStrHash[i] = longStr[i].GetView();
        }
    }

    {
        ScopeTimer timer(&timeShortStr);

        for (size_t i = 0; i < N; i++)
        {
            if (shortStr[i] == shortStr[N - 1 - i])
                ctr++;
        }
    }

    {
        ScopeTimer timer(&timeShortStrHash);

        for (size_t i = 0; i < N; i++)
        {
            if (shortStrHash[i] == shortStrHash[N - 1 - i])
                ctr++;
        }
    }

    {
        ScopeTimer timer(&timeLongStr);

        for (size_t i = 0; i < N; i++)
        {
            if (longStr[i] == longStr[N - 1 - i])
                ctr++;
        }
    }

    {
        ScopeTimer timer(&timeLongStrHash);

        for (size_t i = 0; i < N; i++)
        {
            if (longStrHash[i] == longStrHash[N - 1 - i])
                ctr++;
        }
    }

    // keep the for loop from being stripped in release build.
    std::cout << ctr << std::endl;

    delete[] longStr;
    delete[] longStrHash;
    delete[] shortStr;
    delete[] shortStrHash;

    std::cout << "Startup time " << timeStartup << std::endl;
    std::cout << "Short String Cmp " << timeShortStr << std::endl;
    std::cout << "Short String Hash Cmp " << timeShortStrHash << std::endl;
    std::cout << "Long String Cmp " << timeLongStr << std::endl;
    std::cout << "Long String Hash Cmp " << timeLongStrHash << std::endl;
}

int main()
{
    BenchStringHash();
}