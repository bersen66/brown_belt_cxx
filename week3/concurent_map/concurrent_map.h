//
// Created by ortur on 08.02.2022.
//

#ifndef BROWN_BELT_CONCURRENT_MAP_H
#define BROWN_BELT_CONCURRENT_MAP_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <mutex>

template <typename K, typename V, typename Hash = std::hash<K>>
class ConcurrentMap {
public:
    using MapType = std::unordered_map<K, V, Hash>;

    struct WriteAccess {
        std::lock_guard<std::mutex> guard;
        V& ref_to_value;

    };

    struct ReadAccess {
        std::lock_guard<std::mutex> guard;
        const V& ref_to_value;
    };

public:
    explicit ConcurrentMap(size_t bucket_count)
        : buckets_(bucket_count) {}

    WriteAccess operator[](const K& key) {
        return GetBucket(key).GetValue(key);
    }

    ReadAccess At(const K& key) const {
        return GetBucket(key).GetValue(key);
    }

    bool Has(const K& key) const {
        return GetBucket(key).Has(key);
    }

    MapType BuildOrdinaryMap() const {
        MapType result;
        for (const auto& bucket : buckets_) {
            std::lock_guard<std::mutex> guard(bucket.mutex);
            result.insert(bucket.map.begin(), bucket.map.end());
        }

        return result;
    }


private:
    struct Bucket {
        MapType map;
        mutable std::mutex mutex;

        WriteAccess GetValue(const K& key) {
            return {std::lock_guard(mutex), map[key]};
        }

        ReadAccess GetValue(const K& key) const {
            return {std::lock_guard(mutex), map.at(key)};
        }

        bool Has(const K& key) const {
            return map.count(key);
        }
    };

    Bucket& GetBucket(const K& key) {
        return buckets_[hasher(key) % buckets_.size()];
    }

    const Bucket& GetBucket(const K& key) const {
        return buckets_[hasher(key) % buckets_.size()];
    }
private:
    Hash hasher;
    std::vector<Bucket>  buckets_;
};

#endif //BROWN_BELT_CONCURRENT_MAP_H
