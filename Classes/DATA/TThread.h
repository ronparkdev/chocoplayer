
#ifndef __R2K_T_THREAD__
#define __R2K_T_THREAD__

#ifdef _WIN32

#define _WINSOCKAPI_
#include <windows.h>
#include <process.h>
#define _THREAD_H HANDLE
#define _THREAD_RETVALUE unsigned int __stdcall
#define _THREAD_ID unsigned int 
#define _THREAD_END _endthreadex(0);
#define _THREAD_RUN(ROUTIN, PARAM, THREAD_ID, THREAD_HND) THREAD_HND = (HANDLE)_beginthreadex(NULL,0, ROUTIN, PARAM, 0, &THREAD_ID);
#define _THREAD_KILL(THREAD_HND) 
#else
#include <pthread.h>
#define _THREAD_H pthread_t
#define _THREAD_RETVALUE void *
#define _THREAD_ID int 
#define _THREAD_END pthread_exit(NULL);
#define _THREAD_RUN(ROUTIN, PARAM, THREAD_ID, THREAD_HND) THREAD_ID = pthread_create(&THREAD_HND, NULL, ROUTIN, (void*)PARAM);
#define _THREAD_KILL(THREAD_HND) pthread_kill(THREAD_HND, 0);
#endif

#endif 