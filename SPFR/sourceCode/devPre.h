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
int TOP_K_NEIGHBOR[5] = {5,10,15,20,25};
#define RECALL_K_5 5
#define RECALL_K_10 10
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
	string assignee;
	vector<string> commenter;
	set<string> dev;
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

//vector<Result> bmRes[groupSize];
//vector<Result> Neibor[groupSize];


map<string,double> dev_SocialScore;
map<string,double> dev_ExperienceScore;
struct Dev_pre
{
	string bugid;
	vector<pair<string,double> > top_k_dev;
};
vector<Dev_pre> devCommenList_5[5];
vector<Dev_pre> devCommenList_10[5];
double Ri_total[5] = {0};
double Final_Recall_5[5] = { 0 };
double Final_Precision_5[5] = { 0 };
double Final_Recall_10[5] = { 0 };
double Final_Precision_10[5] = { 0 };
double Final_MRR[5] = { 0 };




double lenSum_unigram = 0;
double lenDesc_unigram = 0;
double lenSum_bigram = 0;
double lenDesc_bigram = 0;


map<string, set<string> > word_inBR_unigram; 
map<pair<string,string>, set<string> > word_inBR_bigram; 

map<string,int> getDocID;
//map<string,  set<string> > comWord_unigram[groupSize]; //trainBRid -> commWord
//map< string, set<pair<string,string> > > comWord_bigram[groupSize]; //trainBRid -> commWord

void getCommWord();
vector<cBugReport> train;
vector<cBugReport> test;



cBugReport getBR(string str,int type);
vector<cBugReport> LoadCSV(const char* path,int type);
vector<cBugReport> trainBR; 
vector<cBugReport> testBR;  

map<string, string> id_dev_Reopened; //bugid -> devName
map<string, vector<string> > dev_id_Reopened; //devName -> reopen_bugidm

map<string,int > dev_comment_times;  // devName -> (bugid,times) dev评论别人的次数
map<string,int > dev_commented_times; // devName -> (bugid,times) dev被别人评论的次数
map<string,int> dev_fixed;  //dev修复的bug数量

