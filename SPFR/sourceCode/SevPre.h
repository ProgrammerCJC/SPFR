#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <set>
#include <ctime>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <sstream>
using namespace std;
//#define TOP_K_NEIGHBOR 10
const int maxSize = 1e6;
const int groupSize = 5e4;
const int querySize = 1e4;

ofstream outFile;
clock_t start_time;

struct cBugReport
{
	string bugID;
	string topic;
	string prod;
	string comp;
	string sever;
	string priority;

	vector<string> summ_unigram;
	vector<string> desc_unigram;
	map<string,int> summ_times_unigram;
	map<string,int> desc_times_unigram;
	vector<string> totalWords_unigram;


	vector<pair<string,string> > summ_bigram;
	vector<pair<string,string> > desc_bigram;
	vector<pair<string,string> > totalWords_bigram;
	map<pair<string,string> ,int> summ_times_bigram;
	map<pair<string,string> ,int> desc_times_bigram;

};

struct Result
{
	string bug_id;
	int docID;
	double REP;
};

double lenSum_unigram = 0;
double lenDesc_unigram = 0;
double lenSum_bigram = 0;
double lenDesc_bigram = 0;

map<string,int> sevScore;
map<string, set<string> > word_inBR_unigram; 
map<pair<string,string>, set<string> > word_inBR_bigram; 

map<string,int> getDocID;

vector<cBugReport> train;
vector<cBugReport> test;

double alpha1[5] = { 0.3, 0.9, 0.7, 0.7, 0.7 };
double alpha2[5] = { 0.3, 0.9, 0.7, 0.7, 0.7 };

vector<cBugReport> trainBR; 
vector<cBugReport> testBR; 


int preNum[5][2][5] = {0};
int rightPre[5][2][5]= {0};
int sevCount[3][5] = {0}; 

double precision_total[5][2][5] = {0};
double recall_total[5][2][5] = {0};
double F_total[5][2][5] = {0};
