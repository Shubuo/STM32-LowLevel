/* Support files for GNU libc.  Files in the system namespace go here.
   Files in the C namespace (ie those that do not start with an
   underscore) go in .c.  */

#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>
#include <reent.h>
#include <unistd.h>
#include <sys/wait.h>

#undef errno
extern int errno;

//#define FreeRTOS
#define MAX_STACK_SIZE 0x200

extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

#ifndef FreeRTOS
  register char * stack_ptr asm("sp");
#endif



#ifdef __cplusplus
extern "C"
#endif
/*
 * _gettimeofday primitive (Stub function)
 * */
#ifdef __cplusplus
extern "C"
#endif
int _gettimeofday (struct timeval * tp, struct timezone * tzp)
{
  /* Return fixed data for the timezone.  */
  if (tzp)
    {
      tzp->tz_minuteswest = 0;
      tzp->tz_dsttime = 0;
    }

  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
void initialise_monitor_handles()
{
}


#ifdef __cplusplus
extern "C"
#endif
int _getpid(void)
{
	return 1;
}

#ifdef __cplusplus
extern "C"
#endif
int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

#ifdef __cplusplus
extern "C"
#endif
void _exit (int status)
{
	_kill(status, -1);
	while (1) {}
}

#ifdef __cplusplus
extern "C"
#endif
int _write(int file, char *ptr, int len)
{
	int DataIdx;

		for (DataIdx = 0; DataIdx < len; DataIdx++)
		{
		   __io_putchar( *ptr++ );
		}
	return len;
}

#ifdef __cplusplus
extern "C"
#endif
int _close(int file)
{
	return -1;
}

#ifdef __cplusplus
extern "C"
#endif
int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
int _isatty(int file)
{
	return 1;
}

#ifdef __cplusplus
extern "C"
#endif
int _lseek(int file, int ptr, int dir)
{
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
int _read(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
	  *ptr++ = __io_getchar();
	}

   return len;
}

#ifdef __cplusplus
extern "C"
#endif
int _open(char *path, int flags, ...)
{
	/* Pretend like we always fail */
	return -1;
}

#ifdef __cplusplus
extern "C"
#endif
int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}

#ifdef __cplusplus
extern "C"
#endif
int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

#ifdef __cplusplus
extern "C"
#endif
int _times(struct tms *buf)
{
	return -1;
}

#ifdef __cplusplus
extern "C"
#endif
int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
int _link(char *old, char *new_)
{
	errno = EMLINK;
	return -1;
}

#ifdef __cplusplus
extern "C"
#endif
int _fork(void)
{
	errno = EAGAIN;
	return -1;
}

#ifdef __cplusplus
extern "C"
#endif
int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}
