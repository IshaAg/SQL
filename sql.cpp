/*

ISHA AGARWAL
Roll No-14075063
B.Tech Part -III
Computer Science and Engineering 

*/
#include <bits/stdc++.h>
using namespace std;
deque <string> q;					//stores the words of the query
vector<vector<string> > v(100);				//stores the results of the query
deque <string> m;					//stores the words of the current inner query to be executed of nested query
vector<string> cross;					//stores the attribute's names which are to be given as output
void create_table(string);				//creates a new table given by user
void insert_table(string);				//inserts values into a table given by user
void query(string);					//processes the query given by user and prints the result
void parser_function(string);				//parses the query for create and insert function 
void parser_function2(string);				//parses the query and decides the operation to be implemented for nested query 
void select_table();					//implements select operation
void project_table();					//implements project operation
void union_table(string,int);				//implements union operation
void set_diff(string,int);				//implements set difference operation
void rename_table(string,int);				//implements rename operation
bool checkfordot();					//checks for presence of '.' (dot) in the given string
void pushinginvector(string,int);			//pushes the contents of file in v vector of vectors
void condition(string);					//checks for the condition in select statement
void checkoperators(string,int,string);			//checks whether condition is satisfied or not
int stoint(string);					//converts string type to int type
int SIZE=0;						//stores the size of v vector of vectors
int flag=0;						//variable 
string table_name_1,table_name_2;			//stores the table names when 2 tables are used
int table_divider;                                      //starting index for table 2 in cross vector
int dot_flag=0;


int main()
{
  
  //sample creation and insertion in database 
 
  create_table("create table student id int name string address string branch string course_id int");
  insert_table("insert into student 1 isha bhu cse 101 ");
  insert_table("insert into student 2 swati bhu cse 102 ");
  insert_table("insert into student 3 arya bhu mnc 202 ");
  insert_table("insert into student 4 ashtha bhu ece 201 ");
  insert_table("insert into student 5 kiran bhu cse 102 ");
  insert_table("insert into student 6 ram bhu mnc 202 ");
  insert_table("insert into student 7 vivek bhu cse 103 ");
  insert_table("insert into student 8 virat bhu cse 103 ");
  create_table("create table course course_id int credits int prof string");
  insert_table("insert into course 101 13 akm");
  insert_table("insert into course 201 11 skg");
  insert_table("insert into course 102 14 rsg");
  insert_table("insert into course 202 15 aps");
  insert_table("insert into course 103 12 kkg");
  insert_table("insert into course 103 12 kkg");

  //Sample Queries


  query("( select course.credits<d.credits courseX( rename d course ) )");	
  query("( project name course_id ( select student.course_id=course.course_id ( select branch=cse studentXcourse ) ) )");
  query("( project name prof course_id ( select prof=aks ( select * studentXcourse ) ) )");
  query("( ( project course_id prof ( select * course ) ) U ( project course_id prof ( select credits=13 course ) ) )");
  //query("( project course_id ( select credits=13 course ) )");
  query("( project course_id ( select name=arya student ) )");
  query("( select credits>12 course )");
  query("( ( project course_id prof ( select * course) ) - ( project course_id prof ( select credits=13 course ) ) )");
  //query("( project name ( select * student ) )");
  query("( select course_id>105 student )");
  query("( select * course )");
  //query("( select * studentXcourse )");
  query("( project id name ( select branch=cse student ) )");
  //query("( project id name student )");
  
  //sql prompt for the user


  string s;							//stores the query given by user 									
  do
  {
	cout<<"\nmysql>";					//displays the sql prompt							
	getline(cin,s);  					//takes the query from user
	if (s[0]=='c')						//if the first letter is 'c', user wants to create a new table
		create_table(s);				//calls the create function
	else if(s[0]=='i')					//if the first letter is 'i', user wants to insert to an existing table
		insert_table(s);				//calls the insert function
	else 
		query(s);					//otherwise user wants to execute the nested or simple query

  }while(s!="exit");
  cout<<"\nBye!"<<endl;						//takes input from user until user types "exit"
  return 0;
}

