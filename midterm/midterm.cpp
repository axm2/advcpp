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
	int missCounter;

public:
	// ptr to start of usable block
	// dumb pointer so we can do pointer arithmetic
	void *p = NULL;


	block(int low, int high, int sizeOfT)
	{
		int space = (high - low + 1) * sizeOfT;
		if(ALLOCATE(space)){
			numBlocks++;
			szAllocated+=space;
			cout << "Blocks allocated: " << numBlocks << endl;
			cout << "Space allocated: " << szAllocated << " bytes" << endl;
		}
		
	}

	bool ALLOCATE(int n)
	{
		// pool = AV = q
		// p = p

		// SET P TO RLINK
		p = static_cast<char *>(AV) + sizeof(p) + sizeof(int) + sizeof(int);
		p = *static_cast<void **>(p);

		// IF WE LOOP AROUND THE WHOLE LIST, STOP
		bool flag = true;
		while (flag)
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
			flag = false;
		}
		cout << "No space can be allocated!" << endl;
		return false;
	}
};

int main(void)
{
	for (int i=0;i<100;i++){
		int low = rand() % 100;
		int high = low + rand() % 200;
		block b (low, high, sizeof(int));
	}

}
