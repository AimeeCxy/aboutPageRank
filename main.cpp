#include<cstdio>
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<string>
#include <fstream>
using namespace std;
FILE* fpdata;
FILE* fpact;
ifstream fname;
const double alpha=0.8;
class user
{
    public:
        string name;
        double* intimacy;//亲密度
        int number;
        int ranking;
        double rank;//最终结果
        double last;//上一次运算结果
        int size;
        double activity;
        static double act_sum;//活跃度和
        double inti_sum;//亲密度和
        static int num;
        user()
        {
            inti_sum=0;
            ranking=0;
        }
        void init1(int si,bool flagact,user*a,bool flagname)//输入：大小，活跃度，是否设定初始权值
        {
            size=si;
            number=++num;
            intimacy=new double[si];
            for(int j=0;j<size;j++)//读数据，相联情况
            {
                if(fscanf(fpdata,"%lf",&intimacy[j])==EOF)//检测输入数据的格式是否正确，保证程序的健壮性
                {
                    cout<<"输入文件亲密度数据格式不正确"<<endl;
                    fclose(fpdata);
                    fclose(fpact);
                    fname.close();
                    exit(0);
                }
                if(number==(j+1))
                    intimacy[j]=0;
                if(intimacy[j]<0)
                    intimacy[j]=-intimacy[j];//将负数转化为正数，保证程序健壮
                a[j].inti_sum+=intimacy[j];
            }
            if(flagact)
            {
                if(fscanf(fpact,"%lf",&activity)==EOF)//检测输入数据的格式是否正确，保证程序的健壮性
                {
                    cout<<"输入文件活跃度数据格式不正确"<<endl;
                    fclose(fpdata);
                    fclose(fpact);
                    fname.close();
                    exit(0);
                }
                if(activity<0)
                    activity=-activity;//将负数转化为正数，保证程序健壮
                act_sum+=activity;
            }
            else
            {
                activity=1;
                act_sum++;
            }
            if(flagname)
            {
                if(fname.eof())//检测输入数据的格式是否正确，保证程序的健壮性
                {
                    cout<<"输入文件姓名数据格式不正确"<<endl;
                    fclose(fpdata);
                    fclose(fpact);
                    fname.close();
                    exit(0);
                }
                else
                {
                    getline(fname,name,'\10');
                }
            }
            else
            {
                name="未命名";
            }
        }
        void init2(user*a)
        {
            if(act_sum)
                activity/=act_sum;
            for(int i=0;i<size;i++)
            {
                if(inti_sum)
                    intimacy[i]/=a[i].inti_sum;
            }
            rank=activity;
            last=rank;
        }
        void move()//覆盖前一结果
        {
            last=rank;
            rank=0;
        }
        void printname()
        {
            cout<<"姓名："<<name<<"  ";
        }
        void printrank()
        {
            printf("网络社交能力：%3.1f  ",rank*size*100);
        }
        void printranking()
        {
            printf("第%3d名 ",ranking);
        }
        void printnumber()
        {
            printf("第%3d个人  ",number);
        }
};
class list//排行榜
{
    public:
        user** NO;
        int size;
        list(int _size,user* a):size(_size)
        {
            int s=-1;
            NO=new user*[size];
            for(int i=0;i<size;i++)//初始化
            {
                NO[i]=NULL;
            }
            for(int i=0;i<size;i++)//选择排序法
            {
                for(int j=0;j<size;j++)
                {
                    if(s==-1&&(!a[j].ranking))
                    {
                        s=j;
                    }
                    if(a[j].rank>a[s].rank&&(!a[j].ranking))
                    {
                        s=j;
                    }
                }
                a[s].ranking=i+1;
                NO[i]=&a[s];
                s=-1;
            }
        }
};
double user::act_sum=0;
int user::num=0;
void mul(int size,user* a)//核心算法，通过反复迭代求出rank值
{
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            a[i].rank+=(a[i].intimacy[j]*a[j].last);
        }
        a[i].rank*=alpha;
        a[i].rank+=(1-alpha)*a[i].activity;
    }
}
bool over(int size,user* a)//判断是否结束
{
    bool flag=false;
    for(int i=0;i<size;i++)
    {
        if(abs(a[i].last-a[i].rank)>=1e-10)
            flag=true;
    }
    return flag;
}
void print1(int size,user* a,list l)//按顺序显示全部结果
{
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            欢迎使用本程序！                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           该程序这可用作研究学习，严禁用于任何其他用途！            ||\n");
    printf("   ||             该程序造成的一切后果，作者不承担任何责任                ||\n");
    printf("   ||            版权所有 未经书面许可 不得复制、翻印、镜像！             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           作者：张钊、褚新宇、曾融、赵鹏程、马爽    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    for(int i=0;i<size;i++)
    {
        a[i].printnumber();
        a[i].printname();
        a[i].printrank();
        a[i].printranking();
        cout<<endl;
    }
    cout<<endl<<"注：网络社交能力为所取范围内的相对值，标准值为100，值越高网络社交能力越强"<<endl;
    cout<<"按任意键继续"<<endl;
    getchar();
    system("cls");
}
void print2(int size,user* a,list l)//按排名显示全部结果
{
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            欢迎使用本程序！                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           该程序这可用作研究学习，严禁用于任何其他用途！            ||\n");
    printf("   ||             该程序造成的一切后果，作者不承担任何责任                ||\n");
    printf("   ||            版权所有 未经书面许可 不得复制、翻印、镜像！             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           作者：张钊、褚新宇、曾融、赵鹏程、马爽    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    for(int i=0;i<size;i++)
    {
        a[l.NO[i]->number-1].printranking();
        a[l.NO[i]->number-1].printname();
        a[l.NO[i]->number-1].printnumber();
        a[l.NO[i]->number-1].printrank();
        cout<<endl;
    }
    cout<<endl<<"注：网络社交能力为所取范围内的相对值，标准值为100，值越高网络社交能力越强"<<endl;
    cout<<"按任意键继续"<<endl;
    getchar();
    system("cls");
}
void print3(int size,user* a,list l)//按编号查找
{
    string num;
    int n;
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            欢迎使用本程序！                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           该程序这可用作研究学习，严禁用于任何其他用途！            ||\n");
    printf("   ||             该程序造成的一切后果，作者不承担任何责任                ||\n");
    printf("   ||            版权所有 未经书面许可 不得复制、翻印、镜像！             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           作者：张钊、褚新宇、曾融、赵鹏程、马爽    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    while(1)
    {
        n=0;
        cout<<"请输入所查编号"<<endl;
        cin>>num;
        n=atoi(num.c_str());//string转int
        if(n)
            break;
    }
    fflush(stdin);
    if(n<=size)
    {
        a[n-1].printname();
        a[n-1].printnumber();
        a[n-1].printrank();
        a[n-1].printranking();
        cout<<endl;
    cout<<endl<<"注：网络社交能力为所取范围内的相对值，标准值为100，值越高网络社交能力越强"<<endl;
    }
    else
    {
        cout<<"无此人"<<endl;
    }
    cout<<"按任意键继续"<<endl;
    getchar();
    system("cls");
}
void print4(int size,user* a,list l)//按排名查找
{
    string num;
    int n;
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            欢迎使用本程序！                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           该程序这可用作研究学习，严禁用于任何其他用途！            ||\n");
    printf("   ||             该程序造成的一切后果，作者不承担任何责任                ||\n");
    printf("   ||            版权所有 未经书面许可 不得复制、翻印、镜像！             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           作者：张钊、褚新宇、曾融、赵鹏程、马爽    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    while(1)
    {
        n=0;
        cout<<"请输入所查名次"<<endl;
        cin>>num;
        n=atoi(num.c_str());//string转int
        if(n)
            break;
    }
    fflush(stdin);
    if(n<=size)
    {
        a[l.NO[n-1]->number-1].printranking();
        a[l.NO[n-1]->number-1].printname();
        a[l.NO[n-1]->number-1].printnumber();
        a[l.NO[n-1]->number-1].printrank();
        cout<<endl;
    cout<<endl<<"注：网络社交能力为所取范围内的相对值，标准值为100，值越高网络社交能力越强"<<endl;
    }
    else
    {
        cout<<"无此人"<<endl;
    }
    cout<<"按任意键继续"<<endl;
    getchar();
    system("cls");
}
void print5(int size,user* a,list l)//按姓名查找
{
    string s;
    bool flag=true;
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            欢迎使用本程序！                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           该程序这可用作研究学习，严禁用于任何其他用途！            ||\n");
    printf("   ||             该程序造成的一切后果，作者不承担任何责任                ||\n");
    printf("   ||            版权所有 未经书面许可 不得复制、翻印、镜像！             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           作者：张钊、褚新宇、曾融、赵鹏程、马爽    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    cout<<"请输入姓名"<<endl;
    cin>>s;
    for(int i=0;i<size;i++)
    {
        if(s==a[i].name)
        {
            a[i].printname();
            a[i].printnumber();
            a[i].printrank();
            a[i].printranking();
            cout<<endl;
            flag=false;
        }
    }
    if(flag)
    {
        cout<<"无此人"<<endl;
    }
    else
    {
        cout<<endl<<"注：网络社交能力为所取范围内的相对值，标准值为100，值越高网络社交能力越强"<<endl;
    }
    fflush(stdin);
    cout<<"按任意键继续"<<endl;
    getchar();
    system("cls");
}
void menu(int size,user* a,list l)
{
    char cho;
    fclose(fpdata);
    fclose(fpact);
    fname.close();
    while(1)
    {
        fflush(stdin);
        system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            欢迎使用本程序！                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           该程序这可用作研究学习， 严禁用于任何其他用途！            ||\n");
    printf("   ||             该程序造成的一切后果，作者不承担任何责任                ||\n");
    printf("   ||            版权所有 未经书面许可 不得复制、翻印、镜像！             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           作者：张钊、褚新宇、曾融、赵鹏程、马爽    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
        cout<<"1.按顺序显示全部结果"<<endl;
        cout<<"2.按排名显示全部结果"<<endl;
        cout<<"3.按编号查找"<<endl;
        cout<<"4.按排名查找"<<endl;
        cout<<"5.按姓名查找"<<endl;
        cin>>cho;
        switch(cho)
        {
            case '1':print1(size,a,l);break;
            case '2':print2(size,a,l);break;
            case '3':print3(size,a,l);break;
            case '4':print4(size,a,l);break;
            case '5':print5(size,a,l);break;
        }
    }
}
int main()
{
    string file;
    char cho;
    int size;
    user* a;
    bool flagact=false,flagname=false;
    printf("   *************************************************************************\n");
    printf("   ||                            欢迎使用本程序！                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           该程序这可用作研究学习，严禁用于任何其他用途！            ||\n");
    printf("   ||             该程序造成的一切后果，作者不承担任何责任                ||\n");
    printf("   ||            版权所有 未经书面许可 不得复制、翻印、镜像！             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           作者：张钊、褚新宇、曾融、赵鹏程、马爽    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    do
    {
        cout<<"请输入保存用户亲密度数据的文件名或路径"<<endl;
        cin>>file;
        fpdata=fopen(file.data(),"r");//将string类型转换成char*类型
        if(!fpdata)//保证文件正确打开
        {
            cout<<"文件名或路径输入错误，文件打开失败，请重新输入"<<endl;
        }
    }while(!fpdata);
    cout<<"文件打开成功"<<endl;
    if(fscanf(fpdata,"%d",&size)==EOF)//读数据，图的大小，同时检测输入数据的格式是否正确，保证程序的健壮性
    {
        cout<<"输入亲密度数据文件为空"<<endl;
        fclose(fpdata);
        return 0;
    }
    a=new user[size];
    cout<<"是否需要输入用户活跃度？"<<endl<<"Y/N   若选择否，则默认所有用户活跃度相同"<<endl;
    do
    {
        cin>>cho;
    }while(cho!='n'&&cho!='N'&&cho!='y'&&cho!='Y');
    if(cho=='y'||cho=='Y')
    {
        do
        {
            flagact=true;
            cout<<"请输入保存用户活跃度数据的文件名或路径"<<endl;
            cin>>file;
            fpact=fopen(file.data(),"r");//将string类型转换成char*类型
            if(!fpact)//保证文件正确打开
            {
                flagact=false;//打开失败
                cout<<"文件名或路径输入错误，文件打开失败，是否继续输入？"<<endl<<"Y/N   若选择否，则默认所有用户活跃度相同"<<endl;
            }
            do
            {
                cin>>cho;
            }while(cho!='n'&&cho!='N'&&cho!='y'&&cho!='Y');
        }while((!fpact)&&(cho=='y'||cho=='Y'));
    }cout<<"是否需要输入用户名？"<<endl<<"Y/N   若选择否，则默认所有用户名称为未命名"<<endl;
    do
    {
        cin>>cho;
    }while(cho!='n'&&cho!='N'&&cho!='y'&&cho!='Y');
    if(cho=='y'||cho=='Y')
    {
        do
        {
            flagname=true;
            cout<<"请输入保存用户名称数据的文件名或路径"<<endl;
            cin>>file;
            fname.open("file.data()");
            if(!fname.is_open())//保证文件正确打开
            {
                flagname=false;//打开失败
                cout<<"文件名或路径输入错误，文件打开失败，是否继续输入？"<<endl<<"Y/N   若选择否，则默认所有用户名称为未命名"<<endl;
            }
            do
            {
                cin>>cho;
            }while(cho!='n'&&cho!='N'&&cho!='y'&&cho!='Y');
        }while((!fname.is_open())&&(cho=='y'||cho=='Y'));
    }
    for(int i=0;i<size;i++)//第一次初始化
    {
        a[i].init1(size,flagact,a,flagname);
    }
    for(int i=0;i<size;i++)//第二次初始化
    {
        a[i].init2(a);
    }
    do//先进行一次运算，再判断是否需要继续
    {
        for(int i=0;i<size;i++)
            a[i].move();
        mul(size,a);
    }while(over(size,a));
    list l(size,a);
    fclose(fpdata);
    fclose(fpact);
    menu(size,a,l);
    return 0;
}
