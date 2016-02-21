#include "head.h"
int string_to_int(string str)
{
	int ans = 0;
	for(int i = 0;i < str.size();i++)
		ans = ans*10 + (str[i]-'0');
	return ans;
}

string int_to_string(int num)
{
	stringstream ss;
	ss << num;
	string s1 = ss.str();
	return s1;
}

void getReopen(const char* path)
{
	ifstream in;
	in.open(path);
	if (in.fail())  
		cout << "File not found" <<endl; 
	string line;
	while(getline(in, line)  && in.good() )
	{
		int idx[3];
		int temp = 0;
		for(int i = 0;i < line.size();i++)
		{
			if(line[i] == ',')
			{
				if(temp == 0)
					idx[0] = i;
				else if(temp == 2)
					idx[1] = i;
				else if(temp == 3)
					idx[2] = i;
				else if(temp > 3)
					break;
				temp++;
			}
		}
		string bug_id = line.substr(0,idx[0]);
		string bug_dev = line.substr(idx[1]+1,idx[2]-idx[1]-1);
		id_dev_Reopened[bug_id] = bug_dev;
		dev_id_Reopened[bug_dev].push_back(bug_id);
	}
}
cBugReport getBR(string str,int type)
{
	cBugReport tempBR;
	int idx = str.find(',');
	string bugID = str.substr(0,idx); //get bugid
	str = str.substr(idx+1,str.size());
	tempBR.bugID = bugID;

	idx = str.find(',');
	tempBR.prod = str.substr(0,idx); //get prod
	str = str.substr(idx+1,str.size());


	idx = str.find(',');
	string strSumm = str.substr(0,idx); //get summary
	str = str.substr(idx+1,str.size());
	string fea = "";
	int wTimes = 0;
	pair<string,string> temp_bigram;
	for(int i = 0;i < strSumm.size();i++)
	{
		if(strSumm[i] == ' ')
		{
			tempBR.summ_unigram.push_back(fea);
			tempBR.summ_times_unigram[fea]++;
			if(type == 1)
				word_inBR_unigram[fea].insert(tempBR.bugID);
			
			tempBR.totalWords_unigram.push_back(fea);
			wTimes++;
			if(wTimes == 2) //bigram
			{
				temp_bigram = make_pair(tempBR.summ_unigram[0], tempBR.summ_unigram[1]);
				tempBR.summ_bigram.push_back(temp_bigram);
				tempBR.summ_times_bigram[temp_bigram]++;
				tempBR.totalWords_bigram.push_back(temp_bigram);
				if(type == 1)
					word_inBR_bigram[temp_bigram].insert(tempBR.bugID);
			}
			else if(wTimes > 2)
			{
				temp_bigram.first = temp_bigram.second;
				temp_bigram.second = fea;
				tempBR.summ_bigram.push_back(temp_bigram);
				tempBR.summ_times_bigram[temp_bigram]++;
				tempBR.totalWords_bigram.push_back(temp_bigram);
				if(type == 1)
					word_inBR_bigram[temp_bigram].insert(tempBR.bugID);
			}
			fea = "";
		}
		else
			fea += strSumm[i];
	}
	
	
	idx = str.find(',');
	string strDesc = str.substr(0,idx); //get Desc
	str = str.substr(idx+1,str.size());
	fea = "";
	wTimes = 0;
	for(int i = 0;i < strDesc.size();i++)
	{
		if(strDesc[i] == ' ')
		{
			tempBR.desc_unigram.push_back(fea);
			tempBR.desc_times_unigram[fea]++;
			tempBR.totalWords_unigram.push_back(fea);
			wTimes++;
			if(type == 1)
				word_inBR_unigram[fea].insert(tempBR.bugID);
			if(wTimes == 2) //bigram
			{
				temp_bigram = make_pair(tempBR.desc_unigram[0], tempBR.desc_unigram[1]);
				tempBR.desc_bigram.push_back(temp_bigram);
				tempBR.desc_times_bigram[temp_bigram]++;
				tempBR.totalWords_bigram.push_back(temp_bigram);
				if(type == 1)
					word_inBR_bigram[temp_bigram].insert(tempBR.bugID);
			}
			else if(wTimes > 2)
			{
				temp_bigram.first = temp_bigram.second;
				temp_bigram.second = fea;
				tempBR.desc_bigram.push_back(temp_bigram);
				tempBR.desc_times_bigram[temp_bigram]++;
				tempBR.totalWords_bigram.push_back(temp_bigram);
				if(type == 1)
					word_inBR_bigram[temp_bigram].insert(tempBR.bugID);
			}
			fea = "";
		}
		else
			fea += strDesc[i];
	}
	lenSum_unigram += tempBR.summ_unigram.size();
	lenSum_bigram += tempBR.summ_bigram.size();
	lenDesc_unigram += tempBR.desc_unigram.size();
	lenDesc_bigram += tempBR.desc_bigram.size();

	idx = str.find(',');
	tempBR.comp = str.substr(0,idx); //get comp
	str = str.substr(idx+1,str.size());

	idx = str.find(',');
	tempBR.sever = str.substr(0,idx); 
	str = str.substr(idx+1,str.size()); //get sev

	idx = str.find(',');
	tempBR.priority = str.substr(0,idx);
	str = str.substr(idx+1,str.size()); //get priority

	idx = str.find(',');
	tempBR.assignee = str.substr(0,idx);
	tempBR.dev.insert(tempBR.assignee);
	str = str.substr(idx+1,str.size()); //get assignee


	idx = str.find(',');
	string sDev = str.substr(0,idx);
	str = str.substr(idx+1,str.size());

	int tidx = -1;
	while(tidx = sDev.find(" "), tidx != -1)
	{
		string temp = sDev.substr(0,tidx);
		sDev = sDev.substr(tidx+1,sDev.size());

		int split = temp.find("&");
		string comName = temp.substr(0,split);
		int comTimes = string_to_int(temp.substr(split+1,temp.size()));
		tempBR.dev.insert(comName);
		tempBR.commenter.push_back(comName);
		if(type == 1)
		{
			dev_comment_times[comName] += comTimes;
			dev_commented_times[tempBR.assignee] += comTimes;
		}
		
	}
	
	idx = str.find(',');
	tempBR.topic = str.substr(0,idx);
	str = str.substr(idx+1,str.size());

	if(str == "FIXED" && type == 1)
		dev_fixed[tempBR.assignee]++;
	
	//tempBR.summ_bigram.clear();
	//tempBR.summ_unigram.clear();
	//tempBR.desc_bigram.clear();
	//tempBR.desc_unigram.clear();
	return tempBR;
}
vector<cBugReport> LoadInfo(int fileNum, int type, string theme)
{
	vector<cBugReport> mBugReport;
	if (type == 1)
	{
		for (int i = 0; i < fileNum; i++)
		{
			string path = theme + int_to_string(i) + ".txt";
			cout << "Loading \\" << path << "..." << endl;
			ifstream in(path);
			string line;
			int idx = 0;
			while (getline(in, line))
			{
				cBugReport tempBR = getBR(line, type);
				getDocID[tempBR.bugID] = idx;
				mBugReport.push_back(tempBR); //save bug_report
				idx++;
			}
		}
	}

	else if (type == 2)
	{
		string path = theme + int_to_string(fileNum) + ".txt";
		cout << "Loading \\" << path << "..." << endl;
		ifstream in(path);
		string line;
		int idx = 0;
		while (getline(in, line))
		{
			cBugReport tempBR = getBR(line, type);
			getDocID[tempBR.bugID] = idx;
			mBugReport.push_back(tempBR);
			idx++;
		}
	}


	cout << "Size: " << mBugReport.size() << endl;
	outFile << "Size: " << mBugReport.size() << endl;
	return mBugReport;
}
double getSocialScore(string dev)
{
	return dev_comment_times[dev] * dev_commented_times[dev];
}