void query(string s)
{
    
    parser_function2(s);					//parses the query and decides the operation to be implemented 
    
    int i,j;
    
    for(i=0;i<cross.size();i++)					//prints --- to make distiction between column name and result
	cout<<"--------------";
    cout<<endl;

    
    for(i=0;i<cross.size();i++)					//print column/attribute name stored in cross vector
        cout<<setw(15)<<left<<cross[i];
    cout<<endl;
    
    for(i=0;i<cross.size();i++)					//prints --- to make distiction between column name and result
	cout<<"--------------";
    cout<<endl;
    
    set<vector<string> > print_set;				//stores the output in set , this deletes the duplicate tuples
    for(i=0;i<=SIZE;i++)
    {       
	    if(v[i].empty()!=true)
	    {   
	        print_set.insert(v[i]);				//insertion in set
	    }
    }

    set<vector<string> >::iterator it;
    
    for (it=print_set.begin(); it!=print_set.end(); it++)	//prints the contents of set
    {
	const vector<string>& vi = (*it);			
	for(j=0;j<vi.size();j++)
	{
		cout<<setw(15)<<left<<vi[j];					
	}
	cout<<endl;				
    }
}

void create_table(string s)					//creates a new table
{
    parser_function(s);						//parses the query and stores in deque q
    q.pop_front();						//pops out "create" from deque q
    q.pop_front();						//pops out "table" from deque q
    string table_name=q.front();				//stores the name of table to be created given by user
    fstream f;							//pointer to the file
    f.open( table_name.c_str(),ios::out );			//creates a file with name given by user
    q.pop_front();						//pops out table name given by user from dequeue q
    while(!q.empty())						//dequeue is left with only attributs name and its type given by user
    {
        string s=q.front();					//stores the attribute name given by user
        f<<s;							//writes the attribute name to the file
        f<<" ";
        q.pop_front();						//pops out attribute name after writing it to the file
        q.pop_front();						//pops out attribute type 
    }
    f<<"\n";
    f.close();							//closes the file
    q.clear();							//clears the dequeue q
}

void insert_table(string s)					//inserts the values in the table
{	
    parser_function(s);						//parses the query and stores in deque q
    q.pop_front();						//pops out "insert" from deque q
    q.pop_front();						//pops out "into" from deque q
    string table_name=q.front();				//stores the name of table in which data is to inserted 
    ofstream f;							//pointer to the file
    f.open( table_name.c_str(),ios::app );			//opens the file of the given table name in append mode
    q.pop_front();						//pops out the table name from dequeue q
    while(!q.empty())						//dequeue only left with values to be inserted
    {
        f<<q.front();						//appends the contents to the file
	q.pop_front();
        f<<" ";
    }    
    f<<"\n";
    f.close();							//closes the file
    q.clear();							//clears the dequeue q
}

void parser_function(string s)					//parses the string and pushes the words dequeue q
{
    q.clear();
    string temp="";
    for(int i=0;i<s.length();i++)
    {
        if(s[i]==' ' || s[i]==',')
        {
            q.push_back(temp);
            temp="";
        }
	else if(i==s.length()-1)
	{
	    temp=temp+s[i];
	    q.push_back(temp);
            temp="";
	}
        else 
            temp=temp+s[i];
    }
}

void parser_function2(string s)					//decides the operation to be implemented given by user
{
    
    q.clear();
    cross.clear();
    for(int i=0;i<=SIZE;i++)
        v[i].clear();
    SIZE=0;
    m.clear();
    string temp="";
    int flag=0;
    size_t found=s.find("rename");				//if rename found first , call rename_table function
    if(found!=string::npos)
    {	
       flag=1;
       rename_table(s,found);
    }
    if(flag==1)
	return;
    for(int i=0;i<s.length();i++)
    {
	if(s[i]=='U')						//if union found , call union_table function
	{
	    union_table(s,i);
	    flag=1;
	    break;	
	}
	else if(s[i]=='-')					//if set difference found, call set_diff function
	{
	    set_diff(s,i);			
	    flag=1;
	    break;
	}
	
    }
    
    if(flag==1) return ;
    for(int i=0;i<s.length();i++)
    {
        if(s[i]==' '|| s[i]==',')
        {
	    if(temp.length()!=0)            
		q.push_back(temp);
            temp="";
        }
        else if(s[i]==')')					//if closing bracket found, pop till opening bracket is found
        {
            m.clear();
            while(q.back()!="(")
            {
                string t;
                t=q.back();
                m.push_front(t);
                q.pop_back();
            }
	    q.pop_back();
	    
            if(m.front()=="select")				//if select found , call select_table function
                select_table();
            else if(m.front()=="project")			//if project found , call project_table function			
                project_table();
        }
	else
        {
            temp=temp+s[i];		
        }

    }
}

