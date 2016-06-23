
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

int main(int argc,char *argv[])
{
	Environment *env;    /**env1,*env2,*env3,*env4,*env5,*env6;*/
    Connection *conn;    /**conn1,*conn2,*conn3,*conn4,*conn5,*conn6;*/
    Statement *stmt;     /**stmt1,*stmt2,*stmt3,*stmt4,*stmt5,*stmt6;*/
    ResultSet *rs;       /**rs1,*rs2,*rs3,*rs4,*rs5,*rs6;*/
    string username =argv[1];
    string password = argv[2];
	string connstring = argv[3];
	string title=argv[4];//"gxuy_test";
	string div_name=argv[5];
	string table_name=argv[6];
	double sv=atof(argv[7]);	
    string sql;
	string sql1;
	string say1="",say2="",say3="";
	long start,end;
	string str1,str2,str3,str4;
	double aa=0,bb=0,cc=0,dd=0;
	int ii=0,jj=0,kk=0,n=0;
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
	int flag[600];
	

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
	vector <double> num1;
	vector <int> dec;

	vector <string> s;
	stringstream ss;
	string sss[310];

	
	for(jj=0;jj<310;jj++)
	{
		ss.str("");
		ss<<jj;
		sss[jj]=ss.str();
		
	}
	sql=" ";
	

    env = Environment::createEnvironment(); 
    conn = env->createConnection(username,password,connstring); 
    stmt = conn->createStatement();
	
	

	MetaData custtab_metaData = conn->getMetaData(table_name, MetaData::PTYPE_TABLE);     //输出表头
        vector <MetaData> listOfColumns = custtab_metaData.getVector(MetaData::ATTR_LIST_COLUMNS);
        int   uiColumnNum   =   listOfColumns.size(); 
        for   (ii=0;   ii <uiColumnNum;   ii++) 
        { 
        s.push_back(listOfColumns[ii].getString(MetaData::ATTR_NAME).c_str());//MetaData   columnObj   =   listOfColumns[ii]; 
	}
	cout<<"Att_num:"<<uiColumnNum<<endl;	

vector< vector<double> > num3(300);
	rs =  stmt->executeQuery(sql+"select * from "+title);
	while (rs->next())
	{
	n++;
	for(ii=0;ii<uiColumnNum;ii++)
	{
		num3[ii].push_back(rs->getDouble(ii+1));
	}
	}
	cout<<"read over,data size is "<<n<<endl;
	for(ii=0;ii<uiColumnNum;ii++)
	{

		if(flag[ii]==1)
		{ 
			rs =  stmt->executeQuery(sql+"select * from "+div_name+" where flag="+sss[ii+1]);
			while (rs->next())
			{
			aa=rs->getDouble(2);
			bb=rs->getDouble(3);
			}
			for(kk=0;kk<n;kk++)
			{
				if(num3[ii][kk]<aa) num3[ii][kk]=0;
				else if((num3[ii][kk]<bb)&&(num3[ii][kk]>=aa)) num3[ii][kk]=1;
				else if(num3[ii][kk]>=bb) num3[ii][kk]=2;
			}
		//cout<<ii+1<<"1st"<<endl;
		}

		if(flag[ii]==2)
		{
			
			rs =  stmt->executeQuery(sql+"select * from "+div_name+" where flag="+sss[ii+1]);
			while(rs->next())
			{
			aa=rs->getDouble(4);
			bb=rs->getDouble(5);
			cc=rs->getDouble(6);
			}
			for(kk=0;kk<n;kk++)
				{
					if(num3[ii][kk]<aa) num3[ii][kk]=0;
					else if((num3[ii][kk]<bb)&&(num3[ii][kk]>=aa)) num3[ii][kk]=1;
					else if((num3[ii][kk]<cc)&&(num3[ii][kk]>=bb)) num3[ii][kk]=2;
					else if(num3[ii][kk]>=cc) num3[ii][kk]=3;
				}
		//cout<<ii+1<<"2nd"<<endl;
		}


		if(flag[ii]==3)
		{
			
			rs =  stmt->executeQuery(sql+"select * from "+div_name+" where flag="+sss[ii+1]);
			while(rs->next())
			{
			aa=rs->getDouble(7);
			bb=rs->getDouble(8);
			cc=rs->getDouble(9);
			dd=rs->getDouble(10);
			}
			for(kk=0;kk<n;kk++)
				{
					if(num3[ii][kk]<=aa) num3[ii][kk]=0;
					else if((num3[ii][kk]<=bb)&&(num3[ii][kk]>aa)) num3[ii][kk]=1;
					else if((num3[ii][kk]<=cc)&&(num3[ii][kk]>bb)) num3[ii][kk]=2;
					else if((num3[ii][kk]<=dd)&&(num3[ii][kk]>cc)) num3[ii][kk]=3;
					else if(num3[ii][kk]>dd) num3[ii][kk]=4;
				}
		//cout<<ii+1<<"3rd"<<endl;
		}
		if(flag[ii]==4)
		{ 
			rs =  stmt->executeQuery(sql+"select * from "+div_name+" where flag="+sss[ii+1]);
			while (rs->next())
			{
			aa=rs->getDouble(2);
			bb=rs->getDouble(3);
			}
			for(kk=0;kk<n;kk++)
			{	
				if (num3[ii][kk]==sv) num3[ii][kk]=0;
				else if((num3[ii][kk]<=aa)&&(num3[ii][kk]!=sv)) num3[ii][kk]=1;
				else if((num3[ii][kk]<=bb)&&(num3[ii][kk]>aa)&&(num3[ii][kk]!=sv)) num3[ii][kk]=2;
				else if((num3[ii][kk]>bb)&&(num3[ii][kk]!=sv)) num3[ii][kk]=3;
			}
		//cout<<ii+1<<"4th"<<endl;
		}

		if(flag[ii]==5)
		{
			
			rs =  stmt->executeQuery(sql+"select * from "+div_name+" where flag="+sss[ii+1]);
			while(rs->next())
			{
			aa=rs->getDouble(4);
			bb=rs->getDouble(5);
			cc=rs->getDouble(6);
			}
			for(kk=0;kk<n;kk++)
				{
					if (num3[ii][kk]==sv) num3[ii][kk]=0;
					else if((num3[ii][kk]<=aa)&&(num3[ii][kk]!=sv)) num3[ii][kk]=1;
					else if((num3[ii][kk]<=bb)&&(num3[ii][kk]>aa)&&(num3[ii][kk]!=sv)) num3[ii][kk]=2;
					else if((num3[ii][kk]<=cc)&&(num3[ii][kk]>bb)&&(num3[ii][kk]!=sv)) num3[ii][kk]=3;
					else if((num3[ii][kk]>cc)&&(num3[ii][kk]!=sv)) num3[ii][kk]=4;
				}
		//cout<<ii+1<<"5th"<<endl;
		}
		if(flag[ii]==6)
		{
			
			rs =  stmt->executeQuery(sql+"select * from "+div_name+" where flag="+sss[ii+1]);
			while(rs->next())
			{
			aa=rs->getDouble(7);
			bb=rs->getDouble(8);
			cc=rs->getDouble(9);
			dd=rs->getDouble(10);
			}
			for(kk=0;kk<n;kk++)
				{
					if (num3[ii][kk]==sv) num3[ii][kk]=0;
					else if((num3[ii][kk]<=aa)&&(num3[ii][kk]!=sv)) num3[ii][kk]=1;
					else if((num3[ii][kk]<=bb)&&(num3[ii][kk]>aa)&&(num3[ii][kk]!=sv)) num3[ii][kk]=2;
					else if((num3[ii][kk]<=cc)&&(num3[ii][kk]>bb)&&(num3[ii][kk]!=sv)) num3[ii][kk]=3;
					else if((num3[ii][kk]<=dd)&&(num3[ii][kk]>cc)&&(num3[ii][kk]!=sv)) num3[ii][kk]=4;
					else if((num3[ii][kk]>dd)&&(num3[ii][kk]!=sv)) num3[ii][kk]=5;
				}
		//cout<<ii+1<<"6th"<<endl;
		}
	}
	cout<<"discretize over"<<endl;

	start=time(NULL);
	say1="";
	say2="";
	for(kk=0;kk<uiColumnNum;kk++)
	{
	say1=say1+s[kk]+",";
	say2=say2+":"+sss[kk+1]+",";
	}
	//cout<<"say1,2构造完成"<<endl;
	say1=say1.substr(0,say1.length()-1);
	say2=say2.substr(0,say2.length()-1);
	sql1=sql+"insert into "+table_name+"("+say1+") values("+say2+")";
	//sql1=sql+"insert into lisanhua(column1,column2,column3,column4,column5,column6,column7,column8,column9,column10,column11,column12,column13,column14,column15,column16,column17,column18,column19,column20,column21,column22,column23,column24,column25,column26,column27,column28,column29,column30,column31,column32,column33,column34,column35,column36,column37,column38,column39,column40,column41,column42,column43,column44,column45,column46,column47,column48,column49,column50,column51,column52,column53,column54,column55,column56,column57,column58,column59,column60,column61,column62,column63,column64,column65,column66,column67,column68,column69,column70,column71,column72,column73,column74,column75,column76,column77,column78,column79,column80,column81,column82,column83,column84,column85,column86,column87,column88,column89,column90,column91,column92,column93,column94,column95,column96,column97,column98) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,:11,:12,:13,:14,:15,:16,:17,:18,:19,:20,:21,:22,:23,:24,:25,:26,:27,:28,:29,:30,:31,:32,:33,:34,:35,:36,:37,:38,:39,:40,:41,:42,:43,:44,:45,:46,:47,:48,:49,:50,:51,:52,:53,:54,:55,:56,:57,:58,:59,:60,:61,:62,:63,:64,:65,:66,:67,:68,:69,:70,:71,:72,:73,:74,:75,:76,:77,:78,:79,:80,:81,:82,:83,:84,:85,:86,:87,:88,:89,:90,:91,:92,:93,:94,:95,:96,:97,:98)";
	stmt->setSQL(sql1);
	stmt->setMaxIterations(n);
	for(ii=0;ii<uiColumnNum;ii++)
	{
	stmt->setMaxParamSize(ii+1,sizeof(double));
	}
	for(kk=0;kk<n;kk++)
	{
	//sql1="insert into lisanhua(column1,column2,column3,column4,column5,column6,column7,column8,column9,column10,column11,column12,column13,column14,column15,column16,column17,column18,column19,column20,column21,column22,column23,column24,column25,column26,column27,column28,column29,column30,column31,column32,column33,column34,column35,column36,column37,column38,column39,column40,column41,column42,column43,column44,column45,column46,column47,column48,column49,column50,column51,column52,column53,column54,column55,column56,column57,column58,column59,column60,column61,column62,column63,column64,column65,column66,column67,column68,column69,column70,column71,column72,column73,column74,column75,column76,column77,column78,column79,column80,column81,column82,column83,column84,column85,column86,column87,column88,column89,column90,column91,column92,column93,column94,column95,column96,column97,column98) values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,:11,:12,:13,:14,:15,:16,:17,:18,:19,:20,:21,:22,:23,:24,:25,:26,:27,:28,:29,:30,:31,:32,:33,:34,:35,:36,:37,:38,:39,:40,:41,:42,:43,:44,:45,:46,:47,:48,:49,:50,:51,:52,:53,:54,:55,:56,:57,:58,:59,:60,:61,:62,:63,:64,:65,:66,:67,:68,:69,:70,:71,:72,:73,:74,:75,:76,:77,:78,:79,:80,:81,:82,:83,:84,:85,:86,:87,:88,:89,:90,:91,:92,:93,:94,:95,:96,:97,:98)";
	//stmt->setSQL(sql1);
	for(ii=0;ii<uiColumnNum;ii++)   
		{
			stmt->setDouble(ii+1,num3[ii][kk]);
		}
	if (kk<n-1) stmt->addIteration();
	//stmt->executeUpdate();
	}
	stmt->executeUpdate();
	
	
	try
	{
	stmt->setSQL(" drop table table_sort5");
	stmt->executeUpdate();
	}
	catch(SQLException &ex) 
	{
		cout << ex.what() << endl;
	}
	stmt->setSQL(" create table table_sort5 as select * from "+table_name+" order by id_1 asc,id_2 asc");
	stmt->executeUpdate();
	cout<<"sort over"<<endl;
	
	end=time(NULL);
	//cout<<"insert time is "<<end-start<<endl;
	conn->terminateStatement(stmt); 
    env->terminateConnection(conn); 
    Environment::terminateEnvironment(env);
	
	
	

	
	cout << "PROGRAM IS END ." << endl;
	//system("pause");

	return 0;
}
	
