//**********************stack.h*************************
//*       written by Mr.Han in 2018.1.11               *
//******************************************************

typedef struct tagStack
{
	void *pData;
	int DataSize;//the size of the members.
	int DataNum;//the number of the members.
	int top;
}Stack,*LPStack;

int InitStack(Stack *pStack,int DataSize,int DataNum);
int DestroyStack(Stack *pStack);
int Push(Stack *pStack,void *pData);
int Pop(Stack *pStack,void *pData);
int GetSizeOfStack(Stack *pStack);//return the size of the stack in bytes.���ֽڵ���ʽ���ض�ջ�Ĵ�С��
int IsStackFull(Stack *pStack);//return 1 when the stack is full,otherwise 0.����ջ��ʱ����1������Ϊ0
int IsStackEmpty(Stack *pStack);//return 1 when the stack is empty,otherwise 0.����ջΪ��ʱ����1������Ϊ0��
