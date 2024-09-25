#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <forward_list>
template <typename K,typename V>
class HashMap {
public:
	struct Entry {
			K key;
			V val;
			Entry(K k, V v) : key(k), val(v){};

	};
	typedef std::vector<std::forward_list<Entry>> buckets_t;
	buckets_t buckets;


	HashMap(size_t cap) : buckets(cap) {}


	V* get(const K& key){
		size_t k = std::hash<K>()(key);
		size_t index = k%buckets.size();
		for(Entry &e : buckets[index]){
			if(e.key == key) return &e.val;
		}
		return nullptr;
	}

	bool put(const K & key,const V & value){
		size_t h = std::hash<K>()(key);
		size_t index = h%buckets.size();
		if(get(key)!=nullptr){
			for(Entry & e : buckets[index]) {
				if(e.key==key){
					e.val=value;
					return true;
				}
			}
		}
		buckets[index].push_front(Entry(key,value));
		return false;
	}

	size_t size() const{
		size_t count = 0;
		for(std::forward_list<Entry> l : buckets){
			for(Entry e : l){
				++count;
			}
		}
		return count;
	}

	buckets_t & realloc(const buckets_t & b ){

		    size_t new_capacity = b.size() * 2;
		    buckets_t res(new_capacity);

		    for (std::forward_list<Entry> &l : b) {
		        for (Entry &e : l) {
		            size_t h = std::hash<K>()(e.key);
		            size_t index = h % new_capacity;
		            res->at(index).push_front(e);
		        }
		    }

		    return res;
	}
	void grow(){
		if(size()>0.8*buckets.cap){
			buckets = realloc(buckets);
		}
	}



};
