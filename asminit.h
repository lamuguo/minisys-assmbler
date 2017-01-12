#ifndef ASMINIT_H
#define ASMINIT_H
//初始化LL(1)预测分析表
void InitForcastTable(int ForcastTable[VN_NUM][VT_NUM])
{
	for(int i=0;i<=VN_NUM;i++)
	{
		for(int j=0;j<=VT_NUM;j++)
		{
			ForcastTable[i][j]=0;
		}
	}
	ForcastTable[N_PRO-50][T_DATA]=1;
	ForcastTable[N_PRO-50][T_CODE]=1;
    ForcastTable[N_DATA-50][T_DATA]=2;
	ForcastTable[N_DATAB-50][T_ENDL] = 3;
	ForcastTable[N_DATAB-50][T_ADDR] = 4;
	ForcastTable[N_DATAA-50][T_ORG_DATA]=5;
	ForcastTable[N_DATAA-50][T_IDNAME]=6;
	ForcastTable[N_DATAA-50][T_DATA]=7;
    ForcastTable[N_ORG_DATA-50][T_ORG_DATA]=8;
	ForcastTable[N_VARS-50][T_IDNAME]=9;
	ForcastTable[N_VARS-50][T_DATA]=10;
	ForcastTable[N_VARS-50][T_ORG_DATA]=10;
	ForcastTable[N_VAR-50][T_IDNAME]=11;
	ForcastTable[N_VARA-50][T_COMMA]=12;
	ForcastTable[N_VARA-50][T_ENDL]=13;
	ForcastTable[N_VARAA-50][T_DW]=14;
	ForcastTable[N_VARAA-50][T_IDNAME]=15;
	ForcastTable[N_VARAA-50][T_ORG_DATA]=15;
	ForcastTable[N_VARAA-50][T_DATA]=15;
    ForcastTable[N_DATAENDS-50][T_DATA]=16;
    ForcastTable[N_CODE-50][T_CODE]=17;
    ForcastTable[N_CODESEG-50][T_CODE]=18;
	ForcastTable[N_CODEA-50][T_ORG_CODE]=19;
	ForcastTable[N_CODEA-50][T_IDNAME]=20;
    ForcastTable[N_ORG_CODE-50][T_ORG_CODE]=21;
    ForcastTable[N_SEG-50][T_IDNAME]=22;
    ForcastTable[N_STARTSEGID-50][T_IDNAME]=23;
    ForcastTable[N_ORDER-50][T_RCOM]=24;
    ForcastTable[N_ORDER-50][T_SRCOM]=24;
    ForcastTable[N_ORDER-50][T_SLLRCOM]=24;
    ForcastTable[N_ORDER-50][T_ICOM]=24;
    ForcastTable[N_ORDER-50][T_SICOM]=24;
    ForcastTable[N_ORDER-50][T_LWICOM]=24;
    ForcastTable[N_ORDER-50][T_JCOM]=24;
	ForcastTable[N_ORDER-50][T_BICOM]=24;
	ForcastTable[N_ORDER-50][T_PCOM]=24;
	ForcastTable[N_ORDER-50][T_JBCOM]=24;
	ForcastTable[N_ORDER-50][T_NOP]=24;
	ForcastTable[N_ORDER-50][T_ORG_CODE]=25;
    ForcastTable[N_COM-50][T_RCOM]=26;
    ForcastTable[N_COM-50][T_SRCOM]=27;
    ForcastTable[N_COM-50][T_SLLRCOM]=28;
    ForcastTable[N_COM-50][T_ICOM]=29;
	ForcastTable[N_COM-50][T_SICOM]=30;
	ForcastTable[N_COM-50][T_LWICOM]=31;
	ForcastTable[N_COM-50][T_JCOM]=32;
    ForcastTable[N_COM-50][T_BICOM]=33;
	ForcastTable[N_COM-50][T_PCOM]=34;
	ForcastTable[N_COM-50][T_JBCOM]=35;
	ForcastTable[N_COM-50][T_NOP]=36;
    ForcastTable[N_IMMEDIATE-50][T_IDNAME]=37;
	ForcastTable[N_IMMEDIATE-50][T_IMMEDIATE]=38;
    ForcastTable[N_ADDR-50][T_IDNAME]=39;
	ForcastTable[N_ADDR-50][T_ADDR]=40;
	ForcastTable[N_ORDERS-50][T_ORG_CODE]=41;
	ForcastTable[N_ORDERS-50][T_RCOM]=41;
	ForcastTable[N_ORDERS-50][T_SRCOM]=41;
	ForcastTable[N_ORDERS-50][T_SLLRCOM]=41;
	ForcastTable[N_ORDERS-50][T_ICOM]=41;
	ForcastTable[N_ORDERS-50][T_SICOM]=41;
	ForcastTable[N_ORDERS-50][T_LWICOM]=41;
	ForcastTable[N_ORDERS-50][T_JCOM]=41;
	ForcastTable[N_ORDERS-50][T_BICOM]=41;
	ForcastTable[N_ORDERS-50][T_PCOM]=41;
	ForcastTable[N_ORDERS-50][T_JBCOM]=41;
	ForcastTable[N_ORDERS-50][T_NOP]=41;
	ForcastTable[N_ORDERS-50][T_IDNAME]=42;
	ForcastTable[N_ORDERS-50][T_END]=43;
	ForcastTable[N_SUBSEGID-50][T_IDNAME]=44;
	ForcastTable[N_ENDSEGID-50][T_END]=45;
	ForcastTable[N_CODEENDS-50][T_CODE]=46;

	ForcastTable[N_ORDERS-50][T_MRCOM]=41;
	ForcastTable[N_ORDERS-50][T_MFCRCOM]=41;
	ForcastTable[N_ORDERS-50][T_MULRCOM]=41;
	ForcastTable[N_ORDERS-50][T_SYSRCOM]=41;
	ForcastTable[N_ORDERS-50][T_JRCOM]=41;
	ForcastTable[N_ORDERS-50][T_BGICOM]=41;

	ForcastTable[N_COM-50][T_MRCOM]=47;
    ForcastTable[N_COM-50][T_MFCRCOM]=48;
    ForcastTable[N_COM-50][T_MULRCOM]=49;
	ForcastTable[N_COM-50][T_SYSRCOM]=50;
	ForcastTable[N_COM-50][T_JRCOM]=51;
	ForcastTable[N_COM-50][T_BGICOM]=52;

	ForcastTable[N_ORDER-50][T_MRCOM]=24;
    ForcastTable[N_ORDER-50][T_MFCRCOM]=24;
	ForcastTable[N_ORDER-50][T_MULRCOM]=24;
	ForcastTable[N_ORDER-50][T_SYSRCOM]=24;
	ForcastTable[N_ORDER-50][T_JRCOM]=24;
	ForcastTable[N_ORDER-50][T_BGICOM]=24;

	ForcastTable[N_CP0-50][T_COMMA]=53;
	ForcastTable[N_CP0-50][T_ENDL]=54;
 
}

