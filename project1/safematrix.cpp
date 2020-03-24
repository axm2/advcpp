#include <iostream>
#include <cstdlib>
#include <cassert>
using namespace std;

template <class T>
class SA
{
private:
    int low, high;
    T *p;

public:
    // default constructor
    // allows for writing things like SA a;

    SA()
    {
        low = 0;
        high = -1;
        p = NULL;
    }

    // 2 parameter constructor lets us write
    // SA x(10,20);

    SA(int l, int h)
    {
        if ((h - l + 1) <= 0)
        {
            cout << "constructor error in bounds definition" << endl;
            exit(1);
        }
        low = l;
        high = h;
        p = new T[h - l + 1];
    }

    // single parameter constructor lets us
    // create a SA almost like a "standard" one by writing
    // SA x(10); and getting an array x indexed from 0 to 9

    SA(int i)
    {
        low = 0;
        high = i - 1;
        p = new T[i];
    }
    // copy constructor for pass by value and
    // initialization

    SA(const SA &s)
    {
        int size = s.high - s.low + 1;
        p = new T[size];
        for (int i = 0; i < size; i++)
            p[i] = s.p[i];
        low = s.low;
        high = s.high;
    }
    // destructor

    ~SA()
    {
        delete[] p;
    }
    //overloaded [] lets us write
    //SA x(10,20); x[15]= 100;

    T &operator[](int i)
    {
        if (i < low || i > high)
        {
            cout << "index " << i << " out of range" << endl;
            exit(1);
        }
        return p[i - low];
    }

    // overloaded assignment lets us assign
    // one SA to another

    SA &operator=(const SA &s)
    {
        if (this == &s)
            return *this;
        delete[] p;
        int size = s.high - s.low + 1;
        p = new int[size];
        for (int i = 0; i < size; i++)
            p[i] = s.p[i];
        low = s.low;
        high = s.high;
        return *this;
    }

    // overloads << so we can directly print SAs

    friend ostream &operator<<(ostream &os, SA s);
};
ostream &operator<<(ostream &os, SA<int> s)
{
    int size = s.high - s.low + 1;
    for (int i = 0; i < size; i++)
        cout << s.p[i] << " ";
    return os;
};
ostream &operator<<(ostream &os, SA<double> s)
{
    int size = s.high - s.low + 1;
    for (int i = 0; i < size; i++)
        cout << s.p[i] << " ";
    return os;
};

template <class T>
class SM
{
private:
    int lowx, highx, lowy, highy;
    SA<SA<T>> *matrix;

public:
    SM()
    {
        lowx = 0;
        lowy = 0;
        highx = -1;
        highy = -1;
        matrix = NULL;
    }

    // double parameter constructor lets us
    // create a SM almost like a standard one by writing
    // SM x(10,10); and getting a 10x10 matrix indexed from 0 to 9
    SM(int hx, int hy)
    {
        lowx = 0;
        lowy = 0;
        highx = hx - 1;
        highy = hy - 1;
        matrix = new SA<SA<T>>(hx);
        for (int i = 0; i < hx; i++)
        {
            SA<T> innerSA(hy);
            (*matrix)[i] = innerSA;
        }
    }

    // quad parameter constructor lets us
    // write SM x(10,20);
    SM(int lx, int hx, int ly, int hy)
    {
        lowx = lx;
        highx = hx;
        lowy = ly;
        highy = hy;
        matrix = new SA<SA<T>>(lx, hx);
        for (int i = lx; i <= hx; i++)
        {
            SA<T> innerSA(ly, hy);
            (*matrix)[i] = innerSA;
        }
    }

    // copy constructor
    SM(const SM &s)
    {
        lowx = s.lowx;
        highx = s.highx;
        lowy = s.lowy;
        highy = s.highy;
        matrix = new SA<SA<T>>(lowx, highx);
        for (int i = lowx; i <= highx; i++)
        {
            SA<T> innerSA(s.lowy, s.highy);
            for (int j = lowy; j <= highy; j++)
            {
                innerSA[j] = (*s.matrix)[i][j];
            }
            (*matrix)[i] = innerSA;
        }
    }

    SA<T> &operator[](int i) const
    {
        return (*matrix)[i];
    }

    // overloaded assignment lets us assign
    // one SM to another
    SM &operator=(const SM &s)
    {
        if (this == &s)
            return *this;
        delete matrix;
        lowx = s.lowx;
        highx = s.highx;
        lowy = s.lowy;
        highy = s.highy;
        matrix = new SA<SA<T>>(s.lowx, s.highx);
        for (int i = s.lowx; i <= s.highx; i++)
        {
            SA<T> innerSA(s.lowy, s.highy);
            for (int j = s.lowy; j <= s.highy; j++)
            {
                innerSA[j] = (*s.matrix)[i][j];
            }
            (*matrix)[i] = innerSA;
        }
        return *this;
    }

