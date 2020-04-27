#include <iostream>
#include <stdlib.h>
using namespace std;

class block
{
private:
	// total memory that block class controls
	void *pool = new char[20000];
	// stats
	static int numBlocks;
	static int szAllocated;
	int missCounter;

public:
	// ptr to start of usable block
	void *p = NULL;

	block(int low, int high, int sizeOfT)
	{
		// allocate memory here
		p=&pool;
		cout << &pool << endl;
	}
};

int main()
{
	block b(0, 5, sizeof(int));
	//int* ptr;
	int *ptr = (int *)b.p;
	cout << ptr << endl;
}
