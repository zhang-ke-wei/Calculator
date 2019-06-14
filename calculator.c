#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<assert.h>
#include"stack.h"
#include"calculator.h"

//preprocess the str,and store the processed string in pstrbuf,and return 1 when success, 0 when failed.
//对STR进行预处理，并将处理后的字符串存储在pSTRUBF中，成功时返回1，失败时返回0。
int PreProcess(char *str,char *pstrbuf)
{
	char ch;
	int i = 0;
	int j = 0;
	assert(str);
	assert(pstrbuf);

	while((ch = *(str+i)) != '\0')
	{
		if((isdigit(ch))||(ch=='+')||(ch=='-')||(ch=='*')||(ch=='/')||(ch=='.')||(ch=='(')||(ch==')'))
		{
			*(pstrbuf+j) = ch;
			j++;
		}
		else
		{
			if(isspace(ch))
				;
			else
			{
				if(isalpha(ch))
				{
					printf("Expression incorrect,do not include alpha chars.\n");
					return 0;
				}
				else
				{
					if(isprint(ch))
					{
						printf("Expression incorrect.\n");
						return 0;
					}
				}
			}

		}
		i++;	
	}
	return j;
}
//analyze the str
int AnalyzeStr(char *str)
{
	int i = 0;
	char ch;
	unsigned char NextFlag = 0x0a; // first char is '(' or numbers
	unsigned char MaskFlag;
	
	assert(str);
	while((ch = *(str+i)) != '\0')
	{
		switch(ch)
		{
			case '(':
				MaskFlag = 0x08;
				if((MaskFlag & NextFlag) == 0)
				{
					printf("Syntax Error.\n");
					return 0;
				}
				NextFlag = 0x0a; //next is '(' or numbers
				break;
			case ')':
				MaskFlag = 0x10;
				if((MaskFlag & NextFlag) == 0)
				{
					printf("Syntax Error.\n");
					return 0;
				}
				NextFlag = 0x11;//next char is ')' or operator
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				MaskFlag = 0x01;
				if((MaskFlag & NextFlag) == 0)
				{
					printf("Syntax Error.\n");
					return 0;
				}
				NextFlag = 0x0a;//next char is '(' or numbers
				break;
			case '.':
				MaskFlag = 0x04;
				if((MaskFlag & NextFlag) == 0)
				{
					printf("Syntax Error.\n");
					return 0;
				}
				NextFlag = 0x03;//next char is numbers or operators
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				MaskFlag = 0x02;
				if((MaskFlag & NextFlag) == 0)
				{
					printf("Syntax Error.\n");
					return 0;
				}
				NextFlag = 0x17;//next char is ')',operators ,numbers or dot 
				break;
		}
		i++;
	}
	return 1;
}
void ProcessData(Stack *pDataStack,Stack *pOpStack,DataToken dt)
{
	char ch;
//   int id;
//	double fd;
	DataToken dt0,DInStack;
	if(IsStackEmpty(pOpStack))
	{
		Push(pDataStack,&dt);
		return;
	}
	ch = *((char *)(pOpStack->pData)+pOpStack->top);
//	printf("In ProcessData,the top of pOpStack is %c\n",ch);
	if(ch == '(')
	{
		Push(pDataStack,&dt);
		return;
	}
	if((ch == '*')||(ch == '/'))
	{
		Pop(pOpStack,NULL);//pop the operator
		Pop(pDataStack,&DInStack);//pop the data token
		if((dt.IsFloat == 1)||(DInStack.IsFloat == 1))
		{
			if(ch == '*')
			{
				dt0.data = DInStack.data * dt.data;
				dt0.IsFloat = 1;
				dt0.DataState = 2;
				Push(pDataStack,&dt0);
			}
			if(ch == '/')
			{
				dt0.data = DInStack.data / dt.data;
				dt0.IsFloat = 1;
				dt0.DataState = 2;
				Push(pDataStack,&dt0);
			}
		}
		else
		{
			if(ch == '*')
			{
				dt0.data = DInStack.data * dt.data;
				dt0.IsFloat = 0;
				dt0.DataState = 2;
				Push(pDataStack,&dt0);
			}
			if(ch == '/')
			{
				dt0.data = (int)DInStack.data / (int)dt.data;
				dt0.IsFloat = 0;
				dt0.DataState = 2;
				Push(pDataStack,&dt0);
			}
		}
	}
	if((ch == '+')||(ch == '-'))
	{
		Push(pDataStack,&dt);
//		printf("In ProcessData ,Push(pDataStack,&dt)\n");
	}
}
char PriorityCmp(char op1,char op2)
{
	if(((op1 == '*')||(op1 == '/'))&&((op2 == '*')||(op2 == '/')))
		return '=';
	if(((op1 == '*')||(op1 == '/'))&&((op2 == '+')||(op2 == '-')))
		return '>';
	if(((op1 == '+')||(op1 == '-'))&&((op2 == '*')||(op2 == '/')))
		return '<';
	if(((op1 == '+')||(op1 == '-'))&&((op2 == '+')||(op2 == '-')))
		return '=';
	return '\0';
}
void ProcessOperator(Stack *pDataStack,Stack *pOpStack,char ch)
{
	char OpTop;
	char Pri;
	DataToken dt,dt1,dt2;
	if(IsStackEmpty(pOpStack))
	{
		Push(pOpStack,&ch);
		return;
	}
	OpTop = *((char *)(pOpStack->pData)+pOpStack->top);
	if(OpTop == '(')
	{
		Push(pOpStack,&ch);
		return;
	}
    Pri = PriorityCmp(ch,OpTop);
	if((Pri  == '=')||((Pri == '<')&&(pOpStack->top == 0)))
	{
		Pop(pDataStack,&dt1);
		Pop(pDataStack,&dt2);
		Pop(pOpStack,NULL);
		switch(OpTop)
		{
			case '+':
				if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
				{
					dt.data = dt1.data + dt2.data;
					dt.IsFloat = 1;
					dt.DataState = 2;
				}
				else
				{
					dt.data = dt1.data + dt2.data;
					dt.IsFloat = 0;
					dt.DataState = 2;
				}
				break;
			case '-':
				if((dt1.IsFloat ==1)||(dt2.IsFloat == 1))
				{
					dt.data = dt2.data - dt1.data;
					dt.IsFloat = 1;
					dt.DataState = 2;
				}
				else
				{
					dt.data = dt2.data - dt1.data;
					dt.IsFloat = 0;
					dt.DataState = 2;
				}
				break;
			case '*':
				dt.data = dt1.data * dt2.data;
				dt.DataState = 2;
				if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
					dt.IsFloat = 1;
				else
					dt.IsFloat = 0;
				break;
			case '/':
				if(dt1.data == 0)
				{
					printf("Divided by zero.\n");
					exit(1);
				}
				if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
				{
					dt.data = dt2.data / dt1.data;
					dt.IsFloat = 1;
					dt.DataState = 2;
				}
				else
				{
					dt.data = (int)dt2.data / (int)dt1.data;
					dt.IsFloat = 0;
					dt.DataState = 2;
				}
				break;
				
		}
		Push(pDataStack,&dt);
		Push(pOpStack,&ch);
	}
	else
		Push(pOpStack,&ch);

}

