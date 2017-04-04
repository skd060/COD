#include<bits/stdc++.h>
using namespace std;
map<int,string>symbols;

bool check(char item,vector<char>terminal)
{
    bool found = false;
    for(int i=0;i<terminal.size();i++)
    {
        char it = terminal[i];
        if(item==it)
        {
            found = true;
            return found;
        }
    }
    return found;
}
void symbolsmodify(vector<string>nonTerminals)
{
    for(int i=0;i<nonTerminals.size();i++)
    {
        for(int j=0;j<symbols.size();j++)
        {
            if(nonTerminals[i]==symbols[j])
            {
                symbols.erase(j);
                break;
            }
        }
    }
}
string get()
{
    for(int i=0;i<symbols.size();i++)
    {
        if(symbols[i]!="")
        {
            string temp = symbols[i];
            symbols.erase(i);
            return temp;
        }
    }
}
void insertTerminal(vector<char>&terminal,vector<string>nonTerminals,map<string,vector<string> >productionRules)
{
    for(int i=0;i<nonTerminals.size();i++)
    {
        vector<string>vec = productionRules[nonTerminals[i]];
        for(int k=0;k<vec.size();k++)
        {
            string leftRule = vec[k];
            for(int j=0;j<leftRule.size();j++)
            {
                if(!isupper(leftRule[j])&&(!check(leftRule[j],terminal)))
                    terminal.push_back(leftRule[j]);
            }
        }
    }
}
void insertMap(map<string,vector<string> >&productionRules,vector<string>&nonTerminals,string rule)
{
    int pos;
    for(int i=0;i<rule.length();i++)
    {
        if(rule.at(i)=='>')
        {
            pos = i;
            break;
        }

    }
    string temp(1,rule[0]);
    vector<string> vec = productionRules[temp];
    vec.push_back(rule.substr(pos+1,rule.length()-1));
    productionRules[temp] = vec;
    bool found = false;
    for(int i=0;i<nonTerminals.size();i++)
    {
        if(nonTerminals[i]==temp)
        {
            found=true;
            break;
        }
    }
    if(found == false)
        nonTerminals.push_back(temp);

}
void leftRecursion(map<string,vector<string> >&productionRules,vector<string>&nonTerminals)
{
    for(int i=0;i<nonTerminals.size();i++)
    {
        vector<string>vecRules = productionRules[nonTerminals[i]];
        for(int j=0;j<vecRules.size();j++)
        {
            string rule = vecRules[j];
            string temp(1,rule[0]);
            if(temp == nonTerminals[i])
            {
                
                vector<string>beta = vecRules;
                beta.erase(beta.begin()+j);
                vector<string>newRules;
                string s = get();
                nonTerminals.push_back(s);
                if(beta.size()==0)
                {
                    newRules.push_back(s);
                }
                else
                {
                    for(int k=0;k<beta.size();k++)
                    {
                        newRules.push_back(beta[k]+s);
                    }
                }
                vector<string>newtempRules;
                newtempRules.push_back(rule.substr(1,rule.length())+s);
                newtempRules.push_back("@");
                productionRules[nonTerminals[i]] = newRules;
                productionRules[s] = newtempRules;
            }
        }
    }
}
void leftFectoring(map<string,vector<string> >&productionRules,vector<string>&nonTerminals)
{
    for(int i=0;i<nonTerminals.size();i++)
    {
        vector<string>vecRules = productionRules[nonTerminals[i]];
        map<string,vector<string> >index;
        string result = "";
        for(int j=0;j<vecRules.size()-1;j++)
        {
            vector<string>index1;
            string rule1 = vecRules[j];
            bool cmn = true;
            for(int k=j+1;k<vecRules.size();k++)
            {
                string rule = vecRules[k];
                int n1 = rule.length();
                int n2 = rule1.length();
                cmn = true;
                if(result=="")
                {
                    for (int m=0, l=0; m<=n1-1&&l<=n2-1; m++,l++)
                    {
                        if (rule[m] != rule1[l])
                            break;
                        result.push_back(rule[m]);
                    }
                    if(result=="")
                        cmn = false;
                }
                else
                {
                    rule1 = result;
                    n2 = rule1.length();
                    for (int m=0, l=0; m<=n1-1&&l<=n2-1; m++,l++)
                    {
                        if (rule[m] != rule1[l])
                        {
                            cmn = false;
                            break;
                        }
                    }
                }
                if(cmn == true && result!="")
                {
                    if(find(index1.begin(), index1.end(), vecRules[j]) == index1.end())
                        index1.push_back(vecRules[j]);
                    index1.push_back(vecRules[k]);
                }
            }
            index[nonTerminals[i]] = index1;
            if(result!="")
                break;
        }
        if(result!="")
        {
            string s = get();
            nonTerminals.push_back(s);
            vector<string> temp;
            temp.push_back(result+s);
            productionRules[nonTerminals[i]] = temp;
            temp.clear();
            temp = productionRules[nonTerminals[i]];
            vector<string>temp1 = index[nonTerminals[i]];
            for(int k=0;k<vecRules.size();k++)
            {
                string str = vecRules[k];
                if(find(temp1.begin(), temp1.end(), str) == temp1.end())
                {
                    temp.push_back(str);
                }
            }
            productionRules[nonTerminals[i]] = temp;
            vector<string>newrule1;
            if(temp1.size()!=0)
            {
                for(int k=0;k<temp1.size();k++)
                {
                    if(temp1[k].substr(result.length(),temp1[k].length()).length()==0)
                    {
                        newrule1.push_back("@");
                        //cout<<"epsilon"<<endl;
                    }
                    else
                        newrule1.push_back(temp1[k].substr(result.length(),temp1[k].length()));
                }
            }
            productionRules[s] = newrule1;

        }

    }
}
void indirectLeftRecursion(map<string,vector<string> >&productionRules,vector<string>&nonTerminals)
{
    vector<string>tempnterm = nonTerminals;
    for(int i=0;i<tempnterm.size();i++)
    {
        vector<string>temp = productionRules[tempnterm[i]];
        vector<string>copytemp = temp;
        for(int j=0;j<=i-1;j++)
        {
            string item = tempnterm[j];
            for(int k=0;k<temp.size();k++)
            {
                string t = temp[k];
                string temp1(1,t[0]);
                if(temp1==item)
                {
                    vector<string>torepRules=productionRules[temp1];
                    for(int m=0;m<torepRules.size();m++)
                    {
                        copytemp.push_back(torepRules[m]+t.substr(1,t.length()));
                    }
                    copytemp.erase(copytemp.begin()+k);
                    productionRules[tempnterm[i]] = copytemp;
                    leftRecursion(productionRules,nonTerminals);
                }
            }
        }
    }
}
/*string first(string item,map<string,vector<string> >&productionRules,vector<string>&nonTerminals,vector<char>Terminals)
{
    vector<string>vec = productionRules[item];
    vector<string>f;
    for(int i=0;i<vec.size();i++)
    {
        string rule = vec[i];
        string ttt(1,rule[0]);
        string tttt(1,rule[1]);
        int ind=1;
        if(tttt=="'")
        {
            ttt = rule.substr(0,2);
            ind = 2;
        }
        if(find(nonTerminals.begin(), nonTerminals.end(), ttt) == nonTerminals.end())
        {
            string tt(1,rule[0]);
            //f.push_back(tt);
            return tt;
        }
        else
        {
            //string t(1,rule[0]);
            if(ttt=="0")
            {
                string tt(1,rule[ind]);
                first(tt,productionRules,nonTerminals,Terminals);
            }
            else
                first(ttt,productionRules,nonTerminals,Terminals);
            //vector<string>temp = productionRules[t];
            /*for(int j=0;j<temp.size();j++)
            {
                rule = temp[j];
                string tt(1,rule[0]);
                if(tt=="0")
                    tt = rule[1];
                first(tt,productionRules,nonTerminals,Terminals);
            }
        }
    }
    //return f;
}*/

