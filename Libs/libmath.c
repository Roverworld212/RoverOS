#include "libstd.h"
#include "libmath.h"

int Clamp(int in, int min, int max){
	if(in >= max){return max;}
	if(in <= min){return min;}
	return in;
}