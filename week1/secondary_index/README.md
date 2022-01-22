# Задание по программированию: Вторичный индекс в базе данных

Часто от хранилища данных требуется быстрый поиск по любому полю. Для этого конструируются вспомогательные структуры данных, ускоряющие поиск, они называются вторичными индексами (secondary index). Реализуйте такое хранилище для данных типа Record ниже:
```c++
struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

class Database {
public:
    bool Put(const Record& record);
    const Record* GetById(const string& id) const;
    bool Erase(const string& id);
  
    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const;
  
    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const;
  
    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const;
};
```
Требования:
* Операция Put должна возвращать true, если вставка удалась, и false, если вставка не удалась, потому что в базе данных уже есть запись с таким же id. В последнем случае состояние базы данных не должно меняться.

* Операция GetById должна возвращать nullptr, если в базе данных нет записи с указанным id.

* Операция Erase должна возвращать true, если удалось удалить элемент с заданным id, и false, если элемент с заданным id не был найден. В последнем случае состояние базы данных не должно меняться.

* Операции RangeByTimestamp, RangeByKarma и AllByUser принимают функтор callback. Подразумевается, что он имеет оператор (), принимающий в качестве аргумента объект типа Record и возвращающий bool.

* Операция RangeByTimestamp должна обходить все записи, у которых поле timestamp находится в диапазоне [low, high] (обе границы включены), и для каждой из них один раз вызывать callback. Если очередной вызов callback вернул false, итерация прекращается.

* Операция RangeByKarma должна обходить все записи, у которых поле karma находится в диапазоне [low, high].

* Операция AllByUser должна обходить все записи в хранилище данных, у которых поле user равно значению параметра user.

* Так же, как и RangeByTimestamp, методы RangeByKarma и AllByUser должны вызывать callback для каждой записи, по которой они итерируются, и прекращать итерацию, если callback вернул false.

* Параметр callback позволяет управлять процессом обхода: он должен возвращать true, если требуется продолжить обход найденных записей, и false в противном случае. Например, это позволит вывести первые 10 записей или найти первую запись, удовлетворяющую дополнительному критерию.

* Время выполнения всех операций должно иметь в среднем сублинейную (например, константную или логарифмическую) зависимость от общего числа записей в базе данных.

* Все границы интервалов - включительные, [low, high] — это наиболее универсальный интервальный запрос, подумайте, как с помощью него выразить условия вида x < high, x > low, x = x_0.

* При решении этой задачи вы можете считать, что bad_alloc кидаться не будет.
## Решение задачи: 
Для повышения читаемости кода введем обозначения

```c++
using Id = std::string;
using User = std::string;
using Karma = int;
using Timestamp = int;
```
### Устройство класса Database:

```c++
    // Построенные индексы
    std::multimap<Karma, const Record *> karma_index;   
    std::multimap<Timestamp, const Record *> timestamp_index;
    std::multimap<User, const Record *> user_index;
    
    std::unordered_map<Id, Record> records;          // Хранит записи
    std::unordered_map<Id, Locator> location_info;   // Используется для доступа к записям в индексах
```


#### Устройство структуры Locator:

Locator хранит итераторы на записи в индексах, используется в методе Erase, для того, чтобы избежать удаления лишних записей из базы данных. 

```c++
    using karma_index_iterator      = std::multimap<Karma, const Record *>::iterator;
    using timestamp_index_iterator  = std::multimap<Timestamp, const Record *>::iterator;
    using user_index_iterator       = std::multimap<User, const Record *>::iterator;

    struct Locator {
        karma_index_iterator         karma_it;
        timestamp_index_iterator     timestamp_it;
        user_index_iterator          user_it;

        Locator(karma_index_iterator karma, timestamp_index_iterator timestamp, user_index_iterator user)
                : karma_it(karma), timestamp_it(timestamp), user_it(user) {}
    };
```

### Реализация метода Put

Данный метод позволяет добавить новую запись в базу данных. Возвращает true, если вставка прошла успешно и false, если вставить объект не удалось.

#### Код:
```c++
bool Database::Put(const Record &record) {

    if (records.count(record.id) != 0) {
        return UNSUCCESS_FLAG;
    }

    auto[node_iterator, is_inserted] = records.try_emplace(record.id, record);
    
    if (is_inserted) {
        // Обновление индексов
        location_info.emplace(
                make_pair(
                          node_iterator->first,
                          Locator(
                                  karma_index.insert({node_iterator->second.karma, &node_iterator->second}),
                                  timestamp_index.insert({node_iterator->second.timestamp, &node_iterator->second}),
                                  user_index.insert({node_iterator->second.user, &node_iterator->second})
                          )
                )
        );
        return SUCCESS_FLAG;
    }
    return UNSUCCESS_FLAG;

}
```


### Реализация метода GetById

Данный метод позволяет получить указатель на запись, по ее id, в случае, если запись найти не удалось, возвращается nullptr.
#### Код:
```c++
const Record *Database::GetById(const std::string &id) const {
    auto record = records.find(id);
    if (record == records.end()) { return nullptr; }
    return &((record)->second);
}
```

### Реализация метода Erase

Удаляет запись из базы данных, если она там есть, возвращает true, если удаление прошло успешно и false, если удалить запись не получилось

#### Код:
```c++
bool Database::Erase(const std::string &id) {
    auto resolved_record = records.find(id);

    if (resolved_record == records.end())
        return UNSUCCESS_FLAG;

    auto locator_it = location_info.find(id);
    if (locator_it != location_info.end()) {
        auto&[identidier, locator] = *locator_it;

        user_index.erase(locator.user_it);
        timestamp_index.erase(locator.timestamp_it);
        karma_index.erase(locator.karma_it);
        location_info.erase(id);
        records.erase(id);
    }
    
    return SUCCESS_FLAG;
}
```
### Обход индексов
На вход методам поступают границы диапазона и колбэк, управляющий процессом обхода
Все  методы используют приватный метод IterateThrough, осуществляющий обход конкретного диапазона, задаваемого итераторами

#### Метод обхода:
```c++
    template<typename Iterator, typename Callback>
    void IterateThrough(Iterator range_begin, Iterator range_end, Callback callback) const {
        for (auto &curr = range_begin; curr != range_end && callback(*curr->second); curr++) {}
    }

```

#### Обход всех записей, у которых поле timestamp находится в диапазоне [low, high]
```c++
template<typename Callback>
void RangeByTimestamp(int low, int high, Callback callback) const {
        if (low > high) { return; }

        IterateThrough(
                timestamp_index.lower_bound(low),
                timestamp_index.upper_bound(high),
                callback
        );
}
```

#### Обход всех записей, у которых поле karma находится в диапазоне [low, high]
```c++
template<typename Callback>
void RangeByKarma(int low, int high, Callback callback) const {
    if (low > high) { return; }
            IterateThrough(
            karma_index.lower_bound(low),
            karma_index.upper_bound(high),
            callback
        );
}   
```

#### Обход всех записей, у которых поле user равно заданному
```c++
template<typename Callback>
void AllByUser(const std::string &user, Callback callback) const {

    auto[range_begin, range_end] =
        user_index.equal_range(user);
    if (range_begin == range_end && range_begin == user_index.end())
        return;
    IterateThrough(
        range_begin,
        range_end,
        callback
    );
} 
```