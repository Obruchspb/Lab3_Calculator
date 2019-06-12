#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stack>
#include <cmath>

//using namespace std;
const double M_PI = 3.14;

class Calculator {
	const std::string DEF = "def"; // строка для определения функции
	const std::string SET = "set"; // строка для введения переменной



	// вектор функций одного аргумента
	const std::vector<std::string> functions = {
		"sin", "cos", "tan", "cotg", "sqrt", "ln", "log"
	};

	// вектор функций двух аргументов
	const std::vector<std::string> functionsBinary = {
		"pow"
	};

	bool degrees; // в градусах ли вычисление тригонометрии

	std::vector<std::string> lexemes; // вектор лексем
	std::vector<std::string> rpn; // обратная польская запись выражения
	std::vector<double> memory; // память

	bool wasCalculated;


	void SplitToLexemes(const std::string& s); // разбивка на лексемы

	std::string CurrLexeme(); // получение текущей дексемы
	std::string NextLexeme(); // получение следующей лексемы
	void CheckLexeme(const std::string& value); // проверка на совпадение с ожидаемой лексемой

	bool IsNumber(const std::string& s) const; // проверка на число
	bool IsInteger(const std::string& s) const; // проверка на целое число
	bool IsFunction(const std::string& s) const; // проверка на функцию одного аргумента
	bool IsBinaryFunction(const std::string& s) const; // проверка на функцию двух аргументов

	void Addition(); // обработка аддитивных операций
	void Multiplying(bool isUnary = true); // обработка мультипликативных операций
	void Exponenting(bool isUnary = true); // обработка возведения в степень
	bool Entity(bool isUnary = true, bool insertUnaryMinus = true); // обработка операндов

	void ParseSet(); // обработка введения переменной
	void ParseDef(); // обработка введения функции

	double Evaluate(const std::vector<std::string>& rpn) const; // вычисление выражения, записанного в ПОЛИЗе

public:
	Calculator(bool degrees); // конструткор из режима тригонометрии
	double lastResult;

	void Calculate(const std::string& command);
};

Calculator::Calculator(bool degrees) {
	this->degrees = degrees; // запоминаем режим
}

// разбивка строки с командой на лексемы
void Calculator::SplitToLexemes(const std::string& s) {
	size_t i = 0;

	while (i < s.length()) {
		// если знак операции, скобки, запятая или []
		if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '^' || s[i] == '(' || s[i] == ')' || s[i] == ',' || s[i] == '[' || s[i] == ']') {
			lexemes.push_back(std::string(1, s[i])); // добавляем лексему операции или скобки
			i++;
		}
		else if (s[i] >= '0' && s[i] <= '9') { // если цифра
			std::string number = ""; // будущее число
			int points = 0; // количество точек

			// пока цифры или точка
			while (i < s.length() && ((s[i] >= '0' && s[i] <= '9') || (s[i] == '.'))) {
				number += s[i]; // добавляем цифру к строке с числом

				// если встретили точку
				if (s[i] == '.')
					points++; // увеличиваем число точек в числе

				if (points > 1) // если их стало слишком много
					throw std::string("incorrect real number '") + number + "'"; // бросаем исключение

				i++;
			}

			lexemes.push_back(number); // добавляем в вектор число
		}
		else if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) { // если буква
			std::string word = ""; // будущее слово

			// пока буквы или цифры
			while (i < s.length() && ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9'))) {
				word += s[i]; // добавляем букву к строке со словом
				i++;
			}

			if (word != "MW" && word != "MR" && word != "mod" && !IsFunction(word))
				throw std::string("incorrect word '") + word + "'";

			lexemes.push_back(word); // добавляем в вектор слово
		}
		else if (s[i] == ' ') { // если пробел
			i++; // пропускаем
		}
		else // неизвестный символ
			throw std::string("unknown character '") + s[i] + "' in command"; // бросаем исключение
	}
}

// получение текущей дексемы
std::string Calculator::CurrLexeme() {
	return lexemes.size() > 0 ? lexemes[0] : ""; // если есть лексемы, берём, иначе пустая строка
}

// получение следующей лексемы
std::string Calculator::NextLexeme() {
	lexemes.erase(lexemes.begin()); // удаляем первую лексему

	return CurrLexeme(); // получаем текущую
}

// проверка на совпадение с ожидаемой лексемой
void Calculator::CheckLexeme(const std::string& value) {
	if (CurrLexeme() != value)
		throw std::string("exprected '") + value + "'"; // если значения не совпали, бросаем исключение
}

