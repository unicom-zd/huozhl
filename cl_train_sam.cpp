#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include<vector>
#include<string>
#include<cstring>
#include<sstream>
using namespace std;

string username = "ucr_zs_south";
string password = "Zd_0918_Model";
string connstring = "//132.121.86.53:1521/zdxmdb";
string train_name = "";
string test_name = "";
string res_name =" ";
string radio_name=" ";
string syscmd;
int cluster_size;


string its(int a)
{
stringstream ss;
string str;
ss<<a;
ss>>str;
return str;
}


int count_check()
{
	int a=0;
	ifstream r;string temp;int count=0;
	r.open("T1_prunedTree_data.txt");
	while(getline(r,temp)&&count<3)
	{
		count++;
	}
	r.close();
	if(count==1)
	{
		a++;
		count=0;
		r.open("T1_rawTree_data.txt");
		while(getline(r,temp)&&count<3)
		{
			count++;
		}
		r.close();
		if(count==1) a++;
	}
	return a;
}



int main(int argc,char *argv[])
{

ofstream w1,w2;
ifstream r1,r2;	
train_name=argv[1];
test_name=argv[2];
res_name=argv[3];
radio_name=argv[4];
cluster_size=atoi(argv[5]);
string sam_value=argv[6];
if(argc<8) 
{
	username = "ucr_zs_south";
    password = "Zd_0918_Model";
    connstring = "//132.121.86.53:1521/zdxmdb";
}
else
{
	username = argv[7];
    password = argv[8];
    connstring = argv[9];
}
/* argv get!!! */

w1.open("cl_train_log.txt");
w2.open(radio_name.c_str());

int ii=0;
while(ii<cluster_size)
{
	w2<<"cluster "<<ii<<":"<<endl;
	w1<<"cluster "<<ii<<":"<<endl;
	syscmd="./tableready";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="./cluster_sam "+train_name+its(ii)+" "+sam_value;
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="./discrete/method_discrete 1 ";
	syscmd+=username;syscmd+=" ";
	syscmd+=password;syscmd+=" ";
	syscmd+=connstring;syscmd+=" "; 
	syscmd+=train_name+its(ii)+"_sam";
	syscmd+=" div_test_sam -1";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;

	syscmd="./discrete/test ";
	syscmd+=username;syscmd+=" ";
	syscmd+=password;syscmd+=" ";
	syscmd+=connstring;syscmd+=" "; 
	syscmd+=train_name+its(ii)+"_sam"+" ";
	syscmd+="div_test_sam zz_train_clus -1";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="./redata table_sort5 table_sort.csv "+username+" "+password+" "+connstring;
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="cp table_sort.csv T1.data";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="./c4.5 -f T1";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	int tag=count_check();
	cout<<"check_tag="<<tag<<endl;
	char special;
	if(tag==2)
	{
		r1.open("T1.data");
		string temp;
		getline(r1,temp);
		cout<<"All lost_tags in T1.data have same value:"<<temp[temp.length()-1]<<endl;
		r1.close();
		special=temp[temp.length()-1];
		ii++;
		//continue;
	}
	else
	{
		syscmd="cp T1_rawTree_data.txt T1_prunedTree_data.txt";
		cout<<syscmd<<endl;system(syscmd.c_str());
		w1<<syscmd<<endl;
		
		syscmd="cp T1_rawTree_struc.txt T1_prunedTree_struc.txt";
		cout<<syscmd<<endl;system(syscmd.c_str());
		w1<<syscmd<<endl;
	}
	
	syscmd="cp T1.data test.csv";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;

	syscmd="./check 1 0 80 800";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;	

	syscmd="./base 0";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="./discrete/test ";
	syscmd+=username;syscmd+=" ";
	syscmd+=password;syscmd+=" ";
	syscmd+=connstring;syscmd+=" "; 
	syscmd+=test_name+its(ii)+" ";
	syscmd+="div_test_sam zz_test1_clus -1";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="./redata table_sort5 table_sort.csv "+username+" "+password+" "+connstring;
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="cp table_sort.csv test.csv";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	if(tag==2)
	{		
		w2<<"All lost_tags in T1.data have same value:"<<special<<endl;
		int specount=0;string hzltemp;
		ifstream csvread;
		csvread.open("test.csv");
		while(getline(csvread,hzltemp))
		{
			specount++;
		}
		csvread.close();
		if(special=='0')
		{
			
			w2<<"模型预测为离网，实际上是离网的数据个数为：0"<<endl;
			w2<<"模型预测为在网，实际上是离网的数据个数为：0"<<endl;
			w2<<"模型预测为离网，实际上是在网的数据个数为：0"<<endl;
			w2<<"模型预测为在网，实际上是在网的数据个数为："<<specount<<endl;
			w2<<"模型查准率为："<<"0%"<<endl;
			w2<<"模型查全率为："<<"0%"<<endl;
		}
		if(special=='1')
		{
			
			w2<<"模型预测为离网，实际上是离网的数据个数为："<<specount<<endl;
			w2<<"模型预测为在网，实际上是离网的数据个数为：0"<<endl;
			w2<<"模型预测为离网，实际上是在网的数据个数为：0"<<endl;
			w2<<"模型预测为在网，实际上是在网的数据个数为：0"<<endl;
			w2<<"模型查准率为："<<"100%"<<endl;
			w2<<"模型查全率为："<<"100%"<<endl;
		}
		
	}
	
	
	
	syscmd="./check 1 0 80 800";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	r1.open("Radio.txt");
	string t;
	while(getline(r1,t))
	{
		w2<<t<<endl;
	}
	w2<<endl;
	r1.close();
	
	syscmd="./reswrite "+res_name+"tr_"+its(ii)+" test_result.txt "+username+" "+password+" "+connstring;
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="./myrule 0";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;

	syscmd="./base 1";
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;

	syscmd="./sqltest "+test_name+its(ii)+" out.txt "+username+" "+password+" "+connstring;
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="./newwrite "+res_name+"dr_"+its(ii)+" out.txt "+username+" "+password+" "+connstring;
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;
	
	syscmd="./saverule "+res_name+"rr_"+its(ii)+" "+username+" "+password+" "+connstring;
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;	
	
	syscmd="./weight_write "+res_name+"w_"+its(ii)+" "+username+" "+password+" "+connstring;
	cout<<syscmd<<endl;system(syscmd.c_str());
	w1<<syscmd<<endl;	
	ii++;
}	
	
	
return 0;
}