#include "user.h"
#include "request.h"

class RequestController {
public:
    char* startRequest(int mode,int capacity);       //创建请求
    char* changeRequest(int mode,int value);     //更改请求
    char* endRequest();      //结束请求
    char* newQueueNum(int mode);     //分配新的排队号
};
