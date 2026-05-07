#include <iostream>
#include <string>
#include <cmath>

class TaylorSeries {

public:
	enum class Function { SIN, COS, EXP };

	TaylorSeries() : current_function(Function::SIN), count_of_terms(1) {} //конструктор по умолчанию
	TaylorSeries(Function func, int terms) : current_function(func), count_of_terms(terms) {  //конструктор с параметрами
		if (count_of_terms < 1)
		{
			count_of_terms = 1;
			std::cout << "count of terms can't be < 1. Set to 1" << std::endl; //проверка на кол-во членов + установим 1 в случае, если <1
		}
	} 


	~TaylorSeries() {} //деструктор, но пустой т.к. не задействуем память

	void SetFunction(Function func) { current_function = func; } //установка текущей функции (выбор) / сеттер
	std::string getFunction() const {   //увидеть выбранную функцию / геттер
		switch (current_function) {
		case Function::SIN: return "SIN";
		case Function::COS: return "COS";
		case Function::EXP: return "EXP";
		default: return ""; //на всякий случай 
		}
	}

	void setCountOfTerms(int terms) { //установка текущего кол-во членов / сеттер
		if (terms >= 1) {
			count_of_terms = terms;
		}
		else {
			count_of_terms = 1;
			std::cout << "count of terms can't be < 1. Set to 1" << std::endl; 
		}
	}
	int getCountOfTerms() const{ return count_of_terms; } //увидеть выбранное кол-во членов / геттер

	std::string getFormula() const {
		switch (current_function) {
		case Function::SIN: return "sin(x) = x - x^3/3! + x^5/5! - ... + (-1)^n * x^(2n+1)/(2n+1)! + o(x^(2n+2))";
		case Function::COS: return "cos(x) = 1 - x^2/2! + x^4/4! - ... + (-1)^n * x^(2n)/(2n)! + o(x^(2n+1))";
		case Function::EXP: return "exp(x) = 1 + x + x^2/2! + x^3/3! + ... + x^n/n! + o(x^n)";
		default: return "";
		}
	}

	double calculate(double x) const {    //вычисление ряда по кол-ву членов
		double sum = 0.0, temp=0.0;
		switch (current_function) {
		case Function::SIN: {
			temp = x;
			for (int i = 0; i < count_of_terms; i++) {
				sum = sum + temp;
				temp = -temp * x * x / ((2 * i + 2) * (2 * i + 3));
			}
			break;
		}
		case Function::COS: {
			temp = 1.0;
			for (int i = 0; i < count_of_terms; ++i) {
				sum += temp;
				temp = -temp * x * x / ((2 * i + 1) * (2 * i + 2));
			}
			break;
		}
		case Function::EXP: {
			temp = 1.0;
			for (int i = 0; i < count_of_terms; i++) {
				sum = sum + temp;
				temp = temp * x / (i + 1);
			}
			break;
		}
		default: break; 
		}
		return sum;
	} 

	double termCalculate(int k, double x) const { //вычисление значения в точке x
		if (k < 0 || k >= count_of_terms) {
			std::cout << "error" << std::endl;
			return 0.0;
		}
		double temp = 0.0;
		switch (current_function) {
		case Function::SIN: {
			temp = x;
			for (int i = 0; i < k; ++i) {
				temp = -temp * x * x / ((2 * i + 2) * (2 * i + 3));
			}
			break;
		}
		case Function::COS: {
			temp = 1.0;
			for (int i = 0; i < k; ++i) {
				temp = -temp * x * x / ((2 * i + 1) * (2 * i + 2));
			}
			break;
		}
		case Function::EXP: {
			temp = 1.0;
			for (int i = 0; i < k; ++i) {
				temp = temp * x / (i + 1);
			}
			break;
		}
		default: break;
		}
		return temp;
	}

	double getDiff(double x) const { //получить разницу от вычисленного и эталонного значения
		double our_calc = calculate(x);
		double etalon=0.0;
		switch (current_function) {
		case Function::SIN: etalon = std::sin(x); break;
		case Function::COS: etalon = std::cos(x); break;
		case Function::EXP: etalon = std::exp(x); break;
		default: break;
		}
		return our_calc - etalon;
	}

private:
	Function current_function;
	int count_of_terms;
};

