#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <map>
#include "Ngram.h"
using namespace std;

void input_td(const char *, vector<string> &);
void input_mp(const char *, map< string, vector<string> > &);
double getBigramProb(const char*, const char*, Vocab &, Ngram &);
vector<string> viterbi(string &, map< string, vector<string> > &, Vocab &, Ngram & );

int main(int argc, char * argv[])
{
	int ngram_order = 2;
    Vocab voc;
    Ngram lm( voc, ngram_order );

    vector<string> testdata;
    map<string, vector<string> > map;
    
    File lmFile( argv[6], "r" );
    lm.read( lmFile );
    lmFile.close();
    
    input_td( argv[2] , testdata);
    input_mp( argv[4] , map);

    for(int i=0;i<testdata.size();i++)
    {
    	cout<<"<s> ";
    	vector<string> answer=viterbi(testdata[i],map,voc,lm);

    	for(int j=0; j<answer.size();j++)
    	{
    		if (j<answer.size()-1)
                cout << answer[j] << " ";
            else 
                cout << answer[j] << endl;
    	}

    }
}

void input_td(const char * filename, vector<string> & td)    //input testdata
{
	fstream fp;
	string ip_line;
	fp.open(filename, ios::in|ios::binary);
	while(getline(fp,ip_line))
	{
		string line;
		for(int i=0;i<ip_line.length();i++)
		{
			if(ip_line[i]!=' ')
				line=line+ip_line[i];
		}
		td.push_back(line);
	}
	fp.close();
}

void input_mp(const char * filename, map<string,vector<string> > & mapping)  //input mapping
{
	fstream fp;
	string ip_line;
	fp.open(filename, ios::in|ios::binary);
	while(getline(fp,ip_line))
	{
		string charac=ip_line.substr(0,2);
		string line;
		for(int i=3;i<ip_line.size();i++)
		{
			if(ip_line[i]!=' ')
				line=line+ip_line[i];
		}
		for(int j=0;j<(line.size()-1);j=j+2)
			mapping[charac].push_back(line.substr(j,2));
    
    string end_tag="</s>";              
    mapping[end_tag].push_back(end_tag);
	}
}

// Get P(W2 | W1) -- bigram
double getBigramProb(const char *w1, const char *w2, Vocab & voc, Ngram & lm)
{
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return lm.wordProb( wid2, context);
}

vector<string> viterbi(string & input, map<string, vector<string> > & mapping, Vocab & voc, Ngram & lm)
{
	int size1=input.size()/2+1;
	vector<vector<string> > chars(size1);
    vector<vector<double> >log_prob(size1);
    vector<vector<int> > prev_index(size1);

    for(int i=0;i<size1;i++)
    {
    	string charac;
    	if(i==size1-1)
    		charac="</s>";
    	else
    		charac=input.substr(i*2,2);

    	int size2=mapping[charac].size();

        chars[i].resize(size2);
        log_prob[i].resize(size2);
        prev_index[i].resize(size2);

        vector<string>::iterator iter=mapping[charac].begin();
        for(int j=0;j<chars[i].size();j++)
        {
        	chars[i][j]= *iter;
        	++iter;
        }
    }

    //initialization
    for(int i=0;i<chars[0].size();i++)
    {
    	prev_index[0][i] = 0;
    	log_prob[0][i] = getBigramProb("<s>", chars[0][i].c_str(), voc, lm);
    }
    //recursion
    for(int i=1;i<log_prob.size();i++)
    {
    	for(int j=0;j<log_prob[i].size();j++)
    	{
    		log_prob[i][j]=-1e9;
    		for(int q=0;q<log_prob[i-1].size();q++)
    		{
    			double tmp_prob;
    			tmp_prob=getBigramProb(chars[i-1][q].c_str(), chars[i][j].c_str(), voc, lm);
    			tmp_prob=tmp_prob+log_prob[i-1][q];
                if(log_prob[i][j]<tmp_prob)
                {
                	log_prob[i][j]=tmp_prob;
                	prev_index[i][j]=q;
                }
    		}
    	}

    }

    //backtracking
    double max_p=-1e9;
    int max_index;
    for(int j=0;j<log_prob.back().size();j++)
    {
    	if(max_p<log_prob.back()[j])
    	{
    		max_p=log_prob.back()[j];
    		max_index=j;
    	}
    }

    vector<string> out(size1);
    for(int i=size1-1;i>=0;i--)
    {
    	out[i]=chars[i][max_index];
    	max_index=prev_index[i][max_index];  
    }

    return out;
}