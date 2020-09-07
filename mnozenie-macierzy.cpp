#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <omp.h>
#include <thread>
#include<chrono>
using namespace std;

vector<double> A,B, *t;
vector<vector<double>> V1,V2, *temp;

void read(string arg, char sep)
{

	fstream plik;
	plik.open( arg, ios::in );
	if( plik.good() )
	    {
	        string napis,a;
	        //char sep=' ';
	        temp= &V1;
	        t=&A;
	        while( !plik.eof() )
	        {
	            getline( plik, napis);
	            if (napis.empty())
	            {
	            	temp=&V2;
	            	t=&B;
	            }
	            auto i =0;
	            a="";
	            auto leng =napis.length();
	            while((i<=leng )& (leng>0))
	            	{
	                	if ((napis[i]==sep) |(i==leng) )
	                		{
	                       if (a!="") (*t).push_back(stod(a));
	                        a="";
	                		}
	                    else a=a+napis[i];
	                    i++;
	                }
	               if( !(*t).empty()) (*temp).push_back((*t));
	               (*t).clear();
	        }

	        plik.close();
	    } else cout << "Error! Nie udalo otworzyc sie pliku!" << endl;
}


vector<vector<double>>  trans(vector<vector<double>> &V){

	int len =V[0].size();
	int len1 =V.size();
	vector<vector<double>> Vt(len,vector<double>(len1));
	for (auto i =0; i<V[0].size(); i++)
	{
		Vt[i].clear();
	for(auto j=0; j<V.size(); j++)
		{
			Vt[i].push_back(V[j][i]);
		}
	}
return Vt;
}

vector<vector<double>>  mnozenie(vector<vector<double>> V1,vector<vector<double>> V2){

	int len1 =V1.size();
	int len2 =V2.size();
	if (V1[0].size()==V2[0].size()){
	vector<vector<double>> Vt(len1,vector<double>(len2,0));
#pragma omp parallel num_threads(len1)
{
#pragma omp for
	for (int i =0; i<len1; i++)
	{

	for(int j=0; j<len2; j++)
		{
			for(int k=0; k<V2[0].size(); k++)
			{
				Vt[i][j]+=V1[i][k]*V2[j][k];
			}
		}
	}
}
return Vt;
	}
	else {
		cout<<"nie odpowiedni wymiar macierzy"<<endl;
		return V1;
	}
	}


void na_ekran(vector<vector<double>> vec , int len_i, int len_j){
	for (int i=0;i<len_i;i++){
		for (int j=0;j<len_j;j++){
			cout<< vec[i][j]<<"  ";
		}
		cout<<endl;
	}
}


void do_pliku(vector<vector<double>> vec , int len_i, int len_j){
	ofstream plik;
	string sep="  ";
	plik.open( "Wynik.txt", ios::out);
	if( plik.good() )
		    {
				for (int i=0;i<len_i;i++){
					for (int j=0;j<len_j;j++){
						plik<< vec[i][j]<<sep;
					}
					plik<<endl;
				}
				plik.close();
		    }
	 else cout << "Error! Nie udalo otworzyc sie pliku!" << endl;
}

int main (int argc, char **argv)
{
read(argv[1],' ');
V2=trans(V2);
int len_i=V1.size();
int len_j =V2.size();
int len_k=V2[0].size();
vector<vector<double>> Wynik, Wynik_Thread (len_i,vector<double>(len_j,0));

Wynik=mnozenie(V1,V2);// mnożenie wielowatkowe z użyciem OpenMP

/* poniżej przedtawiona realizacja
dla mnozenia wielowątkowego za pomocą thread*/
if (V1[0].size()==V2[0].size()){
auto th= [&V1, &V2, &Wynik_Thread]( int i) -> void{
	for(int j=0; j<(V2).size(); j++)
		{
			for(int k=0; k<V2[0].size(); k++)
				{
					Wynik_Thread[i][j]+=V1[i][k]*V2[j][k];
					/*this_thread::sleep_for(chrono::milliseconds (1000));
					if (i==2)this_thread::sleep_for(chrono::milliseconds (1000));
					cout<< this_thread::get_id()<< "  "<< i <<endl; */
				}
		}
};
	vector<thread>  ths;
	for(int p = 0; p < len_i; p++){
		ths.push_back(thread (th,p));
	}


	for (thread& t : ths) t.join();
	na_ekran(Wynik,len_i,len_j);
	na_ekran(Wynik_Thread,len_i,len_j);
	do_pliku(Wynik_Thread,len_i,len_j);

	return 0;




}
else {
		cout<<"nie odpowiedni wymiar macierzy"<<endl;
		return 1;
	};



}


