#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>


uint64_t elapsed_ms(int check)
{
	static uint64_t start_ms = 0;
	static uint64_t now_ms = 0;
	struct timeval	tv;
	if (check)
	{
		gettimeofday(&tv, NULL);

		now_ms = (uint64_t)tv.tv_sec * 1000U + (tv.tv_usec / 1000U);

		if (start_ms == 0)
		{
			// First call → initialize the starting point
			start_ms = now_ms;
		}
	}
	return now_ms - start_ms;
}


uint64_t elapsed_ms_alternative(void)
{
	static uint64_t start_ms = 0;
	struct timeval	tv;
	gettimeofday(&tv, NULL);

	uint64_t now_ms = (uint64_t)tv.tv_sec * 1000 + (tv.tv_usec / 1000);

	// Strictly branchless initialization of start_ms
	// If start_ms == 0 → mask = 0xFFFFFFFFFFFFFFFF, else mask = 0x0
	uint64_t mask = -(start_ms == 0);
	start_ms = (start_ms & ~mask) | (now_ms & mask);

	return now_ms - start_ms;
}
