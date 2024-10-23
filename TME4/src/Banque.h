#pragma once

#include "Compte.h"
#include <vector>

namespace pr {

class Banque {
	typedef std::vector<Compte> comptes_t;
	mutable std::recursive_mutex m;
public :
	comptes_t comptes;
	Banque (size_t ncomptes, size_t solde) : comptes (ncomptes, Compte(solde)){
	}
	void transfert(size_t deb, size_t cred, unsigned int val) ;
	size_t size() const ;
	bool comptabiliser (int attendu) const ;
	std::recursive_mutex & getMutex() const;
	void lock() const;
	void unlock()const;
	bool try_lock() const;


};

}
