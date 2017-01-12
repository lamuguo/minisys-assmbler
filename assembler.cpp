#include "assembler.h"
#include "asmfunction.h"
#include "asminit.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>
#include <regex>
#include <stdlib.h>

char* strlwr(char* s) {
    unsigned char *ucs = (unsigned char *) s;
    for ( ; *ucs != '\0'; ucs++)
    {
        *ucs = tolower(*ucs);
    }
    return s;
}

assembler::assembler()
{
    InitForcastTable(ForcastTable);    //初始化预测分析表
    InitSyntacticRules(SyntacticRules);//初始化语法规则表
    InitKeyWords(KeyWords);            //初始化关键字
    InitOpTable(OpTable);              //初始化助记符表
    InitFuncTable(FuncTable);          //初始化功能表
    InitRegTable(RegTable);            //初始化寄存器表
    InitSelTable(SelTable);			   //初始化Sel表
    InitVar();                         //初始化变量
    strcpy(FileName,"test.asm");       //默认
}
assembler::assembler(string asmFile, int needExplain)
{
    InitForcastTable(ForcastTable);    //初始化预测分析表
    InitSyntacticRules(SyntacticRules);//初始化语法规则表
    InitKeyWords(KeyWords);            //初始化关键字
    InitOpTable(OpTable);              //初始化助记符表
    InitFuncTable(FuncTable);          //初始化功能表
    InitRegTable(RegTable);            //初始化寄存器表
    InitSelTable(SelTable);			   //初始化Sel表
    InitVar();                         //初始化变量
    strcpy(FileName,asmFile.c_str());   //自定义
    NeedExplain=needExplain;           //是否需要注释

    //创建生成ram和rom的文件目录
    int index = asmFile.find_last_of('/');
    string temp = asmFile;
    temp.erase(index+1);
    this->ramFile = temp + "dmem32.coe";
    this->romFile = temp + "prgmip32.coe";
    this->countFile = temp + "CountNum.txt";
    this->errFile = temp + "asmError.log";
    //cout<<"test1: "<<this->romFile<<"\t"<<this->ramFile<<endl;
}
assembler::~assembler()
{
}

//ostream otest;
//语法分析程序
int assembler::Parser(string resultFile)
{
//    otest.open(resultFile.c_str(),ios::out);
    fin.open(FileName);
    this->efout.open(this->errFile.c_str());

    cout<<"assmebling...GO INTO LOOP"<<endl;
    int  State=0;

    while(1)
    {
        char ch=ReadAChar();
        if(ch==' '||ch=='\t') continue;
        else if(ch==';') { State=1;continue; }
        else if(ch=='\n') {State=2;continue; }
        else
        {
            if(State==1) continue;
            else { WriteAChar(ch); break;}
        }
    }

    //如果第一个字符就错，程序终止
    Scanner(N_PRO);
    if(TOKEN.ID==ERROR_NPRO||TOKEN.ID==T_FILEEND)
    {
        TOKEN.ID=ERROR_NPRO;
        Error(ERROR_LEXICAL);
        Fail();
        return -1;
    }

    //LL(1)分析
    while(1)
    {
        for (int i=0;i<=STACK.Top;i++)
        {
            if (i!=STACK.Top)
                cout<<STACK.Depth[i]<<" ";
            else
                cout<<"["<<STACK.Depth[i]<<"] ";
        }
        cout<<TOKEN.ID<<" "<<TOKEN.Content<<endl;


        if(STACK.Depth[STACK.Top]>VT_NUM)//推导
        {
            cout<<"DEDUCTING"<<endl;
            if(Deduction()==-1) {Error(ERROR_LEXICAL);Fail();return 0;}
            continue;
        }
        else if(TOKEN.ID==T_FILEEND)        //分析结束
        {
            if(STACK.Top==0)
            {
                if(NumOfError==0&&SearchBack()==0) {Succeed();return 0;}
                else {Fail();return -1;}
            }
            else { Error(ERROR_LEXICAL);Fail();return -1;}
        }
        else if(STACK.Depth[STACK.Top]==TOKEN.ID) //匹配
        {
            cout<<"MATCHED"<<endl;
            STACK.Depth[STACK.Top--]=0;   //出栈
            if(HasError==0)               //前一个单词正确才翻译
            {
                if(Translater()==-1)      //语义出错
                {
                    if(Error(ERROR_SEMANTIC)==-1) //无法恢复
                    {
                        Fail();
                        return -1;
                    }
                }
            }

            HasError=Scanner(STACK.Depth[STACK.Top]);
            if(HasError==-1)    //词法错误
            {
                cout<<"Lexer error, EXPECTED [" << STACK.Depth[STACK.Top] << "]" << endl;
                Fail();
                return -1;
            }
        }
        else
        {
            Fail();
            return -1;
        }
    }

    return 0;
}
//语法推导函数
int assembler::Deduction()
{
    LastRule=CurrentRule;  //保留前一个规则，可能有用
    cout<<"LastRule <"<<LastRule<<"> "<<STACK.Depth[STACK.Top]<<"  "<<TOKEN.ID;
    CurrentRule=ForcastTable[STACK.Depth[STACK.Top]-50][TOKEN.ID];
    cout<<" CurrentRule <"<<CurrentRule << "> with [N" << STACK.Depth[STACK.Top]
        << ", T" << TOKEN.ID << "]" << endl;
    if(CurrentRule==0) return -1;
    for(int i=0;i<=RULE_DEPTH;i++)
    {
        int Token=SyntacticRules[CurrentRule][i];
        if(Token==0){      //遇到0表示推导完成
            STACK.Top--;   //设定栈顶，在else语句中加了一次
            break;
        }
        else{
            STACK.Depth[STACK.Top++]=Token;  //不考虑栈越界，因为不可能出现
        }
    }
    return 0;
}

void assembler::Succeed()
{
    WriteCount();     //指令统计
    WriteProgram();   //写入程序
    WriteData();      //写入数据
    cout<<"assembling was successful - ";
    cout<<NumOfError<<" error(s), "<<NumOfWarning<<" warning(s)\n";
    this->efout.close();
}
void assembler::Fail()
{
//	DeleteFile("Ram.mif");
//	DeleteFile("Rom.mif");
////	DeleteFile("prgmip32.mif");
////	DeleteFile("dmem32.mif");
//	DeleteFile("CountCom.txt");
    cout<<"assembling was unsuccessful - ";
    cout<<NumOfError<<" error(s), "<<NumOfWarning<<" warning(s)\n";
    this->efout.close();
}
//语法分析程序块结束

