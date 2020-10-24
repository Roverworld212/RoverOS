#include "kheaders.h"
#include "libutil.h"

//Arith

int Clamp(int in, int min, int max){
	if(in >= max){return max;}
	if(in <= min){return min;}
	return in;
}

uint32 dCount(int n){
  uint32 c = 0;
  if(n == 0){
    return 1;
  }
  while(n > 0){
    ++c;
    n = n / 10;
  }
  return c;
}

void itoa(int n, char *num){
  int dc = dCount(n);
  int i = dc - 1;
  char x;
  if(n == 0 && dc == 1){
    num[0] = '0';
    num[1] = '\0';
  }
  else{
    while(n != 0){
      x = n % 10;
      num[i] = x + '0';
      i--;
      n = n / 10;
    }
    num[dc] = '\0';
  }
}

//IO

void outb(uint16 port, uint8 data){
  asm volatile("outb %1, %0" : : "dN" (port), "a" (data));
}

uint8 inb(uint16 port){
  uint8 r;
  asm volatile("inb %1, %0" : "=a" (r) : "dN" (port));
  return r;
}

//Mem

void memcpy(uint8 *d, uint8 *s, uint8 l){
  const uint8 *sp = (const uint8 *)s;
  uint8 *dp = (uint8 *)d;
  for(; l != 0; l--){
    *dp++ = *sp++;
  }
}

void memset(uint8 *d, uint8 v, uint32 l){
  uint8 *t = (uint8 *)d;
  for(; l != 0; l--){
    *t++ = v;
  }
}

void memshift(uint8 *s, uint32 a){//Shift values in array
  if(a == 0){return;}
  uint32 l = strlen(s);
  a = Clamp(a, 1, l);
  for(int i = 0; s[i] != '\0'; ++i){
    s[i] = s[i+1];
  }
}

//Str

int atoi(char *str){ //ASCII to int
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
	for(i = 0; str[i]; ++i){}
	return i;
}

void strcpy(char *d1, const char *s1){
	return memcpy(d1, s1, strlen(s1)+1);
}

