#include <iostream>
#include <stdlib.h>
#include <memory>
using namespace std;

/**
 * @brief 
 * Each allocated memory block will have a 32 byte header and a 16 byte footer
 * The header consists of LLINK, TAG, SIZE, RLINK
 * Footer consists of TAG AND UPLINK
 */

class block
{
private:
	// total memory that block class controls
	// we use a smart pointer to avoid memory leak
	//inline static unique_ptr<char[]> pool = make_unique<char[]>((20000));
	inline static unique_ptr<char[]> pool = [] {
		unique_ptr<char[]> pool = make_unique<char[]>(20000);
		void *dp = (void *)pool.get();
		// START OF POOL // cout << dp << endl;
		// initialize list here
		// LLINK
		*static_cast<void **>(dp) = static_cast<void *>(static_cast<char *>(dp) + sizeof(dp) + sizeof(int) + sizeof(int) + sizeof(dp) + sizeof(dp) + sizeof(int) + sizeof(int));
		dp = static_cast<char *>(dp) + sizeof(dp);
		//*(void **)dp = static_cast<void *>(dp);
		// TAG
		*static_cast<int *>(dp) = 0;
		dp = static_cast<char *>(dp) + sizeof(int);

		// SIZE
		*static_cast<int *>(dp) = 0;
		dp = static_cast<char *>(dp) + sizeof(int);

		//RLINK
		*static_cast<void **>(dp) = static_cast<void *>(static_cast<char *>(dp) + sizeof(dp));
		dp = static_cast<char *>(dp) + sizeof(dp);

		// LLINK
		*static_cast<void **>(dp) = static_cast<void *>(static_cast<char *>(dp) - sizeof(dp));
		dp = static_cast<char *>(dp) + sizeof(dp);

		// TAG
		*static_cast<int *>(dp) = 0;
		dp = static_cast<char *>(dp) + sizeof(int);

		// SIZE
		int size = 20000 - 5 * (sizeof(int)) - 5 * (sizeof(dp));
		*static_cast<int *>(dp) = size;
		dp = static_cast<char *>(dp) + sizeof(int);

		// RLINK
		*static_cast<void **>(dp) = static_cast<void *>(static_cast<char *>(dp) - sizeof(dp) - sizeof(int) - sizeof(int) - sizeof(dp) - sizeof(dp) - sizeof(int) - sizeof(int));

		// move dp to end of block
		// reset to beginning by getting, then move it
		dp = (void *)pool.get();
		dp = static_cast<char *>(dp) + 20000 - (sizeof(dp) + sizeof(int));
		// TAG
		*static_cast<int *>(dp) = 0;
		dp = static_cast<char *>(dp) + sizeof(int);

		// UPLINK
		*static_cast<void **>(dp) = pool.get() + sizeof(dp) + sizeof(int) + sizeof(int) + sizeof(dp);
		return pool;
	}();
	inline static void *AV = pool.get();
	// stats
	inline static int numBlocks = 0;
	inline static int szAllocated = 0;
	inline static int avgSz = 0;
	inline static int freeBlocks = 0;
	int missCounter = 0;

public:
	// ptr to start of usable block
	// dumb pointer so we can do pointer arithmetic
	void *p = NULL;


	block(int low, int high, int sizeOfT)
	{
		int space = (high - low + 1) * sizeOfT;
		if(szAllocated<20000 && ALLOCATE(space)){
			numBlocks++;
			szAllocated+=space;
			cout << "Blocks allocated: " << numBlocks << endl;
			cout << "Space allocated: " << szAllocated << " bytes" << endl;
			avgSz = szAllocated/numBlocks;
			cout << "Average block size on list " << avgSz << "bytes" << endl;
			cout << "Blocks on the free list " << freeBlocks << endl;
			cout << "Misses before we found a suitable block " << missCounter << endl;
		}
		else cout << "No space can be allocated!" << endl;
		
	}

	bool ALLOCATE(int n)
	{
		// pool = AV = q
		// p = p

		// SET P TO RLINK
		p = static_cast<char *>(AV) + sizeof(p) + sizeof(int) + sizeof(int);
		p = *static_cast<void **>(p);

		// IF WE LOOP AROUND THE WHOLE LIST, STOP
		//bool flag = true;
		while (p)
		{
			int size = *(static_cast<int *>(static_cast<void *>(static_cast<char *>(AV) + sizeof(int))));
			//cout << size << endl;
			if (size >= n)
			{
				int diff = size - n;
				// allocate lower N words
				// size(p) = diff
				*(static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(int)))) = diff;
				// uplink(p + diff -1) = p
				*static_cast<void **>(static_cast<void *>(static_cast<char *>(p) + sizeof(int) + sizeof(int) + sizeof(p) + diff + sizeof(int))) = p;
				// set upper portion as unused
				// TAG(p+diff-1) = 0
				*static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(int) + sizeof(int) + sizeof(p) + diff)) = 0;
				// AV = p
				AV = p;
				// p = p+diff
				p = static_cast<char *>(p)+diff;
				// size(p) = n
				*(static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(int)))) = diff;
				// tag(p) = tag(p+n-1) = 1
				*static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(int) + sizeof(int) + sizeof(p) + n)) = 1;
				return true;
			}
			// p = RLINK(p)
			p = *static_cast<void **>(static_cast<void *>(static_cast<char *>(p) + sizeof(int) + sizeof(int)));
			missCounter++;
			//flag = false;
		}
		return false;
	}
};

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
		block z (0,high, sizeof(int));
		p = (T*)z.p;
        //p = new T[i];
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
        //delete[] p;
		// This should call free instead
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
}
ostream &operator<<(ostream &os, SA<double> s)
{
    int size = s.high - s.low + 1;
    for (int i = 0; i < size; i++)
        cout << s.p[i] << " ";
    return os;
}

int main(void)
{
	cout << "------Initializing SA by using a block------" << endl;
	SA<int> arr(3);
	cout << "------Placing a value 5 into the SA------" << endl;
	arr[0] = 5;
	cout << "------Printing the value of SA[0]------" << endl;
	cout << arr[0] << endl;
	cout << "------Attempting to make 100 blocks of random size------" << endl;
	for (int i=0;i<100;i++){
		int low = rand() % 100;
		int high = low + rand() % 120;
		block b (low, high, sizeof(int));
	}

}
