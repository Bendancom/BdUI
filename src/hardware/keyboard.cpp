#include <hardware/keyboard.hpp>

namespace BdUI {
	std::vector<KeyType> KeyList::GetKeys() {
		return Keys;
	}

	KeyList& KeyList::operator+=(KeyType&& key) {
		if (std::find(Keys.begin(), Keys.end(), key) == Keys.end())
			Keys.push_back(key);
		return *this;
	}
	KeyList& KeyList::operator-=(KeyType&& key) {
		Keys.erase(std::find(Keys.begin(), Keys.end(), key));
		return *this;
	}

	bool KeyList::operator==(const KeyList& keylist) {
		if (Keys.size() != keylist.Keys.size()) return false;
		else {
			for (std::vector<KeyType>::iterator iter = Keys.begin(); iter != Keys.end(); iter++) {
				if (std::find(keylist.Keys.begin(),keylist.Keys.end(),*iter) == Keys.end()) return false;
			}
			return true;
		}
	}
	bool KeyList::operator!=(const KeyList& keylist) {
		if (Keys.size() != keylist.Keys.size()) return true;
		else {
			for (std::vector<KeyType>::iterator iter = Keys.begin(); iter != Keys.end(); iter++) {
				if (std::find(keylist.Keys.begin(), keylist.Keys.end(), *iter) == Keys.end()) return true;
			}
			return false;
		}
	}
}