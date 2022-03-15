#ifndef __Cstring_H_
#define __Cstring_H_

#include "MyApplication.h"

void cStringFree(void);
void cStringRestore(void);
char *StrBetwString(char *Str,char *StrBegin,char *StrEnd);
void split(char *src,const char *separator,char **dest,int *num);
/* extern variables-----------------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