void select_table()							//implements select operation
{
    string s=m.back();
    string op1="X";							//cartesian product operator = 'X'
    size_t foundop1=s.find(op1);					//searches for cartesian product operator
    if(foundop1<s.size() && s[foundop1]=='X')				//if 'X' found
    {
	     
	string table1=s.substr(0,s.find(op1));				//table1 to the left of 'X'
	string table2=s.substr(s.find(op1)+1,s.size()-s.find(op1));	//table1 to the right of 'X'
	ifstream t1,t2;
	table_name_1=table1;
	table_name_2=table2; 
        t1.open(table1.c_str(),ios::in);
        t2.open(table2.c_str(),ios::in);
        string head="";
        getline(t1,head);
	string temp="";
        for(int i=0;i<head.length();i++)
        {
	    
            if(head[i]==' ')
            {
                cross.push_back(temp);					//pushing attribute names of table1
                temp="";
            }
            else
                temp=temp+head[i];
        }
	
        head="";
	temp="";
        getline(t2,head);
	table_divider=cross.size();
        for(int i=0;i<head.length();i++)
        {
            
            if(head[i]==' ')
            {
                cross.push_back(temp);					//pushing attribute names of table2
                temp="";
            }
            else
                temp=temp+head[i];
        }
	
        t2.close();
        
        string a,b;
        int i=0;
        while(!t1.eof())
        {
            
       	    getline(t1,a);						//string a from table1 file
	    ifstream t3;
            t3.open(table2.c_str(),ios::in);
            getline(t3,b);						
            while(!t3.eof())
            {
                getline(t3,b);						//string b from table2 file
               
		if(!a.empty() && !b.empty())
		{   
	            b=a+b;
		    pushinginvector(b,i);				//pushing attribute values of both tables in v vector of vectors
                    i++;
                }
            }
            t3.close();
	    
        }
        t1.close();
	m.pop_back();
	condition(m.back());						//checks for the condition of select statement
        
    }
    
    else
    {
        //one table only
	
	int tr=m.size();
	
        if(tr!=2)    							//not a nested query. one table only
        {    
            string table=m.back();					//table name
	    m.pop_back();
            string mt,temp="";
            ifstream t;
            t.open(table.c_str(),ios::in);				//opening file of that table 
	  
                    
            getline(t,mt);						
		    for(int i=0;i<mt.length();i++)          		
    	    	    {
       
        		if(mt[i]==' ')
            		{
                		cross.push_back(temp);			//pushing attribute names in cross vector		
                		temp="";
            		}
        		else
                		temp=temp+mt[i];
    	    	    }
	  	    int i=0;
		    while(!t.eof())
		    {
		    	getline(t,mt);
                	if(!mt.empty())
			    {   
		            	pushinginvector(mt,i);			//pushing attribute values in v vector of vectors
                    		i++;
            		    }
		        
		    }
                
		    condition(m.back());				//checks for the condition of select statement
    
        }
        else if(tr==2)							//nested query, so size of outer query =2
        {
	    condition(m.back());					//checks for the condition of outer query
        }
    }
}

