#include<bits/stdc++.h>
using namespace std;


struct production
{
	char LHS;
	string RHS;
	production(char LHS, string RHS){
		this->LHS = LHS;
		this->RHS = RHS;
	}
};


struct First
{
	char NT;
	vector<char> first;
	First(char NT,vector<char> first){
		this->NT = NT;
		this->first = first;
	}
};


struct Follow
{
	char NT;
	vector<char> follow;
	Follow(char NT,vector<char> follow){
		this->NT = NT;
		this->follow = follow;
	}
};


struct src
{
	int LHS_index;
	int RHS_index;
	vector<char> after_str;
	src(int LHS_index, int RHS_index, vector<char> after_str){
		this->LHS_index = LHS_index;
		this->RHS_index = RHS_index;
		this->after_str = after_str;
	}
};


bool isTerminal(char c, vector<char> &terminal){
	for(int i = 0; i < terminal.size(); i++){
		if(terminal[i] == c)
			return true;
	}
	return false;
}


bool is_nonTerminal(char c, vector<char> &nonTerminal){
	for(int i = 0; i < nonTerminal.size(); i++){
		if(nonTerminal[i] == c)
			return true;
	}
	return false;
}


vector<int> search_I(char c, vector<production> &P){
	vector<int> temp;
	for(int i = 0; i < P.size(); i++){
		if(P[i].LHS == c)
			temp.push_back(i);
	}
	return temp;
}


bool search(vector<char> &v, vector<First> &firsts){
	for(int i = 0; i < v.size(); i++){
		if(v[i] == '^')
			return true;
	}
	return false;
}


vector<char> Union(vector<char> temp, vector<char> v){
	vector<char> v1;
	for(int i = 0; i < temp.size(); i++)
		v1.push_back(temp[i]);
	for(int i = 0; i < v.size(); i++)
		v1.push_back(v[i]);
	return v1;
}


void disp_first(vector<First> &firsts){
	cout<<"First : "<<endl;
	for(int i = 0; i < firsts.size(); i++){
		cout<<"first("<<firsts[i].NT<<") : {";
		for(int j = 0; j < firsts[i].first.size(); j++){
			cout<<firsts[i].first[j];
			if(j == firsts[i].first.size() - 1)
				cout<<"}";
			else
				cout<<",";
		}
		cout<<endl;
	}
	cout<<endl;
}


void createV(vector<char> temp, vector<char> &v){
	for(int i = 0; i < temp.size(); i++)
		v.push_back(temp[i]);
}


void firstOf(char start_sym,vector<production> &P,vector<char> &terminal,vector<char> &nonTerminal,
			vector<First> &firsts, vector<int> &flag, map<char, int> &m,map<char, int> &m1){
	
	stack<char> s;
	s.push('$');
	for(int i = 0; i < nonTerminal.size(); i++)
		s.push(nonTerminal[i]);

	m1['E'] = -1;
	m1['T'] = -1;
	m1['G'] = -1;
	m1['H'] = -1;
	m1['F'] = -1;

	while(s.top() != '$'){
		//cout<<"s.top() : "<<s.top()<<endl;
		if(flag[m[s.top()]] == 0){
			vector<int> index = search_I(s.top(),P);
			vector<char> temp;
			vector<char> v;
			int len = 0;
			vector<char> temp1;
			for(int n = 0; n < index.size(); n++){
				if(isTerminal(P[index[n]].RHS[0],terminal)){
					temp.push_back(P[index[n]].RHS[0]);
					if(n == index.size() - 1){
						First f(s.top(),temp);
						firsts.push_back(f);
						m1[s.top()] = firsts.size() - 1;
						flag[m[s.top()]] = 1;
					}
					}
					else if(is_nonTerminal(P[index[n]].RHS[0],nonTerminal)){
						if(m1[P[index[n]].RHS[0]] == -1){
							s.push(P[index[n]].RHS[0]);
							break;
						}
						else{
							//either RHS is completely traversed or first is not yet cal or there is np '^'
							while(len != P[index[n]].RHS.size()
									&& m1[P[index[n]].RHS[len]] != -1 
									&& search(v,firsts)){
								createV(firsts[m1[P[index[n]].RHS[len]]].first,v);
								temp1 = Union(temp1,v);
								len++;
							}
							if(len == P[index[n]].RHS.size()){
								if(n == index.size() - 1){
									First f(s.top(),temp1);
									firsts.push_back(f);
									m1[s.top()] = firsts.size() - 1;
									flag[m[s.top()]] = 1;
								}
							}
							else if(m1[P[index[n]].RHS[len]] == -1 ){
								s.push(P[index[n]].RHS[len]);
								break;
							}
							else if(!search(v,firsts)){
								createV(firsts[m1[P[index[n]].RHS[len]]].first,v);
								temp1 = Union(temp1,v);
								if(n == index.size() - 1){
									First f(s.top(),temp1);
									firsts.push_back(f);
									m1[s.top()] = firsts.size() - 1;
									flag[m[s.top()]] = 1;
								}
							}
						}
					}
				}
				if(flag[m[s.top()]] == 1)
					s.pop();
		}
		else{
			s.pop();
		}
	}
}


void follow_search(char c,vector<src> &src1, vector<production> &P){
	for(int i = 0; i < P.size(); i++){
		for(int j = 0; j < P[i].RHS.size(); j++){
			if(P[i].RHS[j] == c){
				if(P[i].RHS[j+1] == c)
						break;
				int left = i;
				int right = j+1;
				vector<char> temp;
				temp.push_back('@');
				for(int k = j + 1; k < P[i].RHS.size(); k++){
					if(P[i].RHS[k] == c)
						break;
					temp.pop_back();
					temp.push_back(P[i].RHS[k]);
				}
				src x(left,right,temp);
				src1.push_back(x);
			}
		}
	}
}


