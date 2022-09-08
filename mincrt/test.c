/*
//
// lcc -O2 -target=e2k/linux test.c -S -o test.s
// cc -nostartfiles -nodefaultlibs -nostdlib syscall.s crt0.s test.s -o test
// ./test ; echo $?
//
*/

long syscall(long, ...);

#define SYS_write 4

int main() {
	syscall(SYS_write, 1, "Hello, World!\n", 14);
	return 42;
}
