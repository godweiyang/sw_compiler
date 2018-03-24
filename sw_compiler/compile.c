#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compile.h"

/*
 * 初始化
 */
void init() {
    /* 设置单字符符号 */
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

    /* 设置保留字名字,按照字母顺序，便于二分查找 */
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

    /* 设置保留字符号 */
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

    /* 设置指令名称 */
    strcpy(mnemonic[lit], "lit");
    strcpy(mnemonic[opr], "opr");
    strcpy(mnemonic[lod], "lod");
    strcpy(mnemonic[sto], "sto");
    strcpy(mnemonic[cal], "cal");
    strcpy(mnemonic[ini], "int");
    strcpy(mnemonic[jmp], "jmp");
    strcpy(mnemonic[jpc], "jpc");
    strcpy(mnemonic[jeq], "jeq");

    /* 设置符号集 */
    memset(declbegsys, false, sizeof declbegsys);
    memset(statbegsys, false, sizeof statbegsys);
    memset(facbegsys, false, sizeof facbegsys);

    /* 设置声明开始符号集 */
    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[funcsym] = true;

    /* 设置语句开始符号集 */
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;
    statbegsys[readsym] = true;
    statbegsys[printsym] = true;
    statbegsys[ident] = true;
    statbegsys[forsym] = true;
    statbegsys[repeatsym] = true;

    /* 设置因子开始符号集 */
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;

    strcpy(errorinfo[0], "声明符号后面缺少标识符");
    strcpy(errorinfo[1], "缺少';'");
    strcpy(errorinfo[2], "语句或者函数声明开始符号错误");
    strcpy(errorinfo[3], "缺少语句开始符号");
    strcpy(errorinfo[4], "缺少语句结束符号");
    strcpy(errorinfo[5], "编译未完成");
    strcpy(errorinfo[6], "标识符未声明");
    strcpy(errorinfo[7], "赋值语句等号左侧不是变量");
    strcpy(errorinfo[8], "缺少赋值符号");
    strcpy(errorinfo[9], "缺少分号");
    strcpy(errorinfo[10], "语句结束符号错误");
    strcpy(errorinfo[11], "缺少关系运算符");
    strcpy(errorinfo[12], "标识符不能是函数");
    strcpy(errorinfo[13], "因子开始符号错误");
    strcpy(errorinfo[14], "数字位数超出范围");
    strcpy(errorinfo[15], "数字大小超出范围");
    strcpy(errorinfo[16], "缺少')'");
    strcpy(errorinfo[17], "缺少'('");
    strcpy(errorinfo[18], "变量未声明");
    strcpy(errorinfo[19], "function后面缺少'()'");
    strcpy(errorinfo[20], "缺少'{'");
    strcpy(errorinfo[21], "缺少'}'");
    strcpy(errorinfo[22], "缺少'in'");
    strcpy(errorinfo[23], "for循环缺少左边界");
    strcpy(errorinfo[24], "for循环缺少右边界");
    strcpy(errorinfo[25], "for循环缺少'...'");
    strcpy(errorinfo[30], "函数嵌套层数过多");
    strcpy(errorinfo[31], "call声明后面缺少过程名");
    strcpy(errorinfo[32], "常量声明中的=后应是数字");
    strcpy(errorinfo[33], "repeat后缺少while");

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
    init();		/* 初始化 */

    getsym();

    addset(nxtlev, declbegsys, statbegsys, symnum);
    nxtlev[period] = true;
    block(0, 0, nxtlev);	/* 处理分程序 */

    if (sym != period) {
        error(5);
    }

    if (err == 0) {
        fprintf(foutput,"\n===编译成功!===\n");
    } else {
        fprintf(ferrors,"\n本代码共有 %d 个错误!\n",err);
    }

    debug = true;
    listall();	 /* 输出所有代码 */
    interpret();	/* 调用解释执行程序 */

    fclose(fcode);
    fclose(ftable);
    fclose(foutput);
    fclose(fin);
    fclose(fresult);
}

/*
 * 用数组实现集合的集合运算
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
 *	出错处理，打印出错位置和错误编码
 */
void error(int n) {
    fprintf(ferrors, "第 %d 行 错误: %s\n", linenum, errorinfo[n]);
    err = err + 1;
    if (err > maxerr) {
        exit(1);
    }
}

/*
 * 过滤空格，读取一个字符
 * 每次读一行，存入line缓冲区，line被getsym取空后再读一行
 * 被函数getsym调用
 */