int main() {
	std::cout << "===== DEMONSTRATION OF TAYLOR SERIES CLASS =====\n\n"; //проверку составлял гпт 

	// ----- 1. Конструктор по умолчанию -----
	TaylorSeries ts1;
	std::cout << "Default constructed object:\n";
	std::cout << "  Function: " << ts1.getFunction() << "\n";
	std::cout << "  Terms: " << ts1.getCountOfTerms() << "\n";
	std::cout << "  Formula: " << ts1.getFormula() << "\n\n";

	// ----- 2. Конструктор с параметрами -----
	TaylorSeries ts2(TaylorSeries::Function::EXP, 7);
	std::cout << "Parameterized constructor (EXP, 7 terms):\n";
	std::cout << "  Function: " << ts2.getFunction() << "\n";
	std::cout << "  Terms: " << ts2.getCountOfTerms() << "\n";
	std::cout << "  Formula: " << ts2.getFormula() << "\n\n";

	// ----- 3. Изменение функции и числа членов -----
	TaylorSeries ts;
	ts.SetFunction(TaylorSeries::Function::COS);
	ts.setCountOfTerms(5);
	std::cout << "After setting COS and 5 terms:\n";
	std::cout << "  Function: " << ts.getFunction() << "\n";
	std::cout << "  Terms: " << ts.getCountOfTerms() << "\n";
	std::cout << "  Formula: " << ts.getFormula() << "\n\n";

	// ----- 4. Вычисление значения ряда в точке -----
	double x = 1.0;
	std::cout << "Calculation at x = " << x << ":\n";
	std::cout << "  cos(" << x << ") approximated = " << ts.calculate(x) << "\n";
	std::cout << "  std::cos(" << x << ") = " << std::cos(x) << "\n";
	std::cout << "  Deviation = " << ts.getDiff(x) << "\n\n";

	// ----- 5. Отдельный член ряда -----
	int k = 2; // третий член (нумерация с 0)
	std::cout << "Term " << k << " (k = " << k << ") of cos(" << x << "):\n";
	std::cout << "  Value = " << ts.termCalculate(k, x) << "\n";
	std::cout << "  (Expected: x^4/4! = 1^4/24 = " << 1.0 / 24.0 << ")\n\n";

	// ----- 6. Проверка для sin -----
	ts.SetFunction(TaylorSeries::Function::SIN);
	ts.setCountOfTerms(6);
	x = 0.5;
	std::cout << "Now testing SIN with 6 terms at x = " << x << ":\n";
	std::cout << "  sin(" << x << ") approximated = " << ts.calculate(x) << "\n";
	std::cout << "  std::sin(" << x << ") = " << std::sin(x) << "\n";
	std::cout << "  Deviation = " << ts.getDiff(x) << "\n";
	std::cout << "  Term 3 (k=3): " << ts.termCalculate(3, x)
		<< " (should be x^7/7! ≈ " << std::pow(x, 7) / 5040.0 << ")\n\n";

	// ----- 7. Проверка для exp -----
	ts.SetFunction(TaylorSeries::Function::EXP);
	ts.setCountOfTerms(10);
	x = 2.0;
	std::cout << "Now testing EXP with 10 terms at x = " << x << ":\n";
	std::cout << "  exp(" << x << ") approximated = " << ts.calculate(x) << "\n";
	std::cout << "  std::exp(" << x << ") = " << std::exp(x) << "\n";
	std::cout << "  Deviation = " << ts.getDiff(x) << "\n";
	std::cout << "  Term 5 (k=5): " << ts.termCalculate(5, x)
		<< " (should be x^5/5! = 32/120 ≈ " << 32.0 / 120.0 << ")\n\n";

	// ----- 8. Демонстрация изменения числа членов -----
	ts.setCountOfTerms(3);
	std::cout << "Changed terms to 3 for EXP:\n";
	std::cout << "  New approximation at x = 2: " << ts.calculate(2.0) << "\n";
	std::cout << "  Deviation now: " << ts.getDiff(2.0) << "\n";

	return 0;
}