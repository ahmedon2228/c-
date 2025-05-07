#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>

// Базовый класс пользователя
class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel)
        : name(name), id(id), accessLevel(accessLevel) {
        if (name.empty()) {
            throw std::invalid_argument("Имя пользователя не может быть пустым");
        }
        if (id <= 0) {
            throw std::invalid_argument("ID пользователя должен быть положительным");
        }
        if (accessLevel < 0 || accessLevel > 5) {
            throw std::invalid_argument("Уровень доступа должен быть от 0 до 5");
        }
    }

    virtual ~User() = default;

    // Геттеры
    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    // Сеттеры
    void setName(const std::string& newName) {
        if (newName.empty()) throw std::invalid_argument("Имя не может быть пустым");
        name = newName;
    }

    void setAccessLevel(int level) {
        if (level < 0 || level > 5) throw std::invalid_argument("Уровень доступа должен быть от 0 до 5");
        accessLevel = level;
    }

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Имя: " << name
            << ", ID: " << id
            << ", Уровень доступа: " << accessLevel;
    }

    // Виртуальный метод для сохранения в файл
    virtual void saveToFile(std::ofstream& out) const {
        out << name << '\n' << id << '\n' << accessLevel << '\n';
    }

    // Виртуальный метод для загрузки из файла
    virtual void loadFromFile(std::ifstream& in) {
        std::getline(in, name);
        in >> id >> accessLevel;
        in.ignore(); // Пропустить символ новой строки
    }
};

// Класс студента
class Student : public User {
private:
    std::string group;

public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {
        if (group.empty()) {
            throw std::invalid_argument("Группа не может быть пустой");
        }
    }

    std::string getGroup() const { return group; }
    void setGroup(const std::string& newGroup) {
        if (newGroup.empty()) throw std::invalid_argument("Группа не может быть пустой");
        group = newGroup;
    }

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Группа: " << group << " (Студент)\n";
    }

    void saveToFile(std::ofstream& out) const override {
        out << "Student\n";
        User::saveToFile(out);
        out << group << '\n';
    }

    void loadFromFile(std::ifstream& in) override {
        User::loadFromFile(in);
        std::getline(in, group);
    }
};

// Класс преподавателя
class Teacher : public User {
private:
    std::string department;

public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {
        if (department.empty()) {
            throw std::invalid_argument("Кафедра не может быть пустой");
        }
    }

    std::string getDepartment() const { return department; }
    void setDepartment(const std::string& newDepartment) {
        if (newDepartment.empty()) throw std::invalid_argument("Кафедра не может быть пустой");
        department = newDepartment;
    }

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Кафедра: " << department << " (Преподаватель)\n";
    }

    void saveToFile(std::ofstream& out) const override {
        out << "Teacher\n";
        User::saveToFile(out);
        out << department << '\n';
    }

    void loadFromFile(std::ifstream& in) override {
        User::loadFromFile(in);
        std::getline(in, department);
    }
};

// Класс администратора
class Administrator : public User {
private:
    std::string position;

public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& position)
        : User(name, id, accessLevel), position(position) {
        if (position.empty()) {
            throw std::invalid_argument("Должность не может быть пустой");
        }
    }

    std::string getPosition() const { return position; }
    void setPosition(const std::string& newPosition) {
        if (newPosition.empty()) throw std::invalid_argument("Должность не может быть пустой");
        position = newPosition;
    }

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Должность: " << position << " (Администратор)\n";
    }

    void saveToFile(std::ofstream& out) const override {
        out << "Administrator\n";
        User::saveToFile(out);
        out << position << '\n';
    }

    void loadFromFile(std::ifstream& in) override {
        User::loadFromFile(in);
        std::getline(in, position);
    }
};

// Класс ресурса университета
class Resource {
private:
    std::string name;
    int requiredAccessLevel;

public:
    Resource(const std::string& name, int requiredAccessLevel)
        : name(name), requiredAccessLevel(requiredAccessLevel) {
        if (name.empty()) {
            throw std::invalid_argument("Название ресурса не может быть пустым");
        }
        if (requiredAccessLevel < 0 || requiredAccessLevel > 5) {
            throw std::invalid_argument("Требуемый уровень доступа должен быть от 0 до 5");
        }
    }

    std::string getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }

    void setName(const std::string& newName) {
        if (newName.empty()) throw std::invalid_argument("Название ресурса не может быть пустым");
        name = newName;
    }

    void setRequiredAccessLevel(int level) {
        if (level < 0 || level > 5) throw std::invalid_argument("Требуемый уровень доступа должен быть от 0 до 5");
        requiredAccessLevel = level;
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void displayInfo() const {
        std::cout << "Ресурс: " << name
            << ", Требуемый уровень доступа: " << requiredAccessLevel << "\n";
    }

    void saveToFile(std::ofstream& out) const {
        out << name << '\n' << requiredAccessLevel << '\n';
    }

    void loadFromFile(std::ifstream& in) {
        std::getline(in, name);
        in >> requiredAccessLevel;
        in.ignore(); // Пропустить символ новой строки
    }
};

// Шаблонный класс системы контроля доступа
template<typename T>
class AccessControlSystem {
private:
    std::vector<std::unique_ptr<User>> users;
    std::vector<T> resources;

public:
    // Добавление пользователя
    void addUser(std::unique_ptr<User> user) {
        users.push_back(std::move(user));
    }

