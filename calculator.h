//calculator.h


typedef struct tagData
{
	double data;
    int IsFloat;// 0:integer;1:double
	int DataState;//0:data end;1:data bgein
}DataToken;
int PreProcess(char *str,char *pstrbuf);
int AnalyzeStr(char *str);
void ProcessData(Stack *pDataStack,Stack *pOpStack,DataToken dt);
char PriorityCmp(char op1,char op2);//优先级比较
void ProcessOperator(Stack *pDataStack,Stack *pOpStack,char ch);//处理运算符
void ProcessRightBracket(Stack *pDataStack,Stack *pOpStack);//处理右括号
void FinalProcess(Stack *pDataStack,Stack *pOpStack);
void CalculateStr(char *str);