double getExperienceScore(string dev)
{
	return (double)dev_fixed[dev] / (dev_id_Reopened[dev].size()+1);
}

map<string,  set<string> > getcomWord_unigram(cBugReport tempBR)
{
	map<string,  set<string> > comWord_unigram; //trainBRid -> commWord
	for(int j = 0;j < tempBR.totalWords_unigram.size();j++)
	{
		if(word_inBR_unigram.count(tempBR.totalWords_unigram[j]) > 0)
		{
			set<string>::iterator ite;
			set<string> temp = word_inBR_unigram[tempBR.totalWords_unigram[j]]; //temp保存出现过word的trainBR.bugid
			for(ite = temp.begin(); ite != temp.end(); ite++)
				comWord_unigram[*ite].insert(tempBR.totalWords_unigram[j]);
		}
	}
	return comWord_unigram;
}

map< string, set<pair<string,string> > > getcomWord_bigram(cBugReport tempBR)
{
	map< string, set<pair<string,string> > > comWord_bigram;
	for(int j = 0;j < tempBR.totalWords_bigram.size();j++)
	{
		if(word_inBR_bigram.count(tempBR.totalWords_bigram[j]) > 0)
		{
			set<string>::iterator ite;
			set<string> temp = word_inBR_bigram[tempBR.totalWords_bigram[j]]; //temp保存出现过word的trainBR.bugid
			for(ite = temp.begin(); ite != temp.end(); ite++)
				comWord_bigram[*ite].insert(tempBR.totalWords_bigram[j]);
		}
	}
	return comWord_bigram;
}

