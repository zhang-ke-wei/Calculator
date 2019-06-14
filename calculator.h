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
char PriorityCmp(char op1,char op2);//���ȼ��Ƚ�
void ProcessOperator(Stack *pDataStack,Stack *pOpStack,char ch);//���������
void ProcessRightBracket(Stack *pDataStack,Stack *pOpStack);//����������
void FinalProcess(Stack *pDataStack,Stack *pOpStack);
void CalculateStr(char *str);