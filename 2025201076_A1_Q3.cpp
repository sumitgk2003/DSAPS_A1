#include<iostream>
#include<map>
#include<vector>
using namespace std;

map<long long,long long>m;

void update(int l,int r,int h){
    if (l>=r)return;
    long long lh;
    auto it1=m.upper_bound(l);
    if(it1==m.begin()){
        lh=0;
    }else{
        --it1;
        lh=it1->second;
    }
    long long rh;
    auto it2=m.upper_bound(r);
    if (it2==m.begin()){
        rh=0;
    }else{
        --it2;
        rh=it2->second;
    }
    m[l]=lh;
    m[r]=rh;
    auto it=m.upper_bound(l);
    while (it!=m.end()&&it->first<r) {
        it=m.erase(it);
    }
    m[l]=h;
}

void print(){
    long long prev=-1;
    for (auto i:m) {
        if (i.second!=prev) {
            cout<<i.first<<" "<<i.second<<endl;
            prev=i.second;
        }
    }
}

int main(){
    int q;
    cin>>q;
    vector<vector<int>>v;
    for(int i=0;i<q;i++){
        int type;
        cin>>type;
        if(type==0){
            vector<int>v1;
            v1.push_back(0);
            int l,r,h;
            cin>>l>>r>>h;
            v1.push_back(l);
            v1.push_back(r);
            v1.push_back(h);
            v.push_back(v1);
            //update(l,r,h);
        }else if(type==1){
            vector<int>v2;
            v2.push_back(1);
            v.push_back(v2);
            //print();
        }else{
            cout<<"invalid type";
        }
        
    }
    for(int i=0;i<q;i++){
        if(v[i][0]==0){
            update(v[i][1],v[i][2],v[i][3]);
        }else{
            print();
        }
    }
}
