#include "SevPre.h"
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
	sevCount[type][sevScore[tempBR.sever]]++; //计算5个严重度标签的数量
	
	idx = str.find(',');
	tempBR.priority = str.substr(0,idx);
	str = str.substr(idx+1,str.size()); //get priority

	idx = str.find(',');
	str = str.substr(idx+1,str.size()); //get assignee


	idx = str.find(',');
	string sDev = str.substr(0,idx);
	str = str.substr(idx+1,str.size());

	
	idx = str.find(',');
	tempBR.topic = str.substr(0,idx);
	str = str.substr(idx+1,str.size());

	//tempBR.summ_bigram.clear();
	//tempBR.summ_unigram.clear();
	//tempBR.desc_bigram.clear();
	//tempBR.desc_unigram.clear();
	return tempBR;
}
vector<cBugReport> LoadInfo(int fileNum,int type,string theme)
{
	vector<cBugReport> mBugReport;
	if(type == 1) // train
	{
		for(int i = 0;i < fileNum;i++)
		{
			string path = theme + int_to_string(i) + ".txt";
			cout << "Loading \\" << path << "..." << endl;
			ifstream in(path); 
			string line;
			int idx = 0;
			while(getline(in, line))
			{
				cBugReport tempBR = getBR(line,type);
				getDocID[tempBR.bugID] = idx;
				mBugReport.push_back(tempBR); //save bug_report
				idx++;
			}
		}
	}
	else if(type == 2)
	{
		string path = theme + int_to_string(fileNum) + ".txt";	
		cout << "Loading \\" << path << "..." << endl;
		ifstream in(path); 
		string line;
		int idx = 0;
		while(getline(in, line))
		{
			cBugReport tempBR = getBR(line,type);
			getDocID[tempBR.bugID] = idx;
			mBugReport.push_back(tempBR); 
			idx++;
		}
	}
	cout << "Size: " << mBugReport.size() << endl;
	outFile << "Size: " << mBugReport.size() << endl;
	return mBugReport;
}


map<string,  set<string> > getcomWord_unigram(cBugReport tempBR)
{
	map<string,  set<string> > comWord_unigram; //trainBRid -> commWord
	for(int j = 0;j < tempBR.totalWords_unigram.size();j++)
	{
		if(word_inBR_unigram.count(tempBR.totalWords_unigram[j]) > 0)
		{
			set<string>::iterator ite;
			set<string> temp = word_inBR_unigram[tempBR.totalWords_unigram[j]]; 
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
			set<string> temp = word_inBR_bigram[tempBR.totalWords_bigram[j]]; 
			for(ite = temp.begin(); ite != temp.end(); ite++)
				comWord_bigram[*ite].insert(tempBR.totalWords_bigram[j]);
		}
	}
	return comWord_bigram;
}

static double getTFD_unigram(string str, int DocID)
{
	double res = 0;
	double wf1[2] = { 3.014, 0.764 };
	double bf_summ = 0.499;
	double bf_desc = 1.003;
	if(trainBR[DocID].summ_times_unigram.count(str))
		res += wf1[0] * trainBR[DocID].summ_times_unigram[str] / (1.0 - bf_summ + bf_summ * trainBR[DocID].summ_unigram.size() / (lenSum_unigram / trainBR.size()));
	if(trainBR[DocID].desc_times_unigram.count(str))
	{
		res += wf1[1] * trainBR[DocID].desc_times_unigram[str] / (1.0 - bf_desc + bf_desc * trainBR[DocID].desc_unigram.size() / (lenDesc_unigram / trainBR.size()));
	}
	return res;
}

static double getTFD_bigram(pair<string,string> str, int DocID)
{
	/*******************************************************************
	/	wf[2] 2个field的权重
	/	bf    调节参数 range(0,1) b_desc = 1.0 b_summ = 0.5
	/	type == 1 unigram;  type == 2 bigram
	/******************************************************************/
	double res = 0;
	double wf2[2] = { 2.971, 1.003 };
	double bf_summ = 0.503;
	double bf_desc = 0.969;
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
	double L = 0.031; //控制参数 L==0时，BM25Fext算法变为BM25F算法
	double wf1[2] = {3.014,0.764}; 
	double TFQ = 0;

	int t1 = testBR[docID].summ_times_unigram[str]; //str在query.summ域中出现的次数
	int t2 = testBR[docID].desc_times_unigram[str]; //str在query.desc域中出现的次数
	TFQ += wf1[0] * t1;
	TFQ += wf1[1] * t2;
	return (L+1)*TFQ / (L+TFQ);
}

