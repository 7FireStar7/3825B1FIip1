#include <iostream>

class array {
	size_t size;
	double* data;
public:
	//Конструктор
	array(size_t n, double element = 0.0) : size(n) {
		if (n == 0) {
			data = nullptr;
		}
		else {
			data = new double[size];
			for (size_t i = 0; i < size; i++) {
				data[i] = element;
			}
		}
	}
	
	//Конструктор копирования
	array(const array& arr) : size(arr.size) {
		if (size == 0) {
			data = nullptr;
		}
		else {
			data = new double[size];
			for (size_t i = 0; i < size; i++)
				data[i] = arr.data[i];
		}
	}

	//Деструктор
	~array() {
		delete[] data;
	}

	array& operator=(const array& other) {
		if (this != &other) { 
			array temp(other);
			std::swap(data, temp.data);
			std::swap(size, temp.size);
		}
		return *this;
	}
	
	double& operator[](size_t index) {
		return data[index];
	}

	const double& operator[](size_t index) const {
		return data[index];
	}

	size_t get_size() const {
		return size;
	}

	double min_arr() const {
		double min_element = DBL_MAX;
		for (size_t i = 0; i < size; i++) {
			if (data[i] < min_element) min_element = data[i];
		}
		return min_element;
	}

	bool is_sorted_up() const {
		for (size_t i = 0; i < size-1; i++) {
			if (data[i] > data[i + 1])
				return false;
		}
		return true;
	}

	bool is_sorted_down() const {
		for (size_t i = 0; i < size - 1; i++) {
			if (data[i] < data[i + 1])
				return false;
		}
		return true;
	}

	//select_odd будет выделять нечетные, копировать в отдельный массив
	array select_odd() const{
		array temp(size / 2);
		int index = 0;
		for (size_t i = 1; i < size; i+=2) {
			temp[index++] = data[i];
		}
		return temp;
	}


	//Перегруженный вывод в поток
	friend std::ostream& operator<<(std::ostream& os, const array& arr) {
		for (size_t i = 0; i < arr.size; i++) {
			std::cout << arr.data[i] << " ";
		}
		std::cout << std::endl;
		return os;
	}
};

int main() {
	//Для корректного вывода действительных чисел
	std::cout << std::fixed;
	std::cout.precision(1);

	array a(5);
	for (int i = 0; i < 5; i++) {
		std::cin >> a[i];
	}

	std::cout << a.get_size() << std::endl;
	
	a[0] = 1;
	
	std::cout << a[3] << std::endl;

	std::cout << a.min_arr() << std::endl;

	std::cout << a.is_sorted_up() << std::endl;

	array b = a.select_odd();
	std::cout << b << std::endl;

	return 0;
}