void disp_follow(vector<Follow> &follows){
	cout<<"Follow : "<<endl;
	for(int i = 0; i < follows.size(); i++){
		cout<<"follow("<<follows[i].NT<<") : {";
		for(int j = 0; j < follows[i].follow.size(); j++){
			cout<<follows[i].follow[j];
			if(j == follows[i].follow.size() - 1)
				cout<<"}";
			else
				cout<<",";
		}
		cout<<endl;
	}
	cout<<endl;
}


void createV1(vector<char> temp, vector<char> &v){
	for(int i = 0; i < temp.size(); i++){
		int ck = 0;
		for(int j = 0; j < v.size(); j++){
			if(temp[i] == v[j])
				ck++;
		}
		if(ck == 0 && temp[i] != '^'){
			v.push_back(temp[i]);
		}
	}
}


void followOf(char start_sym,vector<production> &P,vector<char> &terminal,vector<char> nonTerminal,
			vector<First> &firsts,vector<int> &flag1,map<char, int> &m,map<char, int> &m1,vector<Follow> &follows){
	stack<char> S;
	S.push('$');
	for(int i = 0; i < nonTerminal.size(); i++)
		S.push(nonTerminal[i]);
	S.push(start_sym);

	map<char, int> m2;
	m2['E'] = -1;
	m2['T'] = -1;
	m2['G'] = -1;
	m2['H'] = -1;
	m2['F'] = -1;

	while(S.top() != '$'){
		if(flag1[m[S.top()]] == 0){
			vector<src> src1;
			follow_search(S.top(),src1,P);

			if(src1.size() == 0){
				if(S.top() == start_sym){
					vector<char> v1;
					v1.push_back('$');
					Follow f(start_sym,v1);
					follows.push_back(f);
					m[start_sym] = follows.size()-1;
				}
				S.pop();
			}

			int flg = 0;
			vector<char> v;
			for(int i = 0; i < src1.size(); i++){
				if(src1[i].after_str[0] == '@'){
					if(P[src1[i].LHS_index].LHS != S.top()){
						if(m2[P[src1[i].LHS_index].LHS] == -1){
							S.push(P[src1[i].LHS_index].LHS);
							flg++;
							break;
						}
						else{
							createV1(follows[m2[P[src1[i].LHS_index].LHS]].follow,v);
						}
					}
				}
				else{
					if(isTerminal(src1[i].after_str[0],terminal)){
						v.push_back(src1[i].after_str[0]);
					}
					else if(is_nonTerminal(src1[i].after_str[0],nonTerminal)){
						createV1(firsts[m1[src1[i].after_str[0]]].first,v);
						if(search(firsts[m1[src1[i].after_str[0]]].first,firsts)){
							if(1 == src1[i].after_str.size()){
								vector<char> x1;
								x1.push_back('@');
								src s1(src1[i].LHS_index,src1[i].RHS_index+1,x1);
								src1.push_back(s1);
							}
							else{
								vector<char> x1;
								for(int j = 1; j < src1[i].after_str.size(); j++)
									x1.push_back(src1[i].after_str[j]);
								src s1(src1[i].LHS_index,src1[i].RHS_index+1,x1);
								src1.push_back(s1);
							}
						}
					}
				}
			}
			if(flg == 0){
				if(S.top() == start_sym)
				v.push_back('$');
				Follow f1(S.top(),v);
				follows.push_back(f1);
				flag1[m[S.top()]] = 1;
				m2[S.top()] = follows.size()-1;
				S.pop();
			}
		}
		else{
			S.pop();
		}
	}
}


void display(vector<production> &P, vector<char> &terminal, vector<char> &nonTerminal){
	cout<<"Productions of the given Grammar : "<<endl;
	for(int i = 0; i < P.size(); i++){
		cout<<P[i].LHS<<" -> ";
		for(int j = 0; j < P[i].RHS.size(); j++){
			cout<<P[i].RHS[j];
		}
		cout<<endl;
	}
	cout<<endl;
	cout<<"Terminals : ";
	for(int i = 0; i < terminal.size(); i++){
		cout<<terminal[i]<<" ";
	}
	cout<<endl;
	cout<<"Non-Terminals : ";
	for(int i = 0; i < nonTerminal.size(); i++){
		cout<<nonTerminal[i]<<" ";
	}
	cout<<endl<<endl;
}


int main(){
	production p1('E',"TG");
	production p2('G',"+TG");
	production p3('G',"^");
	production p4('T',"FH");
	production p5('H',"*FH");
	production p6('H',"^");
	production p7('F',"i");
	production p8('F',"(E)");

	vector<production> P {p1,p2,p3,p4,p5,p6,p7,p8};
	vector<char> terminal {'i','+','*','(',')','^'};
	vector<char> nonTerminal {'E','T','G','H','F'};
	display(P,terminal,nonTerminal);

	vector<First> firsts;
	vector<int> flag {0,0,0,0,0};
	map<char, int> m;
	m['E'] = 0;
	m['T'] = 1;
	m['G'] = 2;
	m['H'] = 3;
	m['F'] = 4;
	map<char, int> m1;
	char start_sym = 'E';
	firstOf(start_sym,P,terminal,nonTerminal,firsts,flag,m,m1);
	disp_first(firsts);
	vector<Follow> follows;
	vector<int> flag1 {0,0,0,0,0};
	followOf(start_sym,P,terminal,nonTerminal,firsts,flag1,m,m1,follows);
	disp_follow(follows);
	return 0;
}