static double getWQ_bigram(pair<string,string> str,int docID)
{
	double L = 0.154; //控制参数 L==0时，BM25Fext算法变为BM25F算法
	double wf2[2] = {2.971,1.003}; 
	double TFQ = 0;
	int t1 = testBR[docID].summ_times_bigram[str]; //str在query.summ域中出现的次数
	int t2 = testBR[docID].desc_times_bigram[str]; //str在query.desc域中出现的次数
	TFQ += wf2[0] * t1;
	TFQ += wf2[1] * t2;
	return (L+1)*TFQ / (L+TFQ);
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
		for(ite = comWord_unigram[trainBR[j].bugID].begin(); ite != comWord_unigram[trainBR[j].bugID].end(); ite++) //获取testBR[i]和trainBR[j]的ComWord
		{
			double TFD = getTFD_unigram(*ite, j);
			double k_ctl = 2.0; //调节因子
			bmScore_unigram += getIDF_unigram(*ite) * (TFD / (TFD+k_ctl)) * getWQ_unigram(*ite,i);
		}

		for(ite2 = comWord_bigram[trainBR[j].bugID].begin(); ite2 != comWord_bigram[trainBR[j].bugID].end(); ite2++)
		{
			double TFD2 = getTFD_bigram(*ite2, j);
			double k_ctl = 2.0; //调节因子
			bmScore_bigram += getIDF_bigram(*ite2) * (TFD2 / (TFD2+k_ctl)) * getWQ_bigram(*ite2,i);
		}

		double w[5] = {1.159, 0.034, 2.198,0.041,0.988};
		double res_them = 0;
		res = w[0] * bmScore_unigram;
		res += w[1] * bmScore_bigram;
		res += w[2] * (tempBR.prod == trainBR[j].prod);
		res += w[3] * (tempBR.comp == trainBR[j].comp);
		res += w[4] * (tempBR.topic == trainBR[j].topic);
		Result Rtemp_our;
	
		Rtemp_our.REP = res;
		Rtemp_our.bug_id = trainBR[j].bugID;
		Rtemp_our.docID = j;

    	bmRes.push_back(Rtemp_our);
	}
	return bmRes;
}

bool cmp(Result a, Result b)
{
	return a.REP > b.REP;
}

void getSev_our(vector<Result> bmRes,int docID)
{
	
	int topN[5] = {5,10,15,20,25};;
	sort(bmRes.begin(), bmRes.end(), cmp);
	string tempStr[5] = {"blocker","critical", "major", "minor", "trivial"};
	for(int k = 0;k < 5;k++)
	{
		int numLable[5] = {0}; 
		double pLable[5] = {0};
		double neiSim[5] = {0};
		double totalSim = 0;
		for(int j = 0;j < topN[k];j++)
		{
			numLable[sevScore[trainBR[bmRes[j].docID].sever]]++;
			neiSim[sevScore[trainBR[bmRes[j].docID].sever]] += bmRes[j].REP;
			totalSim += bmRes[j].REP;
		}
		for(int j = 0;j < 5;j++)
			pLable[j] = (alpha1[j] * ((double)numLable[j] / topN[k])) + (alpha2[j]) * (neiSim[j] / totalSim);

		int ans = max_element(pLable,pLable+5) - pLable;
		string sevReal = testBR[docID].sever;
		if(tempStr[ans] == testBR[docID].sever)
			rightPre[k][0][ans] ++;
		preNum[k][0][ans]++;
	}

}

void getResult()
{
	int topN[5] = {5,10,15,20,25};;
	for(int k = 0;k < 5;k++)
	{
		outFile << "-------------k = " << topN[k] << "-------------------" << endl;
		double precision[2][5] = {0};
		double recall[2][5] = {0};
		double F[2][5] = {0};
		string tempStr[5] = {"blocker","critical", "major", "minor", "trivial"};
		for(int i = 0;i < 5;i++)
		{
			cout << "real " << tempStr[i] << "is:" << sevCount[2][i] << endl;
			cout << "right predict:" << tempStr[i] << "is:" << rightPre[k][0][i] << endl;
			cout << "we count:" << tempStr[i] << "is:" << preNum[k][0][i] << endl;

			outFile << "real:" << tempStr[i] << "is:" << sevCount[2][i] << endl;
			outFile << "right predict:" << tempStr[i] << "is:" << rightPre[k][0][i] << endl;
			outFile << "we count:" << tempStr[i] << "is:" << preNum[k][0][i] << endl;

			precision[0][i] = rightPre[k][0][i] == 0 ? 0 : ((double)rightPre[k][0][i] / sevCount[2][i]);
			recall[0][i] = rightPre[k][0][i] == 0 ? 0 : ((double)rightPre[k][0][i] / preNum[k][0][i]);
			F[0][i] = (precision[0][i] + recall[0][i]) == 0 ? 0 :
				(2.0 * (precision[0][i] * recall[0][i]) / (precision[0][i] + recall[0][i]));

			precision_total[k][0][i] += precision[0][i];
			recall_total[k][0][i] += recall[0][i];
			//F_total[k][0][i] += F[0][i];
		}

		for(int i = 0;i < 5;i++)
		{
			cout << "------------------------------------------" << endl;
			cout << "the number of "  << tempStr[i] << " is " << sevCount[2][i] << endl;

			cout << "precision" << tempStr[i] << " is " << precision[0][i]*100 << "%" << endl;
			cout << "recall" << tempStr[i] << " is " << recall[0][i]*100 << "%" << endl;
			cout << "F-(our)" << tempStr[i] << " is " << F[0][i]*100 << "%" << endl;

			outFile << "------------------------------------------" << endl;
			outFile << "the number of "  << tempStr[i] << " is " << sevCount[2][i] << endl;

			outFile << "precision-(our)" << tempStr[i] << " is " << precision[0][i]*100 << "%" << endl;
			outFile << "recall-(our)" << tempStr[i] << " is " << recall[0][i]*100 << "%" << endl;
			outFile << "F-(our)" << tempStr[i] << " is " << F[0][i]*100 << "%" << endl;
		}
	}
	
}

