/*
 * sw complier program implemented in C
 *
 * The program has been tested on CodeBlocks
 *
 * ʹ�÷�����
 * ���к�����swԴ�����ļ���
 * �ش��Ƿ�������������
 * �ش��Ƿ�������ű�
 * fcode.txt������������
 * foutput.txt���Դ�ļ��͸��ж�Ӧ�����ɴ����׵�ַ�����޴�
 * fresult.txt������н��
 * ftable.txt������ű�
 * ferror.txt����������д�
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compile.h"


/* ������ʼ */
int main(int argc, char *argv[]) {
    //printf("������sw�����ļ���: ");
    //scanf("%s", fname);		/* �����ļ��� */
    strcpy(fname, argv[1]);
    if ((fin = fopen(fname, "r")) == NULL) {
        printf("�޷��򿪸��ļ�!\n");
        exit(1);
    }

    compile();

    return 0;
}

