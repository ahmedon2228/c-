﻿#include <iostream>
#include <string>

class Person {
private:
    std::string name;
    int age;
    std::string email;
    std::string address;  // Новое поле

public:
    // Геттеры
    std::string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    std::string getEmail() const {
        return email;
    }

    std::string getAddress() const {  // Новый геттер
        return address;
    }

    // Сеттеры
    void setName(const std::string& newName) {
        if (!newName.empty()) {
            name = newName;
        }
        else {
            std::cerr << "Error: Name cannot be empty!" << std::endl;
        }
    }

    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 120) {
            age = newAge;
        }
        else {
            std::cerr << "Error: Age must be between 0 and 120!" << std::endl;
        }
    }

    void setEmail(const std::string& newEmail) {
        if (newEmail.find('@') != std::string::npos) {
            email = newEmail;
        }
        else {
            std::cerr << "Error: Invalid email format!" << std::endl;
        }
    }

    void setAddress(const std::string& newAddress) {  // Новый сеттер
        if (!newAddress.empty()) {
            address = newAddress;
        }
        else {
            std::cerr << "Error: Address cannot be empty!" << std::endl;
        }
    }

    // Метод для вывода информации о человеке
    void displayInfo() const {
        std::cout << "Name: " << name << ", Age: " << age
            << ", Email: " << email << ", Address: " << address << std::endl;
    }
};

int main() {
    Person person;

    // Устанавливаем значения с помощью сеттеров
    person.setName("Artem Kharitonov");
    person.setAge(19);
    person.setEmail("ahmedon2228@mail.ru");
    person.setAddress("Levanevsky 35, Rostov-on-Don");  // Устанавливаем адрес

    // Выводим информацию с помощью геттеров
    std::cout << "Name: " << person.getName() << std::endl;
    std::cout << "Age: " << person.getAge() << std::endl;
    std::cout << "Email: " << person.getEmail() << std::endl;
    std::cout << "Address: " << person.getAddress() << std::endl;  // Выводим адрес

    // Пытаемся установить некорректные значения
    person.setName(""); // Ошибка: имя не может быть пустым
    person.setAge(150); // Ошибка: возраст должен быть от 0 до 120
    person.setEmail("a``e2222.coomn"); // Ошибка: некорректный email
    person.setAddress(""); // Ошибка: адрес не может быть пустым

    // Выводим полную информацию о человеке
    person.displayInfo();

    return 0;
}