void display(map<string,vector<string> >&productionRules,vector<string>&nonTerminals,vector<char>Terminals)
{
    ofstream ofs ("tab5.txt", std::ofstream::out);
    cout<<"Terminals:\n";
    for(int i=0;i<Terminals.size();i++)
        cout<<Terminals[i]<<"\t";
    cout<<endl;
    cout<<"NonTerminals:\n";
    for(int i=0;i<nonTerminals.size();i++)
        cout<<nonTerminals[i]<<"\t";
    cout<<endl;
    cout<<"Production Rules:\n";
    for(int i=0;i<nonTerminals.size();i++)
    {
        vector<string>vec = productionRules[nonTerminals[i]];
        for(int j=0;j<vec.size();j++)
        {
            cout<<nonTerminals[i]<<" -> "<<vec[j]<<endl;
            ofs<<nonTerminals[i]<<" "<<vec[j]<<endl;
        }
    }
}
int main()
{
    symbols[1]="A";symbols[2]="B";symbols[3]="C";symbols[4]="D";symbols[5]="E";symbols[6]="F";symbols[7]="G";symbols[8]="H";symbols[9]="I";symbols[10]="J";
    symbols[11]="K";symbols[12]="L";symbols[13]="M";symbols[14]="N";symbols[15]="O";symbols[16]="P";symbols[17]="Q";symbols[18]="R";symbols[19]="S";
    symbols[20]="T";symbols[21]="U";symbols[22]="V";symbols[23]="W";symbols[24]="X";symbols[25]="Y";symbols[26]="Z";
    int n;
    cout<<"How many production rules you want to insert\n";
    cin>>n;
    map<string,vector<string> >productionRules;
    vector<string>nonTerminals;
    vector<char>Terminals;
    string rule;
    while(n--)
    {
        cin>>rule;
        //getline(cin,rule);
        //cout<<rule<<endl;
        insertMap(productionRules,nonTerminals,rule);
        insertTerminal(Terminals,nonTerminals,productionRules);
    }
    symbolsmodify(nonTerminals);
    cout<<"Before Removing left factoring and recursion:\n";
    display(productionRules,nonTerminals,Terminals);
    leftRecursion(productionRules,nonTerminals);
    indirectLeftRecursion(productionRules,nonTerminals);
    leftFectoring(productionRules,nonTerminals);
    cout<<"After Removing left factoring and recursion:\n";
    display(productionRules,nonTerminals,Terminals);
    /*for(int i=0;i<1;i++)
    {
        string item = nonTerminals[i];
        string f = first(item,productionRules,nonTerminals,Terminals);
        cout<<f;
    }*/
    return 0;
}
