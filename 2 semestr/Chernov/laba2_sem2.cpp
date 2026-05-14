#include <iostream>
#include <string>

using namespace std;

class Events_Calendar
{
private:
	int years[30] = {};
	int months[30] = {};
	int days[30] = {};
	string events[30];
	int count;

public:
	Events_Calendar()
	{
		count = 0;
	}

	Events_Calendar(int d, int m, int y, string e): Events_Calendar()
	{
		set_event(d, m, y, e);
	}

	void set_event(int d, int m, int y, string e)
	{
		if (m < 1 || m > 12 || d < 1 || d > 30)
		{
			cout << "invalid month or day" << endl;
			return;
		}
		if (y > 2020)
		{
			cout << "can't set event to date wich late than 31.12.2020" << endl;
			return;
		}
		if (y < 1)
		{
			cout << "can't set event to date wich early than 01.01.1" << endl;
			return;
		}
		if (count < 30) 
		{
			for (int i = 0; i < count; i++)
			{
				if (years[i] == y && months[i] == m && days[i] == d)
				{
					cout << "error. this day already has this event" << endl;
					return;
				}
			}
			years[count] = y;
			months[count] = m;
			days[count] = d;
			events[count] = e;
			count++;
		}
	}

	~Events_Calendar() {}  

	Events_Calendar& operator=(const Events_Calendar& other)
	{
		if (this != &other)
		{
			count = other.count;
			for (int i = 0; i < count; ++i)
			{
				years[i] = other.years[i];
				months[i] = other.months[i];
				days[i] = other.days[i];
				events[i] = other.events[i];
			}
		}
		return *this;
	}

	int find_event_index(string name) const
	{
		for (int i = 0; i < count; i++)
		{
			if (events[i] == name)
			{
				return i;
			}
		}
		return -1;
	}

	void difference(string e, int d, int m, int y)
	{
		int index = find_event_index(e);
		if (index == -1)
		{
			cout << "error, cant find that event" << endl;
			return;
		}

		int days_event = years[index] * 365 + months[index] * 30 + days[index];
		int days_data = y * 365 + m * 30 + d;
		int diff = abs(days_event - days_data);

		int years_diff = diff / 365;
		int ost = diff % 365;
		int months_diff = ost / 30;
		int days_diff = ost % 30;

		cout << "difference between event " << e << " (" << days[index] << "." << months[index] << "." << years[index] << ") and "
			<< d << "." << m << "." << y << " is: " << days_diff << " days, " << months_diff << " month, " << years_diff << " years" << endl;
	}

	void get_date_event(string name)
	{
		int index = find_event_index(name);
		cout << "Date of event " << name << " is: " << days[index] << "." << months[index] << "." << years[index] << endl;
	}

	void print() const
	{
		if (count == 0) 
		{
			cout << "calendar is empty." << endl;
			return;
		}
		for (int i = 0; i < count; i++)
		{
			cout << "date: " << days[i] << "." << months[i] << "." << years[i] << "  Event: " << events[i] << endl;
		}
	}

	void special_new_event_set(string e, char sign, int d, int m, int y)
	{
		int index = find_event_index(e);
		if (index == -1)
		{
			cout << "error, cant find that event" << endl;
			return;
		}
		 
		int days_event = years[index] * 365 + months[index] * 30 + days[index];
		int days_data = y * 365 + m * 30 + d;
		switch (sign)
		{
		case '+':
		{
			days_event = days_event + days_data;
			int years = days_event / 365;
			int ost = days_event % 365;
			int months = ost / 30;
			int days = ost % 30;
			int count_temp = count;
			if (years > 2020)
			{
				cout << "can't set event to date wich late than 31.12.2020" << endl;
				return;
			}
			if (years < 1)
			{
				cout << "can't set event to date wich early than 01.01.1" << endl;
				return;
			}
			count = index;
			set_event(days, months, years, e);
			count = count_temp;
			break;
		}
		case '-':
		{
			days_event = days_event - days_data;
			int years = days_event / 365;
			int ost = days_event % 365;
			int months = ost / 30;
			int days = ost % 30;
			int count_temp = count;
			if (years > 2020)
			{
				cout << "can't set event to date wich late than 31.12.2020" << endl;
				return;
			}
			if (years < 1)
			{
				cout << "can't set event to date wich early than 01.01.1" << endl;
				return;
			}
			count = index;
			set_event(days, months, years, e);
			count = count_temp;
			break;
		}
		default: break;
		}
	}

};

int main()
{
	Events_Calendar calendar_main;
	Events_Calendar calendar_test_operator;
	
	calendar_main.set_event(25, 12, 2020, "Christmas");
	calendar_main.set_event(1, 1, 2020, "New Year");
	calendar_main.set_event(8, 3, 2020, "International Women's Day");  //point 1 
	calendar_main.set_event(1, 5, 2020, "Labour Day");
	
	
	calendar_test_operator = calendar_main; //assignment operator

	calendar_main.print(); //point 2
	cout << endl;
	calendar_test_operator.print(); //test for assignment operator
	cout << endl;

	calendar_main.difference("Christmas", 12, 3, 2021); //point 3
	cout << endl;
	

	calendar_main.special_new_event_set("Christmas", '-', 15, 3, 5); //point 4
	calendar_main.print();

	return 0;
}