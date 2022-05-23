#if defined(__LCC__) && !defined(__EDG__)
typedef unsigned long size_t;
int atoi(const char*);
int printf(const char*, ...);
int calloc(size_t, size_t);
void free(void*);

typedef long time_t;
typedef long suseconds_t;

struct timeval {
	time_t tv_sec;
	suseconds_t tv_usec;
};

int gettimeofday(struct timeval*, void*);
#define NULL ((void*)0)
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#endif

static int gettime() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return time.tv_sec * 1000 + time.tv_usec / 1000;
}

/* The Computer Language Benchmarks Game
 * https://salsa.debian.org/benchmarksgame-team/benchmarksgame/
 *
 * converted to C by Joseph Piché
 * from Java version by Oleg Mazurov and Isaac Gouy
 *
 */

int fannkuchredux(int n, int *check) {
  int *perm, *perm1, *count;
	int flipsMax = 0, permCount = 0, checksum = 0;
	int i, k, r = n, flips;

  perm = (int*)calloc(n*3, sizeof(*perm));
  perm1 = perm + n;
  count = perm1 + n;

  for (i = 0; i < n; i++) perm1[i] = i; // initial (trivial) permu
  for (;;) {
		for (; r != 1; r--) count[r-1] = r;
		for (i = 0; i < n; i++) perm[i] = perm1[i];
		flips = 0;

		while ((k = perm[0]) != 0) {
			int k2 = (k + 1) >> 1;
			for (i = 0; i < k2; i++) {
				int temp = perm[i]; perm[i] = perm[k-i]; perm[k-i] = temp;
			}
			flips++;
		}

		if (flipsMax < flips) flipsMax = flips;
		checksum += permCount & 1 ? -flips : flips;

		/* Use incremental change to generate another permutation */
		for (;;) {
			int perm0;
			if (r == n) {
				*check = checksum;
				free(perm);
				return flipsMax;
			}

			perm0 = perm1[0];
			i = 0;
			while (i < r) {
				k = i + 1;
				perm1[i] = perm1[k];
				i = k;
			}

			perm1[r] = perm0;
			if ((count[r] -= 1) > 0) break;
			r++;
		}
		permCount++;
	}
}

int main(int argc, char **argv) {
	unsigned time; int i;

	for (i = 1; i < argc; i++) {
		int n = atoi(argv[i]);
		int result, check = 0;

		time = gettime();
		result = fannkuchredux(n, &check);
		time = gettime() - time;

		printf("%d\nPfannkuchen(%d) = %d\n", check, n, result);
		printf("time = %dms\n", time);
	}
	return 0;
}

