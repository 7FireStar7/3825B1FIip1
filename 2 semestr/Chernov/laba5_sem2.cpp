#include <iostream>
#include <string>
#include <vector>

struct Client {
	std::string card_number;
	std::string FIO;
	long long int balance = 0;
	std::string pin_code; 
	bool is_blocked = false;
	short pin_attempts = 0;
};

class ProcessingCenter {
private:
	std::vector<Client> clients;

public:
	void add_client(const Client& new_client) { 
		clients.push_back(new_client);
	}

	Client* find_client(const std::string& card_number) {
		for (int i = 0; i < clients.size(); i++) {
			if (clients[i].card_number == card_number) {
				return &clients[i]; //если совпало, возвращаем адрес нужного клиента чтобы работать с ним
			}
		}
		return nullptr; //если ничего не нашли
	}

};



class CashMachine {
private:
	ProcessingCenter* processing_center;  
	Client* current_client = nullptr;
	bool is_pin_verified = false; //нужно будет проверять, подтвержден ли пин код

	const int max_cassette_capacity = 2000; //макс. емкость одной кассеты (в купюрах)
	const int max_bills_per_operation = 40; //макс. купюр за раз (выдача или прием)

	int nominals[6] = { 5000,2000,1000,500,200,100 }; //массив номиналов
	int cassettes[6] = { 0,0,0,0,0,0 }; //кол-во купюр в кассетах (пока 0)

public:
	CashMachine(ProcessingCenter* center) { //конструктор (по дефолту все кассеты на 80 процентов заполнены)
		processing_center = center;

		int initial_bills = max_cassette_capacity * 0.8;
		for (int i = 0; i < 6; i++) {
			cassettes[i] = initial_bills; //заполняем каждую кассету 1600-ми купюрами
		}
	}

	bool insert_card(const std::string& card_number) {
		if (current_client != nullptr) { //проверяем на наличие внутри карты
			std::cout << "Ошибка. Внутри уже есть карта\n" << std::endl; 
			return false;
		}

		Client* found_card = processing_center->find_client(card_number); 

		if (found_card == nullptr) { //проверяем на наличие карты в базе
			std::cout << "Ошибка. Такой карты нет в базе\n" << std::endl;
			return false;
		}

		if (found_card->is_blocked == true) { //проверяем, заблокирована ли карта?
			std::cout << "Ошибка. Эта карта заблокирована\n" << std::endl;
			return false;
		}
		//если все проверки пройдены
		current_client = found_card; //банкомат получил карту
		std::cout << "Карта успешно вставлена, здравствуйте: " << current_client->FIO << std::endl;

		is_pin_verified = false; //делаем false, чтобы он начал вводить пин код
		return true;
	}

	bool verify_pin(const std::string& input_pin) {
		if (current_client->pin_code == input_pin) { //если пинкод совпадет, выведем фио владельца этой карты
			std::cout << "Пин код верный. Приветствуем вас, " << current_client->FIO << std::endl;

			current_client->pin_attempts = 0;
			is_pin_verified = true;
			return true;
		}
		
		else { //если пинкод неверный
			current_client->pin_attempts++;
			std::cout << "Ошибка. Пин код неверный. Осталось попыток: " << 3 - current_client->pin_attempts << std::endl; //сколько попыток осталось

			if (current_client->pin_attempts >= 3) {
				current_client->is_blocked = true;
				std::cout << "Упс. Карта заблокирована из-за 3 неверных вводов пин кода" << std::endl;

				eject_card(); //выбрасываем пользователя из сессии
			}
			return false; //проверка пина не удалась, возвращаем false
		}
	}

	void eject_card() { //метод для возврата карты (выброса пользователя из сессии)
		if (current_client == nullptr) {
			std::cout << "Ошибка. Карта не вставлена." << std::endl;
			return;
		}

		std::cout << "Карта " << current_client->card_number << " успешно возвращена" << std::endl;
		std::cout << "Удачного дня, " << current_client->FIO << std::endl;

		current_client = nullptr; //заканчиваем сессию пользователя
		is_pin_verified = false;
	}

