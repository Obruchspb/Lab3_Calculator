#include <iostream>
#include <string>

#include "Calculator.hpp"

//using namespace std;

int main() {
	std::string mode;

	std::cout << "Welcome to CALCULATOR!" << std::endl;
	std::cout << "Enter trigonometry mode (1 - degrees, 2 - radians): ";
	std::getline(std::cin, mode);

	while (mode != "1" && mode != "2") {
		std::cout << "Incorrect mide. try again: ";
		std::getline(std::cin, mode);
	}

	std::cout << "Please, type your commands after '>' and press 'Enter'" << std::endl;
	std::cout << "Type 'quit' for quit" << std::endl;

	Calculator calculator(mode == "1");

	do {
		std::string command; // строка для считывания команды
		std::cout << ">"; // приглашение ко вводу
		std::getline(std::cin, command); // считываем строку-команду

		// если пустая строка, то выходим
		if (command == "quit")
			break;

		try {
			calculator.Calculate(command);
		}
		catch (std::string error) {
			std::cout << "error: " << error << std::endl;
		}
	} while (1);
}