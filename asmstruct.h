#ifndef ASMSTRUCT_H
#define ASMSTRUCT_H
struct My_Stack  //采用数组类型定义栈
{
  int Depth[STACK_SIZE];
  int Top;
};
struct My_Token  //单词内码：存储最近读取的一个单词的信息
{
   int  ID;                  //单词类型
   char Content[COMMOM_SIZE];       //单词内容
   int  Idx;                 //下标 
}; 
struct My_Data   //数据表
{
	int Addr;                //数据地址
	char Value[DATA_SIZE];   //数值
	My_Data *Next;
};
struct My_Var    //变量表示符号表：存储变量的信息
{
	int Addr;                //变量地址
    char Name[COMMOM_SIZE];  //变量名称   
    My_Var *Next;
};

struct My_SegID  //指令标号：存储标号信息
{
   int Addr;                 //指令地址(标号值)
   char Name[COMMOM_SIZE];   //标号名称
   My_SegID *Next;
};
struct My_Com	 //指令表：存储指令信息
{
	int  Addr;               //指令地址
	char JSegName[6];        //指令中包含的指令标号名称
	char Content[COMMOM_SIZE];      //内容部分	
	int  TypeOfCom;          //指令类型
	int  NumOfCom;           //指令编号
	char Com[COM_SIZE];      //二进制码指令
	My_Com *Next;            //下一条指令地址
};

struct My_Back   //回填表
{
	My_Com  *Com;            //需要回填指令的地址
	char Name[COMMOM_SIZE];         //标号
	int lineNum;             //所在行的位置
	My_Back *Next;
};
struct My_Op     //助记符表
{
	char Op[OP_SIZE];
	char Name[8];
};
struct My_Func   //功能号表
{
	char Func[FUNC_SIZE];
	char Name[8];
};
struct My_Reg    //寄存器表
{
	char Reg[REG_SIZE];
	char Name[10];
};
struct My_Sel    //Sel表
{
	char Sel[SEL_SIZE];
	char Name[10];
};



#endif