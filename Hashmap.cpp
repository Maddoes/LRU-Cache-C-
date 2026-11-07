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
                std::cout<<"Resizing " << capacity<< " to"<< new_capacity << "\n";
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
                if(map[index].state != STATE::OCCUPIED){
                    index = (index + 1) % capacity;
                    }
                if(first_index == index){
                    found = false;
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
                    return -1;
                }
            }
            else{
                return -1;
            }
        }
};

int main(){

    HashMap<int, int> hash{10};
    hash.Insert(0, 10);
    hash.Insert(0, 20);
    hash.Insert(1, 30);
    hash.Insert(2,40);

    return 0;
}