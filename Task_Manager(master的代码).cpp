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
string V(unsigned int dt=0){//�汾��û��ô������� 
	string ans="V";
	stringstream ss;
	ss<<vx<<"."<<vy+dt;
	ans+=ss.str();
	return ans;
}

POINT curpos;//����λ��

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
int choose(string a="��",string b="��",string c="��",string d="��");
void _return();
void find_task();

void init(){
	system("cls");
	his.clear();
	err.clear();
	temp.clear();
	del.clear();
	err[101]="ʱ������(-1)";
	err[102]="��������ݳ���";
	err[104]="����ı�Ŵ���"; 
}

IL void download(){
	system("cls"); 
	cout<<"ֱ���Ƿ��������°汾��Task_Manager.exe?";
	if(judge()){
		switch(choose("��ת��ҳ����","certutil����(��֧��win xp)")){
			case 0:{
				return;
				break;
			}
			case 1:{
				cout<<"����������� ��ɱ����������\n";
				system("pause");
				system("start https://www.cnblogs.com/send-off-a-friend/p/12461040.html");
				break;
			}
			case 2:{
				system("cls");
				stringstream ss;string str,ver;
				cout<<"�������� Task_Manager_"<<V(1)<<".exe"<<endl<<"����ɱ����������"<<endl;
				system("pause");
				ss<<"certutil -urlcache -split -f https://files-cdn.cnblogs.com/files/send-off-a-friend/Task_Manager_"<<V(1)<<".xml Task_Manager_"<<V(1)<<".exe";
				str=ss.str();
				long long ret=system(str.c_str());
				if(ret==-2145844848) cout<<"��ѽ(?��?��?��)!!�޸��µİ汾\n";
				if(ret==0) cout<<"���سɹ� ���Բ����ҿ� �����رճ��򣡤��褦�ʤ飡\n",system("pause"),exit(0);
				system("pause");
				break;
			}
			/* 
			case 3:{
				system("cls");
				stringstream ss;string str,ver;
				cout<<"�������� Task_Manager_"<<V(1)<<".exe"<<endl<<"����ɱ����������"<<endl;
				system("pause");
				ss<<"bitsadmin /Transfer Task_Manager_"<<V(1)<<" https://files-cdn.cnblogs.com/files/send-off-a-friend/Task_Manager_"<<V(1)<<".xml Task_Manager_"<<V(1)<<".exe";
				str=ss.str();cout<<str<<endl;
				long long ret=system(str.c_str());
				cout<<ret<<endl;
				if(ret==-2145844848) cout<<"��ѽ(?��?��?��)!!�޸��µİ汾\n";
				if(ret==0) cout<<"���سɹ� ���Բ����ҿ� �����رճ��򣡤��褦�ʤ飡\n",system("pause"),exit(0);
				if(ret==1) cout<<"1";
				if(ret==-2147024809) cout<<"����������ʲô��\n";
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
unsigned int pass=-20000;//�Է��ո�����ʱ���������� 
IL bool key(){
	if(clock()-pass<=20000) return 1;//20���������� 
	cout<<"input the key:";
	string str;
	char ch;
	while(true){
		ch=getch();
		if(ch=='\r'||ch=='\n') break;
		if(ch=='\b'){
			if(!str.empty()) cout<<"\b \b",str.pop_back();//string.pop_back()��c++11�ĺ���!!!����-std=c++11! 
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
	if(a!="��") all++;
	if(b!="��") all++;
	if(c!="��") all++; 
	if(d!="��") all++;  
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
		gotoxy(50,18);cout<<"�����밴b��";
		gotoxy(50,19);cout<<"����s���Ա�������";
		gotoxy(50,20);cout<<"����x���Ա��沢�˳�"; 
		gotoxy(50,21);cout<<"������1,2,3,4ѡ��";
		gotoxy(50,22);cout<<"��������Ļظ�";
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
		gotoxy(50,10);cout<<"�����밴b��";
		gotoxy(50,11);cout<<"����s���Ա�������";
		gotoxy(50,12);cout<<"����x���Ա��沢�˳�"; 
		gotoxy(50,13);cout<<"������1,2,3,4ѡ��";
		gotoxy(50,14);cout<<"��������Ļظ�";
	}
	if(MODE==3){
		cout<<"                                                  \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
		cout<<"\n1."<<a;cout<<"2."<<b;cout<<"3."<<c;cout<<"4."<<d;
		cout<<"\n�����밴b��"<<endl<<"����s���Ա�������"<<endl<<"����x���Ա��沢�˳�"<<endl<<"������1,2,3,4ѡ��"<<endl<<"��������Ļظ�\n";
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
			save();cout<<"����ɹ�";Sleep(2000);cout<<"\b\b  \b\b\b\b  \b\b\b\b  \b\b\b\b  \b\b";
		}
		if(ans=='b'||ans=='B'){
			backxy();
			return 0;
		} 
	} 
}

//ʱ���ת���ɱ�׼ʱ�� 
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

//��׼ʱ��׼����ʱ��� 
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
	//printf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);*/   //��׼ʱ���ʽ���磺2016:08:02 12:12:30
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
	cout<<"����ؼ���\n"; 
	string str;cin>>str;

	vector<mem>value;
	for(unsigned int i=0;i<his.size();i++){
		value.push_back({0,i});
		//ȫ��ƥ�� 
		for(int j=0;j+str.size()<his[i].name.size();j++)
		for(unsigned int k=0;k<str.size();k++)
		{
			if(his[i].name[j+k]!=str[k]) break;
			if(k==str.size()-1) value[i]+=10,value[i].point.push_back(j);
		}
		//����ƥ�� 
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
			cout<<"\n�����\n"; 
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
		cout<<"ȷ��Ҫ������";
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
				if(now.stime==-1) cout<<"�������� ������\n";
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
				if(now.ftime==-1) cout<<"�������� ������\n";
			}while(now.ftime==-1);
			break;
		}
		case 0:{
			return;
			break;
		}
		
	}
	
	cout<<"save?";
	switch(choose("����","��ȻҪ���棡","������")){
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
			cout<<"\n�����\n"; 
		}
		if(MODE==2)
			cout<<endl<<endl;
	}
}