void ProcessRightBracket(Stack *pDataStack,Stack *pOpStack)
{
	char OpTop;
	DataToken dt,dt1,dt2;
	if(IsStackEmpty(pOpStack))
		return;
	OpTop = *((char*)(pOpStack->pData)+pOpStack->top);
	if(OpTop == '(')
	{
		Pop(pOpStack,NULL);
//		printf("In ProcessRightBrackets,Pop(pOpStack,NULL).\n");
		return;
	}
	switch(OpTop)
	{
		case '+':
			Pop(pDataStack,&dt1);
			Pop(pDataStack,&dt2);
			if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
			{
				dt.data = dt1.data + dt2.data;
				dt.IsFloat = 1;
				dt.DataState = 2;
			}
			else
			{
				dt.data = dt1.data + dt2.data;
				dt.IsFloat = 0;
				dt.DataState = 2;
			}
			Push(pDataStack,&dt);
			break;
		case '-':
			Pop(pDataStack,&dt1);
			Pop(pDataStack,&dt2);
			if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
			{
				dt.data = dt2.data - dt1.data;
				dt.IsFloat = 1;
				dt.DataState = 2;
			}
			else
			{
				dt.data = dt2.data - dt1.data;
				dt.IsFloat = 0;
				dt.DataState = 2;
			}
			Push(pDataStack,&dt);
			break;
		case '*':
			Pop(pDataStack,&dt1);
			Pop(pDataStack,&dt2);
			if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
			{
				dt.data = dt1.data * dt2.data;
				dt.IsFloat = 1;
				dt.DataState = 2;
			}
			else
			{
				dt.data = dt1.data * dt2.data;
				dt.IsFloat = 0;
				dt.DataState = 2;
			}
			Push(pDataStack,&dt);
			break;
		case '/':
			Pop(pDataStack,&dt1);
			Pop(pDataStack,&dt2);
			if(dt1.data == 0)
			{
				printf("Divided by zero.\n");
				exit(1);
			}
			if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
			{
				dt.data = dt2.data / dt1.data;
				dt.IsFloat = 1;
				dt.DataState = 2;
			}
			else
			{
				dt.data = (int)dt2.data / (int)dt1.data;
				dt.IsFloat = 0;
				dt.DataState = 2;
			}
			Push(pDataStack,&dt);
			break;
	}
	Pop(pOpStack,NULL);
	Pop(pOpStack,NULL);
}
void FinalProcess(Stack *pDataStack,Stack *pOpStack)
{
	DataToken dt,dt1,dt2;
	char ch;

	if(IsStackEmpty(pOpStack))
		return;
	if(pDataStack->top <= 0)
		return;

	while(pOpStack->top >= 0)
	{
		Pop(pOpStack,&ch);

		Pop(pDataStack,&dt1);
		Pop(pDataStack,&dt2);
		switch(ch)
		{
			case '+':
				if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
				{
					dt.data = dt1.data + dt2.data;
					dt.IsFloat = 1;
					dt.DataState = 2;
				}
				else
				{
					dt.data = dt1.data + dt2.data;
					dt.IsFloat = 0;
					dt.DataState = 2;
				}
				Push(pDataStack,&dt);
//				printf("In FinalProcess,dt.data = %lf\n",dt.data);
				break;
			case '-':
				if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
				{
					dt.data = dt2.data - dt1.data;
					dt.IsFloat = 1;
					dt.DataState = 2;
				}
				else
				{
					dt.data = dt2.data - dt1.data;
					dt.IsFloat = 0;
					dt.DataState = 2;
				}
				Push(pDataStack,&dt);
				break;
			case '*':
				if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
				{
					dt.data = dt1.data * dt2.data;
					dt.IsFloat = 1;
					dt.DataState = 2;
				}
				else
				{
					dt.data = dt1.data *dt2.data;
					dt.IsFloat = 0;
					dt.DataState = 2;
				}
				Push(pDataStack,&dt);
				break;
			case '/':
				if(dt1.data == 0)
				{
					printf("Divided by zero.\n");
					exit(1);
				}
				if((dt1.IsFloat == 1)||(dt2.IsFloat == 1))
				{
					dt.data = dt2.data / dt1.data;
					dt.IsFloat = 1;
					dt.DataState = 2;
				}
				else
				{
					dt.data = (int)dt2.data / (int)dt1.data;
					dt.IsFloat = 0;
					dt.DataState = 2;
				}
				Push(pDataStack,&dt);
				break;
		}
	}
}
//calculate the str
void CalculateStr(char *str)
{
	Stack DataStack;
	Stack OpStack;
	char ch;
	int i = 0;
	int j = 0;
	char data[100]={0};
	DataToken dt;

	assert(str);
	InitStack(&DataStack,sizeof(DataToken),10);
	InitStack(&OpStack,sizeof(char),10);
	
	dt.data = 0;
	dt.IsFloat = 0;
	dt.DataState = 0;
	while((ch = *(str+i)) != '\0')
	{
		switch(ch)
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(dt.DataState == 0)
					dt.DataState = 1;
				data[j] = ch;
				j++;
				break;
			case '.':
				if(dt.DataState == 1)
				{
					data[j] = ch;
					j++;
					dt.IsFloat = 1;
				}
				break;
			case '(':
				Push(&OpStack,&ch);
				break;
			case ')':
				if(dt.DataState == 1)
				{
					if(dt.IsFloat)
					{
						dt.data = atof(data);
						dt.DataState = 2;
						ProcessData(&DataStack,&OpStack,dt);
					}
					else
					{
						dt.data = atoi(data);
						dt.DataState = 2;
						ProcessData(&DataStack,&OpStack,dt);
					}
					j = 0;
					memset(data,0,100);
					memset(&dt,0,sizeof(dt));
				}
				ProcessRightBracket(&DataStack,&OpStack);
				break;
			case '+':
			case '-':
				if(dt.DataState == 1)
				{
					if(dt.IsFloat)
					{
						dt.data = atof(data);
						dt.DataState = 2;
						ProcessData(&DataStack,&OpStack,dt);			
					}
					else
					{
						dt.data = atoi(data);
						dt.DataState = 2;
						ProcessData(&DataStack,&OpStack,dt);
					}
					j = 0;
					memset(data,0,100);
					memset(&dt,0,sizeof(dt));
				}
				//process the operator;
				ProcessOperator(&DataStack,&OpStack,ch);
				break;
			case '*':
			case '/':
				if(dt.DataState == 1)
				{
					if(dt.IsFloat)
					{
						dt.data = atof(data);
						dt.DataState = 2;
						ProcessData(&DataStack,&OpStack,dt);
					}
					else
					{
						dt.data = atoi(data);
						dt.DataState = 2;
						ProcessData(&DataStack,&OpStack,dt);
					}
					j = 0;
					memset(data,0,100);
					memset(&dt,0,sizeof(dt));
				}
				ProcessOperator(&DataStack,&OpStack,ch);
				break;
		}
//              printf("DataStack:%lf\n",*(double*)DataStack.pData);
//		printf("OpStack %c\n",*(char*)OpStack.pData);
		i++;
	}
	//handle the last data
	if(dt.DataState == 1)
	{
		if(dt.IsFloat)
		{
			dt.data = atof(data);
			dt.DataState = 2;
			ProcessData(&DataStack,&OpStack,dt);
		}
		else
		{
			dt.data = atoi(data);
			dt.DataState = 2;
			ProcessData(&DataStack,&OpStack,dt);
		}
	}
	FinalProcess(&DataStack,&OpStack);
        //get the result
