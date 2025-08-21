#include "logger.h"
#include "timer.h"
#include "interface.h"
#include "stdio.h"

void logger(t_log_level log, char* message, char* file, int line)
{
	static const char* const format[LOG_LEVELS] = {[NOLOG] = ANSI_RESET,
												   [FATAL] = COL_FATAL,
												   [ERROR] = COL_ERROR,
												   [WARNING] = COL_WARNING,
												   [NOTICE] = COL_NOTICE,
												   [INFO] = COL_INFO,
												   [SUCCESS] = COL_SUCCESS,
												   [TRACE] = COL_TRACE,
												   [DEBUG1] = COL_DEBUG1,
												   [DEBUG2] = COL_DEBUG2,
												   [DEBUG3] = COL_DEBUG3,
												   [DEBUG4] = COL_DEBUG4};
	static const char* const level[LOG_LEVELS] = {[NOLOG] = "NOLOG",
												   [FATAL] = "FATAL",
												   [ERROR] = "ERROR",
												   [WARNING] = "WARNING",
												   [NOTICE] = "NOTICE",
												   [INFO] = "INFO",
												   [SUCCESS] = "SUCCESS",
												   [TRACE] = "TRACE",
												   [DEBUG1] = "DEBUG1",
												   [DEBUG2] = "DEBUG2",
												   [DEBUG3] = "DEBUG3",
												   [DEBUG4] = "DEBUG4"};
	if (log > LOG_LEVEL)
		return ;
	else if (LOG_LEVEL > TRACE)
		printf("%s%8lu %7s: %-32.32s %25.25s:%3i%s\n", // 8 + 1 + 7 + 2 + 40 + 1 + 20 + 1 + 3
			format[log],
			elapsed_ms(),
			level[log],
			message,
			file,
			line,
			ANSI_RESET); // reset color at end
	else
		printf("%s%8lu %7s: %-62.62s%s\n", // 8 + 1 + 7 + 2 + 40 + 1 + 20 + 1 + 3
			format[log],
			elapsed_ms(),
			level[log],
			message,
			ANSI_RESET);
	// printf("%-80s\n", "12345678901234567890123456789012345678901234567890123456789012345678901234567890");
}