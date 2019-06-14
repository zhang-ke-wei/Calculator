//********************stack.c*************************
//* written by Mr.Han in 2018.1.11                   *
//****************************************************

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"stack.h"

static int ExtendStack(Stack *pStack);//return the new size of the stack in bytes.���ֽ�Ϊ��λ���ض�ջ���´�С��

int InitStack(Stack *pStack,int DataSize,int DataNum)//��ʼ��ջ
{
	if((pStack == NULL)||(DataSize <= 0)||(DataNum <= 0))
		return -1;

	pStack->DataSize = DataSize;
	pStack->DataNum = DataNum;
	pStack->top = -1;
	pStack->pData = malloc(pStack->DataSize*pStack->DataNum);
	assert(pStack->pData);
	
	return 1;
}
int DestroyStack(Stack *pStack)//�ͷŵ��ڴ�
{
	if(pStack == NULL)
		return -1;
	free(pStack->pData);
	return 0;
}
int IsStackFull(Stack *pStack)//ջ����
{
	assert(pStack);
	if(pStack->top == pStack->DataNum -1)
		return 1;
	return 0;
}
int IsStackEmpty(Stack *pStack)//վ�գ�
{
	assert(pStack);
	if(pStack->top == -1)
		return 1;
	return 0;
}
static int ExtendStack(Stack *pStack)//����ջ���ڴ�ռ�
{
	if(pStack == NULL)
		return -1;
	pStack->DataNum *= 2;
	pStack->pData = realloc(pStack->pData,pStack->DataSize*pStack->DataNum);
	assert(pStack->pData);
	return pStack->DataSize*pStack->DataNum;
}

int Push(Stack *pStack,void *pData)//��ջ
{
	if((pStack == NULL)||(pData == NULL))
		return -1;
	if(IsStackFull(pStack))
		ExtendStack(pStack);
	pStack->top++;
	memcpy((unsigned char *)pStack->pData+pStack->DataSize*pStack->top,pData,pStack->DataSize);
	return pStack->top;
}
int Pop(Stack *pStack,void *pData)//��ջ
{
	if(pStack == NULL)
		return -1;
	if(IsStackEmpty(pStack))
		return -1;
	if(pData)
		memcpy(pData,(unsigned char*)pStack->pData+pStack->DataSize*pStack->top,pStack->DataSize);
	pStack->top--;
	return pStack->top;
}
int GetSizeOfStack(Stack *pStack)//��ȡ��ջ��С
{
	return pStack->DataSize*pStack->DataNum;
}

//just for test//����

int main(int argc ,char *argv[])
{
	Stack s;
	int i;
	char arr[10]={"ABCDEFGHIJ"};
	int arrr[]={ 1,2,3,4,5,6,7,8,9,10 };
	InitStack(&s,sizeof(int),1);
	
	for(i = 0;i<10;i++)
	{
		Push(&s,&arr[i]);
	}
	for (i = 0; i<10; i++)
	{
		Push(&s, &arrr[i]);
	}

	for(i = 1;i<=10;i++)
	{
		int data;
		Pop(&s,&data);
		printf("Pop %d time = %d\n",i,data);
	}
	for (i = 1; i <= 10; i++)
	{
		char data;
		Pop(&s, &data);
		printf("Pop %d time = %c\n", i, data);
	}

	printf("��ջ�Ĵ�С��%d�ֽڡ�.\n",GetSizeOfStack(&s));
	DestroyStack(&s);
	getchar();
	return 0;
}