	bool withdraw_cash(long long int amount) {
		if (is_pin_verified == false) {
			std::cout << "Ошибка. Пин код не подтвержден" << std::endl;
			return false;
		}

		if (amount > current_client->balance) { //сначала проверим кол-во денег у клиента
			std::cout << "Ошибка. Недостаточно средств" << std::endl;
			return false;
		}

		long long int remaining = amount; //пометим, сколько нужно отдать
		int bills_to_give[6] = { 0,0,0,0,0,0 }; //сколько купюр нужно будет отдать
		int total_bills_count = 0; //общий счетчик купюр, готовых для выдачи клиенту (нужен, чтобы проверять условия 40 купюр)

		for (int i = 0; i < 6; i++) { //будем стараться отдавать как можно меньше купюр
			if (remaining >= nominals[i]) { //если кол-во денег, которое нужно отдать, больше 5000, то сначала выдадим ими (и так для каждого i из массива номиналов)
				int needed_bills = remaining / nominals[i]; //сколько купюр этим номиналом можно отдать

				int actual_bills = std::min(needed_bills, cassettes[i]);  //если в кассете с этим номиналом меньше чем needed_bills, то он отдаст столько, сколько сможет)

				if (actual_bills > 0) { //выдаем сколько посчитали выше
					bills_to_give[i] = actual_bills;
					remaining = remaining - (actual_bills * nominals[i]); 
					total_bills_count = total_bills_count + actual_bills;
				}
			}
		}

		if (remaining > 0) { //после того как он пробежал все номиналы, если есть остаток, то ошибка 
			std::cout << "Ошибка. Банкомат не может выдать эту сумму имеющимися купюрами" << std::endl;
			return false;
		}

		if (total_bills_count > max_bills_per_operation) {
			std::cout << "Ошибка. Превышен лимит купюр за 1 операцию (понизте сумму вывода)" << std::endl;
			return false;
		}

		current_client->balance = current_client->balance - amount; //меняем баланс клиента

		std::cout << "Успешно! Выдано денег: " << amount << " руб." << std::endl;
		for (int i = 0; i < 6; i++) {
			if (bills_to_give[i] > 0) {
				cassettes[i] = cassettes[i] - bills_to_give[i]; //уменьшаем в кассетах столько, сколько выдали выше
				std::cout << "Купюры номиналом " << nominals[i] << " руб. - " <<bills_to_give[i] << " шт." << std::endl;
			}
		}
		return true;
	}

	bool deposit_cash(int inserted_bills[6]) {
		if (current_client == nullptr) {
			std::cout << "Ошибка. Карта не вставлена" << std::endl;
			return false;
		}
		int total_bills_count = 0;
		long long int total_amount = 0;

		for (int i = 0; i < 6; i++) {
			if (cassettes[i] + inserted_bills[i] > max_cassette_capacity) { //чтобы не могли положить больше чем ограничение кассеты
				std::cout << "Ошибка. Кассета номиналом " << nominals[i] << " переполнена" << std::endl;
				return false;
			}

			total_bills_count = total_bills_count + inserted_bills[i];
			total_amount = total_amount + inserted_bills[i] * nominals[i];
		}

		if (total_bills_count > max_bills_per_operation) {
			std::cout << "Ошибка. Нельзя внести больше 40 штук за раз." << "\nВы внесли: " << total_bills_count << " шт." << std::endl;
			return false;
		}

		if (total_bills_count == 0) {
			std::cout << "Вы не внесли не одной купюры, отмена операции" << std::endl;
			return false;
		}

		for (int i = 0; i < 6; i++) {
			cassettes[i] = cassettes[i] + inserted_bills[i]; //пополняем кассеты
		}
		current_client->balance = current_client->balance + total_amount;
		std::cout << "Успешно внесено: " << total_amount << " руб. (всего купюр: " << total_bills_count << " шт.)" << std::endl;
		return true;
	}

	void show_cassettes_status() const { //просто, чтобы можно было увидеть что состояние кассет меняется 
		std::cout << "\nСостояние кассет банкомата" << std::endl;
		std::cout << "Номинал | Количество купюр | Свободное место" << std::endl;
		for (int i = 0; i < 6; i++) {
			std::cout << "  " << nominals[i] << " руб. |      " << cassettes[i] << "           | " << (max_cassette_capacity - cassettes[i]) << "\n";
		}
	}