IL void read(){
	ifstream fin("E:\\task\\task.lib",ios::in);
	if(fin.eof()){
		cout<<"������ǳ���ʹ�ã����Ƚ���help->about�鿴���ڴ��������ز���\n";
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
	cout<<"��������ɵ�������\n";
	unsigned int num;num=readud();
	if(num>(unsigned int)-3) return;
	if(num>=1000) num-=1000;
	if(num<0||num>=his.size()){
		cout<<"error 104\n";system("pause");return;
	} 
	if(his[num].rtime)
	{
		cout<<"�������Ѿ�����ˡ��㻹����\n";
		switch(choose("ȡ�����","�޸����ʱ��","ɾ��������","����ɾ��������(��ĺܾ�)"))
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
					if(his[num].rtime==-1) cout<<"�������� ������\n";
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
		cout<<"ʲôʱ����ɵģ�\n";
		switch(choose("�ո�","֮ǰ")) {
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
					if(his[num].rtime==-1) cout<<"�������� ������\n";
				}while(his[num].rtime==-1);
				break;
			}
		}
	}
	
}

IL void remove()
{
	switch(choose("ɾ�����������","ɾ��һ��","����ɾ��һ��(��ĺܾ�)")){
		case 0:{
			return;
			break;
		}
		case 1:{
			for(int i=his.size()-1;i>=0;i--) if(his[i].rtime) remove_one(i,1);
			break;
		}
		case 2:{
			cout<<"����Ҫɾ����������\n";
			unsigned int num=readud();
			remove_one(num,1); 
			break;
		}
		case 3:{
			if(!key()) break;
			cout<<"����Ҫɾ����������\n";
			int num;cin>>num;
			remove_one(num,3); 
			break;
		}
	}
}

