#include <type_traits>
#include <iostream>
#include <algorithm>
#include <concepts>
#include <future>
#include <string>
#include <cmath>

using namespace std;

static double eps = numeric_limits<double>::epsilon();

int check(double x_sq, double x)
{
    double minus = abs(x_sq - x);

    if(minus < eps)
    {
        return 0;
    }
    if(x_sq > x)
    {
        return 1;
    }
    return -1;
}

double sq(double x)
{
    double low = 1.0;
    double high = x;

    int iterations = 128;
    double mid;
    while(iterations--)
    {
        mid = low + (high - low) / 2.0;
        int c = check(mid * mid, x);
        if(c == 0)
        {
            return mid;
        }
        if(c == 1)
        {
            high = mid;
        }
        else
        {
            low = mid;
        }
    }
    return mid;
}

auto main() -> int
{
    cout << sq(2.0) << endl;

    return 0;
}
