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
		cout << dp << endl;
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
	// stats
	inline static int numBlocks = 0;
	inline static int szAllocated = 0;
	int missCounter;

public:
	// ptr to start of usable block
	// dumb pointer so we can do pointer arithmetic
	void *p = NULL;

	block(int low, int high, int sizeOfT)
	{
		int space = (high-low)*sizeOfT;
		ALLOCATE(space);
	}

	void ALLOCATE(int n)
	{
		// pool = AV = q
		// p = p
		

	}
};

int main(void)
{
	block b(0, 5, sizeof(int));
	//cout << *(int *)b.p << endl;
	//cout << *(static_cast<int *>(b.p) - 1) << endl;
}