void project_table()							//projects the columns given by user as output
{   
    
    if(SIZE==0)								//not a nested query then SIZE=0
    {   
        
        string table=m.back();						//last element is table_name
	m.pop_back();							//pops out table name from back from dequeue m
	m.pop_front();							//pops out "project" from front from dequeue m
	ifstream t;
        t.open(table.c_str(),ios::in);					//opens the file of that table name for taking input
        string head="";
        getline(t,head);
	string temp="";
        for(int i=0;i<head.length();i++)
        {

            if(head[i]==' ')
            {
                cross.push_back(temp);					//pushes the attribute names in cross vector
                temp="";
            }
            else
                temp=temp+head[i];
        }
	int ii=0;
	while(!t.eof())
	{
	    getline(t,head);
	    if(!head.empty())
	    {
		pushinginvector(head,ii);				//pushes attribute values in v vector of vectors
	        ii++;
	    }	
	}
	int i,j,k;
	for(i=0;i<cross.size();i++)
	{
	    for(j=0;j<m.size();j++)
	    {
	        if(cross[i]==m.at(j))					//checks which attributes to be projected in m dequeue
		    break;
	    }
	    if(j==m.size())						//if not found in dequeue m
	    {
		int index=i;	    	
		cross.erase(cross.begin() + index);			//erase it from cross vector
		for(k=0;k<=SIZE;k++)
		    if(v[k].empty()!=true)				//erase all its corrresponding values from v vector of vectors	    
			v[k].erase(v[k].begin() + index);
		i=index-1;
	    }	
	}
	t.close();							//closes the file
    }
    else								//else it is a nested query
    {
        
	m.pop_front();							// pops out "project" from dequeue m
	int i,j,k;
	
	for(i=0;i<cross.size();i++)					//delete the columns with duplicate names
	{
	    for(j=i+1;j<cross.size();j++)
	    {
		    if(cross[i]==cross[j])
		    {
			    int index=j;
			    cross.erase(cross.begin() + index);
		        for(k=0;k<=SIZE;k++)
			    if(v[k].empty()!=true)		            
				v[k].erase(v[k].begin() + index);
		        j=index-1;
		    }
	    }	
	}
	for(i=0;i<cross.size();i++)					//checks which attributes to be projected in m dequeue			
	{
	    for(j=0;j<m.size();j++)
	    {
	        if(cross[i]==m.at(j))
		    break;
	    }
	    if(j==m.size())						//if not found in dequeue m
	    {
		int index=i;	    	
		cross.erase(cross.begin() + index);			//erase it from cross vector
		for(k=0;k<=SIZE;k++)
		    if(v[k].empty()!=true)		    
			v[k].erase(v[k].begin() + index);		//erase all its corrresponding values from v vector of vectors
		i=index-1;
	    }	
	}
	
    }
}

void pushinginvector(string s,int i)					//pushes the contents of file in v vector of vectors
{
    string temp="";
    for(int j=0;j<=s.length();j++)					//parses the string s
    {
        if(s[j]==' ' || j==s.length())
        {
            v[i].push_back(temp);					//pushes the word in v vector of vectors
            temp="";
        }
        else
            temp=temp+s[j];
    }
    SIZE=i;								//size of current v vector of vectors
}

