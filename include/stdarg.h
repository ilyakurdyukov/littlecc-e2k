#ifndef __STDARG
#define __STDARG

#if !defined(_VA_LIST) && !defined(__VA_LIST_DEFINED)
#define _VA_LIST
#define _VA_LIST_DEFINED
typedef struct { char *p; float f; } __va_list;
#endif
typedef __va_list va_list;

#define va_start(list, start) ((void)((list).p = (sizeof(start)<8 ? \
	(char*)((long*)&(start)+1) : (char*)(&(start)+1))))
#define va_arg(list, mode) (\
	__typecode(mode)==1 && sizeof(mode)==4 ? \
	  (list).f = ((double*)((list).p += sizeof(double)))[-1], \
		*(mode*)&(list).f : \
	  *(mode*)&((list).p += ((sizeof(mode)+7u)&~7u))[-(int)((sizeof(mode)+7u)&~7u)])
#define va_end(list) ((void)0)
#endif