IL void move()
{
	while(true){
		switch(choose("����","ѡ��һ��","���","��һ������"))
		{
			case 0:{
				return;
				break;
			}
			case 1:{
				cout<<"��������\n";
				switch(choose("��ʼʱ��","����","Ԥ��ʱ��","��ת"))
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
				cout<<"����ѡ������ı��\n";
				unsigned int t;
				cin>>t;
				if(t>=1000) t-=1000;
				cout<<"����s ����x ����b\n";
				char ch=getch();
				while(ch!='b'){
					if(ch=='s'){
						if(t>0) swap(his[t],his[t-1]),t--;
						else cout<<"�Ѿ�������!\n";
					}
					if(ch=='x'){
						if(t<his.size()-1) swap(his[t],his[t+1]),t++;
						else cout<<"�Ѿ�������!\n";
					}
					system("cls");
					show();
					cout<<"����s ����x ����b\n";
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
			cout<<"\n�����\n"; 
		}
		cout<<endl<<endl;
	}
	
	switch(choose("��ԭ","����ɾ��һ��","�������Ͱ"))
	{
		case 0:{
			system("cls");
			show();
			return;
			break;
		}
		case 1:{
			cout<<"����Ҫ��ԭ��������\n";
			int num;cin>>num;
			remove_one(num,4); 
			break;
		}
		case 2:{
			cout<<"����Ҫɾ����������\n";
			int num;cin>>num;
			remove_one(num,2); 
			break;
		}
		case 3:{
			cout<<"��ȷ��Ҫ�������Ͱ��\n";
			if(judge()) del.clear(); 
			break;
		}
	}
	system("cls");
	show(); 
} 

IL void edit()
{
	cout<<"����Ҫ�޸ĵ�������\n";
	unsigned int t;cin>>t;
	if(t>999) t-=1000;
	if(t<0||t>his.size()-1){
		cout<<"error 104\n";system("pauxe");return;
	}
	while(true){
		//չʾ���� 
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
			cout<<"\n�����\n";
		}
		//�޸Ĳ��� 
		cout<<"Ҫ�޸���һ�\n"; 
		switch(choose("name","beginning time","end time","descript")){
			case 0:{
				return;
				break;
			}
			case 1:{
				cout<<"����������:";
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
							if(his[t].stime==-1) cout<<"�������� ������\n";
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
							if(his[t].ftime==-1) cout<<"�������� ������\n";
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
		switch(choose("�������վ","�ƶ�","����վ","�༭"))
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
	cout<<"��ǰ�汾:"<<V()<<endl; 
	cout<<"������˱���������֢���飬�С���������֢�����㡷���ҿ�������;����Ȿ���ʺ��ң���Ϊ�Ҿ����Լ�̫�����ˣ���\n"
		<<"����������������֢��ԭ�򣬵����߼������֪ʶ��Ҳ�����С���£�ÿ�����¶���Ӧ��һ�����͵�����֢���֣���ȻҲ������ģ�\n"
		<<"���о��и���ʩ�ǡ�������ƶ��ƻ��������룬��������ı���£��ƶ��ƻ�Ӧ�û�����װɣ��������˼����Ƚϲ���Ĺ���\n"
		<<"�����ҵ�����һ�ѣ��õ��Ĵ��붼�޷������ҵ�Ҫ����Ȼ�ҵı�׼Ҳ���Ǻ㶨����ģ����Ҿ����ҿ���д����������Ĵ��룩\n"
		<<"�����ҾͿ�ʼ������\n\n" 
		; 
	cout<<"���ݱ�����E:\\task\\task.lib,��ʹ��txt�ı��鿴����\n";
	cout<<"��ʼ����:778205\n"; 
	cout<<"b ������һ��\nf ������������\ns ����\nx ���沢�˳�\nu �ֶ�����\n";
	cout<<"������־&����:";//////////////////////////////////////////////////////////////////////////////////////////////////////// 
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
					int ans=choose("����ģʽ","����ģʽ","��ͨģʽ");
					if(ans<=3&&ans>0) MODE=ans,system("cls"),show();
					else break;
				}while(true);
				break;
			}
			case 4:{
				while(true)
				switch(choose("����ģʽ","ҹ��ģʽ","����ģʽ","�Զ���")){
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
    cout<<"���������������뷨�������ж���\n";Sleep(1000);
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
	cout<<"���Զ�����";
	switch(choose("�˳�","ȡ��","����")) {
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

