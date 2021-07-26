#include "libstd.h"
#include "libmath.h"

int Clamp(uint64 in, uint64 min, uint64 max){
	if(in >= max){return max;}
	if(in <= min){return min;}
	return in;
}