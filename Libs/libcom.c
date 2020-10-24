#include "kheaders.h"
#include "libcom.h"
#include "rcom.h"

uint32 warns, errors = 0;

void writeChar(char ch){
  sendData(COM1, (uint8)ch);
}

void qwrite(char *str){
  for(int i = 0; str[i]; ++i){
    writeChar(str[i]);
  }
}

void qbool(bool vl){
  if(vl == true){qwrite("true");}
  if(vl == false){qwrite("false");}
}

void qint(char n){
  char str[dCount(n)+1];
  itoa(n, str);
  qwrite(str);
}

void qhex(int n){
  int tmp;
  char noZeroes = 1;
  int i;
  for (i = 28; i > 0; i -= 4){
    tmp = (n >> i) & 0xF;
    if (tmp == 0 && noZeroes != 0){
      continue;
    }
    if(tmp >= 0xA){
      noZeroes = 0;
      writeChar(tmp-0xA+'a');
    }
    else{
      noZeroes = 0;
      writeChar(tmp+'0');
    }
  }
  tmp = n & 0xF;
  if (tmp >= 0xA){
    writeChar(tmp-0xA+'a');
  }
  else{
    writeChar(tmp+'0');
  }
}

void qdebug_int(char *str, char *func, int elvl){
  if(!kvinf.kio.debugOut){return;}
  if(!kvinf.kio.infoOut && (elvl == INFO)){return;}
  if(!kvinf.kio.warnOut && (elvl == WARNING)){return;}
  if(!kvinf.kio.errorOut && (elvl == ERROR)){return;}
  qwrite(func);
  switch(elvl){
    case INFO:
    qwrite("[i]: ");
    break;
    case WARNING:
    qwrite("[w]: ");
    ++warns;
    break;
    case ERROR:
    qwrite("[e]: ");
    ++errors;
    break;
    default:
    qwrite("[gds]: ");
    break;
  }
  qwrite(str);
}

/* Syntax Guide
// %f %u %d - int types
// %h - Char type
// %x %X - print int type as hex
// %b - Print bool as (true/false)
// Place these at the start!
// %I - Info
// %W - Warning
// %E - Error
// %g - Write chars to screen (cannot be closed) (also must be at the start)
*/

void printq(char *str){
  if(!kvinf.kio.debugOut){return;}
  char *c = str;
  uint32 *a= &str+1;
  uint32 b = 0;
  bool screen = false;
  uint32 type = LINT; //defaults to int
  char *ob;
  for(uint32 i = 0; c[i] != '\0'; ++i){
    if(c[i] != '%'){
      sendData(COM1, c[i]);
    }
    else{
      ++i;
      switch(c[i]){
        case 'I':
        qwrite(a[b]);
        qwrite("[i]: ");
        break;
        case 'W':
        qwrite(a[b]);
        qwrite("[w]: ");
        break;
        case 'E':
        qwrite(a[b]);
        qwrite("[e]: ");
        break;
        case '%':
        sendData(COM1, (uint8)'%');
        ++i;
        break;
        case 'l':
        type = LU32;
        break;
        case 'h':
        type = SHR8;
        break;
        case 'L':
        break;
        case 'c':
        if(type == LINT){sendData(COM1, (int)a[b]);}
        else if(type == LU32){sendData(COM1, (uint32)a[b]);}
        else{sendData(COM1, (char)a[b]);}
        break;
        case 'd':
        case 'i':
        case 'f':
        case 'u':
        qwrite(a[b]);
        break;
        case 'p':
        qwrite("0x");
        qhex((int)a[b]);
        break;
        case 'x':
        printq("0x");
        case 'X':
        qhex((int)a[b]);
        break;
        case 's':
        qwrite(a[b]);
        break;
        case 'b':
        if((uint32)a[b] >= 1){
          printq("true");
        }
        else{
          printq("false");
        }
        break;
      }
      ++b;
    }
  }
}