#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compile.h"

/*
 * ��ʼ��
 */
void init() {
    /* ���õ��ַ����� */
    memset(ssym, 0, sizeof ssym);
    ssym['%'] = mod;
    ssym['+'] = plus;
    ssym['-'] = minus;
    ssym['*'] = times;
    ssym['/'] = slash;
    ssym['('] = lparen;
    ssym[')'] = rparen;
    ssym['{'] = lbrace;
    ssym['}'] = rbrace;
    ssym['='] = becomes;
    ssym['$'] = period;
    ssym[';'] = semicolon;

    /* ���ñ���������,������ĸ˳�򣬱��ڶ��ֲ��� */
    strcpy(word[0], "call");
    strcpy(word[1], "const");
    strcpy(word[2], "else");
    strcpy(word[3], "for");
    strcpy(word[4], "func");
    strcpy(word[5], "if");
    strcpy(word[6], "in");
    strcpy(word[7], "odd");
    strcpy(word[8], "print");
    strcpy(word[9], "read");
    strcpy(word[10], "repeat");
    strcpy(word[11], "var");
    strcpy(word[12], "while");

    /* ���ñ����ַ��� */
    wsym[0] = callsym;
    wsym[1] = constsym;
    wsym[2] = elsesym;
    wsym[3] = forsym;
    wsym[4] = funcsym;
    wsym[5] = ifsym;
    wsym[6] = insym;
    wsym[7] = oddsym;
    wsym[8] = printsym;
    wsym[9] = readsym;
    wsym[10] = repeatsym;
    wsym[11] = varsym;
    wsym[12] = whilesym;

    /* ����ָ������ */
    strcpy(mnemonic[lit], "lit");
    strcpy(mnemonic[opr], "opr");
    strcpy(mnemonic[lod], "lod");
    strcpy(mnemonic[sto], "sto");
    strcpy(mnemonic[cal], "cal");
    strcpy(mnemonic[ini], "int");
    strcpy(mnemonic[jmp], "jmp");
    strcpy(mnemonic[jpc], "jpc");
    strcpy(mnemonic[jeq], "jeq");

    /* ���÷��ż� */
    memset(declbegsys, false, sizeof declbegsys);
    memset(statbegsys, false, sizeof statbegsys);
    memset(facbegsys, false, sizeof facbegsys);

    /* ����������ʼ���ż� */
    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[funcsym] = true;

    /* ������俪ʼ���ż� */
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;
    statbegsys[readsym] = true;
    statbegsys[printsym] = true;
    statbegsys[ident] = true;
    statbegsys[forsym] = true;
    statbegsys[repeatsym] = true;

    /* �������ӿ�ʼ���ż� */
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;

    strcpy(errorinfo[0], "�������ź���ȱ�ٱ�ʶ��");
    strcpy(errorinfo[1], "ȱ��';'");
    strcpy(errorinfo[2], "�����ߺ���������ʼ���Ŵ���");
    strcpy(errorinfo[3], "ȱ����俪ʼ����");
    strcpy(errorinfo[4], "ȱ������������");
    strcpy(errorinfo[5], "����δ���");
    strcpy(errorinfo[6], "��ʶ��δ����");
    strcpy(errorinfo[7], "��ֵ���Ⱥ���಻�Ǳ���");
    strcpy(errorinfo[8], "ȱ�ٸ�ֵ����");
    strcpy(errorinfo[9], "ȱ�ٷֺ�");
    strcpy(errorinfo[10], "���������Ŵ���");
    strcpy(errorinfo[11], "ȱ�ٹ�ϵ�����");
    strcpy(errorinfo[12], "��ʶ�������Ǻ���");
    strcpy(errorinfo[13], "���ӿ�ʼ���Ŵ���");
    strcpy(errorinfo[14], "����λ��������Χ");
    strcpy(errorinfo[15], "���ִ�С������Χ");
    strcpy(errorinfo[16], "ȱ��')'");
    strcpy(errorinfo[17], "ȱ��'('");
    strcpy(errorinfo[18], "����δ����");
    strcpy(errorinfo[19], "function����ȱ��'()'");
    strcpy(errorinfo[20], "ȱ��'{'");
    strcpy(errorinfo[21], "ȱ��'}'");
    strcpy(errorinfo[22], "ȱ��'in'");
    strcpy(errorinfo[23], "forѭ��ȱ����߽�");
    strcpy(errorinfo[24], "forѭ��ȱ���ұ߽�");
    strcpy(errorinfo[25], "forѭ��ȱ��'...'");
    strcpy(errorinfo[30], "����Ƕ�ײ�������");
    strcpy(errorinfo[31], "call��������ȱ�ٹ�����");
    strcpy(errorinfo[32], "���������е�=��Ӧ������");
    strcpy(errorinfo[33], "repeat��ȱ��while");

    err = 0;
    cc = ll = cx = 0;
    ch = ' ';
    linenum = 0;

    foutput = fopen("foutput.txt", "w");
    ftable = fopen("ftable.txt", "w");
    ferrors = fopen("ferror.txt", "w");
    fcode = fopen("fcode.txt", "w");
    fresult = fopen("fresult.txt", "w");
    fdebug = fopen("fdebug.txt", "w");
}