//扫描器核心程序，即词法分析
int assembler::Scanner(int TypeOfToken)
{
    cout << "xfguo: Scanner() 0" << endl;
    TOKEN.ID=0;
    TOKEN.Idx=-1;
    for(int i=0;i<COMMOM_SIZE;i++)	TOKEN.Content[i]='\0';
    //滤除空格和制表符
    if(TypeOfToken!=T_ENDL)
    {
        char ch;
        do{
            ch=ReadAChar();
            if(ch=='\0') { TOKEN.ID=T_FILEEND; return 0; } //文件末尾
        }while(ch==' '||ch=='\t');
        WriteAChar(ch);
    }
    //下面根据TypeOfToken进行扫描
    char Temp[COMMOM_SIZE]={'\0'};	  //临时数组
    if(TypeOfToken>VT_NUM) //非终结符，依据预测分析表扫描所需单词
    {
        switch (TypeOfToken)
        {
        case N_PRO:
        case N_DATA:     //扫描T_DATA|T_CODE
            {
                ReadAString(Temp);
                if(strcmp(Temp,".text")==0) { TOKEN.ID=T_CODE; return 0; }      //识别出CODE
                else if(strcmp(Temp,".data")==0) { TOKEN.ID=T_DATA; return 0; } //识别出DATA
                else { TOKEN.ID=ERROR_NPRO; return -1; }                       //出错，无法继续分析,程序终止
            }
        case N_DATAA:     //扫描T_DATA|T_IDNAME|T_ORG_DATA
            {
                if(ScanIdname()==0) return 0;
                ReadAString(Temp);
                if(strcmp(Temp,".org_data")==0)  { TOKEN.ID=T_ORG_DATA; return 0; }
                else if(strcmp(Temp,".data")==0) { TOKEN.ID=T_DATA; return 0; }
                else { TOKEN.ID=ERROR_TERMINAL; return -1; }    //终止
            }
        case N_DATAB:
            {
                if (ScanAddress() == 0) {
                    return 0;
                }
                return ScanEndl();
            }
        case N_VARAA: //扫描T_DATA|T_IDNAME|T_DD|T_ORG_DATA|T_CODE
            {
                if(ScanIdname()==0) return 0;
                ReadAString(Temp);
                if(strcmp(Temp,"word")==0) { TOKEN.ID=T_DW; return 0; }
                else if (strcmp(Temp, ".text") == 0) { TOKEN.ID = T_CODE; return 0; }
                else if(strcmp(Temp,".data")==0) { TOKEN.ID=T_DATA; return 0; }
                else if(strcmp(Temp,".org_data")==0) { TOKEN.ID=T_ORG_DATA; return 0; }
                else { TOKEN.ID=ERROR_TERMINAL; return -1; }    //终止
            }
        case N_DATASEG:
        case N_DATAENDS: //扫描T_DATA
            {
                TOKEN.ID=T_DATA;
                ReadAString(Temp);
                if(strcmp(Temp,".data")==0)  return 0;
                else   return -1;       //滤过这个单词，认为已经识别
            }
        case N_ORG_DATA: //扫描T_ORG_DATA
            {
                TOKEN.ID=T_ORG_DATA;
                ReadAString(Temp);
                if(strcmp(Temp,".org_data")==0) return 0;
                else  return -1;     //滤过这个单词，认为已经识别
            }
        case N_VAR:
        case N_SEG:
        case N_STARTSEGID:
        case N_SUBSEGID: //扫描T_IDNAME
            {
                TOKEN.ID=T_IDNAME;
                return ScanIdname(); //滤过这个单词，认为已经识别
            }
        case N_VARS:  //扫描T_IDNAME||T_DATA
            {
                if(ScanIdname()==0) return 0;
                ReadAString(Temp);
                if(strcmp(Temp,".data")==0)  { TOKEN.ID=T_DATA; return 0; }
                else { TOKEN.ID=ERROR_NVARS; return -1; }  //滤过一个单词，认为已经识别出T_IDNAME
            }
        case N_VARA:  //扫描T_ENDL|T_COMMA
            {
                char ch=ReadAChar();
                if (ch==',') { TOKEN.Content[++TOKEN.Idx]=ch;TOKEN.ID=T_COMMA; return 0; }
                else
                {
                    WriteAChar(ch);
                    if(ScanEndl()==0)  return 0;
                    else { TOKEN.ID=ERROR_NVARA; return -1; }  //滤过一行，认为已经识别出T_ENDL
                }
            }
        case N_CODE:
        case N_CODESEG:
        case N_CODEENDS: //扫描T_CODE
            {
                TOKEN.ID=T_CODE;
                ReadAString(Temp);
                if(strcmp(Temp,".text")==0)  return 0;
                else  return -1;             //滤过这个单词，认为已经识别
            }
        case N_CODEA: //扫描T_IDNAME|T_ORG_CODE
            {
                if(ScanIdname()==0) return 0;
                if (ScanCom() == 0) return 0;
                ReadAString(Temp);
                if(strcmp(Temp,".org_code")==0)  { TOKEN.ID=T_ORG_CODE; return 0; }
                else { TOKEN.ID=ERROR_NCODEA; return -1; }     //可以修复的错误
            }
        case N_ORG_CODE: //扫描T_ORG_CODE
            {
                TOKEN.ID=T_ORG_CODE;
                ReadAString(Temp);
                if(strcmp(Temp,".org_code")==0)   return 0;
                else  return -1;        //滤过这个单词，认为已经识别
            }
        case N_ORDER:   //扫描T_COM|T_ORG_CODE
            {
                if(ScanCom()==0) return 0;
                else { TOKEN.ID=ERROR_NORDER; return -1; }     //滤过一条指令，认为已经识别出T_ENDL
            }
        case N_COM:      //扫描T_COM
            {
                if(ScanCom()==0&&TOKEN.ID!=T_ORG_CODE) return 0;
                else { TOKEN.ID=ERROR_NCOM; return -1; }       //滤过一条指令，认为已经识别出T_ENDL
            }
        case N_ORDERS:   //扫描T_IDNAME|T_END|T_ORG_CODE|T_COM|T_CODE
            {
                if(ScanIdname()==0) return 0;
                if (ScanCom() == 0) return 0;
                ReadAString(Temp);
                if (strcmp(Temp, "end") == 0) {
                    TOKEN.ID == T_END; return 0;
                }
                if (strcmp(Temp, ".text") == 0) {
                    TOKEN.ID = T_CODE; return 0;
                }
                TOKEN.ID = ERROR_NORDERS;
                return -1;
            }
        case N_ADDR:    //扫描T_IDNAME|T_ADDR
            {
                if (ScanIdname() == 0)
                    return 0;
                else
                    return ScanAddress();
            }
        case N_IMMEDIATE: //扫描T_IDNAME|T_IMMEDIATE
            {
                if(ScanIdname()==0)  return 0;
                else
                {
                    TOKEN.ID=T_IMMEDIATE;
                    if(fin.peek()=='-')
                    {
                        TOKEN.Content[++TOKEN.Idx]=ReadAChar();
                        return Scan10Radix(6);
                    }
                    if(Scan16Radix(4)==0) return 0; //16进制最长是4位
                    WriteAString();
                    if(Scan10Radix(5)==0) return 0; //10进制最长是5位
                    WriteAString();
                    if(Scan2Radix(16)==0) return 0; //2进制最长是16位
                    TOKEN.ID=ERROR_NIMMEDIATE;      //可以修复的错误
                    return -1;
                }
            }
        case N_ENDSEGID: //扫描T_END
            {
                TOKEN.ID=T_END;
                ReadAString(Temp);
                if(strcmp(Temp,"end")==0)   return 0;
                else  return -1;   //滤过这个单词，认为已经识别
            }
        case N_CP0: //扫描T_COMMA|T_ENDL
            {
                char ch=ReadAChar();
                if (ch==',') {
                    TOKEN.Content[++TOKEN.Idx]=ch;TOKEN.ID=T_COMMA; return 0;
                }
                else
                {
                    WriteAChar(ch);
                    if(ScanEndl()==0)  return 0;
                    else { TOKEN.ID=ERROR_NVARA; return -1; }  //滤过一行，认为已经识别出T_ENDL
                }
            }
        default: //不可能出现的情况
            break;
            }
    }
    else     //终结符，依据栈顶元素查找所需单词
    {
        TOKEN.ID=TypeOfToken;
        switch (TOKEN.ID)
        {
        case T_IDNAME:
            return ScanIdname();
        case T_ENDL:
            return ScanEndl();
        case T_NUM: //数据段中的数据
            {
                if(fin.peek()=='-')
                {
                    TOKEN.Content[++TOKEN.Idx]=ReadAChar();
                    return Scan10Radix(11);
                }
                if(Scan16Radix(8)==0)  return 0; //16进制最长是8位
                WriteAString();
                if(Scan10Radix(10)==0) return 0; //10进制最长是10位
                WriteAString();
                return Scan2Radix(32);           //2进制最长是32位
            }
        case T_ADDR:
            {
                if(Scan16Radix(4)==0) return 0; //16进制最长是4位
                WriteAString();
                if(Scan10Radix(5)==0) return 0; //10进制最长是5位
                WriteAString();
                return Scan2Radix(16);          //2进制最长是16位
            }
        case T_IMMEDIATE:
            {
                if(fin.peek()=='-')
                {
                    TOKEN.Content[++TOKEN.Idx]=ReadAChar();
                    return Scan10Radix(6);
                }
                if(Scan16Radix(4)==0) return 0; //16进制最长是4位
                WriteAString();
                if(Scan10Radix(5)==0) return 0; //10进制最长是5位
                WriteAString();
                return Scan2Radix(16);          //2进制最长是16位
            }
        case T_SHAMT:
            {
                if(Scan16Radix(2)==0) return 0; //16进制最长是2位
                WriteAString();
                if(Scan10Radix(2)==0) return 0; //10进制最长是2位
                WriteAString();
                return Scan2Radix(5);           //2进制最长是5位
            }
        case T_RS:
        case T_RT:
        case T_RD:
            return ScanReg();//TypeOfToken);
        case T_BRS:
            {
                char ch=ReadAChar();
                if(ch!='(') { WriteAChar(ch); return -1; }
                else
                {
                    TOKEN.Content[++TOKEN.Idx]='(';
                    if(ScanReg()==0)//T_RS)==0)
                    {
                        TOKEN.ID=T_BRS;
                        if(ReadAChar()==')') { TOKEN.Content[++TOKEN.Idx]=')'; return 0; }
                        else { WriteAChar(ch);  return -1; }
                    }
                    else { TOKEN.ID=T_BRS; return -1; }
                }
            }
        case T_ENDS:
            {
                ReadAString(Temp);
                if(strcmp(Temp,"ends")==0) return 0;
                else return -1;
            }
        case T_SEG:
            {
                ReadAString(Temp);
                if(strcmp(Temp,"seg")==0) return 0;
                else return -1;
            }
        case T_DW:
            {
                ReadAString(Temp);
                if(strcmp(Temp,".word")==0) return 0;
                else return -1;
            }
        case T_COMMA:
            {
                char ch=ReadAChar();
                if(ch==',') { TOKEN.Content[++TOKEN.Idx]=','; return 0; }
                else  { WriteAChar(ch); return -1; }
            }
        case T_COLON:
            {
                char ch=ReadAChar();
                //扫描T_ENDL直到遇到新的指令
                if(ch==':') { ScanEndl();TOKEN.ID=T_COLON;TOKEN.Content[++TOKEN.Idx]=':'; return 0; }
                else  { WriteAChar(ch);  return -1; }
            }
        case T_SEL:
            {
                if(fin.peek()=='-')
                {
                    TOKEN.Content[++TOKEN.Idx]=ReadAChar();
                    return Scan10Radix(4);
                }
                if(Scan16Radix(1)==0) return 0; //16进制最长是1位
                WriteAString();
                if(Scan10Radix(1)==0) return 0; //10进制最长是1位
                WriteAString();
                return Scan2Radix(3);           //2进制最长是3位
            }
        default:
            break;
        }
    }
    return 0;
}

//扫描T_RCOM||T_SRCOM||T_SLLRCOM||T_ICOM||T_SICOM||T_LWICOM||T_JCOM||T_ORG_CODE||T_NOP||T_PCOM||T_JBCOM
int assembler::ScanCom()
{
    char Temp[COMMOM_SIZE]={'\0'};
    ReadAString(Temp);
    for(int i=0;i<67;i++)               //开始查指令表
    {
        if(strcmp(Temp,KeyWords[i])==0)  //找到了
        {

            if(i<=13)      {TOKEN.ID=T_RCOM;}
            else if(i<=16) {TOKEN.ID=T_SLLRCOM;}
            else if(i<=19) {TOKEN.ID=T_SRCOM;}
            else if(i<=21) {TOKEN.ID=T_MRCOM;}
            else if(i<=23) {TOKEN.ID=T_MFCRCOM;}
            else if(i<=27) {TOKEN.ID=T_MULRCOM;}
            else if(i<=30) {TOKEN.ID=T_SYSRCOM;}
            else if(i<=31) {TOKEN.ID=T_JRCOM;}

            else if(i<=38) {TOKEN.ID=T_ICOM;}
            else if(i<=46) {TOKEN.ID=T_LWICOM;}
            else if(i<=47) {TOKEN.ID=T_SICOM;}
            else if(i<=49) {TOKEN.ID=T_BICOM;}
            else if(i<=55) {TOKEN.ID=T_BGICOM;}

            else if(i<=57) {TOKEN.ID=T_JCOM;}

            else if(i<=58) {TOKEN.ID=T_NOP;}
            else if(i<=60) {TOKEN.ID=T_PCOM;}
            else if(i<=64) {TOKEN.ID=T_JBCOM;}
            else          {TOKEN.ID=T_ORG_CODE;}
            strlwr(TOKEN.Content);
            return 0;
        }
    }
    WriteAString();
    return -1;     //未找到，指令有误
}
//扫描T_IDNAME，正规式(a~z|A~Z|_)(0~9|a~z|A~Z|_)*，最长为99个字符
int assembler::ScanIdname()
{
    TOKEN.ID=T_IDNAME;
    char ch;
    while(1)
    {
        ch=ReadAChar();
        if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_')  //字母或下划线
        {
            TOKEN.Content[++TOKEN.Idx]=ch;
        }
        else if(ch>='0'&&ch<='9')  //数字
        {
            if(TOKEN.Idx==-1) { WriteAChar(ch); return -1;}  //不允许数字出现在标志符第一位
            TOKEN.Content[++TOKEN.Idx]=ch;
        }
        else if(IsSeparate(ch))  //正常
        {
            WriteAChar(ch);                 //放回去，因为该字符不一定是无用字符
            if(TOKEN.Idx!=-1&&!IsKeyWords(TOKEN.Content))
            {
                strlwr(TOKEN.Content);     //不区分大小写
                return 0;
            }
            else
            {
                WriteAString();
                return -1;
            }
        }
        else  //出错
        {
            WriteAChar(ch);
            WriteAString();
            return -1;
        }
    }
}

int assembler::ScanAddress() {
    TOKEN.ID = T_ADDR;
    if(Scan16Radix(4)==0) return 0; //16进制最长是4位
    WriteAString();
    if(Scan10Radix(5)==0) return 0; //10进制最长是5位
    WriteAString();
    if(Scan2Radix(16)==0) return 0; //2进制最长是16位
    TOKEN.ID=ERROR_NADDR;           //可以修复的错误
    return -1;
}

//扫描16进制数
int assembler::Scan16Radix(int maxlen) {
    static char Temp[COMMOM_SIZE]={'0'};
    ReadAString(Temp);
    // Add 2 chars for the prefix "0x".
    if (TOKEN.Idx > maxlen + 2) {
        return -1;
    }
    if (!std::regex_match(Temp, std::regex("0x[0-9a-f]*"))) {
        return -1;
    }
    for (int i = 0; i <= TOKEN.Idx; ++i) {
        TOKEN.Content[i] = TOKEN.Content[i + 2];
    }
    return 0;
}

//扫描2进制数
int assembler::Scan2Radix(int maxlen)
{
    char ch;
    while(1)
    {
        ch=ReadAChar();
        if(ch=='b'||ch=='B')
        {
            if(TOKEN.Idx==-1||TOKEN.Idx>=maxlen) break;  //数据位数不对
            TOKEN.Content[++TOKEN.Idx]='b';    //添加后缀，成功识别
            return 0;
        }
        else if(ch=='0'||ch=='1')
            TOKEN.Content[++TOKEN.Idx]=ch;
        else
            break;
    }
    WriteAChar(ch);
    return -1;
}

//扫描10进制数
int assembler::Scan10Radix(int maxlen)
{
    static char Temp[COMMOM_SIZE]={'0'};
    ReadAString(Temp);
    // Add 2 chars for the prefix "0x".
    if (TOKEN.Idx > maxlen) {
        return -1;
    }
    if (!std::regex_match(Temp, std::regex("[+-]?[0-9]*"))) {
        return -1;
    }
    return 0;
}

