#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <locale>
#include <fstream>

class Dictionary {
	std::multimap<std::string, std::string> dict;
public:
	Dictionary() = default;

	Dictionary(const Dictionary&) = default;
	Dictionary& operator=(const Dictionary&) = default;

	~Dictionary() = default;

	void add_word(std::string english, std::string russian) {
		dict.emplace(english, russian);
	}

	void change_translate(std::string english, std::string new_russian) {
		dict.erase(english);
		dict.emplace(english, new_russian);
	}

	void print_translate(std::string english) const {
		auto it = dict.equal_range(english);
		for (auto& i = it.first; i != it.second; i++) {
			std::cout << i->second << ' ';
		}
		std::cout << std::endl;
	}

	bool check_word(std::string english) const {
		if (dict.find(english) != dict.end()) return true;
		return false;
	}

	//возвращает число английских слов
	size_t size_key() {
		std::unordered_set<std::string> s;
		for (auto& i : dict) {
			s.insert(i.first);
		}
		return s.size();
	}

	//возвращает число всех переводов
	size_t size_value() const{
		return dict.size();
	}

	void load_from_file() {
		std::ifstream file("input.txt");
		if (!file) {
			throw std::runtime_error("Не удалось открыть файл для чтения");
		}
		dict.clear();
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty()) continue;
			size_t space_pos = line.find(' ');
			if (space_pos != std::string::npos) {
				std::string english = line.substr(0, space_pos);
				std::string russian = line.substr(space_pos + 1);
				dict.emplace(english, russian);
			}
		}
	}

	void save_to_file() const {
		std::ofstream file("output.txt");
		if (!file) {
			throw std::runtime_error("Не удалось открыть файл для записи");
		}
		for (const auto& i : dict) {
			file << i.first << " " << i.second << std::endl;
		}
	}

};

int main() {
	setlocale(LC_ALL, "Russian");

	try {
		Dictionary a;
		a.load_from_file();

		a.add_word("apple", "яблоко");
		a.add_word("hospital","больница");

		std::cout << "Уникальных английских слов: " << a.size_key() << std::endl;
		std::cout << "Число всех переводов: " << a.size_value() << std::endl;

		a.save_to_file();
	}
	catch (const std::exception& e) {
		std::cerr << "Ошибка: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}