void compile() {
    bool nxtlev[symnum];
    init();		/* ��ʼ�� */

    getsym();

    addset(nxtlev, declbegsys, statbegsys, symnum);
    nxtlev[period] = true;
    block(0, 0, nxtlev);	/* ����ֳ��� */

    if (sym != period) {
        error(5);
    }

    if (err == 0) {
        fprintf(foutput,"\n===����ɹ�!===\n");
    } else {
        fprintf(ferrors,"\n�����빲�� %d ������!\n",err);
    }

    debug = true;
    listall();	 /* ������д��� */
    interpret();	/* ���ý���ִ�г��� */

    fclose(fcode);
    fclose(ftable);
    fclose(foutput);
    fclose(fin);
    fclose(fresult);
}

/*
 * ������ʵ�ּ��ϵļ�������
 */
int inset(int e, bool* s) {
    return s[e];
}

void addset(bool* sr, bool* s1, bool* s2, int n) {
    for (int i = 0; i < n; i++) {
        sr[i] = s1[i] || s2[i];
    }
}

void subset(bool* sr, bool* s1, bool* s2, int n) {
    for (int i = 0; i < n; i++) {
        sr[i] = s1[i] && (!s2[i]);
    }
}

void mulset(bool* sr, bool* s1, bool* s2, int n) {
    for (int i = 0; i < n; i++) {
        sr[i] = s1[i] && s2[i];
    }
}

/*
 *	��������ӡ����λ�úʹ������
 */
void error(int n) {
    fprintf(ferrors, "�� %d �� ����: %s\n", linenum, errorinfo[n]);
    err = err + 1;
    if (err > maxerr) {
        exit(1);
    }
}

/*
 * ���˿ո񣬶�ȡһ���ַ�
 * ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ��
 * ������getsym����
 */
void getch() {
    if (cc == ll) { /* �жϻ��������Ƿ����ַ��������ַ����������һ���ַ����������� */
        if (feof(fin)) {
            ch = '$';
            return;
        }
        ll = 0;
        cc = 0;
        linenum++;
        fprintf(foutput, "%d ", cx);
        ch = ' ';
        while (ch != 10) {
            if (EOF == fscanf(fin,"%c", &ch)) {
                line[ll] = 0;
                break;
            }
            fprintf(foutput, "%c", ch);
            line[ll] = ch;
            ll++;
        }
    }
    ch = line[cc];
    cc++;
}

/*
 * �ʷ���������ȡһ������
 */
void getsym() {
    int i,j,k;

    while (ch == ' ' || ch == 10 || ch == 9) {	/* ���˿ո񡢻��к��Ʊ�� */
        getch();
    }
    if (ch == '$') sym = period;
    else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) { /* ��ǰ�ĵ����Ǳ�ʶ�����Ǳ����� */
        k = 0;
        do {
            if(k < al) {
                a[k] = ch;
                k++;
            }
            getch();
        } while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
        a[k] = 0;
        strcpy(id, a);
        i = 0;
        j = norw - 1;
        do {    /* ������ǰ�����Ƿ�Ϊ�����֣�ʹ�ö��ַ����� */
            k = (i + j) / 2;
            if (strcmp(id,word[k]) <= 0) {
                j = k - 1;
            }
            if (strcmp(id,word[k]) >= 0) {
                i = k + 1;
            }
        } while (i <= j);
        if (i-1 > j) { /* ��ǰ�ĵ����Ǳ����� */
            sym = wsym[k];
        } else { /* ��ǰ�ĵ����Ǳ�ʶ�� */
            sym = ident;
        }
    } else {
        if (ch >= '0' && ch <= '9') { /* ��ǰ�ĵ��������� */
            k = 0;
            num = 0;
            sym = number;
            do {
                num = 10 * num + ch - '0';
                k++;
                getch();;
            } while (ch >= '0' && ch <= '9'); /* ��ȡ���ֵ�ֵ */
            k--;
            if (k > nmax) { /* ����λ��̫�� */
                error(14);
            }
        } else {
            if (ch == '.') {	/* ���...���� */
                getch();
                if (ch == '.') {
                    getch();
                    if (ch == '.') {
                        sym = range;
                        getch();
                    } else {
                        sym = nul;
                    }
                } else {
                    sym = nul;	/* ����ʶ��ķ��� */
                }
            } else if (ch == '<') {
                getch();
                if (ch == '=') {
                    sym = leq;
                    getch();
                } else {
                    sym = lss;
                }
            } else if (ch == '>') {
                getch();
                if (ch == '=') {
                    sym = geq;
                    getch();
                } else {
                    sym = gtr;
                }
            } else if (ch == '=') {
                getch();
                if (ch == '=') {
                    sym = eql;
                    getch();
                } else
                    sym = becomes;
            } else if (ch == '!') {
                getch();
                if (ch == '=') {
                    sym = neq;
                    getch();
                } else
                    sym = nul;
            } else if (ch == '/') {
                getch();
                if (ch == '/') {    //��ע��
                    cc = ll;
                    getch();
                    getsym();
                } else if (ch == '*') { //��ע��
                    int flag = 0;
                    while (ch != '$') {
                        getch();
                        if (ch == '*') flag = 1;
                        else if (ch == '/' && flag == 1) {
                            getch();
                            getsym();
                            break;
                        } else {
                            flag = 0;
                        }
                    }
                    if (ch == '$') sym = period;
                } else {    //����
                    sym = slash;
                    getch();
                }
            } else if (ch == '+') {
                getch();
                if (ch == '+') {
                    sym = add;
                    getch();
                } else {
                    sym = plus;
                }
            } else if (ch == '-') {
                getch();
                if (ch == '-') {
                    sym = sub;
                    getch();
                } else {
                    sym = minus;
                }
            } else {
                sym = ssym[ch];
                if (sym != period)
                    getch();
            }
        }
    }
}