//扫描T_RS|T_RT|T_RD  正规式$v(0~1)|$a(0~3)|$t(0~9)|$s(0~9)|$i(0~1)|$sp|$ra|$zero|$(0~31)|$at|SR|Cause|EPC|Ebase
int assembler::ScanReg()
{

    int State=0;
    char ch;
    while(1)
    {
        ch=ReadAChar();
        TOKEN.Content[++TOKEN.Idx]=ch;
        switch(ch)
        {
        case '$':
            if(State==0){ State=51; break; }
            else{return -1; }
            break;
        case 'B':
            if(State==0){ State=169; break; }
            if(State==62){ State=125; break; }
            else{return -1; }
            break;
        case 'C':
            if(State==0){ State=110; break; }
            if(State==62){ State=66; break; }
            if(State==63){ State=157; break; }
            if(State==65){ State=7; break; }
            if(State==66){ State=26; break; }
            if(State==67){ State=27; break; }
            if(State==68){ State=33; break; }
            if(State==74){ State=156; break; }
            if(State==163){ State=120; break; }
            else{return -1; }
            break;
        case 'D':
            if(State==0){ State=64; break; }
            else{return -1; }
            break;
        case 'E':
            if(State==0){ State=62; break; }
            if(State==64){ State=75; break; }
            if(State==70){ State=34; break; }
            if(State==84){ State=181; break; }
            if(State==164){ State=77; break; }
            if(State==165){ State=123; break; }
            else{return -1; }
            break;
        case 'H':
            if(State==0){ State=81; break; }
            if(State==69){ State=115; break; }
            if(State==166){ State=112; break; }
            if(State==167){ State=113; break; }
            if(State==168){ State=114; break; }
            else{return -1; }
            break;
        case 'I':
            if(State==0){ State=180; break; }
            if(State==71){ State=93; break; }
            else{return -1; }
            break;
        case 'L':
            if(State==0){ State=72; break; }
            if(State==69){ State=130; break; }
            if(State==72){ State=183; break; }
            if(State==166){ State=131; break; }
            if(State==167){ State=128; break; }
            if(State==168){ State=129; break; }
            else{return -1; }
            break;
        case 'P':
            if(State==0){ State=76; break; }
            if(State==62){ State=65; break; }
            if(State==75){ State=68; break; }
            if(State==77){ State=67; break; }
            else{return -1; }
            break;
        case 'R':
            if(State==0){ State=171; break; }
            if(State==76){ State=71; break; }
            if(State==79){ State=5; break; }
            if(State==80){ State=165; break; }
            else{return -1; }
            break;
        case 'S':
            if(State==0){ State=79; break; }
            if(State==5){ State=63; break; }
            if(State==75){ State=162; break; }
            else{return -1; }
            break;
        case 'T':
            if(State==0){ State=170; break; }
            else{return -1; }
            break;
        case 'W':
            if(State==0){ State=83; break; }
            if(State==81){ State=80; break; }
            else{return -1; }
            break;
        case '1':
            if(State==20){ State=50; break; }
            if(State==46){ State=49; break; }
            if(State==51){ State=53; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==56){ State=14; break; }
            if(State==57){ State=41; break; }
            if(State==58){ State=42; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            if(State==61){ State=45; break; }
            else{return -1; }
            break;
        case '2':
            if(State==20){ State=50; break; }
            if(State==51){ State=54; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==58){ State=42; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            else{return -1; }
            break;
        case '3':
            if(State==20){ State=50; break; }
            if(State==51){ State=46; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==58){ State=42; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            else{return -1; }
            break;
        case '0':
            if(State==46){ State=49; break; }
            if(State==51){ State=52; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==56){ State=13; break; }
            if(State==57){ State=41; break; }
            if(State==58){ State=42; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            if(State==61){ State=45; break; }
            else{return -1; }
            break;
        case '4':
            if(State==51){ State=52; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            else{return -1; }
            break;
        case '5':
            if(State==51){ State=52; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            else{return -1; }
            break;
        case '6':
            if(State==51){ State=52; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            else{return -1; }
            break;
        case '7':
            if(State==51){ State=52; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            else{return -1; }
            break;
        case '8':
            if(State==51){ State=52; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            else{return -1; }
            break;
        case '9':
            if(State==51){ State=52; break; }
            if(State==53){ State=47; break; }
            if(State==54){ State=48; break; }
            if(State==59){ State=43; break; }
            if(State==60){ State=44; break; }
            else{return -1; }
            break;
        case 'a':
            if(State==51){ State=58; break; }
            if(State==64){ State=154; break; }
            if(State==76){ State=102; break; }
            if(State==83){ State=155; break; }
            if(State==86){ State=2; break; }
            if(State==87){ State=40; break; }
            if(State==88){ State=168; break; }
            if(State==110){ State=89; break; }
            if(State==125){ State=147; break; }
            if(State==169){ State=94; break; }
            if(State==170){ State=107; break; }
            if(State==171){ State=124; break; }
            if(State==172){ State=134; break; }
            if(State==173){ State=135; break; }
            if(State==174){ State=140; break; }
            else{return -1; }
            break;
        case 'i':
            if(State==51){ State=61; break; }
            if(State==83){ State=141; break; }
            if(State==108){ State=105; break; }
            if(State==112){ State=12; break; }
            if(State==113){ State=30; break; }
            if(State==114){ State=31; break; }
            if(State==115){ State=36; break; }
            else{return -1; }
            break;
        case 'r':
            if(State==51){ State=86; break; }
            if(State==62){ State=143; break; }
            if(State==133){ State=160; break; }
            if(State==136){ State=10; break; }
            if(State==137){ State=25; break; }
            if(State==138){ State=39; break; }
            if(State==139){ State=164; break; }
            if(State==140){ State=97; break; }
            if(State==141){ State=101; break; }
            if(State==142){ State=103; break; }
            if(State==143){ State=122; break; }
            if(State==144){ State=127; break; }
            if(State==181){ State=137; break; }
            else{return -1; }
            break;
        case 's':
            if(State==51){ State=60; break; }
            if(State==135){ State=111; break; }
            if(State==146){ State=96; break; }
            if(State==147){ State=98; break; }
            else{return -1; }
            break;
        case 't':
            if(State==51){ State=59; break; }
            if(State==58){ State=4; break; }
            if(State==90){ State=74; break; }
            if(State==95){ State=179; break; }
            if(State==120){ State=116; break; }
            if(State==145){ State=133; break; }
            if(State==151){ State=8; break; }
            if(State==152){ State=11; break; }
            if(State==153){ State=38; break; }
            if(State==154){ State=88; break; }
            if(State==155){ State=158; break; }
            if(State==156){ State=118; break; }
            if(State==157){ State=119; break; }
            else{return -1; }
            break;
        case 'v':
            if(State==51){ State=57; break; }
            else{return -1; }
            break;
        case 'z':
            if(State==51){ State=178; break; }
            else{return -1; }
            break;
        case 'A':
            if(State==55){ State=182; break; }
            if(State==162){ State=78; break; }
            if(State==183){ State=148; break; }
            else{return -1; }
            break;
        case 'p':
            if(State==60){ State=1; break; }
            if(State==126){ State=174; break; }
            if(State==134){ State=24; break; }
            else{return -1; }
            break;
        case 'n':
            if(State==62){ State=145; break; }
            if(State==117){ State=95; break; }
            if(State==120){ State=153; break; }
            if(State==123){ State=87; break; }
            if(State==124){ State=175; break; }
            if(State==149){ State=151; break; }
            if(State==180){ State=90; break; }
            else{return -1; }
            break;
        case 'M':
            if(State==63){ State=172; break; }
            if(State==73){ State=173; break; }
            else{return -1; }
            break;
        case 'e':
            if(State==64){ State=85; break; }
            if(State==76){ State=142; break; }
            if(State==91){ State=161; break; }
            if(State==96){ State=6; break; }
            if(State==97){ State=9; break; }
            if(State==98){ State=21; break; }
            if(State==99){ State=73; break; }
            if(State==100){ State=84; break; }
            if(State==101){ State=92; break; }
            if(State==178){ State=144; break; }
            if(State==179){ State=159; break; }
            else{return -1; }
            break;
        case 'V':
            if(State==78){ State=70; break; }
            if(State==82){ State=55; break; }
            else{return -1; }
            break;
        case 'b':
            if(State==85){ State=150; break; }
            else{return -1; }
            break;
        case 'c':
            if(State==89){ State=104; break; }
            if(State==158){ State=109; break; }
            else{return -1; }
            break;
        case 'u':
            if(State==89){ State=146; break; }
            if(State==117){ State=149; break; }
            if(State==150){ State=106; break; }
            else{return -1; }
            break;
        case 'd':
            if(State==90){ State=91; break; }
            if(State==92){ State=18; break; }
            if(State==93){ State=19; break; }
            if(State==94){ State=82; break; }
            if(State==148){ State=177; break; }
            if(State==175){ State=132; break; }
            if(State==176){ State=136; break; }
            if(State==177){ State=138; break; }
            if(State==182){ State=176; break; }
            else{return -1; }
            break;
        case 'f':
            if(State==95){ State=108; break; }
            if(State==103){ State=163; break; }
            else{return -1; }
            break;
        case 'g':
            if(State==102){ State=99; break; }
            if(State==105){ State=20; break; }
            if(State==106){ State=32; break; }
            if(State==107){ State=167; break; }
            else{return -1; }
            break;
        case 'h':
            if(State==104){ State=100; break; }
            if(State==109){ State=69; break; }
            else{return -1; }
            break;
        case 'o':
            if(State==110){ State=117; break; }
            if(State==122){ State=139; break; }
            if(State==127){ State=3; break; }
            if(State==128){ State=28; break; }
            if(State==129){ State=29; break; }
            if(State==130){ State=35; break; }
            if(State==131){ State=56; break; }
            if(State==132){ State=121; break; }
            else{return -1; }
            break;
        case 'k':
            if(State==111){ State=16; break; }
            else{return -1; }
            break;
        case 'l':
            if(State==116){ State=37; break; }
            if(State==118){ State=22; break; }
            if(State==119){ State=23; break; }
            else{return -1; }
            break;
        case 'm':
            if(State==117){ State=126; break; }
            if(State==121){ State=17; break; }
            else{return -1; }
            break;
        case 'x':
            if(State==159){ State=152; break; }
            if(State==161){ State=15; break; }
            else{return -1; }
            break;
        case 'y':
            if(State==160){ State=166; break; }
            else{return -1; }
            break;
        case ' ':
        case '\t':
        case '\n':
        case ';':
        case ',':
        case ':':
        case '\0':
        case ')':
            WriteAChar(ch);
            TOKEN.Content[TOKEN.Idx--]='\0';
            if (State==1||State==2||State==3||State==4||State==5||State==6||State==7||State==8||State==9||State==10||State==11||State==12||State==13||State==14||State==15||State==16||State==17||State==18||State==19||State==20||State==21||State==22||State==23||State==24||State==25||State==26||State==27||State==28||State==29||State==30||State==31||State==32||State==33||State==34||State==35||State==36||State==37||State==38||State==39||State==40||State==41||State==42||State==43||State==44||State==45||State==46||State==47||State==48||State==49||State==50||State==52||State==53||State==54){
            strlwr(TOKEN.Content);return 0;}
            else return -1;
        default:
            return -1;
        }
    }
}
//扫描T_ENDL，正规式 (('\n'|'\t'|' ')*;(除换行符外的任意符号,注释部分)*'\n')*
int assembler::ScanEndl()
{
    TOKEN.ID=T_ENDL;
    int State=0;
    while(1)
    {
        char ch=ReadAChar();
        switch(ch)
        {
        case '\t':
        case ' ':
            if(State==0) {State=1;break;}
            else break;
        case ';':
            State=2;break;
        case '\n':
            State=3;break;
        case '\0':
            return 0;
        default:
            if(State==2) break;
            else if(State==3){WriteAChar(ch);return 0;}
            else  { WriteAChar(ch);return -1; }
        }
    }
}
//扫描器结束

//翻译器核心程序
int assembler::Translater() //根据当前规则以及读到的单词进行翻译
{
    switch(CurrentRule)           //根据当前规则
    {
    //数据部分
    case 11:
        if(TOKEN.ID==T_IDNAME)     return InsertVar();
        else if(TOKEN.ID==T_NUM)   return InsertData();
        else return 0;
    case 12:
    case 14:
        if(TOKEN.ID==T_NUM)  return InsertData();
        else return 0;
    case 38:
        return TranslateImmi();
    //地址部分
    case 8:
    case 21:
    case 40:
        if(TOKEN.ID==T_ADDR)  return TranslateAddr();
        else return 0;
    //指令部分
    case 26:
        return TranslateRcom();
    case 27:
        return TranslateSrcom();
    case 28:
        return TranslateSllRcom();
    case 29:
        return TranslateIcom();
    case 30:
        return TranslateSicom();
    case 31:
        return TranslateLwicom();
    case 32:
        return TranslateJcom();
    case 33:
        return TranslateBcom();
    case 34:
        return TranslatePcom();
    case 35:
        return TranslateJbcom();
    case 36:
        return TranslateNop();
    case 37:
    case 39:
        return TranslateID();
    //标号部分，填表
    case 23:
    case 44:
        if(TOKEN.ID==T_IDNAME) return InsertSegID();
        else return 0;
    case 45:  //和第一个标号一样
        if(TOKEN.ID==T_IDNAME) return SearchFirstSegID(TOKEN.Content);
        else return 0;

    //新增指令部分
    case 47:
        return TranslateMRCOM();
    case 48:
        return TranslateMFCRCOM();
    case 53:
        return TranslateSel();
    case 49:
        return TranslateMULRCOM();
    case 50:
        return TranslateSYSRCOM();
    case 51:
        return TranslateJRCOM();
    case 52:
        return TranslateBGICOM();
    default:
        return 0;  //其他语法规则不用翻译
    }
}
//插入一个变量，保存了变量所定义数据的首地址
int assembler::InsertVar()
{
    for(My_Var *p=VARS_TABLE;p!=NULL;p=p->Next) //判断变量是否重定义
    {
        if(strcmp(p->Name,TOKEN.Content)==0)
        {
            ErrorID=ERROR_VAR_REDEFINITION;
            return -1;
        }
    }
    //如果上面的情况都没有出现，正常插入变量
    My_Var* pVar=new My_Var;                      //新变量
    pVar->Addr=CurrentDataAddr+1;                 //加1，变量所定义数据的地址
    strcpy(pVar->Name,TOKEN.Content);             //变量名
    pVar->Next=NULL;                              //下一个变量
    //填表
    if(VARS_TABLE==NULL)  VAR_POS=VARS_TABLE=pVar;
    else   VAR_POS=VAR_POS->Next=pVar;
    pVar=NULL;
    return 0;
}
//插入一个数据
int assembler::InsertData()
{
    CurrentDataAddr++;                  //地址加1
    for(My_Data *p=DATAS_TABLE;p!=NULL;p=p->Next)   //判断地址是否冲突
    {
        if(p->Addr==CurrentDataAddr)    //存在冲突
        {
            ErrorID=ERROR_DADDRESS_CONFLICT;
            return -1;
        }
    }
    if(CurrentDataAddr>=RAM_SIZE)              //判断地址是否越界
    {
        ErrorID=ERROR_DADDRESS_OVERFLOW;
        return -1;
    }
    char Value[DATA_SIZE]={'0'};
    if(StrtoBStr(TOKEN.Content,Value,32)==-1)       //判断数据是否越界
    {
        ErrorID=ERROR_DATA_OVERFLOW;
        return -1;
    }
    //如果上面的情况都没有出现，表明数据以及数据地址都没问题
    My_Data *pData=new My_Data;        //新数据
    pData->Addr=CurrentDataAddr;       //内存中的地址
    strcpy(pData->Value,Value);        //二进制表示形式
    pData->Next=NULL;                  //下一个数据
    //填表
    if(DATAS_TABLE==NULL)	DATA_POS=DATAS_TABLE=pData;
    else	DATA_POS=DATA_POS->Next=pData;
    pData=NULL;
    return 0;
}
//插入一个标号
int assembler::InsertSegID()
{
    for(My_SegID *p=SEGIDS_TABLE;p!=NULL;p=p->Next) //判断标号是否冲突
    {
        if(strcmp(p->Name,TOKEN.Content)==0)
        {
            ErrorID=ERROR_SEGID_REDEFINITION;
            return -1;
        }
    }
    //如果上面的情况都没有出现，正常插入标号
    My_SegID *pSegID=new My_SegID;             //新标号
    pSegID->Addr=CurrentComAddr+1;             //加1,因为标号表示的是下一条指令的地址
    strcpy(pSegID->Name,TOKEN.Content);        //标号内容
    pSegID->Next=NULL;                         //下一个标号
    //填表
    if(SEGIDS_TABLE==NULL) SEGID_POS=SEGIDS_TABLE=pSegID;
    else  SEGID_POS=SEGID_POS->Next=pSegID;
    pSegID=NULL;
    return 0;
}
//插入一条指令
int assembler::InsertCom(char segName[6],int type)
{
    cout<<segName<<" "<<type;
    CurrentComAddr++;
    for(My_Com *p=COMS_TABLE;p!=NULL;p=p->Next)//判断地址是否冲突
    {
        if(p->Addr==CurrentComAddr) //存在冲突
        {
            cout<<" 地址冲突"<<endl;

            ErrorID=ERROR_CADDRESS_CONFLICT;
            return -1;
        }
    }
    cout<<" 地址不冲突";
    if(CurrentComAddr>=ROM_SIZE)          //判断地址是否越界
    {
        cout<<" 地址越界"<<endl;
        ErrorID=ERROR_CADDRESS_OVERFLOW;
        return -1;
    }
    cout<<" 地址不越界"<<endl;
    //如果上面的情况都没有出现，表明代码以及代码地址都没问题
    My_Com* pCom=new My_Com;                       //新指令
    pCom->Addr=CurrentComAddr;                     //地址
    strcpy(pCom->JSegName,segName);                //助记符
    pCom->NumOfCom=SearchNum(segName);             //编号
    pCom->TypeOfCom=type;                          //类型
    for(int i=0;i<COM_SIZE;i++) pCom->Com[i]='\0'; //机器码
    pCom->Next=NULL;                               //下一条指令
    //填表
    if(COMS_TABLE==NULL) COM_POS=COMS_TABLE=pCom;
    else	COM_POS=COM_POS->Next=pCom;
    pCom=NULL;
    return 0;
}
//插入回填表
int assembler::InsertBack()
{
    My_Back* pBack=new My_Back;        //新的回填指令
    pBack->Com=COM_POS;                //指令指针
    strcpy(pBack->Name,TOKEN.Content); //标号内容
    pBack->lineNum=NumOfLine;          //记录标号的位置
    pBack->Next=NULL;                  //下一条回填指令
    //填表
    if(BACKS_TABLE==NULL)	BACK_POS=BACKS_TABLE=pBack;
    else	BACK_POS=BACK_POS->Next=pBack;
    pBack=NULL;
    return 0;
}
//查符号表
int assembler::SearchVar(char Name[COMMOM_SIZE])
{
    for(My_Var *pVar=VARS_TABLE;pVar!=NULL;pVar=pVar->Next)
    {
        if(strcmp(Name,pVar->Name)==0)
            return pVar->Addr;
    }
    return -1;
}

//查标号表
int assembler::SearchSegID(char Name[COMMOM_SIZE])
{
    for(My_SegID *pSegID=SEGIDS_TABLE;pSegID!=NULL;pSegID=pSegID->Next)
    {
        if(strcmp(Name,pSegID->Name)==0)
            return pSegID->Addr;
    }
    return -1;
}
int assembler::SearchFirstSegID(char Name[COMMOM_SIZE])
{
    if(SEGIDS_TABLE!=NULL)
    {
        if(strcmp(Name,SEGIDS_TABLE->Name)==0)
            return 0;
        else
        {
            ErrorID=ERROR_NOTFIRST_LABEL;
            return -1;
        }
    }
    else
    {
        ErrorID=ERROR_MISSING_LABEL;
        return -1;
    }
}
//查指令编号
int assembler::SearchNum(char Name[COMMOM_SIZE])
{
    for(int i=0;i<COM_TABLE_SIZE;i++)
    {
        if(strcmp(Name,KeyWords[i])==0)
            return i;
    }
    return -1;
}
//查助记符表
int assembler::SearchOp(char Name[COMMOM_SIZE],char Op[OP_SIZE])
{

    for(int i=0;i<OP_TABLE_SIZE;i++)
    {
        if(strcmp(Name,OpTable[i].Name)==0)
        {
            strcpy(Op,OpTable[i].Op);
            return 0;
        }
    }
    strcpy(Op,"");
    return -1;
}
//查功能号表
int assembler::SearchFunc(char Name[COMMOM_SIZE],char Func[FUNC_SIZE])
{
    for(int i=0;i<FUNC_TABLE_SIZE;i++)
    {
        if(strcmp(Name,FuncTable[i].Name)==0)
        {
            strcpy(Func,FuncTable[i].Func);
            return 0;
        }
    }
    strcpy(Func,"");
    return -1;
}
//查寄存器号
int assembler::SearchReg(char Name[COMMOM_SIZE],char Reg[REG_SIZE])
{
    for(int i=0;i<REG_TABLE_SIZE;i++)
    {
        if(strcmp(Name,RegTable[i].Name)==0)
        {
            if (i < 64) //属于普通寄存器
            {
                strcpy(Reg,RegTable[i].Reg);
                return 0;
            }
            else
            {
                strcpy(Reg,"");
                ErrorID = ERROR_CP0_REG;
                Error(ERROR_SEMANTIC);
                return -1;
            }
        }
    }

    strcpy(Reg,"");
    return -1;

}

//查寄存器号
int assembler::SearchSel(char Name[COMMOM_SIZE],char Sel[SEL_SIZE])
{
    for(int i=0;i<SEL_TABLE_SIZE;i++)
    {
        if(strcmp(Name,SelTable[i].Name)==0)
        {

                strcpy(Sel,SelTable[i].Sel);
                return 0;
        }
    }

    strcpy(Sel,"");
    return -1;

}

int assembler::SearchCP0Reg(char Name[COMMOM_SIZE],char Reg[REG_SIZE],char Sel[SEL_SIZE])
{
    for(int i=0;i<REG_TABLE_SIZE;i++)
    {
        if(strcmp(Name,RegTable[i].Name)==0)
        {
            if (i<64)//普通，返回需要翻译Sel
            {

                Warning(WARNING_CP0_REG);

                strcpy(Reg,RegTable[i].Reg);
                return 1;
            }
            else
            {//在此处翻译Sel部分
                strcpy(Reg,RegTable[i].Reg);
                int findSel=SearchSel(Name,Sel);
                if (findSel == -1)
                    strcpy(Sel,"000");
                return 0;
            }
        }


    }

    strcpy(Reg,"");
    return -1;

}
//回填函数
int assembler::SearchBack()
{
    for(My_Back* pBack=BACKS_TABLE;pBack!=NULL;pBack=pBack->Next)           //外层是需要回填的指令地址表
    {
        My_SegID* pSegID;
        for(pSegID =SEGIDS_TABLE;pSegID!=NULL;pSegID=pSegID->Next) //内层是标号表
        {
            if(strcmp(pBack->Name,pSegID->Name)==0)  //如果匹配
            {
                int addr=pSegID->Addr;
                if(pBack->Com->TypeOfCom==T_BICOM)    //bne或beq指令，需要特殊处理
                    addr=addr-(pBack->Com->Addr)-1;
                char Addr[ADDR_SIZE]={'0'};
                DNumtoBStr(addr,Addr,16);              //把一个十进制的数转换为二进制字符串
                for(int i=16;i<32;i++)
                    pBack->Com->Com[i]=Addr[i-16];
                break;
            }
        }
        if(pSegID==NULL)//所引用的标号不存在
        {
            NumOfLine=pBack->lineNum;
            strcpy(TOKEN.Content,pBack->Name);
            ErrorID=ERROR_UNDEFINED_LABEL;  //使用了未定义的标号
            Error(ERROR_SEMANTIC);          //语义错误
        }
    }
    if(NumOfError>0) return -1;
    else return 0;
}
//31 <N_COM>->{T_LWICOM}{T_RT}{T_COMMA}<N_IMMEDIATE>{T_BRS}
//35 <N_COM>->{T_JBCOM}{T_RT}{T_COMMA}{T_RS}{T_COMMA}<N_IMMEDIATE>
//33 <N_COM>->{T_BCOM}{T_RT}{T_COMMA}{T_RS}{T_COMMA}<N_IMMEDIATE>
//52 <N_COM>->{T_BGICOM}{T_RS}{T_COMMA}<N_ IMMEDIATE >
int assembler::TranslateImmi()
{
    if(CurrentRule==38)
    {
        if(LastRule==31) CurrentRule=LastRule;
        else CurrentRule=24;
    }
    int immi=StrtoDNum(TOKEN.Content);    //字符串转换为十进制数
    char Immi[IMMI_SIZE]={'0'};
    DNumtoBStr(immi,Immi,16);        //十进制数转换为二进制字符串
    for(int i=16;i<32;i++) COM_POS->Com[i]=Immi[i-16];//填入当前指令
    strcat(COM_POS->Content,TOKEN.Content);
    return 0;
}
//翻译Addr
//8  <N_ORG_DATA>->{T_ORG_DATA}{T_ADDR}{T_ENDL}
//21 <N_ORG_CODE> {T_ORG_CODE}{T_ADDR}{T_ENDL}
//32 <N_COM>->{T_JCOM}<N_ADDR>
//40 <N_ADDR>->{T_ADDR}
int assembler::TranslateAddr()
{
    if(CurrentRule==40)  CurrentRule=24;  //修改当前规则

    int addr=StrtoDNum(TOKEN.Content);    //字符串转换为十进制数
    if(LastRule==19&&addr!=0)            //第一条指令的地址不为0
    {
        ErrorID=ERROR_NOTBEGIN_ZERO;
        return -1;
    }
    if(addr%4!=0)  //地址必须是4的倍数，否则出错
    {
        ErrorID=ERROR_NOTFOUR;
        return -1;
    }
    if(addr>=ROM_SIZE*4||addr<0) //地址越界
    {
        ErrorID=ERROR_CADDRESS_OVERFLOW;
        return -1;
    }
    addr/=4;     //除以4，因为给出的是绝对地址
    //上面的情况都没有出现，则地址正常，进行翻译
    switch(CurrentRule)
    {
    case 8:
        CurrentDataAddr=addr-1;    //减1是因为addr表示的是下一个数据的地址
        break;
    case 21:
        CurrentComAddr=addr-1;     //减1是因为addr表示的是下一条指令的地址
        break;
    case 24:
        {
            //填充指令中的地址部分
            char Addr[ADDR_SIZE]={'0'};
            DNumtoBStr(addr,Addr,16);        //十进制数转换为二进制字符串
            for(int i=16;i<32;i++) COM_POS->Com[i]=Addr[i-16];//填入当前指令
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    default:
        break;
    }
    return 0;
}
//翻译变量
int assembler::TranslateID()
{
    int addr;
    char Addr[ADDR_SIZE]={'0'};
    if(LastRule==31)
    {
        CurrentRule=LastRule;	           //回去处理前一条规则
        addr=SearchVar(TOKEN.Content);
        if(addr==-1)    //没找到，则出错
        {
            ErrorID=ERROR_UNDEFINED_VAR;
            return -1;
        }
        DNumtoBStr(addr,Addr,16);
        for(int i=16;i<32;i++) COM_POS->Com[i]=Addr[i-16];
    }
    else
    {
        CurrentRule=24;
        int addr=SearchSegID(TOKEN.Content);     //根据标号查找跳转地址
        if(addr==-1)  { InsertBack();}           //没找到，则需回填
        else
        {
            if(LastRule==33||LastRule==35||LastRule==52)
                addr=addr-CurrentComAddr-1;
            DNumtoBStr(addr,Addr,16);
            for(int i=16;i<32;i++) COM_POS->Com[i]=Addr[i-16];
        }
    }
    strcat(COM_POS->Content,TOKEN.Content);
    return 0;
}
//翻译Rcom(add|addu|and|nor|or|sllv|slt|sltu|srav|srlv|sub|subu|xor|mul)
//000000 Rs Rt Rd 00000 Func
int assembler::TranslateRcom()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_RS:
    case T_RD:
    case T_RT:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg); //寄存器号
            if(TOKEN.ID==T_RS)
            {
                //注意有三条指令的寄存器位置和其他不一样
                if(strcmp(COM_POS->JSegName,"sllv")==0||
                   strcmp(COM_POS->JSegName,"srlv")==0||
                   strcmp(COM_POS->JSegName,"srav")==0)
                {
                    for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];//Rt
                }
                else
                {
                    for(i=6;i<11;i++)  COM_POS->Com[i]=Reg[i-6]; //Rs
                }
            }
            else if(TOKEN.ID==T_RT)
            {
                if(strcmp(COM_POS->JSegName,"sllv")==0||
                   strcmp(COM_POS->JSegName,"srlv")==0||
                   strcmp(COM_POS->JSegName,"srav")==0)
                {
                    for(i=6;i<11;i++)  COM_POS->Com[i]=Reg[i-6]; //Rs
                }
                else
                {
                    for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];//Rt
                }
            }
            else
            {
                for(i=16;i<21;i++) COM_POS->Com[i]=Reg[i-16];//Rd
            }
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_RCOM:
        {
            if(InsertCom(TOKEN.Content,T_RCOM)==-1) return -1;  //插入一条新指令
            char Func[FUNC_SIZE]={'0'};
            SearchFunc(TOKEN.Content,Func);          //根据指令助记符获得Func号
            //指令翻译
            for(i=0;i<6;i++)   COM_POS->Com[i]='0';  //Op，为000000
            if (strcmp(COM_POS->JSegName,"mul")==0)	//mul为特例，Op不为0
            {
                char Op[OP_SIZE]={'0'};
                SearchOp(TOKEN.Content,Op);
                for(i=0;i<6;i++) COM_POS->Com[i]=Op[i];
            }
            for(i=21;i<26;i++) COM_POS->Com[i]='0';  //Shamt，为00000
            for(i=26;i<32;i++) COM_POS->Com[i]=Func[i-26]; //Func
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

//翻译SllRcom(sll|sra|srl)
//000000 00000 Rt Rd Shamt Func
int assembler::TranslateSllRcom()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_RD:
    case T_RT:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            if(TOKEN.ID==T_RT)
            {
                for(i=11;i<16;i++)  COM_POS->Com[i]=Reg[i-11]; //Rt
            }
            else
            {
                for(i=16;i<21;i++)  COM_POS->Com[i]=Reg[i-16]; //Rd
            }
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_SHAMT:
        {
            char Shamt[SHAMT_SIZE]={'0'};
            int shamt=StrtoDNum(TOKEN.Content);
            if(shamt>31) { ErrorID=ERROR_SHAMT_OVERFLOW; return -1; }  //越界
            DNumtoBStr(shamt,Shamt,5);
            for(i=21;i<26;i++) COM_POS->Com[i]=Shamt[i-21];    //Shamt
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_SLLRCOM:
        {
            if(InsertCom(TOKEN.Content,T_SLLRCOM)==-1) return -1;
            char Func[FUNC_SIZE]={'0'};
            SearchFunc(TOKEN.Content,Func);
            for(i=0;i<11;i++)  COM_POS->Com[i]='0';            //Op为000000，Rs为00000
            for(i=26;i<32;i++) COM_POS->Com[i]=Func[i-26];     //Func
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

//翻译Icom(addi|addiu|andi ori|slti|sltiu|xori)
//Op Rs Rt Immediate
int assembler::TranslateIcom()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_RS:
    case T_RT:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            if(TOKEN.ID==T_RS)
            {
                for(i=6;i<11;i++)  COM_POS->Com[i]=Reg[i-6]; //Rs
            }
            else
            {
                for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];//Rt
            }
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_IMMEDIATE:
        {
            char Immi[IMMI_SIZE]={'0'};
            if(StrtoBStr(TOKEN.Content,Immi,16)==-1) { ErrorID=ERROR_IMMEDIATE_OVERFLOW;return -1;}
            for(i=16;i<32;i++)
            {
                COM_POS->Com[i]=Immi[i-16];                 //Immi
            }
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_ICOM:
        {
            if(InsertCom(TOKEN.Content,T_ICOM)==-1) return -1;
            char Op[OP_SIZE]={'0'};
            SearchOp(TOKEN.Content,Op);
            for(i=0;i<6;i++) COM_POS->Com[i]=Op[i];            //Op
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

//翻译Srcom(jr|mthi|mtlo)
//000000 Rs 00000 00000 00000 001000
int assembler::TranslateSrcom()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_SRCOM:
        {
            if(InsertCom(TOKEN.Content,T_SRCOM)==-1) return -1;

            char Func[FUNC_SIZE]={'0'};
            SearchFunc(TOKEN.Content,Func);
            for(i=0;i<6;i++)   COM_POS->Com[i]='0';            //Op
            for(i=11;i<26;i++) COM_POS->Com[i]='0';	           //Rt Rd Shamt
            for(i=26;i<32;i++) COM_POS->Com[i]=Func[i-26];     //Func
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    case T_RS:
        {
            char Reg[REG_SIZE]={'0'};                      //Reg
            SearchReg(TOKEN.Content,Reg);
            for(i=6;i<11;i++) COM_POS->Com[i]=Reg[i-6];    //Rs
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

//翻译Sicom(lui)
//Op 00000 Rt Immi
int assembler::TranslateSicom()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_SICOM:
        {
            if(InsertCom(TOKEN.Content,T_SICOM)==-1) return -1;
            char Op[OP_SIZE]={'0'};
            SearchOp(TOKEN.Content,Op);
            for(i=0;i<6;i++)   COM_POS->Com[i]=Op[i];          //Op
            for(i=6;i<11;i++)  COM_POS->Com[i]='0';            //Rs
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    case T_IMMEDIATE:
        {
            char Immi[IMMI_SIZE]={'0'};
            if(StrtoBStr(TOKEN.Content,Immi,16)==-1) { ErrorID=ERROR_IMMEDIATE_OVERFLOW;return -1;}
            for(i=16;i<32;i++) COM_POS->Com[i]=Immi[i-16];     //Immi
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_RT:
        {
            char Reg[REG_SIZE]={'0'};                      //Reg
            SearchReg(TOKEN.Content,Reg);
            for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];  //Rt
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

//翻译Lwicom(lw|sw|lb|lbu|lh|lhu|sb|sh)
//Op Rs Rt Addr
int assembler::TranslateLwicom()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_LWICOM:
        {
            if(InsertCom(TOKEN.Content,T_LWICOM)==-1) return -1;
            char Op[OP_SIZE]={'0'};
            SearchOp(TOKEN.Content,Op);
            for(i=0;i<6;i++) COM_POS->Com[i]=Op[i];            //Op
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    case T_BRS:
    case T_RT:
        {
            char Reg[REG_SIZE]={'0'};
            if(TOKEN.ID==T_BRS)
            {
                for(i=0;TOKEN.Content[i]!=')';i++) TOKEN.Content[i]=TOKEN.Content[i+1]; //去括号
                TOKEN.Content[i-1]=TOKEN.Content[i]='\0';
                SearchReg(TOKEN.Content,Reg);                  //查寄存器表
                for(i=6;i<11;i++) COM_POS->Com[i]=Reg[i-6];	   //Rs
                strcat(COM_POS->Content,"(");
                strcat(COM_POS->Content,TOKEN.Content);
                strcat(COM_POS->Content,")");
            }
            else
            {
                SearchReg(TOKEN.Content,Reg);
                for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];  //Rt
                strcat(COM_POS->Content,TOKEN.Content);
            }
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

//翻译Jcom(j|jal)
//Op 00000 00000 Addr
int assembler::TranslateJcom()
{
    if(InsertCom(TOKEN.Content,T_JCOM)==-1) return -1;
    char Op[OP_SIZE]={'0'};
    SearchOp(TOKEN.Content,Op);
    for(int i=0;i<6;i++)  COM_POS->Com[i]=Op[i];               //Op
    for(int j=6;j<16;j++) COM_POS->Com[j]='0';
    if(TOKEN.ID!=T_ENDL)
    {
        strcpy(COM_POS->Content,TOKEN.Content);
        strcat(COM_POS->Content," ");
    }
    return 0;
}

//翻译Bcom(bne|beq)
//Op Rs Rt Addr
int assembler::TranslateBcom()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_BICOM:
        {
            if(InsertCom(TOKEN.Content,T_BICOM)==-1) return -1;
            char Op[OP_SIZE]={'0'};
            SearchOp(TOKEN.Content,Op);
            for(i=0;i<6;i++) COM_POS->Com[i]=Op[i];
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    case T_RS:
    case T_RT:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            if(TOKEN.ID==T_RS)
            {
                for(i=6;i<11;i++) COM_POS->Com[i]=Reg[i-6];
            }
            else
            {
                for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];
            }
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL) strcat(COM_POS->Content,TOKEN.Content);
        break;
    }

    return 0;
}

//翻译Pcom(push|pop)  主要是将一条指令翻译两条指令
int assembler::TranslatePcom()
{
    //扩展指令
    int i=0;
    char com[COM_SIZE]={'0'};
    switch(TOKEN.ID)
    {
    case T_PCOM:
        {
            Warning(WARNING_EXTEND);
            if(strcmp(TOKEN.Content,"push")==0){
                strcpy(com,"10101111101000000000000000000000");
                if(InsertCom("sw",T_LWICOM)==-1) return -1;    //插入sw指令
                for(i=0;i<32;i++) COM_POS->Com[i]=com[i];
                strcpy(COM_POS->Content,"sw ");
            }
            else{
                strcpy(com,"00100011101111011111111111111100"); //32位指令码
                if(InsertCom("addi",T_ICOM)==-1) return -1;     //插入addi $sp,$sp,-4
                strcpy(COM_POS->Content,"addi $sp,$sp,-4");
                for(i=0;i<32;i++) COM_POS->Com[i]=com[i];
                //处理下一条指令
                strcpy(com,"10001111101000000000000000000000");
                if(InsertCom("lw",T_LWICOM)==-1) return -1;
                for(i=0;i<32;i++) COM_POS->Com[i]=com[i];
                strcpy(COM_POS->Content,"lw ");
            }
            break;
        }
    case T_RS:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];
            strcat(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content,",0($sp)");
            //处理下一条指令
            if(strcmp(COM_POS->JSegName,"sw")==0)
            {
                strcpy(com,"00100011101111010000000000000100"); //32位指令码
                if(InsertCom("addi",T_ICOM)==-1) return -1;     //插入addi $sp,$sp,4
                for(i=0;i<32;i++)  COM_POS->Com[i]=com[i];
                strcpy(COM_POS->Content,"addi $sp,$sp,4");
            }
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL) strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}
//翻译Jbcom(jl|jle|jg|jge)  主要是将一条指令翻译三条指令
//Op Rs Rt Addr
int assembler::TranslateJbcom()
{
    int i=0;
    char com[COM_SIZE]={'0'};
    static char Temp[COMMOM_SIZE]={'0'};
    switch(TOKEN.ID)
    {
    case T_JBCOM:
        {
            //sub
            Warning(WARNING_EXTEND);
            strcpy(com,"00000000000000000000100000100010");
            if(InsertCom("sub",T_RCOM)==-1) return -1;
            for(i=0;i<32;i++) COM_POS->Com[i]=com[i];
            strcpy(COM_POS->Content,"sub $1,");
            strcpy(Temp,TOKEN.Content);
            break;
        }
    case T_RS:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];
            strcat(COM_POS->Content,TOKEN.Content);
            //sub结束
            if(strcmp(Temp,"jg")==0||strcmp(Temp,"jle")==0)
            {
                //slt $1,$0,$1 ; $1=($0<$1)
                strcpy(com,"00000000000000010000100000101010");
                if(InsertCom("slt",T_RCOM)==-1) return -1;
                for(i=0;i<32;i++)  COM_POS->Com[i]=com[i];
                strcpy(COM_POS->Content,"slt $1,$0,$1");

                if(strcmp(Temp,"jg")==0)
                {
                    //bne $1,$0,0
                    strcpy(com,"00010100000000010000000000000000");
                    if(InsertCom("bne",T_BICOM)==-1) return -1;
                    strcpy(COM_POS->Content,"bne $1,$0");
                }
                else
                {
                    //beq $1,$0,0
                    strcpy(com,"00010000000000010000000000000000");
                    if(InsertCom("beq",T_BICOM)==-1) return -1;
                    strcpy(COM_POS->Content,"beq $1,$0");
                }
                for(i=0;i<32;i++)  COM_POS->Com[i]=com[i];
            }
            if(strcmp(Temp,"jl")==0||strcmp(Temp,"jge")==0)
            {
                //slt $1,$1,$0;$1 =($1<$0)
                strcpy(com,"00000000001000000000100000101010");
                if(InsertCom("slt",T_RCOM)==-1) return -1;
                for(i=0;i<32;i++)  COM_POS->Com[i]=com[i];
                strcpy(COM_POS->Content,"slt $1,$1,$0");
                if(strcmp(Temp,"jl")==0)
                {
                    //bne $1,$0,0
                    strcpy(com,"00010100000000010000000000000000");
                    if(InsertCom("bne",T_BICOM)==-1) return -1;
                    strcpy(COM_POS->Content,"bne $1,$0");
                }
                else
                {
                    //beq $1,$0,0
                    strcpy(com,"00010000000000010000000000000000");
                    if(InsertCom("beq",T_BICOM)==-1) return -1;
                    strcpy(COM_POS->Content,"beq $1,$0");
                }
                for(i=0;i<32;i++)  COM_POS->Com[i]=com[i];
            }
            break;
        }
    case T_RT:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            for(i=6;i<11;i++) COM_POS->Com[i]=Reg[i-6];
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL) strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}
//翻译Nop
int assembler::TranslateNop()
{
    CurrentRule=24;     //恢复规则
    if(InsertCom("nop",T_NOP)==0)
    {
        for(int i=0;i<32;i++) COM_POS->Com[i]='0';
        strcpy(COM_POS->Content,"nop");
        return 0;
    }
    return -1;
}

//翻译MRCOM(mfhi|mflo)
//Op 0000000000 Rd 00000 Func
int assembler::TranslateMRCOM()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_RD:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            for(i=16;i<21;i++)  COM_POS->Com[i]=Reg[i-16]; //Rd
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_MRCOM:
        {
            if(InsertCom(TOKEN.Content,T_MRCOM)==-1) return -1;
            for(i=0;i<6;i++) COM_POS->Com[i]='0';
            for(i=6;i<16;i++) COM_POS->Com[i]='0';//Rt,Rs为0
            for(i=21;i<26;i++) COM_POS->Com[i]='0';
            char Func[FUNC_SIZE]={'0'};
            SearchFunc(TOKEN.Content,Func);
            for(i=26;i<32;i++) COM_POS->Com[i]=Func[i-26];     //Func
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

int assembler::TranslateSel()
{
    if (NeedSel == 1)
    {
        int i=0;
        if (TOKEN.ID==T_SEL)
        {
            char Sel[SEL_SIZE]={'0'};
            if(StrtoBStr(TOKEN.Content,Sel,3)==-1) {ErrorID=ERROR_SEL_OVERFLOW;return -1;}
            for(i=29;i<32;i++) COM_POS->Com[i]=Sel[i-29];  //Sel
            strcat(COM_POS->Content,TOKEN.Content);
        }
        else
            strcat(COM_POS->Content,TOKEN.Content);

        return 0;
    }
    else
    {
        if (TOKEN.ID==T_COMMA)
        {
            //在逗号时报错
            ErrorID = ERROR_SEL;
            Error(ERROR_SEMANTIC);
        }

    }
    return 0;
}
//翻译MFCRCOM(mfc0|mtc0)
//Op Func Rt Rd 00000000 Sel
int assembler::TranslateMFCRCOM()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_RD:
        {
            //rd是cp0寄存器
            char Reg[REG_SIZE]={'0'};
            char Sel[SEL_SIZE]={'0'};
            int ns = SearchCP0Reg(TOKEN.Content,Reg,Sel);
            for(i=16;i<21;i++) COM_POS->Com[i]=Reg[i-16];
            if (ns == 0)
            {
                NeedSel = 0;
                for(i=29;i<32;i++) COM_POS->Com[i]=Sel[i-29];  //Sel
            }
            else
                NeedSel = 1;

            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_RT:
        {
            //rt是cpu寄存器
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_MFCRCOM:
        {
            if(InsertCom(TOKEN.Content,T_MFCRCOM)==-1) return -1;
            char Op[OP_SIZE]={'0'};        //Op不为000000
            SearchOp(TOKEN.Content,Op);
            for(i=0;i<6;i++) COM_POS->Com[i]=Op[i];
            char Func[FUNC_SIZE]={'0'};
            SearchFunc(TOKEN.Content,Func);
            for(i=6;i<11;i++) COM_POS->Com[i]=Func[i-5];     //Func
            for(i=21;i<29;i++) COM_POS->Com[i]='0';
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

//翻译MULRCOM(mult|multu|div|divu)
//000000 rs rt 0000000000 Func
int assembler::TranslateMULRCOM()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_RS:
    case T_RT:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            if(TOKEN.ID==T_RT)
            {
                for(i=11;i<16;i++) COM_POS->Com[i]=Reg[i-11];
            }
            else
            {
                for(i=6;i<11;i++) COM_POS->Com[i]=Reg[i-6];
            }
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_MULRCOM:
        {
            if(InsertCom(TOKEN.Content,T_MULRCOM)==-1) return -1;
            for(i=0;i<6;i++) COM_POS->Com[i]='0';  //Op为000000
            for(i=16;i<26;i++) COM_POS->Com[i]='0';
            char Func[FUNC_SIZE]={'0'};
            SearchFunc(TOKEN.Content,Func);
            for(i=26;i<32;i++) COM_POS->Com[i]=Func[i-26];     //Func
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

//翻译SYSRCOM(syscall|break|eret)
//000000 Code Func
int assembler::TranslateSYSRCOM()
{
    int i=0;

    if (TOKEN.ID==T_SYSRCOM)
    {
        if(InsertCom(TOKEN.Content,T_SYSRCOM)==-1) return -1;
        for(i=0;i<6;i++) COM_POS->Com[i]='0';  //Op为0
        if (strcmp(COM_POS->JSegName,"eret")==0)
        {
            for(i=6;i<26;i++) COM_POS->Com[i]='0'; //Code 为0
        }
        else
        {
            for(i=6;i<26;i++)
            {
                COM_POS->Com[i]=CODE[i-6];
            }
        }

        char Func[FUNC_SIZE]={'0'};
        SearchFunc(TOKEN.Content,Func);
        for(i=26;i<32;i++) COM_POS->Com[i]=Func[i-26];     //Func
        strcpy(COM_POS->Content,TOKEN.Content);
        strcat(COM_POS->Content," ");

    }
    else
    {
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
    }
    //for (i=0;i<32;i++) cout<<COM_POS->Com[i];
    //		cout<<endl;
    return 0;
}

//翻译JRCOM(jalr)
//000000 Rs 00000 Rd 00000 Func
int assembler::TranslateJRCOM()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_RD:
    case T_RS:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            if(TOKEN.ID==T_RD)
            {
                for(i=16;i<21;i++) COM_POS->Com[i]=Reg[i-16];
            }
            else
            {
                for(i=6;i<11;i++) COM_POS->Com[i]=Reg[i-6];
            }
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_JRCOM:
        {
            if(InsertCom(TOKEN.Content,T_JRCOM)==-1) return -1;
            for(i=0;i<6;i++) COM_POS->Com[i]='0';
            for(i=11;i<16;i++) COM_POS->Com[i]='0';//Rt为0
            for(i=21;i<26;i++) COM_POS->Com[i]='0';//00000
            char Func[FUNC_SIZE]={'0'};
            SearchFunc(TOKEN.Content,Func);
            for(i=26;i<32;i++) COM_POS->Com[i]=Func[i-26];     //Func
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}

//翻译BGICOM(bgez|bgezal|bltzal|bgtz|blez|bltz)
//Op Rs 区别码 Offset
int assembler::TranslateBGICOM()
{
    int i=0;
    switch(TOKEN.ID)
    {
    case T_RS:
        {
            char Reg[REG_SIZE]={'0'};
            SearchReg(TOKEN.Content,Reg);
            for(i=6;i<11;i++)  COM_POS->Com[i]=Reg[i-6]; //Rs
            strcat(COM_POS->Content,TOKEN.Content);
            break;
        }
    case T_BGICOM:
        {
            if(InsertCom(TOKEN.Content,T_BGICOM)==-1) return -1;
            char Op[OP_SIZE]={'0'};        //Op不为000000
            SearchOp(TOKEN.Content,Op);
            for(i=0;i<6;i++) COM_POS->Com[i]=Op[i];
            char Func[FUNC_SIZE]={'0'};
            SearchFunc(TOKEN.Content,Func);
            for(i=11;i<16;i++) COM_POS->Com[i]=Func[i-10];     //区别码，存于func中
            strcpy(COM_POS->Content,TOKEN.Content);
            strcat(COM_POS->Content," ");
            break;
        }
    default:
        if(TOKEN.ID!=T_ENDL)
            strcat(COM_POS->Content,TOKEN.Content);
        break;
    }
    return 0;
}
//翻译器结束

//警告处理程序
int assembler::Warning(int TypeOfWarning)
{
    NumOfWarning++;
    switch(TypeOfWarning)
    {
    case WARNING_EXTEND:
        cout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        cout<<"\' : warning : one extended instruction is two instructions\n";
        break;
    case WARNING_CP0_REG:
        cout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        cout<<"\' : warning : should not use a CPU register to represent CP0 register\n";
        break;

    default:
        break;
    }
    return 0;
}
//错误处理程序
int assembler::Error(int TypeOfError)
{
    NumOfError++;         //错误计数
    switch(TypeOfError)
    {
    case ERROR_LEXICAL:   //词法错误
        return ErrorLexical();
    case ERROR_SEMANTIC:  //语义错误
        return ErrorSemantic();
    default:
        return -1;        //程序终止
    }
}
//词法错误处理
int assembler::ErrorLexical()
{
    switch(TOKEN.ID)
    {
    case T_CODE:
        {
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing  keyword \'code\'\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : misspelling keyword \'code\'\n";
            }
            return 0;
        }
    case T_ORG_CODE:
        {
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing  keyword \'org_code\'\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : misspelling keyword \'org_code\'\n";
            }
            return 0;
        }
    case T_DATA:
        {
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing  keyword \'data\'\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : misspelling keyword \'data\'\n";
            }
            return 0;
        }
    case T_ORG_DATA:
        {
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing  keyword \'org_data\'\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : misspelling keyword \'org_data\'\n";
            }
            return 0;
        }
    case T_END:
        {
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing  keyword \'end\'\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : misspelling keyword \'end\'\n";
            }
            return 0;
        }
    case T_DW:
        {
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing  keyword \'.word\'\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : misspelling keyword \'.word\'\n";
            }
            return 0;
        }
    case T_SEG:
        {
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing  keyword \'seg\'\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : misspelling keyword \'seg\'\n";
            }
            return 0;
        }
    case T_ENDS:
        {
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing  keyword \'ends\'\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : misspelling keyword \'ends\'\n";
            }
            return 0;
        }
    case T_ENDL:
        {
            efout<<"line "<<NumOfLine<<" : syntax error : missing \';\' at the end of sentence\n";
            ReadErrorSentence(); //读去一个错误句子
            return 0;
        }
    case T_IDNAME:
        {
            ReadErrorToken();    //读去一个错误单词
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing  a identifier\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : wrong identifier\n";
            }
            return 0;
        }
    case T_COMMA:
        {
            efout<<"line "<<NumOfLine<<" : syntax error : missing \',\'\n";
            return 0;
        }
    case T_COLON:
        {
            efout<<"line "<<NumOfLine<<" : syntax error : missing \':\'\n";
            ReadErrorSentence();  //读去一个句子
            return 0;
        }
    case T_NUM:
        {
            ReadErrorToken();     //读完整错误单词
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : syntax error : missing a num\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : wrong num\n";
            }
            return 0;
        }
    case T_SHAMT:
        {
            ReadErrorToken();    //读完整错误单词
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : syntax error : missing a shamt\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : wrong shamt\n";
            }
            return 0;
        }
    case T_ADDR:
        {
            ReadErrorToken();   //读完整错误单词
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : syntax error : missing a address\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : wrong address\n";
            }
            return 0;
        }
    case T_IMMEDIATE:
        {
            ReadErrorToken();  //读完整错误单词
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : syntax error : missing a immediate\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : wrong immiediate\n";
            }
            return 0;
        }
    case T_RS:
    case T_RT:
    case T_RD:
    case T_BRS:
        {
            ReadErrorToken(); //读完整错误单词
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : syntax error : missing a reg\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : undefined register\n";
            }
            return 0;
        }

    case ERROR_NVARS: //滤过一个单词，认为已经识别出T_IDNAME
        {
            ReadErrorToken(); //读完整错误单词
            if(IsKeyWords(TOKEN.Content))
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : keyword can\'t be identifier\n";
            }
            else if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : missing identifier\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : wrong identifier\n";
            }
            TOKEN.ID=T_IDNAME;
            return 0;
        }
    case ERROR_NVARA: //滤过一个句子，认为已经识别出T_ENDL
        {
            efout<<"line "<<NumOfLine<<" : syntax error : missing \',\' or missing \';\'\n";
            ReadErrorSentence();
            TOKEN.ID=T_ENDL;
            return 0;
        }
    case ERROR_NCODEA:
        {
            if(IsKeyWords(TOKEN.Content))
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : keyword can\'t be identifier\n";
            }
            else if(strcmp(TOKEN.Content,""))
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : missing identifier\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : wrong identifier\n";
            }
            TOKEN.ID=T_IDNAME;
            return 0;
        }
    case ERROR_NORDER:
    case ERROR_NCOM:  //滤过一个句子，认为已经识别出T_ENDL
        {
            ReadErrorToken();
            while(STACK.Depth[STACK.Top]!=N_ORDERS){
                STACK.Depth[STACK.Top--]=0;
            }
            TOKEN.ID=STACK.Depth[++STACK.Top]=T_ENDL;
            efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : wrong instruction\n";
            ReadErrorSentence();
            return 0;
        }
    case ERROR_NORDERS: //滤过一个句子，认为已经识别出T_ENDL
        {
            ReadErrorToken();
            TOKEN.ID=STACK.Depth[++STACK.Top]=T_ENDL;
            efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : wrong instruction\n";
            ReadErrorSentence();
            return 0;
        }
    case ERROR_NIMMEDIATE://滤过一个句子，认为已经识别出T_ENDL
        {
            ReadErrorToken();
            while(STACK.Depth[STACK.Top]!=T_ENDL){
                STACK.Depth[STACK.Top--]=0;
            }
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing a immiediate or identifier\n";
            }
            else if(IsKeyWords(TOKEN.Content))
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : keyword can\'t be identifier\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : wrong immiediate or identifier\n";
            }
            ReadErrorSentence();
            TOKEN.ID=T_ENDL;
            return 0;
        }
    case ERROR_NADDR: //滤过一个句子，认为已经识别出T_ENDL
        {
            ReadErrorToken();
            while(STACK.Depth[STACK.Top]!=T_ENDL){
                STACK.Depth[STACK.Top--]=0;
            }
            if(strcmp(TOKEN.Content,"")==0)
            {
                efout<<"line "<<NumOfLine<<" : lexical error : missing a address or identifier\n";
            }
            else if(IsKeyWords(TOKEN.Content))
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\': lexical error : keyword can\'t be identifier\n";
            }
            else
            {
                efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content<<"\' : lexical error : wrong address\n";
            }
            ReadErrorSentence();
            TOKEN.ID=T_ENDL;
            return 0;
        }
    case T_FILEEND:
        {
            efout<<"line "<<NumOfLine<<" : program has not been completed\n";
            return -1;
        }
    case ERROR_NPRO:      //第一个单词就出错，终止程序
        {
            efout<<"line "<<NumOfLine<<" : program doesn\'t begin with \'data\' or \'code\'\n";
            return -1;
        }
    case ERROR_TERMINAL:  //无法解决的错误
        {
            efout<<"line "<<NumOfLine<<" : syntax error : wrong data seg \n";
            return -1;
        }
    default:
        efout<<"词法错误之不可能的情况"<<endl;//不可能的情况
        return -1;
    }
}
//处理语义错误
int assembler::ErrorSemantic()
{
    switch(ErrorID)
    {
    case ERROR_NOTFOUR:			    //地址不能被四整除
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' semantic error : address can not be divisible by 4\n";
        return 0;
    case ERROR_DADDRESS_OVERFLOW:   //数据地址越界
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : address(data seg)  is overflow,not between 0 and ";
        efout<<(RAM_SIZE-1)*4<<endl;
        return 0;
    case ERROR_CADDRESS_OVERFLOW:   //代码地址越界
        efout<<"line "<<NumOfLine<<" : instruction \'"<<TOKEN.Content;
        efout<<"\' : semantic error : address(code seg) is overflow,not between 0 and ";
        efout<<(ROM_SIZE-1)*4<<endl;
        return 0;
    case ERROR_IMMEDIATE_OVERFLOW:  //立即数越界
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : immiediate is overflow, not between -32768 and 32767\n";
        return 0;
    case ERROR_DATA_OVERFLOW:       //数据段定义的数据越界
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : data is overflow, not between -2147483646 and 2147483646\n";
        return 0;
    case ERROR_SHAMT_OVERFLOW:      //移位位数越界
        efout<<"line "<<NumOfLine<<" \'"<<TOKEN.Content;
        efout<<"\' : semantic error : shamt is overflow, not between 0 and 31\n";
        return 0;
    case ERROR_UNDEFINED_LABEL:     //未定义的标号
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : undefined label\n";
        return 0;
    case ERROR_NOTFIRST_LABEL:      //END后面跟的不是第一个标号
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : not the first label\n";
        return 0;
    case ERROR_MISSING_LABEL:       //缺少标号
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : not existed label\n";
        return 0;
    case ERROR_UNDEFINED_VAR:       //未定义的变量
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : undefined variable\n";
        return 0;
    case ERROR_NOTBEGIN_ZERO:       //代码段没从0开始
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : code seg doesn\'t begin from 0000h\n";
        return 0;
    case ERROR_DADDRESS_CONFLICT:   //数据地址冲突
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : address conflict\n";
        return 0;
    case ERROR_VAR_REDEFINITION:    //变量重定义
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : variable redefinition\n";
        return 0;
    case ERROR_SEGID_REDEFINITION:  //标号重定义
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : label redefinition\n";
        return 0;
    case ERROR_CADDRESS_CONFLICT:   //指令地址冲突
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : address conflict\n";
        return -1;                  //终止程序
    case ERROR_SEL_OVERFLOW:  //立即数越界
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : sel is overflow, not between -8 and 7\n";
        return 0;
    case ERROR_CP0_REG:  //CP0寄存器误用
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : register can only be used for CP0\n";
        return 0;
    case ERROR_SEL:  //此时不该出现sel字段
        efout<<"line "<<NumOfLine<<" : \'"<<TOKEN.Content;
        efout<<"\' : semantic error : should't have a sel when CP0 register is used\n";
        NeedSel = 0;
        return 0;
    default:                        //不可能的情况
        efout<<"语义错误之不可能的情况"<<endl;
        return -1;
    }
}
//读去一个错误单词
int assembler::ReadErrorToken()
{
    char ch;
    while(1)
    {
        ch=ReadAChar();
        TOKEN.Content[++TOKEN.Idx]=ch;
        switch(ch)
        {
        case ' ':
        case '\t':
        case '\n':
        case ';':
        case ',':
        case ':':
        case '\0':
            WriteAChar(ch);                 //退回一个字符
            TOKEN.Content[TOKEN.Idx--]='\0';//最后一个字符不要
            return 0;
        default:
            break;
        }
    }
}
//读去一个错误句子,包括多行注释
int assembler::ReadErrorSentence()
{
    char ch;
    int State=0;
    while(1)
    {
        ch=ReadAChar();
        switch(ch)
        {
        case '\n':
            State=1;break;
        case ';':
            State=2;break;
        case ' ':
        case '\t':
            break;
        case '\0':
            TOKEN.ID=T_FILEEND;
            return 0;
        default:
            if(State==1) { WriteAChar(ch); return 0; }
            else break;
        }
    }
}
//错误处理程序结束

