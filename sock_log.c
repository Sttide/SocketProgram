#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SOCK_LOG_MSG_BUFSIZE  512
#define SOCK_LOG_TIME_BUFSIZE 128
#define SOCK_LOG_ONELOG_BUFSIZE 1024
#define SOCK_FILE_MAX_SIZE    100*1024*1024

#define SOCK_LOG_TEST "SOCKLOG"
#define LOG_PRIORITY_ERROR "ERROR"
#define LOG_PRIORITY_INFO "INFO"


int g_LogFd = -1;
long long int g_FileLen = 0;
#define SOCK_LOG_FILE_NAME  "sock.log"

#define ERROR_SUCCESS     0
#define ERROR_FAILED      1

void sock_trace_Addlog_Second(char *faturename, char *infoPri, char * pcformat, ...)
{
    va_list ap;
    time_t currentTime;
    struct  tm *localTime;
    char szbuf[SOCK_LOG_MSG_BUFSIZE];
    char szTimeBuf[SOCK_LOG_TIME_BUFSIZE];
    char logBuf[SOCK_LOG_ONELOG_BUFSIZE];
    char *pcbuf = szbuf;
    unsigned int uiLen = 0;


    va_start(ap, pcformat);
    vsnprintf(pcbuf + uiLen, SOCK_LOG_MSG_BUFSIZE - uiLen, pcformat, ap);
    va_end(ap);

    currentTime = time(NULL);
    localTime = localtime(&currentTime);

    //strftime(pcbuf, 80, "%Y-%m-%d %H:%M:%S", localTime);
    snprintf(szTimeBuf, sizeof(szTimeBuf), "%04d-%02d-%02d %02d:%02d:%02d  %s %s: ",
                (localTime->tm_year + 1900), (localTime->tm_mon +1), localTime->tm_mday,
                localTime->tm_hour, localTime->tm_min, localTime->tm_sec,
                faturename, infoPri);

    uiLen = snprintf(logBuf, sizeof(logBuf), "%s%s\r\n", szTimeBuf, szbuf);
    if (uiLen + g_FileLen > SOCK_FILE_MAX_SIZE)
    {
        lseek(g_LogFd, 0, SEEK_SET);
        g_FileLen = 0;
    }
    uiLen = write(g_LogFd, logBuf, uiLen);
    g_FileLen += uiLen;
}

void sock_trace_Addlog_uSecond(char *faturename, char *infoPri, char * pcformat, ...)
{
    va_list ap;
    struct timeval stcurrentTime;
    struct  tm *localTime;
    char szbuf[SOCK_LOG_MSG_BUFSIZE];
    char szTimeBuf[SOCK_LOG_TIME_BUFSIZE];
    char logBuf[SOCK_LOG_ONELOG_BUFSIZE];
    char *pcbuf = szbuf;
    unsigned int uiLen = 0;


    va_start(ap, pcformat);
    vsnprintf(pcbuf + uiLen, SOCK_LOG_MSG_BUFSIZE - uiLen, pcformat, ap);
    va_end(ap);

    (void)gettimeofday(&stcurrentTime, NULL);
    localTime = localtime(&(stcurrentTime.tv_sec));

    //strftime(pcbuf, 80, "%Y-%m-%d %H:%M:%S", localTime);
    snprintf(szTimeBuf, sizeof(szTimeBuf), "%04d-%02d-%02d %02d:%02d:%02d:%06ld %s %s: ",
                (localTime->tm_year + 1900), (localTime->tm_mon +1), localTime->tm_mday,
                localTime->tm_hour, localTime->tm_min, localTime->tm_sec,
                stcurrentTime.tv_usec ,faturename, infoPri);

    uiLen = snprintf(logBuf, sizeof(logBuf), "%s%s\r\n", szTimeBuf, szbuf);
    if (uiLen + g_FileLen > SOCK_FILE_MAX_SIZE)
    {
        lseek(g_LogFd, 0, SEEK_SET);
        g_FileLen = 0;
    }
    uiLen = write(g_LogFd, logBuf, uiLen);
    g_FileLen += uiLen;
}

#define sock_trace_log_s(faturename, infoPri, pcformat, ...) \
{   \
    sock_trace_Addlog_Second(faturename, infoPri, pcformat, ##__VA_ARGS__); \
}

#define sock_trace_log_us(faturename, infoPri, pcformat, ...) \
{   \
    sock_trace_Addlog_uSecond(faturename, infoPri, pcformat, ##__VA_ARGS__); \
}

int sock_log_init()
{
    int ifd = -1;
    ifd = open(SOCK_LOG_FILE_NAME, O_APPEND | O_RDONLY | O_WRONLY | O_CREAT);
    if (ifd < 0)
    {
        return ERROR_FAILED;
    }

    g_LogFd = ifd;
    return ERROR_SUCCESS;
}

void sock_log_fin()
{
    close(g_LogFd);
}

int main()
{
    int num = 5;
    int iResult = 0;
    iResult = sock_log_init();
    if (iResult != ERROR_SUCCESS)
    {
        return -1;
    }
    
    sock_trace_log_s(SOCK_LOG_TEST, LOG_PRIORITY_ERROR, "This is a %d log.", num);
    sock_trace_log_us(SOCK_LOG_TEST, LOG_PRIORITY_INFO, "This is a %d log.", num+1);

    sock_log_fin();
    return 0;
}