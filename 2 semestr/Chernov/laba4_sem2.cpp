#include <iostream>
#include <string>
#include <vector>
#include <fstream> //для 8-9

class Thermometer
{
private:
	struct observation
	{
		int day;
		int month;
		int hour;
		double temperature;
	};
	std::vector<observation> observations;
	int start_day;
	int start_month;
	int start_hour;
	int year;  //т.к. мы не можем выйти за рамки установленного года то оставляем одну переменную для года

public:
	Thermometer() {
		start_day = 1;
		start_month = 1;
		start_hour = 1;
		year = 1;
	}

	void set_start_time(int startDay, int startMonth, int startHour, int Year) { //пункт 1
		start_day = startDay;
		start_month = startMonth;
		start_hour = startHour;
		year = Year;
	}

	~Thermometer() {};

	std::string get_start_time() const {  //пукнт 2, используем to_string чтобы перевести int в string
		return std::to_string(start_day) + "." + std::to_string(start_month) + "." +
			std::to_string(year) + " " + std::to_string(start_hour) + ":00";
	}

	bool date_valid(int day, int month, int hour) const { //будем проверять пытается ли пользователь добавить наблюдение раньше стартового времени
		if (month < start_month) {
			return false;
		}
		if (month > start_month) {
			return true;
		}
		if (day < start_day) {
			return false;
		}
		if (day > start_day) {
			return true;
		}
		if (hour < start_hour) {
			return false;
		}
		if (hour >= start_hour) {
			return true;
		}
	}

	void add_observation(int day, int month, int hour, double temp) {
		if (date_valid(day, month, hour) == false) {   //проверяем 
			std::cout << "\nobservation " << day << "." << month << " " << hour << ":00 " << "is before start time, ignored\n" << std::endl;
			return;
		}
		for (observation& obs : observations) {    //ищем совпадает ли с уже существующим наблюдением, меняем, если да //читается как для каждого вектора obs типа observation из observations
			if (obs.day == day && obs.month == month && obs.hour == hour) {
				obs.temperature = temp;
				return;
			}
		}
		observation obs{};   //если выше ни с чем не совпало
		obs.day = day;
		obs.month = month;
		obs.hour = hour;
		obs.temperature = temp;
		observations.push_back(obs); //push_back в std::vector увеличивает размер вектора на 1 и добавляет элемент
	}

	double get_temperature(int day, int month, int hour) const { //пункт 4 (геттер)
		for (const observation& obs : observations) {   //используем константную ссылку и const у метода иначе каждый элемент observations бы копировался и только затем проверялся
			if (obs.day == day && obs.month == month && obs.hour == hour) {
				return obs.temperature;
			}
		}
		return 1000.0; //на случай опечатки или ненахода
	}

	void set_day_series(int day, int month, const std::vector<int>& hours, const std::vector<double>& temperatures) { //пункт 5, главное чтобы на входе было 2 одинаковых по длинне вектора
		size_t n = hours.size(); //size_t - беззнаковое целое (лушче всего для размеров чего-либо)
		for (size_t i=0; i < n; i++) {  //поэтому i - тоже size_t
			add_observation(day, month, hours[i], temperatures[i]);
		}
	}

	double average_temperature(int day, int month) const { //пункт 6
		double sum = 0.0;
		int count = 0;
		for (const observation& obs : observations) {
			if (obs.day == day && obs.month == month) {
				sum = sum + obs.temperature;
				count++;
			}
		}
		if (count == 0) {   //если ни одного наблюдения в этот день не было
			return 1000.0;
		}
		return sum / count;
	}

	double average_night_month_temperature(int month) const { //пункт 7 (ночь с 22:00 до 5:00 вкл.)
		double sum = 0.0;
		int count = 0;
		for (const observation& obs : observations) {
			if (obs.month == month && (obs.hour >= 22 || obs.hour <= 5)) {
				sum = sum + obs.temperature;
				count++;
			}
		}
		if (count == 0) {
			return 1000.0;
		}
		return sum / count;
	}

	double average_day_month_temperature(int month) const { //пункт 7 (день с 6:00 до 21:00 вкл.)
		double sum = 0.0;
		int count = 0;
		for (const observation& obs : observations) {
			if (obs.month == month && (obs.hour >= 6 && obs.hour <= 21)) {
				sum = sum + obs.temperature;
				count++;
			}
		}
		if (count == 0) {
			return 1000.0;
		}
		return sum / count;
	}

