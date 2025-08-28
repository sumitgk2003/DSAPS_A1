#include<iostream>
#include<string.h>
#include<vector>
using namespace std;

class Node{
    public:
        vector<Node*>alphabets;
        bool end=false;
    Node():alphabets(26,NULL){}
};

class Trie{
    private:
        Node *root;
    public:
        Trie(){
            root=new Node();
        }
        
        void insertWord(string s){
            Node* temp=root;
            for(int i=0;i<s.size();i++){
                if(temp->alphabets[s[i]-'a']==NULL){
                    Node* temp1=new Node();
                    temp->alphabets[s[i]-'a']=temp1;
                    temp=temp1;
                }else{
                    temp=temp->alphabets[s[i]-'a'];
                }
            }
            temp->end=true;
        }
        void spellCheck(string s){
            Node* temp=root;
            for(int i=0;i<s.size();i++){
                if(temp->alphabets[s[i]-'a']==NULL){
                    cout<<0<<endl;
                    return;
                }else{
                    temp=temp->alphabets[s[i]-'a'];
                }
            }
            if(temp->end==true){
                cout<<1<<endl;
            }else{
                cout<<0<<endl;
            }
        }

        void dfs(Node* temp,string s,vector<string>&v){
            bool flag=true;
            if(temp->end==true){
                v.push_back(s);
            }
            for(int i=0;i<26;i++){
                if(temp->alphabets[i]!=NULL){
                    flag=false;
                    break;
                }
            }
            if(flag==true){
                return;
            }

            for(int i=0;i<26;i++){
                if(temp->alphabets[i]!=NULL){
                    char a='a'+i;
                    dfs(temp->alphabets[i],s+a,v);
                }
            }

        }

        void dfsAutoCorrect(Node* temp,string s,string ans,vector<string>&v,vector<int>dp){
            int min=4;
            for(int i=0;i<dp.size();i++){
               if(dp[i]<min)min=dp[i]; 
            }
            if(min>3){
                return;
            }
            if(temp->end==true){
                if(dp[dp.size()-1]<=3)v.push_back(ans);
            }
            bool flag=true;
            for(int i=0;i<26;i++){
                if(temp->alphabets[i]!=NULL){
                    flag=false;
                    break;
                }
            }
            if(flag==true){
                return;
            }
            for(int i=0;i<26;i++){
                if(temp->alphabets[i]!=NULL){
                    vector<int>dp1;
                    dp1.push_back(ans.size());
                    for(int k=1;k<dp.size();k++){
                        if(k!=0 && 'a'+i==s[k-1]){
                            dp1.push_back(dp[k-1]);
                        }else{
                            int mini=1000;
                            if(mini>dp[k])mini=dp[k];
                            if(mini>dp[k-1])mini=dp[k-1];
                            if(mini>dp1[k-1])mini=dp1[k-1];
                            dp1.push_back(mini+1);
                        }
                    }
                    // for(auto i:dp1){
                    //     cout<<i<<" ";
                    // }cout<<endl;
                    char a='a'+i;
                    dfsAutoCorrect(temp->alphabets[i],s,ans+a,v,dp1);
                }
            }
        }

        void autoComplete(string s){
            Node* temp=root;
            for(int i=0;i<s.size();i++){
                if(temp->alphabets[s[i]-'a']==NULL){
                    cout<<"Word does not exist"<<endl;
                    return;
                }else{
                    temp=temp->alphabets[s[i]-'a'];
                }
            }
            vector<string>v;
            dfs(temp,s,v);
            cout<<v.size()<<endl;
            for(auto i:v){
                cout<<i<<endl;
            }
        }
        void autoCorrect(string s){
            Node *temp=root;
            vector<int>dp;
            for(int i=0;i<=s.size();i++){
                dp.push_back(i);
            }
            // for(auto i:dp){
            //     cout<<i<<" ";
            // }cout<<endl;
            vector<string>v;
            dfsAutoCorrect(temp,s,"",v,dp);
            cout<<v.size()<<endl;
            for(auto i:v){
                cout<<i<<endl;
            }
        }
};


int main(){
    int n,q;
    cin>>n>>q;
    Trie t;
    for(int i=0;i<n;i++){
        string s;
        cin>>s;
        t.insertWord(s);
    }
    for(int i=0;i<q;i++){
        int type;
        string s;
        cin>>type>>s;
        if(type==1){
            t.spellCheck(s);
        }else if(type==2){
            t.autoComplete(s);
        }else if(type==3){
            t.autoCorrect(s);
        }else{
            cout<<"Invalid type.";
        }
    }

}