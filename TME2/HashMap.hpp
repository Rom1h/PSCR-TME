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
	typedef std::forward_list<Entry> list_t;
	typedef std::vector<std::forward_list<Entry>> buckets_t;
	buckets_t buckets;
	class iterator {
	    buckets_t& buckets; // référence aux buckets
	    typename buckets_t::iterator vit; // itérateur pour les buckets
	    typename list_t::iterator lit; // itérateur pour la liste chaînée

	public:
	    iterator(buckets_t& b, typename buckets_t::iterator vi, typename list_t::iterator li)
	        : buckets(b), vit(vi), lit(li) {}

	    // Opérateur de pré-incrémentation
	    iterator& operator++() {
	        ++lit; // avancer dans la forward_list
	        if (lit == vit->end()) { // si on est à la fin de la liste actuelle
	            for (++vit; vit != buckets.end(); ++vit) { // avancer dans les buckets
	                if (!vit->empty()) { // trouver un bucket non vide
	                    lit = vit->begin();
	                    break;
	                }
	            }
	        }
	        return *this;
	    }

	    Entry& operator*() {
	        return *lit; // déférencer l'entrée actuelle
	    }

	    bool operator!=(const iterator& other) const {
	        return vit != other.vit || lit != other.lit; // comparaison des itérateurs
	    }
	};

	iterator begin() {
	    for (auto vit = buckets.begin(); vit != buckets.end(); ++vit) {
	        if (!vit->empty()) {
	            return iterator(buckets, vit, vit->begin()); // retourne l'itérateur au premier élément non vide
	        }
	    }
	    return end(); // si tout est vide, retourne l'itérateur de fin
	}

	iterator end() {
	    return iterator(buckets, buckets.end(), typename list_t::iterator());
	}

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
		//grow();
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

		    size_t new_capacity = b.capacity() * 2;
		    buckets_t res(new_capacity);

		    for (const std::forward_list<Entry> &k : b) {
		        for (const Entry &e : k) {
		            size_t h = std::hash<K>()(e.key);
		            size_t index = h % new_capacity;
		            res[index].push_front(e);
		        }
		    }

		    return &res;
	}
	void grow(){
		if(size()>0.8*buckets.capacity()){
			buckets = realloc(buckets);
		}
	}







};
