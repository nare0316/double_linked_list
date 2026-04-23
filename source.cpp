#include <iostream>
#include <type_traits>

template <typename T>
T&& forward(std::remove_reference_t<T>& arg) {
    return static_cast<T&&>(arg);
}

template <typename T>
T&& forward(std::remove_reference_t<T>&& arg) {
    static_assert(!std::is_lvalue_reference_v<T>);
    return static_cast<T&&>(arg);
}

template <typename T>
struct Node {
    public:
        T val;
        Node<T>* prev;
        Node<T>* next;
    public:
        template <typename ... Types>
        Node(Types ... args) : val(std::forward<Types>(args)...), prev(nullptr), next(nullptr) {}
        Node() : val(), prev(nullptr), next(nullptr) { }          
        Node(const T& val) : val(val), prev(nullptr), next(nullptr) { }  
        ~Node() = default;
        friend std::ostream& operator<<(std::ostream& os, const Node& node) {
            os << node.val;
            return os;
        }
};


template <typename T>
class list {
    public:
        Node<T>* head;
        Node<T>* tail;
        size_t size;
    public:
    //constructors
        list() : head(nullptr), tail(nullptr), size(0) { }
        list(size_t n, const T& val = T()) : head(nullptr), tail(nullptr), size(n) {
            if (n == 0) {
                return;
            }
            Node<T>* new_node = new Node<T>(val);
            head = new_node;

            Node<T>* cur = head;
            size_t i = 1;
            while (i < n) {
                new_node = new Node<T>(val);
                cur->next = new_node;
                new_node->prev = cur;
                cur = cur->next;
                ++i;
            }
            tail = new_node;
        }

        list(const list& oth) : head(nullptr), tail(nullptr), size(0) {
            if (!oth.head) {
                return;
            }
        
            Node<T>* new_node;
            Node<T>* tmp = oth.head;
            head = new Node<T>(tmp->val);
            size = 1;

            Node<T>* cur = head;
            tmp = tmp->next;

            while (tmp) {
                new_node = new Node<T>(tmp->val);
                cur->next = new_node;
                new_node->prev = cur;
                cur = cur->next;
                tmp = tmp->next;
                ++size;
            }
            tail = new_node;
        }

        list& operator=(const list& oth) {
            if (this == &oth) {
                return *this;
            }
            clear();
            if (!oth.head) {
                return *this;
            }
           
            Node<T>* new_node;
            Node<T>* tmp = oth.head;
            head = new Node<T>(tmp->val);
            size = 1;

            Node<T>* cur = head;
            tmp = tmp->next;

            while (tmp) {
                new_node = new Node<T>(tmp->val);
                cur->next = new_node;
                new_node->prev = cur;
                cur = cur->next;
                tmp = tmp->next;
                ++size;
            }
            tail = new_node;
            return *this;
        }

        list(list&& oth) noexcept : head(oth.head), tail(oth.tail), size(oth.size) {
            oth.head = nullptr;
            oth.tail = nullptr;
            oth.size = 0;
        }

        list& operator=(list&& oth) noexcept {
            if (this == &oth) {
                return *this;
            }
            clear();
            head = oth.head;
            tail = oth.tail;
            size = oth.size;

            oth.head = nullptr;
            oth.tail = nullptr;
            oth.size = 0;

            return *this;
        }
        list(std::initializer_list<T> init) : head(nullptr), tail(nullptr), size(0) {
            Node<T>* cur = nullptr;
            Node<T>* new_node;

            for (const T& val : init) {
                new_node = new Node<T>(val);
                if (cur == nullptr) {
                    head = new_node;
                    cur = head;
                } else {
                    cur->next = new_node;
                    new_node->prev = cur;
                    cur = cur->next;
                }
                ++size;
            }
            tail = new_node;
        }

    // main functions
        void print() const {
            Node<T>* cur = head;
            while (cur) {
                std::cout << *cur << " ";
                cur = cur->next;
            }
            std::cout << std::endl;
        }

        void print_reverse() const {
            Node<T>* cur = tail;
            while (cur) {
                std::cout << *cur << " ";
                cur = cur->prev;
            }
            std::cout << std::endl;
        }

        void clear() {
            Node<T>* tmp = head;
            while(head) {
                tmp = head;
                head = head->next;
                delete tmp; 
            }
            head = nullptr;
            tail = nullptr;
            size = 0;
        }

        void clear_from(Node<T>* start_node) {
            if (!start_node) {
                return;
            }
            if (start_node == head) {
                head = tail = nullptr;
            } else {
                tail = start_node->prev;
                tail->next = nullptr;
            }
            Node<T>* tmp = start_node;
            while(start_node) {
                tmp = start_node;
                start_node = start_node->next;
                delete tmp; 
                --size;
            }
        }

        T& front() const {
            if (!head) { 
                throw std::runtime_error("empty list");
            }
            return head->val;
        }

        T& back() const {
            if (!head) { 
                throw std::runtime_error("empty list");
            }
            return tail->val;
        }

        bool empty()  const {
            return size == 0;
        }

        size_t len() {
            return size;
        }

        void push_back(const T& val) {
            Node<T>* new_node = new Node<T>(val);
            if (!head) {
                head = new_node;
                tail = new_node;
            } else {
                new_node->prev = tail;
                tail->next = new_node;
                tail = new_node;
            }
            ++size;
        }