//初始化语法规则
/*
BNF:

 */
void InitSyntacticRules(int SyntacticRules[RULE_NUM][RULE_DEPTH])
{
    for(int i=0;i<=RULE_NUM;i++)
	{
		for(int j=0;j<=RULE_DEPTH;j++)
		{
			SyntacticRules[i][j]=0;
		}
	}

    //RULE 1: N_PRO->N_DATA N_CODE
	SyntacticRules[1][0]=N_CODE;
	SyntacticRules[1][1]=N_DATA;

	//RULE 2: N_DATA -> T_DATA N_DATAB N_DATAA
	SyntacticRules[2][0] = N_DATAENDS;
	SyntacticRules[2][1] = N_DATAA;
	SyntacticRules[2][2] = N_DATAB;
	SyntacticRules[2][3] = T_DATA;
	//RULE 3: N_DATAB -> ENDL
	SyntacticRules[3][0] = T_ENDL;
	//RULE 4: N_DATAB -> T_ADDR T_ENDL
	SyntacticRules[4][0] = T_ENDL;
	SyntacticRules[4][1] = T_ADDR;

	//RULE 5: N_DATAA->N_ORG_DATA N_VARS N_DATAA
	SyntacticRules[5][0]=N_DATAA;
	SyntacticRules[5][1]=N_VARS;
	SyntacticRules[5][2]=N_ORG_DATA;
	//RULE 6: N_DATAA->N_VARS N_DATAA
	SyntacticRules[6][0]=N_DATAA;
	SyntacticRules[6][1]=N_VARS;
	//RULE 7: N_DATA-> nothing

    //RULE 8: N_ORG_DATA->T_ORG_DATA T_ADDR T_ENDL
	SyntacticRules[8][0]=T_ENDL;
	SyntacticRules[8][1]=T_ADDR;
	SyntacticRules[8][2]=T_ORG_DATA;
	//RULE 9:N_VARS->N_VAR N_VARS
	SyntacticRules[9][0]=N_VARS;
	SyntacticRules[9][1]=N_VAR;
	//RULE 10:N_VARS->nothing

	//RULE 11:N_VAR->T_IDNAME T_DD T_16NUM N_VARA T_ENDL N_VARAA
	SyntacticRules[11][0]=N_VARAA;
	SyntacticRules[11][1]=T_ENDL;
	SyntacticRules[11][2]=N_VARA;
	SyntacticRules[11][3]=T_NUM;
	SyntacticRules[11][4]=T_DW;
	SyntacticRules[11][5]=T_IDNAME;
	//RULE 12:N_VARA->T_COMMA T_16NUM N_VARA  
	SyntacticRules[12][0]=N_VARA;
	SyntacticRules[12][1]=T_NUM;
	SyntacticRules[12][2]=T_COMMA;
	//RULE 13:N_VARA->nothing

	//RULE 14:N_VARAA->T_DD T_16NUM N_VARA T_ENDL N_VARAA
    SyntacticRules[14][0]=N_VARAA;
	SyntacticRules[14][1]=T_ENDL;
	SyntacticRules[14][2]=N_VARA;
	SyntacticRules[14][3]=T_NUM;
	SyntacticRules[14][4]=T_DW;
	//RULE 15:N_VARAA->nothing

	//RULE 16: N_DATAENDS->T_DATA T_ENDS T_ENDL
	SyntacticRules[16][0]=T_ENDL;
	SyntacticRules[16][1]=T_ENDS;
	SyntacticRules[16][2]=T_DATA;
	//RULE 17: N_CODE->N_CODESEG N_CODEA N_CODEENDS
	SyntacticRules[17][0]=N_CODEENDS;
	SyntacticRules[17][1]=N_CODEA;
	SyntacticRules[17][2]=N_CODESEG;
	//RULE 18: N_CODESEG->T_CODE T_SEG T_ENDL
	SyntacticRules[18][0]=T_ENDL;
	SyntacticRules[18][1]=T_SEG;
	SyntacticRules[18][2]=T_CODE;
	//RULE 19: N_CODEA->N_ORG_CODE N_SEG
	SyntacticRules[19][0]=N_SEG;
	SyntacticRules[19][1]=N_ORG_CODE;
	//RULE 20: N_CODEA->N_SEG
	SyntacticRules[20][0]=N_SEG;
	//RULE 21: N_ORG_CODE->T_ORG_CODE T_ADDR T_ENDL
	SyntacticRules[21][0]=T_ENDL;
	SyntacticRules[21][1]=T_ADDR;
	SyntacticRules[21][2]=T_ORG_CODE;
	//RULE 22: N_SEG->N_STARTSEGID N_ORDER N_ORDERS N_ENDSEGID
	SyntacticRules[22][0]=N_ENDSEGID;
	SyntacticRules[22][1]=N_ORDERS;
	SyntacticRules[22][2]=N_ORDER;
	SyntacticRules[22][3]=N_STARTSEGID;
	//RULE 23: N_STARTSEGID->T_IDNAME T_COLON
	SyntacticRules[23][0]=T_COLON;
	SyntacticRules[23][1]=T_IDNAME;
	//RULE 24: N_ORDER->N_COM T_ENDL
	SyntacticRules[24][0]=T_ENDL;
	SyntacticRules[24][1]=N_COM;
	//RULE 25: N_ORFER->N_ORG_CODE
	SyntacticRules[25][0]=N_ORG_CODE;
	//RULE 26: N_COM->T_RCOM T_RD T_COMMA T_RS T_COMMA T_RT
	SyntacticRules[26][0]=T_RT;
	SyntacticRules[26][1]=T_COMMA;
	SyntacticRules[26][2]=T_RS;
	SyntacticRules[26][3]=T_COMMA;
	SyntacticRules[26][4]=T_RD;
	SyntacticRules[26][5]=T_RCOM;
	//RULE 27: N_COM->T_SRCOM  T_RS
	SyntacticRules[27][0]=T_RS;
	SyntacticRules[27][1]=T_SRCOM;
	//RULE 28: N_COM->T_SLLRCOM T_RD T_COMMA T_RT T_COMMA T_SHAMT
	SyntacticRules[28][0]=T_SHAMT;
	SyntacticRules[28][1]=T_COMMA;
	SyntacticRules[28][2]=T_RT;
	SyntacticRules[28][3]=T_COMMA;
	SyntacticRules[28][4]=T_RD;
	SyntacticRules[28][5]=T_SLLRCOM;
	//RULE 29: N_COM->T_ICOM T_RT T_COMMA T_RS T_COMMA T_IMMEDIATE
	SyntacticRules[29][0]=T_IMMEDIATE;
	SyntacticRules[29][1]=T_COMMA;
	SyntacticRules[29][2]=T_RS;
	SyntacticRules[29][3]=T_COMMA;
	SyntacticRules[29][4]=T_RT;
	SyntacticRules[29][5]=T_ICOM;

	//RULE 30: N_COM->T_SICOM T_RT T_COMMA T_IMMEDIATE
	SyntacticRules[30][0]=T_IMMEDIATE;
	SyntacticRules[30][1]=T_COMMA;
	SyntacticRules[30][2]=T_RT;
	SyntacticRules[30][3]=T_SICOM;
	//RULE 31: N_COM->T_LWICOM T_RT T_COMMA N_IMMEDIATE T_BRS
	SyntacticRules[31][0]=T_BRS;
	SyntacticRules[31][1]=N_IMMEDIATE;
	SyntacticRules[31][2]=T_COMMA;
	SyntacticRules[31][3]=T_RT;
	SyntacticRules[31][4]=T_LWICOM;
	//RULE 32: N_COM->T_JCOM N_ADDR
	SyntacticRules[32][0]=N_ADDR;
	SyntacticRules[32][1]=T_JCOM;
	//RULE 33: N_COM->T_BICOM T_RT T_COMMA T_RS T_COMMA N_IMMEDIATE
	SyntacticRules[33][0]=N_IMMEDIATE;
	SyntacticRules[33][1]=T_COMMA;
	SyntacticRules[33][2]=T_RS;
	SyntacticRules[33][3]=T_COMMA;
	SyntacticRules[33][4]=T_RT;
	SyntacticRules[33][5]=T_BICOM;
	//RULE 34: N_COM->T_PCOM T_RS
	SyntacticRules[34][0]=T_RS;
	SyntacticRules[34][1]=T_PCOM;
	//RULE 35: N_COM->T_JBCOM T_RT T_COMMA T_RS T_COMMA N_IMMEDIATE
	SyntacticRules[35][0]=N_IMMEDIATE;
	SyntacticRules[35][1]=T_COMMA;
	SyntacticRules[35][2]=T_RS;
	SyntacticRules[35][3]=T_COMMA;
	SyntacticRules[35][4]=T_RT;
	SyntacticRules[35][5]=T_JBCOM;
	//Rule 36: N_COM->T_NOP
	SyntacticRules[36][0]=T_NOP;
	//RULE 37: N_IMMEDIATE->T_IDNAME
	SyntacticRules[37][0]=T_IDNAME;
    //RULE 38: N_IMMEDIATE->T_IMMEDIATE
	SyntacticRules[38][0]=T_IMMEDIATE;
	//RULE 39: N_ADDR->T_IDNAME
	SyntacticRules[39][0]=T_IDNAME;
	//RULE 40: N_ADDR->T_ADDR
	SyntacticRules[40][0]=T_ADDR;
	//RULE 41: N_ORDERS->N_ORDER N_ORDERS
	SyntacticRules[41][0]=N_ORDERS;
	SyntacticRules[41][1]=N_ORDER;
	//RULE 42: N_ORDERS->N_SUBSEGID N_ORDER N_ORDERS
	SyntacticRules[42][0]=N_ORDERS;
	SyntacticRules[42][1]=N_ORDER;
	SyntacticRules[42][2]=N_SUBSEGID;
	//RULE 43: N_ORDERS->nothing

	//RULE 44: N_SUBSEGID->T_IDNAME T_COLON
	SyntacticRules[44][0]=T_COLON;
	SyntacticRules[44][1]=T_IDNAME;
	//RULE 45: N_ENDSEGID->T_END T_IDNAME T_ENDL
	SyntacticRules[45][0]=T_ENDL;
	SyntacticRules[45][1]=T_IDNAME;
	SyntacticRules[45][2]=T_END;
	//RULE 46: N_CODEENDS->T_CODE T_ENDS T_ENDL
	SyntacticRules[46][0]=T_ENDL;
	SyntacticRules[46][1]=T_ENDS;
	SyntacticRules[46][2]=T_CODE;

	//开始新增8条规则
	//RULE 47: N_COM->T_MRCOM T_RD
	SyntacticRules[47][0]=T_RD;
	SyntacticRules[47][1]=T_MRCOM;
	//RULE 48: N_COM->T_MFCRCOM T_RT T_COMMA T_RD N_CP0
	SyntacticRules[48][0]=N_CP0;
	SyntacticRules[48][1]=T_RD;
	SyntacticRules[48][2]=T_COMMA;
	SyntacticRules[48][3]=T_RT;
	SyntacticRules[48][4]=T_MFCRCOM;
	//RULE 49: N_COM->T_MULRCOM T_RS T_COMMA T_RT
	SyntacticRules[49][0]=T_RT;
	SyntacticRules[49][1]=T_COMMA;
	SyntacticRules[49][2]=T_RS;
	SyntacticRules[49][3]=T_MULRCOM;
	//RULE 50: N_COM->T_SYSRCOM
	SyntacticRules[50][0]=T_SYSRCOM;
	//RULE 51: N_COM->T_JRCOM T_RD T_COMMA T_RS
	SyntacticRules[51][0]=T_RS;
	SyntacticRules[51][1]=T_COMMA;
	SyntacticRules[51][2]=T_RD;
	SyntacticRules[51][3]=T_JRCOM;
	//RULE 52: N_COM->T_BGICOM T_RS T_COMMA N_ IMMEDIATE
	SyntacticRules[52][0]=N_IMMEDIATE;
	SyntacticRules[52][1]=T_COMMA;
	SyntacticRules[52][2]=T_RS;
	SyntacticRules[52][3]=T_BGICOM;

	//RULE 53: N_CP0->T_COMMA T_SEL
	SyntacticRules[53][0]=T_SEL;
	SyntacticRules[53][1]=T_COMMA;

	//RULE 54: N_COM->nothing

}
//初始化关键字


