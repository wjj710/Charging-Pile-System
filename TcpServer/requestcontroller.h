#include "user.h"
#include "request.h"

class RequestController {
public:
    void startRequest(int mode,int capacity);       //创建请求
    void changeRequest(int mode,int value);     //更改请求
    void endRequest();      //结束请求
    void newQueueNum(int mode);     //分配新的排队号
};
