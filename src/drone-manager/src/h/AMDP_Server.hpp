#ifndef __AMDP_SERVER_H__
#define __AMDP_SERVER_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Port Numbers
#define DRONE_API_PORT 60001 
#define DRONE_MANAGER_PORT 60002 
#define DRONE_AI_PORT 60003 
/*********************************************************************************************************/

//Status codes
//1xx
#define AMDP_SERVER_OVERLOAD 100

//2xx
#define AMDP_SERVER_OK 200
#define AMDP_SERVER_OK_MODIFY 201

//3xx
#define AMDP_SERVER_MOVED 300

//4xx
#define AMDP_SERVER_SYNTAX_ERROR 400
#define AMDP_SERVER_AUNTHENTICATION_FAILED 401

//5xx
#define AMDP_SERVER_COLLAPSED 500
#define AMDP_SERVER_CANNOT_RESPOND 501

/*********************************************************************************************************/



#endif