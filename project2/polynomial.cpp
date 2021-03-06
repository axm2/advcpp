// Polynomial math without STL

#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// We want to do math on pairs.

void canonical(int *arr, int size);
void print(int *arr, int size);
string flip(string s);

int main()
{
    // Printing original polynomials -----------------------------------------------------------------
    int counter = 0;
    const char *filename = "input.txt";
    ifstream infile(filename);
    int i;
    string s;
    cout << "The inputs : " << endl;
    while (!infile.eof())
    {
        getline(infile, s);
        stringstream t(s);
        while (t >> i)
        {
            if (counter % 2 == 0) // Coeff
            {
                if (counter ==0){
                    cout << i;
                }
                else if (i == 1)
                {
                    cout;
                }
                else if (i == 0)
                {
                    cout << i;
                }
                else if (i > 0)
                {
                    cout << "+" << i;
                }
                else if (i < 0)
                {
                    cout << i;
                }
            }
            else // Exponent
            {
                if (i == 1)
                {
                    cout << "x";
                }
                else if (i != 0)
                {
                    cout << "x^" << i;
                }
            }
            counter++;
        }
        cout << endl;
    }
    cout << endl;

    //Printing canonical polynomials ------------------------------------------------------------------

    ifstream infile2(filename);
    string p1;
    cout << "Canonical form: " << endl;
    int j;
    while (!infile2.eof())
    {
        int arrindex = 0;
        int *arr;
        getline(infile2, p1);
        stringstream t(p1);
        int coeffAndExpCount = 0;
        for (char const &c : p1)
        {
            if (isspace(c))
            {
                coeffAndExpCount++;
            }
        }
        coeffAndExpCount++;
        arr = new int[coeffAndExpCount]();
        // Fill array
        while (t >> j)
        {
            arr[arrindex++] = j;
        }
        canonical(arr, coeffAndExpCount);
        delete[] arr;
    }
    cout << endl;

    // printing summed pairs ---------------------------------------------------

    ifstream infile3(filename);
    string s1;
    string s2;
    string s3;
    int k;
    cout << "Sum: ";
    while (!infile3.eof())
    {
        int arrindex = 0;
        int *arr;
        getline(infile3, s1);
        getline(infile3, s2);
        s3 = s1 + " " + s2;
        stringstream t(s3);
        int coeffAndExpCount = 0;
        for (char const &c : s3)
        {
            if (isspace(c))
            {
                coeffAndExpCount++;
            }
        }
        coeffAndExpCount++;
        arr = new int[coeffAndExpCount]();
        // Fill array
        while (t >> k)
        {
            arr[arrindex++] = k;
        }
        canonical(arr, coeffAndExpCount);
        delete[] arr;
    }
    cout << endl;

    // printing subtracted pairs ---------------------------------------------

    ifstream infile4(filename);
    string t1;
    string t2;
    string t3;
    int l;
    cout << "Difference: ";
    while (!infile4.eof())
    {
        int arrindex = 0;
        int *arr;
        getline(infile4, t1);
        getline(infile4, t2);
        // flip the signs of one
        string flipt2 = flip(t2);
        t3 = t1 + " " + flipt2;
        stringstream t(t3);
        int coeffAndExpCount = 0;
        for (char const &c : t3)
        {
            if (isspace(c))
            {
                coeffAndExpCount++;
            }
        }
        coeffAndExpCount++;
        arr = new int[coeffAndExpCount]();
        // Fill array
        while (t >> l)
        {
            arr[arrindex++] = l;
        }
        canonical(arr, coeffAndExpCount);
        delete[] arr;
    }
    cout << endl;

    ifstream infile5(filename);
    string u1;
    string u2;
    string u3;
    int m;
    cout << "Product: ";
    while(!infile5.eof())
    {
        int arrindex = 0;
        int smallarr1index = 0;
        int smallarr2index = 0;
        int *arr;
        int *smallarr1;
        int *smallarr2;
        int x1;
        int x2;
        getline(infile5, u1);
        getline(infile5, u2);
        stringstream v1(u1);
        stringstream v2(u2);
        int coeffAndExpCount1 = 0;
        int coeffAndExpCount2 = 0;
        for (char const &c : u1)
        {
            if (isspace(c))
            {
                coeffAndExpCount1++;
            }
        }
        coeffAndExpCount1++;
        for (char const &d : u2)
        {
            if (isspace(d))
            {
                coeffAndExpCount2++;
            }
        }
        coeffAndExpCount2++;

        smallarr1 = new int[coeffAndExpCount1]();
        smallarr2 = new int[coeffAndExpCount2]();
        while( v1 >> x1)
        {
            smallarr1[smallarr1index++] = x1;
        }
        while ( v2 >> x2)
        {
            smallarr2[smallarr2index++] = x2;
        }

        int biggerCoeffAndExpCount = (coeffAndExpCount1*coeffAndExpCount2);
        arr = new int[biggerCoeffAndExpCount]();
        int outercounter=0;
        for (int i=0;i<coeffAndExpCount1;i=i+2){
            for(int j=0;j<coeffAndExpCount2;j=j+2){
                arr[outercounter] = smallarr1[i] * smallarr2[j];
                outercounter++;
                arr[outercounter] = smallarr1[i+1] + smallarr2[j+1];
                outercounter++;
            }
        }
        canonical(arr, biggerCoeffAndExpCount);
        delete[] arr;
        delete[] smallarr1;
        delete[] smallarr2;

    }
    cout << endl;
}

