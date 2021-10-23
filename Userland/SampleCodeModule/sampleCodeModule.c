/* sampleCodeModule.c */

// #include <naiveConsole.h>
#include <userstdlib.h>
#include <shell.h>

static int var1 = 0;
static int var2 = 0;


int main() {

	sprint(1,"Welcome to Userland \n");

	//my_printf
	int i = 17;
 	char str[] = "not printed with printf :D";
  	my_printf("\nMessage = %s %d %x", str, i, i);
	my_printf("\nHola gentusa :)\n");	

	//my_scanf	
	char c;
	int k;
	int h;
	int ret = 0;
	char * str2;
	//ret = my_scanf("%c %d %x", &c, &k, &h);
	//my_printf("\n C = %c, I = %d, H = %X, Return %d", c, k, h, ret);
	ret = my_scanf("%d , %s", &k, str2);
	my_printf("\n Num = %d, String = %s, Return %d", k, str2, ret);
	

	while(1){
		put_char(2,'>');
		wait_command();
	}

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}
