#include "user.h"
#include "request.h"
#include "pile.h"
#include "global.h"

class RequestController {
public:
    std::string startRequest(int mode,int capacity);       //创建请求
    std::string changeRequest(int mode,int value);     //更改请求
    std::string endRequest();      //结束请求
    int newQueueNum(int mode);     //分配新的排队号
};