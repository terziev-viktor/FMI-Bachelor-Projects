#include <iostream>
using std::cin;
using std::cout;
using std::endl;


typedef unsigned long long int big_int;

big_int lazy_power(big_int x, big_int n)
{
    for(big_int i = 0; i < n - 1; ++i)
    {
        x *= x;
    }
    return x;
}

big_int fast_power(big_int x, big_int n)
{
    if(n == 0)
    {
        return 1;
    }
    if(n == 1)
    {
        return x;
    }
    big_int tmp = n / 2;
    if(n % 2 == 0)
    {
        return fast_power(x * x, tmp);
    }
    else
    {
        return x * fast_power(x * x, tmp);
    }
}

int main()
{
    big_int x, n;
    cout << "x = ";
    cin >> x;
    cout << "n = ";
    cin >> n;
    cout << "x^n = " << fast_power(x, n);
    return 0;
}