void getch() {
    if (cc == ll) { /* 判断缓冲区中是否有字符，若无字符，则读入下一行字符到缓冲区中 */
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
 * 词法分析，获取一个符号
 */
void getsym() {
    int i,j,k;

    while (ch == ' ' || ch == 10 || ch == 9) {	/* 过滤空格、换行和制表符 */
        getch();
    }
    if (ch == '$') sym = period;
    else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) { /* 当前的单词是标识符或是保留字 */
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
        do {    /* 搜索当前单词是否为保留字，使用二分法查找 */
            k = (i + j) / 2;
            if (strcmp(id,word[k]) <= 0) {
                j = k - 1;
            }
            if (strcmp(id,word[k]) >= 0) {
                i = k + 1;
            }
        } while (i <= j);
        if (i-1 > j) { /* 当前的单词是保留字 */
            sym = wsym[k];
        } else { /* 当前的单词是标识符 */
            sym = ident;
        }
    } else {
        if (ch >= '0' && ch <= '9') { /* 当前的单词是数字 */
            k = 0;
            num = 0;
            sym = number;
            do {
                num = 10 * num + ch - '0';
                k++;
                getch();;
            } while (ch >= '0' && ch <= '9'); /* 获取数字的值 */
            k--;
            if (k > nmax) { /* 数字位数太多 */
                error(14);
            }
        } else {
            if (ch == '.') {	/* 检测...符号 */
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
                    sym = nul;	/* 不能识别的符号 */
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
                if (ch == '/') {    //行注释
                    cc = ll;
                    getch();
                    getsym();
                } else if (ch == '*') { //块注释
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
                } else {    //除号
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
 * 生成虚拟机代码
 *
 * x: instruction.f;
 * y: instruction.l;
 * z: instruction.a;
 */
void gen(enum fct x, int y, int z ) {
    if (cx >= cxmax) {
        printf("生成的虚拟机代码程序过长!\n");	/* 生成的虚拟机代码程序过长 */
        exit(1);
    }
    if ( z >= addrmax) {
        printf("地址偏移越界!\n");	/* 地址偏移越界 */
        exit(1);
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
}


/*
 * 测试当前符号是否合法
 *
 * 在语法分析程序的入口和出口处调用测试函数test，
 * 检查当前单词进入和退出该语法单位的合法性
 *
 * s1:	需要的单词集合
 * s2:	如果不是需要的单词，在某一出错状态时，
 *      可恢复语法分析继续正常工作的补充单词符号集合
 * n:  	错误号
 */
void test(bool* s1, bool* s2, int n) {
    if (!inset(sym, s1)) {
        error(n);
        /* 当检测不通过时，不停获取符号，直到它属于需要的集合或补救的集合 */
        while ((!inset(sym,s1)) && (!inset(sym,s2))) {
            getsym();
        }
    }
}

/*
 * 编译程序主体
 *
 * lev:    当前分程序所在层
 * tx:     符号表当前尾指针
 * fsys:   当前模块后继符号集合
 */
void block(int lev, int tx, bool* fsys) {
    int i;

    int dx;                 /* 记录数据分配的相对地址 */
    int tx0;                /* 保留初始tx */
    int cx0;                /* 保留初始cx */
    bool nxtlev[symnum];    /* 在下级函数的参数中，符号集合均为值参，但由于使用数组实现，
	                           传递进来的是指针，为防止下级函数改变上级函数的集合，开辟新的空间
	                           传递给下级函数*/

    dx = 3;                 /* 三个空间用于存放静态链SL、动态链DL和返回地址RA  */
    tx0 = tx;		        /* 记录本层标识符的初始位置 */
    table[tx].adr = cx;	    /* 记录当前层代码的开始位置 */
    gen(jmp, 0, 0);         /* 产生跳转指令，跳转位置未知暂时填0 */

    if (lev > 1) {	/* 嵌套层数过多 */
        error(30);
    }
    if (sym == nul) sym = period;
    do {

        while (sym == varsym || sym == constsym) {	/* 遇到变量声明符号，开始处理变量声明 */
            if (sym == varsym) {
                getsym();
                vardeclaration(&tx, lev, &dx);
                if (sym == semicolon) {
                    getsym();
                } else {
                    error(9); /* 漏掉了分号 */
                }
            } else {
                getsym();
                constdeclaration(&tx, lev, &dx);	/* dx的值会被constdeclaration改变，使用指针 */
                if (sym == semicolon) { /* 遇到分号结束定义常量 */
                    getsym();
                } else {
                    error(9);   /* 漏掉了分号 */
                }
            }
        }

        while (sym == funcsym) { /* 遇到过程声明符号，开始处理过程声明 */
            getsym();

            if (sym == ident) {
                enter(function, &tx, lev, &dx);	/* 填写符号表 */
                getsym();
            } else {
                error(0);	/* procedure后应为标识符 */
            }

            if (sym == lparen) getsym();
            else error(19);
            if (sym == rparen) getsym();
            else error(19);
            if (sym == lbrace) getsym();
            else error(20);

            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[rbrace] = true;
            block(lev + 1, tx, nxtlev); /* 递归调用 */

            if(sym == rbrace) {
                getsym();
                memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
                nxtlev[funcsym] = true;
                test(nxtlev, fsys, 6);
            } else {
                error(38);	/* 漏掉了'}' */
            }
        }
        if (sym == period) break;
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        test(nxtlev, declbegsys, 7);
    } while (inset(sym, declbegsys));	/* 直到没有声明符号 */

    code[table[tx0].adr].a = cx;	/* 把前面生成的跳转语句的跳转位置改成当前位置 */
    table[tx0].adr = cx;	        /* 记录当前过程代码地址 */
    table[tx0].size = dx;	        /* 声明部分中每增加一条声明都会给dx增加1，声明部分已经结束，dx就是当前过程数据的size */
    cx0 = cx;
    gen(ini, 0, dx);	            /* 生成指令，此指令执行时在数据栈中为被调用的过程开辟dx个单元的数据区 */

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
    /* 语句后继符号为分号或end */
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);	/* 每个后继符号集合都包含上层后继符号集合，以便补救 */
    nxtlev[semicolon] = true;
    statement(nxtlev, &tx, lev);
    gen(opr, 0, 0);	                    /* 每个过程出口都要使用的释放数据段指令 */
    memset(nxtlev, 0, sizeof(bool) * symnum);	/* 分程序没有补救集合 */
    test(fsys, nxtlev, 8);            	/* 检测后继符号正确性 */
}

/*
 * 在符号表中加入一项
 *
 * k:      标识符的种类为const，var或procedure
 * ptx:    符号表尾指针的指针，为了可以改变符号表尾指针的值
 * lev:    标识符所在的层次
 * pdx:    dx为当前应分配的变量的相对地址，分配后要增加1
 *
 */
void enter(enum object k, int* ptx,	int lev, int* pdx) {
    (*ptx)++;
    strcpy(table[(*ptx)].name, id); /* 符号表的name域记录标识符的名字 */
    table[(*ptx)].kind = k;
    switch (k) {
    case constant:	/* 常量 */
        if (num > addrmax) {
            error(15);	/* 常数越界 */
            num = 0;
        }
        table[(*ptx)].val = num; /* 登记常数的值 */
        break;
    case variable:	/* 变量 */
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        (*pdx)++;
        break;
    case function:	/* 过程 */
        table[(*ptx)].level = lev;
        break;
    }
}

/*
 * 查找标识符在符号表中的位置，从tx开始倒序查找标识符
 * 找到则返回在符号表中的位置，否则返回0
 *
 * id:    要查找的名字
 * tx:    当前符号表尾指针
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
 * 常量声明处理
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
                error(32);	/* 常量声明中的=后应是数字 */
            }
        } else {
            error(8);	/* 常量声明中的标识符后应是= */
        }
    } else {
        error(0);	/* const后应是标识符 */
    }
}

/*
 * 变量声明处理
 */
void vardeclaration(int* ptx,int lev,int* pdx) {
    if (sym == ident) {
        enter(variable, ptx, lev, pdx);	// 填写符号表
        getsym();
    } else {
        error(0);	/* var后面应是标识符 */
    }
}

/*
 * 输出所有目标代码
 */
void listall() {
    int i;
    for (i = 0; i < cx; i++) {
        fprintf(fcode,"%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
    }
}

/*
 * 语句处理
 */
void statement(bool* fsys, int* ptx, int lev) {
    int i, cx1, cx2;
    bool nxtlev[symnum];

    while (inset(sym, statbegsys)) {
        if (sym == ident) {	/* 准备按照赋值语句处理 */
            i = position(id, *ptx);/* 查找标识符在符号表中的位置 */
            if (i == 0) {
                error(6);	/* 标识符未声明 */
            } else {
                if(table[i].kind != variable) {
                    error(7);	/* 赋值语句中，赋值号左部标识符应该是变量 */
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
                            error(8);	/* 没有检测到赋值符号 */
                        }
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        expression(nxtlev, ptx, lev);	/* 处理赋值符号右侧表达式 */
                        if(i != 0) {
                            /* expression将执行一系列指令，但最终结果将会保存在栈顶，执行sto命令完成赋值 */
                            gen(sto, lev-table[i].level, table[i].adr);
                        }
                    }
                }
            }
        } else {
            if (sym == readsym) {	/* 准备按照read语句处理 */
                getsym();
                if (sym != lparen) {
                    error(17);	/* 格式错误，应是左括号 */
                } else {
                    getsym();
                    if (sym == ident) {
                        i = position(id, *ptx);	/* 查找要读的变量 */
                    } else {
                        i = 0;
                    }

                    if (i == 0) {
                        error(18);	/* read语句括号中的标识符应该是声明过的变量 */
                    } else {
                        gen(opr, 0, 16);	/* 生成输入指令，读取值到栈顶 */
                        gen(sto, lev-table[i].level, table[i].adr);	/* 将栈顶内容送入变量单元中 */
                    }
                    getsym();
                }
                if(sym != rparen) {
                    error(16);	/* 格式错误，应是右括号 */
                    while (!inset(sym, fsys)) {	/* 出错补救，直到遇到上层函数的后继符号 */
                        getsym();
                    }
                } else {
                    getsym();
                }
            } else {
                if (sym == printsym) {	/* 准备按照write语句处理 */
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
                            error(16);	/* 格式错误，应是右括号 */
                        } else {
                            getsym();
                        }
                    }

                } else {
                    if (sym == callsym) {	/* 准备按照call语句处理 */
                        getsym();
                        if (sym != ident) {
                            error(0);	/* call后应为标识符 */
                        } else {
                            i = position(id, *ptx);
                            if (i == 0) {
                                error(6);	/* 过程名未找到 */
                            } else {
                                if (table[i].kind == function) {
                                    gen(cal, lev-table[i].level, table[i].adr);	/* 生成call指令 */
                                } else {
                                    error(31);	/* call后标识符类型应为过程 */
                                }
                            }
                            getsym();
                            if (sym == lparen) getsym();
                            else error(19);
                            if (sym == rparen) getsym();
                            else error(19);
                        }
                    } else {
                        if (sym == ifsym) {	/* 准备按照if语句处理 */
                            getsym();
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[lbrace] = true;
                            condition(nxtlev, ptx, lev); /* 调用条件处理 */
                            if (sym == lbrace) {
                                getsym();
                            } else {
                                error(20);
                            }
                            cx1 = cx;	/* 保存当前指令地址 */
                            gen(jpc, 0, 0);	/* 生成条件跳转指令，跳转地址未知，暂时写0 */
                            nxtlev[rbrace] = true;
                            nxtlev[elsesym] = true;
                            statement(fsys, ptx, lev);	/* 处理then后的语句 */
                            cx2 = cx;
                            gen(jmp, 0, 0);
                            code[cx1].a = cx;
                            code[cx1].a = cx;	/* 经statement处理后，cx为then后语句执行完的位置，它正是前面未定的跳转地址，此时进行回填 */
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
                            if (sym == whilesym) {	/* 准备按照while语句处理 */
                                cx1 = cx;	/* 保存判断条件操作的位置 */
                                getsym();
                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[lbrace] = true;	/* 后继符号为do */
                                condition(nxtlev, ptx, lev);	/* 调用条件处理 */
                                cx2 = cx;	/* 保存循环体的结束的下一个位置 */
                                gen(jpc, 0, 0);	/* 生成条件跳转，但跳出循环的地址未知，标记为0等待回填 */
                                if (sym == lbrace) {
                                    getsym();
                                } else {
                                    error(20);	/* 缺少do */
                                }
                                memcpy(nxtlev, fsys, sizeof nxtlev);
                                nxtlev[rbrace] = true;
                                statement(fsys, ptx, lev);	/* 循环体 */
                                if (sym == rbrace) getsym();
                                else error(21);
                                gen(jmp, 0, cx1);	/* 生成条件跳转指令，跳转到前面判断条件操作的位置 */
                                code[cx2].a = cx;	/* 回填跳出循环的地址 */
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
 * 表达式处理
 */
void expression(bool* fsys, int* ptx, int lev) {
    enum symbol addop;	/* 用于保存正负号 */
    bool nxtlev[symnum];

    if(sym == plus || sym == minus) {	/* 表达式开头有正负号，此时当前表达式被看作一个正的或负的项 */
        addop = sym;	/* 保存开头的正负号 */
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        term(nxtlev, ptx, lev);	/* 处理项 */
        if (addop == minus) {
            gen(opr,0,1);	/* 如果开头为负号生成取负指令 */
        }
    } else {	/* 此时表达式被看作项的加减 */
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        term(nxtlev, ptx, lev);	/* 处理项 */
    }
    while (sym == plus || sym == minus) {
        addop = sym;
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        term(nxtlev, ptx, lev);	/* 处理项 */
        if (addop == plus) {
            gen(opr, 0, 2);	/* 生成加法指令 */
        } else {
            gen(opr, 0, 3);	/* 生成减法指令 */
        }
    }
}

/*
 * 项处理
 */
void term(bool* fsys, int* ptx, int lev) {
    enum symbol mulop;	/* 用于保存乘除法符号 */
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    nxtlev[mod] = true;
    factor(nxtlev, ptx, lev);	/* 处理因子 */
    while(sym == times || sym == slash || sym == mod) {
        mulop = sym;
        getsym();
        factor(nxtlev, ptx, lev);
        if(mulop == times) {
            gen(opr, 0, 4);	/* 生成乘法指令 */
        } else if (mulop == slash) {
            gen(opr, 0, 5);	/* 生成除法指令 */
        } else {
            gen(opr, 0, 17);
        }
    }
}

/*
 * 因子处理
 */
void factor(bool* fsys, int* ptx, int lev) {
    int i;
    bool nxtlev[symnum];
    test(facbegsys, fsys, 24);	/* 检测因子的开始符号 */
    while(inset(sym, facbegsys)) {	/* 循环处理因子 */
        if(sym == ident) {	/* 因子为常量或变量 */
            i = position(id, *ptx);	/* 查找标识符在符号表中的位置 */
            if (i == 0) {
                error(6);	/* 标识符未声明 */
            } else {
                switch (table[i].kind) {
                case constant:	/* 标识符为常量 */
                    gen(lit, 0, table[i].val);	/* 直接把常量的值入栈 */
                    break;
                case variable:	/* 标识符为变量 */
                    gen(lod, lev-table[i].level, table[i].adr);	/* 找到变量地址并将其值入栈 */
                    break;
                case function:	/* 标识符为过程 */
                    error(12);	/* 不能为过程 */
                    break;
                }
            }
            getsym();
        } else {
            if(sym == number) {	/* 因子为数 */
                if (num > addrmax) {
                    error(15); /* 数越界 */
                    num = 0;
                }
                gen(lit, 0, num);
                getsym();
            } else {
                if (sym == lparen) {	/* 因子为表达式 */
                    getsym();
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[rparen] = true;
                    expression(nxtlev, ptx, lev);
                    if (sym == rparen) {
                        getsym();
                    } else {
                        error(16);	/* 缺少右括号 */
                    }
                }
            }
        }
        memset(nxtlev, 0, sizeof(bool) * symnum);
        nxtlev[lparen] = true;
        test(fsys, nxtlev, 4); /* 一个因子处理完毕，遇到的单词应在fsys集合中 */
        /* 如果不是，报错并找到下一个因子的开始，使语法分析可以继续运行下去 */
    }
}

/*
 * 条件处理
 */
void condition(bool* fsys, int* ptx, int lev) {
    enum symbol relop;
    bool nxtlev[symnum];
    if(sym == oddsym) {	/* 准备按照odd运算处理 */
        getsym();
        expression(fsys, ptx, lev);
        gen(opr, 0, 6);	/* 生成odd指令 */
        return;
    }
    /* 逻辑表达式处理 */
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[eql] = true;
    nxtlev[neq] = true;
    nxtlev[lss] = true;
    nxtlev[leq] = true;
    nxtlev[gtr] = true;
    nxtlev[geq] = true;
    expression(nxtlev, ptx, lev);
    if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq) {
        error(11); /* 应该为关系运算符 */
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
 * 解释程序
 */
void interpret() {
    int p = 0; /* 指令指针 */
    int b = 1; /* 指令基址 */
    int t = 0; /* 栈顶指针 */
    struct instruction i;	/* 存放当前指令 */
    int s[stacksize];	/* 栈 */

    s[0] = 0; /* s[0]不用 */
    s[1] = 0; /* 主程序的三个联系单元均置为0 */
    s[2] = 0;
    s[3] = 0;

    do {
        if (cx == 0) break;
        i = code[p];	/* 读当前指令 */
        fprintf(fdebug, "#\n");
        fprintf(fdebug, "%d\n", p);
        p = p + 1;
        switch (i.f) {
        case lit:	/* 将常量a的值取到栈顶 */
            t = t + 1;
            s[t] = i.a;
            break;
        case opr:	/* 数学、逻辑运算 */
            switch (i.a) {
            case 0:  /* 函数调用结束后返回 */
                t = b - 1;
                p = s[t + 3];
                b = s[t + 2];
                break;
            case 1: /* 栈顶元素取反 */
                s[t] = - s[t];
                break;
            case 2: /* 次栈顶项加上栈顶项，退两个栈元素，相加值进栈 */
                t = t - 1;
                s[t] = s[t] + s[t + 1];
                break;
            case 3:/* 次栈顶项减去栈顶项 */
                t = t - 1;
                s[t] = s[t] - s[t + 1];
                break;
            case 4:/* 次栈顶项乘以栈顶项 */
                t = t - 1;
                s[t] = s[t] * s[t + 1];
                break;
            case 5:/* 次栈顶项除以栈顶项 */
                t = t - 1;
                s[t] = s[t] / s[t + 1];
                break;
            case 6:/* 栈顶元素的奇偶判断 */
                s[t] = s[t] % 2;
                break;
            case 8:/* 次栈顶项与栈顶项是否相等 */
                t = t - 1;
                s[t] = (s[t] == s[t + 1]);
                break;
            case 9:/* 次栈顶项与栈顶项是否不等 */
                t = t - 1;
                s[t] = (s[t] != s[t + 1]);
                break;
            case 10:/* 次栈顶项是否小于栈顶项 */
                t = t - 1;
                s[t] = (s[t] < s[t + 1]);
                break;
            case 11:/* 次栈顶项是否大于等于栈顶项 */
                t = t - 1;
                s[t] = (s[t] >= s[t + 1]);
                break;
            case 12:/* 次栈顶项是否大于栈顶项 */
                t = t - 1;
                s[t] = (s[t] > s[t + 1]);
                break;
            case 13: /* 次栈顶项是否小于等于栈顶项 */
                t = t - 1;
                s[t] = (s[t] <= s[t + 1]);
                break;
            case 14:/* 栈顶值输出 */
                printf("%d", s[t]);
                fprintf(fresult, "%d", s[t]);
                break;
            case 15:/* 输出换行符 */
                printf("\n");
                fprintf(fresult,"\n");
                break;
            case 16:/* 读入一个输入置于栈顶 */
                t = t + 1;
                scanf("%d", &(s[t]));
                break;
            case 17: //求余
                t = t - 1;
                s[t] = s[t] % s[t + 1];
                break;
            }
            break;
        case lod:	/* 取相对当前过程的数据基地址为a的内存的值到栈顶 */
            t = t + 1;
            s[t] = s[base(i.l,s,b) + i.a];
            break;
        case sto:	/* 栈顶的值存到相对当前过程的数据基地址为a的内存 */
            s[base(i.l, s, b) + i.a] = s[t];
            t = t - 1;
            break;
        case cal:	/* 调用子过程 */
            s[t + 1] = base(i.l, s, b);	/* 将父过程基地址入栈，即建立静态链 */
            s[t + 2] = b;	/* 将本过程基地址入栈，即建立动态链 */
            s[t + 3] = p;	/* 将当前指令指针入栈，即保存返回地址 */
            b = t + 1;	/* 改变基地址指针值为新过程的基地址 */
            p = i.a;	/* 跳转 */
            break;
        case ini:	/* 在数据栈中为被调用的过程开辟a个单元的数据区 */
            t = t + i.a;
            break;
        case jmp:	/* 直接跳转 */
            p = i.a;
            break;
        case jpc:	/* 条件跳转 */
            if (s[t] == 0)
                p = i.a;
            t = t - 1;
            break;
        case jeq:	/* 条件跳转 */
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

/* 通过过程基址求上l层过程的基址 */
int base(int l, int* s, int b) {
    int b1;
    b1 = b;
    while (l > 0) {
        b1 = s[b1];
        l--;
    }
    return b1;
}