	void save_to_file(const std::string& filename) const {
		std::ofstream file(filename);
		if (!file) {  //если не получилось открыть файл
			std::cout << "error\n";
			return;
		}
		file << start_day << " " << start_month << " " << start_hour << " " << year << "\n"; //стартовые параметры
		for (const observation& obs : observations) {
			file << obs.day << " " << obs.month << " " << obs.hour << " " << obs.temperature << "\n";
		}
		std::cout << "observations history saved to " << filename << "\n";
	}

	void load_from_file(const std::string& filename) {
		std::ifstream file(filename);
		if (!file) {
			std::cout << "error\n";
			return;
		}
		int sd, sm, sh, yr;
		file >> sd >> sm >> sh >> yr;
		start_day = sd;
		start_month = sm;
		start_hour = sh;
		year = yr;
		observations.clear();
		int d, m, h;
		double t;
		while (file >> d >> m >> h >> t) {
			observation obs{};
			obs.day = d;
			obs.month = m;
			obs.hour = h;
			obs.temperature = t;
			observations.push_back(obs);
		}
		std::cout << "observations history loaded from " << filename << "\n";
	}

	friend std::ostream& operator<<(std::ostream& os, const Thermometer& t);

};

std::ostream& operator<<(std::ostream& os, const Thermometer& t) {
	os << "<=== THERMOMETR STATS ===>\n";
	os << "Start: " << t.get_start_time() << "\n";
	os << "Total amount of observations: " << t.observations.size() << "\n";
	if (!t.observations.empty()) {
		os << "Observations list:\n";
		for (const Thermometer::observation& obs : t.observations) {
			os << "  " << obs.day << "." << obs.month << " " << obs.hour << ":00 ==> " << obs.temperature << " C\n";
		}
	}
	else {
		os << "there are no observations .\n";
	}
	return os;
}

