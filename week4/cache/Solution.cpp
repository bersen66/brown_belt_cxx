#include "Common.h"
#include <mutex>
#include <deque>
#include <unordered_map>
#include <list>

using namespace std;

class LruCache : public ICache {
public:

    LruCache(
            shared_ptr<IBooksUnpacker> books_unpacker,
            const Settings& settings
    ) : books_unpacker_(books_unpacker), settings_(settings) {}

    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> g(mutex_);

        if (Contains(book_name)) {
            BoostElementRating(book_name);
        } else {

            auto book = move(books_unpacker_->UnpackBook(book_name));
            size_t book_size = book->GetContent().size();

            if (IsEnoughMemoryForAdding(book_size)) {
                AddBook(move(book));
            } else {

                if (book_size > settings_.max_memory) {
                    return nullptr;
                }

                DeleteLastRecentlyUsedForAdding(book_size);
                AddBook(move(book));
            }
        }
        return cached_books_.front();
    }
private:

    bool Contains(const string& book_name) const {
        return cache_locators_.find(book_name) != cache_locators_.end();
    }

    bool IsEnoughMemoryForAdding(size_t adding_size) const {
        return settings_.max_memory >= memory_used_ + adding_size;
    }

    void AddBook(unique_ptr<IBook> book) {
        const string& book_name = book->GetName();
        if (!IsEnoughMemoryForAdding(book->GetContent().size()))
            return;
        cached_books_.push_front(move(book));
        cache_locators_[book_name] = cached_books_.begin();
        memory_used_ += cached_books_.front()->GetContent().size();
    }

    void DeleteLastRecentlyUsedForAdding(size_t needed_space) {
        if (needed_space > settings_.max_memory)
            throw runtime_error("too big");
        while (settings_.max_memory - memory_used_ < needed_space) {
            DeleteLastUsed();
        }
    }

    void DeleteLastUsed()  {
        if (cached_books_.empty() && cache_locators_.empty())
            return;
        auto last_used = cached_books_.back();
        cache_locators_.erase(last_used->GetName());
        memory_used_ -= last_used->GetContent().size();
        cached_books_.pop_back();
    }

    void BoostElementRating(const string& book_name) {
        if (cached_books_.empty() || cache_locators_.empty())
            return;
        auto entry_it = cache_locators_.find(book_name)->second;
        cached_books_.push_front(move(*entry_it));
        cached_books_.erase(entry_it);
        cache_locators_[book_name] = cached_books_.begin();
    }

private:
    mutable list< BookPtr > cached_books_;
    using ListIterator = list<BookPtr>::iterator;
    mutable unordered_map<string, ListIterator> cache_locators_;


    size_t memory_used_ = 0;
    shared_ptr<IBooksUnpacker> books_unpacker_;
    const Settings settings_;
    mutex mutex_;
};


unique_ptr<ICache> MakeCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const ICache::Settings& settings
) {
    return make_unique<LruCache>(books_unpacker, settings);
}