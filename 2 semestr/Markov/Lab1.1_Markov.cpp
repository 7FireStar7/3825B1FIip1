#include <iostream>
#include <cmath>
#include <numeric>

class Rational {
	int ch;
	int zn;

	int gcd(int a, int b) {
		a = std::abs(a);
		b = std::abs(b);
		while (b != 0) {
			int temp = b;
			b = a % b;
			a = temp;
		}
		return a;
	}

	void rationalization() {
		if (zn == 0) {
			std::cout << "Ошибка! Знаменатель не может быть 0, знаменателю присвоено значение 1" << std::endl;
			zn = 1;
		}
		if (zn < 0) {
			ch = -ch;
			zn = -zn;
		}
		int gcdd = gcd(ch, zn);
		if (gcdd != 0) {
			ch /= gcdd;
			zn /= gcdd;
		}
	}
public:
	Rational() {
		ch = 0;
		zn = 1;
	}

	Rational(int a, int b) {
		ch = a;
		zn = b;
		rationalization();
	}

	Rational(const Rational& a) {
		ch = a.ch;
		zn = a.zn;
	}

	Rational operator+(const Rational& other) const {
		int new_ch = ch * other.zn + other.ch * zn;
		int new_zn = zn * other.zn;
		return Rational(new_ch, new_zn);
	}

	Rational operator-(const Rational& other) const {
		int new_ch = ch * other.zn - other.ch * zn;
		int new_zn = zn * other.zn;
		return Rational(new_ch, new_zn);
	}

	Rational operator*(const Rational& other) const {
		int new_ch = ch * other.ch;
		int new_zn = zn * other.zn;
		return Rational(new_ch, new_zn);
	}

	Rational operator/(const Rational& other) const {
		if (other.ch == 0) {
			std::cout << "Ошибка! Знаменатель равен 0";
			return Rational(0, 1);
		}
		int new_ch = ch * other.zn;
		int new_zn = zn * other.ch;
		return Rational(new_ch, new_zn);
	}

	void print() const {
		if (zn == 1) {
			std::cout << ch << std::endl;
		}
		else {
			std::cout << ch << "/" << zn << std::endl;
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
		r.print();
		return os;
	}
};

int main() {
	setlocale(LC_ALL, "Russian");
	Rational a(1, 2);
	Rational b(2, 1);
	Rational c(b);
	std::cout << c;
	return 0;
}