//	printf("There is %d datas in DataStack\n",DataStack.top);
	Pop(&DataStack,&dt);
	if(dt.IsFloat)
		printf("= %lf\n",dt.data);
	else
		printf("= %d\n",(int)dt.data);

	DestroyStack(&DataStack);
	DestroyStack(&OpStack);
	return;
}


//int main(int argc,char *argv[])
//{
//	char strbuf[100];
//	char ch;
//	int i ;
//	char *pstr = NULL;
//	while(1)
//	{
//		memset(strbuf,0,100);
//		i = 0;
//		printf("Please enter an expression('q' to quit):");
//        while((ch = getchar()) != '\n')
//		{
//			if(ch == 'q')
//				exit(0);
//			strbuf[i++]=ch;
//		}
//		//now preprocess the string strbuf[];
//		pstr = (char *)malloc(100);
//		assert(pstr);
//		memset(pstr,0,100);
//		if(PreProcess(strbuf,pstr) == 0)
//		{
//			free(pstr);
//			continue;
//		}
//		printf("%s ",pstr);
//		//now we get a correct string,no do some test to see if it's in correct order
//		if(AnalyzeStr(pstr))
//		{
////			printf("Syntax analyze OK.\n");
//			CalculateStr(pstr);
//			free(pstr);
//		}
//		else
//		{
//			free(pstr);
//			printf("Syntax analyze failed.\n");
//		}
//	}
//
//	return 0;
//}
