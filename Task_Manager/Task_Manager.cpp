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
#define V 2.0

//choose传输类型
//用此类型将n个选项传参
struct Strs {
    string str[5];

};

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

int choose(int n, Strs strs) {

    for (int i = 0; i < n && i < 5; i++) {
        gotoxy(50, i*4+1);
        cout << "----------------";
        gotoxy(50, i*4+3);
        cout << "                          ";
        gotoxy(50, i*4+3);
        cout << i + 1 << "." << strs.str[i];
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

//时间类型转换
//时间戳转换成标准时间 
typedef struct times
{
    int Year;
    int Mon;
    int Day;
    int Hour;
    int Min;
    int Second;
} Times;

Times stamp_to_standard(int stampTime, bool output = false)
{
    time_t tick = (time_t)stampTime;
    struct tm tm;
    char s[100];
    Times standard;
    //tick = time(NULL);
    tm = *localtime(&tick);
    strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
    if (output)
        printf("%d: %s", (int)tick, s);
    //	cout<<b.Year<<" "<<b.Mon<<" "<<b.Day<<" "<<b.Hour<<" "<<b.Min<<" "<<b.Second; 
    standard.Year = atoi(s);
    standard.Mon = atoi(s + 5);
    standard.Day = atoi(s + 8);
    standard.Hour = atoi(s + 11);
    standard.Min = atoi(s + 14);
    standard.Second = atoi(s + 17);
    return standard;
}

//标准时间准换成时间戳 
int standard_to_stamp(char* str_time)
{
    struct tm stm;
    int iY, iM, iD, iH, iMin, iS;

    memset(&stm, 0, sizeof(stm));
    iY = atoi(str_time);
    iM = atoi(str_time + 5);
    iD = atoi(str_time + 8);
    iH = atoi(str_time + 11);
    iMin = atoi(str_time + 14);
    iS = atoi(str_time + 17);

    stm.tm_year = iY - 1900;
    stm.tm_mon = iM - 1;
    stm.tm_mday = iD;
    stm.tm_hour = iH;
    stm.tm_min = iMin;
    stm.tm_sec = iS;
    //printf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);*/   //标准时间格式例如：2016:08:02 12:12:30
    return (int)mktime(&stm);
}

void new_task()
{
    system("cls");
    task now;

    cout << "请输入新任务的名字：";
    getline(cin,now.name);

    cout << "请告诉我这项任务准备什么时候开始？\n";
    switch (choose(2, { "现在开始","我来输入时间" }))
    {
    case 1: {
        now.stime = time(0);
        break;
    }
    case 2: {
        do {
            cout << "请输入时间：";
            char a[21];
            while ((a[0] = getchar()) == '\n' || a[0] > '9' || a[0] < '0');
            memset(a, sizeof(a), 0);
            for (int i = 1;; i++) {
                a[i] = getchar();
                if (a[i] == '\n') {
                    a[i] = 0; break;
                }
            }
            now.stime = standard_to_stamp(a);
            if (now.stime == -1) cout << "输入有误 请重输\n";
        } while (now.stime == -1);
        break;
    }
    case 0: {
        return;
        break;
    }
    }

    cout << "请告诉我这项任务计划在何时完成？\n";
    switch (choose(2, { "暂时没有计划完成的时间", "输入完成的时间" }))
    {
    case 2: {
        do {
            cout << "time:";
            char a[21];
            memset(a, sizeof(a), 0);
            while ((a[0] = getchar()) == '\n' || a[0] > '9' || a[0] < '0');
            for (int i = 1;; i++) {
                a[i] = getchar();
                if (a[i] == '\n') {
                    a[i] = 0; break;
                }
            }
            now.ftime = standard_to_stamp(a);
            if (now.ftime == -1) cout << "输入有误 请重输\n";
        } while (now.ftime == -1);
        break;
    }
    case 0: {
        return;
        break;
    }
    }
}

void finish()
{

}

void edit()
{

}

void more()
{

}

void help()
{


}
int main()
{



    while (1)
    {
        switch (choose(4, { "新建任务","结束任务","修改任务","更多选项","我要帮助" }))
        {
        case 1:{
            new_task();
            break;
            }
        case 2: {
            finish();
            break;
        }
        case 3: {
            edit();
            break;
        }
        case 4: {
            more();
            break;
        }
        case 5: {
            help();
            break;
        }
        }
    }
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