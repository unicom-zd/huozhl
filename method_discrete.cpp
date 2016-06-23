
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <math.h>
#include "malloc.h"
#include<ctime>
#include <omp.h>
#include <occi.h>
#include <sstream>
#include<time.h>


using namespace std;
using namespace oracle::occi;


//储存原始数据
struct Data{
	double value;
	bool offline;

};

struct Max{
	double value;
	int inum;
	int jnum;
	int knum;
	int lnum;
};

//储存distinct值n
struct Dist{
	int order;
	int per0;
	int per1;
};
//储存跳步后用于计算的distinct值
struct Dist2{
	int order;
	int per0;
	int per1;
	int difforder;
};

int mymax (int X,int Y){
	if (X>Y) return X;
	else return Y;
};

int mymin (int X,int Y){
	if (X<Y) return X;
	else return Y;
};

bool mybool (int X){
	if (X==0)
		return false;
	else 
		return true;
};
//用于排序的比较函数
bool cmp(const Data &x ,const Data &y){
	if(x.value<y.value)
		return true;
	return false;
};

//计算信息熵
double info(double dTrue,double dFalse)
{
	double dInfo = 0.0;
	if (dTrue==0 || dFalse==0) return 0.0;
	dInfo = ((dTrue)*(log(dTrue/(dTrue+dFalse)))+(dFalse)*(log(dFalse/(dTrue+dFalse))))*(-1);
	return dInfo;
}
vector <double> num1;
vector< vector <double> > num2(210);



int huozhl_check(int hh,int n_size)
{
	double a=num2[hh][0];
	for(int ww=1;ww<n_size;ww++)
	{
	if (num2[hh][ww]!=a)
		return 1;
	}
	return 0;
}

int main(int argc,char* argv[])



