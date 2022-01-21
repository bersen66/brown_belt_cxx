# Тренировочное задание по программированию: Интеграция структуры Point в unordered_set

Мы хотим научиться встраивать структуру Point3D в контейнер unordered_set<Point3D, Hasher>. Для этого вам нужно реализовать структуру Hasher и оператор сравнения: 

```c++
using CoordType = int;

struct Point3D {
CoordType x;
CoordType y;
CoordType z;

bool operator==(const Point3D& other) const {
// реализуйте оператор
}
};

struct Hasher {
// реализуйте структуру
};
```


## Решение задачи: 

```c++

using CoordType = int;

struct Point3D {
    CoordType x;
    CoordType y;
    CoordType z;

    // Оператор сравнения
    bool operator==(const Point3D& other) const {
        return std::tie(x, y, z) == std::tie(other.x, other.y, other.z);
    }
};

// Хэш-функция
struct Hasher {

    std::size_t operator()(const Point3D& point)  const {
        int base = 37;
        std::hash<CoordType> hash;
        size_t A = std::hash<CoordType>{}(point.x);
        size_t B = std::hash<CoordType>{}(point.y);
        size_t C = std::hash<CoordType>{}(point.z);
        return base * base * base * base * C + base * base * base * A + B;
    }
};
```