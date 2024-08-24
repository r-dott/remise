#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cmath>
#include <unordered_map>
#include <map>
#include <random>
#include <queue>
#include <bitset>
#include <chrono>
#include <vector>
#include <fstream>

#include <random> // For random number generation

using namespace std;


string indexString(int len, int val) {
    // Convert val to its binary representation with at least len bits
    string binary = bitset<32>(val).to_string(); // Assuming int is 32 bits
    binary = binary.substr(binary.length() - len); // Take the least significant len bits
    
    return binary;
}

int DFAeval(map<int,pair<int,int>>&DFAmatrix, string index){
    pair<int,int> curr_state = {2,0};
    for(auto c: index){
        int bit = (c=='1')? 1: 0;
        curr_state = DFAmatrix[2*(curr_state.first) + bit];
    }
    return curr_state.second;
}


int MAG(const string& binary){
    for( int i=0; i<binary.size(); i++){
        if(binary[i]=='1'){
            return 1;
        }
    }
    return 0;
}

pair<int,int> State(unordered_map<string, pair<int,int>>&stateMap, string &key, int& globalState){

    //zero strings
    if(MAG(key)==0){
        return stateMap["0"];
    }
    
    //old non zero strings
    auto it = stateMap.find(key);
    if(it!=stateMap.end()){
        return it->second;
    }
    else{
        stateMap[key] = {globalState++, 0};
        return stateMap[key];
    }
}


map<int,pair<int,int>> DFAmake(int N, string &AccessList){

    map<int,pair<int,int>> DFAmatrix;

    int rejectState = 0;
    int acceptState = 1;
    int startState = 2;
    int globalState = startState;

    unordered_map<string, pair<int,int>> stateMap;
    stateMap["0"] = {rejectState, 0};
    stateMap["1"] = {acceptState, 1};

    DFAmatrix[2*rejectState + 0] = {rejectState, 0};
    DFAmatrix[2*rejectState + 1] = {rejectState, 0};
    DFAmatrix[2*acceptState + 0] = {rejectState, 0};
    DFAmatrix[2*acceptState + 1] = {rejectState, 0};

    State(stateMap, AccessList, globalState);


    if(MAG(AccessList)==0){
        DFAmatrix[2*startState + 0] = {rejectState, 0};
        DFAmatrix[2*startState + 1] = {rejectState, 0};
        return DFAmatrix;
    }

    set<string> Q;
    Q.insert(AccessList);

    // cout << "starting loop" << endl;

    while(!Q.empty()){

        string p = *(Q.begin());
        Q.erase(p);

        // cout << "parent: " << p << endl;

        int mid = p.size()/2;

        string p0 = p.substr(0,mid);
        string p1 = p.substr(mid,mid);

        pair<int, int> parent_state = (State(stateMap, p, globalState));

        pair<int,int> left_child = State(stateMap, p0, globalState);
        pair<int,int> right_child = State(stateMap, p1, globalState);

        DFAmatrix[2*(parent_state.first)+0] = left_child;
        DFAmatrix[2*(parent_state.first)+1] = right_child;
        
        if(MAG(p0)!=0){Q.insert(p0);}
        if(MAG(p1)!=0){Q.insert(p1);}
        
    }   

    return DFAmatrix;
}


void Experiments(vector<int> DBSizes, vector<int> PageSizes, int tests){

    int ilim = DBSizes.size();
    int jlim = PageSizes.size();

    ofstream file("random_construct.csv");

    for(int i=0; i<ilim; i++){
        for(int j=0; j< jlim; j++){

            int N = DBSizes[i];
            int P = PageSizes[j];

            int lgN = log(N)/log(2);

            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            mt19937 gen(seed);  
            uniform_int_distribution<>dis(1, N); 

            double time[10];
            int LUTsize[10];

            file << "DBSize:" << N << "," << "PageSize:" << P << endl;
        
            for(int t=0; t<tests; t++){

                set<int> AcceptIndices = {};
                string AccessList(N, '0');

                while(AcceptIndices.size() < P){AcceptIndices.insert(dis(gen));}

                for(int i: AcceptIndices){
                    AccessList[i] = '1';
                }

                
                
                map<int,pair<int,int>> DFAmatrix = DFAmake(N, AccessList);

                file << DFAmatrix.size() << ",";               
            }


            file << endl;

        }
    }
}

int main(int argc, char* argv[]){

    int N1 = atoi(argv[1]);
    int N2  = atoi(argv[2]);

    int P1 = atoi(argv[3]);
    int P2 = atoi(argv[4]);

    vector<int> DBsizes;
    int n = N1;
    while(n<=N2){
        DBsizes.push_back(int(pow(2,n)));
        n = n+1;
    }

    vector<int> Psizes;
    int p = P1;
    while(p<=P2){
        Psizes.push_back(int(pow(2,p)));
        p = p+1;
    }

    int t = atoi(argv[5]);          //num of test

    Experiments(DBsizes, Psizes, t);

    return 0;
}

