#include "xor.h"
#include "hashutil.h"
#include <set>
#include <queue>
#include <stack>
#include <utility>
#include <iterator>
#include <vector>
#include <cmath>

class xor
{
private:

    std::set<int> key_set;
    unsigned int arrayB;

    hashing::SimpleMixSplit hash_0;
    hashing::SimpleMixSplit hash_1;
    hashing::SimpleMixSplit hash_2;

    uint64_t h1;
    uint64_t h2;
    uint64_t h3;

    std::queue<int> queue;
    std::stack<std::pair<int,int>> sigma;
    std::vector<uint64_t> B;

    bool map();
    void assign();
    uint64_t fingerprint(uint64_t key);
public:
    xor(std::set<int> keys);
};


xor::xor(std::set<int> keys)
{   
    key_set = keys;
    while(true){
        hash_0 = *new hashing::SimpleMixSplit();
        hash_1 = *new hashing::SimpleMixSplit();
        hash_2 = *new hashing::SimpleMixSplit();
        if(map()){
            int size = floor(1.23 * sizeof(key_set)) + 32;
            std::vector<uint64_t> temp(size);
            B = temp;
            return;
        }
    }
    

    
}

void xor::assign(){
    while(!sigma.empty()){
        std::pair<int,int> valPair = sigma.top();
        sigma.pop();
        B[valPair.second] = 0;
        B[valPair.second] = fingerprint(valPair.first) ^ B[hash_0.murmur64(valPair.first)] ^ B[hash_0.murmur64(valPair.first)] ^ B[hash_1.murmur64(valPair.first)] ^ B[hash_2.murmur64(valPair.first)];

    }
    
}

uint64_t xor::fingerprint(uint64_t key){
     return (uint64_t) key ^ (key >> 32);
}

bool xor::map(){
    int c = floor(1.23 * sizeof(key_set)) + 32;

    //Array of sets, just use vector
    std::vector<std::set<int>> H(c);

    //Generate 3 murmur hashes here

    for (auto x: key_set)
    {
        H[hash_0.murmur64(x)].insert(x);
        H[hash_1.murmur64(x)].insert(x);
        H[hash_2.murmur64(x)].insert(x);
    }

    std::queue<int> queue;
    for(int i = 0; i < c; i++){
        if (H[i].size() == 1){
           queue.push(i); 
        }
    }

    std::stack<std::pair<int,int>> sigma;
    while (!queue.empty())
    {
        int temp = queue.front();
        queue.pop();

        if (H[temp].size() == 1){
            //temp is index
            //x is the value in the set at H[temp]
            int x = *H[temp].begin();
            sigma.push(std::make_pair(x, temp));
            std::set<int>::iterator iter = H[hash_0.murmur64(x)].find(x);

            H[hash_0.murmur64(x)].erase(iter);
            if(H[hash_0.murmur64(x)].size() == 1){
                queue.push(hash_0.murmur64(x));
            }

            iter = H[hash_1.murmur64(x)].find(x);
            H[hash_1.murmur64(x)].erase(iter);
            if(H[hash_1.murmur64(x)].size() == 1){
                queue.push(hash_1.murmur64(x));
            }

            iter = H[hash_2.murmur64(x)].find(x);
            H[hash_2.murmur64(x)].erase(iter);
            if(H[hash_2.murmur64(x)].size() == 1){
                queue.push(hash_2.murmur64(x));
            }
            
        }
        return sizeof(key_set)/sizeof(key_set.begin()) == sigma.size();
    }

    
}