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
string username = "ucr_zs_south";
string password = "Ora_123QWE";
string connstring = "//132.121.86.53:1521/zdxmdb";
string title="sysu_test_y";
string sql;
string sql1;
string say1="",say2="",say3="";
long start,end;
string str1,str2,str3,str4;
double aa=0,bb=0,cc=0,dd=0;
int ii=0,jj=0,kk=0,n=0;

int datasize=10000000;

int main(int argc, char* argv[]){
	
	ofstream write;
	write.open(argv[2]);
	title=argv[1];
	vector <string> s;
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
	cout<<"att_num:"<<uiColumnNum<<endl;	
	rs =  stmt->executeQuery(sql+"select * from "+title);
	while (rs->next())
	{
	n++;
	for(ii=0;ii<uiColumnNum-1;ii++)
	{
		write<<rs->getString(ii+1)<<",";
	}
	write<<rs->getString(uiColumnNum)<<endl;
	}
	write.close();
	cout<<"write over,data size = "<<n<<endl;
	conn->terminateStatement(stmt); 
    env->terminateConnection(conn); 
    Environment::terminateEnvironment(env);
	return 0;
	}
