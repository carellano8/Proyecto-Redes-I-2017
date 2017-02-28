#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define LINF 32
#define LSUP 126
#define MAXC 2000


int main(int argc, char **argv)
{
	srand(time(NULL));
	for(int i=0; i<MAXC; i++)
    {
      cout<<(char)(LINF+rand()%(LSUP+1-LINF));
    }
	return 0;
}
