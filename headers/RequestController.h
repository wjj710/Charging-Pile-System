//Edited by Wang Yiwei on 2022/6/1

#include "User.h"
#include "Request.h"
#include "Pile.h"

class RequestController {
public:
    void startRequest(int mode,int capacity);       //��������
    void changeRequest(int mode,int value);     //��������
    void endRequest();      //��������
    void newQueueNum(int mode);     //�����µ��ŶӺ�
};