static double getTFD_unigram(string str, int DocID)
{

	double res = 0;
	double wf1[2] = { 2.98, 0.287 };
	double bf_summ = 0.50;
	double bf_desc = 1.0;
	if(trainBR[DocID].summ_times_unigram.count(str))
		res += wf1[0] * trainBR[DocID].summ_times_unigram[str] / (1.0 - bf_summ + bf_summ * trainBR[DocID].summ_unigram.size() / (lenSum_unigram / trainBR.size()));
	if(trainBR[DocID].desc_times_unigram.count(str))
	{
		//double t1 = wf1[1]*trainBR[DocID].desc_times_unigram[str];
		//double t2 = (lenDesc_unigram/trainBR.size());
		//double t3 = 2.0*bf * trainBR[DocID].desc_unigram.size();
		res += wf1[1] * trainBR[DocID].desc_times_unigram[str] / (1.0 - bf_desc + bf_desc * trainBR[DocID].desc_unigram.size() / (lenDesc_unigram / trainBR.size()));
	}
	return res;
}

static double getTFD_bigram(pair<string,string> str, int DocID)
{

	double res = 0;
	double wf2[2] = { 2.999, 0.994 };
	double bf_summ = 0.5;
	double bf_desc = 1.0;
	if(trainBR[DocID].summ_times_bigram.count(str))
		res += wf2[0] * trainBR[DocID].summ_times_bigram[str] / (1.0 - bf_summ + bf_summ * trainBR[DocID].summ_bigram.size() / (lenSum_bigram / trainBR.size()));
	if(trainBR[DocID].desc_times_bigram.count(str))
		res += wf2[1] * trainBR[DocID].desc_times_bigram[str] / (1.0 - bf_desc + bf_desc * trainBR[DocID].desc_bigram.size() / (lenDesc_bigram / trainBR.size()));
	
	return res;
}

static double getIDF_unigram(string str)
{
	int nt = 0;
	nt = word_inBR_unigram[str].size();
	if(!nt)
		return 0;
	return log((double)trainBR.size() / nt);
}

static double getIDF_bigram(pair<string,string> str)
{
	int nt = 0;
	nt = word_inBR_bigram[str].size();
	if(!nt)
		return 0;
	return log((double)trainBR.size() / nt);
}

static double getWQ_unigram(string str,int docID)
{
	double L = 0.001; 
	double wf1[2] = { 2.98, 0.287 };    //w_summ,w_desc
	double TFQ = 0;

	int t1 = testBR[docID].summ_times_unigram[str]; 
	int t2 = testBR[docID].desc_times_unigram[str]; 
	TFQ += wf1[0] * t1;
	TFQ += wf1[1] * t2;
	return (L+1)*TFQ / (L+TFQ);
}