void condition(string s)						// s contains the condition of select statement
{
    
    int i,index;
    if(s=="*")								// if s==* , no condition is present
        return;
    int dots=0;
    int op;
    int index_dot[2]={0,0};
    
    for(i=0;i<s.length();i++)
    {
        if(s[i]=='.')
        {
            index_dot[dots]=i;						//stores the index of dot
            dots++;							//counts the number of dots
        }
        if(s[i]=='=' || s[i]=='!' || s[i]=='>' || s[i]=='<')		//stores the index of operator
            op=i;
    }
    if(dots==2)								// 2 dots in select condition
    {									// table1.col1(operator)table2.col2 eg: table1.col1=table2.col2
        string table1,table2,col1,col2;
	table1=s.substr(0,index_dot[0]);				//table1
        col1=s.substr(index_dot[0]+1,op-index_dot[0]-1);		//col1 - attribute name of table1
        table2=s.substr(op+1,index_dot[1]-op-1);			//table2
        col2=s.substr(index_dot[1]+1,s.length()-index_dot[1]);		//col2 - attribute name of table2
	
        if(table1!=table_name_1)
        {
            string temp;						//swapping table and column name for tables 1 and 2
            temp=table2;
            table2=table1;
            table1=temp;
            temp=col2;
            col2=col1;
            col1=temp;
        }
        int index_col1;							//index of col1 in cross vector(contains attribute names)
        int index_col2;							//index of col2 in cross vector(contains attribute names)
        for(i=0;i<cross.size();i++)
        {
            if(cross[i]==col1 && i<table_divider)
                index_col1=i;
            if(cross[i]==col2 && i>=table_divider)
                index_col2=i;
        }
        int j;
	if(s[op]=='=')
        for(j=0;j<=SIZE;j++)
        {
            if(v[j].empty()!=true)
            {
                if(v[j][index_col1]!=v[j][index_col2])			//if not satisfying '=' condition [equals]
                    v[j].clear();					//delete that tuple
            }
        }
	else if(s[op]=='<')
        for(j=0;j<=SIZE;j++)
        {   
            if(v[j].empty()!=true)
            {
                if(stoint(v[j][index_col1])>=stoint(v[j][index_col2]))	//if not satisfying '<' condition [less than]
                    v[j].clear();					//delete that tuple
            }
        }
	else if(s[op]=='>')
        for(j=0;j<=SIZE;j++)
        {   
            if(v[j].empty()!=true)
            {
                if(stoint(v[j][index_col1])<=stoint(v[j][index_col2]))	//if not satisfying '>' condition [greater than]
                    v[j].clear();					//delete that tuple
            }
        }
	else if(s[op]=='!')
        for(j=0;j<=SIZE;j++)
        {   
            if(v[j].empty()!=true)
            {
                if(stoint(v[j][index_col1])==stoint(v[j][index_col2]))	//if not satisfying '!' condition [not equals]
                    v[j].clear();					//delete that tuple
            }
        }   
    }
    
    else								//no dots in condition
    {
        string col,value;
        col=s.substr(0,op);						//attribute name is to the left of operator
        value=s.substr(op+1,s.length()-op);				//value with which it is to be compared, is to the right of operator
        string oper=s.substr(op,1);					//stores the operator
        for(i=0;i<cross.size();i++)
        {
            if(col==cross[i])
            {
                index=i;						//index of attribute name in cross vector
                break;
            }
        }
        checkoperators(oper,index,value);				//checks whether condition is satisfied or not in that attribute column
    }
    
}

void checkoperators(string op,int index,string value)			//checks whether condition is satisied or not
{									//op : operator
    int i;								//index : for which attribute column we have to check
    if(op=="=")								//value : value with which it is to be compared
    {
        for(i=0;i<=SIZE;i++)
        {
            if(v[i][index]!=value)					//if does not satisfies the condition [equals]
                v[i].clear();    					//delete the tuple
        }
    }
    else if(op=="!")
    {
        for(i=0;i<=SIZE;i++)
        {
            if(v[i][index]==value)					//if does not satisfies the condition [not equals]
                v[i].clear();    					//delete the tuple
        }
    }
    else if(op==">")
    {
        for(i=0;i<=SIZE;i++)
        {
            if(stoint(v[i][index])<=stoint(value))			//if does not satisfies the condition [greater than]
                v[i].clear();    					//delete the tuple
        }
    }
    else if(op=="<")
    {
        for(i=0;i<=SIZE;i++)
        {
            if(stoint(v[i][index])>=stoint(value))			//if does not satisfies the condition [less than]
                v[i].clear();    					//delete the tuple
        }
    }
}

