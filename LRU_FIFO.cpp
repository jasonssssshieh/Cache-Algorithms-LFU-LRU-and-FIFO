//
//  main.cpp
//  Cache Algorithm: LRU + FIFO
//
//  Created by Zijian Xie on 2016/12/3.
//  Copyright © 2016年 Jason Xie. All rights reserved.
//
/*
 Design and implement a data structure for Least Recently Used (LRU) cache. It should support the following operations: get and set.
 
 get(key) - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
 set(key, value) - Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the least recently used item before inserting a new item.
 */

#include <iostream>
#include <unordered_map>
#include <set>
#include <list>
using namespace std;

class Cache{
public:
    unordered_map<int, list<pair<int,int>>::iterator> mp;
    list<pair<int,int>> data;
    int size;
    int n = 0;
    virtual int get(int key) = 0;
    virtual void set(int key, int value) = 0;
    virtual ~Cache(){};
};

class FIFO : public Cache{
    //如果一个数据最先进入缓存中，则应该最早淘汰掉。
public:
    FIFO(int capacity){size = capacity;}
    int get(int key){
        if(mp.find(key) == mp.end()) return -1;
        auto itr = mp[key];
        data.splice(data.begin(), data, itr);
        return itr->second;
    }
    void set(int key, int value){
        if(size <= 0) return;
        if(mp.find(key) != mp.end()){
            auto itr = mp[key];
            itr->second = value;
            data.splice(data.end(), data, itr);
            return;
        }else if(n == size){
            int k = data.front().second;
            mp.erase(k);
            data.pop_front();
            n--;
        }
        n++;
        data.emplace_back(key, value);
        mp[key] = --data.end();
    }
};

class LRU : public Cache{
public:
    LRU(int capacity){size = capacity;}
    int get(int key){
        if(mp.find(key) == mp.end()) return -1;
        auto itr = mp[key];
        data.splice(data.begin(), data, itr);
        return itr->second;
    }
    
    void set(int key, int value){
        if(size <= 0) return;
        if(mp.find(key)!=mp.end()){
            auto itr = mp[key];
            itr->second = value;
            data.splice(data.begin(), data, itr);
            return;
        }else if(n == size){
            auto k = data.back().second;
            mp.erase(k);
            data.pop_back();
            n--;
        }
        data.emplace_front(key,value);
        n++;
        mp[key] = data.begin();
    }
};
