#include<bits/stdc++.h>
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


struct Transition
{
	vector<int> Initial;
	vector<int> Final;
	char Input;
	Transition(vector<int> Initial,vector<int> Final, char Input){
		this->Initial = Initial;
		this->Final = Final;
		this->Input = Input;
	}
};


struct IP
{
	vector<int> end;
	IP(vector<int> end){
		this->end = end;
	}
};


bool search(vector<int> temp,vector<IP> S){
	if(S.size() == 0)
		return true;
	else{
		for(int i = 0; i < S.size(); i++){
			if(S[i].end.size() == temp.size()){
				for(int j = 0; j < S[i].end.size(); j++){
					if(S[i].end[j] != temp[j])
						return true;
				}
				return false;
			}
		}
		return true;
	}
	return false;
}


vector<int> Union(vector<IP> temp, vector<int> v,int s_cont){
	vector<int> r;
	for(int i = 0; i < v.size(); i++){
		for(int j = 0; j < temp[v[i]].end.size(); j++){
			if(temp[v[i]].end[0] != 100){
				int flag = 0;
				for(int k = 0; k < r.size(); k++){
					if (r[k] == temp[v[i]].end[j])
						flag++;
				}
				if(flag == 0)
					r.push_back(temp[v[i]].end[j]);
			}
		}
	}
	/*cout<<"union :"<<endl;
	for(int i = 0; i < r.size(); i++)
		cout<<r[i]<<endl;*/
	return r;
}


void end_state(vector<IP> &S, vector<vector<int> > &Final_State, vector<int> &final_state){
	for(int i = 0; i < S.size(); i++){
		for(int j = 0; j < final_state.size(); j++){
			for(int k = 0; k < S[i].end.size(); k++){
				if(S[i].end[k] == final_state[j]){
					Final_State.push_back(S[i].end);
					break;
				}
			}
		}
	}
}


void NFAtoDFA(int &s_cont,vector<Transition> &T,int &Initial_State,vector<vector<int> > &Final_State,vector<transition> &t,vector<int> &final_state){
	vector<IP> a;
	vector<IP> b;
	for(int i = 0; i < s_cont; i++){
		vector<int> x;
		vector<int> y;
		x.push_back(100);
		y.push_back(100);
		for(int j = 0; j < t.size(); j++){
			if(t[j].initial == i && t[j].input == 'a'){
				if(x[0] == 100)
					x.pop_back();
				x.push_back(t[j].final);
			}
			else if(t[j].initial == i && t[j].input == 'b'){
				if(y[0] == 100)
					y.pop_back();
				y.push_back(t[j].final);
				//IP y1(y);
			}
		}
		IP x1(x);
		IP y1(y);
		a.push_back(x1);
		b.push_back(y1);
	}
	int flag = 0;
	vector<int> st;
	vector<IP> A;
	vector<IP> B;
	vector<IP> S;
	st.push_back(Initial_State);
	stack<vector<int> > s;
	vector<int> z;
	z.push_back(100);
	s.push(z);
	s.push(st);
	while(s.top()[0] != 100){
		if(search(s.top(),S)){
			IP xyz(s.top());
			S.push_back(xyz);
			s.pop();
			if(S[S.size()-1].end.size() == 1){
				vector<int> fnA;
				vector<int> fnB;
				int ind =  S[S.size()-1].end[0];
				if((a[S[S.size()-1].end[0]].end[0] != 100) 
					&& (b[S[S.size()-1].end[0]].end[0] != 100)){
					A.push_back(a[ind].end);
					B.push_back(b[ind].end);
				}
				else if((a[S[S.size()-1].end[0]].end[0] == 100) 
					&& (b[S[S.size()-1].end[0]].end[0] != 100)){
					fnA.push_back(s_cont);
					A.push_back(fnA);
					B.push_back(b[ind].end);
					flag++;
				}
				else if((a[S[S.size()-1].end[0]].end[0] != 100) 
					&& (b[S[S.size()-1].end[0]].end[0] == 100)){
					A.push_back(a[ind].end);
					fnB.push_back(s_cont);
					B.push_back(fnB);
					flag++;
				}
				else if((a[S[S.size()-1].end[0]].end[0] == 100) 
					&& (b[S[S.size()-1].end[0]].end[0] == 100)){
					fnA.push_back(s_cont);
					fnB.push_back(s_cont);
					A.push_back(fnA);
					B.push_back(fnB);
					flag++;
				}
			}
			else{
				vector<int> fnA;
				vector<int> fnB;
				fnA = Union(a,S[S.size()-1].end,s_cont);
				fnB = Union(b,S[S.size()-1].end,s_cont);
				IP vr(fnA);
				A.push_back(vr);
				IP vd(fnB);
				B.push_back(vd);
			}
			s.push(A[A.size()-1].end);
			s.push(B[B.size()-1].end);
		}
		else{
			s.pop();
		}
	}
	if(flag != 0){
		vector<int> temp;
		temp.push_back(s_cont);
		IP temp1(temp);
		S.push_back(temp1);
		A.push_back(temp1);
		B.push_back(temp1);
		s_cont++; 
	}
	/*cout<<"vectors in S :"<<endl;
	for(int i = 0; i < S.size(); i++){
		for(int j = 0; j < S[i].end.size(); j++)
			cout<<S[i].end[j]<<" ";
		cout<<endl;
	}
	cout<<endl;*/
	for(int i = 0; i < S.size(); i++){
		Transition temp_a(S[i].end,A[i].end,'a');
		Transition temp_b(S[i].end,B[i].end,'b');
		T.push_back(temp_a);
		T.push_back(temp_b);
	}
	end_state(S,Final_State,final_state);
}