// проверка на число
bool Calculator::IsNumber(const std::string& s) const {
	for (size_t i = 0; i < s.length(); i++)
		if ((s[i] < '0' || s[i] > '9') && s[i] != '.') // если какой-то символ не цифра и не точка
			return false; // значит не число

	return true; // иначе число
}

// проверка на целое число
bool Calculator::IsInteger(const std::string& s) const {
	if (s.length() == 0)
		return false;

	for (size_t i = 0; i < s.length(); i++)
		if (s[i] < '0' || s[i] > '9')
			return false;

	return true;
}

// проверка на функцию одного аргумента
bool Calculator::IsFunction(const std::string& s) const {
	for (size_t i = 0; i < functions.size(); i++)
		if (functions[i] == s)
			return true;

	return false;
}

// проверка на функцию двух аргументов
bool Calculator::IsBinaryFunction(const std::string& s) const {
	for (size_t i = 0; i < functionsBinary.size(); i++)
		if (functionsBinary[i] == s)
			return true;

	return false;
}

// обработка аддитивных операций
void Calculator::Addition() {
	Multiplying();

	while (CurrLexeme() == "+" || CurrLexeme() == "-") {
		std::string operation = CurrLexeme();
		NextLexeme();

		Multiplying(false);

		rpn.push_back(operation);
	}
}

// обработка мультипликативных операций
void Calculator::Multiplying(bool isUnary) {
	Exponenting(isUnary);

	while (CurrLexeme() == "*" || CurrLexeme() == "/" || CurrLexeme() == "mod") {
		std::string operation = CurrLexeme();
		NextLexeme();

		Exponenting(false);

		rpn.push_back(operation);
	}
}

// обработка операции возведения в степень
void Calculator::Exponenting(bool isUnary) {
	bool wasUnaryMinus = Entity(isUnary, false);

	while (CurrLexeme() == "^") {
		std::string operation = CurrLexeme();
		NextLexeme();

		Entity(false, false);

		rpn.push_back(operation);
	}

	if (wasUnaryMinus)
		rpn.push_back("!");
}

bool Calculator::Entity(bool isUnary, bool insertUnaryMinus) {
	if (CurrLexeme() == "(") { // если скобка
		NextLexeme();

		Addition(); // парсим заного выражение в скобках

		CheckLexeme(")"); // проверяем закрывающую скобку
		NextLexeme();
	}
	else if (IsNumber(CurrLexeme())) { // если число
		rpn.push_back(CurrLexeme()); // заносим его в полиз
		NextLexeme();
	}
	else if (IsFunction(CurrLexeme())) { // если функция
		std::string func = CurrLexeme();
		NextLexeme();
		CheckLexeme("(");
		NextLexeme();
		Addition();

		CheckLexeme(")");
		NextLexeme();

		rpn.push_back(func); // добавляем функцию в полиз
	}
	else if (IsBinaryFunction(CurrLexeme())) {
		std::string func = CurrLexeme();

		NextLexeme();
		CheckLexeme("(");
		NextLexeme();
		Addition(); // парсим первый аргумент

		CheckLexeme(","); // проверяем на разделитель
		NextLexeme();

		Addition(); // парсим второй аргумент

		CheckLexeme(")");
		NextLexeme();

		rpn.push_back(func); // добавляем функцию в полиз
	}
	else if (CurrLexeme() == "MR") { // если чтение из памяти
		NextLexeme();

		if (CurrLexeme() == "[") { // если с индексом
			NextLexeme();

			// проверяем, что индекс целый
			if (!IsInteger(CurrLexeme()))
				throw std::string("index must be integer number");

			rpn.push_back(CurrLexeme());
			NextLexeme();

			CheckLexeme("]");
			NextLexeme();
		}
		else {
			rpn.push_back(std::to_string(memory.size() - 1)); // иначе добавляем размер памяти - 1
		}

		rpn.push_back("MR"); // добавляем функцию в полиз
	}
	else if (isUnary && CurrLexeme() == "-") { // если унарный минус и минус
		NextLexeme();
		Entity(false); // парсим аргумент

		if (insertUnaryMinus) {
			rpn.push_back("!"); // заносим символ унарного минуса
		}

		return true;
	}
	else {
		throw std::string("symbol '") + CurrLexeme() + "' is not correct"; // иначе некорректный символ
	}

	return false;
}

