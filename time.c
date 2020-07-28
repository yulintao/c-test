       #define _XOPEN_SOURCE
       #include <stdio.h>
       #include <stdlib.h>
       #include <string.h>
       #include <time.h>

       int
       main(void)
       {
           struct tm tm;
           char buf[255];

           memset(&tm, 0, sizeof(struct tm));
           strptime("2001-11-12 18:31:01", "%Y-%m-%d %H:%M:%S", &tm);
           strftime(buf, sizeof(buf), "%d %b %Y %H:%M", &tm);
printf("h:%d m:%d s:%d\n",tm.tm_hour,tm.tm_min,tm.tm_sec);
           puts(buf);
           exit(EXIT_SUCCESS);
       }

