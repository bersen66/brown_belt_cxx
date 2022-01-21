# Задание по программированию: Хеш-таблица

Теперь, когда вы знаете, как unordered-контейнеры устроены внутри, вам предлагается самостоятельно реализовать
упрощённую версию хеш-таблицы. Реализуйте шаблон HashSet:

```c++
template <typename Type, typename Hasher>
class HashSet {
public:
  using BucketList = forward_list<Type>;

public:
  explicit HashSet(
      size_t num_buckets,
      const Hasher& hasher = {}
  );

  void Add(const Type& value);
  bool Has(const Type& value) const ;
  void Erase(const Type& value);
  const BucketList& GetBucket(const Type& value) const;
};
```

Требования:

* В любой момент времени в вашей хеш-таблице должно быть ровно num_buckets корзин. Рехеширование выполнять не нужно.

* Метод Add не должен менять содержимое таблицы, если элемент, равный данному, уже есть в таблице.

* Метод Has должен возвращать true тогда и только тогда, когда элемент, равный данному, есть в таблице.

* Метод Erase должен стирать элемент, равный данному, из таблицы, если он там есть, в противном случае он не должен
  менять содержимое таблицы.

* Метод GetBucket должен возвращать ссылку на бакет, который содержал бы данный элемент, вне зависимости от того, есть
  он в таблице или нет.

## Решение задачи:

```c++

template<typename Type, typename Hasher>
class HashSet {
public:
    using BucketList = std::forward_list<Type>;
public:
    explicit HashSet(
        size_t num_buckets,
        const Hasher &hasher = {}
    ) : buckets(num_buckets), hasher(hasher){}

    void Add(const Type &value) {
        if (Has(value))
            return;
        auto& bucket = GetBucket(value);
        bucket.push_front(value);
    }

    bool Has(const Type &value) const {
        const auto& bucket = GetBucket(value);
        return std::find(bucket.begin(), bucket.end(), value) != bucket.end();
    }

    void Erase(const Type &value) {
        if (Has(value)) {
            auto& bucket = GetBucket(value);
            bucket.remove(value);
        }
    }

    const BucketList &GetBucket(const Type &value) const {
        size_t value_hash = hasher(value);
        return buckets[value_hash % buckets.size()];
    }
    
    BucketList &GetBucket(const Type &value) {
        size_t value_hash = hasher(value);
        return buckets[value_hash % buckets.size()];
    }
private:
    Hasher hasher;
    std::vector<BucketList> buckets;
};
```