{
	Environment *env;    /**env1,*env2,*env3,*env4,*env5,*env6;*/
    Connection *conn;    /**conn1,*conn2,*conn3,*conn4,*conn5,*conn6;*/
    Statement *stmt;     /**stmt1,*stmt2,*stmt3,*stmt4,*stmt5,*stmt6;*/
	ResultSet *rs;       /**rs1,*rs2,*rs3,*rs4,*rs5,*rs6;*/
 string username;
    string password;
    string connstring;
    string title;
	string outputtitle;
	double sv;
    if (atoi(argv[1])==0)
    {
    username = "ucr_zs_south";
    password = "Zd_0918_Model";
    connstring = "//132.121.86.53:1521/zdxmdb";
    title=argv[2];
	outputtitle=argv[3];
	sv=atof(argv[4]);
    }
    else
{
    username = argv[2];
    password = argv[3];
    connstring = argv[4];
    title=argv[5];
	outputtitle=argv[6];
	sv=atof(argv[7]);
} 
    string sql;
	string sql1;
	string say1="",say2="",say3="";
	long start,end;
	string str1,str2,str3,str4;
	double aa=0,bb=0,cc=0,dd=0;
	int ii=0,jj=0,kk=0,n=0;
	/*int flag[103]={0,0,0,0,0,0,0,0,2,0,
	               0,0,0,0,0,0,0,0,0,0,
				   2,2,2,2,2,2,2,2,2,2,
				   2,1,2,1,2,1,2,1,1,2,
				   2,3,2,3,2,1,2,3,2,1,
				   1,1,2,3,4,1,2,1,1,1,
				   1,2,2,2,2,1,2,3,4,2,
				   1,1,1,1,1,1,1,1,1,1,
				   1,1,1,1,1,1,1,1,1,1,
				   0,0,0,0,0,0,0,0,0,0,
				   1,2,3};*/
	/*int flag[100]={0,0,0,0,0,0,0,0,2,0,
				   0,0,0,0,0,0,0,0,0,0,
				   0,0,0,2,2,2,0,0,2,2,
				   2,2,2,2,2,0,2,0,2,2,
				   2,2,2,2,2,2,2,2,2,2,
				   2,2,0,2,0,2,0,0,0,0,
				   0,2,2,2,2,2,2,2,2,2,
				   2,2,2,2,0,2,2,2,2,2,
				   2,2,2,2,2,2,2,2,2,2,
				   2,2,2,2,2,2,2,2,2,2};*/
	/*int flag[99]={0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0};*/
	/*int flag[60]={0,0,0,0,0,2,2,2,2,2,
				  2,2,2,2,2,2,0,2,2,2,
				  2,0,2,0,0,2,2,2,2,2,
				  2,2,2,2,2,2,2,2,2,2,
				  2,0,2,2,2,2,2,2,2,2,
				  0,2,0,2,0,0};*/
	int flag[600];
	vector <int> dec;

	ifstream flagread;
	flagread.open("flag.txt");
	int v;char c;int mk=0;
	while(flagread>>v)
	{
		flagread>>c;
		flag[mk]=v;
		mk++;
	}
	flagread.close();
	
 	double t_start = clock();
	

	vector <string> s;
	stringstream ss;
	string sss[211];

	
	for(jj=0;jj<210;jj++)
	{
		ss<<jj;
		sss[jj]=ss.str();
		ss.str("");
	}
	sql=" ";
	try
	{
    env = Environment::createEnvironment(); 
    conn = env->createConnection(username,password,connstring); 
    stmt = conn->createStatement();


	MetaData custtab_metaData = conn->getMetaData(title, MetaData::PTYPE_TABLE);     //输出表头
        vector <MetaData> listOfColumns = custtab_metaData.getVector(MetaData::ATTR_LIST_COLUMNS);
        int   uiColumnNum   =   listOfColumns.size(); 
        for   (ii=0;   ii <uiColumnNum;   ii++) 
        { 
        s.push_back(listOfColumns[ii].getString(MetaData::ATTR_NAME).c_str());//MetaData   columnObj   =   listOfColumns[ii]; 
	}
	cout<<"Att_num:"<<uiColumnNum<<endl;	
	rs=stmt->executeQuery(sql+"select "+s[uiColumnNum-1]+" from "+title);
	while (rs->next())
	{
		dec.push_back(rs->getInt(1));
		n++;
	}
	//cout<<"总数是"<<n<<" 是否离网数据读入完成"<<endl;
	cout<<"data_size:"<<n<<endl;
	start=time(NULL);
	struct Data *data = new  struct Data[n];
	for (ii=0;ii<uiColumnNum;ii++)
	num2[ii].clear();
	rs =  stmt->executeQuery(sql+"select * from "+title);
	while (rs->next())
				{   for(ii=0;ii<uiColumnNum;ii++)
					{
						num2[ii].push_back(rs->getDouble(ii+1));
					}
				}
	
	
	
	end=time(NULL);
	
	//cout<<"读取时间为"<<end-start<<endl;






	//struct Data *data = new  struct Data[n];
	for(jj=0;jj<uiColumnNum;jj++)
	{
		if (flag[jj]==0)
		{ 
			stmt->executeUpdate(sql+"insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values("+sss[jj+1]+",-1,-1,-1,-1,-1,-1,-1,-1,-1)");
			cout<<"No."<<sss[jj+1]<<" is not coding"<<endl;
		}
		
		else if(huozhl_check(jj,n)==0)
		{
			
			stmt->executeUpdate(sql+"insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values("+sss[jj+1]+",5,6,7,8,9,10,11,12,13)");
			cout<<"No."<<sss[jj+1]<<" is not coding as we have same value in this attribute "<<endl;
			
		}
		
		
		else
		{
	//num1.clear();
	//rs =  stmt->executeQuery(sql+"select "+s[jj]+" from zs10mpublic");
	//while (rs->next())
				//{
					//num2[jj].push_back(rs->getDouble(1));
				//}
	
	for(ii=0;ii<n;ii++)
	{
		data[ii].value=num2[jj][ii];
		data[ii].offline=mybool(dec[ii]);
		//data[ii].offline=mybool(num2[uiColumnNum-1][ii]);
	}

	bool cmp(const Data &x ,const Data &y);//cmp函数,x<y 返回true，否则false
	sort(data,data+n,cmp);        //用sort函数排序，从小到大
	/*struct Data Temp;
	for(int i=0;i<n-1;i++)        //用冒泡法排序
	for(int j=0;j<n-1-i;j++)
	if(data[j].percent<data[j+1].percent){
	Temp=data[j];
	data[j]=data[j+1];
	data[j+1]=Temp;
	}*/
	double curvalue=data[0].value;
	double curvalue2=data[0].value;
	int cur0=0;
	int	cur1=0;
	int cur20=0;//不排除-1时候的记录前面一共多少个0
	int cur21=0;
	std::vector<Dist> diff;
	std::vector<Dist> diff2;
	struct Dist temp; 


	for (int i=0;i<n;i++)
	{   //cout<<data[i].value<<" "<<curvalue<<endl;
		/*if (data[i].offline==0)
			cur0++;              //之前有多少个0，包含自身
		if (data[i].offline==1)
			cur1++;              //之前有多少个1，包含自身
        */
		/*if ((i==0)&&(curvalue!=sv)) //先把第一个也要加入diff数组中
		{temp.order=i;
		temp.per0=cur0;   
		temp.per1=cur1;  
		diff.push_back(temp);
		} */
		if ((data[i].value!=curvalue)&&(data[i].value!=sv))//当当前样本value值变化时，添加样本到diff数组中,排除-1
		//if (data[i].value!=curvalue)  //当当前样本value值变化时，添加样本到diff数组中
		{temp.order=i;
		temp.per0=cur0;
		temp.per1=cur1;
		diff.push_back(temp);
		curvalue=data[i].value;        
		}
		if (data[i].offline==0)
			cur0++;
		if (data[i].offline==1)
			cur1++;
	}

	int diffsize=diff.size();//记录distinct值数量
	//cout<<diffsize<<endl;
	//for (int i=0;i<diffsize;i++)
	//outfp<<data[diff[i].order].value<<endl;
	int diffaver=int (n/diffsize);//记录每个值平均样本数
	double sum,stand;//计算distict值标准差
	sum=0.0;
	for (int i=1;i<diffsize;i++)
		sum+=(diff[i].order-diff[i-1].order)^2;
	stand=sqrt(sum);
	int tempp=0;
	struct Dist2 *newdiff =new struct Dist2[diffsize];//该数组用于存储用于遍历的distinct点
	int m=1;
	int step=int(sqrt(double(diffsize/2)));//基于distinct值数量计算跳步数
	newdiff[0].order=diff[0].order;
	newdiff[0].per0=diff[0].per0;
	newdiff[0].per1=diff[0].per1;
	newdiff[0].difforder=0;

	for (int i=1;i<diffsize;i++)//判断哪些distinct放到newdiff数组中用于计算
	{  tempp++;
	if ((diff[i].order-diff[i-1].order)>(diffaver+stand))//如果该distinct样本数量大于平均数量+标准差则放到newdiff
	{
		newdiff[m].order=diff[i].order;
		newdiff[m].per0=diff[i].per0;
		newdiff[m].per1=diff[i].per1;
		newdiff[m].difforder=i;
		/* 
		newdiff[m].per2=diff[i].per2;
		newdiff[m].per2=diff[i].per3;
		*/
		m++;
		tempp=0;
	}
	else
		if (tempp==step)//满跳步数即将当前样本放到newdiff中
		{
			newdiff[m].order=diff[i].order;
			newdiff[m].per0=diff[i].per0;
			newdiff[m].per1=diff[i].per1;
			newdiff[m].difforder=i;
			m++;
			tempp=0;
		}


	}


	//全部数据参与离散化，distinct值储存在diff2中，
	for (int i=0;i<n;i++)
	{   
		/*if (i==0) //第一个也要加入diff数组中
		{temp.order=i;
		temp.per0=cur20;   
		temp.per1=cur21;  
		diff2.push_back(temp);
		}*/
		if (data[i].value!=curvalue2)//当当前样本value值变化时，添加样本到diff数组中,排除-1
		//if (data[i].value!=curvalue)  //当当前样本value值变化时，添加样本到diff数组中
		{temp.order=i;
		temp.per0=cur20;
		temp.per1=cur21;
		diff2.push_back(temp);
		curvalue2=data[i].value;        
		}
		if (data[i].offline==0)
			cur20++;
		if (data[i].offline==1)
			cur21++;
	}

	int diffsize2=diff2.size();//记录distinct值数量
	int diffaver2=int (n/diffsize2);//记录每个值平均样本数
	double sum2,stand2;//计算distict值标准差
	sum2=0.0;
	for (int i=1;i<diffsize2;i++)
		sum2+=(diff2[i].order-diff2[i-1].order)^2;
	stand2=sqrt(sum2);
	int tempp2=0;
	struct Dist2 *newdiff2 =new struct Dist2[diffsize2];//该数组用于存储用于遍历的distinct点
	int m2=1;

	int step2=int(sqrt(double(diffsize2/2)));//基于distinct值数量计算跳步数
	newdiff2[0].order=diff2[0].order;
	newdiff2[0].per0=diff2[0].per0;
	newdiff2[0].per1=diff2[0].per1;
	newdiff2[0].difforder=0;
	for (int i=1;i<diffsize2;i++)//判断哪些distinct放到newdiff数组中用于计算
	{  tempp2++;
	if ((diff2[i].order-diff2[i-1].order)>(diffaver2+stand2))//如果该distinct样本数量大于平均数量+标准差则放到newdiff
	{
		newdiff2[m2].order=diff2[i].order;
		newdiff2[m2].per0=diff2[i].per0;
		newdiff2[m2].per1=diff2[i].per1;
		newdiff2[m2].difforder=i;
		/* 
		newdiff[m].per2=diff[i].per2;
		newdiff[m].per2=diff[i].per3;
		*/
		m2++;
		tempp2=0;
	}
	else
		if (tempp2==step2)//满跳步数即将当前样本放到newdiff中
		{
			newdiff2[m2].order=diff2[i].order;
			newdiff2[m2].per0=diff2[i].per0;
			newdiff2[m2].per1=diff2[i].per1;
			newdiff2[m2].difforder=i;
			m2++;
			tempp2=0;
		}


	}

	int newdiffsize=m;
	int newdiffsize2=m2;
	//cout<<diffsize<<endl;
	//cout<<diffsize2<<endl;
	//cout<<newdiffsize<<endl;
	//cout<<newdiffsize2<<endl;
	ofstream outfp;
	outfp.open("D:\\work\\联通数据挖掘\\输出\\离散输出测试数据17.csv");
	int p1=0;
	int p2=1;
	int p3=2;
	int p4=3;

	std::vector <Max> iMax;
	struct Max maxtemp;


	//进行newdiff数组遍历，求最小信息熵 0是不离散化 1是全部数据3分 2是4分 3是5分 4是除-1外3分 5是除-1外4分 6是除-1外5分 
	//3分
	if(flag[jj]==1)
	{
	iMax.clear();
	p1=0;
	p2=1;
	p3=2;
	p4=3;
		double Entropy4=(info(newdiff2[0].per1,newdiff2[0].per0)  +  info(newdiff2[1].per1-newdiff2[0].per1,newdiff2[1].per0-newdiff2[0].per0)  +  info(cur21-newdiff2[1].per1,cur20-newdiff2[1].per0));//初始化最小信息熵
	maxtemp.value=Entropy4;
	maxtemp.inum=0;
	maxtemp.jnum=1;

	for (int i=0;i<newdiffsize2-1;i++)
		iMax.push_back(maxtemp);
	#pragma omp parallel for schedule(dynamic)
	for (int i=0;i<newdiffsize2-1;i++)
	{	
		double curEntropy;
		double iEntropy=info(newdiff2[i].per1,newdiff2[i].per0);//记录第一个区间的信息熵，避免之后重复运算
		for (int j=i+1;j<newdiffsize2;j++)
		{ curEntropy=( iEntropy+  info(newdiff2[j].per1-newdiff2[i].per1,newdiff2[j].per0-newdiff2[i].per0)  +  info(cur21-newdiff2[j].per1,cur20-newdiff2[j].per0));//计算当前遍历点信息熵
		  if  (iMax[i].value >curEntropy)//如发现更小的信息熵，即记录下来
			{         
				iMax[i].value=curEntropy;
				iMax[i].inum=i;
				iMax[i].jnum=j;
			}
		}
	}

	for (int i=0;i<newdiffsize2-1;i++)
	{ if (iMax[i].value<Entropy4)
	  {Entropy4=iMax[i].value;
	   p1=iMax[i].inum;  //分割点1的位置
	   p2=iMax[i].jnum;  //分割点2的位置
	  }
	}
	cout<<"3 part1 end"<<endl;//找到newdiff中最佳分割点，记录位置，存储在p1和p2中，最小信息熵值存储在Entropy4

	double tempEntropy=Entropy4;
	iMax.clear();
	maxtemp.value=Entropy4;
	for (int i=mymax(newdiff2[mymax(p1-1,1)].difforder,1);i<mymin(newdiff2[p1+1].difforder,newdiff2[p2].difforder);i++)
		iMax.push_back(maxtemp);
	//cout<<iMax.size()<<endl;
	
	//寻找局部最优
	#pragma omp parallel for schedule(dynamic)
	for (int i=mymax(newdiff2[mymax(p1-1,1)].difforder,1);i<mymin(newdiff2[p1+1].difforder,newdiff2[p2].difforder);i++)//在上方计算出的点附近搜寻，以计算局部最优点，mymax与mymin控制点不会溢出
	{	
	double curEntropy;
	double iEntropy=info(diff2[i].per1,diff2[i].per0);
		for (int j=mymax(newdiff2[p2-1].difforder,i+1);j<mymin(newdiff2[p2+1].difforder,diffsize2);j++)
		{ 	
			curEntropy=( iEntropy+  info(diff2[j].per1-diff2[i].per1,diff2[j].per0-diff2[i].per0)  +  info(cur21-diff2[j].per1,cur20-diff2[j].per0));
			int tempp1=i-mymax((newdiff2[p1-1].difforder),1);
			if  (iMax[tempp1].value >curEntropy)
			{         
				iMax[tempp1].value=curEntropy;
				iMax[tempp1].inum=i;  
				iMax[tempp1].jnum=j;  
			}
		}
	}
	for (int i=0;i<int(iMax.size());i++)
		{ if (iMax[i].value<Entropy4)
			  {Entropy4=iMax[i].value;
			   p1=iMax[i].inum;    //分割点1在diff集合里面的位置
			   p2=iMax[i].jnum;    //分割点2在diff集合里面的位置
			  }
		}
	if (tempEntropy==Entropy4)
		{
				aa=(data[newdiff2[p1].order].value);
				bb=(data[newdiff2[p2].order].value);
			    sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setDouble(2,aa);
				stmt->setDouble(3,bb);
				stmt->setInt(4,-1);
				stmt->setInt(5,-1);
				stmt->setInt(6,-1);
				stmt->setInt(7,-1);
				stmt->setInt(8,-1);
				stmt->setInt(9,-1);
				stmt->setInt(10,-1);
				stmt->executeUpdate();
		
				cout<<sss[jj+1]<<" in three-divide finished"<<endl;
	}

	else
	{
				aa=data[diff2[p1].order].value;
				bb=data[diff2[p2].order].value;
				sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setDouble(2,aa);
				stmt->setDouble(3,bb);
				stmt->setInt(4,-1);
				stmt->setInt(5,-1);
				stmt->setInt(6,-1);
				stmt->setInt(7,-1);
				stmt->setInt(8,-1);
				stmt->setInt(9,-1);
				stmt->setInt(10,-1);
				stmt->executeUpdate();
				
				cout<<sss[jj+1]<<" in three-divide finished"<<endl;
	}
	}

	

	
	//4分法
	else if(flag[jj]==2)
	{
	iMax.clear();
	p1=0;
	p2=1;
	p3=2;
	p4=3;
	
	double Entropy5=(info(newdiff2[0].per1,newdiff2[0].per0)  +  info((newdiff2[1].per1-newdiff2[0].per1),(newdiff2[1].per0-newdiff2[0].per0)) +info((newdiff2[2].per1-newdiff2[1].per1),(newdiff2[2].per0-newdiff2[1].per0))  +  info((cur21-newdiff2[2].per1),(cur20-newdiff2[2].per0)));
	maxtemp.value=Entropy5;
	maxtemp.inum=0;
	maxtemp.jnum=1;
	maxtemp.knum=2;
	for (int i=0;i<newdiffsize2-2;i++)
		iMax.push_back(maxtemp);
    #pragma omp parallel for schedule(dynamic)
	for (int i=0;i<newdiffsize2-2;i++)
	{	
		double curEntropy;
		double iEntropy=info(newdiff2[i].per1,newdiff2[i].per0);

		for (int j=i+1;j<newdiffsize2-1;j++)
		{ 
			double jEntropy=info(newdiff2[j].per1-newdiff2[i].per1,newdiff2[j].per0-newdiff2[i].per0);
			for(int k=j+1;k<newdiffsize2;k++)
			{ curEntropy=( iEntropy+   jEntropy +info(newdiff2[k].per1-newdiff2[j].per1,newdiff2[k].per0-newdiff2[j].per0)+  info(cur21-newdiff2[k].per1,cur20-newdiff2[k].per0));
			  if  (iMax[i].value >curEntropy)//如发现更小的信息熵，即记录下来
					{         
						iMax[i].value=curEntropy;
						iMax[i].inum=i;
						iMax[i].jnum=j;
						iMax[i].knum=k;
					}
			}
		}
	}

	for (int i=0;i<newdiffsize2-2;i++)
	{ if (iMax[i].value<Entropy5)
	  {Entropy5=iMax[i].value;
	   p1=iMax[i].inum;
	   p2=iMax[i].jnum;
	   p3=iMax[i].knum;
	  }
	}
	cout<<"4 part1 end"<<endl;
	double tempEntropy=Entropy5;
	iMax.clear();
	maxtemp.value=Entropy5;

	for (int i=(mymax(newdiff2[p1-1].difforder,1));i<mymin(newdiff2[p1+1].difforder,newdiff2[p2].difforder);i++)
		iMax.push_back(maxtemp);
	//4分局部最优
	#pragma omp parallel for schedule(dynamic)
	for (int i=(mymax(newdiff2[p1-1].difforder,1));i<mymin(newdiff2[p1+1].difforder,newdiff2[p2].difforder);i++)
	{	
		double curEntropy;
		double iEntropy=info(diff2[i].per1,diff2[i].per0);
		for (int j=mymax(newdiff2[p2-1].difforder,i+1);j<mymin(newdiff2[p2+1].difforder,newdiff2[p3].difforder);j++)
		{ 
			double jEntropy=info(diff2[j].per1-diff2[i].per1,diff2[j].per0-diff2[i].per0);
			for (int k=mymax(newdiff2[p3-1].difforder,j+1);k<mymin(newdiff2[p3+1].difforder,diffsize);k++)
			{
				curEntropy=( iEntropy+ jEntropy+info(diff2[k].per1-diff2[j].per1,diff2[k].per0-diff2[j].per0)  +  info(cur21-diff2[k].per1,cur20-diff2[k].per0));
				int tempp1=i-mymax((newdiff2[p1-1].difforder),1);
				if  (iMax[tempp1].value >curEntropy)
				{         
				 iMax[tempp1].value=curEntropy;
				 iMax[tempp1].inum=i;
				 iMax[tempp1].jnum=j;
				 iMax[tempp1].knum=k;
				}
			}
		}
	}

	for (int i=0;int(i<iMax.size());i++)
	{ if (iMax[i].value<Entropy5)
		  {Entropy5=iMax[i].value;
		   p1=iMax[i].inum;
		   p2=iMax[i].jnum;
		   p3=iMax[i].knum;
		  }
	}
	
	if (tempEntropy== Entropy5)
	{
				aa=data[newdiff2[p1].order].value;
				bb=data[newdiff2[p2].order].value;
				cc=data[newdiff2[p3].order].value;
			    sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setInt(2,-1);
				stmt->setInt(3,-1);
				stmt->setDouble(4,aa);
				stmt->setDouble(5,bb);
				stmt->setDouble(6,cc);
				stmt->setInt(7,-1);
				stmt->setInt(8,-1);
				stmt->setInt(9,-1);
				stmt->setInt(10,-1);
				stmt->executeUpdate();
			
				cout<<sss[jj+1]<<" in four-divide finished"<<endl;
	}

	else
	{
				aa=data[diff2[p1].order].value;
				bb=data[diff2[p2].order].value;
				cc=data[diff2[p3].order].value;
			    sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setInt(2,-1);
				stmt->setInt(3,-1);
				stmt->setDouble(4,aa);
				stmt->setDouble(5,bb);
				stmt->setDouble(6,cc);
				stmt->setInt(7,-1);
				stmt->setInt(8,-1);
				stmt->setInt(9,-1);
				stmt->setInt(10,-1);
				stmt->executeUpdate();

				cout<<sss[jj+1]<<" in four-divide finished"<<endl;
	}
	}


	//5分
	else if(flag[jj]==3)
	{
	double Entropy6=(info(diff2[0].per1,diff2[0].per0)  +  info((diff2[p1].per1-diff2[0].per1),(diff2[p1].per0-diff2[0].per0)) +info((diff2[p2].per1-newdiff2[p1].per1),(diff2[p2].per0-diff2[p1].per0))+info((diff2[p3].per1-diff2[p2].per1),(diff2[p3].per0-diff2[p2].per0))+info((diff2[p4].per1-diff2[p3].per1),(diff2[p4].per0-diff2[p3].per0))  +  info((cur21-diff2[p4].per1),(cur20-diff2[p4].per0)));

	int pp1,pp2,pp3,pp4;
	int ppp1=0,ppp2=p1,ppp3=p2,ppp4=p3;

	for (int i=0;i<diffsize2;i++)
	{	
       if ((i!=p1) && (i!=p2) &&(i!=p3))
		{
			if (i<p1)
			{
				pp4=p3;
				pp3=p2;
				pp2=p1;
				pp1=i;
		     }
			if ((i>p1) && (i<p2))
			{
				pp4=p3;
				pp3=p2;
				pp2=i;
				pp1=p1;
		     }
			if ((i>p2) && (i<p3))
			{
				pp4=p3;
				pp3=i;
				pp2=p2;
				pp1=p1;
		     }
			if (i>p3)
			{
				pp4=i;
				pp3=p3;
				pp2=p2;
				pp1=p1;
		     }
			double curEntropy=(info(diff2[pp1].per1,diff2[pp1].per0)  +  info((diff2[pp2].per1-diff2[pp1].per1),(diff2[pp2].per0-diff2[pp1].per0)) +info((diff2[pp3].per1-diff2[pp2].per1),(diff2[pp3].per0-diff2[pp2].per0))+info((diff2[pp4].per1-diff2[pp3].per1),(diff2[pp4].per0-diff2[pp3].per0))  +  info((cur21-diff2[pp4].per1),(cur20-diff2[pp4].per0)));
			if (curEntropy<=Entropy6)
			{ Entropy6=curEntropy;
			  ppp1=pp1;
			  ppp2=pp2;
			  ppp3=pp3;
			  ppp4=pp4;
	        }
	   }
	}

				

				aa=data[diff2[ppp1].order].value;
				bb=data[diff2[ppp2].order].value;
				cc=data[diff2[ppp3].order].value;
				dd=data[diff2[ppp4].order].value;
			    sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setInt(2,-1);
				stmt->setInt(3,-1);
				stmt->setInt(4,-1);
				stmt->setInt(5,-1);
				stmt->setInt(6,-1);
				stmt->setDouble(7,aa);
				stmt->setDouble(8,bb);
				stmt->setDouble(9,cc);
				stmt->setDouble(10,dd);
				stmt->executeUpdate();
				
				cout<<sss[jj+1]<<" in five-divide finished"<<endl;
	}


	//进行newdiff数组遍历，求最小信息熵
	//3分，除1 flag=4
	if(flag[jj]==4)
	{
	iMax.clear();
	p1=0;
	p2=1;
	p3=2;
	p4=3;
		double Entropy4=(info(newdiff[0].per1,newdiff[0].per0)  +  info(newdiff[1].per1-newdiff[0].per1,newdiff[1].per0-newdiff[0].per0)  +  info(cur1-newdiff[1].per1,cur0-newdiff[1].per0));//初始化最小信息熵
	maxtemp.value=Entropy4;
	maxtemp.inum=0;
	maxtemp.jnum=1;

	for (int i=0;i<newdiffsize-1;i++)
		iMax.push_back(maxtemp);
	#pragma omp parallel for schedule(dynamic)
	for (int i=0;i<newdiffsize-1;i++)
	{	
		double curEntropy;
		double iEntropy=info(newdiff[i].per1,newdiff[i].per0);//记录第一个区间的信息熵，避免之后重复运算
		for (int j=i+1;j<newdiffsize;j++)
		{ curEntropy=( iEntropy+  info(newdiff[j].per1-newdiff[i].per1,newdiff[j].per0-newdiff[i].per0)  +  info(cur1-newdiff[j].per1,cur0-newdiff[j].per0));//计算当前遍历点信息熵
		  if  (iMax[i].value >curEntropy)//如发现更小的信息熵，即记录下来
			{         
				iMax[i].value=curEntropy;
				iMax[i].inum=i;
				iMax[i].jnum=j;
			}
		}
	}

	for (int i=0;i<newdiffsize-1;i++)
	{ if (iMax[i].value<Entropy4)
	  {Entropy4=iMax[i].value;
	   p1=iMax[i].inum;  //分割点1的位置
	   p2=iMax[i].jnum;  //分割点2的位置
	  }
	}
	cout<<"3 part1 end"<<endl;//找到newdiff中最佳分割点，记录位置，存储在p1和p2中，最小信息熵值存储在Entropy4

	double tempEntropy=Entropy4;
	iMax.clear();
	maxtemp.value=Entropy4;
	for (int i=mymax(newdiff[mymax(p1-1,1)].difforder,1);i<mymin(newdiff[p1+1].difforder,newdiff[p2].difforder);i++)
		iMax.push_back(maxtemp);
	//cout<<iMax.size()<<endl;
	
	//寻找局部最优
	#pragma omp parallel for schedule(dynamic)
	for (int i=mymax(newdiff[mymax(p1-1,1)].difforder,1);i<mymin(newdiff[p1+1].difforder,newdiff[p2].difforder);i++)//在上方计算出的点附近搜寻，以计算局部最优点，mymax与mymin控制点不会溢出
	{	
	double curEntropy;
	double iEntropy=info(diff[i].per1,diff[i].per0);
		for (int j=mymax(newdiff[p2-1].difforder,i+1);j<mymin(newdiff[p2+1].difforder,diffsize);j++)
		{ 	
			curEntropy=( iEntropy+  info(diff[j].per1-diff[i].per1,diff[j].per0-diff[i].per0)  +  info(cur1-diff[j].per1,cur0-diff[j].per0));
			int tempp1=i-mymax((newdiff[p1-1].difforder),1);
			if  (iMax[tempp1].value >curEntropy)
			{         
				iMax[tempp1].value=curEntropy;
				iMax[tempp1].inum=i;  
				iMax[tempp1].jnum=j;  
			}
		}
	}
	for (int i=0;i<int(iMax.size());i++)
		{ if (iMax[i].value<Entropy4)
			  {Entropy4=iMax[i].value;
			   p1=iMax[i].inum;    //分割点1在diff集合里面的位置
			   p2=iMax[i].jnum;    //分割点2在diff集合里面的位置
			  }
		}
	if (tempEntropy==Entropy4)
		{
				aa=(data[newdiff[p1].order].value);
				bb=(data[newdiff[p2].order].value);
			    sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setDouble(2,aa);
				stmt->setDouble(3,bb);
				stmt->setInt(4,-1);
				stmt->setInt(5,-1);
				stmt->setInt(6,-1);
				stmt->setInt(7,-1);
				stmt->setInt(8,-1);
				stmt->setInt(9,-1);
				stmt->setInt(10,-1);
				stmt->executeUpdate();
				/*ss<<aa;
				str1=ss.str();
				ss.str("");
				ss<<bb;
				str2=ss.str();
				ss.str("");
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =0 where "+s[jj]+" <"+str1);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =1 where "+s[jj]+" >="+str1+" and "+s[jj]+" <"+str2);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =2 where "+s[jj]+" >="+str2);*/
				/*for(kk=0;kk<n;kk++)
				{
					if(num2[jj][kk]<aa) num2[jj][kk]=0;
					else if(num2[jj][kk]<bb && num2[jj][kk]>=aa) num2[jj][kk]=1;
					else if(num2[jj][kk]>=bb) num2[jj][kk]=2;
				}*/
				cout<<sss[jj+1]<<" in three-divide finished"<<endl;
	}
				//根据局部最优步骤是否有优化来调整输出
	//outfp<<'3'<<","<<data[newdiff[p1].order].value<<","<<data[newdiff[p2].order].value<<endl<<endl; 
	else
	{
				aa=data[diff[p1].order].value;
				bb=data[diff[p2].order].value;
				sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setDouble(2,aa);
				stmt->setDouble(3,bb);
				stmt->setInt(4,-1);
				stmt->setInt(5,-1);
				stmt->setInt(6,-1);
				stmt->setInt(7,-1);
				stmt->setInt(8,-1);
				stmt->setInt(9,-1);
				stmt->setInt(10,-1);
				stmt->executeUpdate();
				/*ss<<aa;
				str1=ss.str();
				ss.str("");
				ss<<bb;
				str2=ss.str();
				ss.str("");
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =0 where "+s[jj]+" <"+str1);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =1 where "+s[jj]+" >="+str1+" and "+s[jj]+"<"+str2);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =2 where "+s[jj]+" >="+str2);*/
				/*for(kk=0;kk<n;kk++)
				{
					if(num2[jj][kk]<aa) num2[jj][kk]=0;
					else if(num2[jj][kk]<bb && num2[jj][kk]>=aa) num2[jj][kk]=1;
					else if(num2[jj][kk]>=bb) num2[jj][kk]=2;
				}*/
				cout<<sss[jj+1]<<" in three-divide finished"<<endl;
	}
	}
	//outfp<<'3'<<","<<data[diff[p1].order].value<<","<<data[diff[p2].order].value<<endl<<endl;  
	

	
	//4分法，除1 flag=5
	else if(flag[jj]==5)
	{
	iMax.clear();
	p1=0;
	p2=1;
	p3=2;
	p4=3;
	
	double Entropy5=(info(newdiff[0].per1,newdiff[0].per0)  +  info((newdiff[1].per1-newdiff[0].per1),(newdiff[1].per0-newdiff[0].per0)) +info((newdiff[2].per1-newdiff[1].per1),(newdiff[2].per0-newdiff[1].per0))  +  info((cur1-newdiff[2].per1),(cur0-newdiff[2].per0)));
	maxtemp.value=Entropy5;
	maxtemp.inum=0;
	maxtemp.jnum=1;
	maxtemp.knum=2;
	for (int i=0;i<newdiffsize-2;i++)
		iMax.push_back(maxtemp);
    #pragma omp parallel for schedule(dynamic)
	for (int i=0;i<newdiffsize-2;i++)
	{	
		double curEntropy;
		double iEntropy=info(newdiff[i].per1,newdiff[i].per0);

		for (int j=i+1;j<newdiffsize-1;j++)
		{ 
			double jEntropy=info(newdiff[j].per1-newdiff[i].per1,newdiff[j].per0-newdiff[i].per0);
			for(int k=j+1;k<newdiffsize;k++)
			{ curEntropy=( iEntropy+   jEntropy +info(newdiff[k].per1-newdiff[j].per1,newdiff[k].per0-newdiff[j].per0)+  info(cur1-newdiff[k].per1,cur0-newdiff[k].per0));
			  if  (iMax[i].value >curEntropy)//如发现更小的信息熵，即记录下来
					{         
						iMax[i].value=curEntropy;
						iMax[i].inum=i;
						iMax[i].jnum=j;
						iMax[i].knum=k;
					}
			}
		}
	}

	for (int i=0;i<newdiffsize-2;i++)
	{ if (iMax[i].value<Entropy5)
	  {Entropy5=iMax[i].value;
	   p1=iMax[i].inum;
	   p2=iMax[i].jnum;
	   p3=iMax[i].knum;
	  }
	}
	cout<<"4 part1 end"<<endl;
	double tempEntropy=Entropy5;
	iMax.clear();
	maxtemp.value=Entropy5;

	for (int i=(mymax(newdiff[p1-1].difforder,1));i<mymin(newdiff[p1+1].difforder,newdiff[p2].difforder);i++)
		iMax.push_back(maxtemp);
	//4分局部最优
	#pragma omp parallel for schedule(dynamic)
	for (int i=(mymax(newdiff[p1-1].difforder,1));i<mymin(newdiff[p1+1].difforder,newdiff[p2].difforder);i++)
	{	
		double curEntropy;
		double iEntropy=info(diff[i].per1,diff[i].per0);
		for (int j=mymax(newdiff[p2-1].difforder,i+1);j<mymin(newdiff[p2+1].difforder,newdiff[p3].difforder);j++)
		{ 
			double jEntropy=info(diff[j].per1-diff[i].per1,diff[j].per0-diff[i].per0);
			for (int k=mymax(newdiff[p3-1].difforder,j+1);k<mymin(newdiff[p3+1].difforder,diffsize);k++)
			{
				curEntropy=( iEntropy+ jEntropy+info(diff[k].per1-diff[j].per1,diff[k].per0-diff[j].per0)  +  info(cur1-diff[k].per1,cur0-diff[k].per0));
				int tempp1=i-mymax((newdiff[p1-1].difforder),1);
				if  (iMax[tempp1].value >curEntropy)
				{         
				 iMax[tempp1].value=curEntropy;
				 iMax[tempp1].inum=i;
				 iMax[tempp1].jnum=j;
				 iMax[tempp1].knum=k;
				}
			}
		}
	}

	for (int i=0;int(i<iMax.size());i++)
	{ if (iMax[i].value<Entropy5)
		  {Entropy5=iMax[i].value;
		   p1=iMax[i].inum;
		   p2=iMax[i].jnum;
		   p3=iMax[i].knum;
		  }
	}
	
	if (tempEntropy== Entropy5)
	{
				aa=data[newdiff[p1].order].value;
				bb=data[newdiff[p2].order].value;
				cc=data[newdiff[p3].order].value;
			    sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setInt(2,-1);
				stmt->setInt(3,-1);
				stmt->setDouble(4,aa);
				stmt->setDouble(5,bb);
				stmt->setDouble(6,cc);
				stmt->setInt(7,-1);
				stmt->setInt(8,-1);
				stmt->setInt(9,-1);
				stmt->setInt(10,-1);
				stmt->executeUpdate();
				/*ss<<aa;
				str1=ss.str();
				ss.str("");
				ss<<bb;
				str2=ss.str();
				ss.str("");
				ss<<cc;
				str3=ss.str();
				ss.str("");
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =0 where "+s[jj]+" <"+str1);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =1 where "+s[jj]+" >="+str1+" and "+s[jj]+" <"+str2);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =2 where "+s[jj]+" >="+str2+" and "+s[jj]+" <"+str3);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =3 where "+s[jj]+" >="+str3);*/
				/*for(kk=0;kk<n;kk++)
				{
					if(num2[jj][kk]<aa) num2[jj][kk]=0;
					else if(num2[jj][kk]<bb && num2[jj][kk]>=aa) num2[jj][kk]=1;
					else if(num2[jj][kk]<cc && num2[jj][kk]>=bb) num2[jj][kk]=2;
					else if(num2[jj][kk]>=cc) num2[jj][kk]=3;
				}*/
				cout<<sss[jj+1]<<" in four-divide finished"<<endl;
	}
	//outfp<<'4'<<","<<data[newdiff[p1].order].value<<","<<data[newdiff[p2].order].value<<','<<data[newdiff[p3].order].value<<endl<<endl; 
	else
	{
				aa=data[diff[p1].order].value;
				bb=data[diff[p2].order].value;
				cc=data[diff[p3].order].value;
			    sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setInt(2,-1);
				stmt->setInt(3,-1);
				stmt->setDouble(4,aa);
				stmt->setDouble(5,bb);
				stmt->setDouble(6,cc);
				stmt->setInt(7,-1);
				stmt->setInt(8,-1);
				stmt->setInt(9,-1);
				stmt->setInt(10,-1);
				stmt->executeUpdate();
				/*ss<<aa;
				str1=ss.str();
				ss.str("");
				ss<<bb;
				str2=ss.str();
				ss.str("");
				ss<<cc;
				str3=ss.str();
				ss.str("");
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =0 where "+s[jj]+" <"+str1);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =1 where "+s[jj]+" >="+str1+" and "+s[jj]+" <"+str2);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =2 where "+s[jj]+" >="+str2+" and "+s[jj]+" <"+str3);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =3 where "+s[jj]+" >="+str3);*/
				/*for(kk=0;kk<n;kk++)
				{
					if(num2[jj][kk]<aa) num2[jj][kk]=0;
					else if(num2[jj][kk]<bb && num2[jj][kk]>=aa) num2[jj][kk]=1;
					else if(num2[jj][kk]<cc && num2[jj][kk]>=bb) num2[jj][kk]=2;
					else if(num2[jj][kk]>=cc) num2[jj][kk]=3;
				}*/
				cout<<sss[jj+1]<<" in four-divide finished"<<endl;
	}
	}
	//outfp<<'4'<<","<<data[diff[p1].order].value<<","<<data[diff[p2].order].value<<','<<data[diff[p3].order].value<<endl<<endl;  


	//5分 除1 flag=6
	else if(flag[jj]==6)
	{
	double Entropy6=(info(diff[0].per1,diff[0].per0)  +  info((diff[p1].per1-diff[0].per1),(diff[p1].per0-diff[0].per0)) +info((diff[p2].per1-newdiff[p1].per1),(diff[p2].per0-diff[p1].per0))+info((diff[p3].per1-diff[p2].per1),(diff[p3].per0-diff[p2].per0))+info((diff[p4].per1-diff[p3].per1),(diff[p4].per0-diff[p3].per0))  +  info((cur1-diff[p4].per1),(cur0-diff[p4].per0)));

	int pp1,pp2,pp3,pp4;
	int ppp1=0,ppp2=p1,ppp3=p2,ppp4=p3;
	//double Entropy6=(info(diff[ppp1].per1,diff[ppp1].per0)  +  info((diff[ppp2].per1-diff[ppp1].per1),(diff[ppp2].per0-diff[ppp1].per0)) +info((diff[ppp3].per1-diff[ppp2].per1),(diff[ppp3].per0-diff[ppp2].per0))+info((diff[ppp4].per1-diff[ppp3].per1),(diff[pp4].per0-diff[pp3].per0))  +  info((cur1-diff[pp4].per1),(cur0-diff[pp4].per0)));
	//outfp<<'5'<<","<<Entropy6<<","<<data[diff[ppp1].order].value<<","<<data[diff[ppp2].order].value<<','<<data[diff[ppp3].order].value<<','<<data[diff[ppp4].order].value<<endl<<endl; 
 //   #pragma omp parallel for schedule(dynamic)
	for (int i=0;i<diffsize;i++)
	{	
       if ((i!=p1) && (i!=p2) &&(i!=p3))
		{
			if (i<p1)
			{
				pp4=p3;
				pp3=p2;
				pp2=p1;
				pp1=i;
		     }
			if ((i>p1) && (i<p2))
			{
				pp4=p3;
				pp3=p2;
				pp2=i;
				pp1=p1;
		     }
			if ((i>p2) && (i<p3))
			{
				pp4=p3;
				pp3=i;
				pp2=p2;
				pp1=p1;
		     }
			if (i>p3)
			{
				pp4=i;
				pp3=p3;
				pp2=p2;
				pp1=p1;
		     }
			double curEntropy=(info(diff[pp1].per1,diff[pp1].per0)  +  info((diff[pp2].per1-diff[pp1].per1),(diff[pp2].per0-diff[pp1].per0)) +info((diff[pp3].per1-diff[pp2].per1),(diff[pp3].per0-diff[pp2].per0))+info((diff[pp4].per1-diff[pp3].per1),(diff[pp4].per0-diff[pp3].per0))  +  info((cur1-diff[pp4].per1),(cur0-diff[pp4].per0)));
			//outfp<<curEntropy<<","<<data[diff[ppp1].order].value<<","<<data[diff[ppp2].order].value<<","<<data[diff[ppp3].order].value<<","<<data[diff[ppp4].order].value<<endl;
			if (curEntropy<=Entropy6)
			{ Entropy6=curEntropy;
			  ppp1=pp1;
			  ppp2=pp2;
			  ppp3=pp3;
			  ppp4=pp4;
	        }
	   }
	}

				/*aa=(data[newdiff[p1].order].value);
				bb=(data[newdiff[p2].order].value);
				cc=data[newdiff[p3].order].value;
				dd=data[newdiff[p4].order].value;
			    sql1="insert into "+outputtitle+"(flag,five1,five2,five3,five4) values(:v,:w,:x,:y,:z)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setDouble(2,aa);
				stmt->setDouble(3,bb);
				stmt->setDouble(4,cc);
				stmt->setDouble(5,dd);
				stmt->executeUpdate();
				ss<<aa;
				str1=ss.str();
				ss.str("");
				ss<<bb;
				str2=ss.str();
				ss.str("");
				ss<<cc;
				str3=ss.str();
				ss.str("");
				ss<<dd;
				str4=ss.str();
				ss.str("");
				stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =0 where "+s[jj]+" <"+str1);
				stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =1 where "+s[jj]+" >="+str1+" and "+s[jj]+" <"+str2);
				stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =2 where "+s[jj]+" >="+str2+" and "+s[jj]+" <"+str3);
				stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =3 where "+s[jj]+" >="+str3+" and "+s[jj]+" <"+str4);
				stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =4 where "+s[jj]+" >="+str4);
				for(kk=0;kk<n;kk++)
				{
					if(num2[jj][kk]<aa) num2[jj][kk]=0;
					else if(num2[jj][kk]<bb && num2[jj][kk]>=aa) num2[jj][kk]=1;
					else if(num2[jj][kk]<cc && num2[jj][kk]>=bb) num2[jj][kk]=2;
					else if(num2[jj][kk]<dd && num2[jj][kk]>=cc) num2[jj][kk]=3;
					else if(num2[jj][kk]>=dd) num2[jj][kk]=4;
				}
				cout<<sss[jj+1]<<" in five-divide finished"<<endl;

	}*/

	//cout<<'5'<<","<<data[newdiff[p1].order].value<<","<<data[newdiff[p2].order].value<<','<<data[newdiff[p3].order].value<<','<<data[newdiff[p4].order].value<<endl<<endl; 

				aa=data[diff[ppp1].order].value;
				bb=data[diff[ppp2].order].value;
				cc=data[diff[ppp3].order].value;
				dd=data[diff[ppp4].order].value;
			    sql1="insert into "+outputtitle+"(flag,three1,three2,four1,four2,four3,five1,five2,five3,five4) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10)";
				stmt->setSQL(sql1);
				stmt->setInt(1,jj+1);
				stmt->setInt(2,-1);
				stmt->setInt(3,-1);
				stmt->setInt(4,-1);
				stmt->setInt(5,-1);
				stmt->setInt(6,-1);
				stmt->setDouble(7,aa);
				stmt->setDouble(8,bb);
				stmt->setDouble(9,cc);
				stmt->setDouble(10,dd);
				stmt->executeUpdate();
				/*ss<<aa;
				str1=ss.str();
				ss.str("");
				ss<<bb;
				str2=ss.str();
				ss.str("");
				ss<<cc;
				str3=ss.str();
				ss.str("");
				ss<<dd;
				str4=ss.str();
				ss.str("");
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =0 where "+s[jj]+" <"+str1);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =1 where "+s[jj]+" >="+str1+" and "+s[jj]+" <"+str2);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =2 where "+s[jj]+" >="+str2+" and "+s[jj]+" <"+str3);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =3 where "+s[jj]+" >="+str3+" and "+s[jj]+" <"+str4);
				//stmt->executeUpdate(sql+"update zs10mpublic set "+s[jj]+" =4 where "+s[jj]+" >="+str4);*/
				/*for(kk=0;kk<n;kk++)
				{
					if(num2[jj][kk]<aa) num2[jj][kk]=0;
					else if(num2[jj][kk]<bb && num2[jj][kk]>=aa) num2[jj][kk]=1;
					else if(num2[jj][kk]<cc && num2[jj][kk]>=bb) num2[jj][kk]=2;
					else if(num2[jj][kk]<dd && num2[jj][kk]>=cc) num2[jj][kk]=3;
					else if(num2[jj][kk]>=dd) num2[jj][kk]=4;
				}*/
				cout<<sss[jj+1]<<" in five-divide finished"<<endl;
	}
	

	//cout<<'5'<<","<<data[diff[p1].order].value<<","<<data[diff[p2].order].value<<','<<data[diff[p3].order].value<<','<<data[diff[p4].order].value<<endl<<endl;  
	//outfp.close();
	
	}
	} //离散化方案结束


	/*cout<<"start to insert"<<endl;
	start=time(NULL);
	for(jj=0;jj<uiColumnNum;jj++)
	{
	say1=say1+s[jj]+",";
	say2=say2+":"+sss[jj+1]+",";
	}
	say1=say1.substr(0,say1.length()-1);
	say2=say2.substr(0,say2.length()-1);
	sql1=sql+"insert into lisanhua("+say1+") values("+say2+")";
	//sql1=sql+"insert into lisanhua(column1,column2,column3,column4,column5,column6,column7,column8,column9,column10,column11,column12,column13,column14,column15,column16,column17,column18,column19,column20,column21,column22,column23,column24,column25,column26,column27,column28,column29,column30,column31,column32,column33,column34,column35,column36,column37,column38,column39,column40,column41,column42,column43,column44,column45,column46,column47,column48,column49,column50,column51,column52,column53,column54,column55,column56,column57,column58,column59,column60,column61,column62,column63,column64,column65,column66,column67,column68,column69,column70,column71,column72,column73,column74,column75,column76,column77,column78,column79,column80,column81,column82,column83,column84,column85,column86,column87,column88,column89,column90,column91,column92,column93,column94,column95,column96,column97,column98) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,:11,:12,:13,:14,:15,:16,:17,:18,:19,:20,:21,:22,:23,:24,:25,:26,:27,:28,:29,:30,:31,:32,:33,:34,:35,:36,:37,:38,:39,:40,:41,:42,:43,:44,:45,:46,:47,:48,:49,:50,:51,:52,:53,:54,:55,:56,:57,:58,:59,:60,:61,:62,:63,:64,:65,:66,:67,:68,:69,:70,:71,:72,:73,:74,:75,:76,:77,:78,:79,:80,:81,:82,:83,:84,:85,:86,:87,:88,:89,:90,:91,:92,:93,:94,:95,:96,:97,:98)";
	stmt->setSQL(sql1);
	stmt->setMaxIterations(1000000);
	for(jj=0;jj<uiColumnNum;jj++)
	{
	stmt->setMaxParamSize(jj+1,sizeof(double));
	}
	for(kk=0;kk<n;kk++)
	{
	//sql1="insert into lisanhua(column1,column2,column3,column4,column5,column6,column7,column8,column9,column10,column11,column12,column13,column14,column15,column16,column17,column18,column19,column20,column21,column22,column23,column24,column25,column26,column27,column28,column29,column30,column31,column32,column33,column34,column35,column36,column37,column38,column39,column40,column41,column42,column43,column44,column45,column46,column47,column48,column49,column50,column51,column52,column53,column54,column55,column56,column57,column58,column59,column60,column61,column62,column63,column64,column65,column66,column67,column68,column69,column70,column71,column72,column73,column74,column75,column76,column77,column78,column79,column80,column81,column82,column83,column84,column85,column86,column87,column88,column89,column90,column91,column92,column93,column94,column95,column96,column97,column98) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,:11,:12,:13,:14,:15,:16,:17,:18,:19,:20,:21,:22,:23,:24,:25,:26,:27,:28,:29,:30,:31,:32,:33,:34,:35,:36,:37,:38,:39,:40,:41,:42,:43,:44,:45,:46,:47,:48,:49,:50,:51,:52,:53,:54,:55,:56,:57,:58,:59,:60,:61,:62,:63,:64,:65,:66,:67,:68,:69,:70,:71,:72,:73,:74,:75,:76,:77,:78,:79,:80,:81,:82,:83,:84,:85,:86,:87,:88,:89,:90,:91,:92,:93,:94,:95,:96,:97,:98)";
	//stmt->setSQL(sql1);
	for(jj=0;jj<uiColumnNum;jj++)   
		{
			stmt->setDouble(jj+1,num2[jj][kk]);
		}
	if (kk!=n-1) stmt->addIteration();
	//stmt->executeUpdate();
	}
	stmt->executeUpdate();
	end=time(NULL);
	//cout<<"insert time is "<<end-start<<endl;*/

	
	conn->terminateStatement(stmt); 
    env->terminateConnection(conn); 
    Environment::terminateEnvironment(env);
	}


	catch(SQLException &ex) 
		{
			cout << ex.what() << endl;
		}
	cout << "PROGRAM IS END ." << endl;
	//system("pause");

	return 0;
}