        void pop_back() {
            if (size == 0) {
                return;
            }
            if (size == 1) {
                delete head;
                head = tail = nullptr;
            } else {
                tail = tail->prev;
                delete tail->next;
                tail->next = nullptr;
            }
            --size;
        }

        void insert(int pos, const T& val) {
            if (!head) {
                push_back(val);
                return;
            }
            Node<T>* new_node = new Node<T>(val);
            Node<T>* cur = head;
            if (pos <= 0) {
                head->prev = new_node;
                new_node->next = head;
                head = new_node;
            } else if (pos >= size) {
                new_node->prev = tail;
                tail->next = new_node;
                tail = new_node;
            } else {
                while(pos--) {
                    cur = cur->next;
                }
                new_node->prev = cur->prev;
                new_node->next = cur;
                cur->prev->next = new_node;
                cur->prev =new_node;
            }
            ++size;
        }
        void erase(int pos) {
            if (size == 0) {
                return;
            }
            if (pos < 0 || pos >= size) {
                throw std::range_error("Out of range\n");
            }
            if (size == 1) {
                pop_back();
                return;
            }
            Node<T>* cur = head;
            if (pos == 0) {
                head = head->next;
                delete head->prev;
                head->prev = nullptr;
            } else if (pos == size - 1) {
                tail = tail->prev;
                delete tail->next;
                tail->next = nullptr;
            } else {
                while (pos--) {
                    cur = cur->next;
                }
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                delete cur;
            }
            --size;
        }

        void push_front(const T& val) {
            Node<T>* new_node = new Node<T>(val);
            if (size == 0) {
                head = tail = new_node;
            } else {
                new_node->next = head;
                head->prev = new_node;
                head = new_node;
            }
            ++size;
        }
        
        void pop_front() {
            if (size == 0) {
                return;
            }
            if (size == 1) {
                delete head;
                head = tail = nullptr;
            } else {
                head = head->next;
                delete head->prev;
                head->prev = nullptr;
            }
            --size;
        }

        void resize(size_t count) {
            if (count == size) {
                return;
            }
            if (size == 0) {
                for (size_t i = 0; i < count; ++i)
                    push_back(T());
                return;
            }
            Node<T>* cur = head;
            if (count < size) {
                while(count--) {
                    cur = cur->next;
                }
                clear_from(cur);
            } else {
                Node<T>* new_node;
                cur = tail;
                for (size_t i{0}; i < count - size; ++i) {
                    new_node = new Node<T>();
                    new_node->prev = cur;
                    cur->next = new_node;
                    cur = cur->next;
                }
                size = count;
                tail = new_node;
            }
        }
        template <typename ... Types>
        void emplace_front(Types&& ... args) {
            Node<T> * new_node = new Node<T>(forward<Types>(args)...);
            if (size == 0) {
                head = tail = new_node;
            } else {
                new_node->next = head;
                head->prev = new_node;
                head = new_node;
            }
            ++size;
        }
        template <typename ... Types>
        void emplace_back(Types&& ... args) {
            Node<T> * new_node = new Node<T>(forward<Types>(args)...);
            if (!head) {
                head = new_node;
                tail = new_node;
            } else {
                new_node->prev = tail;
                tail->next = new_node;
                tail = new_node;
            }
            ++size;
        }

        template <typename ... Types>
        void emplace(int pos, Types&& ... args) {
            if (!head) {
                emplace_back(std::forward<Types>(args)...);
                return;
            }
        
            if (pos <= 0) {
                emplace_front(std::forward<Types>(args)...);
                return;
            }
        
            if (pos >= size) {
                emplace_back(std::forward<Types>(args)...);
                return;
            }
        
            Node<T>* cur = head;
            for (int i = 0; i < pos; ++i) {
                cur = cur->next;
            }
        
            Node<T>* new_node = new Node<T>(std::forward<Types>(args)...);
        
            new_node->prev = cur->prev;
            new_node->next = cur;
        
            cur->prev->next = new_node;
            cur->prev = new_node;
        
            ++size;
        }
       

    //destructor
        ~list() {
           clear();
        }

};


int main() {
    list<int> ls = {'a', 'b', 'c'};
    ls.front() = 'p';
    assert(ls.front() == 'p');
    assert(ls.back() == 'c');
    assert(!ls.empty());
    assert(ls.len() == 3);
    ls.clear();
    assert(ls.empty());
    assert(ls.len() == 0);
    ls.insert(0, 'o');
    ls.insert(2, 'f');
    ls.insert(9, 'g');
    ls.insert(-2, 'k');
    ls.insert(0, 's');
    ls.insert(3, 'n');
    ls.emplace(0, 's');
    assert(ls.len() == 7);
    ls.erase(0);
    assert(ls.len() == 6);
    ls.push_front('j');
    ls.push_back('w');
    ls.emplace_back('j');
    ls.emplace_back('w');
    ls.emplace_front('a');
    ls.emplace_front('b');
    ls.pop_back();
    ls.pop_back();
    ls.pop_back();
    assert(ls.len() == 9);
    ls.resize(6);
    ls.resize(10);
    assert(ls.len() == 10);
    std::cout << "Test passed.\n";
}

