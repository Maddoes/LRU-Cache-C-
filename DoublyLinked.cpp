#include <iostream>
#include <vector>

enum class STATE{
    OCCUPIED,
    EMPTY,
    DEAD
};

template <typename Key, typename Value>
struct Slot{
    Key key{};
    Value value{};
    STATE state = STATE::EMPTY;

    Slot(){}

    Slot(Key key_, Value value_, STATE state_ = STATE::OCCUPIED): state(state_), key(key_), value(value_){}
};

template <typename K, typename V>
class HashMap{

    private:
        std::vector<Slot<K, V>> map;
    public:

        int capacity, count = 0, dead = 0;

        HashMap(int capacity = 8) : capacity(capacity){
            map.resize(capacity);
        }

        int get_index(K key){
            return std::hash<K>{}(key) % capacity;
        }
        void Resize(){
            if(count >= capacity * 0.75){
                int new_capacity = capacity * 2;
                std::cout<<"Resizing " << capacity<< " to "<< new_capacity << "\n";
                std::vector<Slot<K,V>> oldmap = map;
                std::vector<Slot<K,V>> newmap;
                
                newmap.resize(new_capacity);
                map = std::move(newmap);

                for(auto& item: oldmap){
                        Slot<K, V> empty_slot{};

                        int index = get_index(item.key);
                        int first_index = index;
                        bool valid_slot = true;

                        while(map[index].state == STATE::OCCUPIED){
                            index = (index + 1) % new_capacity;
                            if (index == first_index){
                                    valid_slot = false;
                                break;
                            }
                        }
                        if(valid_slot){
                            if(item.state == STATE:: OCCUPIED){
                                map[index] = item;
                            }
                            else{
                                map[index] = empty_slot;
                            }
                        }
                    }
                }
        }
        //break insert up in 1 smaller one and one bigger one
        void Insert(K k, V v){
            //Check if map full
            Resize();
            //Create new Slot to insert into vector
            Slot<K, V> slot(k,v);
            
            //get the index by hashing the key 
            int index = get_index(k);
            int first_index = index;
            bool valid_spot = true;

            while(map[index].state == STATE:: OCCUPIED){
                if(map[index].key == k && map[index].state != STATE::DEAD){
                    count -= 1;
                    break;
                }
                index = (index + 1) % capacity;
                if(index == first_index){
                    valid_spot = false;
                    break;
                }
            }
            if(valid_spot){
                map[index] = slot;
                count += 1;
            }
        }

        void Delete(K key){
            int index = get_index(key);
            int first_index = index;
            bool found = true; 

            while(map[index].key != key){
                index = (index + 1) % capacity;
                if(first_index == index){
                    found = false;
                    break;
                }
                if(map[index].state != STATE::OCCUPIED){
                    index = (index + 1) % capacity;
                }
            }
            if(found){
                map[index].state = STATE::DEAD;
                dead += 1;
                count -= 1;
            }
            else{
                std::cout<<"Item not found \nNothing deleted \n";
            }
        }
        V Find(K key){
            bool found = true;
            int index = get_index(key);
            int first_index = index;

            while(map[index].key != key){
                index = (index + 1) % capacity;
                if(map[index].state != STATE:: OCCUPIED){
                    continue;
                }
                if(index == first_index){
                    found = false;
                    break;
                }
            }
            if(found){
                if(map[index].state == STATE::OCCUPIED){
                    return map[index].value;
                }
                else{
                    return nullptr;
                }
            }
            else{
                return nullptr;
            }
        }
};

template <typename Key, typename Value>
struct Node{
    Node* prev;
    Node* next;
    Key key;
    Value value;
    Node(){
    }
    Node(Key _key, Value _value, Node* _next = nullptr, Node* _prev = nullptr ) 
    : prev(_prev), next(_next), key(_key), value(_value){
    }
    
};

template<typename Key, typename Value>
class LinkedList{
    public:
        int capacity;
        Node<Key, Value>* current;
        Node<Key, Value>* head = nullptr;
        Node<Key, Value>* tail = nullptr;  
    LinkedList(int _capacity = 3): capacity(_capacity){

    }

    void InsertHead(Key key, Value value){
        Node<Key, Value>* cur_node = new Node<Key, Value>(key, value);
        current = cur_node;

        if(head == nullptr){
            head = cur_node;
            tail = cur_node;
        }
        else{
            head->prev = cur_node;
            cur_node->next = head;
            head = cur_node;
        }
    }

    void RemoveNode(Node<Key, Value>* cur_node){
        //after lookup in the hashmap         
        if(cur_node->next != nullptr){
            cur_node->next->prev = cur_node->prev;
            cur_node->prev->next = cur_node->next;
        }
        else if(cur_node->prev != nullptr){
            tail = cur_node->prev;
            cur_node->prev->next = nullptr;

        } 
    } 
    
    void put(Key key, Value value){
        Node<Key, Value>* cur_node = new Node<Key, Value>(key, value);
        current = cur_node;
        if(head == nullptr){
            head = cur_node;
            tail = cur_node;
        }
        else{
            head->prev = cur_node;
            cur_node->next = head;
            head = cur_node;                 
        }
    }
};

//Explanation LRU cache
//When put is run, create a node pointer, 
//That Node is put into the double linked list with Key and Value and pointer to prev and next
//Then Hashmap stores the same Key with the ptr to Node
//When put run again, create a node1 pointer, 
//Node is put into the linked list at the head, node b ecomes the tail node 1 is the header
//Hashmap stores key with node1 ptr
//when Used is run on node,
//Node becomes head, node1 becomes tail
//Find the Node by the key in the hash, then from the node ptr get the valuje
//Hashmap stays the same



template <typename Key, typename Value>
class LRUCache{
    private:
        int count = 0, capacity;
        
        HashMap<Key, Node<Key, Value>*> map{};
        LinkedList<Key, Value> linkedlist{};
    public:

        LRUCache(int _capacity = 3): capacity(_capacity){
        }

        void delete_tail(){
            if(count == linkedlist.capacity)
        }
        void put(Key key, Value value){
            int search = map.Find(key);
            if(search == -1){
                //This is put
            }
            else{
                //This is get
                
            }
            linkedlist.put(key, value);
            count += 1;
            map.Insert(key, linkedlist.current);
        }
};

int main(){


    return 0;
}