void union_table(string s, int op)					//implements union operation 
{									//s : query given by user
    string s1,s2;							//op : location of 'U' union operator in string s
    s1=s.substr(2,op-3);						//s1 : sub query 1 - left of 'U'
    s2=s.substr(op+2,s.length()-op-4);					//s2 : sub query 2 - right of 'U'
    parser_function2(s1);						//parses subquery s1
    
    int i,j;

    set<vector<string> >myset;						//create a set, so that duplicacy of tuples after union does not occur 
    for(i=0;i<=SIZE;i++)
    {
	if(v[i].empty()!=true)	
		myset.insert(v[i]);					//insert the result of s1 in set
    }
    parser_function2(s2);						//parses subquery s2
  
    for(i=0;i<=SIZE;i++)
    {
	if(v[i].empty()!=true)	
		myset.insert(v[i]),v[i].clear();			//insert the result of s2 in set
    }
    set<vector<string> >::iterator it;					//iterator for the set
    SIZE=0;
    for (it=myset.begin(); it!=myset.end(); it++)
    {
	const vector<string>& vi = (*it);
	for(int j=0;j<vi.size();j++)
	{
		v[SIZE].push_back(vi[j]);				//push the contents of set to v vector of vectors
					
	}
	SIZE++;				
    }
}


void set_diff(string s,int op)						//implements the set difference operation
{									//s : query given by user
    string s1,s2;							//op : location of '-' set difference operator in string s
    s1=s.substr(2,op-3);						//s1 : sub query 1 - left of '-'
    s2=s.substr(op+2,s.length()-op-4);					//s2 : sub query 2 - right of '-'

    parser_function2(s1);						//parses subquery s1
    
    int i,j;

    set<vector<string> >myset;						//create a set
    for(i=0;i<=SIZE;i++)
    {
	if(v[i].empty()!=true)	
		myset.insert(v[i]);					//insert the result of s1 in set
    }
    parser_function2(s2);						//parses subquery s1
    for(i=0;i<=SIZE;i++)
    {
	if(v[i].empty()!=true)	
		myset.erase(myset.find(v[i])),v[i].clear();		//deletes the tuple in result of s2 from s1 if found in s1
    }
    set<vector<string> >::iterator it;					//iterator for the set
    SIZE=0;
    for (it=myset.begin(); it!=myset.end(); it++)
    {
	const vector<string>& vi = (*it);
	for(int j=0;j<vi.size();j++)
	{
		v[SIZE].push_back(vi[j]);				//push the contents of set to v vector of vectors 					
	}
	SIZE++;				
    }
}


void rename_table(string s,int op)					//implements the rename operation
{									//s : query given by user
    string t;								//op : location of 'r' of rename in string s
    int i,end=0;
    for(i=op;i<s.length();i++)
    {
        if(s[i]==')')
            end=i;
	if(end!=0)
	    break;
    }
    int start=op-2;
    t=s.substr(op,end-start-3);
    string table,new_name,temp="";
    vector<string> r;
    for(i=0;i<t.length();i++)
    {
        if(t[i]==' ')
        {
            if(temp.length()!=0) r.push_back(temp);
            temp="";
        }
	else if(i==t.length()-1)
	{
	    temp=temp+t[i];
	    if(temp.length()!=0) r.push_back(temp);
	    
            temp="";	
	}
        else
            temp=temp+t[i];
    }
    table=r[2];								//table which is to be renamed
    new_name=r[1];							//new name of the table
    s.erase(start,end-start+1);						//replace the rename string in s with new name
    s.insert(start,r[1]);
    fstream f1;
    f1.open( new_name.c_str(),ios::out );				//create a new file with new table name
    f1.close();
    ofstream f;
    f.open( new_name.c_str(),ios::app );
    ifstream t1;
    t1.open(table.c_str(),ios::in);
    string head="";
    while(!t1.eof())							//copy the contents of old table file into new table file name
    {
        getline(t1,head);		
	f<<head;
	f<<"\n";
    }    
    f.close();
    t1.close();
    parser_function2(s);						//parse the resultant new query
}

int stoint(string s)							//converts string to integer
{
    int num=0;
    
    int l=s.length();
    int i;
    int k=1;
    for(i=l-1;i>=0;i--)
    {
        num=num+(s[i]-'0')*k;
        k=k*10;
    }
    return num;								//returns the integer value
}


bool checkfordot()							//checks for presence of dot in a given condition
{
    int j;
    for(j=0;j<m.size();j++)
    {
	string str=m.at(j);	
	size_t found = str.find(".");
  	if (found!=std::string::npos)
	{    
		dot_flag=1;		
		return true;    
	}
    }
    return false;
}