void init(int fileNum)
{
	trainBR.clear();
	testBR.clear();
	word_inBR_unigram.clear();
	word_inBR_bigram.clear();
	getDocID.clear();
	lenSum_unigram = 0;
	lenSum_bigram = 0;
	lenDesc_unigram = 0;
	lenDesc_bigram = 0;
	memset(sevCount, 0 ,sizeof(sevCount));
	memset(preNum, 0 ,sizeof(preNum));
	memset(rightPre, 0 ,sizeof(rightPre));
	printf("the %d group\n", fileNum);
	outFile << endl << "-----------------------the " << fileNum << " group" << "-----------------------" << endl;
}

void printResult(int FileSize)
{
	for(int i = 0;i < 5;i++)
		outFile << "alpha1_" << i << " = " << alpha1[i] << " " ;
	outFile << endl;
	for(int i = 0;i < 5;i++)
		outFile << "alpha1_" << i << " = " << alpha2[i] << " " ;
	outFile << endl;
	cout << "--------------------final result----------------------" << endl;
	outFile << "--------------------final result----------------------" << endl;
	string tempStr[5] = {"blocker","critical", "major", "minor", "trivial"};
	int topN[5] = {5,10,15,20,25};; 
	for(int k = 0;k < 5;k++)
	{
		outFile << "-------------k = " << topN[k] << "-------------------" << endl;
		for(int i = 0;i < 5;i++)
		{
			F_total[k][0][i] = (precision_total[k][0][i]+ recall_total[k][0][i]) == 0 ? 0 :
				(2.0 * (precision_total[k][0][i]*recall_total[k][0][i]) / (precision_total[k][0][i]+ recall_total[k][0][i]));

			F_total[k][1][i] = (precision_total[k][1][i]+ recall_total[k][1][i]) == 0 ? 0 :
				(2.0 * (precision_total[k][1][i]*recall_total[k][1][i]) / (precision_total[k][1][i]+ recall_total[k][1][i]));
			
			cout << "precision" << tempStr[i] << " is " << precision_total[k][0][i] / FileSize *100 << "%" << endl;
			cout << "recall" << tempStr[i] << " is " << recall_total[k][0][i] / FileSize *100 << "%" << endl;
			cout << "F" << tempStr[i] << " is " << F_total[k][0][i] / FileSize *100 << "%" << endl <<endl;

			outFile << "precision" << tempStr[i] << " is " << precision_total[k][0][i] / FileSize *100 << "%" << endl;
			outFile << "recall" << tempStr[i] << " is " << recall_total[k][0][i] / FileSize *100 << "%" << endl;
			outFile << "F" << tempStr[i] << " is " << F_total[k][0][i] / FileSize * 100 << "%" << endl << endl;
		}
	}	
}
int main()
{
	string resultPath = "sample_result.txt";
	string theme = "sample_file";
	sevScore["blocker"] = 0;
	sevScore["critical"] = 1;
	sevScore["major"] = 2;
	sevScore["minor"] = 3;
	sevScore["trivial"] = 4;
	start_time = clock();
	outFile.open(resultPath);
	int FileSize = 10;
	for(int fileNum = 1;fileNum <= FileSize;fileNum++)
	{
		init(fileNum);	
		trainBR = LoadInfo(fileNum,1,theme);
		testBR = LoadInfo(fileNum, 2, theme);
		for(int i = 0;i < testBR.size();i++)
		{
			if(i % 10 == 0)
				printf("Proceed：%.2f%%\n",((double)i/testBR.size()*100));
			getSev_our(getBM25FextAndREP(testBR[i], i), i);
		}
		getResult();
	}
	
	printResult(FileSize);
	
 	clock_t end_time = clock();
	cout << "Running time is: " << static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC<<"s"<<endl;
	outFile << "Running time is: " << static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}