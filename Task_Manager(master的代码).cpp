#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<map>
#include<vector>
#include<fstream>
#include<windows.h>
#include<stdlib.h>
#include<ctime> 
#include<conio.h>
#include<sstream>
#define IL inline
#define re register
using namespace std;

int MODE=2;
/*
V1.3
 


*/ 
int vx=1,vy=2;
string V(unsigned int dt=0){//版本号没那么多规则啦 
	string ans="V";
	stringstream ss;
	ss<<vx<<"."<<vy+dt;
	ans+=ss.str();
	return ans;
}

POINT curpos;//鼠标的位置

//window
RECT rect; 
HWND hwnd;
//mouse
HANDLE ConsoleWin=GetStdHandle(STD_INPUT_HANDLE);
INPUT_RECORD eventMsg;
DWORD Pointer;

struct task{
	string name;
	int stime,ftime,rtime;
	task(){
		stime=ftime=rtime=0;
	}
	bool operator<(task x){
		if(stime==x.stime) return ftime<x.ftime;
		return stime<x.stime;
	}
	
	bool operator==(task x){
		if(name==x.name&&stime==x.stime&&ftime==x.ftime&&rtime==x.ftime) return 1;
		return 0;
	}
	
	bool operator!=(task x){
		return !(*this==x);
	}
};

const task empty;

IL bool task_bt(task a,task b){
	return a<b;
}

IL bool task_str(task a,task b){
	return a.name<b.name;
}

IL bool task_et(task a,task b){
	if(a.ftime==b.ftime) return a.stime<b.stime;
	if(a.ftime==0) return 0;
	if(b.ftime==0) return 1;
	return a.ftime<b.ftime;
}

vector<task>his,del,temp;
map<int,string>err;

IL bool judge(){
	string t;cin>>t;
	if(t=="1"||t=="y"||t=="Y"||t=="yes"||t=="YES"||t=="Yes") return 1;
	return 0;
}


int main();
void read();
void save();
int choose(string a="无",string b="无",string c="无",string d="无");
void _return();
void find_task();

void init(){
	system("cls");
	his.clear();
	err.clear();
	temp.clear();
	del.clear();
	err[101]="时间点错误(-1)";
	err[102]="输入的数据超大";
	err[104]="输入的编号错误"; 
}

IL void download(){
	system("cls"); 
	cout<<"直接是否下载最新版本的Task_Manager.exe?";
	if(judge()){
		switch(choose("跳转网页下载","certutil下载(不支持win xp)")){
			case 0:{
				return;
				break;
			}
			case 1:{
				cout<<"即将打开浏览器 若杀软报错请信任\n";
				system("pause");
				system("start https://www.cnblogs.com/send-off-a-friend/p/12461040.html");
				break;
			}
			case 2:{
				system("cls");
				stringstream ss;string str,ver;
				cout<<"即将下载 Task_Manager_"<<V(1)<<".exe"<<endl<<"若若杀软报错请信任"<<endl;
				system("pause");
				ss<<"certutil -urlcache -split -f https://files-cdn.cnblogs.com/files/send-off-a-friend/Task_Manager_"<<V(1)<<".xml Task_Manager_"<<V(1)<<".exe";
				str=ss.str();
				long long ret=system(str.c_str());
				if(ret==-2145844848) cout<<"哎呀(?｀?Д?′)!!无更新的版本\n";
				if(ret==0) cout<<"下载成功 可以不用我咯 即将关闭程序！さようなら！\n",system("pause"),exit(0);
				system("pause");
				break;
			}
			/* 
			case 3:{
				system("cls");
				stringstream ss;string str,ver;
				cout<<"即将下载 Task_Manager_"<<V(1)<<".exe"<<endl<<"若若杀软报错请信任"<<endl;
				system("pause");
				ss<<"bitsadmin /Transfer Task_Manager_"<<V(1)<<" https://files-cdn.cnblogs.com/files/send-off-a-friend/Task_Manager_"<<V(1)<<".xml Task_Manager_"<<V(1)<<".exe";
				str=ss.str();cout<<str<<endl;
				long long ret=system(str.c_str());
				cout<<ret<<endl;
				if(ret==-2145844848) cout<<"哎呀(?｀?Д?′)!!无更新的版本\n";
				if(ret==0) cout<<"下载成功 可以不用我咯 即将关闭程序！さようなら！\n",system("pause"),exit(0);
				if(ret==1) cout<<"1";
				if(ret==-2147024809) cout<<"参数错误是什么？\n";
				system("pause");system("pause");
				break;
			}
			*/
		} 
	} 
} 

