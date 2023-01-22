#include <hardware/keyboard.hpp>

namespace BdUI {
	KeyList::KeyList(KeyType k) {
		this->push_back(k);
	}

	KeyList& KeyList::operator+=(KeyType key) {
		if (std::find(this->begin(), this->end(), key) == this->end())
			this->push_back(key);
		return *this;
	}
	KeyList& KeyList::operator-=(KeyType key) {
		if (auto item = std::find(this->begin(), this->end(), key);item != this->end())
			this->erase(item);
		return *this;
	}
}