int main() {
	std::cout << "===== FULL TEST OF THERMOMETER CLASS =====\n\n";

	// ------------------------------------------------------------
	// Пункты 1 и 2: установка и получение начальных даты/времени
	// ------------------------------------------------------------
	Thermometer thermo;
	thermo.set_start_time(1, 1, 6, 2023);   // 1 января 2023, 6:00
	std::cout << "--- Initial state (points 1 & 2) ---\n";
	std::cout << thermo << "\n";            // вывод через перегруженный <<
	std::cout << "get_start_time(): " << thermo.get_start_time()
		<< " (expected: 1.1.2023 6:00)\n\n";

	// ------------------------------------------------------------
	// Пункт 3: задать наблюдение (с заменой)
	// ------------------------------------------------------------
	std::cout << "--- Adding observations (point 3) ---\n";
	thermo.add_observation(15, 7, 10, 23.5);   // 15.07 10:00
	thermo.add_observation(15, 7, 14, 26.1);   // 15.07 14:00
	thermo.add_observation(16, 7, 10, 24.0);   // 16.07 10:00
	// Замена:
	thermo.add_observation(15, 7, 10, 20.0);   // должно заменить 23.5 -> 20.0
	std::cout << "After adding 3 observations (one replaced):\n";
	std::cout << thermo << "\n";

	// ------------------------------------------------------------
	// Пункт 4: узнать температуру в выбранном наблюдении
	// ------------------------------------------------------------
	std::cout << "--- Getting temperatures (point 4) ---\n";
	std::cout << "get_temperature(15,7,10) = "
		<< thermo.get_temperature(15, 7, 10)
		<< " (expected: 20.0)\n";
	std::cout << "get_temperature(15,7,14) = "
		<< thermo.get_temperature(15, 7, 14)
		<< " (expected: 26.1)\n";
	std::cout << "get_temperature(10,1,12) = "
		<< thermo.get_temperature(10, 1, 12)
		<< " (expected: 1000.0, no data)\n\n";

	// ------------------------------------------------------------
	// Пункт 5: задать серию наблюдений для выбранной даты
	// ------------------------------------------------------------
	std::cout << "--- Setting day series (point 5) ---\n";
	std::vector<int> hours = { 8, 12, 16, 20 };
	std::vector<double> temps = { 22.0, 25.0, 24.5, 19.0 };
	thermo.set_day_series(20, 8, hours, temps);   // 20 августа
	std::cout << "After series for 20.08:\n" << thermo << "\n";

	// ------------------------------------------------------------
	// Пункт 6: средняя температура для выбранной даты
	// ------------------------------------------------------------
	std::cout << "--- Average temperature for a day (point 6) ---\n";
	double avg15 = thermo.average_temperature(15, 7);
	std::cout << "average_temperature(15,7) = " << avg15
		<< " (expected: " << (20.0 + 26.1) / 2.0 << ")\n";
	double avg20 = thermo.average_temperature(20, 8);
	std::cout << "average_temperature(20,8) = " << avg20
		<< " (expected: " << (22.0 + 25.0 + 24.5 + 19.0) / 4.0 << ")\n";
	double avgEmpty = thermo.average_temperature(1, 1);
	std::cout << "average_temperature(1,1)  = " << avgEmpty
		<< " (expected: 1000.0, no data)\n\n";

	// ------------------------------------------------------------
	// Пункт 7: средняя дневная / ночная температура для месяца
	// ------------------------------------------------------------
	std::cout << "--- Day/night averages for month (point 7) ---\n";
	// Для июля: только наблюдения 15.07 в 10:00 (день) и 14:00 (день) – оба дневные.
	double dayJul = thermo.average_day_month_temperature(7);
	std::cout << "Day average July   = " << dayJul
		<< " (expected: " << (20.0 + 26.1 + 24.0) / 3.0 << ")\n";
	double nightJul = thermo.average_night_month_temperature(7);
	std::cout << "Night average July = " << nightJul
		<< " (expected: 1000.0, no night data)\n";

	// Для августа: все четыре записи (8,12,16,20) – дневные.
	double dayAug = thermo.average_day_month_temperature(8);
	std::cout << "Day average August = " << dayAug
		<< " (expected: " << (22.0 + 25.0 + 24.5 + 19.0) / 4.0 << ")\n";
	double nightAug = thermo.average_night_month_temperature(8);
	std::cout << "Night average August = " << nightAug
		<< " (expected: 1000.0)\n\n";

	// ------------------------------------------------------------
	// Пункт 8: сохранить историю в файл
	// ------------------------------------------------------------
	std::cout << "--- Saving to file (point 8) ---\n";
	thermo.save_to_file("history.txt");

	// ------------------------------------------------------------
	// Пункт 9: считать историю из файла
	// ------------------------------------------------------------
	std::cout << "\n--- Loading from file (point 9) ---\n";
	Thermometer thermo2;
	thermo2.load_from_file("history.txt");
	std::cout << "\nVerification of loaded object:\n";
	std::cout << thermo2;   // вывод через перегруженный <<

	// Сравнение ключевых значений
	std::cout << "get_start_time() = " << thermo2.get_start_time()
		<< " (expected: 1.1.2023 6:00)\n";
	std::cout << "get_temperature(15,7,10) = "
		<< thermo2.get_temperature(15, 7, 10)
		<< " (expected: 20.0)\n";
	std::cout << "get_temperature(20,8,16) = "
		<< thermo2.get_temperature(20, 8, 16)
		<< " (expected: 24.5)\n";
	std::cout << "average_temperature(20,8) = "
		<< thermo2.average_temperature(20, 8)
		<< " (expected: " << (22.0 + 25.0 + 24.5 + 19.0) / 4.0 << ")\n";

	//тестим отдельно перегрузку << для наглядности
	std::cout << "\n\n test of overloading <<:" << std::endl;
	Thermometer peregruzka;
	peregruzka.set_start_time(13, 5, 4, 2025);
	peregruzka.add_observation(22, 5, 6, 30.5);
	peregruzka.add_observation(23, 5, 6, 30.5);
	peregruzka.add_observation(11, 5, 7, 30.5);
	std::vector<int> hours1 = { 8, 12, 16, 20 };
	std::vector<double> temps1 = { 22.0, 25.0, 24.5, 19.0 };
	peregruzka.set_day_series(11, 5, hours1, temps1); //тут тестим, что раньше start'овых значений не принимает
	std::cout << peregruzka << std::endl; //тестим перегрузку

	return 0;
}