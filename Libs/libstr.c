#include "libstd.h"
#include "libstr.h"
#include "libmutex.h"

uint32 dCount(int n){
  uint32 c = 0;
  if(n == 0){
    return 1;
  }
  while(n > 0){
    c++;
    n = n / 10;
  }
  return c;
}

void reverse(char *str, uint32 len){
  char b;
  uint32 end = len-1;
  for(uint32 start = 0; start < end; ++start){
    swap((int*)&str[start], (int*)&str[end]);
    --end;
  }
}

char *itoa(int num, char *str, int base){
  int i = 0;
  bool n = false;
  if(num == 0){str[0] = 0; str[1] = '\0'; return str;}
  if(num < 0 && base == 10){num = -num; n = true;}
  while(num != 0){
    int rem = num%base;
    str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
    num /= base;
  }
  str[i] = '\0';
  reverse(str, i);
  return str;
}


int atoi(const char *str){ //ASCII to int
	int ret = 0;
	for(int i = 0; str[i] != '\0'; ++i){
		ret = ret*10+str[i]-'0';
	}
	return ret;
}

int strcmp(const char *s1, const char *s2){ //Credit: Apple
    for(; *s1 == *s2; s1++, s2++){
		  if(*s1 == '\0'){
			  return 0;
		  }
	  }
	  return ((*(uint8*)s1 < *(uint8*)s2) ? -1 : +1);
}

uint32 strlen(const char *str){
	  uint32 i;
	  for(i = 0; str[i] != '\0'; ++i){}
	  return i;
}

void strcpy(char *d1, char *s1){
	return memcpy(d1, s1, strlen(s1)+1);
}