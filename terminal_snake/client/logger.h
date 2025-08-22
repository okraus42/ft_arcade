

#ifndef LOGGER_H
# define LOGGER_H
# pragma once
#include "interface.h"

#define LOG_LEVEL DEBUG1

/* ANSI reset */
#define ANSI_RESET "\033[0m"

/* Log level colors — 256-color mode, direct, no nested macros */

/* Important levels — bold */
#define COL_FATAL   "\033[1;38;5;15;48;5;196m"   /* white on bright red   */
#define COL_ERROR   "\033[1;38;5;15;48;5;160m"   /* white on dark red     */
#define COL_WARNING "\033[1;38;5;0;48;5;220m"    /* black on yellow       */

/* Less critical — not bold */
#define COL_NOTICE  "\033[0;38;5;0;48;5;81m"     /* black on sky blue     */
#define COL_INFO    "\033[0;38;5;15;48;5;33m"    /* white on blue         */
#define COL_SUCCESS "\033[0;38;5;0;48;5;46m"     /* black on green        */
#define COL_TRACE   "\033[0;38;5;0;48;5;45m"     /* black on cyan         */

/* Debug levels — greyscale backgrounds */
#define COL_DEBUG1  "\033[0;38;5;15;48;5;236m"   /* white on very dark grey */
#define COL_DEBUG2  "\033[0;38;5;15;48;5;240m"   /* white on dark grey      */
#define COL_DEBUG3  "\033[0;38;5;15;48;5;244m"   /* white on mid grey       */
#define COL_DEBUG4  "\033[0;38;5;0;48;5;248m"    /* black on light grey     */


void logger(t_log_level log, char* message, char* file, int line);

#endif /*LOGGER_H*/
