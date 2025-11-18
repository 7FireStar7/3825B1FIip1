#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int guess_the_digit(digit)
{
	int a = -5;
	while (a != digit)
	{
		a = rand() % 10;
	}
	return 1;
}
int count_of_digits(number)
{
	int count = 0;
	while (number > 0)
	{
		count++;
		number = number / 10;
	}
	return count;
}
void main()
{
	srand(time(NULL));
	long long int first_number, second_number;
	int programm_mode;
	int arr_of_digits[2][100];
	printf("What do you want to do?\n1. Calculate sum of two numbers\n2. Exit the programm\n----> "); scanf_s("%d", &programm_mode);
	switch (programm_mode)
	{
	case 1:
	{
		printf("\nInput first number: "); scanf_s("%lld", &first_number);
		printf("Input second number: "); scanf_s("%lld", &second_number);
		int first_number1 = first_number;
		int second_number1 = second_number;
		for (int i = 0; i < 2; i++)
		{
			if (i == 0)
			{
				for (int j = 0; j < count_of_digits(first_number); j++)
				{
					arr_of_digits[i][count_of_digits(first_number)-j-1] = first_number1 % 10;
					first_number1 = first_number1 / 10;
				}
			}
			else
			{
				for (int j = 0; j < count_of_digits(second_number); j++)
				{
					arr_of_digits[i][count_of_digits(second_number)-j-1] = second_number1 % 10;
					second_number1 = second_number1 / 10;
				}
			}
		}
		int random1 = 0;
		long long int first_number_prime = 0;
		long long int second_number_prime = 0;
		for (int i = 0; i < 2; i++)
		{
			if (i == 0)
			{
				for (int j = 0; j < count_of_digits(first_number); j++)
				{
					while (arr_of_digits[i][j] != random1)
					{
						random1 = rand() % 10;
					}
					first_number_prime = first_number_prime * 10 + random1;
				}
			}
			else
			{
				for (int j = 0; j < count_of_digits(second_number); j++)
				{
					while (arr_of_digits[i][j] != random1)
					{
						random1 = rand() % 10;
					}
					second_number_prime = second_number_prime * 10 + random1;
				}
			}
		}
		printf("\nFinal answer: %lld", first_number_prime+second_number_prime);
	}
	case 2: return 0; break;
	default: break;
	}
}