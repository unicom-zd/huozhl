#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include<vector>
#include<string>
#include<cstring>
using namespace std;

string username = "ucr_zs_south";
string password = "Zd_0918_Model";
string connstring = "//132.121.86.53:1521/zdxmdb";


int main(int argc,char *argv[])

{	
	if(argc>1)
	{
		username=argv[1];
		password=argv[2];
		connstring=argv[3];
	}
	
	int sqlflag;
	cout<<"Do you want to ready the data in the database?(0-no,1-yes)   ";
	cin>>sqlflag;
	if(sqlflag==1)
		system("./SqlHdlOracle");
	int verflag;
	cout<<"Choose a input pattern for table_name(0 for input by yourself,1 for input for the file)   ";
	cin>>verflag;
	vector <string> table_name;
	if(verflag==1)
	{
		ifstream fn;
		fn.open("tablename.txt");
		
		string ntemp;
		while(getline(fn,ntemp))
			table_name.push_back(ntemp);
		fn.close();
	}
	
	

	int method_flag;
	cout<<"Do you need to make a new method of discrete?  (0-no;1-yes):";
	cin>>method_flag;
	
	string cmd;
	string yuanshishuju;string div;string table;
	string translate_data; 
	int key=1;
	if(method_flag==1)
	{
		while(key==1)
		{
		if(verflag==0)
		{
		cout<<"Please input the table name of The original data:"<<endl;
		cin>>yuanshishuju;
		cout<<"Please input the table name of div result:"<<endl;
		cin>>div;
		cout<<"Do you want to change your inputs? (0-no;1-yes)"<<endl;
		key=0;
		cin>>key;
		}
		else
		{
			yuanshishuju=table_name[0];
			cout<<yuanshishuju<<endl;
			div=table_name[1];
			cout<<div<<endl;
			key=0;
		}
		cmd="./discrete/method_discrete 1 ";
		cmd+=username;cmd+=" ";
		cmd+=password;cmd+=" ";
		cmd+=connstring;cmd+=" "; 
		cmd+=yuanshishuju;
		//cout<<cmd<<endl;
		cmd+=" ";
		cmd+=div;
		cmd+=" -1";
		//cmd+=" -1";
		}
		//cout<<cmd<<endl;
		system(cmd.c_str());
	}
	key=1;
	int continue_flag;
	cout<<"continue? (0-no;1-yes):"<<endl;
	cin>>continue_flag;
		if(continue_flag==0)
			return 0;
	
	int skip=0;
	cout<<"Do you want to skip discrete? (0-no;1-yes)"<<endl;
	cin>>skip;
	
	if(skip!=1)
	{	
	while(key==1){
		
		if(verflag==0)
	{cout<<"Please input the table name of The original training data:"<<endl;
	cin>>yuanshishuju;
	
	cout<<"Please input the table name of div:"<<endl;
	cin>>div;
	
	cout<<"Please input the table where the discrete result for training data will be saved:"<<endl;
	cin>>table;
	key=0;	
	cout<<"Do you want to change your inputs? (0-no;1-yes)"<<endl;
	cin>>key;
	}
	else
	{
		yuanshishuju=table_name[0];
		div=table_name[1];
		table=table_name[2];
		key=0;
	}
	cmd="./discrete/test ";
	cmd+=username;cmd+=" ";
	cmd+=password;cmd+=" ";
	cmd+=connstring;cmd+=" "; 
	cmd+=yuanshishuju+" ";
	cmd+=div+" ";
	cmd+=table+" -1";
		}
	//cout<<cmd<<endl;
	system(cmd.c_str());
	}
	
	
	
	cout<<"Do you want to build the Decision tree? (0-no;1-yes):"<<endl;
	cin>>continue_flag;
		if(continue_flag!=0)
		{//return 0;
	cout<<"Choose a way to build the Decision tree:"<<endl;
	cout<<"0-c45_Quinlan;1-c45_zhang;2-chaid_zhang:"<<endl;
	int build_tag;
	cin>>build_tag;
	cout<<"getting training data..."<<endl;
	cmd="./ociuldr user="+username+"/"+password+" query=\"select * from table_sort5\" field=0x2c record=0x0a file=table_sort.csv log=table_sort.log；";
	system(cmd.c_str());
	if(build_tag==0)
	{
	cmd="cp table_sort.csv T1.data";
	system(cmd.c_str());
	//system("cp table_sort.csv T1.data");
	//cmd="cp table_sort5.csv T1.data";
	cout<<"training data got!!!"<<endl;
	cout<<"building tree..."<<endl;
	//system("./c4.5 -f T1");
	cmd="./c4.5 -f T1";
	system(cmd.c_str());
	}
	else if(build_tag==1)
	{
		cmd="cp table_sort.csv T1.Data";
		system(cmd.c_str());
		cmd="cp table_sort.csv T1.Test";
		system(cmd.c_str());
		
		cout<<"training data got!!!"<<endl;
		cout<<"building tree..."<<endl;
		system("./tree/c45 T1");
		system("cp T1_Prune_Structure.txt T1_prunedTree_struc.txt");
		system("cp T1_Prune_Data.txt T1_prunedTree_data.txt");
		system("cp T1_Raw_Structure.txt T1_rawTree_struc.txt");
		system("cp T1_Raw_Data.txt T1_rawTree_data.txt");
		
	}
	else if(build_tag==2)
	{
		system("cp table_sort.csv T1.Data");
		system("cp table_sort.csv T1.Test");
		cout<<"training data got!!!"<<endl;
		cout<<"building tree..."<<endl;
		system("./tree/chaid T1");
		system("cp T1_Prune_Structure.txt T1_prunedTree_struc.txt");
		system("cp T1_Prune_Data.txt T1_prunedTree_data.txt");
		system("cp T1_Raw_Structure.txt T1_rawTree_struc.txt");
		system("cp T1_Raw_Data.txt T1_rawTree_data.txt");
		
	}
	cout<<"decision tree get!!!"<<endl;
	
	system("cp T1.data test.csv");
	system("./check 1 0 80 800");
	system("./base 0");
	cout<<"base is ready!"<<endl;
	
	
	cout<<endl<<endl<<endl<<endl<<endl;
	
	}
	
	cout<<"Do you want to check your Decision tree with your test data? (0-no;1-yes)?"<<endl;
	cin>>continue_flag;
		if(continue_flag==0)
			return 0;
	
	
	
	if(verflag==0)
	{
	cout<<"Please input the table name of The original test data:"<<endl;
	cin>>yuanshishuju;
	cout<<"Please input the table name of div:"<<endl;
	cin>>div;
	cout<<"Please input the table where the discrete result for test data will be saved:"<<endl;
	cin>>table;
	}
	else
	{
	yuanshishuju=table_name[3];
	div=table_name[4];
	table=table_name[5];
	}
	cmd="./discrete/test ";
	cmd+=username;cmd+=" ";
	cmd+=password;cmd+=" ";
	cmd+=connstring;cmd+=" "; 
	cmd+=yuanshishuju+" ";
	cmd+=div+" ";
	cmd+=table+" -1";
	system(cmd.c_str());
	
	cout<<"getting test data..."<<endl;
	cmd="./ociuldr user="+username+"/"+password+" query=\"select * from table_sort5\" field=0x2c record=0x0a file=table_sort.csv log=table_sort.log；";
	system(cmd.c_str());
	
	cmd="cp table_sort.csv test.csv";
	system(cmd.c_str());
	//system("cp table_sort.csv test.csv");
	//cout<<"cp table_sort5.csv test.csv"<<endl;
	cout<<"test data got!!!"<<endl;
	int check_flag=1;
	string mb,ma;
	while(check_flag==1)
	{
	string a,b,c,d;
	cout<<"Please input parameters for check:"<<endl;
	cout<<"prune or not:(0-no;1-yes)   ";
	cin>>a;
	cout<<"motify type:"<<endl;
	cout<<"(0-without motify;1-put down Recall ratio to rise Precision ratio ;2-put down Precision ratio to rise Recall ratio)  ";
	cin>>b;
	if(b[0]!='0')
	{cout<<"motify parameter hit radio:   ";
	cin>>c;
	cout<<"motify parameter unhit number:   ";
	cin>>d;
	}
	else
	{
		c="80";
		d="500";
	}
	cout<<"check begin"<<endl;
	cmd="./check "+a+" "+b+" "+c+" "+d;
	system(cmd.c_str());
	cout<<"check over"<<endl;
	check_flag=0;
	cout<<"Do you want to check again with difference parameters？(0-no;1-yes)"<<endl;
	cin>>check_flag;
	mb=b;ma=a;
	}
	int at;
	cout<<"Do you want to cover the tree by the motify one？(0-no;1-yes)"<<endl;
	cin>>at;
	if(at==1)
	{
		if (mb[0]=='1'&&ma[0]=='1') system("cp data2.txt T1_Prune_Data.txt");
		else if (mb[0]=='1'&&ma[0]=='0') system("cp data2.txt T1_Raw_Data.txt");
		else if (mb[0]=='0'&&ma[0]=='1') system("cp data3.txt T1_Prune_Data.txt");
		else if (mb[0]=='0'&&ma[0]=='0') system("cp data3.txt T1_Raw_Data.txt");
	}
	
	
	cout<<"Do you want to upload your test result? (0-no;1-yes):  "<<endl;
	cin>>continue_flag;
		if(continue_flag==0)
			return 0;
	
	cout<<"Please input the table where the discrete result will be saved"<<endl;
	cin>>yuanshishuju;
	
	cmd="./reswrite "+yuanshishuju+" test_result.txt ";
	cmd+=username;cmd+=" ";
	cmd+=password;cmd+=" ";
	cmd+=connstring;cmd+=" "; 
	system(cmd.c_str());
	
	cout<<"Do you want to upload your rule? (0-no;1-yes):"<<endl;
	cin>>continue_flag;
		if(continue_flag==0)
			return 0;
	if (verflag==0)
	{
	cout<<"Please input the table name of The original test data:"<<endl;
	cin>>yuanshishuju;
	}
	else
		yuanshishuju=table_name[6];
	cout<<"Please input the table where the discrete result will be saved:"<<endl;
	cin>>table;
	
	cmd="./myrule 0 ";
	system(cmd.c_str());
	cmd="./base 1 ";
	system(cmd.c_str());
	cmd="./sqltest "+yuanshishuju+" out.txt ";
	cmd+=username;cmd+=" ";
	cmd+=password;cmd+=" ";
	cmd+=connstring;cmd+=" "; 	
	system(cmd.c_str());
	cmd="./newwrite "+table+" out.txt ";
	cmd+=username;cmd+=" ";
	cmd+=password;cmd+=" ";
	cmd+=connstring;cmd+=" "; 
	system(cmd.c_str());
	
	cout<<endl<<"all over!!!"<<endl;
	return 0;
}