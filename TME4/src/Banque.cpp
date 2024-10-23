#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {

	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
	if(deb<cred){
		debiteur.lock();
		crediteur.lock();

	}
	else {
		crediteur.lock();
		debiteur.lock();
	}

	if (debiteur.debiter(val)) {
		crediteur.crediter(val);

	}
}
size_t Banque::size() const {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {

		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}

		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
std::recursive_mutex & Banque::getMutex()const{
	return m;
}

void Banque::lock() const{
	m.lock();
}
void Banque::unlock()const{
	m.unlock();
}
bool Banque::try_lock() const{
	 return m.try_lock();
}
}
