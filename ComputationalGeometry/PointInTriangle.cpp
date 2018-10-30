#include <iostream>
#include <vector>
using std::vector;
using std::cin;
using std::cout;
using std::endl;

// Point in 2D space or a vector in 2D space - depends on the interpretation
class Vector2D
{
public:
    double x;
    double y;
public:
    Vector2D(){x = 0.0f; y = 0.0;}
    Vector2D(double X, double Y){x = X; y = Y;}
   
    // Length of the vector that is cross product of this vector and the vector p
    double cross_product(const Vector2D & p) const
    {
        return this->x * p.y - this->y * p.x;
    }
};

double cross_product(const Vector2D & v1, const Vector2D & v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

class Vertice
{
public:
    Vector2D p1;
    Vector2D p2;
public:
    Vertice(){}
    Vertice(const Vector2D & p1, const Vector2D & p2){ this->p1 = p1; this->p2=p2; }
};

// Dot product
double operator*(const Vector2D & a, const Vector2D b)
{
    return a.x * b.x + a.y * b.y;
}

Vector2D operator-(const Vector2D & a, const Vector2D b)
{
    Vector2D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

// Triangle in 2D space
class Triangle
{
public:
    // Public for simplicity, no need of getters and setters for the demo
    Vector2D A, B, C;

public:
    Triangle(const Vector2D & a, const Vector2D & b, const Vector2D & c)
    {
        this->A = a;
        this->B = b;
        this->C = c;
    }
    // Returns whether Point p is within the triangle
    bool operator()(const Vector2D & P) const
    {
        Vector2D p = P - this->A;
        Vector2D c = this->C - this->A;
        Vector2D b = this->B - this->A;

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

class Polygon
{
private:
    bool vertices_calculated = false;
    vector<Vector2D> points;
    vector<Vertice> vertices;
    void get_vertices()
    {
        this->vertices.clear();
        vector<Vector2D>::iterator it = points.begin();
        Vector2D a = *it;
        while(it != points.end())
        {
            ++it;
            Vector2D b = *it;
            this->vertices.push_back(Vertice(a, b));
            a = b;
        }
        return this->vertices;
    } 
public:
    Polygon(){}
    Polygon(const vector<Vector2D> & points) {this->points = points;}

    void add_point(const Vector2D & point) { this->points.push_back(point); this->vertices_calculated = false; }
    
    vector<Vector2D> get_points() const { return this->points; }

    vector<Vector2D> & get_points() {return this->points; }

    // Check if point p is in the polygon using ray casting algorithm
    bool ray_casting_check(const Vector2D & p)
    {
        size_t intersections = 0;
        
        if(!this->vertices_calculated){ this->get_vertices(); }

        for(size_t i = 0; i < this->vertices.size(); ++i)
        {
            if(p.y == this->vertices[i].p1.y && p.y == this->vertices[i].p2.y) // sliding on the vertex
            {
                if(intersections > 0)
                {
                    --i;
                }
                continue;
            }

            if(p.y == this->vertices[i].p2.y) { ++intersections; continue; }

            if(p.y == this->vertices[i].p1.y) { continue; }

            if(p.y > this->vertices[i].p1.y 
                && p.y < this->vertices[i].p2.y 
                && cross_product(this->vertices[i].p1 - p, this->vertices[i].p2 - p) > 0)
            {
                ++intersections;
            }
        }
        return intersections % 2 != 0;
    }
    
};

int main()
{
    Triangle t(Vector2D(0.0, 0.0), Vector2D(3.0, 3.0), Vector2D(3.0, 0.0));
    Vector2D p;
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