void InitKeyWords(char *KeyWords[KEYWORDS_SIZE])
{
	for (int i=0;i<KEYWORDS_SIZE;i++)
	{
		KeyWords[i]="";
	}

	//0-13:RCOM 新增mul
	KeyWords[0]="add";
	KeyWords[1]="addu";
	KeyWords[2]="sub";
	KeyWords[3]="subu";
	KeyWords[4]="and";
	KeyWords[5]="or";
	KeyWords[6]="xor";
	KeyWords[7]="nor";
	KeyWords[8]="slt";
	KeyWords[9]="sltu";
	KeyWords[10]="sllv";
	KeyWords[11]="srlv";
	KeyWords[12]="srav";
	KeyWords[13]="mul";

	//14-16:SLLRCOM 没改动
	KeyWords[14]="sll";
	KeyWords[15]="srl";
	KeyWords[16]="sra";

	//17-19:SRCOM 增加mthi,mtlo
	KeyWords[17]="jr";
	KeyWords[18]="mthi";
	KeyWords[19]="mtlo";

	//20-21:MRCOM 增加MRCOM
	KeyWords[20]="mfhi";
	KeyWords[21]="mflo";

	//22-23:MFCRCOM 增加MFCRCOM
	KeyWords[22]="mfc0";
	KeyWords[23]="mtc0";

	//24-27:MULRCOM 增加MULRCOM
	KeyWords[24]="mult";
	KeyWords[25]="multu";
	KeyWords[26]="div";
	KeyWords[27]="divu";

	//28-30:SYSRCOM 增加SYSRCOM
	KeyWords[28]="break";
	KeyWords[29]="syscall";
    KeyWords[30]="eret";

	//31:JRCOM 增加JRCOM
	KeyWords[31]="jalr";


	//32-38:ICOM 没改动
	KeyWords[32]="addi";
	KeyWords[33]="addiu";
	KeyWords[34]="andi";
	KeyWords[35]="ori";
	KeyWords[36]="xori";
	KeyWords[37]="slti";
	KeyWords[38]="sltiu";
	
	//39-46:LWICOM 新增lb,lbu;lh,lhu,sb,sh
	KeyWords[39]="lb";
	KeyWords[40]="lbu";
	KeyWords[41]="lh";
	KeyWords[42]="lhu";
	KeyWords[43]="sb";
	KeyWords[44]="sh";
	KeyWords[45]="lw";
	KeyWords[46]="sw";

	//47:SICOM 未改动
	KeyWords[47]="lui";

	//48-49:BICOM 未改动
	KeyWords[48]="beq";
	KeyWords[49]="bne";

	//50-55:BGICOM 新增
	KeyWords[50]="bgez";
	KeyWords[51]="bgtz";
	KeyWords[52]="blez";
	KeyWords[53]="bltz";
	KeyWords[54]="bgezal";
	KeyWords[55]="bltzal";

	//56-57:JCOM 没改动
	KeyWords[56]="j";
	KeyWords[57]="jal";

	//58: NOP
	KeyWords[58]="nop";

	//59-60:PCOM
	KeyWords[59]="push";
	KeyWords[60]="pop";

	//61-64:JBCOM
	KeyWords[61]="jl";
	KeyWords[62]="jle";
	KeyWords[63]="jg";
	KeyWords[64]="jge";

	//65-72:ORG_CODE
	KeyWords[65]=".org_code";
	KeyWords[66]=".org_data";
	KeyWords[67]=".text";
	KeyWords[68]=".data";
	KeyWords[69]=".word";
	KeyWords[71]=".end";
	KeyWords[72]=".ends";

}