static double getWQ_bigram(pair<string,string> str,int docID)
{
	double L = 0.001; 
	double wf2[2] = { 2.999, 0.994 };   //w_summ,w_desc
	double TFQ = 0;
	int t1 = testBR[docID].summ_times_bigram[str];
	int t2 = testBR[docID].desc_times_bigram[str]; 
	TFQ += wf2[0] * t1;
	TFQ += wf2[1] * t2;
	return (L+1)*TFQ / (L+TFQ);
}

bool cmp(Result a, Result b)
{
	return a.REP > b.REP;
}

bool Dev_Score_cmp(pair<string,double> a, pair<string,double> b)
{
	return a.second > b.second;
}


static double setUnion(vector<pair<string,double> > top_k_dev, set<string> real)
{
	double sunion = 0;
	for(int i = 0;i < top_k_dev.size();i++)
		if(real.count(top_k_dev[i].first))
			sunion++;
	return sunion;
}

double getRecallK(vector<Dev_pre> dev_pre)
{
	//map<string,set<string> > dev_pre bugid -> 预测出来的开发者
	double recall = 0;
	for(int i = 0;i < dev_pre.size();i++)
		recall += (setUnion(dev_pre[i].top_k_dev, testBR[i].dev) / testBR[i].dev.size());
	//cout << recall << " " <<  dev_pre.size() << endl;
	return (recall / dev_pre.size());
}

double getPrecisionK(vector<Dev_pre> dev_pre)
{
	//map<string,set<string> > dev_pre bugid -> 预测出来的开发者
	double recall = 0;
	for(int i = 0;i < dev_pre.size();i++)
		recall += (setUnion(dev_pre[i].top_k_dev, testBR[i].dev) / dev_pre[i].top_k_dev.size());
	return (recall / dev_pre.size());
}

vector<Result> getBM25FextAndREP(cBugReport tempBR,int i) 
{
	map<string, set<string> > comWord_unigram = getcomWord_unigram(tempBR);
	map< string, set<pair<string,string> > > comWord_bigram = getcomWord_bigram(tempBR);
	vector<Result> bmRes;
	for(int j = 0;j < trainBR.size();j++)
	{
		double bmScore_unigram = 0;
		double bmScore_bigram = 0;
		double res = 0;
		set<string>::iterator ite;
		set<pair<string,string> >::iterator ite2;
		for(ite = comWord_unigram[trainBR[j].bugID].begin(); ite != comWord_unigram[trainBR[j].bugID].end(); ite++) 
		{
			double TFD = getTFD_unigram(*ite, j);
			double k_ctl = 2; //调节因子   k1_unigram
			bmScore_unigram += getIDF_unigram(*ite) * (TFD / (TFD+k_ctl)) * getWQ_unigram(*ite,i);
		}

		for(ite2 = comWord_bigram[trainBR[j].bugID].begin(); ite2 != comWord_bigram[trainBR[j].bugID].end(); ite2++)
		{
			double TFD2 = getTFD_bigram(*ite2, j);
			double k_ctl = 2; //调节因子  k1_bigram
			bmScore_bigram += getIDF_bigram(*ite2) * (TFD2 / (TFD2+k_ctl)) * getWQ_bigram(*ite2,i);
		}

		double w[5] = { 1.163, 0.013, 2.285, 0.032, 1.0 };
		res = w[0] * bmScore_unigram;
		res += w[1] * bmScore_bigram;
		res += w[2] * (tempBR.prod == trainBR[j].prod);
		res += w[3] * (tempBR.comp == trainBR[j].comp);
		res += w[4] * (tempBR.topic == trainBR[j].topic);
		Result Rtemp;
		Rtemp.REP = res;
		Rtemp.bug_id = trainBR[j].bugID;
		Rtemp.docID = j;
		bmRes.push_back(Rtemp);
	}
	return bmRes;
}

