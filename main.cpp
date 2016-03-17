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
        double* intimacy;//���ܶ�
        int number;
        int ranking;
        double rank;//���ս��
        double last;//��һ��������
        int size;
        double activity;
        static double act_sum;//��Ծ�Ⱥ�
        double inti_sum;//���ܶȺ�
        static int num;
        user()
        {
            inti_sum=0;
            ranking=0;
        }
        void init1(int si,bool flagact,user*a,bool flagname)//���룺��С����Ծ�ȣ��Ƿ��趨��ʼȨֵ
        {
            size=si;
            number=++num;
            intimacy=new double[si];
            for(int j=0;j<size;j++)//�����ݣ��������
            {
                if(fscanf(fpdata,"%lf",&intimacy[j])==EOF)//����������ݵĸ�ʽ�Ƿ���ȷ����֤����Ľ�׳��
                {
                    cout<<"�����ļ����ܶ����ݸ�ʽ����ȷ"<<endl;
                    fclose(fpdata);
                    fclose(fpact);
                    fname.close();
                    exit(0);
                }
                if(number==(j+1))
                    intimacy[j]=0;
                if(intimacy[j]<0)
                    intimacy[j]=-intimacy[j];//������ת��Ϊ��������֤����׳
                a[j].inti_sum+=intimacy[j];
            }
            if(flagact)
            {
                if(fscanf(fpact,"%lf",&activity)==EOF)//����������ݵĸ�ʽ�Ƿ���ȷ����֤����Ľ�׳��
                {
                    cout<<"�����ļ���Ծ�����ݸ�ʽ����ȷ"<<endl;
                    fclose(fpdata);
                    fclose(fpact);
                    fname.close();
                    exit(0);
                }
                if(activity<0)
                    activity=-activity;//������ת��Ϊ��������֤����׳
                act_sum+=activity;
            }
            else
            {
                activity=1;
                act_sum++;
            }
            if(flagname)
            {
                if(fname.eof())//����������ݵĸ�ʽ�Ƿ���ȷ����֤����Ľ�׳��
                {
                    cout<<"�����ļ��������ݸ�ʽ����ȷ"<<endl;
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
                name="δ����";
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
        void move()//����ǰһ���
        {
            last=rank;
            rank=0;
        }
        void printname()
        {
            cout<<"������"<<name<<"  ";
        }
        void printrank()
        {
            printf("�����罻������%3.1f  ",rank*size*100);
        }
        void printranking()
        {
            printf("��%3d�� ",ranking);
        }
        void printnumber()
        {
            printf("��%3d����  ",number);
        }
};
class list//���а�
{
    public:
        user** NO;
        int size;
        list(int _size,user* a):size(_size)
        {
            int s=-1;
            NO=new user*[size];
            for(int i=0;i<size;i++)//��ʼ��
            {
                NO[i]=NULL;
            }
            for(int i=0;i<size;i++)//ѡ������
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
void mul(int size,user* a)//�����㷨��ͨ�������������rankֵ
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
bool over(int size,user* a)//�ж��Ƿ����
{
    bool flag=false;
    for(int i=0;i<size;i++)
    {
        if(abs(a[i].last-a[i].rank)>=1e-10)
            flag=true;
    }
    return flag;
}
void print1(int size,user* a,list l)//��˳����ʾȫ�����
{
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            ��ӭʹ�ñ�����                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           �ó�����������о�ѧϰ���Ͻ������κ�������;��            ||\n");
    printf("   ||             �ó�����ɵ�һ�к�������߲��е��κ�����                ||\n");
    printf("   ||            ��Ȩ���� δ��������� ���ø��ơ���ӡ������             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           ���ߣ����ȡ���������ڡ������̡���ˬ    ||\n");
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
    cout<<endl<<"ע�������罻����Ϊ��ȡ��Χ�ڵ����ֵ����׼ֵΪ100��ֵԽ�������罻����Խǿ"<<endl;
    cout<<"�����������"<<endl;
    getchar();
    system("cls");
}
void print2(int size,user* a,list l)//��������ʾȫ�����
{
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            ��ӭʹ�ñ�����                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           �ó�����������о�ѧϰ���Ͻ������κ�������;��            ||\n");
    printf("   ||             �ó�����ɵ�һ�к�������߲��е��κ�����                ||\n");
    printf("   ||            ��Ȩ���� δ��������� ���ø��ơ���ӡ������             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           ���ߣ����ȡ���������ڡ������̡���ˬ    ||\n");
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
    cout<<endl<<"ע�������罻����Ϊ��ȡ��Χ�ڵ����ֵ����׼ֵΪ100��ֵԽ�������罻����Խǿ"<<endl;
    cout<<"�����������"<<endl;
    getchar();
    system("cls");
}
void print3(int size,user* a,list l)//����Ų���
{
    string num;
    int n;
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            ��ӭʹ�ñ�����                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           �ó�����������о�ѧϰ���Ͻ������κ�������;��            ||\n");
    printf("   ||             �ó�����ɵ�һ�к�������߲��е��κ�����                ||\n");
    printf("   ||            ��Ȩ���� δ��������� ���ø��ơ���ӡ������             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           ���ߣ����ȡ���������ڡ������̡���ˬ    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    while(1)
    {
        n=0;
        cout<<"������������"<<endl;
        cin>>num;
        n=atoi(num.c_str());//stringתint
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
    cout<<endl<<"ע�������罻����Ϊ��ȡ��Χ�ڵ����ֵ����׼ֵΪ100��ֵԽ�������罻����Խǿ"<<endl;
    }
    else
    {
        cout<<"�޴���"<<endl;
    }
    cout<<"�����������"<<endl;
    getchar();
    system("cls");
}
void print4(int size,user* a,list l)//����������
{
    string num;
    int n;
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            ��ӭʹ�ñ�����                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           �ó�����������о�ѧϰ���Ͻ������κ�������;��            ||\n");
    printf("   ||             �ó�����ɵ�һ�к�������߲��е��κ�����                ||\n");
    printf("   ||            ��Ȩ���� δ��������� ���ø��ơ���ӡ������             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           ���ߣ����ȡ���������ڡ������̡���ˬ    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    while(1)
    {
        n=0;
        cout<<"��������������"<<endl;
        cin>>num;
        n=atoi(num.c_str());//stringתint
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
    cout<<endl<<"ע�������罻����Ϊ��ȡ��Χ�ڵ����ֵ����׼ֵΪ100��ֵԽ�������罻����Խǿ"<<endl;
    }
    else
    {
        cout<<"�޴���"<<endl;
    }
    cout<<"�����������"<<endl;
    getchar();
    system("cls");
}
void print5(int size,user* a,list l)//����������
{
    string s;
    bool flag=true;
    fflush(stdin);
    system("cls");
    printf("   *************************************************************************\n");
    printf("   ||                            ��ӭʹ�ñ�����                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           �ó�����������о�ѧϰ���Ͻ������κ�������;��            ||\n");
    printf("   ||             �ó�����ɵ�һ�к�������߲��е��κ�����                ||\n");
    printf("   ||            ��Ȩ���� δ��������� ���ø��ơ���ӡ������             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           ���ߣ����ȡ���������ڡ������̡���ˬ    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    cout<<"����������"<<endl;
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
        cout<<"�޴���"<<endl;
    }
    else
    {
        cout<<endl<<"ע�������罻����Ϊ��ȡ��Χ�ڵ����ֵ����׼ֵΪ100��ֵԽ�������罻����Խǿ"<<endl;
    }
    fflush(stdin);
    cout<<"�����������"<<endl;
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
    printf("   ||                            ��ӭʹ�ñ�����                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           �ó�����������о�ѧϰ�� �Ͻ������κ�������;��            ||\n");
    printf("   ||             �ó�����ɵ�һ�к�������߲��е��κ�����                ||\n");
    printf("   ||            ��Ȩ���� δ��������� ���ø��ơ���ӡ������             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           ���ߣ����ȡ���������ڡ������̡���ˬ    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
        cout<<"1.��˳����ʾȫ�����"<<endl;
        cout<<"2.��������ʾȫ�����"<<endl;
        cout<<"3.����Ų���"<<endl;
        cout<<"4.����������"<<endl;
        cout<<"5.����������"<<endl;
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
    printf("   ||                            ��ӭʹ�ñ�����                         ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||           �ó�����������о�ѧϰ���Ͻ������κ�������;��            ||\n");
    printf("   ||             �ó�����ɵ�һ�к�������߲��е��κ�����                ||\n");
    printf("   ||            ��Ȩ���� δ��������� ���ø��ơ���ӡ������             ||\n");
    printf("   ||                                                                     ||\n");
    printf("   ||                           ���ߣ����ȡ���������ڡ������̡���ˬ    ||\n");
    printf("   ||                                                                     ||\n");
    printf("   *************************************************************************\n");
    cout<<endl;
    do
    {
        cout<<"�����뱣���û����ܶ����ݵ��ļ�����·��"<<endl;
        cin>>file;
        fpdata=fopen(file.data(),"r");//��string����ת����char*����
        if(!fpdata)//��֤�ļ���ȷ��
        {
            cout<<"�ļ�����·����������ļ���ʧ�ܣ�����������"<<endl;
        }
    }while(!fpdata);
    cout<<"�ļ��򿪳ɹ�"<<endl;
    if(fscanf(fpdata,"%d",&size)==EOF)//�����ݣ�ͼ�Ĵ�С��ͬʱ����������ݵĸ�ʽ�Ƿ���ȷ����֤����Ľ�׳��
    {
        cout<<"�������ܶ������ļ�Ϊ��"<<endl;
        fclose(fpdata);
        return 0;
    }
    a=new user[size];
    cout<<"�Ƿ���Ҫ�����û���Ծ�ȣ�"<<endl<<"Y/N   ��ѡ�����Ĭ�������û���Ծ����ͬ"<<endl;
    do
    {
        cin>>cho;
    }while(cho!='n'&&cho!='N'&&cho!='y'&&cho!='Y');
    if(cho=='y'||cho=='Y')
    {
        do
        {
            flagact=true;
            cout<<"�����뱣���û���Ծ�����ݵ��ļ�����·��"<<endl;
            cin>>file;
            fpact=fopen(file.data(),"r");//��string����ת����char*����
            if(!fpact)//��֤�ļ���ȷ��
            {
                flagact=false;//��ʧ��
                cout<<"�ļ�����·����������ļ���ʧ�ܣ��Ƿ�������룿"<<endl<<"Y/N   ��ѡ�����Ĭ�������û���Ծ����ͬ"<<endl;
            }
            do
            {
                cin>>cho;
            }while(cho!='n'&&cho!='N'&&cho!='y'&&cho!='Y');
        }while((!fpact)&&(cho=='y'||cho=='Y'));
    }cout<<"�Ƿ���Ҫ�����û�����"<<endl<<"Y/N   ��ѡ�����Ĭ�������û�����Ϊδ����"<<endl;
    do
    {
        cin>>cho;
    }while(cho!='n'&&cho!='N'&&cho!='y'&&cho!='Y');
    if(cho=='y'||cho=='Y')
    {
        do
        {
            flagname=true;
            cout<<"�����뱣���û��������ݵ��ļ�����·��"<<endl;
            cin>>file;
            fname.open("file.data()");
            if(!fname.is_open())//��֤�ļ���ȷ��
            {
                flagname=false;//��ʧ��
                cout<<"�ļ�����·����������ļ���ʧ�ܣ��Ƿ�������룿"<<endl<<"Y/N   ��ѡ�����Ĭ�������û�����Ϊδ����"<<endl;
            }
            do
            {
                cin>>cho;
            }while(cho!='n'&&cho!='N'&&cho!='y'&&cho!='Y');
        }while((!fname.is_open())&&(cho=='y'||cho=='Y'));
    }
    for(int i=0;i<size;i++)//��һ�γ�ʼ��
    {
        a[i].init1(size,flagact,a,flagname);
    }
    for(int i=0;i<size;i++)//�ڶ��γ�ʼ��
    {
        a[i].init2(a);
    }
    do//�Ƚ���һ�����㣬���ж��Ƿ���Ҫ����
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