/*
 * �������������
 *
 * x: instruction.f;
 * y: instruction.l;
 * z: instruction.a;
 */
void gen(enum fct x, int y, int z ) {
    if (cx >= cxmax) {
        printf("���ɵ����������������!\n");	/* ���ɵ���������������� */
        exit(1);
    }
    if ( z >= addrmax) {
        printf("��ַƫ��Խ��!\n");	/* ��ַƫ��Խ�� */
        exit(1);
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
}


/*
 * ���Ե�ǰ�����Ƿ�Ϸ�
 *
 * ���﷨�����������ںͳ��ڴ����ò��Ժ���test��
 * ��鵱ǰ���ʽ�����˳����﷨��λ�ĺϷ���
 *
 * s1:	��Ҫ�ĵ��ʼ���
 * s2:	���������Ҫ�ĵ��ʣ���ĳһ����״̬ʱ��
 *      �ɻָ��﷨�����������������Ĳ��䵥�ʷ��ż���
 * n:  	�����
 */
void test(bool* s1, bool* s2, int n) {
    if (!inset(sym, s1)) {
        error(n);
        /* ����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ��� */
        while ((!inset(sym,s1)) && (!inset(sym,s2))) {
            getsym();
        }
    }
}

/*
 * �����������
 *
 * lev:    ��ǰ�ֳ������ڲ�
 * tx:     ���ű�ǰβָ��
 * fsys:   ��ǰģ���̷��ż���
 */
void block(int lev, int tx, bool* fsys) {
    int i;

    int dx;                 /* ��¼���ݷ������Ե�ַ */
    int tx0;                /* ������ʼtx */
    int cx0;                /* ������ʼcx */
    bool nxtlev[symnum];    /* ���¼������Ĳ����У����ż��Ͼ�Ϊֵ�Σ�������ʹ������ʵ�֣�
	                           ���ݽ�������ָ�룬Ϊ��ֹ�¼������ı��ϼ������ļ��ϣ������µĿռ�
	                           ���ݸ��¼�����*/

    dx = 3;                 /* �����ռ����ڴ�ž�̬��SL����̬��DL�ͷ��ص�ַRA  */
    tx0 = tx;		        /* ��¼�����ʶ���ĳ�ʼλ�� */
    table[tx].adr = cx;	    /* ��¼��ǰ�����Ŀ�ʼλ�� */
    gen(jmp, 0, 0);         /* ������תָ���תλ��δ֪��ʱ��0 */

    if (lev > 1) {	/* Ƕ�ײ������� */
        error(30);
    }
    if (sym == nul) sym = period;
    do {

        while (sym == varsym || sym == constsym) {	/* ���������������ţ���ʼ����������� */
            if (sym == varsym) {
                getsym();
                vardeclaration(&tx, lev, &dx);
                if (sym == semicolon) {
                    getsym();
                } else {
                    error(9); /* ©���˷ֺ� */
                }
            } else {
                getsym();
                constdeclaration(&tx, lev, &dx);	/* dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��ָ�� */
                if (sym == semicolon) { /* �����ֺŽ������峣�� */
                    getsym();
                } else {
                    error(9);   /* ©���˷ֺ� */
                }
            }
        }

        while (sym == funcsym) { /* ���������������ţ���ʼ����������� */
            getsym();

            if (sym == ident) {
                enter(function, &tx, lev, &dx);	/* ��д���ű� */
                getsym();
            } else {
                error(0);	/* procedure��ӦΪ��ʶ�� */
            }

            if (sym == lparen) getsym();
            else error(19);
            if (sym == rparen) getsym();
            else error(19);
            if (sym == lbrace) getsym();
            else error(20);

            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[rbrace] = true;
            block(lev + 1, tx, nxtlev); /* �ݹ���� */

            if(sym == rbrace) {
                getsym();
                memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
                nxtlev[funcsym] = true;
                test(nxtlev, fsys, 6);
            } else {
                error(38);	/* ©����'}' */
            }
        }
        if (sym == period) break;
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        test(nxtlev, declbegsys, 7);
    } while (inset(sym, declbegsys));	/* ֱ��û���������� */

    code[table[tx0].adr].a = cx;	/* ��ǰ�����ɵ���ת������תλ�øĳɵ�ǰλ�� */
    table[tx0].adr = cx;	        /* ��¼��ǰ���̴����ַ */
    table[tx0].size = dx;	        /* ����������ÿ����һ�����������dx����1�����������Ѿ�������dx���ǵ�ǰ�������ݵ�size */
    cx0 = cx;
    gen(ini, 0, dx);	            /* ����ָ���ָ��ִ��ʱ������ջ��Ϊ�����õĹ��̿���dx����Ԫ�������� */

    for (i = 1; i <= tx; i++) {
        switch (table[i].kind) {
        case constant:
            fprintf(ftable, "%d const %s ", i, table[i].name);
            fprintf(ftable, "val=%d\n", table[i].val);
            break;
        case variable:
            fprintf(ftable, "%d var   %s ", i, table[i].name);
            fprintf(ftable, "lev=%d addr=%d\n", table[i].level, table[i].adr);
            break;
        case function:
            fprintf(ftable,"%d func  %s ", i, table[i].name);
            fprintf(ftable,"lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
            break;
        }
    }
    fprintf(ftable,"\n");
    /* ����̷���Ϊ�ֺŻ�end */
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);	/* ÿ����̷��ż��϶������ϲ��̷��ż��ϣ��Ա㲹�� */
    nxtlev[semicolon] = true;
    statement(nxtlev, &tx, lev);
    gen(opr, 0, 0);	                    /* ÿ�����̳��ڶ�Ҫʹ�õ��ͷ����ݶ�ָ�� */
    memset(nxtlev, 0, sizeof(bool) * symnum);	/* �ֳ���û�в��ȼ��� */
    test(fsys, nxtlev, 8);            	/* ����̷�����ȷ�� */
}

/*
 * �ڷ��ű��м���һ��
 *
 * k:      ��ʶ��������Ϊconst��var��procedure
 * ptx:    ���ű�βָ���ָ�룬Ϊ�˿��Ըı���ű�βָ���ֵ
 * lev:    ��ʶ�����ڵĲ��
 * pdx:    dxΪ��ǰӦ����ı�������Ե�ַ�������Ҫ����1
 *
 */
void enter(enum object k, int* ptx,	int lev, int* pdx) {
    (*ptx)++;
    strcpy(table[(*ptx)].name, id); /* ���ű��name���¼��ʶ�������� */
    table[(*ptx)].kind = k;
    switch (k) {
    case constant:	/* ���� */
        if (num > addrmax) {
            error(15);	/* ����Խ�� */
            num = 0;
        }
        table[(*ptx)].val = num; /* �Ǽǳ�����ֵ */
        break;
    case variable:	/* ���� */
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        (*pdx)++;
        break;
    case function:	/* ���� */
        table[(*ptx)].level = lev;
        break;
    }
}

/*
 * ���ұ�ʶ���ڷ��ű��е�λ�ã���tx��ʼ������ұ�ʶ��
 * �ҵ��򷵻��ڷ��ű��е�λ�ã����򷵻�0
 *
 * id:    Ҫ���ҵ�����
 * tx:    ��ǰ���ű�βָ��
 */
int position(char* id, int tx) {
    int i;
    strcpy(table[0].name, id);
    i = tx;
    while (strcmp(table[i].name, id) != 0) {
        i--;
    }
    return i;
}

/*
 * ������������
 */
void constdeclaration(int* ptx, int lev, int* pdx) {
    if (sym == ident) {
        getsym();
        if (sym == becomes) {
            getsym();
            if (sym == number) {
                enter(constant, ptx, lev, pdx);
                getsym();
            } else {
                error(32);	/* ���������е�=��Ӧ������ */
            }
        } else {
            error(8);	/* ���������еı�ʶ����Ӧ��= */
        }
    } else {
        error(0);	/* const��Ӧ�Ǳ�ʶ�� */
    }
}

/*
 * ������������
 */
void vardeclaration(int* ptx,int lev,int* pdx) {
    if (sym == ident) {
        enter(variable, ptx, lev, pdx);	// ��д���ű�
        getsym();
    } else {
        error(0);	/* var����Ӧ�Ǳ�ʶ�� */
    }
}

/*
 * �������Ŀ�����
 */
void listall() {
    int i;
    for (i = 0; i < cx; i++) {
        fprintf(fcode,"%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
    }
}

/*
 * ��䴦��
 */
void statement(bool* fsys, int* ptx, int lev) {
    int i, cx1, cx2;
    bool nxtlev[symnum];

    while (inset(sym, statbegsys)) {
        if (sym == ident) {	/* ׼�����ո�ֵ��䴦�� */
            i = position(id, *ptx);/* ���ұ�ʶ���ڷ��ű��е�λ�� */
            if (i == 0) {
                error(6);	/* ��ʶ��δ���� */
            } else {
                if(table[i].kind != variable) {
                    error(7);	/* ��ֵ����У���ֵ���󲿱�ʶ��Ӧ���Ǳ��� */
                    i = 0;
                } else {
                    getsym();
                    if (sym == add) {
                        gen(lod, lev-table[i].level, table[i].adr);
                        gen(lit, 0, 1);
                        gen(opr, 0, 2);
                        gen(sto, lev-table[i].level, table[i].adr);
                        getsym();
                    } else if (sym == sub) {
                        gen(lod, lev-table[i].level, table[i].adr);
                        gen(lit, 0, 1);
                        gen(opr, 0, 3);
                        gen(sto, lev-table[i].level, table[i].adr);
                        getsym();
                    } else {
                        if(sym == becomes) {
                            getsym();
                        } else {
                            error(8);	/* û�м�⵽��ֵ���� */
                        }
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        expression(nxtlev, ptx, lev);	/* ����ֵ�����Ҳ���ʽ */
                        if(i != 0) {
                            /* expression��ִ��һϵ��ָ������ս�����ᱣ����ջ����ִ��sto������ɸ�ֵ */
                            gen(sto, lev-table[i].level, table[i].adr);
                        }
                    }
                }
            }
        } else {
            if (sym == readsym) {	/* ׼������read��䴦�� */
                getsym();
                if (sym != lparen) {
                    error(17);	/* ��ʽ����Ӧ�������� */
                } else {
                    getsym();
                    if (sym == ident) {
                        i = position(id, *ptx);	/* ����Ҫ���ı��� */
                    } else {
                        i = 0;
                    }

                    if (i == 0) {
                        error(18);	/* read��������еı�ʶ��Ӧ�����������ı��� */
                    } else {
                        gen(opr, 0, 16);	/* ��������ָ���ȡֵ��ջ�� */
                        gen(sto, lev-table[i].level, table[i].adr);	/* ��ջ���������������Ԫ�� */
                    }
                    getsym();
                }
                if(sym != rparen) {
                    error(16);	/* ��ʽ����Ӧ�������� */
                    while (!inset(sym, fsys)) {	/* �����ȣ�ֱ�������ϲ㺯���ĺ�̷��� */
                        getsym();
                    }
                } else {
                    getsym();
                }
            } else {
                if (sym == printsym) {	/* ׼������write��䴦�� */
                    getsym();
                    if (sym == lparen) {
                        getsym();
                        if (sym == ident)
                            i = position(id, *ptx);
                        else i = 0;

                        if (i == 0) error(18);
                        else {
                            memcpy(nxtlev, fsys, sizeof nxtlev);
                            nxtlev[rparen] = true;
                            expression(nxtlev, ptx, lev);
                            gen(opr, 0, 14);
                            gen(opr, 0, 15);
                        }
                        if (sym != rparen) {
                            error(16);	/* ��ʽ����Ӧ�������� */
                        } else {
                            getsym();
                        }
                    }

                } else {
                    if (sym == callsym) {	/* ׼������call��䴦�� */
                        getsym();
                        if (sym != ident) {
                            error(0);	/* call��ӦΪ��ʶ�� */
                        } else {
                            i = position(id, *ptx);
                            if (i == 0) {
                                error(6);	/* ������δ�ҵ� */
                            } else {
                                if (table[i].kind == function) {
                                    gen(cal, lev-table[i].level, table[i].adr);	/* ����callָ�� */
                                } else {
                                    error(31);	/* call���ʶ������ӦΪ���� */
                                }
                            }
                            getsym();
                            if (sym == lparen) getsym();
                            else error(19);
                            if (sym == rparen) getsym();
                            else error(19);
                        }
                    } else {
                        if (sym == ifsym) {	/* ׼������if��䴦�� */
                            getsym();
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[lbrace] = true;
                            condition(nxtlev, ptx, lev); /* ������������ */
                            if (sym == lbrace) {
                                getsym();
                            } else {
                                error(20);
                            }
                            cx1 = cx;	/* ���浱ǰָ���ַ */
                            gen(jpc, 0, 0);	/* ����������תָ���ת��ַδ֪����ʱд0 */
                            nxtlev[rbrace] = true;
                            nxtlev[elsesym] = true;
                            statement(fsys, ptx, lev);	/* ����then������ */
                            cx2 = cx;
                            gen(jmp, 0, 0);
                            code[cx1].a = cx;
                            code[cx1].a = cx;	/* ��statement�����cxΪthen�����ִ�����λ�ã�������ǰ��δ������ת��ַ����ʱ���л��� */
                            if (sym == rbrace) getsym();
                            else error(21);
                            if (sym == elsesym) {
                                getsym();
                                if (sym == lbrace) getsym();
                                else error(20);

                                memcpy(nxtlev, fsys, sizeof nxtlev);
                                nxtlev[rbrace] = true;
                                statement(nxtlev, ptx, lev);

                                if (sym == rbrace) getsym();
                                else error(21);
                            }
                            code[cx2].a = cx;
                        } else {
                            if (sym == whilesym) {	/* ׼������while��䴦�� */
                                cx1 = cx;	/* �����ж�����������λ�� */
                                getsym();
                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[lbrace] = true;	/* ��̷���Ϊdo */
                                condition(nxtlev, ptx, lev);	/* ������������ */
                                cx2 = cx;	/* ����ѭ����Ľ�������һ��λ�� */
                                gen(jpc, 0, 0);	/* ����������ת��������ѭ���ĵ�ַδ֪�����Ϊ0�ȴ����� */
                                if (sym == lbrace) {
                                    getsym();
                                } else {
                                    error(20);	/* ȱ��do */
                                }
                                memcpy(nxtlev, fsys, sizeof nxtlev);
                                nxtlev[rbrace] = true;
                                statement(fsys, ptx, lev);	/* ѭ���� */
                                if (sym == rbrace) getsym();
                                else error(21);
                                gen(jmp, 0, cx1);	/* ����������תָ���ת��ǰ���ж�����������λ�� */
                                code[cx2].a = cx;	/* ��������ѭ���ĵ�ַ */
                            } else if (sym == forsym) {
                                getsym();
                                if (sym == ident) {
                                    i = position(id, *ptx);
                                    if (i == 0) error(6);
                                    else {
                                        if (table[i].kind != variable) {
                                            error(7);
                                            i = 0;
                                        } else {
                                            getsym();
                                            if (sym != insym) error(22);
                                            else getsym();


                                            memcpy(nxtlev, fsys, sizeof nxtlev);
                                            nxtlev[range] = true;
                                            expression(nxtlev, ptx, lev);
                                            gen(sto, lev-table[i].level, table[i].adr);

                                            if (sym != range) error(25);
                                            else getsym();

                                            cx1 = cx;
                                            gen(lod, lev-table[i].level, table[i].adr);
                                            memcpy(nxtlev, fsys, sizeof nxtlev);
                                            nxtlev[lbrace] = true;
                                            expression(nxtlev, ptx, lev);

                                            gen(opr, 0, 13);

                                            cx2 = cx;
                                            gen(jpc, 0, 0);

                                            if (sym == lbrace)
                                                getsym();
                                            else error(20);

                                            memcpy(nxtlev, fsys, sizeof nxtlev);
                                            nxtlev[rbrace] = true;
                                            statement(nxtlev, ptx, lev);

                                            if (sym == rbrace)
                                                getsym();
                                            else error(21);


                                            gen(lod, lev-table[i].level, table[i].adr);
                                            gen(lit, 0, 1);
                                            gen(opr, 0, 2);

                                            gen(sto, lev-table[i].level, table[i].adr);
                                            gen(jmp, 0, cx1);
                                            code[cx2].a = cx;
                                        }
                                    }
                                }
                            } else if (sym == repeatsym) {
                                getsym();
                                if (sym == lbrace) getsym();
                                else error(20);

                                cx1 = cx;
                                memcpy(nxtlev, fsys, sizeof nxtlev);
                                nxtlev[rbrace] = true;
                                statement(nxtlev, ptx, lev);

                                printf("%d\n", sym);
                                if (sym == rbrace) getsym();
                                else error(21);
                                printf("%d\n", sym);
                                if (sym == whilesym) getsym();
                                else error(33);

                                if (sym == lparen) getsym();
                                else error(17);

                                memcpy(nxtlev, fsys, sizeof nxtlev);
                                nxtlev[rparen] = true;
                                condition(nxtlev, ptx, lev);
                                gen(jeq, 0, cx1);

                                if (sym == rparen) getsym();
                                else error(16);
                            }
                        }
                    }
                }
            }
        }
        if (sym == semicolon)
            getsym();
        else error(1);
    }
}

/*
 * ���ʽ����
 */
void expression(bool* fsys, int* ptx, int lev) {
    enum symbol addop;	/* ���ڱ��������� */
    bool nxtlev[symnum];

    if(sym == plus || sym == minus) {	/* ���ʽ��ͷ�������ţ���ʱ��ǰ���ʽ������һ�����Ļ򸺵��� */
        addop = sym;	/* ���濪ͷ�������� */
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        term(nxtlev, ptx, lev);	/* ������ */
        if (addop == minus) {
            gen(opr,0,1);	/* �����ͷΪ��������ȡ��ָ�� */
        }
    } else {	/* ��ʱ���ʽ��������ļӼ� */
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        term(nxtlev, ptx, lev);	/* ������ */
    }
    while (sym == plus || sym == minus) {
        addop = sym;
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        term(nxtlev, ptx, lev);	/* ������ */
        if (addop == plus) {
            gen(opr, 0, 2);	/* ���ɼӷ�ָ�� */
        } else {
            gen(opr, 0, 3);	/* ���ɼ���ָ�� */
        }
    }
}

/*
 * ���
 */
void term(bool* fsys, int* ptx, int lev) {
    enum symbol mulop;	/* ���ڱ���˳������� */
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    nxtlev[mod] = true;
    factor(nxtlev, ptx, lev);	/* �������� */
    while(sym == times || sym == slash || sym == mod) {
        mulop = sym;
        getsym();
        factor(nxtlev, ptx, lev);
        if(mulop == times) {
            gen(opr, 0, 4);	/* ���ɳ˷�ָ�� */
        } else if (mulop == slash) {
            gen(opr, 0, 5);	/* ���ɳ���ָ�� */
        } else {
            gen(opr, 0, 17);
        }
    }
}

/*
 * ���Ӵ���
 */
void factor(bool* fsys, int* ptx, int lev) {
    int i;
    bool nxtlev[symnum];
    test(facbegsys, fsys, 24);	/* ������ӵĿ�ʼ���� */
    while(inset(sym, facbegsys)) {	/* ѭ���������� */
        if(sym == ident) {	/* ����Ϊ��������� */
            i = position(id, *ptx);	/* ���ұ�ʶ���ڷ��ű��е�λ�� */
            if (i == 0) {
                error(6);	/* ��ʶ��δ���� */
            } else {
                switch (table[i].kind) {
                case constant:	/* ��ʶ��Ϊ���� */
                    gen(lit, 0, table[i].val);	/* ֱ�Ӱѳ�����ֵ��ջ */
                    break;
                case variable:	/* ��ʶ��Ϊ���� */
                    gen(lod, lev-table[i].level, table[i].adr);	/* �ҵ�������ַ������ֵ��ջ */
                    break;
                case function:	/* ��ʶ��Ϊ���� */
                    error(12);	/* ����Ϊ���� */
                    break;
                }
            }
            getsym();
        } else {
            if(sym == number) {	/* ����Ϊ�� */
                if (num > addrmax) {
                    error(15); /* ��Խ�� */
                    num = 0;
                }
                gen(lit, 0, num);
                getsym();
            } else {
                if (sym == lparen) {	/* ����Ϊ���ʽ */
                    getsym();
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[rparen] = true;
                    expression(nxtlev, ptx, lev);
                    if (sym == rparen) {
                        getsym();
                    } else {
                        error(16);	/* ȱ�������� */
                    }
                }
            }
        }
        memset(nxtlev, 0, sizeof(bool) * symnum);
        nxtlev[lparen] = true;
        test(fsys, nxtlev, 4); /* һ�����Ӵ�����ϣ������ĵ���Ӧ��fsys������ */
        /* ������ǣ������ҵ���һ�����ӵĿ�ʼ��ʹ�﷨�������Լ���������ȥ */
    }
}

/*
 * ��������
 */
void condition(bool* fsys, int* ptx, int lev) {
    enum symbol relop;
    bool nxtlev[symnum];
    if(sym == oddsym) {	/* ׼������odd���㴦�� */
        getsym();
        expression(fsys, ptx, lev);
        gen(opr, 0, 6);	/* ����oddָ�� */
        return;
    }
    /* �߼����ʽ���� */
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[eql] = true;
    nxtlev[neq] = true;
    nxtlev[lss] = true;
    nxtlev[leq] = true;
    nxtlev[gtr] = true;
    nxtlev[geq] = true;
    expression(nxtlev, ptx, lev);
    if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq) {
        error(11); /* Ӧ��Ϊ��ϵ����� */
    } else {
        relop = sym;
        getsym();
        expression(fsys, ptx, lev);
        switch (relop) {
        case eql:
            gen(opr, 0, 8);
            break;
        case neq:
            gen(opr, 0, 9);
            break;
        case lss:
            gen(opr, 0, 10);
            break;
        case geq:
            gen(opr, 0, 11);
            break;
        case gtr:
            gen(opr, 0, 12);
            break;
        case leq:
            gen(opr, 0, 13);
            break;
        }
    }
}

/*
 * ���ͳ���
 */
void interpret() {
    int p = 0; /* ָ��ָ�� */
    int b = 1; /* ָ���ַ */
    int t = 0; /* ջ��ָ�� */
    struct instruction i;	/* ��ŵ�ǰָ�� */
    int s[stacksize];	/* ջ */

    s[0] = 0; /* s[0]���� */
    s[1] = 0; /* �������������ϵ��Ԫ����Ϊ0 */
    s[2] = 0;
    s[3] = 0;

    do {
        if (cx == 0) break;
        i = code[p];	/* ����ǰָ�� */
        fprintf(fdebug, "#\n");
        fprintf(fdebug, "%d\n", p);
        p = p + 1;
        switch (i.f) {
        case lit:	/* ������a��ֵȡ��ջ�� */
            t = t + 1;
            s[t] = i.a;
            break;
        case opr:	/* ��ѧ���߼����� */
            switch (i.a) {
            case 0:  /* �������ý����󷵻� */
                t = b - 1;
                p = s[t + 3];
                b = s[t + 2];
                break;
            case 1: /* ջ��Ԫ��ȡ�� */
                s[t] = - s[t];
                break;
            case 2: /* ��ջ�������ջ���������ջԪ�أ����ֵ��ջ */
                t = t - 1;
                s[t] = s[t] + s[t + 1];
                break;
            case 3:/* ��ջ�����ȥջ���� */
                t = t - 1;
                s[t] = s[t] - s[t + 1];
                break;
            case 4:/* ��ջ�������ջ���� */
                t = t - 1;
                s[t] = s[t] * s[t + 1];
                break;
            case 5:/* ��ջ�������ջ���� */
                t = t - 1;
                s[t] = s[t] / s[t + 1];
                break;
            case 6:/* ջ��Ԫ�ص���ż�ж� */
                s[t] = s[t] % 2;
                break;
            case 8:/* ��ջ������ջ�����Ƿ���� */
                t = t - 1;
                s[t] = (s[t] == s[t + 1]);
                break;
            case 9:/* ��ջ������ջ�����Ƿ񲻵� */
                t = t - 1;
                s[t] = (s[t] != s[t + 1]);
                break;
            case 10:/* ��ջ�����Ƿ�С��ջ���� */
                t = t - 1;
                s[t] = (s[t] < s[t + 1]);
                break;
            case 11:/* ��ջ�����Ƿ���ڵ���ջ���� */
                t = t - 1;
                s[t] = (s[t] >= s[t + 1]);
                break;
            case 12:/* ��ջ�����Ƿ����ջ���� */
                t = t - 1;
                s[t] = (s[t] > s[t + 1]);
                break;
            case 13: /* ��ջ�����Ƿ�С�ڵ���ջ���� */
                t = t - 1;
                s[t] = (s[t] <= s[t + 1]);
                break;
            case 14:/* ջ��ֵ��� */
                printf("%d", s[t]);
                fprintf(fresult, "%d", s[t]);
                break;
            case 15:/* ������з� */
                printf("\n");
                fprintf(fresult,"\n");
                break;
            case 16:/* ����һ����������ջ�� */
                t = t + 1;
                scanf("%d", &(s[t]));
                break;
            case 17: //����
                t = t - 1;
                s[t] = s[t] % s[t + 1];
                break;
            }
            break;
        case lod:	/* ȡ��Ե�ǰ���̵����ݻ���ַΪa���ڴ��ֵ��ջ�� */
            t = t + 1;
            s[t] = s[base(i.l,s,b) + i.a];
            break;
        case sto:	/* ջ����ֵ�浽��Ե�ǰ���̵����ݻ���ַΪa���ڴ� */
            s[base(i.l, s, b) + i.a] = s[t];
            t = t - 1;
            break;
        case cal:	/* �����ӹ��� */
            s[t + 1] = base(i.l, s, b);	/* �������̻���ַ��ջ����������̬�� */
            s[t + 2] = b;	/* �������̻���ַ��ջ����������̬�� */
            s[t + 3] = p;	/* ����ǰָ��ָ����ջ�������淵�ص�ַ */
            b = t + 1;	/* �ı����ַָ��ֵΪ�¹��̵Ļ���ַ */
            p = i.a;	/* ��ת */
            break;
        case ini:	/* ������ջ��Ϊ�����õĹ��̿���a����Ԫ�������� */
            t = t + i.a;
            break;
        case jmp:	/* ֱ����ת */
            p = i.a;
            break;
        case jpc:	/* ������ת */
            if (s[t] == 0)
                p = i.a;
            t = t - 1;
            break;
        case jeq:	/* ������ת */
            if (s[t] != 0)
                p = i.a;
            t = t - 1;
            break;
        }
        for (int i = 1; i <= t; ++i) {
            fprintf(fdebug, "%d\n", s[i]);
        }
    } while (p != 0);
}

/* ͨ�����̻�ַ����l����̵Ļ�ַ */
int base(int l, int* s, int b) {
    int b1;
    b1 = b;
    while (l > 0) {
        b1 = s[b1];
        l--;
    }
    return b1;
}