void getDevcom(vector<Result> bmRes, cBugReport tempBR)
{

	set<string> dev_res[5]; //最终预测出来的开发者集合
	sort(bmRes.begin(), bmRes.end(), cmp);
	for(int i = 0;i < 5;i++)
	{
		vector<Result> Neibor;
		int top_k = TOP_K_NEIGHBOR[i];
		for(int j = 0;j < top_k;j++)
		{
			Neibor.push_back(bmRes[j]);
			dev_res[i].insert(trainBR[getDocID[bmRes[j].bug_id]].assignee);
			for(int k = 0;k < trainBR[getDocID[bmRes[j].bug_id]].commenter.size();k++)
				dev_res[i].insert(trainBR[getDocID[bmRes[j].bug_id]].commenter[k]);
		}
	}
	double w1 = 0.1;
	double w2 = 0.8;
	for (int i = 0; i < 5; i++)
	{
		set<string>::iterator ite;
		double max_SocialScore = -1;
		double max_ExperienceScore = -1;
		for (ite = dev_res[i].begin(); ite != dev_res[i].end(); ite++)
		{
			double SocialScore = getSocialScore(*ite);
			double ExperienceScore = getExperienceScore(*ite);
			if (SocialScore > max_SocialScore)
				max_SocialScore = SocialScore;
			if (ExperienceScore > max_ExperienceScore)
				max_ExperienceScore = ExperienceScore;
			dev_SocialScore[*ite] = SocialScore;
			dev_ExperienceScore[*ite] = ExperienceScore;
		}

		vector<pair<string, double> > dev_Score; //最后的devScore
		for (ite = dev_res[i].begin(); ite != dev_res[i].end(); ite++)
		{
			double devScore = 0;
			if (max_SocialScore != 0)
				devScore += w1 * dev_SocialScore[*ite] / max_SocialScore;
			if (max_ExperienceScore != 0)
				devScore += w2 * dev_ExperienceScore[*ite] / max_ExperienceScore;
			dev_Score.push_back(make_pair(*ite, devScore));
		}
		sort(dev_Score.begin(), dev_Score.end(), Dev_Score_cmp);
		double temp_ri = 0;
		for (int j = 0; j < dev_Score.size(); j++)
		{
			if (tempBR.assignee == dev_Score[j].first)
			{
				temp_ri = 1.0 / (j + 1);
				break;
			}
		}
		Dev_pre temp_5, temp_10;
		temp_5.bugid = tempBR.bugID;
		temp_10.bugid = tempBR.bugID;
		int Loop_5 = RECALL_K_5 > dev_Score.size() ? dev_Score.size() : RECALL_K_5;
		int Loop_10 = RECALL_K_10 > dev_Score.size() ? dev_Score.size() : RECALL_K_10;
		for (int k = 0; k < Loop_5; k++)
			temp_5.top_k_dev.push_back(dev_Score[k]);
		for (int k = 0; k < Loop_10; k++)
			temp_10.top_k_dev.push_back(dev_Score[k]);
		devCommenList_5[i].push_back(temp_5);
		devCommenList_10[i].push_back(temp_10);
		Ri_total[i] += temp_ri;
	}
}

double getF(double precision, double recall)
{
	return 2.0 * precision * recall / (precision + recall);
}

void getResult()
{
	for(int k_idx = 0;k_idx < 5;k_idx++)
	{
		outFile << " ----------------K = " << TOP_K_NEIGHBOR[k_idx] << "------------------------" << endl;

		double recall_k_5 = getRecallK(devCommenList_5[k_idx]);
		double precision_k_5 = getPrecisionK(devCommenList_5[k_idx]);
		double F5 = getF(precision_k_5, recall_k_5);
		Final_Recall_5[k_idx] += recall_k_5;
		Final_Precision_5[k_idx] += precision_k_5;
				

		double recall_k_10 = getRecallK(devCommenList_10[k_idx]);
		double precision_k_10 = getPrecisionK(devCommenList_10[k_idx]);
		double F10 = getF(precision_k_10, recall_k_10);

		Final_Recall_10[k_idx] += recall_k_10;
		Final_Precision_10[k_idx] += precision_k_10;

		double MRR = Ri_total[k_idx] / testBR.size();
		Final_MRR[k_idx] += MRR;
		
		outFile << "recall" << RECALL_K_5 << " ===== " << recall_k_5 << endl;
		outFile << "precision" << RECALL_K_5 << " ===== " << precision_k_5 << endl;
		outFile << "F_measure" << RECALL_K_5 << " ===== " << F5 << endl;

		outFile << "recall" << RECALL_K_10 << " ===== " << recall_k_10 << endl;
		outFile << "precision" << RECALL_K_10 << " ===== " << precision_k_10 << endl;
		outFile << "F_measure" << RECALL_K_10 << " ===== " << F10 << endl;

		outFile << "MRR  ===== " << MRR << endl;

		cout << "recall" << RECALL_K_5 << " ===== " << recall_k_5 << endl;
		cout << "precision" << RECALL_K_5 << " ===== " << precision_k_5 << endl;
		cout << "F_measure" << RECALL_K_5 << " ===== " << F5 << endl;

		cout << "recall" << RECALL_K_10 << " ===== " << recall_k_10 << endl;
		cout << "precision" << RECALL_K_10 << " ===== " << precision_k_10 << endl;
		cout << "F_measure" << RECALL_K_10 << " ===== " << F10 << endl;

		cout << "MRR  ===== " << MRR << endl;
		
	}

}

