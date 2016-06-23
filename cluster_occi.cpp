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

Environment *env;    /**env1,*env2,*env3,*env4,*env5,*env6;*/
Connection *conn;    /**conn1,*conn2,*conn3,*conn4,*conn5,*conn6;*/
Statement *stmt;     /**stmt1,*stmt2,*stmt3,*stmt4,*stmt5,*stmt6;*/
ResultSet *rs;       /**rs1,*rs2,*rs3,*rs4,*rs5,*rs6;*/
string username = "";
string password = "";
string connstring = "";
string title="";string title2;
string sqlcmd;
string syscmd;
vector <string> filename;
vector <string> tablename;


string its(int a)
{
stringstream ss;
string str;
ss<<a;
ss>>str;
return str;
}

void getnames()
{
ifstream read;
read.open("cluster_name.txt");
string temp;
while(getline(read,temp))
{
tablename.push_back(temp);
}
string a="train_";string b="test_";string c;
for(int i=0;i<5;i++)
{
	c=a+its(i)+".txt";
	filename.push_back(c);
	c=b+its(i)+".txt";
	filename.push_back(c);
}
read.close();
}




int main(int argc,char *argv[])
{
	ifstream read;
	ofstream write;
	ofstream log_file;
	log_file.open("log_for_tableready.txt");
	if(argc>5)
	{
		username=argv[5];
		password=argv[6];
		connstring=argv[7];
	}
	else
	{
		username = "ucr_zs_south";
		password = "Zd_0918_Model";
		connstring = "//132.121.86.53:1521/zdxmdb";
	}
	
	vector <string> s;
    env = Environment::createEnvironment("ZHS16GBK","ZHS16GBK"); 
    conn = env->createConnection(username,password,connstring); 
    stmt = conn->createStatement();
	int index=0;
	
	string hzl_fnbase=argv[1];//file
	string hzl_ytbase=argv[2];//yuanshi
	string hzl_ttbese=argv[3];//target
	int m_size=atoi(argv[4]);
	while(index<m_size){
	string fname=hzl_fnbase+its(index)+".txt";
	syscmd="./idwrite cluster_id "+fname+" "+username+" "+password+" "+connstring;
	log_file<<"syscmd:"<<syscmd<<endl;
	//cout<<"syscmd:"<<syscmd<<endl;
	system(syscmd.c_str());

	title=hzl_ttbese+its(index);
	sqlcmd=" drop table "+title;
	cout<<title<<endl;
	try
	{
	log_file<<"sqlcmd:"<<sqlcmd<<endl;
	stmt->setSQL(sqlcmd);
	stmt->executeUpdate();
	}
	catch(SQLException &ex) 
	{
		log_file << ex.what() << endl;
	}
	sqlcmd="CREATE TABLE "+title+" AS(SELECT * FROM "+hzl_ytbase+" where user_id in (select user_id from cluster_id))";
	log_file<<"sqlcmd:"<<sqlcmd<<endl;
	stmt->setSQL(sqlcmd);
	stmt->executeUpdate();
	index++;
	}
	log_file.close();
	return 0;
}