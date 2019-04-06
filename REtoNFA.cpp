#include <bits/stdc++.h>
using namespace std;


struct transition
{
	int initial;
	int final;
	char input;

	transition(int initial, int final, char input){
		this->initial = initial;
		this->final = final;
		this->input = input;
	}
};


struct state
{
	int start;
	int end;
	state(int start, int end){
		this->start = start;
		this->end = end;
	}
};


int prec(char c){
	if(c == '*') 
	return 3; 
	else if(c == '.' || c == '/') 
	return 2; 
	else if(c == '+' || c == '-') 
	return 1; 
	else
	return -1;
}


string postfix(string temp){
	string str;
	stack <char> s;
	s.push('$');
	for(int i = 0; i < temp.size(); i++){
		if(temp[i] >= 'a' && temp[i] <= 'z'){
			str.push_back(temp[i]);
		}
		else if(temp[i] == '(')
			s.push('(');
		else if(temp[i] == ')'){
			while(s.top() != '$' || s.top() != '('){
				if(s.top() == '(')
					break;
				str.push_back(s.top());
				s.pop();
			}
			if(s.top() == '(')
				s.pop();
		}
		else{
			while(s.top() != '$' && prec(temp[i]) <= prec(s.top())){
				str.push_back(s.top());
				s.pop();
			}
			s.push(temp[i]);
		}
	}
	while(s.top() != '$'){
		str.push_back(s.top());
		s.pop();
	}
	return str;
}



void inp(char c,vector<transition> &v,int &s_cont,stack<state> &s){
	transition t(s_cont,s_cont+1,c);
	state s1(s_cont,s_cont+1);
	v.push_back(t);
	s_cont++;
	s_cont++;
	s.push(s1);
}


void kleeni_closer(vector<transition> &v, int &s_cont,stack<state> &s){
	state temp = s.top();
	transition t1(s_cont,temp.start,'^');
	s_cont++;
	v.push_back(t1);
	transition t2(temp.end,temp.start,'^');
	v.push_back(t2);
	transition t3(temp.end,s_cont,'^');
	s_cont++;
	v.push_back(t3);
	transition t4(t1.initial,t3.final,'^');
	v.push_back(t4);
	state s1(t1.initial,t4.final);
	s.pop();
	s.push(s1);
}


void sumation(vector<transition> &v, int &s_cont,stack<state> &s){
	state x1 = s.top();
	s.pop();
	state x2 = s.top();
	s.pop();
	transition t1(s_cont,x1.start,'^');
	v.push_back(t1);
	transition t2(s_cont,x2.start,'^');
	v.push_back(t2);
	s_cont++;
	transition t3(x1.end,s_cont,'^');
	v.push_back(t3);
	transition t4(x2.end,s_cont,'^');
	v.push_back(t4);
	s_cont++;
	state s1(t1.initial,t4.final);
	s.push(s1);
}


void dot(vector<transition> &v, int &s_cont,stack<state> &s){
	state x1 = s.top();
	s.pop();
	state x2 = s.top();
	s.pop();
	transition t1(s_cont,x2.start,'^');
	v.push_back(t1);
	s_cont++;
	transition t2(x2.end,x1.start,'^');
	v.push_back(t2);
	state s1(t1.initial,x1.end);
	s.push(s1);
}


void display(vector<transition> &v,stack<state> &s){
	cout<<"Transition Function : "<<endl;
	for(int n = 0; n < v.size(); n++){
		cout<<"q("<<v[n].initial<<","<<v[n].input<<")"<<"->"<<"q("<<v[n].final<<")"<<endl;
	}
	cout<<"Starting State : "<<s.top().start<<endl;
	cout<<"Final State : "<<s.top().end<<endl;
	cout<<endl;
}


void RE_to_NFA(string pexp,vector<transition> &v,stack<state> &s){
	int s_cont = 0;
	for(int i = 0; i < pexp.size(); i++){
		if(pexp[i] >= 'a' && pexp[i] <= 'z'){
			inp(pexp[i],v,s_cont,s);
		}
		else{
			switch(pexp[i]){
				case '*' :{
					kleeni_closer(v,s_cont,s);
					break;
				}
				case '+' :{
					sumation(v,s_cont,s);
					break;
				}
				case '.' :{
					dot(v,s_cont,s);
					break;
				}
				default:{
					cout<<"error"<<endl;
				}
			}
		}
	}
}


int main(){
	//string exp = "a*.b+c*.d";
	//string exp = "(a+b)*.a.b.c";
	//string exp = "a+b.c";
	//string exp = "a*.b.c*.d";
	string exp = "a*+b*";
	std::vector<transition> v;
	stack<state> s;
	state s1(0,100);
	s.push(s1);
	string post_exp = postfix(exp);
	cout<<"Given RE : "<<exp<<endl;
	cout<<"RE in Postfix :"<<(post_exp)<<endl;
	RE_to_NFA(post_exp,v,s);
	display(v,s);
	return 0;
}
