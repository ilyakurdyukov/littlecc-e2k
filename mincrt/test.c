/*
//
// lcc -O2 -target=e2k/linux test.c -S -o test.s
// cc -nostartfiles -nodefaultlibs -nostdlib syscall.s crt0.s test.s -o test
// ./test 123 456 ; echo $?
//
*/

typedef unsigned long size_t;

long syscall(long, ...);

#define SYS_read 3
#define SYS_write 4
#define EOF -1

static int getchar() {
	char ch;
	return syscall(SYS_read, 1, &ch, 1) == 1 ? ch : EOF;
}

static int putchar(int ch) {
	return syscall(SYS_write, 1, &ch, 1) == 1 ? ch : EOF;
}

static size_t strlen(const char *s) {
	const char *p = s;
	while (*p) p++;
	return p - s;
}

static int puts(const char *s) {
	long n = strlen(s);
	return syscall(SYS_write, 1, s, n) == n ?
			putchar('\n') : EOF;
}

int main(int argc, char **argv) {
	int i;
	for (i = 0; i < argc; i++) puts(argv[i]);
	putchar('\n');

	puts("Hello, World!");
	return 42;
}