IL unsigned int readud(){
	string t;cin>>t;
	if(t=="b"||t=="back") return -1;
	for(unsigned int i=0;i<t.size();i++) if(t[i]<'0'||t[i]>'9') return -2;
	unsigned int ans=0;
	for(unsigned int i=0;i<t.size();i++) ans=ans*10+t[i]-'0';
	if(ans>999) ans-=1000;
	if(ans>1000) return -1;
	return ans;
}

string cipher="778205";
unsigned int pass=-20000;//以防刚刚启动时可以免密码 
IL bool key(){
	if(clock()-pass<=20000) return 1;//20秒内免密码 
	cout<<"input the key:";
	string str;
	char ch;
	while(true){
		ch=getch();
		if(ch=='\r'||ch=='\n') break;
		if(ch=='\b'){
			if(!str.empty()) cout<<"\b \b",str.pop_back();//string.pop_back()是c++11的函数!!!加入-std=c++11! 
		}
		else str.push_back(ch),cout<<"*";
	}
	cout<<"\n";
	if(str==cipher) pass=clock();
	return str==cipher;
}

int lx,ly;

IL void goto_and_get_xy(int x,int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
	lx=csbi.dwCursorPosition.X,ly=csbi.dwCursorPosition.Y;
	
	COORD pos;
	pos.X = x - 1;
	pos.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

IL void backxy()
{
	COORD pos;
	pos.X = lx;
	pos.Y = ly;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
} 

IL void gotoxy(int x, int y)
{
	COORD pos;
	pos.X = x - 1;
	pos.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

IL void getxy(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
	lx=csbi.dwCursorPosition.X,ly=csbi.dwCursorPosition.Y;
} 

int choose(string a,string b,string c,string d){
	int all=0;
	if(a!="无") all++;
	if(b!="无") all++;
	if(c!="无") all++; 
	if(d!="无") all++;  
	getxy();
	if(MODE==2){
		gotoxy(50,1);
		cout<<"----------------";
		gotoxy(50,3);cout<<"                          ";
		gotoxy(50,3);cout<<"1."<<a;
		
		gotoxy(50,5);
		cout<<"----------------";
		gotoxy(50,7);cout<<"                          ";
		gotoxy(50,7);cout<<"2."<<b;
		
		gotoxy(50,9);
		cout<<"----------------";
		gotoxy(50,11);cout<<"                          ";
		gotoxy(50,11);cout<<"3."<<c;
		
		gotoxy(50,13);
		cout<<"----------------";
		gotoxy(50,15);cout<<"                          ";
		gotoxy(50,15);cout<<"4."<<d;
		
		gotoxy(50,17);
		cout<<"----------------";
		gotoxy(50,18);cout<<"返回请按b键";
		gotoxy(50,19);cout<<"按下s键以保存数据";
		gotoxy(50,20);cout<<"按下x键以保存并退出"; 
		gotoxy(50,21);cout<<"用数字1,2,3,4选择";
		gotoxy(50,22);cout<<"请输入你的回复";
	}
	
	if(MODE==1){
		gotoxy(50,1);
		cout<<"----------------";
		gotoxy(50,2);cout<<"                          ";
		gotoxy(50,2);cout<<"1."<<a;
		
		gotoxy(50,3);
		cout<<"----------------";
		gotoxy(50,4);cout<<"                          ";
		gotoxy(50,4);cout<<"2."<<b;
		
		gotoxy(50,5);
		cout<<"----------------";
		gotoxy(50,6);cout<<"                          ";
		gotoxy(50,6);cout<<"3."<<c;
		
		gotoxy(50,7);
		cout<<"----------------";
		gotoxy(50,8);cout<<"                          ";
		gotoxy(50,8);cout<<"4."<<d;
		
		gotoxy(50,9);
		cout<<"----------------";
		gotoxy(50,10);cout<<"返回请按b键";
		gotoxy(50,11);cout<<"按下s键以保存数据";
		gotoxy(50,12);cout<<"按下x键以保存并退出"; 
		gotoxy(50,13);cout<<"用数字1,2,3,4选择";
		gotoxy(50,14);cout<<"请输入你的回复";
	}
	if(MODE==3){
		cout<<"                                                  \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
		cout<<"\n1."<<a;cout<<"2."<<b;cout<<"3."<<c;cout<<"4."<<d;
		cout<<"\n返回请按b键"<<endl<<"按下s键以保存数据"<<endl<<"按下x键以保存并退出"<<endl<<"用数字1,2,3,4选择"<<endl<<"请输入你的回复\n";
	} 
	char ans;
	while(true){
		ans=getch();
		if(ans=='u'||ans=='U'){
			download();
			return 0;
		}
		if(ans=='r'||ans=='R'){
			backxy();
			main();
			exit(0);
		}
		if(ans=='f'||ans=='F'){
			backxy();
			find_task();
			return 0;
		}
		if(ans>'0'&&ans<=all+'0')
		{
			backxy();
			return ans-'0';
		}  
		if(ans=='x'||ans=='X'){
			backxy();
			_return();return 0;
		}
		if(ans=='s'||ans=='S'){
			backxy();
			save();cout<<"保存成功";Sleep(2000);cout<<"\b\b  \b\b\b\b  \b\b\b\b  \b\b\b\b  \b\b";
		}
		if(ans=='b'||ans=='B'){
			backxy();
			return 0;
		} 
	} 
}

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

Times stamp_to_standard(int stampTime,bool output=false)
{
	time_t tick = (time_t)stampTime;
	struct tm tm; 
	char s[100];
	Times standard;
	//tick = time(NULL);
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	if(output)
		printf("%d: %s", (int)tick, s);
//	cout<<b.Year<<" "<<b.Mon<<" "<<b.Day<<" "<<b.Hour<<" "<<b.Min<<" "<<b.Second; 
	standard.Year = atoi(s);
	standard.Mon = atoi(s+5);
	standard.Day = atoi(s+8);
	standard.Hour = atoi(s+11);
	standard.Min = atoi(s+14);
	standard.Second = atoi(s+17);
	return standard;
}

//标准时间准换成时间戳 
int standard_to_stamp(char *str_time)
{  
	struct tm stm;  
	int iY, iM, iD, iH, iMin, iS;  
	
	memset(&stm,0,sizeof(stm));  
	iY = atoi(str_time);  
	iM = atoi(str_time+5);  
	iD = atoi(str_time+8);  
	iH = atoi(str_time+11);  
	iMin = atoi(str_time+14);  
	iS = atoi(str_time+17);  
	
	stm.tm_year=iY-1900;  
	stm.tm_mon=iM-1;  
	stm.tm_mday=iD;  
	stm.tm_hour=iH;  
	stm.tm_min=iMin;  
	stm.tm_sec=iS;  
	//printf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);*/   //标准时间格式例如：2016:08:02 12:12:30
	return (int)mktime(&stm);  
} 

struct mem{
	unsigned int val,num;
	vector<int>point;
	mem operator+=(unsigned int x){
		val+=x;
		return *this;
	}
	mem operator++(int){
		mem now=*this;
		val++;
		return now;
	}
	bool operator<(mem x){
		return (val==x.val)?num<x.num:val>x.val;
	}
};

IL void find_task(){
	system("cls");
	cout<<"输入关键字\n"; 
	string str;cin>>str;

	vector<mem>value;
	for(unsigned int i=0;i<his.size();i++){
		value.push_back({0,i});
		//全字匹配 
		for(int j=0;j+str.size()<his[i].name.size();j++)
		for(unsigned int k=0;k<str.size();k++)
		{
			if(his[i].name[j+k]!=str[k]) break;
			if(k==str.size()-1) value[i]+=10,value[i].point.push_back(j);
		}
		//部分匹配 
		for(unsigned int j=0;j<his[i].name.size();j++)
		for(unsigned int k=0;k<str.size();k++)
		if(his[i].name[j]==str[k]) value[i]++,value[i].point.push_back(j);
	}
	sort(value.begin(),value.end());
	for(unsigned int i=0;i<value.size();i++)
	if(value[i].val){
		cout<<value[i].num+1000<<" "<<his[value[i].num].name<<endl;
		if(his[value[i].num].stime==-1){
			cout<<"error 101\n";
		}
		else stamp_to_standard(his[value[i].num].stime,true),cout<<endl;
		if(his[value[i].num].ftime){
			cout<<"planning time:";
			if(his[value[i].num].ftime==-1){
				cout<<"error 101\n";break;
			}
			stamp_to_standard(his[value[i].num].ftime,true);
			cout<<"\n";
		}
		if(his[value[i].num].rtime){
			cout<<"real time:";
			if(his[value[i].num].rtime==-1){
				cout<<"error 101\n";break;
			}
			stamp_to_standard(his[value[i].num].rtime,true);
			cout<<"\n已完成\n"; 
		}
		if(MODE==2)
			cout<<endl<<endl;
	}
	system("pause");
}

void new_task(){
	system("cls");
	task now;
	cout<<"*name:";
	cin>>now.name;
	if(now.name=="b"||now.name=="back"){
		cout<<"确定要返回吗？";
		if(judge()) return;
	}
	
	cout<<"*what beginning time it is?\n";
	switch(choose("now","input")){
		case 1:{
			now.stime=time(0);
			break;
		}
		case 2:{
			do{
				cout<<"time:";
				char a[21];
				while((a[0]=getchar())=='\n'||a[0]>'9'||a[0]<'0');
				memset(a,sizeof(a),0);
				for(int i=1;;i++){
					a[i]=getchar();
					if(a[i]=='\n'){
						a[i]=0;break;
					}
				}
				now.stime=standard_to_stamp(a);
				if(now.stime==-1) cout<<"输入有误 请重输\n";
			}while(now.stime==-1);
			break;
		}
		case 0:{
			return;
			break;
		}
	} 
	
	cout<<"what ending time it is?\n";
	switch(choose("no","input")){
		case 2:{
			do{
				cout<<"time:";
				char a[21];
				memset(a,sizeof(a),0);
				while((a[0]=getchar())=='\n'||a[0]>'9'||a[0]<'0');
				for(int i=1;;i++){
					a[i]=getchar();
					if(a[i]=='\n'){
						a[i]=0;break;
					}
				}
				now.ftime=standard_to_stamp(a);
				if(now.ftime==-1) cout<<"输入有误 请重输\n";
			}while(now.ftime==-1);
			break;
		}
		case 0:{
			return;
			break;
		}
		
	}
	
	cout<<"save?";
	switch(choose("保存","当然要保存！","不保存")){
		case 3:
		case 0:{
			return;
			break;
		}
		case 1:
		case 2:{
			his.push_back(now);
			save();
			break;
		}
		
	} 
}

void show(){
	cout<<"total:"<<his.size()<<endl;
	cout<<"bin:"<<del.size()<<endl;
	for(unsigned int i=0;i<his.size();i++){
		cout<<i+1000<<" "<<his[i].name<<endl;
		if(his[i].stime==-1){
			cout<<"error 101\n";
		}
		else stamp_to_standard(his[i].stime,true),cout<<endl;
		if(his[i].ftime){
			cout<<"planning time:";
			if(his[i].ftime==-1){
				cout<<"error 101\n";break;
			}
			stamp_to_standard(his[i].ftime,true);
			cout<<"\n";
		}
		if(his[i].rtime){
			cout<<"real time:";
			if(his[i].rtime==-1){
				cout<<"error 101\n";break;
			}
			stamp_to_standard(his[i].rtime,true);
			cout<<"\n已完成\n"; 
		}
		if(MODE==2)
			cout<<endl<<endl;
	}
}

IL void read(){
	ifstream fin("E:\\task\\task.lib",ios::in);
	if(fin.eof()){
		cout<<"如果您是初次使用，请先进入help->about查看关于此软件的相关操作\n";
		system("pause");
		return; 
	}
	task t;int n;fin>>n;
	for(int i=0;i<n;i++){
		fin>>t.name;
		fin>>t.stime>>t.ftime>>t.rtime;
		his.push_back(t);
	}
	fin>>n;
	for(int i=0;i<n;i++){
		fin>>t.name;
		fin>>t.stime>>t.ftime>>t.rtime;
		del.push_back(t);
	}
	fin>>cipher;
	
}

IL void remove_one(unsigned int num,int mode)
{
	if(num>999) num-=1000;
	/*
	his->del
	del->x
	his->x
	del->his
	*/
	if(mode%2&&num>=his.size()){
		cout<<"error 102"<<endl;system("pause");return; 
	}
	if(mode%2==0&&num>=del.size()){
		cout<<"error 102"<<endl;system("pause");return; 
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
	if(mode==1){//his->del
		del.push_back(his[num]);
		his[num]=empty;
		temp=his;
		his.clear();
		for(unsigned int i=0;i<temp.size();i++) if(temp[i]!=empty) his.push_back(temp[i]);
	}
	if(mode==2){//del->x
		del[num]=empty;
		temp=del;
		del.clear();
		for(unsigned int i=0;i<temp.size();i++) if(temp[i]!=empty) del.push_back(temp[i]);
	}
	if(mode==3){//his->x
		his[num]=empty;
		temp=his;
		his.clear();
		for(unsigned int i=0;i<temp.size();i++) if(temp[i]!=empty) his.push_back(temp[i]);
	}
	if(mode==4){//del->his
		his.push_back(del[num]);
		del[num]=empty;
		temp=del;
		del.clear();
		for(unsigned int i=0;i<temp.size();i++) if(temp[i]!=empty) his.push_back(temp[i]);
	}
}

IL void finish()
{
	system("cls");show();
	cout<<"输入已完成的事项编号\n";
	unsigned int num;num=readud();
	if(num>(unsigned int)-3) return;
	if(num>=1000) num-=1000;
	if(num<0||num>=his.size()){
		cout<<"error 104\n";system("pause");return;
	} 
	if(his[num].rtime)
	{
		cout<<"该事项已经完成了。你还可以\n";
		switch(choose("取消完成","修改完成时间","删除该事项","永久删除该事项(真的很久)"))
		{
			case 0:{
				return;
				break;
			}
			case 1:{
				his[num].rtime=0;
				break;
			}
			case 2:{
				do{
					cout<<"input time:";
					char a[21];
					memset(a,sizeof(a),0);
					while((a[0]=getchar())=='\n'||a[0]>'9'||a[0]<'0');
					for(int i=1;i<21;i++){
						a[i]=getchar();
						if(a[i]=='\n'){
							a[i]=0;break;
						}
					}
					his[num].rtime=standard_to_stamp(a);
					if(his[num].rtime==-1) cout<<"输入有误 请重输\n";
				}while(his[num].rtime==-1);
				break;
			}
			case 3:{
				remove_one(num,1);
				break;
			}
			case 4:{
				remove_one(num,3);
				break;
			}
		}

	}
	else
	{
		cout<<"什么时候完成的？\n";
		switch(choose("刚刚","之前")) {
			case 0:{
				return;
				break;
			}
			case 1:{
				his[num].rtime=time(0);
				break;
			}
			case 2:{
				do{
					cout<<"input time:";
					char a[21];
					memset(a,sizeof(a),0);
					while((a[0]=getchar())=='\n'||a[0]>'9'||a[0]<'0');
					for(int i=1;i<21;i++){
						a[i]=getchar();
						if(a[i]=='\n'){
							a[i]=0;break;
						}
					}
					his[num].rtime=standard_to_stamp(a);
					if(his[num].rtime==-1) cout<<"输入有误 请重输\n";
				}while(his[num].rtime==-1);
				break;
			}
		}
	}
	
}

IL void remove()
{
	switch(choose("删除所有已完成","删除一项","永久删除一项(真的很久)")){
		case 0:{
			return;
			break;
		}
		case 1:{
			for(int i=his.size()-1;i>=0;i--) if(his[i].rtime) remove_one(i,1);
			break;
		}
		case 2:{
			cout<<"输入要删除的事项编号\n";
			unsigned int num=readud();
			remove_one(num,1); 
			break;
		}
		case 3:{
			if(!key()) break;
			cout<<"输入要删除的事项编号\n";
			int num;cin>>num;
			remove_one(num,3); 
			break;
		}
	}
}

IL void move()
{
	while(true){
		switch(choose("排序","选中一项","随机","下一个排列"))
		{
			case 0:{
				return;
				break;
			}
			case 1:{
				cout<<"排序依据\n";
				switch(choose("起始时间","名称","预设时间","翻转"))
				{
					case 0:{
						return;
						break;
					}
					case 1:{
						sort(his.begin(),his.end(),task_bt);
						break;
					}
					case 2:{
						sort(his.begin(),his.end(),task_str);
						break;
					}
					case 3:{
						sort(his.begin(),his.end(),task_et);
						break;
					}
					case 4:{
						reverse(his.begin(),his.end());
						break;
					}
				}

				break;
			}
			case 2:{
				cout<<"输入选中任务的编号\n";
				unsigned int t;
				cin>>t;
				if(t>=1000) t-=1000;
				cout<<"上移s 下移x 返回b\n";
				char ch=getch();
				while(ch!='b'){
					if(ch=='s'){
						if(t>0) swap(his[t],his[t-1]),t--;
						else cout<<"已经到顶了!\n";
					}
					if(ch=='x'){
						if(t<his.size()-1) swap(his[t],his[t+1]),t++;
						else cout<<"已经到底了!\n";
					}
					system("cls");
					show();
					cout<<"上移s 下移x 返回b\n";
					ch=getch();
				}
				break;
			}
			case 3:{
				random_shuffle(his.begin(),his.end());
				break;
			}
			case 4:{
				next_permutation(his.begin(),his.end());
				break;
			}
		}
		system("cls");
		show();
	}
}

IL void bin()
{
	cout<<"bin:"<<del.size()<<endl;
	for(unsigned int i=0;i<del.size();i++){
		cout<<i+1000<<" "<<del[i].name<<endl;
		if(del[i].stime==-1){
			cout<<"error 101\n";
		}
		else stamp_to_standard(del[i].stime,true),cout<<endl;
		if(del[i].ftime){
			cout<<"planning time:";
			if(del[i].ftime==-1){
				cout<<"error 101\n";break;
			}
			stamp_to_standard(del[i].ftime,true);
			cout<<"\n";
		}
		if(del[i].rtime){
			cout<<"real time:";
			if(del[i].rtime==-1){
				cout<<"error 101\n";break;
			}
			stamp_to_standard(del[i].rtime,true);
			cout<<"\n已完成\n"; 
		}
		cout<<endl<<endl;
	}
	
	switch(choose("还原","永久删除一项","清空垃圾桶"))
	{
		case 0:{
			system("cls");
			show();
			return;
			break;
		}
		case 1:{
			cout<<"输入要还原的事项编号\n";
			int num;cin>>num;
			remove_one(num,4); 
			break;
		}
		case 2:{
			cout<<"输入要删除的事项编号\n";
			int num;cin>>num;
			remove_one(num,2); 
			break;
		}
		case 3:{
			cout<<"你确定要清空垃圾桶吗？\n";
			if(judge()) del.clear(); 
			break;
		}
	}
	system("cls");
	show(); 
} 

IL void edit()
{
	cout<<"输入要修改的任务编号\n";
	unsigned int t;cin>>t;
	if(t>999) t-=1000;
	if(t<0||t>his.size()-1){
		cout<<"error 104\n";system("pauxe");return;
	}
	while(true){
		//展示部分 
		system("cls");
		cout<<t+1000<<" "<<his[t].name<<endl;
		if(his[t].stime==-1){
			cout<<"error 101\n";
		}
		else stamp_to_standard(his[t].stime,true),cout<<endl;
		if(his[t].ftime){
			cout<<"planning time:";
			if(his[t].ftime==-1){
				cout<<"error 101\n";
			}
			else
				stamp_to_standard(his[t].ftime,true);
			cout<<"\n";
		}
		if(his[t].rtime){
			cout<<"real time:";
			if(his[t].rtime==-1){
				cout<<"error 101\n";
			}
			else
				stamp_to_standard(his[t].rtime,true);
			cout<<"\n已完成\n";
		}
		//修改部分 
		cout<<"要修改哪一项？\n"; 
		switch(choose("name","beginning time","end time","descript")){
			case 0:{
				return;
				break;
			}
			case 1:{
				cout<<"输入新名称:";
				cin>>his[t].name;
				break;
			}
			case 2:{
				cout<<"*what beginning time it is?\n";
				switch(choose("now","input")){
					case 1:{
						his[t].stime=time(0);
						break;
					}
					case 2:{
						do{
							cout<<"time:";
							char a[21];
							while((a[0]=getchar())=='\n'||a[0]>'9'||a[0]<'0');
							memset(a,sizeof(a),0);
							for(int i=1;;i++){
								a[i]=getchar();
								if(a[i]=='\n'){
									a[i]=0;break;
								}
							}
							his[t].stime=standard_to_stamp(a);
							if(his[t].stime==-1) cout<<"输入有误 请重输\n";
						}while(his[t].stime==-1);
						break;
					}
					case 0:{
						return;
						break;
					}
				} 
				break;
			}
			case 3:{
				cout<<"what ending time it is?\n";
				switch(choose("no","input")){
					case 2:{
						do{
							cout<<"time:";
							char a[21];
							memset(a,sizeof(a),0);
							while((a[0]=getchar())=='\n'||a[0]>'9'||a[0]<'0');
							for(int i=1;;i++){
								a[i]=getchar();
								if(a[i]=='\n'){
									a[i]=0;break;
								}
							}
							his[t].ftime=standard_to_stamp(a);
							if(his[t].ftime==-1) cout<<"输入有误 请重输\n";
						}while(his[t].ftime==-1);
						break;
					}
					case 0:{
						return;
						break;
					}
					
				}
				break;
			}
		}
	}
	
}

IL void more()
{
	while(true){
		switch(choose("放入回收站","移动","回收站","编辑"))
		{
			case 0:{
				return;
				break;
			}
			case 1:{
				remove();
				break;
			}
			case 2:{
				move();
				break;
			}
			case 3:{
				system("cls");
				if(key()){
					bin();
				}
				break;
			}
			case 4:{
				if(key()){
					edit();
				}
				break;
			}
		}
		system("cls");
		show();
	}
	

}

IL void error()
{
	system("cls");
	for(int i=101;i<300;i++){
		if(err.find(i)!=err.end()) cout<<i<<" "<<err[i]<<endl;
	}
	system("pause");
}

IL void about()
{
	system("cls"); 
	cout<<"当前版本:"<<V()<<endl; 
	cout<<"妈妈给了本关于拖延症的书，叫《别让拖延症毁了你》，我看到标题就觉得这本书适合我（因为我觉得自己太拖延了！）\n"
		<<"里面有许多关于拖延症的原因，道理，逻辑方面的知识，也有许多小故事，每个故事都对应这一种类型的拖延症表现（当然也有正面的）\n"
		<<"其中就有个措施是“合理的制定计划”，我想，在这神奇的编程下，制定计划应该会很容易吧，就设想了几个比较不错的功能\n"
		<<"但是我到网上一搜，得到的代码都无法满足我的要求（虽然我的标准也不是恒定不变的，但我觉得我可以写出让我满意的代码）\n"
		<<"于是我就开始动手了\n\n" 
		; 
	cout<<"数据保存在E:\\task\\task.lib,可使用txt文本查看器打开\n";
	cout<<"初始密码:778205\n"; 
	cout<<"b 返回上一层\nf 启动搜索引擎\ns 保存\nx 保存并退出\nu 手动更新\n";
	cout<<"更新日志&官网:";//////////////////////////////////////////////////////////////////////////////////////////////////////// 
	system("pause");
	system("cls"); 
}

IL void help()
{
	while(true){
		switch(choose("error","about","MODE","color"))
		{
			case 0:{
				return;
				break;
			}
			case 1:{
				error();
				break;
			}
			case 2:{
				about();
				break;
			}
			case 3:{
				do{
					int ans=choose("精简模式","美观模式","普通模式");
					if(ans<=3&&ans>0) MODE=ans,system("cls"),show();
					else break;
				}while(true);
				break;
			}
			case 4:{
				while(true)
				switch(choose("白天模式","夜间模式","护眼模式","自定义")){
					case 0:{
						return;
						break;
					}
					case 1:{
						system("color f0");
						break;
					}
					case 2:{
						system("color 0f");
						break;
					}
					case 3:{
						system("color 30");
						break;
					}
					case 4:{
						system("color /?");
						string str;
						getline(cin,str);
						system(str.c_str());
						system("pause");
						break;
					}
				}
				break;
			}
			system("cls");
			show();
		}
	}
	

}

IL void save()
{
	ofstream fout("E:\\task\\task.lib",ios::out);
	fout<<his.size()<<endl;
	for(unsigned int i=0;i<his.size();i++){
		fout<<his[i].name<<endl<<his[i].stime<<" "<<his[i].ftime<<" "<<his[i].rtime<<endl<<endl;
	}
	fout<<del.size()<<endl;
	for(unsigned int i=0;i<del.size();i++){
		fout<<del[i].name<<endl<<del[i].stime<<" "<<del[i].ftime<<" "<<del[i].rtime<<endl<<endl;
	}
	fout<<cipher<<endl;
}

int main()
{
//	cout<<system("xcopy /f D:\task\task.lib E:\task\ ")<<endl;
//	system("pause");
	system("color 30");
	init();
	system("title Loading...");
	read(); 
    system("title Code by S.O.A.F");
    cout<<"唤醒欲望。若有想法，立即行动。\n";Sleep(1000);
    while(true){
    	system("cls");
    	show();
    	switch(choose("new task","finish task","more","help"))
		{
	    	case 1:{ 
				new_task();
				break;
			}
			case 2:{
				finish(); 
				break;
			}
			case 3:{
				more(); 
				break;
			}
			case 4:{
				help();
				break;
			}
		}
	}
	return 0;
}

void _return(){
	system("cls");
	save();
	cout<<"已自动保存";
	switch(choose("退出","取消","重启")) {
		case 1:{
			exit(0);
			break;
		}
		case 2:{
			return;
			break;
		}
		case 3:{
			main();
			exit(0);
			break;
		}
	}
	 
}

