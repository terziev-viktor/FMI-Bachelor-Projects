#include <iostream>
using std::cin;
using std::cout;
using std::endl;

typedef unsigned long long big_int;

class Matrix2x2
{
public:
    big_int m[2][2];

    Matrix2x2(big_int r1c1, big_int r1c2, big_int r2c1, big_int r2c2)
    {
        m[0][0] = r1c1;
        m[0][1] = r1c2;
        m[1][0] = r2c1;
        m[1][1] = r2c2;
    }
    
    Matrix2x2& operator*=(const Matrix2x2& other)
    {
        big_int m00, m01, m10, m11;
        m00 = this->m[0][0];
        m01 = this->m[0][1];
        m10 = this->m[1][0];
        m11 = this->m[1][1];
        this->m[0][0] = m00 * other.m[0][0] + m01 * other.m[1][0];
        this->m[0][1] = m00 * other.m[0][1] + m01 * other.m[1][1];
        this->m[1][0] = m10 * other.m[0][0] + m11 * other.m[1][0];
        this->m[1][1] = m10 * other.m[0][1] + m11 * other.m[1][1];
        return *this;
    }

};

big_int lazy_fibonacci(big_int n)
{
    if(n == 0)
    {
        return 0;
    }
    if(n == 1)
    {
        return 1;
    }
    if(n == 2)
    {
        return 2;
    }
    return lazy_fibonacci(n - 1) + lazy_fibonacci(n - 2);
}

big_int fast_fibonacci(big_int n)
{
    if(n == 0)
    {
        return 0;
    }
    if(n == 1)
    {
        return 1;
    }
    big_int a, b, c;
    a = 0;
    b = 1;
    for(big_int i = 0; i < n - 1; ++i)
    {
        c = a + b;
        a = b;
        b = c;
    }

    return c;
}

// This algorithm is based on an innocent-looking identity(which can be proven by mathematical induction)
// [1 1; 1 0]^n = [F(n + 1) F(n); F(n) F(n - 1)]
// where F(n) is the fibonacci function
// this method gives us complexity of O(logN)

big_int matrix_exponential_fibonacci(big_int n)
{
    if(n == 0)
    {
        return 0;
    }
    if(n == 1)
    {
        return 1;
    }
    Matrix2x2 one(1,1,1,0);
    Matrix2x2 result(1,1,1,0);
    // using fast_power(x, n) would give us the best result
    for(big_int i = 0; i < n - 2; ++i)
    {
        result *= one;
    }
    //----------------------------------------------------
    return result.m[0][0];
}


int main()
{
    big_int n, p;
    cout << "n = ";
    cin >> n;
    cout << "working..." << endl;
    big_int fib = matrix_exponential_fibonacci(n);
    cout << fib;

    return 0;
}