//文件读写函数
int assembler::WriteData()
{
    //dfout.open("Ram.mif");
    dfout.open(this->ramFile.c_str());
    dfout<<hex;
    dfout<<"MEMORY_INITIALIZATION_RADIX=2;\n";
    dfout<<"MEMORY_INITIALIZATION_VECTOR=\n";
    //地址使用情况记录
    bool IsUse[RAM_SIZE]={false};
    char *Data[RAM_SIZE]={NULL};
    for(My_Data* pData=DATAS_TABLE;pData!=NULL;pData=pData->Next)
    {
        IsUse[pData->Addr]=true;       //标记已经使用
        Data[pData->Addr]=new char[DATA_SIZE];
        strcpy(Data[pData->Addr],pData->Value);
    }
    //开始输出
    for(int i=0;i<RAM_SIZE;i++)            //然后从地址0开始输出数据
    {
        if(IsUse[i])
        {
            Write32Bit(i,Data[i],dfout);
            if(i != RAM_SIZE-1)
                dfout<<",\n";
            else
                dfout<<";\n";
        }
        else
        {
//            int j=i++;
//            while(i<RAM_SIZE&&!IsUse[i]) i++;
//            i--;
//            if(j==i)
//            {
//                dfout<<"\t"<<setw(3)<<setfill('0')<<j<<" : 00000000;\n";
//            }
//            else
//            {
//                dfout<<"\t["<<setw(3)<<setfill('0')<<j<<"..";
//                dfout<<setw(3)<<setfill('0')<<i<<"] : 00000000;\n";
//            }
            if(i != RAM_SIZE-1)
                dfout<<"00000000000000000000000000000000,\n";
            else
                dfout<<"00000000000000000000000000000000;\n";

        }
    }
    //dfout<<"END;\n"<<dec;
    dfout.close();
    //释放空间
    for(int x=0;x<RAM_SIZE;x++)
    {
        if(Data[x]!=NULL)
        {
            delete Data[x];
            Data[x]=NULL;
        }
    }
    return 0;
}
int assembler::WriteProgram()
{
    //cfout.open("Rom.mif");
    cfout.open(this->romFile.c_str());
//    cfout<<hex;             //以16进制输出
//    cfout<<"MEMORY_INITIALIZATION_RADIX=16;\n";
    cfout<<"MEMORY_INITIALIZATION_RADIX=2;\n";
    cfout<<"MEMORY_INITIALIZATION_VECTOR=\n";
    bool IsUse[ROM_SIZE]={false}; //指令地址使用情况记录
    My_Com *Com[ROM_SIZE]={NULL};
    for(My_Com* pCom=COMS_TABLE;pCom!=NULL;pCom=pCom->Next) //记录地址使用情况
    {
        IsUse[pCom->Addr]=true;
        Com[pCom->Addr]=new My_Com;
        Com[pCom->Addr]=pCom;
    }
    for(int i=0;i<ROM_SIZE;i++)               //输出指令
    {
        if(IsUse[i])      //地址被使用
        {
            Write32Bit(i,Com[i]->Com,cfout);  //输出指令码
            if(i != RAM_SIZE-1)
                cfout<<",\n";
            else
                cfout<<";\n";
//            switch(NeedExplain)
//            {
//            case 0:
//                cfout<<endl;
//                break;
//            case 1:
//                cfout<<"   -- "<<Com[i]->Content<<endl;
//                break;
//            default:
//                if(Com[i]->TypeOfCom!=T_NOP) WriteExplain(Com[i]);
//                else cfout<<"   -- nop\n";
//            }
        }
        else             //未被使用
        {
//			int j=i++;
//			while(i<ROM_SIZE&&!IsUse[i]) i++;
//			i--;
//			if(j==i)
//			{
//				cfout<<"\t"<<setw(3)<<setfill('0')<<j<<" : 00000000;\n";
//			}
//			else
//			{
//				cfout<<"\t["<<setw(3)<<setfill('0')<<j<<"..";
//				cfout<<setw(3)<<setfill('0')<<i<<"] : 00000000;\n";
//			}
            if(i!=ROM_SIZE-1)
                cfout<<"00000000000000000000000000000000,\n";
            else
                cfout<<"00000000000000000000000000000000;\n";

        }
    }
//	cfout<<"END;\n";
    cfout.close();
    //释放空间
    for(int x=0;x<ROM_SIZE;x++)
    {
        if(Com[x]!=NULL)
        {
            delete Com[x];
            Com[x]=NULL;
        }
    }
    return 0;
}
//输出32位数据
void assembler::Write32Bit(int addr,char Line[COMMOM_SIZE],ofstream &out)
{
    char High[17]={'\0'};
    char Low[17]={'\0'};
    for(int i=0;i<16;i++)
    {
        High[i]=Line[i];
        Low[i]=Line[i+16];
    }
//    out<<"\t"<<setw(3)<<setfill('0')<<addr<<" : ";
//    out<<setw(4)<<setfill('0')<<strtol(High,NULL,2);
//    out<<setw(4)<<setfill('0')<<strtol(Low,NULL,2);
    out<<bitset<sizeof(int)*4>(strtol(High,NULL,2));
    out<<bitset<sizeof(int)*4>(strtol(Low,NULL,2));
}
//获得指令注释
void assembler::WriteExplain(My_Com* pCom)
{
    cfout<<dec;      //数据默认以10进制输出
    cfout<<"   -- "<<pCom->Content<<"   \t";
    int i=0;
    for(i=0;i<6;i++) cfout<<pCom->Com[i];
    cfout<<'('<<pCom->JSegName<<")\t";
    switch(pCom->TypeOfCom) //各类指令输出格式有所差异
    {
    case T_RCOM:
        {
            for(i=6;i<11;i++)  cfout<<pCom->Com[i];
            cfout<<"(rs) ";
            for(i=11;i<16;i++) cfout<<pCom->Com[i];
            cfout<<"(rt) ";
            for(i=16;i<21;i++) cfout<<pCom->Com[i];
            cfout<<"(rd) ";
            for(i=21;i<26;i++) cfout<<pCom->Com[i];
            cfout<<" ";
            for(i=26;i<32;i++) cfout<<pCom->Com[i];
            cfout<<"(func)";
            break;
        }
    case T_SRCOM:
        {
            for(i=6;i<11;i++)  cfout<<pCom->Com[i];
            cfout<<"(rs) ";
            for(i=11;i<16;i++) cfout<<pCom->Com[i];
            cfout<<"     ";
            for(i=16;i<21;i++) cfout<<pCom->Com[i];
            cfout<<" ";
            for(i=21;i<26;i++) cfout<<pCom->Com[i];
            cfout<<" ";
            for(i=26;i<32;i++) cfout<<pCom->Com[i];
            cfout<<"(func)";
            break;
        }
    case T_SLLRCOM:
        {
            for(i=6;i<11;i++) cfout<<pCom->Com[i];
            cfout<<"     ";
            for(i=11;i<16;i++) cfout<<pCom->Com[i];
            cfout<<"(rt) ";
            for(i=16;i<21;i++) cfout<<pCom->Com[i];
            cfout<<"(rd) ";
            char Shamt[SHAMT_SIZE]={'\0'};
            for(i=21;i<26;i++)
            {
                cfout<<pCom->Com[i];
                Shamt[i-21]=pCom->Com[i];
            }
            cfout<<"(shamt "<<strtol(Shamt,NULL,2)<<") "; //输出移位值
            for(i=26;i<32;i++) 	cfout<<pCom->Com[i];
            cfout<<"(func)";
            break;
        }
    case T_ICOM:
    case T_BICOM:
    case T_LWICOM:
        {
            for(i=6;i<11;i++) cfout<<pCom->Com[i];
            cfout<<"(rs) ";
            for(i=11;i<16;i++) cfout<<pCom->Com[i];
            cfout<<"(rt) ";
            char Immi[IMMI_SIZE]={'\0'};
            for(i=16;i<32;i++)
            {
                cfout<<pCom->Com[i];
                Immi[i-16]=pCom->Com[i];
            }
            int immi=strtol(Immi,NULL,2);
            if(Immi[0]=='1'){       //负数的情况
                immi=65535-immi+1;
                cfout<<"(immi -"<<immi<<") ";
            }
            else{
                cfout<<"(immi "<<immi<<") ";
            }
            break;
        }
    case T_SICOM:
        {
            for(i=6;i<11;i++) cfout<<pCom->Com[i];
            cfout<<"     ";
            for(i=11;i<16;i++) cfout<<pCom->Com[i];
            cfout<<"(rt) ";
            char Immi[IMMI_SIZE]={'\0'};
            for(i=16;i<32;i++)
            {
                cfout<<pCom->Com[i];
                Immi[i-16]=pCom->Com[i];
            }
            int immi=strtol(Immi,NULL,2);
            if(Immi[0]=='1'){       //负数的情况
                immi=65535-immi+1;
                cfout<<"(immi -"<<immi<<") ";
            }
            else{
                cfout<<"(immi "<<immi<<") ";
            }
            break;
        }
    case T_JCOM:
        {
            for(i=6;i<11;i++) cfout<<pCom->Com[i];
            cfout<<"     ";
            for(i=11;i<16;i++) cfout<<pCom->Com[i];
            cfout<<"     ";
            char Addr[ADDR_SIZE]={'\0'};
            for(i=16;i<32;i++)
            {
                cfout<<pCom->Com[i];
                Addr[i-16]=pCom->Com[i];
            }
            cfout<<"(address "<<hex<<setw(3)<<setfill('0');
            cfout<<strtol(Addr,NULL,2)<<"h) "<<dec; //注意恢复进制
            break;
        }
    case T_NOP:
        {
            for(i=6;i<32;i++) cfout<<pCom->Com[i];
            break;
        }
    default:
        break;
    }
    cfout<<endl<<hex;  //恢复16进制
}
//指令使用情况统计
int assembler::WriteCount()
{
    int count[COM_TABLE_SIZE]={0};
    cout<<"进入循环"<<endl;
    for(My_Com *p=COMS_TABLE;p!=NULL;p=p->Next)
    {
        cout<<p->Addr<<" "<<p->JSegName<<" "<<p->Content<<" "<<p->TypeOfCom<<" "<<p->NumOfCom<<" "<<p->Com<<endl;
        count[p->NumOfCom]++;
    }
    //sfout.open("CountCom.txt");
    sfout.open(this->countFile.c_str());
    sfout<<"Name"<<"\tNum"<<endl;
    for(int i=0;i<COM_TABLE_SIZE;i++)
    {
        sfout<<KeyWords[i]<<'\t'<<count[i]<<endl;
    }
    sfout.close();
    return 0;
}
//判断是不是关键字
bool assembler::IsKeyWords(char ID[COMMOM_SIZE])
{
    char Temp[COMMOM_SIZE];
    strcpy(Temp,ID);
    strlwr(Temp);
    for(int i=0;i<KEYWORDS_SIZE;i++)
    {
        if(strcmp(Temp,KeyWords[i])==0)
            return true;
    }
    return false;
}
//分隔符
bool assembler::IsSeparate(char ch)
{
    if(ch==' '||ch=='\t'||ch=='\n'||ch=='\0'||
       ch==';'||ch==':'||ch==','||ch=='(')
        return true;
    else if((int)ch<0)   //中文字符，也当做是分隔符
        return true;
    else
        return false;
}
//读一个字符串
void assembler::ReadAString(char Str[COMMOM_SIZE])
{
    char ch;
    while(1)
    {
        ch=ReadAChar();
        if(IsSeparate(ch)) { WriteAChar(ch); break;}
        else  TOKEN.Content[++TOKEN.Idx]=ch;
    }
    strcpy(Str,TOKEN.Content);
    strlwr(Str);
}
//回写一个字符串
void assembler::WriteAString()
{
    while(TOKEN.Idx!=-1)  WriteAChar(TOKEN.Content[TOKEN.Idx--]);
    strcpy(TOKEN.Content," ");
}
//读入一个字符
char assembler::ReadAChar()
{
    if(!fin.eof())
    {
        char ch=fin.get();
        if((int)ch==-1) return '\0';
        if(ch=='\n') NumOfLine++;
        return ch;
    }
    return '\0';
}
//回写一个字符
void assembler::WriteAChar(char c)
{
    if(c!='\0')
    {
        if(c=='\n') NumOfLine--;
        fin.putback(c);
    }
}
//读写文件程序结束
//初始化类成员变量
void assembler::InitVar()
{
    NumOfError=0;         //错误数
    NumOfWarning=0;       //警告数
    NumOfLine=1;          //行号
    CurrentDataAddr=-1;   //当前数据地址
    CurrentComAddr=-1;    //当前指令地址
    CurrentRule=0;        //当前规则
    LastRule=0;           //前一条规则
    HasError=0;           //单词有没有错
    NeedExplain=2;        //默认需要输出注释
    NeedSel=0;			  //默认不需要翻译sel
    DATAS_TABLE=NULL;     //数据表首地址
    DATA_POS=NULL;        //数据表当前位置
    COMS_TABLE=NULL;      //指令表首地址
    COM_POS=NULL;         //指令表当前位置
    VARS_TABLE=NULL;      //变量表首地址
    VAR_POS=NULL;         //变量表当前位置
    SEGIDS_TABLE=NULL;    //标号表首地址
    SEGID_POS=NULL;       //标号表当前位置
    BACKS_TABLE=NULL;     //回填表首地址
    BACK_POS=NULL;        //回填表当前位置
    //栈初始化
    STACK.Depth[0]=-1;    //栈底压-1
    STACK.Depth[1]=N_PRO; //初始化为程序开始符N_PRO
    STACK.Top=1;          //当前栈位置


    CODE="10001001101010111100";

}
