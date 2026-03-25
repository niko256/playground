#include <iostream>
#include <list>
#include <optional>
#include <unordered_map>

template <typename Key, typename Value>
class LruCache {
  public:
    /* { key, value } */
    using Node = std::pair<Key, Value>;
    using ListIter = typename std::list<Node>::iterator;

  private:
    size_t capacity_;
    std::list<Node> cache_list_; /* head = new, tail = old values */
    std::unordered_map<Key, ListIter> cache_map_;

  public:
    LruCache(size_t capacity)
        : capacity_(capacity) {}

    std::optional<Value> get(Key key) {
        if (cache_map_.find(key) == cache_map_.end()) {
            return std::nullopt;
        }

        // если элемент находится в кэше, то
        // необходимо пометить его как недавно использованный
        // и вернуть его
        ListIter it = cache_map_[key];

        // перемещаем элемент, на который указывает it в начало списка
        // splice : "куда" -> "откуда" -> "что"
        cache_list_.splice(cache_list_.begin(), cache_list_, it);

        return it->second;
    }

    void put(Key key, Value value) {
        // #1 ключ уже присутствует => перезаписываем значение
        // и помечаем значение как недавно использованное
        // => splice в cache_list_
        if (cache_map_.find(key) != cache_map_.end()) {
            ListIter it = cache_map_[key];

            it->second = std::move(value);

            cache_list_.splice(cache_list_.begin(), cache_list_, it);

            return;
        }

        /* #2 */
        if (cache_list_.size() == capacity_) {
            auto deletion = cache_list_.back().first;

            cache_list_.pop_back();
            cache_map_.erase(deletion);
        }

        cache_list_.push_front({key, value});

        cache_map_[key] = cache_list_.begin();
    }
};

int main() {
    LruCache<int, std::string> lru(4);

    lru.put(1, "first");
    lru.put(2, "second");
    lru.put(3, "third");
    lru.put(4, "fourth");

    std::cout << lru.get(1).value() << " " << lru.get(2).value() << " " << lru.get(3).value()
              << " " << lru.get(4).value() << std::endl;

    lru.put(5, "fifth");
    std::cout << lru.get(1).value_or("this element was evicted") << std::endl;

    lru.put(6, "sixth");
    std::cout << lru.get(2).value_or("this element was evicted") << std::endl;

    return 0;
}
