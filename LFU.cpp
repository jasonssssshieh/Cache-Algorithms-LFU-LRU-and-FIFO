class Node{
public:
    Node* pre;
    Node* next;
    int frq;
    int key;
    unordered_map<int, list<int>::iterator> KeyMap;
    list<int> KeyList;//time-ordered, first -> end, new -> old;
    Node(int frq, int key, Node* pre, Node* next){
        this->frq = frq;
        this->key = key;
        this->pre = pre;
        this->next = next;
        KeyList.insert(KeyList.begin(), key);
        KeyMap[key] = KeyList.begin();
    }
};
class LFUCache{
    Node* head = nullptr;
    unordered_map<int, int> valueMap;// key, value
    unordered_map<int, Node*> nodeMap;//key，Node*
    int _size;
public:
    LFUCache(int capacity){
        this->_size = capacity;
    }
    int get(int key){
        if(valueMap.find(key) == valueMap.end()) return -1;
        UpdateKey(key);
        return valueMap[key];
    }
    void set(int key, int value){
        if(_size<=0) return;
        if(valueMap.find(key) != valueMap.end()){
            valueMap[key] = value;
        }else{
            if(valueMap.size() == _size){popOldNode();}
            valueMap[key] = value;
            addNewNode(key);
        }
        UpdateKey(key);
    }
    void RemoveKeyfromNode(Node* node, int key){
        auto itr = node->KeyMap[key];//找到在当前frq下的node的排序
        node->KeyList.splice(node->KeyList.end(), node->KeyList, itr);//把这个node置换到最后面，
        node->KeyList.pop_back();//弹出
        node->KeyMap.erase(key);
    }
    void InsertKeyToNode(Node* node, int key){
        node->KeyList.insert(node->KeyList.begin(), key);
        node->KeyMap[key] = node->KeyList.begin();
    }
    void UpdateKey(int key){
        Node* node = nodeMap[key];
        RemoveKeyfromNode(node, key);
        if(node->next == nullptr){
            Node* tmp = new Node(node->frq + 1, key, node, node->next);
            node->next = tmp;
        }else if(node->next->frq > node->frq + 1){
            Node* tmp = new Node(node->frq + 1, key, node, node->next);
            node->next->pre = tmp;
            node->next = tmp;
        }else{//node->next->frq == node->frq + 1
            InsertKeyToNode(node->next, key);
        }
        nodeMap[key] = node->next;
        if(node->KeyMap.size() == 0) removeNode(node);
    }
    void popOldNode(){
        if(head == nullptr) return;
        if(head->KeyMap.empty()) head = head->next;
         int n = head->KeyList.back();
         RemoveKeyfromNode(head, n);
         nodeMap.erase(n);
         valueMap.erase(n);
         if(head->KeyMap.size() == 0) removeNode(head);
    }
    void addNewNode(int key){
        if(head == nullptr){
            Node* tmp = new Node(0, key, nullptr, nullptr);
            head = tmp;
        }else if(head->frq > 0){
            Node* tmp = new Node(0, key, nullptr, head);
            head->pre = tmp;
            head = tmp;
        }else{
            InsertKeyToNode(head, key);
        }
        nodeMap[key] = head;
    }
    void removeNode(Node* node){
        if(node == nullptr) return;
        if(node->pre == nullptr){
            node = node->next;
        }else{
            node->pre->next = node->next;
        }
        if(node->next != nullptr){
            node->next->pre = node->pre;
        }
    }
};
