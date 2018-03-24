/*
 * sw complier program implemented in C
 *
 * The program has been tested on CodeBlocks
 *
 * 使用方法：
 * 运行后输入sw源程序文件名
 * 回答是否输出虚拟机代码
 * 回答是否输出符号表
 * fcode.txt输出虚拟机代码
 * foutput.txt输出源文件和各行对应的生成代码首地址（如无错）
 * fresult.txt输出运行结果
 * ftable.txt输出符号表
 * ferror.txt输出错误（如有错）
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compile.h"


/* 主程序开始 */
int main(int argc, char *argv[]) {
    //printf("请输入sw代码文件名: ");
    //scanf("%s", fname);		/* 输入文件名 */
    strcpy(fname, argv[1]);
    if ((fin = fopen(fname, "r")) == NULL) {
        printf("无法打开该文件!\n");
        exit(1);
    }

    compile();

    return 0;
}

