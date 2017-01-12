#ifndef ASSEMBLER_H  
#define ASSEMBLER_H 
#include <fstream>
#include "asmconst.h"
#include "asmstruct.h"
using namespace std;
class assembler     
{
private:
	int ForcastTable[VN_NUM][VT_NUM];     //LL(1)预测分析表
	int SyntacticRules[RULE_NUM][RULE_DEPTH]; //MiniSys语法规则表
	char *KeyWords[KEYWORDS_SIZE];              //关键字表,前面是指令名称
	char FileName[COMMOM_SIZE];   //输入文件名
   	ifstream fin;                 //输入文件
    ofstream dfout,cfout,sfout,efout;   //输出文件，程序和数据文件,以及统计指令个数文件
    string ramFile, romFile, countFile, errFile;
	int ErrorID;            //错误编号，语义处理要用到
	int NumOfLine;          //行号
	int NumOfError;         //错误数
	int NumOfWarning;       //警告数
	int CurrentDataAddr;    //当前数据地址
	int CurrentComAddr;     //当前指令地址
	int LastRule;           //前一条规则
	int CurrentRule;        //当前规则
	int HasError;           //前一个单词有没有错  
	int NeedExplain;        //是否需要注释
	int NeedSel;			//mtc0和mfc0是否需要翻译sel
 
	My_Stack STACK;         //下推栈
	My_Token TOKEN;         //识别的单词
	My_Data*  DATAS_TABLE;  //数据表
	My_Com*   COMS_TABLE;   //指令表	
	My_Var*   VARS_TABLE;   //变量表
	My_SegID* SEGIDS_TABLE; //标号表
	My_Back*  BACKS_TABLE;  //回填表
	My_Op   OpTable[OP_TABLE_SIZE];      //助记符表
	My_Func FuncTable[FUNC_TABLE_SIZE];  //功能号表
	My_Reg  RegTable[REG_TABLE_SIZE];    //寄存器编号表
	My_Sel  SelTable[SEL_TABLE_SIZE];    //Sel编号表
	My_Data*  DATA_POS;     //表当前位置 
	My_Com*   COM_POS;	
	My_Var*   VAR_POS;
	My_SegID* SEGID_POS;
	My_Back*  BACK_POS;
	char *CODE;//="01010101010101010101";
public:
    assembler();
    assembler(string asmFile,int needExplain);
	virtual ~assembler();
	void InitVar();                       //初始化类变量
//核心部分
    int Parser(string resultFile);                         //语法分析器
	int Scanner(int TypeOfToken);         //扫描器
	int Translater();                     //翻译器
	int Error(int TypeOfError);           //错误处理
	int Warning(int TypeOfWarning);       //警告处理
//语法部分
	int Deduction();                      //语法推导器
	void Succeed();                       //成功
	void Fail();                          //失败
//扫描器部分
protected:
	int ScanCom();
	int ScanIdname();
	int ScanAddress();
	int Scan16Radix(int maxlen);
	int Scan10Radix(int maxlen);
	int Scan2Radix(int maxlen);
	int ScanReg();
	int ScanEndl();
	bool IsKeyWords(char ID[COMMOM_SIZE]);//判断标志符是不是关键字
	bool IsSeparate(char ch);             //判断是不是分隔符
	char ReadAChar();                     //读一个字符
	void ReadAString(char Str[COMMOM_SIZE]);     //读一个字符串
	void WriteAChar(char ch);             //写一个字符
	void WriteAString();                  //写一个字符串
//翻译器部分
protected:   
	//翻译指令
	int	TranslateRcom();
	int	TranslateSrcom();
	int	TranslateSllRcom();
	int	TranslateIcom();
	int	TranslateSicom();
	int	TranslateLwicom();
	int	TranslateJcom();
	int TranslateBcom();
	int	TranslatePcom();
	int TranslateJbcom();
	int TranslateNop();
	//新增6条指令以及Sel
	int	TranslateMRCOM();
	int TranslateMFCRCOM();
	int TranslateMULRCOM();
	int TranslateSYSRCOM();
	int TranslateJRCOM();
	int TranslateBGICOM();
	int	TranslateSel();

	//翻译立即数
	int TranslateImmi();
	//翻译地址
	int	TranslateAddr();
	//翻译变量
	int TranslateID();
	//填表,查表
	int InsertVar();                         //插入变量名
	int InsertData();                        //插入数据
	int InsertSegID();	                     //插入标号
	int InsertCom(char segName[6],int type); //插入指令
	int InsertBack();                        //插入回填表
	int SearchVar(char Name[COMMOM_SIZE]);   //查变量表
	int SearchSegID(char Name[COMMOM_SIZE]); //查标号表
	int SearchFirstSegID(char Name[COMMOM_SIZE]);
	int SearchOp(char Name[COMMOM_SIZE],char Op[OP_SIZE]);       //助记符
	int SearchFunc(char Name[COMMOM_SIZE],char Func[FUNC_SIZE]); //查功能号
	int SearchReg(char Name[COMMOM_SIZE],char Reg[REG_SIZE]);    //寄存器号
	int SearchNum(char Name[COMMOM_SIZE]);                       //指令编号
	int SearchBack();                                     //回填指令
	int SearchCP0Reg(char Name[COMMOM_SIZE],char Reg[REG_SIZE],char Sel[SEL_SIZE]);    //CP0寄存器号
	int SearchSel(char Name[COMMOM_SIZE],char Sel[SEL_SIZE]);    //CP0寄存器号
//错误处理部分(包括词法，语法，语义错误)
protected:
	int ErrorLexical();
	int ErrorSemantic();
	int ReadErrorToken();
	int ReadErrorSentence();
//其他
protected:
	//输出函数
    int  WriteProgram();                  //程序输出    
	int  WriteData();                     //数据输出 
	int  WriteCount();                    //指令使用情况统计
	void Write32Bit(int addr,char Line[COMMOM_SIZE],ofstream &out);
	void WriteExplain(My_Com* pCom);      //机器码解释内容 
};
#endif