	friend std::ostream& operator<< (std::ostream& out, const CashMachine& cash_machine);
};

std::ostream& operator<< (std::ostream& out, const CashMachine& cash_machine) {
	if (cash_machine.current_client == nullptr) {
		out << "Ошибка. Карта не вставлена" << std::endl;
		return out;
	}

	if (cash_machine.is_pin_verified == false) {
		out << "Ошибка. Пин код не подтвержден" << std::endl;
		return out;
	}

	out << "\nИнформация о счете через перегрузку <<" << std::endl;
	out << "Владелец: " << cash_machine.current_client->FIO << std::endl;
	out << "Текущий баланс: " << cash_machine.current_client->balance << " руб." << std::endl;

	return out;
}

int main() {
	std::setlocale(LC_ALL, "Russian");

	// 1. Создаем базу данных (Процессинговый центр)
	ProcessingCenter bank;

	// Создаем тестового клиента (10 000 рублей на балансе)
	Client client1;
	client1.card_number = "1111";
	client1.FIO = "Иванов Иван Иванович";
	client1.balance = 10000;
	client1.pin_code = "1234";
	bank.add_client(client1);

	// Создаем второго клиента для тестов (например, уже заблокированного)
	Client client2;
	client2.card_number = "2222";
	client2.FIO = "Петров Петр Петрович";
	client2.balance = 50000;
	client2.pin_code = "5555";
	client2.is_blocked = true;
	bank.add_client(client2);

	// 2. Создаем банкомат и подключаем к банку
	CashMachine atm(&bank);

	int choice;
	std::string input_str; //нужно для ввода пина и номера карты

	while (true) {
		std::cout << "\n===== МЕНЮ БАНКОМАТА =====" << std::endl;
		std::cout << "1. Вставить карту" << std::endl;
		std::cout << "2. Ввести ПИН-код" << std::endl;
		std::cout << "3. Проверить баланс (Оператор <<)" << std::endl;
		std::cout << "4. Снять наличные" << std::endl;
		std::cout << "5. Внести наличные" << std::endl;
		std::cout << "6. Вернуть карту (Выход)" << std::endl;
		std::cout << "7. Показать состояние кассет" << std::endl;
		std::cout << "0. Завершить работу программы" << std::endl;
		std::cout << "Выберите действие: ";

		std::cin >> choice;

		switch (choice) {
		case 1: 
			std::cout << "Введите номер карты (например, 1111 или 2222): ";
			std::cin >> input_str;
			atm.insert_card(input_str);
			break;

		case 2:
			std::cout << "Введите ПИН-код: ";
			std::cin >> input_str;
			atm.verify_pin(input_str);
			break;

		case 3:
			// через перегрузку
			std::cout << atm;
			break;

		case 4: {
			long long int amount;
			std::cout << "Введите сумму для снятия: ";
			std::cin >> amount;
			atm.withdraw_cash(amount);
			break;
		}

		case 5: {
			// Симулируем пачку купюр, которую пользователь сует в купюроприемник
			// Индексы: [0]-5000, [1]-2000, [2]-1000, [3]-500, [4]-200, [5]-100
			int deposit_bills[6] = { 0 };

			std::cout << "Внесите купюры по очереди:" << std::endl;
			std::cout << "Сколько купюр по 5000 руб? ----> "; std::cin >> deposit_bills[0];
			std::cout << "Сколько купюр по 2000 руб? ----> "; std::cin >> deposit_bills[1];
			std::cout << "Сколько купюр по 1000 руб? ----> "; std::cin >> deposit_bills[2];
			std::cout << "Сколько купюр по 500 руб? ----> ";  std::cin >> deposit_bills[3];
			std::cout << "Сколько купюр по 200 руб? ----> ";  std::cin >> deposit_bills[4];
			std::cout << "Сколько купюр по 100 руб? ----> ";  std::cin >> deposit_bills[5];

			atm.deposit_cash(deposit_bills);
			break;
		}

		case 6:
			atm.eject_card();
			break;

		case 7:
			atm.show_cassettes_status();
			break;

		case 0:
			std::cout << "Программа завершена. До свидания!" << std::endl;
			return 0;

		default:
			std::cout << "Неверный пункт меню!" << std::endl;
			break;
		}
	}

	return 0;
}