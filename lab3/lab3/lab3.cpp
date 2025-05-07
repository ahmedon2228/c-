#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& n, int d) : name(n), damage(d) {}

    // Перегрузка оператора + (сложение урона оружия)
    Weapon operator+(const Weapon& other) const {
        std::string newName = name + " + " + other.name;
        int newDamage = damage + other.damage;
        return Weapon(newName, newDamage);
    }

    // Перегрузка оператора > (сравнение по урону)
    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    // Перегрузка оператора << для вывода информации об оружии
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.name << ", Damage: " << weapon.damage;
        return os;
    }
};

int main() {
    Weapon sword("Sword", 50);
    Weapon bow("Bow", 40);
    Weapon axe("Axe", 60);

    // Демонстрация оператора +
    Weapon combined = sword + bow;
    std::cout << combined << std::endl;  // Вывод: Weapon: Sword + Bow, Damage: 90

    // Демонстрация оператора >
    std::cout << std::boolalpha;  // Для вывода true/false вместо 1/0
    std::cout << "Is sword stronger than bow? " << (sword > bow) << std::endl;      // true
    std::cout << "Is bow stronger than axe? " << (bow > axe) << std::endl;          // false
    std::cout << "Is axe stronger than combined? " << (axe > combined) << std::endl; // false

    // Создадим еще одно комбинированное оружие
    Weapon superWeapon = combined + axe;
    std::cout << superWeapon << std::endl;  // Weapon: Sword + Bow + Axe, Damage: 150

    return 0;
}