#include <iostream>
#include <stdlib.h>
#include <memory>
using namespace std;

/**
 * @brief 
 * Each memory block (except for head node) will have a 24 byte header and a 12 byte footer
 * The header consists of LLINK, TAG, SIZE, RLINK
 * Footer consists of TAG AND UPLINK
 */

class block
{
private:
	// total memory that block class controls
	// we use a smart pointer to avoid memory leak
	//inline static unique_ptr<char[]> pool = make_unique<char[]>((40000));
	inline static unique_ptr<char[]> pool = [] {
		unique_ptr<char[]> pool = make_unique<char[]>(40000);
		void *dp = (void *)pool.get();
		cout << "START OF POOL " << dp << endl;
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
		int size = 40000 - 5 * (sizeof(int)) - 5 * (sizeof(dp));
		*static_cast<int *>(dp) = size;
		dp = static_cast<char *>(dp) + sizeof(int);

		// RLINK
		*static_cast<void **>(dp) = static_cast<void *>(static_cast<char *>(dp) - sizeof(dp) - sizeof(int) - sizeof(int) - sizeof(dp) - sizeof(dp) - sizeof(int) - sizeof(int));
		dp = static_cast<char *>(dp) + sizeof(int);

		// move dp to end of block
		dp = static_cast<char *>(dp) + size;
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
	inline static int freeBlocks = 0;
	int missCounter = 0;

public:
	// ptr to start of usable block
	// dumb pointer so we can do pointer arithmetic
	void *p = NULL;

	block(int low, int high, int sizeOfT)
	{
		int space = (high - low + 1) * sizeOfT;
		if (ALLOCATE(space))
		{
			numBlocks++;
			szAllocated += space;
			cout << "Blocks allocated: " << numBlocks << endl;
			cout << "Space allocated: " << szAllocated << " bytes" << endl;
			int avgSz = szAllocated / numBlocks;
			cout << "Average block size on list " << avgSz << "bytes" << endl;
			cout << "Blocks on the free list " << freeBlocks << endl;
			cout << "Misses before we found a suitable block " << missCounter << endl;
		}
		else
			cout << "No space can be allocated!" << endl;
	}

	static void free(void *pp)
	{
		cout << "DESTROYING!" << endl;
		numBlocks--;
		freeBlocks++;
		int size = *(static_cast<int *>(static_cast<void *>(static_cast<char *>(pp) - sizeof(int) - sizeof(p))));
		szAllocated -= size;

		cout << "Blocks allocated: " << numBlocks << endl;
		cout << "Space allocated: " << szAllocated << " bytes" << endl;
		if (numBlocks > 0)
		{
			int avgSz = szAllocated / numBlocks;
			cout << "Average block size on list " << avgSz << "bytes" << endl;
		}
		cout << "Blocks on the free list " << freeBlocks << endl;

		// return a block beginning at p and of size size(p)
		// n = size(p)
		int n = *(static_cast<int *>(static_cast<void *>(static_cast<char *>(pp) - sizeof(int) - sizeof(p))));
		// check tags to see which blocks are available
		int LT = *(static_cast<int *>(static_cast<void *>(static_cast<char *>(pp) - sizeof(pp) - sizeof(int) - sizeof(int))));
		int RT = *(static_cast<int *>(static_cast<void *>(static_cast<char *>(pp) + n)));
		if (LT == 1 && RT == 1)
		{
			cout << "Left and right allocated" << endl;
			// CREATE BOTH BLANK TAGS
			*static_cast<int *>(static_cast<void *>(static_cast<char *>(pp) - sizeof(int) - sizeof(int) + sizeof(pp))) = 0; // upper tag
			*static_cast<int *>(static_cast<void *>(static_cast<char *>(pp) + n)) = 0;										//lower tag
			//create uplink UPLINK(p+n-1) = P
			*static_cast<void **>(static_cast<void *>(static_cast<char *>(pp) + n + sizeof(int))) = static_cast<char *>(pp) - sizeof(int) - sizeof(int) - sizeof(pp) - sizeof(pp);
			//create LLINK LLINK(p) = AV
			*static_cast<void **>(static_cast<void *>(static_cast<char *>(pp) - sizeof(pp) - sizeof(pp) - sizeof(int) - sizeof(int))) = AV;
			//create RLINK RLINK(p) = RLINK(AV)
			*static_cast<void **>(static_cast<void *>(static_cast<char *>(pp) - sizeof(pp))) = *static_cast<void **>(static_cast<void *>(static_cast<char *>(AV) + sizeof(int) + sizeof(int) + sizeof(pp)));
			// INSERT BLOCK INTO FREE LIST
			// LLINK(RLINK(p)) = p
			*static_cast<void **>(*static_cast<void **>(static_cast<void *>(static_cast<char *>(pp) - sizeof(pp)))) = static_cast<char *>(pp) - sizeof(pp) - sizeof(int) - sizeof(int) - sizeof(pp);
			// RLINK(AV)=p
			*static_cast<void **>(static_cast<void *>(static_cast<char *>(AV) + sizeof(pp) + sizeof(int) + sizeof(int))) = static_cast<char *>(pp) - sizeof(int) - sizeof(int) - sizeof(pp) - sizeof(pp);
		}
		else
		{
			cout << "UH OH!!!!";
			return;
		}
	}

	bool ALLOCATE(int n)
	{
		// TODO: something's defenitely wrong with the size...
		// pool = AV = q
		// p = p

		// SET P TO RLINK
		p = static_cast<char *>(AV) + sizeof(p) + sizeof(int) + sizeof(int);
		p = *static_cast<void **>(p);

		// IF WE LOOP AROUND THE WHOLE LIST, STOP
		do
		{
			//cout << "P" << p << endl;
			int size = *(static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(int) + sizeof(p))));
			//cout << "SIZE" << size << endl;
			if (size >= n)
			{
				int diff = size - (n + 3 * sizeof(p) + 3 * sizeof(int));
				// if (diff < 64)
				// {
				// 	cout << "Epsilon NOT COMPLETE!!!" << endl;
				// 	//cout << "p" << p << endl;
				// 	// ALLOCATE WHOLE BLOCK
				// 	// RLINK(LLINK(P)) = RLINK(p)
				// 	*static_cast<void **>(*static_cast<void **>(p)) = *static_cast<void **>(static_cast<void *>(static_cast<char *>(p) + sizeof(p) + sizeof(int) + sizeof(int)));

				// 	// LLINK(RLINK(p)) = LLINK(p)
				// 	//*static_cast<void **>(static_cast<void *>(*static_cast<void **>(static_cast<void *>(static_cast<char *>(p) + sizeof(int) + sizeof(int))))) = *static_cast<void **>(p);
				// 	*static_cast<void **>(*static_cast<void **>(static_cast<void *>(static_cast<char *>(p) + sizeof(p) + sizeof(int) + sizeof(int)))) = *static_cast<void **>(p);

				// 	//TAG(p)=TAG(p)+size(p)-1 = 1
				// 	*static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(p) + sizeof(int) + sizeof(int) + sizeof(p) + size)) = 1; //ending TAG
				// 	*static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(p))) = 1;												 // first TAG

				// 	// AV = LLINK(p)
				// 	AV = *static_cast<void **>(p);

				// 	return true;
				// }
				if (true)
				{
					// allocate lower N words
					// size(p) = diff
					*(static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(int) + sizeof(p)))) = diff;
					// uplink(p + diff -1) = p
					*static_cast<void **>(static_cast<void *>(static_cast<char *>(p) + sizeof(p) + sizeof(int) + sizeof(int) + sizeof(p) + diff + sizeof(int))) = p;
					// set upper portion as unused
					// TAG(p+diff-1) = 0
					*static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(p) + sizeof(int) + sizeof(int) + sizeof(p) + diff)) = 0;
					// AV = p
					AV = p;
					// p = p+diff+header+allocatedheader
					// move p
					p = static_cast<char *>(p) + diff + sizeof(p) + sizeof(int) + sizeof(int) + sizeof(p) + sizeof(int) + sizeof(p);
					// ----- MAKING ALLOCATED TAGS ------
					// size(p) = n
					*(static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(int) + sizeof(p)))) = n;
					// tag(p) = tag(p+n-1) = 1
					*static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(p))) = 1; // first tag
					*static_cast<int *>(static_cast<void *>(static_cast<char *>(p) + sizeof(p) + sizeof(int) + sizeof(int) + sizeof(p) + n)) = 1;
					// move p so we can use it
					p = static_cast<char *>(p) + sizeof(p) + sizeof(int) + sizeof(int) + sizeof(p);
					return true;
				}
			}
			// p = RLINK(p)
			p = *static_cast<void **>(static_cast<void *>(static_cast<char *>(p) + sizeof(p) + sizeof(int) + sizeof(int)));
			missCounter++;
			//flag = false;
		} while (p != *static_cast<void **>(static_cast<void *>(static_cast<char *>(AV) + sizeof(AV) + sizeof(int) + sizeof(int))));
		p = NULL;
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
		//p = new T[h - l + 1];
		block z(low,high,sizeof(T));
		p = (T *)z.p;
	}

	// single parameter constructor lets us
	// create a SA almost like a "standard" one by writing
	// SA x(10); and getting an array x indexed from 0 to 9

	SA(int i)
	{
		low = 0;
		high = i - 1;
		block z(low, high, sizeof(T));
		p = (T *)z.p;
		//p = new T[i];
	}
	// copy constructor for pass by value and
	// initialization

	SA(const SA &s)
	{
		int size = s.high - s.low + 1;
		//p = new T[size];
		block z(s.low+1,s.high,sizeof(T));
		p = (T *)z.p;
		for (int i = 0; i < size; i++)
			p[i] = s.p[i];
		low = s.low;
		high = s.high;
	}
	// destructor

	~SA()
	{
		//delete[] p;
		block::free(p);
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
		block::free(p);
		int size = s.high - s.low + 1;
		block z(s.low+1,s.high,sizeof(T));
		p = (T *)z.p;
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
	cout << "------Placing a values 5,9,355 into the SA------" << endl;
	arr[0] = 5;
	arr[1] = 9;
	arr[2] = 355;
	cout << "------Printing the value of SA[0], SA[1] SA[2]------" << endl;
	cout << arr[0] << endl;
	cout << arr[1] << endl;
	cout << arr[2] << endl;
	cout << "------Attempting to make 100 blocks of random size------" << endl;
	for (int i = 0; i < 100; i++)
	{
		int low = rand() % 100;
		int high = low + rand() % 80;
		block b(low, high, sizeof(int));
		// example from assignment page
	}
	cout << "------reprinting the value of SA[0], SA[1] SA[2] to show we haven't lost information------" << endl;
	cout << arr[0] << endl;
	cout << arr[1] << endl;
	cout << arr[2] << endl;

	cout << "--------Now that the program is done, SA will call free on the block-------" << endl;
}