    SM operator*(const SM &s) const
    {

        int r1 = highx - lowx + 1;
        int c1 = highy - lowy + 1;
        int r2 = s.highx - s.lowx + 1;
        int c2 = s.highy - s.lowy + 1;
        int i, j, k;
        SM<T> *product = new SM<T>(lowx, highx, s.lowy, s.highy);
        if (c1 != r2)
        {
            cout << "Column of the first matrix should be equal to row of second matrix";
        }
        else
        {
            for (i = lowx; i <= highx; ++i)
                for (j = s.lowy; j <= s.highy; ++j)
                {
                    (*product)[i][j] = 0;
                }
            for (i = lowx; i <= highx; ++i)
                for (j = s.lowy; j <= s.highy; ++j)
                    for (k = lowy; k <= highy; ++k)
                    {
                        (*product)[i][j] += (*this)[i][k] * (*s.matrix)[k][j];
                    }
        }
        SM<T> ans(lowx, highx, s.lowy, s.highy);
        for (int i = lowx; i <= highx; i++)
        {
            for (int j = s.lowy; j <= s.highy; j++)
            {
                ans[i][j] = (*product)[i][j];
            }
        }
        delete product;
        return ans;
    }

    //destructor
    ~SM()
    {
        delete matrix; // deletes the outer matrix; the inner matrices are auto destroyed
    }

    friend ostream &operator<<(ostream &os, SM s);
};
ostream &operator<<(ostream &os, SM<int> s)
{
    for (int i = s.lowx; i <= s.highx; i++)
    {
        for (int j = s.lowy; j <= s.highy; j++)
        {
            cout << s[i][j] << " ";
        }
        cout << endl;
    }
    return os;
};
ostream &operator<<(ostream &os, SM<double> s)
{
    for (int i = s.lowx; i <= s.highx; i++)
    {
        for (int j = s.lowy; j <= s.highy; j++)
        {
            cout << s[i][j] << " ";
        }
    }
    return os;
};

int main()
{
    // Making a safe array of safearrays without using safe matrix class
    SA<SA<int>> a(3);
    SA<int> b(3), c(3), d(3);
    a[0] = b;
    a[1] = c;
    a[2] = d;
    a[0][0] = 1;
    a[0][1] = 2;
    a[0][2] = 3;
    a[1][0] = 4;
    a[1][1] = 5;
    a[1][2] = 6;
    a[2][0] = 7;
    a[2][1] = 8;
    a[2][2] = 9;
    // printing using safe array's left shift operator
    cout << "Printing matrix A" << endl;
    cout << a[0] << endl;
    cout << a[1] << endl;
    cout << a[2] << endl;
    cout << endl;
    // Creating a safe array of safe arrays using the safe matrix's 2 parameter constructor
    SM<int> f(3, 3);
    int q = 1;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            f[i][j] = q++;
        }
    }
    cout << "Printing matrix F" << endl;
    cout << f[0] << endl;
    cout << f[1] << endl;
    cout << f[2] << endl;
    cout << endl;
    // Creating another using SM class using 2 param constructor
    SM<int> g(2, 4);
    g[0][0] = 3;
    g[0][1] = 2;
    g[0][2] = 1;
    g[0][3] = 5;
    g[1][0] = 9;
    g[1][1] = 1;
    g[1][2] = 3;
    g[1][3] = 0;
    // Printing using SM left shift operator
    cout << "Printing matrix G" << endl;
    cout << g;
    cout << endl;
    // Creating using 4 param constructor
    SM<int> h(0, 1, 0, 3);
    h[0][0] = 3;
    h[0][1] = 2;
    h[0][2] = 1;
    h[0][3] = 5;
    h[1][0] = 9;
    h[1][1] = 1;
    h[1][2] = 3;
    h[1][3] = 0;

    cout << "Printing matrix H" << endl;
    cout << h;
    cout << endl;
    // Creating safe matrix using assignment operator and copy constructor
    SM<int> i = h;
    cout << "Printing matrix I" << endl;
    cout << i;
    cout << endl;

    SM<int> threebyfour(0, 0, 0, 1);
    threebyfour[0][0] = 3;
    threebyfour[0][1] = 4;
    // Demonstrates multiply operator, assignment operator, and copy constructor
    SM<int> product = (threebyfour * h);
    // Demonstrates printing using the overloaded left shift for SM class
    cout << "Printing product matrix of threebyfour and H" << endl;
    cout << product << endl;
    cout << endl;

    return 0;
}
