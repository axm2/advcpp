#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
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

class VNT
{
private:
    SM<int> *vnt;
    int max_m;
    int max_n;

    void bubbleUp()
    {
        // after inserting, we need to bubble the value up
        // we only call this after adding a new value, so we start at bottom right
        // non recursive way
        // 2 loops one for x direction and one for y direction
        int i = max_m - 1;
        int j = max_n - 1;
        while (j != 0 && i != 0 && (((*vnt)[i][j] < (*vnt)[i][j - 1]) || ((*vnt)[i][j] < (*vnt)[i - 1][j])))
        {
            // prefer to swap the bigger
            if (((*vnt)[i][j - 1] > (*vnt)[i - 1][j]))
            {

                if (j != 0 && ((*vnt)[i][j] < (*vnt)[i][j - 1]))
                {
                    swap(i, j, i, j - 1);
                    j--;
                }
            }
            else if (i != 0 && ((*vnt)[i][j] < (*vnt)[i - 1][j]))
            {
                swap(i, j, i - 1, j);
                i--;
            }
        }
        while (i != 0 && ((*vnt)[i][j] < (*vnt)[i - 1][j]))
        {
            swap(i, j, i - 1, j);
            i--;
        }
        while (j != 0 && ((*vnt)[i][j] < (*vnt)[i][j - 1]))
        {
            swap(i, j, i, j - 1);
            j--;
        }
        return;
    }

    /**
     * @brief swap value of VNT[i][j] into VNT[k][l]
     * 
     * @param i 
     * @param j 
     * @param k 
     * @param l 
     */
    void swap(int i, int j, int k, int l)
    {
        int temp = (*vnt)[i][j];
        (*vnt)[i][j] = (*vnt)[k][l];
        (*vnt)[k][l] = temp;
        return;
    }

    void bubbleDown()
    {
        int i = 0;
        int j = 0;
        // if we haven't hit a wall and we still need to swap
        while (i != max_m - 1 && j != max_n - 1 && ((*vnt)[i][j] > (*vnt)[i + 1][j] || (*vnt)[i][j] > (*vnt)[i][j + 1]))
        {
            // prefer to swap the smaller
            if ((*vnt)[i][j + 1] < (*vnt)[i + 1][j])
            {
                if ((*vnt)[i][j] > (*vnt)[i][j + 1])
                {
                    swap(i, j, i, j + 1);
                    j++;
                }
            }
            else if ((*vnt)[i][j] > (*vnt)[i + 1][j])
            {
                swap(i, j, i + 1, j);
                i++;
            }
        }
        // if we hit a wall and still need to swap
        while (i != max_m - 1 && (*vnt)[i][j] > (*vnt)[i + 1][j])
        {
            swap(i, j, i + 1, j);
            i++;
        }
        while (j != max_n - 1 && (*vnt)[i][j] > (*vnt)[i][j + 1])
        {
            swap(i, j, i, j + 1);
            j++;
        }
        return;
    }

public:
    VNT(int m, int n)
    {
        max_m = m;
        max_n = n;
        vnt = new SM<int>(m, n);
        // fill vnt with int_max
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // INT_MAX might be compiler dependent. I'm using clang here
                (*vnt)[i][j] = INT32_MAX;
            }
        }
    }

    ~VNT()
    {
        delete vnt; // should call the safe matrix destructor i think
    }

    void add(int x)
    {
        (*vnt)[max_m - 1][max_n - 1] = x;
        bubbleUp();
        return;
    }

    int getMin()
    {
        // lowest val will always be here
        int result = (*vnt)[0][0];
        // remove the lowest value
        (*vnt)[0][0] = INT32_MAX;
        bubbleDown();
        return result;
    }

    SA<int> &operator[](int i) const
    {
        return (*vnt)[i];
    }

    void sort(int k[], int size)
    {
        for (int i = 0; i < size; i++)
        {
            add(k[i]);
        }
        for (int i = 0; i < size; i++)
        {
            k[i] = getMin();
        }
        return;
    }

    bool find(int x)
    {
        vector<int> vect;
        vect.push_back(getMin());
        while (vect.back() != INT32_MAX && vect.size() != max_m * max_n)
        {
            if (vect.back() == x)
            {
                for (int x : vect)
                {
                    add(x);
                }
                return true;
            }
            vect.push_back(getMin());
        }
        if (vect.size() == max_m * max_n && vect.back() == x)
        {
            for (int x : vect)
            {
                add(x);
            }
            return true;
        }

        for (int x : vect)
        {
            add(x);
        }
        return false;
    }
};

int main()
{
    // demonstrating creating an mxn vnt object
    VNT A(4, 4);
    // demonstrating adding ints to vnt object
    A.add(125);
    for (int i = 0; i < 15; i++)
    {
        A.add(rand() % 100);
    }
    // demonstrating access to underlying safe matrix
    cout << "Printing VNT A: " << endl;
    cout << A[0][0] << " " << A[0][1] << " " << A[0][2] << " " << A[0][3] << endl
         << A[1][0] << " " << A[1][1] << " " << A[1][2] << " " << A[1][3] << endl
         << A[2][0] << " " << A[2][1] << " " << A[2][2] << " " << A[2][3] << endl
         << A[3][0] << " " << A[3][1] << " " << A[3][2] << " " << A[3][3] << endl;
    // demonstrating extracting minimum value from vnt object
    cout << "Removing the minimum value from VNT A which is " << A.getMin() << endl;
    cout << endl;
    VNT B(3, 3);
    int k[] = {1, 2, 3, 7, 8, 9, -1, -5, 25};
    cout << "Unsorted array: ";
    for (int x : k)
    {
        cout << x << " ";
    }
    // demonstrating sort using only vnt object
    B.sort(k, 9);
    cout << endl;
    cout << "Sorted array: ";
    for (int x : k)
    {
        cout << x << " ";
    }
    cout << endl;
    cout << endl;
    cout << "Reinserting 15 into VNT A" << endl;
    A.add(15);
    // demonstrating find VNT function
    cout << "Checking if 125 is in VNT A" << endl;
    A.find(125) ? cout << "Found 125! " << endl : cout << "Didn't find 125";
    cout << "Checking if -5 is in VNT A" << endl;
    A.find(-5) ? cout << "Found -5! " << endl : cout << "Didn't find -5" << endl
                                                     << endl;

    // demonstrating that find function does not modify VNT
    cout << A[0][0] << " " << A[0][1] << " " << A[0][2] << " " << A[0][3] << endl
         << A[1][0] << " " << A[1][1] << " " << A[1][2] << " " << A[1][3] << endl
         << A[2][0] << " " << A[2][1] << " " << A[2][2] << " " << A[2][3] << endl
         << A[3][0] << " " << A[3][1] << " " << A[3][2] << " " << A[3][3] << endl;
    return 0;
}