#include <stdint.h>
#include <userstdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define ULONG_MAX ((unsigned long)(~0L))      // 0xFFFFFFFF 
#define LONG_MAX ((long)(ULONG_MAX >> 1))     // 0x7FFFFFFF 
#define LONG_MIN ((long)(~LONG_MAX))          // 0x80000000 
#define IS_SPACE(x) ((x) == ' ' || (x) == '\t')
#define IS_ALPHA(x) (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z'))
#define IS_UPPER(x) (((x) >= 'A' && (x) <= 'Z'))
#define IS_DIGIT(x) (((x) >= '0' && (x) <= '9'))


extern int sys_write(uint64_t fd, char * buffer, uint64_t size, uint64_t screen_id);
extern int sys_read(uint64_t fd, char * buffer, uint64_t size);
extern void sys_time(char * buffer);
extern int sys_tick();
extern void sys_clear(uint8_t screen_id);
extern void sys_restartCursor(uint8_t screen_id);
extern void sys_divide();
extern void sys_uniqueWindow();


void divideWindow(){
  sys_divide();
}

void uniqueWindow(){
  sys_uniqueWindow();
}


int atoi(char *str){
    int answer = 0;
    int neg = 1;
    if(*str == '-'){
        neg = -1;
        str++;
    }
    while(*str){
        if(!IS_DIGIT(*str)){
            return 0;
        }
        answer = 10*answer + (*str - '0');
        str++;
    }
    return neg*answer;
}


int _strlen(const char * str){
    int i=0;
    while( *(str + (i++)) );
    return i-1;
}


int strcmp(char * s1, char * s2) {
    int i;
    for (i = 0; s1[i] && s1[i] == s2[i]; i++);
    return s1[i] - s2[i];
}


char * my_strcpy( char * destination, char * source){
	return my_strncpy(destination, source, _strlen(source) + 1);
}

char * my_strncpy( char * destination, char * source, int size){
	int i = 0;
	char * ret = destination;
	while(*source && i < size){
		*(destination++) = *(source++);
		i++;
	}
	*destination = '\0';
	return ret;
} 


int sprint(uint8_t fd, char * str){
  return sys_write(fd, str, _strlen(str),0);
}


int sprintId(uint8_t fd, char * str, uint8_t screen_id){
  return sys_write(fd, str, _strlen(str),screen_id);
}


int put_char(uint8_t fd, char c){
  return sys_write(fd, &c, 1,0);
}


int put_charId(uint8_t fd, char c, uint8_t screen_id){
  return sys_write(fd, &c, 1,screen_id);
}


void get_time(char * buffer){
  sys_time(buffer);
}


int tick(){
  return sys_tick();
}


int get_char(){
    char c;
    int returnval;
    do{
      returnval = sys_read(0, &c, 1);
    }while(returnval!=1);

	  return c;
}


int read_char(){
    char c;
    sys_read(STDIN, &c, 1);
    return c;
}


void clearScreen(){
  sys_clear(0);
}


void restartCursor(uint8_t screen_id){
  sys_restartCursor(screen_id);
}


//----------------------------------------------------------------
// https://stackoverflow.com/questions/1735236/how-to-write-my-own-printf-in-c
void my_printf(const char * frmt, ...)
{
  //Module 1: Initializing Myprintf's arguments using stdarg.h
  va_list arg;   //declares a variable which we use to manipulating the argument list contaning variable arugments
  va_start(arg, frmt);   //initialize arg with function's last fixed argument, i.e. format

  const char *aux;

  uint64_t i;
  unsigned u;
  char *s;

  for(aux=frmt; *aux != '\0'; aux++){
    while(*aux != '%'){
      if(*aux == '\0'){
        va_end(arg);
        return;
      }
      put_char(1, *aux);
      aux++;
    }
    aux++;

    //Module 2: Fetching and executing arguments
    //va_arg() fetches the next argument from the argument list, where the 2do parameter is the data type expected
    //ONLY accept char*, unsigned int, int or double
    switch(*aux){
      case 'c' :  i = va_arg(arg, int);  //Fetch char argument
                  put_char(1, i);
                  break;
      case 'd' :  i = va_arg(arg, int);   //Fetch Decimal/Integer argument
                  if(i < 0){
                    i = -i;
                    put_char(1, '-');
                  }
                  sprint(1, convert(i,10));
                  break;
      case 'o':   i = va_arg(arg, unsigned int);   //Fetch Octal representation
                  sprint(1, convert(i,8));
                  break;
      case 's':   s = va_arg(arg, char *);   //Fetch string
                  sprint(1, s);
                  break;
      case 'u':   u = va_arg(arg, unsigned int);   // Fetch Unsigned decimal integer
                  sprint(1, convert(u,10));
                  break;
      case 'x':   u = va_arg(arg, unsigned int);   //Fetch Hexadecimal representation
                  sprint(1, convert(u,16));
                  break;
      case '%':   put_char(1, '%');
                  break;
    }
  }
  //Module 3: Closing argument list to necessary clean-up
  va_end(arg);
}

char *convert(unsigned int num, int base)
{
  static char Representation[] = "0123456789ABCDEF";
  static char buff[33];
  char *ptr;

  ptr = &buff[sizeof(buff)-1];
  *ptr = '\0';

  do{
    *--ptr = Representation[num % base];
    num /= base;
  }while(num != 0);

  return(ptr);
} 

//--------------------------------------------------------------------------------------------------------------------
// SCANF

//https://www.equestionanswers.com/c/c-printf-scanf-working-principle.php
// Otra fuente pero la implementacion no esta completa
// https://www.daniweb.com/programming/software-development/threads/432590/scanf-function