void display(vector<transition> &t, int initial_state, vector<int> &final_state){
	cout<<"Transition Function of the given NFA : "<<endl;
	for(int n = 0; n < t.size(); n++){
		cout<<"q("<<t[n].initial<<","<<t[n].input<<")"<<"->"<<"q("<<t[n].final<<")"<<endl;
	}
	cout<<"Starting State : "<<initial_state<<endl;
	cout<<"Final State : ";
	for(int i = 0; i < final_state.size(); i++){
		cout<<final_state[i]<<" ";
	}
	cout<<endl;
}


void Display(vector<Transition> &T, int Initial_State, vector<vector<int> > &Final_State){
	cout<<"Transition Function of the corresponding DFA : "<<endl;
	for(int i = 0; i < T.size(); i++){
		cout<<"q({";
		for(int j = 0; j < T[i].Initial.size(); j++){
			cout<<T[i].Initial[j];
			if(j != T[i].Initial.size()-1)
				cout<<",";
			else
				cout<<"},";
		}
		cout<<T[i].Input<<")"<<"->"<<"q({";
		for(int j = 0; j < T[i].Final.size(); j++){
			cout<<T[i].Final[j];
			if(j != T[i].Final.size()-1)
				cout<<",";
			else
				cout<<"})";
		}
		cout<<endl;
	}
	cout<<"Starting State : "<<Initial_State<<endl;
	cout<<"Final State : "<<endl;
	cout<<"{";
	for(int i = 0; i < Final_State.size(); i++){
		cout<<"{";
		for(int j = 0; j < Final_State[i].size(); j++){
			cout<<Final_State[i][j];
			if(j != Final_State[i].size()-1)
				cout<<",";
			else
				cout<<"}";
		}
		cout<<"}"<<endl;
	}
}


int main(){
	vector<transition> t;
	transition t1(0,0,'a');
	t.push_back(t1);
	transition t2(0,0,'b');
	t.push_back(t2);
	transition t3(0,1,'a');
	t.push_back(t3);
	transition t4(1,2,'b');
	t.push_back(t4);
	int s_cont = 3;

	int initial_state = 0;
	vector<int> final_state;
	final_state.push_back(2);
	display(t,initial_state,final_state);

	vector<Transition> T;
	int Initial_State = initial_state;
	vector<vector<int> > Final_State;
	NFAtoDFA(s_cont,T,Initial_State,Final_State,t,final_state);
	Display(T,Initial_State,Final_State);
	return 0;
}