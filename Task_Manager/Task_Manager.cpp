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

//color
void SetColor(unsigned short BackGroundColor, unsigned short ForeColor)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, (ForeColor % 16) | (BackGroundColor % 16 * 16));
}
/*
    0 = 黑色       8 = 灰色
    1 = 蓝色       9 = 淡蓝色
    2 = 绿色       A = 淡绿色
    3 = 浅绿色     B = 淡浅绿色
    4 = 红色       C = 淡红色
    5 = 紫色       D = 淡紫色
    6 = 黄色       E = 淡黄色
    7 = 白色       F = 亮白色
*/

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
    void clear() {
        name.clear();
        stime = ptime = ftime = 0;
    }
    bool empty() {
        return name.empty() && !stime && !ptime && !ftime;
    }
};
//下面的比较函数只有第一排序条件，没有第二，第三，第四条件
//之所以没有加是因为我觉得没必要，也没人会看我的代码

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
    ofstream fout("C:\\task\\task.lib", ios::out);
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
    getxy();
    for (int i = 0; i < 27; i++) {
        gotoxy(72, i + 1); cout << "                    ";
    }
    for (int i = 0; i < n && i < 5; i++) {
        gotoxy(72, i*4+1);
        cout << "-------------------";
        gotoxy(72, i*4+3);
        cout << "                          ";
        gotoxy(72, i*4+3);
        cout << i + 1 << "." << strs.str[i];
    }
    gotoxy(72, 4 * n + 1);
    cout << "-------------------";
    gotoxy(72, 4*n+2); cout << "返回请按b键";
    gotoxy(72, 4*n+3); cout << "按下s键以保存数据";
    gotoxy(72, 4*n+4); cout << "按下x键以保存并退出";
    gotoxy(72, 4*n+5); cout << "用数字1,2,3,4选择";
    gotoxy(72, 4*n+6); cout << "请输入你的回复";
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
    backxy();
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

unsigned int readud() {
    string t; cin >> t;
    if (t == "b" || t == "back") return -1;
    for (unsigned int i = 0; i < t.size(); i++) if (t[i] < '0' || t[i]>'9') return -2;
    unsigned int ans = 0;
    for (unsigned int i = 0; i < t.size(); i++) ans = ans * 10 + t[i] - '0';
    if (ans > 999) ans -= 1000;
    if (ans > 1000) return -1;
    return ans;
}


void read() {
    ifstream fin("C:\\task\\task.lib", ios::in);
    if (fin.eof()) {
        cout << "如果您是初次使用，请先进入 帮助->关于 查看关于此软件的相关操作\n";
        system("pause");
        return;
    }
    task t; int n; fin >> n;
    for (int i = 0; i < n; i++) {
        while (t.name == "" || t.name == "\n") getline(fin, t.name);
        fin >> t.stime >> t.stime >> t.ftime;
        his.push_back(t);
    }
    fin >> n;
    for (int i = 0; i < n; i++) {
        getline(fin, t.name);
        fin >> t.stime >> t.stime >> t.ftime;
        del.push_back(t);
    }
}

void show()
{
    cout << "total:" << his.size() << endl;
    cout << "bin:" << del.size() << endl;
    for (unsigned int i = 0; i < his.size(); i++) {
        if (his[i].ftime) SetColor(0, 8);
        else SetColor(0, 7);
        cout << i + 1000 << " " << his[i].name << endl;
        stamp_to_standard(his[i].stime, true), cout << endl;
        if (his[i].ptime) {
            cout << "plan time:";
            stamp_to_standard(his[i].ptime, true);
            cout << "\n";
        }
        if (his[i].ftime) {
            cout << "finish time:";
            stamp_to_standard(his[i].ftime, true);
            cout << "\n已完成\n";
        }
        cout << endl << endl;
    }
    SetColor(0, 7);
}

void new_task()
{
    system("cls");
    task now;

    cout << "请输入新任务的名字：";
    getline(cin,now.name);

    system("cls");
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

    system("cls");
    cout << "请告诉我这项任务计划在何时完成？\n";
    switch (choose(2, { "暂时没有计划完成的时间", "输入完成的时间" }))
    {
    case 2: {
        do {
            cout << "请输入时间：";
            char a[21];
            memset(a, sizeof(a), 0);
            while ((a[0] = getchar()) == '\n' || a[0] > '9' || a[0] < '0');
            for (int i = 1;; i++) {
                a[i] = getchar();
                if (a[i] == '\n') {
                    a[i] = 0; break;
                }
            }
            now.ptime = standard_to_stamp(a);
            if (now.ptime == -1) cout << "输入有误 请重输\n";
        } while (now.ptime == -1);
        break;
    }
    case 0: {
        return;
        break;
    }
    }
    cout << "您是否要保存这项任务？";
    switch (choose(3, { "保存", "当然要保存！", "不保存" })) {
    case 3:
    case 0: {
        return;
        break;
    }
    case 1:
    case 2: {
        his.push_back(now);
        save();
        break;
    }
    }
    save();
}