    // Добавление ресурса
    void addResource(const T& resource) {
        resources.push_back(resource);
    }

    // Проверка доступа
    bool checkAccess(int userId, const std::string& resourceName) const {
        auto userIt = std::find_if(users.begin(), users.end(),
            [userId](const auto& user) { return user->getId() == userId; });

        auto resourceIt = std::find_if(resources.begin(), resources.end(),
            [resourceName](const auto& resource) { return resource.getName() == resourceName; });

        if (userIt == users.end() || resourceIt == resources.end()) {
            return false;
        }

        return resourceIt->checkAccess(**userIt);
    }

    // Поиск пользователя по имени
    std::vector<User*> findUsersByName(const std::string& name) const {
        std::vector<User*> result;
        for (const auto& user : users) {
            if (user->getName() == name) {
                result.push_back(user.get());
            }
        }
        return result;
    }

    // Поиск пользователя по ID
    User* findUserById(int id) const {
        auto it = std::find_if(users.begin(), users.end(),
            [id](const auto& user) { return user->getId() == id; });
        return it != users.end() ? it->get() : nullptr;
    }

    // Сортировка пользователей по уровню доступа
    void sortUsersByAccessLevel() {
        std::sort(users.begin(), users.end(),
            [](const auto& a, const auto& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
    }

    // Вывод всех пользователей
    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    // Вывод всех ресурсов
    void displayAllResources() const {
        for (const auto& resource : resources) {
            resource.displayInfo();
        }
    }

    // Сохранение данных в файл
    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) {
            throw std::runtime_error("Не удалось открыть файл для записи");
        }

        // Сохраняем пользователей
        out << users.size() << '\n';
        for (const auto& user : users) {
            user->saveToFile(out);
        }

        // Сохраняем ресурсы
        out << resources.size() << '\n';
        for (const auto& resource : resources) {
            resource.saveToFile(out);
        }
    }

    // Загрузка данных из файла
    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) {
            throw std::runtime_error("Не удалось открыть файл для чтения");
        }

        users.clear();
        resources.clear();

        // Загружаем пользователей
        int userCount;
        in >> userCount;
        in.ignore(); // Пропустить символ новой строки

        for (int i = 0; i < userCount; ++i) {
            std::string userType;
            std::getline(in, userType);

            std::unique_ptr<User> user;
            if (userType == "Student") {
                user = std::make_unique<Student>("", 0, 0, "");
            }
            else if (userType == "Teacher") {
                user = std::make_unique<Teacher>("", 0, 0, "");
            }
            else if (userType == "Administrator") {
                user = std::make_unique<Administrator>("", 0, 0, "");
            }
            else {
                throw std::runtime_error("Неизвестный тип пользователя в файле");
            }

            user->loadFromFile(in);
            users.push_back(std::move(user));
        }

        // Загружаем ресурсы
        int resourceCount;
        in >> resourceCount;
        in.ignore(); // Пропустить символ новой строки

        for (int i = 0; i < resourceCount; ++i) {
            T resource("", 0);
            resource.loadFromFile(in);
            resources.push_back(resource);
        }
    }
};

// Функция для демонстрации работы системы
void demonstrateSystem() {
    try {
        AccessControlSystem<Resource> system;

        // Добавляем пользователей
        system.addUser(std::make_unique<Student>("Иван Иванов", 1, 1, "Группа 101"));
        system.addUser(std::make_unique<Teacher>("Петр Петров", 2, 3, "Кафедра математики"));
        system.addUser(std::make_unique<Administrator>("Сергей Сергеев", 3, 5, "Начальник отдела"));

        // Добавляем ресурсы
        system.addResource(Resource("Аудитория 101", 1));
        system.addResource(Resource("Лаборатория физики", 3));
        system.addResource(Resource("Библиотека", 0));
        system.addResource(Resource("Серверная", 5));

        // Демонстрация полиморфизма
        std::cout << "Все пользователи:\n";
        system.displayAllUsers();

        std::cout << "\nВсе ресурсы:\n";
        system.displayAllResources();

        // Проверка доступа
        std::cout << "\nПроверка доступа:\n";
        std::cout << "Студент в аудитории 101: "
            << (system.checkAccess(1, "Аудитория 101") ? "Доступ разрешен" : "Доступ запрещен") << "\n";
        std::cout << "Студент в лаборатории физики: "
            << (system.checkAccess(1, "Лаборатория физики") ? "Доступ разрешен" : "Доступ запрещен") << "\n";
        std::cout << "Преподаватель в лаборатории физики: "
            << (system.checkAccess(2, "Лаборатория физики") ? "Доступ разрешен" : "Доступ запрещен") << "\n";
        std::cout << "Администратор в серверной: "
            << (system.checkAccess(3, "Серверная") ? "Доступ разрешен" : "Доступ запрещен") << "\n";

        // Поиск пользователей
        std::cout << "\nПоиск пользователя с ID 2:\n";
        User* user = system.findUserById(2);
        if (user) {
            user->displayInfo();
        }

        // Сохранение и загрузка данных
        system.saveToFile("university_system.txt");

        AccessControlSystem<Resource> newSystem;
        newSystem.loadFromFile("university_system.txt");

        std::cout << "\n\nДанные после загрузки из файла:\n";
        newSystem.displayAllUsers();
        newSystem.displayAllResources();

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

int main() {
    demonstrateSystem();
    return 0;
}