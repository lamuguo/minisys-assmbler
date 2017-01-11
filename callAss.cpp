#include "assembler.h"
#include <iostream>
#include <string>
using namespace std;

void callAssemblerParser(string srcFile, string errFile)
{
    //cout<<"初始化"<<endl;

    //cout<<"asm file: "<<srcFile<<endl;

    assembler asmParser(srcFile,1);

    //cout<<"初始化结束，开始Parser"<<endl;

    //cout<<"asm error file: "<<errFile<<endl;

    asmParser.Parser(errFile);

}
