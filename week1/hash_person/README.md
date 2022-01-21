# Задание по программированию: Интеграция структуры Person в unordered_set


Мы хотим научиться встраивать структуру Person в контейнер unordered_set<Person, PersonHasher>. Для этого вам нужно реализовать структуры PersonHasher, AddressHasher, и операторы сравнения:
```c++
struct Address {
    string city, street;
    int building;
  
    bool operator==(const Address& other) const {
      // реализуйте оператор
    }
};

struct Person {
    string name;
    int height;
    double weight;
    Address address;
  
    bool operator==(const Person& other) const {
      // реализуйте оператор
    }
};

struct AddressHasher {
  // реализуйте структуру
};

struct PersonHasher {
  // реализуйте структуру
};
```
Требования:

* хеш-функции должны зависеть от всех полей структур

* хеш-функция должна равномерно рассеивать объекты типа Person; это её свойство проверяется тестом TestDistribution в заготовке решения.

## Решение задачи: 

```c++

struct Address {
    string city, street;
    int building;
    
    bool operator==(const Address& other) const {
        return
        std::tie(city, street, building) == std::tie(other.city, other.street, other.building);
    }
};

struct Person {
    string name;
    int height;
    double weight;
    Address address;
    
    bool operator==(const Person& other) const {
        return
        std::tie(name, height, weight, address) == 
            std::tie(other.name, other.height, other.weight, other.address);
    }
};

struct AddressHasher {
    size_t operator()(const Address& address) const {
        int base = 47;
        size_t A = std::hash<std::string>{}(address.city);
        size_t B = std::hash<std::string>{}(address.street);
        size_t C = std::hash<int>{}(address.building);
        
        // C x^3 + A * x + B
        return base * base * base * C +
               base * A +
               B;
    }
    
};


// Позволяет использовать std::hash для кастомных структур
template<>
struct std::hash<Address> {
    size_t operator()(const Address& address) const {
        AddressHasher hasher;
        return hasher(address);
    }
};
    
struct PersonHasher {
    size_t operator()(const Person& person) const {
        int base = 17;
        size_t A = std::hash<std::string>{}(person.name);
        size_t B = std::hash<Address>{}(person.address);
        size_t C = std::hash<int>{}(person.height);
        size_t D = std::hash<double>{}(person.weight);
        
        // A x^4 + B x^3 + C x + D
        return base * base * base * base * A +
               base * base * base * B +
               base * C +
               D;
    }
};

template<>
struct std::hash<Person> {
    size_t operator()(const Person& person) const {
        PersonHasher hash;
        return hash(person);
    }
};
```
