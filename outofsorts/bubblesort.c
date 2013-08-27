/*
 * We all know the old adage:
 *
 *   "One cannot make smooth and shiny
 *    the surface of a lump of excrement
 *    simply by rubbing it."
 *
 * ...or, well, at least most of us understand the gist of such sentiment.
 *
 * While I believe that the statement regarding feces has been proven
 * false (http://www.youtube.com/watch?v=yiJ9fy1qSFI), I do understand
 * the attitude and cede at least a bit of credence to the notion.
 *
 * Slight me if you will, but even Knuth has yet to relegate the
 * tradition of the Bubble Sort algorithm:
 *
 *   "...the bubble sort seems to have nothing to recommend it,
 *    except a catchy name and the fact that it leads to some
 *    interesting theoretical problems."
 *
 *      Donald Knuth. The Art of Computer Programming,
 *      Volume 3: Sorting and Searching, Second Edition.
 *      Addison-Wesley, 1998. ISBN 0-201-89685-0.
 *      Pages 106-110 of section 5.2.2: Sorting by Exchanging.
 *
 * Thus, gentle people: The (Optimized) Bubble Sort.
 *
 * NOTES:
 *
 * Sanity Check - How do we access the members of the array that is passed
 * to bubblesort()? Here's an illustration:
 *
 * void
 * examplefunc(void *base, size_t nmemb, size_t size)
 * {
 *     char *tmp = (char *)base;
 *     for (size_t i = 0; i < nmemb; i++) {
 *         printf("item: %p -> %s\n", (char *)tmp + (size * i),
 *         (char *)tmp + (size * i));
 *     }
 * }
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "bubblesort.h"

/*
 * Clever swapping taken from "Engineering a Sort Function" by
 * Jon L. Bentley and M. Douglas McIlroy.
 *
 * See the FreeBSD qsort(3) function for another example of this
 * technique.
 */
#define simpleswap(TYPE, parmi, parmj, n) {				\
	long i = (n) / sizeof (TYPE);					\
	TYPE *pi = (TYPE *) (parmi);					\
	TYPE *pj = (TYPE *) (parmj);					\
	do {								\
		TYPE t = *pi;						\
		*pi++ = *pj;						\
		*pj++ = t;						\
	} while (--i > 0);						\
}

/*
 *   "The strange formula to check data size and alignment works even
 *    on Cray computers, where plausible code such as:
 *
 *      ((long)a | es) % sizeof(long)
 *
 *    fails because the least significant part of a byte address occupies
 *    the most significant part of a long"
 *
 *    -- From "Engineering a Sort Function" by Bentley and McIlroy
 */
#define SWAPINIT(a, es) {						\
	swaptype = ((char *)a - (char *)0) % sizeof(long) ||		\
	    es % sizeof(long) ? 2 : es == sizeof(long) ? 0 : 1;		\
}									\

/*
 * If the elements are single long-sized objects, we can optimize,
 * swapping in-place the contents of the memory addresses.
 * Else, we'll need to perform a less optimal, simplistic memory
 * content copying algorithm.
 */
#define swap(a, b)							\
	if (swaptype == 0) {						\
		long t = *(long *)(a);					\
		*(long *)(a) = *(long *)(b);				\
		*(long *)(b) = t;					\
	} else if (swaptype <= 1) {					\
		simpleswap(long, a, b, size);				\
	} else {							\
		simpleswap(char, a, b, size);				\
	}								\

void
bubblesort(void *base, size_t nmemb, size_t size,
    int (*cmpfunc)(const void *, const void *))
{
	int swaptype;
	char *tmp1 = (char *)base;
	char *tmp2 = (char *)base + size;

	SWAPINIT(base, size);

	size_t i = 0;
	size_t n = nmemb;
	size_t nnew;
	do {
		nnew = 0;
		for (i = 1; i <= n - 1; i++) {
			tmp1 = (char *)base + (size * (i - 1));
			tmp2 = (char *)base + (size * ((i - 1) + 1));
			if (cmpfunc(tmp1, tmp2) > 0) {
				swap(tmp1, tmp2);
				nnew = i;
			}
		}
		n = nnew;
	} while (n > 0);
}