int my_scanf(const char * str, ...) { 
  va_list vl;
  int i=0, j=0, ret=0;

  char buff[100] = {0};
  char tmp[20];

  char c;

  char *out_loc;
  while(c != '\n') {
//    if (fread(&c, 1, 1, stdin)) {     
    c = read_char();
    if (c != '\n') {
      buff[i] = c;
      i++;
    }
  }

  va_start(vl, str);
  i = 0;

  while (str && str[i]) {
    if (str[i] == '%') {
      i++;
      switch (str[i]) {
        case 'c': {
          *(char *)va_arg( vl, char* ) = buff[j];
          j++;
          ret++;
          break;
        }
        case 'd': {
          *(int *)va_arg( vl, int* ) = my_strtol(&buff[j], &out_loc, 10);
          j += out_loc -&buff[j];
          ret++;
          break;
        }
        case 'x': {
          *(int *)va_arg( vl, int* ) = my_strtol(&buff[j], &out_loc, 16);
          j += out_loc -&buff[j];
          ret++;
          break;
        }
        case 'o': {
          *(int *)va_arg( vl, int* ) = my_strtol(&buff[j], &out_loc, 8);
          j += out_loc -&buff[j];
          ret++;
          break;
        }
        case 's': {
          out_loc = (char *)va_arg( vl, char* );
          my_strcpy(out_loc, &buff[j]);
          j += _strlen(&buff[j]);
          ret++;
          break;
        }
      }
    } 
    else {
      buff[j] = str[i];
      j++;
    }
    i++;
  }
  va_end(vl);
  return ret;
} 


//----------------------------------------------------------------
// https://stackoverflow.com/questions/7457163/what-is-the-implementation-of-strtol
// https://code.woboq.org/gcc/libiberty/strtol.c.html

// STRTOL - convierte un string en un long int
// Skips all white-space characters at the beginning of the string, converts the subsequent characters
// as part of the number, and then stops when it encounters the first character that isn't a number.

long my_strtol(const char *restrict nptr, char **restrict endptr, int base) {
    const char *p = nptr, *endp;
    _Bool is_neg = 0, overflow = 0;
    // Need unsigned so (-LONG_MIN) can fit in these: 
    unsigned long n = 0UL, cutoff;
    int cutlim;
    if (base < 0 || base == 1 || base > 36) {
        return 0L;
    }
    endp = nptr;
    while (IS_SPACE(*p)){    // TO DO
        p++;
    }
    if (*p == '+') {
        p++;
    } 
    else if (*p == '-') {
        is_neg = 1;
        p++;
    }
    if (*p == '0') {
        p++;
        // For strtol(" 0xZ", &endptr, 16), endptr should point to 'x';
        // pointing to ' ' or '0' is non-compliant.
        // (Many implementations do this wrong.) 
        endp = p;
        if (base == 16 && (*p == 'X' || *p == 'x')) {
            p++;
        } 
        else if (base == 0) {
            if (*p == 'X' || *p == 'x') {
                base = 16;
                p++;
            } 
            else {
                base = 8;
            }
        }
    } 
    else if (base == 0) {
        base = 10;
    }

    cutoff = (is_neg) ? -(LONG_MIN / base) : LONG_MAX / base;
    cutlim = (is_neg) ? -(LONG_MIN % base) : LONG_MAX % base;
    while (1) {
        int c;
        if (*p >= 'A'){
            c = ((*p - 'A') & (~('a' ^ 'A'))) + 10;
        }
        else if (*p <= '9'){
            c = *p - '0';
        }
        else{
            break;
        }
        if (c < 0 || c >= base){
            break;
        }
        endp = ++p;
        if (overflow) {
            // endptr should go forward and point to the non-digit character
            // (of the given base); required by ANSI standard. 
            if (endptr) continue;
            break;
        }
        if (n > cutoff || (n == cutoff && c > cutlim)) {
            overflow = 1;
            continue;
        }
        n = n * base + c;
    }

    if (endptr){
      *endptr = (char *)endp;
    }
    if (overflow) {
        //errno = ERANGE;
        return ((is_neg) ? LONG_MIN : LONG_MAX);
    }
    return (long)((is_neg) ? -n : n);
}   

// ------------------------------------------------------------------------------------------------------

int64_t my_strtol(const char *nptr, char **endptr, register int base)
{
  register const char *s = nptr;
  register unsigned long acc;
  register int c;
  register unsigned long cutoff;
  register int neg = 0, any, cutlim;
       
  do {
    c = *s++;
  } while (IS_SPACE(c));

  if (c == '-') {
    neg = 1;
    c = *s++;
  } 
  else if (c == '+'){
    c = *s++;
  }
  if ((base == 0 || base == 16) && (c == '0' && (*s == 'x' || *s == 'X'))) {
    c = s[1];
    s += 2;
    base = 16;
  }
  if (base == 0){
    base = ((c == '0') ? 8 : 10);
  }
       
  cutoff = neg ? -(unsigned long) LONG_MIN : LONG_MAX;
  cutlim = cutoff % (unsigned long) base;
  cutoff /= (unsigned long) base;

  for (acc=0, any=0; ; c = *s++) {
    if (IS_DIGIT(c)){
      c -= '0';
    }
    else if (IS_ALPHA(c)){
      c -= IS_UPPER(c) ? 'A' - 10 : 'a' - 10;
    }
    else{
      break;
    }
    if (c >= base){
      break;
    }
    if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)){
      any = -1;
    }
    else {
      any = 1;
      acc *= base;
      acc += c;
    }
  }
  if (any < 0) {
    acc = neg ? LONG_MIN : LONG_MAX;
    //errno = ERANGE;
  } 
  else if (neg){
    acc = -acc;
  }
  if (endptr != 0){
    *endptr = (char *) (any ? s - 1 : nptr);
  }
  return (acc);
}
