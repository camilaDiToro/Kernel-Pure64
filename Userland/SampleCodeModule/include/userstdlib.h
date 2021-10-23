#ifndef _USERSTDLIB_H_
#define _USERSTDLIB_H_

#include <stdint.h>

int put_char(uint8_t fd, char c);
int put_charId(uint8_t fd, char c, uint8_t screen_id);
int sprint(uint8_t fd, char * str);
int sprintId(uint8_t fd, char * str, uint8_t screen_id);

int atoi(char *str);
int get_char();
int read_char();
void get_time(char * buffer);
int _strlen(const char * str);
int strcmp(char * s1, char * s2);
char * my_strcpy ( char * destination, char * source);
char * my_strncpy ( char * destination, char * source, int size); 
int tick();
void clearScreen();
void restartCursor(uint8_t screen_id);
void divideWindow();
void uniqueWindow();
void divideByZero();
void invalidOp();

void my_printf(const char * frmt, ...);
char *convert(unsigned int, int);


// https://gist.github.com/frankie-yanfeng/b88715e740163149cc5c2f3e8971f753
// https://stackoverflow.com/questions/32291808/how-to-implement-stdarg-in-c

//typedef char *va_list;
// #define va_start(ap, parmn) (void)((ap) = (char*)(&(parmn) + 1))
// #define va_end(ap) (void)((ap) = 0)
// #define va_arg(ap, type) (((type*)((ap) = ((ap) + sizeof(type))))[-1])


int my_scanf(const char* str,...);
long my_strtol(const char *restrict nptr, char **restrict endptr, int base); 
int64_t my_strtol(const char *nptr, char **endptr, register int base);


//----------------------------------------------------------------

#endif /* _USERSTDLIB_H_*/
