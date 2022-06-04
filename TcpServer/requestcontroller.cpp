#include "requestcontroller.h"

/*������list<request>��ز����������붨��*/
std::string checkList()
{
	//...
}
//...

std::string RequestController::startRequest(int mode, int capacity)//��������
{
	/*3.�ύ�������*/
	int number;//�µ��ŶӺ�
	/*1.1-1.2*/
	if (isFinish() == "no\t")//isFinish()����user.h
	{
		return "no/�û�����δ��ɵ�����\t";
	}
	/*1.3-1.4*/
	if (checkList() == "no\t")//checkList()����list<request>����
	{
		return "no/�޿��г�λ��\t";
	}
	/*1.5*/
	number = newQueueNum(mode);
	/*1.6-1.8*/
	if (add(Request(number, ownerID, mode, capacity, batteryCapacity)) == "no\t")//add(r)����list<request>������ownerID��bCapacity��ȫ�ֱ�����ȡ
	{
		return "no/���Request��l1ʧ��\t";
	}
	/*1.9-1.10*/
	if (writeQueueNum(number) == "no\t")//writeQueueNum(number)����user.h
	{
		return "no/��¼�ŶӺ�ʧ��\t";
	}
	/*1.11-1.12*/
	if (changeState(waitingState) == "no\t")//changeState(state)����user.h
	{
		return "no/����״̬ʧ��\t";
	}
	return "yes\t";
}

std::string RequestController::changeRequest(int mode, int value)//��������
{
	/*4.�޸ĳ������*/
	int oldNumber;//�ɳ���
	/*1.1-1.2*/
	if (isWaiting() == "no\t")//isWaiting()����user.h
	{
		return "no/�û������ڵȴ���\t";
	}
	/*1.3-1.4*/
	oldNumber = getNumber();//getNumber()����user.h
	/*mode=0*/
	if (mode == 0)//�޸ĳ����
	{
		/*1.1-1.2*/
		if (changeCapacity(oldNumber, value)=="no\t")//changeCapacity(oldNumber,value)����list<request>����
		{
			return "no/�޸ĳ����ʧ��\t";
		}
	}
	/*mode=1*/
	else if (mode == 1)
	{
		/*1.1*/
		int newNumber = newQueueNum(cMode);//cMode�Ǹ��ĺ�ĳ��ģʽ����ȫ�ֱ�����ȡ
		/*1.2-1.3*/
		if (deleteNum(oldNumber) == "no\t")//deleteNum(number)����list<request>����
		{
			return "no/������ɾ��ʧ��\t";
		}
		/*1.4-1.5*/
		if (addNum(newNumber) == "no\t")//addNum(newNumber)����list<request>����
		{
			return "no/���������ʧ��\t";
		}
		/*1.6-1.7*/
		if (writeQueueNum(newNumber) == "no\t")//writeQueueNum(number)����user.h
		{
			return "no/��¼�ŶӺ�ʧ��\t";
		}
	}
	return "yes\t";
}

std::string RequestController::endRequest()//��������
{
	/*5.�������*/
	Request r;//rΪrequest���󣬺�������õ�
	/*1.1-1.2*/
	if (isFinish() == "no\t")//isFinish()����user.h
	{
		return "no/�û�����δ��ɵ�����\t";
	}
	/*1.3-1.4*/
	int state = isWaiting();//isWaiting()����user.h
	/*1.5-1.6*/
	int number = getNumber();//getNumber()����user.h
	/*state=1���ڵȴ���*/
	if (state == 1)
	{
		/*1.1-1.2*/
		if (deleteNum(number) == "no\t")//deleteNum(number)����list<request>����
		{
			return "no/������ɾ��ʧ��\t";
		}
	}
	/*state=0�����ڵȴ���*/
	else if (state == 0)
	{
		/*1.1-1.2*/
		int pileNo = getPileNo();//getPileNo()����user.h
		/*1.3-1.4*/
		r = removeFromPileList(pileNo, ownerID);//removeFromPileList(pileNo, ownerID)����pile.h��ownerID����ȫ�ֱ���������һ��Request���󸳸�r
	}
	/*1.7-1.8*/
	if (add(r) == "no\t")//add(r)����list<request>����
	{
		return "no/���Request��l1ʧ��\t";
	}
	/*1.9-1.10*/
	if (changeState(finishState) == "no\t")
	{
		return "no/����״̬ʧ��\t";
	}
	return "yes\t";
}

int RequestController::newQueueNum(int mode);//�����µ��ŶӺ�
{
	return getNewNum(mode);//getNewNum(mode)�������ŶӺţ�����list<request>
}