#include <iostream>
#include <vector>
using std::vector;
using std::cin;
using std::cout;
using std::endl;

// Point in 2D space or a vector in 2D space - depends on the interpretation
struct Vector2
{
    Vector2(){x = 0.0f; y = 0.0;}
    Vector2(double X, double Y){x = X; y = Y;}
    double x;
    double y;
};

// Dot product
double operator*(const Vector2 & a, const Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

Vector2 operator-(const Vector2 & a, const Vector2 b)
{
    Vector2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

// Triangle in 2D space
class Triangle
{
public:
    // Public for simplicity, no need of getters and setters for the demo
    Vector2 A, B, C;

public:
    Triangle(const Vector2 & a, const Vector2 & b, const Vector2 & c)
    {
        this->A = a;
        this->B = b;
        this->C = c;
    }
    // Returns whether Point p is within the triangle
    bool operator()(const Vector2 & P)
    {
        Vector2 p = P - this->A;
        Vector2 c = this->C - this->A;
        Vector2 b = this->B - this->A;

        // Our vector p is a linear combination of c and b with coefficients v and w
        // So p = v*c + w*b for some v and w
        long double v, w;
        double cSq = c*c;
        double bSq = b*b;

        double BdotC = b*c;
        v = ((cSq)*(p*b) - (c*b)*(p*c) ) / ( (cSq*bSq) - (c*b)*(b*c) );
        w = (bSq*(p*c)-(b*c)*(p*b)) / (cSq*bSq - (c*b)*(b*c) );
        if(v < 0.0 || w < 0.0)
        {
            return false;
        }
        if(v > 1.0 || w > 1.0)
        {
            return false;
        }
        if((v + w) > 1.0)
        {
            return false;
        }
        return true;
    }
};

int main()
{
    Triangle t(Vector2(0.0, 0.0), Vector2(3.0, 3.0), Vector2(3.0, 0.0));
    Vector2 p;
    int n = 20;
    while(--n)
    {
        cin >> p.x;
        cin >> p.y;
        if(t(p))
        {
            cout << "true" << endl;
        }
        else
        {
            cout << "false" << endl;
        }
    }

    return 0;
}