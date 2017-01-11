#ifndef ASMFUNCTION_H
#define ASMFUNCTION_H
#include "asmconst.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
using namespace std;

int StrtoDNum(char Str[COMMOM_SIZE]);                              //字符串转换为十进制数
int DNumtoBStr(int dex,char *Str,int size) ;                       //十进制数转换为二进制串 
int StrtoBStr(char Str[COMMOM_SIZE],char *BStr,int size);          //将任意进制字符串转换为定长二进制字符串
int DStrtoBStr(char Str[COMMOM_SIZE],char *BStr,int size,int idx); //将十进制字符串转换为定长二进制字符串
int BStrtoBStr(char Str[COMMOM_SIZE],char *BStr,int size,int idx); //将二进制字符串转换为定长二进制字符串
int HStrtoBStr(char Str[COMMOM_SIZE],char *BStr,int size,int idx); //将十六进制字符串转换为定长二进制字符串
int Strlen(char Str[COMMOM_SIZE]);                                 //计算字符串长度

int Strlen(char Str[COMMOM_SIZE])   
{
	for(int i=0;i<COMMOM_SIZE;i++)
	{
		if(Str[i]=='\0') return i;
	} 
	return -1;
}
int StrtoDNum(char Str[COMMOM_SIZE]) 
{
	int  i=Strlen(Str)-1;
	char ch=Str[i];
	if(ch=='h'||ch=='H') {return strtol(Str,NULL,16);}
	else if(ch=='b'||ch=='B') {return strtol(Str,NULL,2);}
	else { return strtol(Str,NULL,10);}
} 
int DNumtoBStr(int dex,char *Str,int size)  
{
	int  i=size-1;
	bool flag=0;
	for(int x=0;x<size;x++) Str[x]='0';   //初始化
	if(dex<0) { dex=-dex; flag=1; }       //负数的情况
	//除2取余法
	while(dex>0)            
	{
		Str[i--]=dex%2+48;	//字符转换0->'0',1->'1'
		dex=dex/2;           
		if(i<-1) return -1;
	}
	//负数求补码
	if(flag==1)   
	{
		for(i=size-1;i>=0;i--) //取反
		{
			if(Str[i]=='0') Str[i]='1';
			else Str[i]='0';
		}
		for(i=size-1;i>=0;i--) //加一
		{
			if(Str[i]=='0') {Str[i]='1';break;}
			else Str[i]='0';
		}
	}
	return 0;
}
int StrtoBStr(char Str[COMMOM_SIZE],char *BStr,int size) 
{
	if (size<=0) return -1;
	char ch;
	strcpy(BStr,"");
	if(size>32) return -1;  //肯定不行，越界了

	int i;
	for(i=0;i<COMMOM_SIZE;i++)
	{
		if(Str[i]=='\0') { ch=Str[--i]; break; }
	}

	if(ch=='h'||ch=='H') return HStrtoBStr(Str,BStr,size,i-1);
	else if(ch=='b'||ch=='B') return BStrtoBStr(Str,BStr,size,i-1);
	else return DStrtoBStr(Str,BStr,size,i);
}
int DStrtoBStr(char Str[COMMOM_SIZE],char *BStr,int size,int idx)
{
	int dex=strtol(Str,NULL,10);
	if(size==32)  //32位数特别处理
	{
		if(dex<-2147483646||dex>2147483646) return -1; //越界
	}
	else 
	{
		int l=(int)pow(2.0,size);
		if(dex<-l||dex>=l) return -1;//越界
	}
	
	return DNumtoBStr(dex,BStr,size);
}
int BStrtoBStr(char Str[COMMOM_SIZE],char *BStr,int size,int idx)
{
	for(int i=size-1;i>=0;i--) //扩展
	{
		if(idx!=-1) BStr[i]=Str[idx--];
		else BStr[i]='0';
	}
	return 0;
}
int HStrtoBStr(char Str[COMMOM_SIZE],char *BStr,int size,int idx)
{
	int dex=strtol(Str,NULL,16);
	if(size==32)  //32位数特别处理
	{
		if(dex<-2147483646||dex>2147483646) return -1; //越界
	}
	else 
	{
		int l=(int)pow(2.0,size);
		if(dex<-l||dex>=l) {cout<<l<<"<>"<<dex<<endl;return -1;}//越界
	}
	
	return DNumtoBStr(dex,BStr,size);
	/*
	char Temp[COMMOM_SIZE]={'\0'}; 
	if(idx==size)//截取
	{
		for(int i=0;i<size;i++)	Temp[i]=Str[i+1];
	}
	else   //扩展 
	{
		for(int i=size-1;i>=0;i--)
		{
			if(idx!=-1) Temp[i]=Str[idx--];
			else Temp[i]='0';
		}
	}
	//转换
	for(int i=0;i<size;i++)
	{
		switch(Temp[i])
		{
		case '0':
			strcat(BStr,"0000");
			break;
		case '1':
			strcat(BStr,"0001");
			break;
		case '2':
			strcat(BStr,"0010");
			break;
		case '3':
			strcat(BStr,"0011");
			break;
		case '4':
			strcat(BStr,"0100");
			break;
		case '5':
			strcat(BStr,"0101");
			break;
		case '6':
			strcat(BStr,"0110");
			break;
		case '7':
			strcat(BStr,"0111");
			break;
		case '8':
			strcat(BStr,"1000");
			break;
		case '9':
			strcat(BStr,"1001");
			break;
		case 'a':
			strcat(BStr,"1010");
			break;
		case 'b':
			strcat(BStr,"1011");
			break;
		case 'c':
			strcat(BStr,"1100");
			break;
		case 'd':
			strcat(BStr,"1101");
			break;
		case 'e':
			strcat(BStr,"1110");
			break;
		case 'f':
			strcat(BStr,"1111");
			break;
		}
	}
	return 0;
	*/
}
#endif