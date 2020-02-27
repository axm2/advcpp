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
        cout << s.p[i];
    return os;
};
ostream &operator<<(ostream &os, SA<double> s)
{
    int size = s.high - s.low + 1;
    for (int i = 0; i < size; i++)
        cout << s.p[i];
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

    SM(int hx, int hy)
    {
        lowx = 0;
        lowy = 0;
        highx = hx;
        highy = hy;
        matrix = new SA<SA<T>>(hx);
    }

    SM(int lx, int hx, int ly, int hy)
    {
        lowx = lx;
        highx = hx;
        lowy = ly;
        highy = hy;
        matrix = new SA<SA<T>>(lx, hx);
    }

    SM(const SM &s)
    {
        int sizex = s.highx - s.lowx + 1;
        int sizey = s.highy - s.lowy + 1;
        matrix = new SA<SA>(s.lowx, s.highx); // do I need to do SA<SA<T>> or do I do SA<SA>
        for(int i=0;i<sizex;i++){
            matrix[i] = new SA<T>(s.lowy, s.highy);
        }
        lowx = s.lowx;
        highx = s.highx;
        lowy = s.lowy;
        highy = s.highy;

    }

    // Don't need an explicit destructor because we already wrote a SA destructor
    // Don't need to overload brackets, because SA operator already overloaded
    // TODO: Need to overload '='
    // TODO: Need to overload '*'
    // TODO: Need to set left shift operator as a friend function
};

int main()
{
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
    //b[0] = 4;
    cout << a[0] << endl;
    cout << a[1] << endl;
    cout << a[2] << endl;
    //cout << b << endl;

    return 0;
}