// вычисление выражения, записанного в ПОЛИЗе
double Calculator::Evaluate(const std::vector<std::string>& rpn) const {
	std::stack<double> stack;

	for (size_t i = 0; i < rpn.size(); i++) {
		if (rpn[i] == "+" || rpn[i] == "-" || rpn[i] == "*" || rpn[i] == "/" || rpn[i] == "^" || rpn[i] == "mod") { // если операция
			// получаем аргументы из стека
			double arg2 = stack.top();
			stack.pop();
			double arg1 = stack.top();
			stack.pop();

			// и выполняем операцию
			if (rpn[i] == "+") {
				stack.push(arg1 + arg2);
			}
			else if (rpn[i] == "-") {
				stack.push(arg1 - arg2);
			}
			else if (rpn[i] == "*") {
				stack.push(arg1 * arg2);
			}
			else if (rpn[i] == "/") {
				if (arg2 == 0)
					throw std::string("division by zero");

				stack.push(arg1 / arg2);
			}
			else if (rpn[i] == "^") {
				stack.push(pow(arg1, arg2));
			}
			else if (rpn[i] == "mod") {
				stack.push(fmod(arg1, arg2));
			}
		}
		else if (rpn[i] == "!") { // если унарный минус
			stack.top() *= -1; // меняем знак у числа на верхушке стека
		}
		else if (IsFunction(rpn[i])) {
			double arg = stack.top();
			stack.pop();

			if (rpn[i] == "sin" || rpn[i] == "cos" || rpn[i] == "tan" || rpn[i] == "cotg") {
				// если градусы, то переводим в радианы
				if (degrees)
					arg = M_PI / 180 * arg;

				if (rpn[i] == "sin") {
					stack.push(sin(arg));
				}
				else if (rpn[i] == "cos") {
					stack.push(cos(arg));
				}
				else if (rpn[i] == "tan") {
					stack.push(tan(arg));
				}
				else if (rpn[i] == "cotg") {
					stack.push(1.0 / tan(arg));
				}
			}
			else if (rpn[i] == "sqrt") {
				stack.push(sqrt(arg));
			}
			else if (rpn[i] == "log") {
				stack.push(log(arg) / log(10)); // log(x) = ln(x) / ln(10)
			}
			else if (rpn[i] == "ln") {
				stack.push(log(arg));
			}
			else
				throw std::string("unhandled function '") + rpn[i] + "'";
		}
		else if (IsBinaryFunction(rpn[i])) {
			// получаем аргументы из стека
			double arg2 = stack.top();
			stack.pop();
			double arg1 = stack.top();
			stack.pop();

			if (rpn[i] == "pow") {
				stack.push(pow(arg1, arg2));
			}
			else
				throw std::string("unhandled function '") + rpn[i] + "'";
		}
		else if (rpn[i] == "MR") {
			int index = stack.top();
			stack.pop();

			if (index < 0 || index >= memory.size())
				throw std::string("invalid index for reading memory");

			stack.push(memory[index]);
		}
		else { // число, кидаем его в стек
			stack.push(stod(rpn[i])); // то кидаем его в стек
		}
	}

	if (stack.size() != 1)
		throw std::string("error during computation expression");

	return stack.top();
}

// выполнение команды
void Calculator::Calculate(const std::string& command) {
	lexemes.clear();
	rpn.clear();

	SplitToLexemes(command);

	if (lexemes.size() == 0)
		throw std::string("Command is invalid");

	// если запись в память
	if (lexemes[0] == "MW") {
		if (lexemes.size() == 1) { // если только MW
			if (!wasCalculated) // если не было вычислений
				throw std::string("no expression was evaluated yet"); // брочаем исключеине

			memory.push_back(lastResult); // добалвляем в память вычисленное значение
		}
		else if (lexemes.size() == 4 && lexemes[1] == "[" && lexemes[3] == "]" && IsInteger(lexemes[2])) { // если MW[index]
			int index = stoi(lexemes[2]); // получаем индекс

			// проверяем его значение
			if (index < 0 || index > memory.size())
				throw std::string("invalid index for write to memory");

			// проверяем, было ли вычислено значение
			if (!wasCalculated)
				throw std::string("no expression was evaluated yet");

			// кладём в память
			if (index < memory.size()) {
				memory[index] = lastResult;
			}
			else {
				memory.push_back(lastResult);
			}
		}
		else {
			throw std::string("incorrect type for MW command");
		}
	}
	else {
		Addition(); // иначе парсим выражение

		if (lexemes.size() > 0)
			throw std::string("incorrect expression");

		lastResult = Evaluate(rpn); // вычисляем его
		wasCalculated = true;
		//std::cout << std::setprecision(10) << lastResult << std::endl; // и выводим результат
	}
}