//初始化功能号表
void InitFuncTable(My_Func FuncTable[FUNC_TABLE_SIZE])
{

	strcpy(FuncTable[0].Name,"add");
	strcpy(FuncTable[0].Func,"100000");
	strcpy(FuncTable[1].Name,"addu");
	strcpy(FuncTable[1].Func,"100001");
	strcpy(FuncTable[2].Name,"sub");
	strcpy(FuncTable[2].Func,"100010");
	strcpy(FuncTable[3].Name,"subu");
	strcpy(FuncTable[3].Func,"100011");
	strcpy(FuncTable[4].Name,"and");
	strcpy(FuncTable[4].Func,"100100");
	strcpy(FuncTable[5].Name,"or");
	strcpy(FuncTable[5].Func,"100101");
	strcpy(FuncTable[6].Name,"xor");
	strcpy(FuncTable[6].Func,"100110");
	strcpy(FuncTable[7].Name,"nor");
	strcpy(FuncTable[7].Func,"100111");
	strcpy(FuncTable[8].Name,"slt");
	strcpy(FuncTable[8].Func,"101010");
	strcpy(FuncTable[9].Name,"sltu");
	strcpy(FuncTable[9].Func,"101011");
	strcpy(FuncTable[10].Name,"sll");
	strcpy(FuncTable[10].Func,"000000");
	strcpy(FuncTable[11].Name,"srl");
	strcpy(FuncTable[11].Func,"000010");
	strcpy(FuncTable[12].Name,"sra");
	strcpy(FuncTable[12].Func,"000011");
	strcpy(FuncTable[13].Name,"sllv");
	strcpy(FuncTable[13].Func,"000100");
	strcpy(FuncTable[14].Name,"srlv");
	strcpy(FuncTable[14].Func,"000110");
	strcpy(FuncTable[15].Name,"srav");
	strcpy(FuncTable[15].Func,"000111");
	strcpy(FuncTable[16].Name,"jr");
	strcpy(FuncTable[16].Func,"001000");

	//新增的
	strcpy(FuncTable[17].Name,"mul");
	strcpy(FuncTable[17].Func,"000010");
	strcpy(FuncTable[18].Name,"mult");
	strcpy(FuncTable[18].Func,"011000");
	strcpy(FuncTable[19].Name,"multu");
	strcpy(FuncTable[19].Func,"011001");
	strcpy(FuncTable[20].Name,"div");
	strcpy(FuncTable[20].Func,"011010");
	strcpy(FuncTable[21].Name,"divu");
	strcpy(FuncTable[21].Func,"011011");
	strcpy(FuncTable[22].Name,"mfhi");
	strcpy(FuncTable[22].Func,"010000");
	strcpy(FuncTable[23].Name,"mflo");
	strcpy(FuncTable[23].Func,"010010");
	strcpy(FuncTable[24].Name,"mthi");
	strcpy(FuncTable[24].Func,"010001");
	strcpy(FuncTable[25].Name,"mtlo");
	strcpy(FuncTable[25].Func,"010011");
	strcpy(FuncTable[26].Name,"mfc0");
	strcpy(FuncTable[26].Func,"000000");//无func，将rs区分码作为func
	strcpy(FuncTable[27].Name,"mtc0");
	strcpy(FuncTable[27].Func,"000100");
	strcpy(FuncTable[28].Name,"jalr");
	strcpy(FuncTable[28].Func,"001001");
	strcpy(FuncTable[29].Name,"break");
	strcpy(FuncTable[29].Func,"001101");
	strcpy(FuncTable[30].Name,"syscall");
	strcpy(FuncTable[30].Func,"001100");
	strcpy(FuncTable[31].Name,"eret");
	strcpy(FuncTable[31].Func,"011000");

	//I类型中有区别码的
	strcpy(FuncTable[32].Name,"bgez");
	strcpy(FuncTable[32].Func,"000001");
	strcpy(FuncTable[33].Name,"bgezal");
	strcpy(FuncTable[33].Func,"010001");
	strcpy(FuncTable[34].Name,"bltzal");
	strcpy(FuncTable[34].Func,"010000");
	strcpy(FuncTable[35].Name,"bgtz");
	strcpy(FuncTable[35].Func,"000000");
	strcpy(FuncTable[36].Name,"blez");
	strcpy(FuncTable[36].Func,"000000");
	strcpy(FuncTable[37].Name,"bltz");
	strcpy(FuncTable[37].Func,"000000");

}
//初始化助记符表
void InitOpTable(My_Op  OpTable[OP_TABLE_SIZE])
{
	strcpy(OpTable[0].Name,"addi");
	strcpy(OpTable[0].Op,"001000");
	strcpy(OpTable[1].Name,"addiu");
	strcpy(OpTable[1].Op,"001001");
	strcpy(OpTable[2].Name,"andi");
	strcpy(OpTable[2].Op,"001100");
	strcpy(OpTable[3].Name,"ori");
	strcpy(OpTable[3].Op,"001101");
	strcpy(OpTable[4].Name,"xori");
	strcpy(OpTable[4].Op,"001110");
	strcpy(OpTable[5].Name,"lui");
	strcpy(OpTable[5].Op,"001111");
	strcpy(OpTable[6].Name,"lw");
	strcpy(OpTable[6].Op,"100011");
	strcpy(OpTable[7].Name,"sw");
	strcpy(OpTable[7].Op,"101011");
	strcpy(OpTable[8].Name,"beq");
	strcpy(OpTable[8].Op,"000100");
	strcpy(OpTable[9].Name,"bne");
	strcpy(OpTable[9].Op,"000101");
	strcpy(OpTable[10].Name,"slti");
	strcpy(OpTable[10].Op,"001010");
	strcpy(OpTable[11].Name,"sltiu");
	strcpy(OpTable[11].Op,"001011");
	strcpy(OpTable[12].Name,"j");
	strcpy(OpTable[12].Op,"000010");
	strcpy(OpTable[13].Name,"jal");
	strcpy(OpTable[13].Op,"000011");

	//新增的R类型中有op的
	strcpy(OpTable[14].Name,"mul");
	strcpy(OpTable[14].Op,"011100");
	strcpy(OpTable[15].Name,"mfc0");
	strcpy(OpTable[15].Op,"010000");
	strcpy(OpTable[16].Name,"mtc0");
	strcpy(OpTable[16].Op,"010000");

	//新增的I类型
	strcpy(OpTable[17].Name,"lb");
	strcpy(OpTable[17].Op,"100000");
	strcpy(OpTable[18].Name,"lbu");
	strcpy(OpTable[18].Op,"100100");
	strcpy(OpTable[19].Name,"lh");
	strcpy(OpTable[19].Op,"100001");
	strcpy(OpTable[20].Name,"lhu");
	strcpy(OpTable[20].Op,"100101");
	strcpy(OpTable[21].Name,"sb");
	strcpy(OpTable[21].Op,"101000");
	strcpy(OpTable[22].Name,"sh");
	strcpy(OpTable[22].Op,"101001");

	//新增I类型中有区别码的
	strcpy(OpTable[23].Name,"bgez");
	strcpy(OpTable[23].Op,"000001");
	strcpy(OpTable[24].Name,"bgtz");
	strcpy(OpTable[24].Op,"000111");
	strcpy(OpTable[25].Name,"blez");
	strcpy(OpTable[25].Op,"000110");
	strcpy(OpTable[26].Name,"bltz");
	strcpy(OpTable[26].Op,"000001");
	strcpy(OpTable[27].Name,"bgezal");
	strcpy(OpTable[27].Op,"000001");
	strcpy(OpTable[28].Name,"bltzal");
	strcpy(OpTable[28].Op,"000001");
}
//初始化积存器编号表
void InitRegTable(My_Reg  RegTable[REG_TABLE_SIZE])
{
	strcpy(RegTable[0].Name,"$zero");
	strcpy(RegTable[0].Reg,"00000");
	strcpy(RegTable[1].Name,"$at");
	strcpy(RegTable[1].Reg,"00001");
	strcpy(RegTable[2].Name,"$v0");
	strcpy(RegTable[2].Reg,"00010");
	strcpy(RegTable[3].Name,"$v1");
	strcpy(RegTable[3].Reg,"00011");
	strcpy(RegTable[4].Name,"$a0");
	strcpy(RegTable[4].Reg,"00100");
	strcpy(RegTable[5].Name,"$a1");
	strcpy(RegTable[5].Reg,"00101");
	strcpy(RegTable[6].Name,"$a2");
	strcpy(RegTable[6].Reg,"00110");
	strcpy(RegTable[7].Name,"$a3");
	strcpy(RegTable[7].Reg,"00111");
	strcpy(RegTable[8].Name,"$t0");
	strcpy(RegTable[8].Reg,"01000");
	strcpy(RegTable[9].Name,"$t1");
	strcpy(RegTable[9].Reg,"01001");
	strcpy(RegTable[10].Name,"$t2");
	strcpy(RegTable[10].Reg,"01010");
	strcpy(RegTable[11].Name,"$t3");
	strcpy(RegTable[11].Reg,"01011");
	strcpy(RegTable[12].Name,"$t4");
	strcpy(RegTable[12].Reg,"01100");
	strcpy(RegTable[13].Name,"$t5");
	strcpy(RegTable[13].Reg,"01101");
	strcpy(RegTable[14].Name,"$t6");
	strcpy(RegTable[14].Reg,"01110");
	strcpy(RegTable[15].Name,"$t7");
	strcpy(RegTable[15].Reg,"01111");
	strcpy(RegTable[16].Name,"$s0");
	strcpy(RegTable[16].Reg,"10000");
	strcpy(RegTable[17].Name,"$s1");
	strcpy(RegTable[17].Reg,"10001");
	strcpy(RegTable[18].Name,"$s2");
	strcpy(RegTable[18].Reg,"10010");
	strcpy(RegTable[19].Name,"$s3");
	strcpy(RegTable[19].Reg,"10011");
	strcpy(RegTable[20].Name,"$s4");
	strcpy(RegTable[20].Reg,"10100");
	strcpy(RegTable[21].Name,"$s5");
	strcpy(RegTable[21].Reg,"10101");
	strcpy(RegTable[22].Name,"$s6");
	strcpy(RegTable[22].Reg,"10110");
	strcpy(RegTable[23].Name,"$s7");
	strcpy(RegTable[23].Reg,"10111");
	strcpy(RegTable[24].Name,"$t8");
	strcpy(RegTable[24].Reg,"11000");
	strcpy(RegTable[25].Name,"$t9");
	strcpy(RegTable[25].Reg,"11001");
	strcpy(RegTable[26].Name,"$i0");
	strcpy(RegTable[26].Reg,"11010");
	strcpy(RegTable[27].Name,"$i1");
	strcpy(RegTable[27].Reg,"11011");
	strcpy(RegTable[28].Name,"$s9");
	strcpy(RegTable[28].Reg,"11100");
	strcpy(RegTable[29].Name,"$sp");
	strcpy(RegTable[29].Reg,"11101");
	strcpy(RegTable[30].Name,"$s8");
	strcpy(RegTable[30].Reg,"11110");
	strcpy(RegTable[31].Name,"$ra");
	strcpy(RegTable[31].Reg,"11111");
	strcpy(RegTable[32].Name,"$0");
	strcpy(RegTable[32].Reg,"00000");
	strcpy(RegTable[33].Name,"$1");
	strcpy(RegTable[33].Reg,"00001");
	strcpy(RegTable[34].Name,"$2");
	strcpy(RegTable[34].Reg,"00010");
	strcpy(RegTable[35].Name,"$3");
	strcpy(RegTable[35].Reg,"00011");
	strcpy(RegTable[36].Name,"$4");
	strcpy(RegTable[36].Reg,"00100");
	strcpy(RegTable[37].Name,"$5");
	strcpy(RegTable[37].Reg,"00101");
	strcpy(RegTable[38].Name,"$6");
	strcpy(RegTable[38].Reg,"00110");
	strcpy(RegTable[39].Name,"$7");
	strcpy(RegTable[39].Reg,"00111");
	strcpy(RegTable[40].Name,"$8");
	strcpy(RegTable[40].Reg,"01000");
	strcpy(RegTable[41].Name,"$9");
	strcpy(RegTable[41].Reg,"01001");
	strcpy(RegTable[42].Name,"$10");
	strcpy(RegTable[42].Reg,"01010");
	strcpy(RegTable[43].Name,"$11");
	strcpy(RegTable[43].Reg,"01011");
	strcpy(RegTable[44].Name,"$12");
	strcpy(RegTable[44].Reg,"01100");
	strcpy(RegTable[45].Name,"$13");
	strcpy(RegTable[45].Reg,"01101");
	strcpy(RegTable[46].Name,"$14");
	strcpy(RegTable[46].Reg,"01110");
	strcpy(RegTable[47].Name,"$15");
	strcpy(RegTable[47].Reg,"01111");
	strcpy(RegTable[48].Name,"$16");
	strcpy(RegTable[48].Reg,"10000");
	strcpy(RegTable[49].Name,"$17");
	strcpy(RegTable[49].Reg,"10001");
	strcpy(RegTable[50].Name,"$18");
	strcpy(RegTable[50].Reg,"10010");
	strcpy(RegTable[51].Name,"$19");
	strcpy(RegTable[51].Reg,"10011");
	strcpy(RegTable[52].Name,"$20");
	strcpy(RegTable[52].Reg,"10100");
	strcpy(RegTable[53].Name,"$21");
	strcpy(RegTable[53].Reg,"10101");
	strcpy(RegTable[54].Name,"$22");
	strcpy(RegTable[54].Reg,"10110");
	strcpy(RegTable[55].Name,"$23");
	strcpy(RegTable[55].Reg,"10111");
	strcpy(RegTable[56].Name,"$24");
	strcpy(RegTable[56].Reg,"11000");
	strcpy(RegTable[57].Name,"$25");
	strcpy(RegTable[57].Reg,"11001");
	strcpy(RegTable[58].Name,"$26");
	strcpy(RegTable[58].Reg,"11010");
	strcpy(RegTable[59].Name,"$27");
	strcpy(RegTable[59].Reg,"11011");
	strcpy(RegTable[60].Name,"$28");
	strcpy(RegTable[60].Reg,"11100");
	strcpy(RegTable[61].Name,"$29");
	strcpy(RegTable[61].Reg,"11101");
	strcpy(RegTable[62].Name,"$30");
	strcpy(RegTable[62].Reg,"11110");
	strcpy(RegTable[63].Name,"$31");
	strcpy(RegTable[63].Reg,"11111");
	
	/////////////////新加寄存器///////////////////////////
	strcpy(RegTable[64].Name,"sr");
	strcpy(RegTable[64].Reg,"01100");
	strcpy(RegTable[65].Name,"cause");
	strcpy(RegTable[65].Reg,"01101");
	strcpy(RegTable[66].Name,"epc");
	strcpy(RegTable[66].Reg,"01110");
	strcpy(RegTable[67].Name,"count");
	strcpy(RegTable[67].Reg,"01001");
	strcpy(RegTable[68].Name,"compare");
	strcpy(RegTable[68].Reg,"01011");
	strcpy(RegTable[69].Name,"badvaddr");
	strcpy(RegTable[69].Reg,"01000");
	strcpy(RegTable[70].Name,"context");
	strcpy(RegTable[70].Reg,"00100");
	strcpy(RegTable[71].Name,"entryhi");
	strcpy(RegTable[71].Reg,"01010");
	strcpy(RegTable[72].Name,"entrylo0");
	strcpy(RegTable[72].Reg,"00010");
	strcpy(RegTable[73].Name,"entrylo1");
	strcpy(RegTable[73].Reg,"00011");
	strcpy(RegTable[74].Name,"index");
	strcpy(RegTable[74].Reg,"00000");
	strcpy(RegTable[75].Name,"pagemask");
	strcpy(RegTable[75].Reg,"00101");
	strcpy(RegTable[76].Name,"random");
	strcpy(RegTable[76].Reg,"00001");
	strcpy(RegTable[77].Name,"wired");
	strcpy(RegTable[77].Reg,"00110");
	strcpy(RegTable[78].Name,"prid");
	strcpy(RegTable[78].Reg,"01111");
	strcpy(RegTable[79].Name,"config");
	strcpy(RegTable[79].Reg,"10000");
	strcpy(RegTable[80].Name,"config1");
	strcpy(RegTable[80].Reg,"10000");
	strcpy(RegTable[81].Name,"config2");
	strcpy(RegTable[81].Reg,"10000");
	strcpy(RegTable[82].Name,"config3");
	strcpy(RegTable[82].Reg,"10000");
	strcpy(RegTable[83].Name,"ebase");
	strcpy(RegTable[83].Reg,"01111");
	strcpy(RegTable[84].Name,"intctl");
	strcpy(RegTable[84].Reg,"01100");
	strcpy(RegTable[85].Name,"srsctl");
	strcpy(RegTable[85].Reg,"01100");
	strcpy(RegTable[86].Name,"srsmap");
	strcpy(RegTable[86].Reg,"01100");
	strcpy(RegTable[87].Name,"cacheerr");
	strcpy(RegTable[87].Reg,"11011");
	strcpy(RegTable[88].Name,"ecc");
	strcpy(RegTable[88].Reg,"11010");
	strcpy(RegTable[89].Name,"errorepc");
	strcpy(RegTable[89].Reg,"11110");
	strcpy(RegTable[90].Name,"taglo");
	strcpy(RegTable[90].Reg,"11100");
	strcpy(RegTable[91].Name,"datalo");
	strcpy(RegTable[91].Reg,"11100");
	strcpy(RegTable[92].Name,"taghi");
	strcpy(RegTable[92].Reg,"11101");
	strcpy(RegTable[93].Name,"datahi");
	strcpy(RegTable[93].Reg,"11101");
	strcpy(RegTable[94].Name,"debug");
	strcpy(RegTable[94].Reg,"10111");
	strcpy(RegTable[95].Name,"depc");
	strcpy(RegTable[95].Reg,"11000");
	strcpy(RegTable[96].Name,"desave");
	strcpy(RegTable[96].Reg,"11111");
	strcpy(RegTable[97].Name,"watchlo");
	strcpy(RegTable[97].Reg,"10010");
	strcpy(RegTable[98].Name,"watchhi");
	strcpy(RegTable[98].Reg,"10011");
	strcpy(RegTable[99].Name,"perfctl");
	strcpy(RegTable[99].Reg,"11001");
	strcpy(RegTable[100].Name,"perfcnt");
	strcpy(RegTable[100].Reg,"11001");
	strcpy(RegTable[101].Name,"lladdr");
	strcpy(RegTable[101].Reg,"10001");
	strcpy(RegTable[102].Name,"hwrena");
	strcpy(RegTable[102].Reg,"00111");



}

void InitSelTable(My_Sel SelTable[SEL_TABLE_SIZE])
{
	strcpy(SelTable[0].Name,"config1");
	strcpy(SelTable[0].Sel,"001");
	strcpy(SelTable[1].Name,"config2");
	strcpy(SelTable[1].Sel,"010");
	strcpy(SelTable[2].Name,"config3");
	strcpy(SelTable[2].Sel,"011");
	strcpy(SelTable[3].Name,"ebase");
	strcpy(SelTable[3].Sel,"001");
	strcpy(SelTable[4].Name,"intctl");
	strcpy(SelTable[4].Sel,"001");
	strcpy(SelTable[5].Name,"srsctl");
	strcpy(SelTable[5].Sel,"010");
	strcpy(SelTable[6].Name,"srsmap");
	strcpy(SelTable[6].Sel,"011");
	strcpy(SelTable[7].Name,"datalo");
	strcpy(SelTable[7].Sel,"001");
	strcpy(SelTable[8].Name,"datahi");
	strcpy(SelTable[8].Sel,"001");
	strcpy(SelTable[9].Name,"perfcnt");
	strcpy(SelTable[9].Sel,"001");
}


#endif