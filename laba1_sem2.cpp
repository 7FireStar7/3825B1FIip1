#include <iostream>
#include <cstdlib>  // for abs()

using namespace std;

class Rational
{
private:
	int numerator;
	int denominator;
public:
	Rational(int numer, int denomer)
	{
		if (denomer == 0) 
		{
			cout << "Denominator can't be 0! set to 0/1" << endl;
			numerator = 0;
			denominator = 1;   
		}
		else
		{
			numerator = numer;
			denominator = denomer;
			normal();         
		}
	}

	int gcd(int a, int b)
	{
		a = abs(a);
		b = abs(b);
		int temp = 0;
		while (b != 0)
		{
			temp = a % b;
			a = b;
			b = temp;
		}
		return a;
	}

	void normal()
	{
		if (denominator < 0)
		{
			numerator = -numerator;
			denominator = -denominator;
		}
		int gcd_a = gcd(numerator, denominator);
		numerator = numerator / gcd_a;
		denominator = denominator / gcd_a;
	}

	Rational sum(Rational other_rational)
	{
		int new_num = numerator * other_rational.denominator + other_rational.numerator * denominator;
		int new_denom = denominator * other_rational.denominator;
		return Rational(new_num, new_denom);
	}

	Rational difference(Rational other_rational)
	{
		int new_num = numerator * other_rational.denominator - other_rational.numerator * denominator;
		int new_denom = denominator * other_rational.denominator;
		return Rational(new_num, new_denom);
	}

	Rational multiplication(Rational other_rational)
	{
		int new_num = numerator * other_rational.numerator;
		int new_denom = denominator * other_rational.denominator;
		return Rational(new_num, new_denom);
	}

	Rational division(Rational other_rational) //division - delenie
	{
		int new_num = numerator * other_rational.denominator;
		int new_denom = denominator * other_rational.numerator;
		return Rational(new_num, new_denom);
	}

	void print()
	{
		cout << numerator << "/" << denominator << endl;
	}
};

int main()
{
	Rational a(1, 6);
	Rational b(3, 8);

	Rational sum = a.sum(b);
	cout << "a + b = ";
	sum.print();

	Rational dif = a.difference(b);
	cout << "a - b = ";
	dif.print();

	Rational div = a.division(b);
	cout << "a / b = ";
	div.print();

	Rational mult = a.multiplication(b);
	cout << "a * b = ";
	mult.print();
	return 0;
}