void print(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (i % 2 == 0)
        { // coeff
            if (arr[i] == 1)
            {
                cout;
            }
            else if (arr[i] > 0)
            {
                if (i == 0)
                {
                    cout << arr[i];
                }
                else
                {
                    cout << "+" << arr[i];
                }
            }
            else if (arr[i] < 0)
            {
                cout << arr[i];
            }
        }
        else // exponent
        {
            if (arr[i - 1] == 0)
            {
                cout;
            }
            else if (arr[i] == 1)
            {
                cout << "x";
            }
            else if (arr[i] != 0)
            {
                cout << "x^" << arr[i];
            }
        }
    }
    cout << endl;
}

void canonical(int *arr, int size)
{
    // Pairs are coeff and exp pairs.
    // Work from the back to the front.
    // canonical also needs to sort

    // sort
    for (int i = 1; i < size; i = i + 2)
    {
        for (int j = i + 2; j < size; j = j + 2)
        {
            if (arr[i] < arr[j])
            {
                int coefftemp = arr[i - 1];
                int exptemp = arr[i];
                arr[i] = arr[j];
                arr[i - 1] = arr[j - 1];
                arr[j - 1] = coefftemp;
                arr[j] = exptemp;
            }
        }
    }

    // consolidate
    int uniques = 0;
    for (int i = 1; i < size; i = i + 2)
    {
        while (i < size - 1 && arr[i] == arr[i + 2])
        {
            i = i + 2;
        }
        uniques++;
    }
    int *consolidated_coeff_arr = new int[uniques]();
    int *consolidated_exp_arr = new int[uniques]();
    int uniquecounter = 0;
    for (int i = 1; i < size; i = i + 2)
    { // duplicates and non duplicates
        consolidated_coeff_arr[uniquecounter] += arr[i - 1];
        while (i < size - 1 && arr[i] == arr[i + 2])
        { // duplicates
            consolidated_coeff_arr[uniquecounter] += arr[i + 2 - 1];
            i = i + 2;
        }
        consolidated_exp_arr[uniquecounter] = arr[i];
        uniquecounter++;
    }
    int *consolidated_arr = new int[uniques * 2]();
    int outercounter = 0;
    for (int i = 0; i < uniques * 2; i++)
    {
        if (i % 2 == 0)
        { // coeff
            consolidated_arr[i] = consolidated_coeff_arr[outercounter];
        }
        else
        {
            consolidated_arr[i] = consolidated_exp_arr[outercounter];
            outercounter++;
        }
    }
    delete[] consolidated_coeff_arr;
    delete[] consolidated_exp_arr;
    print(consolidated_arr, uniques * 2);
    delete[] consolidated_arr;
}

string flip(string s)
{
    int i;
    string result;
    stringstream t(s);
    int counter = 0;
    while (t >> i)
    {
        if (counter % 2 == 0)
        {
            result = result + to_string(i * -1) + ' ';
        }
        else
        {
            result = result + to_string(i) + ' ';
        }

        counter++;
    }
    return result;
}