// Task_Manager.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//V2.0

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <map>
#include <vector>
#include <fstream>
#include <windows.h>
#include <stdlib.h>
#include <ctime> 
#include <conio.h>
#include <sstream>
using namespace std;

//版本号信息
int vx = 1, vy = 2;
string V(unsigned int dt = 0)
{
    string ans = "V";
    stringstream ss;
    ss << vx << "." << vy + dt;
    ans += ss.str();
    return ans;
}

//窗口设置
POINT curpos;//鼠标的位置
//window
RECT rect;
HWND hwnd;
//mouse
HANDLE ConsoleWin = GetStdHandle(STD_INPUT_HANDLE);
INPUT_RECORD eventMsg;
DWORD Pointer;

//任务结构
struct task {
    string name;
    int stime, ptime, ftime;
    task() {
        stime = ptime = ftime = 0;
    }
    bool operator<(task x) {
        if (stime == x.stime) return ptime < x.ptime;
        return stime < x.stime;
    }
    bool operator==(task x) {
        return name == x.name && stime == x.stime && ptime == x.ptime && ftime == x.ftime;
    }
    bool operator!=(task x) {
        return !(*this == x);
    }
};
//下面的比较函数只有第一排序条件，没有第二，第三，第四条件
//之所以没有加是因为我觉得没必要，也没人会看我的代码
const task empty;

bool task_str(task a, task b) {
    return a.name < b.name;
}

bool task_st(task a, task b) {
    return a.stime < b.stime;
}

bool task_pt(task a, task b) {
    return a.ptime < b.ptime;
}

bool task_ft(task a, task b) {
    return a.ftime < b.ftime;
}

vector<task>his, del, temp;
//map<int,string>err
//此处不用err来存错误信息，还是把它放到出错的地方吧

bool judge(string t = "None") {
    if (t == "None") cin >> t;
    if (t == "1" || t == "y" || t == "Y" || t == "yes" || t == "YES" || t == "Yes") return 1;
    return 0;
}

int lx, ly;

void goto_and_get_xy(int x, int y){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    lx = csbi.dwCursorPosition.X, ly = csbi.dwCursorPosition.Y;

    COORD pos;
    pos.X = x - 1;
    pos.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void backxy()
{
    COORD pos;
    pos.X = lx;
    pos.Y = ly;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void gotoxy(int x, int y)
{
    COORD pos;
    pos.X = x - 1;
    pos.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void getxy() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    lx = csbi.dwCursorPosition.X, ly = csbi.dwCursorPosition.Y;
}

void save() {
    ofstream fout("E:\\task\\task.lib", ios::out);
    fout << his.size() << endl;
    for (unsigned int i = 0; i < his.size(); i++) {
        fout << his[i].name << endl << his[i].stime << " " << his[i].ptime << " " << his[i].ftime << endl << endl;
    }
    fout << del.size() << endl;
    for (unsigned int i = 0; i < del.size(); i++) {
        fout << del[i].name << endl << del[i].stime << " " << del[i].ptime << " " << del[i].ftime << endl << endl;
    }
}

int choose(int n, string str[4]) {

    for (int i = 0; i < n && i < 4; i++) {
        gotoxy(50, i*4+1);
        cout << "----------------";
        gotoxy(50, i*4+3);
        cout << "                          ";
        gotoxy(50, i*4+3);
        cout << i + 1 << "." << str[i];
    }
    gotoxy(50, 4 * n + 1);
    cout << "----------------";
    gotoxy(50, 4*n+2); cout << "返回请按b键";
    gotoxy(50, 4*n+3); cout << "按下s键以保存数据";
    gotoxy(50, 4*n+4); cout << "按下x键以保存并退出";
    gotoxy(50, 4*n+5); cout << "用数字1,2,3,4选择";
    gotoxy(50, 4*n+6); cout << "请输入你的回复";
    char ans;
    while (true) {
        ans = getch();
        if (ans == 'r' || ans == 'R') {
            backxy();
            main();
            exit(0);
        }
        if (ans > '0' && ans <= n + '0')
        {
            backxy();
            return ans - '0';
        }
        if (ans == 'x' || ans == 'X') {
            backxy();
            save(); exit(0);
        }
        if (ans == 's' || ans == 'S') {
            backxy();
            save(); cout << "保存成功"; Sleep(2000); cout << "\b\b  \b\b\b\b  \b\b\b\b  \b\b\b\b  \b\b";
        }
        if (ans == 'b' || ans == 'B') {
            backxy();
            return 0;
        }
    }
}


int main()
{
    
}

/*
新建
修改
更多
    回收站
        还原
    删除
        删除一个
        批量删除
    查询
    更新
帮助
保存--s
退出--x

*/