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
	inline static shared_ptr<uint64_t[]> pool = make_unique<uint64_t[]>((4000000));
	// stats
	static int numBlocks;
	static int szAllocated;
	int missCounter;

public:
	// ptr to start of usable block
	// dumb pointer so we can do pointer arithmetic
	uint64_t *p = NULL;

	block(int low, int high, int sizeOfT)
	{
		// allocate memory here
		p = pool.get();
		pool[0] = 'c';
		pool[1] = 'e';
		pool[2] = INT64_MAX;
		cout << (char)*(p+1) << endl;
		cout << (uint64_t)*(p+2) << endl;

	}

	void FF(int n)
	{
		while (p != 0)
		{
			cout << "Hey";
		}
		return;
	}
};

int block::numBlocks=0;

int main(void)
{
	block b(0, 5, sizeof(int));
	int *ptr = (int *)b.p;
	cout << ptr << endl;
}