void init()
{
	testBR.clear();
	trainBR.clear();
	word_inBR_unigram.clear();
	word_inBR_bigram.clear();
	getDocID.clear();
	lenSum_unigram = 0;
	lenDesc_unigram = 0;
	lenSum_bigram = 0;
	lenDesc_bigram = 0;
	dev_comment_times.clear();
	dev_commented_times.clear();
	dev_fixed.clear();
	for (int i = 0; i < 5; i++)
	{
		devCommenList_5[i].clear();
		devCommenList_10[i].clear();
		Ri_total[i] = 0;
	}
}

void printResult(int FileSize)
{
	cout << "--------------------final result----------------------" << endl;
	outFile << "--------------------final result----------------------" << endl;
	int topN[5] = { 5, 10, 15, 20, 25 };;
	for (int k = 0; k < 5; k++)
	{
		double f5 = (Final_Recall_5[k] + Final_Precision_5[k]) == 0 ? 0 :
			(2 * Final_Recall_5[k] * Final_Precision_5[k] / (Final_Recall_5[k] + Final_Precision_5[k]));
		double f10 = (Final_Recall_10[k] + Final_Precision_10[k]) == 0 ? 0 :
			(2 * Final_Recall_10[k] * Final_Precision_10[k] / (Final_Recall_10[k] + Final_Precision_10[k]));
		outFile << "-------------k = " << topN[k] << "-------------------" << endl;
		outFile << "recall5 =====" << Final_Recall_5[k] / FileSize * 100 << "%" << endl;
		outFile << "Precision5 =====" << Final_Precision_5[k] / FileSize * 100 << "%" << endl;
		outFile << "F5 =====" << f5 / FileSize * 100 << "%" << endl;

		outFile << "recall10 =====" << Final_Recall_10[k] / FileSize * 100 << "%" << endl;
		outFile << "Precision10 =====" << Final_Precision_10[k] / FileSize * 100 << "%" << endl;
		outFile << "F10 =====" << f10 / FileSize * 100 << "%" << endl;

		outFile << "MRR ===== " << Final_MRR[k] / FileSize * 100 << "%" << endl;
	}
}
int main()
{
	/********************************filePath***********************************/
	string reOpenPath = "Mozilla_reopened.csv";
	string resultPath = "sample_result.txt";
	string theme = "sample_dev";
	/********************************filePath***********************************/
	start_time = clock();
	outFile.open(resultPath);
	getReopen(reOpenPath.c_str());

	int FileSize = 10;
	for (int fileNum = 1; fileNum <= FileSize; fileNum++)
	{
		init();
		trainBR = LoadInfo(fileNum, 1, theme);
		testBR  = LoadInfo(fileNum, 2, theme);
		for (int i = 0; i < testBR.size(); i++)
		{
			if (i % 10 == 0)
				printf("Proceed：%.2f%%\n", ((double)i / testBR.size() * 100));
			vector<Result> bmRes = getBM25FextAndREP(testBR[i], i);
			getDevcom(bmRes, testBR[i]);
		}
		getResult();
	}
	printResult(FileSize);
	
	clock_t end_time = clock();
	cout << "Running time is: " << static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC<<"s"<<endl;
	outFile << "Running time is: " << static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}