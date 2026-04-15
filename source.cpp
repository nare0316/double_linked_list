#include <iostream>

template <typename T>
struct Node {
    public:
        T val;
        Node<T>* prev;
        Node<T>* next;
    public:
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
        size_t size;
    public:
    //constructors
        list() : head(nullptr), size(0) { }
        list(size_t n, const T& val = T()) : head(nullptr), size(n) {
            if (n == 0) {
                return;
            }
            head = new Node<T>(val);
            Node<T>* cur = head;
            Node<T>* new_node;

            size_t i = 1;
            while (i < n) {
                new_node = new Node<T>(val);
                cur->next = new_node;
                new_node->prev = cur;
                cur = cur->next;
                ++i;
            }
        }

        list(const list& oth) : head(nullptr), size(0) {
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
            return *this;
        }

        list(list&& oth) noexcept : head(oth.head), size(oth.size) {
            oth.head = nullptr;
            oth.size = 0;
        }

        list& operator=(list&& oth) noexcept {
            if (this == &oth) {
                return *this;
            }
            clear();
            head = oth.head;
            size = oth.size;

            oth.head = nullptr;
            oth.size = 0;

            return *this;
        }
        list(std::initializer_list<T> init) : head(nullptr), size(0) {
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
        }

    // main functions
        void print() {
            Node<T>* cur = head;
            while (cur) {
                std::cout << *cur << " ";
                cur = cur->next;
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
            size = 0;
        }

    //operators overloading
   
    //destructor
        ~list() {
           clear();
        }

};


int main() {
    list<double> ls(4, 4.6);
    ls.print();
}