void remove_one(unsigned int num, int mode)
{
    if (num > 999) num -= 1000;
    /*
    his->del
    del->x
    his->x
    del->his
    */
    if (mode % 2 && num >= his.size()) {
        cout << "error 102" << endl; system("pause"); return;
    }
    if (mode % 2 == 0 && num >= del.size()) {
        cout << "error 102" << endl; system("pause"); return;
    }
    /*
    if(mode==1||mode==3) for(int i=his.size()-1;i>num;i--) temp.push_back(his.back()),his.pop_back();
    else for(int i=del.size()-1;i>num;i--) temp.push_back(del.back()),del.pop_back();

    if(mode==1) del.push_back(his.back()),his.pop_back();
    if(mode==2) del.pop_back();
    if(mode==3) his.pop_back();
    if(mode==4) his.push_back(del.back()),del.pop_back();

    if(mode==1||mode==3) for(int i=0;i<temp.size();i++) his.push_back(temp.back()),temp.pop_back();
    else for(int i=0;i<temp.size();i++) del.push_back(temp.back()),temp.pop_back();
    */
    if (mode == 1) {//his->del
        del.push_back(his[num]);
        his[num].clear();
        temp = his;
        his.clear();
        for (unsigned int i = 0; i < temp.size(); i++) if (!temp[i].empty()) his.push_back(temp[i]);
    }
    if (mode == 2) {//del->x
        del[num].clear();
        temp = del;
        del.clear();
        for (unsigned int i = 0; i < temp.size(); i++) if (!temp[i].empty()) del.push_back(temp[i]);
    }
    if (mode == 3) {//his->x
        his[num].clear();
        temp = his;
        his.clear();
        for (unsigned int i = 0; i < temp.size(); i++) if (!temp[i].empty()) his.push_back(temp[i]);
    }
    if (mode == 4) {//del->his
        his.push_back(del[num]);
        del[num].clear();
        temp = del;
        del.clear();
        for (unsigned int i = 0; i < temp.size(); i++) if (!temp[i].empty()) his.push_back(temp[i]);
    }
}

void finish()
{
    system("cls");
    show();
    //这里必须要cls+show，因为下面有提示语。。。
    cout << "输入已完成的事项编号\n";
    unsigned int num; num = readud();
    if (num > (unsigned int)-3) return;
    if (num >= 1000) num -= 1000;
    if (num < 0 || num >= his.size()) {
        cout << "error 104\n"; system("pause"); return;
    }
    if (his[num].ftime)
    {
        cout << "该事项已经完成了。你还可以\n";
        switch (choose(4, { "取消完成", "修改完成时间", "删除该事项", "永久删除该事项(真的很久)" }))
        {
        case 0: {
            return;
            break;
        }
        case 1: {
            his[num].ftime = 0;
            break;
        }
        case 2: {
            do {
                cout << "input time:";
                char a[21];
                memset(a, sizeof(a), 0);
                while ((a[0] = getchar()) == '\n' || a[0] > '9' || a[0] < '0');
                for (int i = 1; i < 21; i++) {
                    a[i] = getchar();
                    if (a[i] == '\n') {
                        a[i] = 0; break;
                    }
                }
                his[num].ftime = standard_to_stamp(a);
                if (his[num].ftime == -1) cout << "输入有误 请重输\n";
            } while (his[num].ftime == -1);
            break;
        }
        case 3: {
            remove_one(num, 1);
            break;
        }
        case 4: {
            remove_one(num, 3);
            break;
        }
        }

    }
    else
    {
        system("cls");
        cout << "什么时候完成的？\n";
        switch (choose(2, { "刚刚完成", "之前完成" })) {
        case 0: {
            return;
            break;
        }
        case 1: {
            his[num].ftime = time(0);
            break;
        }
        case 2: {
            do {
                cout << "input time:";
                char a[21];
                memset(a, sizeof(a), 0);
                while ((a[0] = getchar()) == '\n' || a[0] > '9' || a[0] < '0');
                for (int i = 1; i < 21; i++) {
                    a[i] = getchar();
                    if (a[i] == '\n') {
                        a[i] = 0; break;
                    }
                }
                his[num].ftime = standard_to_stamp(a);
                if (his[num].ftime == -1) cout << "输入有误 请重输\n";
            } while (his[num].ftime == -1);
            break;
        }
        }
    }
    save();
}

void edit()
{

}

void more()
{

}

void help()
{
    system("cls");
    cout << "当前版本:" << V << endl;
    cout << "妈妈给了本关于拖延症的书，叫《别让拖延症毁了你》，我看到标题就觉得这本书适合我（因为我觉得自己太拖延了！）\n"
        << "里面有许多关于拖延症的原因，道理，逻辑方面的知识，也有许多小故事，每个故事都对应这一种类型的拖延症表现（当然也有正面的）\n"
        << "其中就有个措施是“合理的制定计划”，我想，在这神奇的编程下，制定计划应该会很容易吧，就设想了几个比较不错的功能\n"
        << "但是我到网上一搜，得到的代码都无法满足我的要求（虽然我的标准也不是恒定不变的，但我觉得我可以写出让我满意的代码）\n"
        << "于是我就开始动手了\n\n"
        ;
    cout << "数据保存在C:\\task\\task.lib,可使用txt文本查看器打开\n";
    cout << "b 返回\ns 保存\nx 保存并退出\n\n";
    cout << "更新日志&官网:https://github.com/hsh778205/Task_Manager\n";
    system("pause");
    system("cls");

}
int main()
{
    read();
    while (1)
    {
        system("cls");
        show();
        switch (choose(5, { "新建任务","结束任务","修改任务","更多选项","我要帮助" }))
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
完成
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