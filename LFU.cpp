class Node{
public:
Node* pre;
Node* next;
int frq;
unordered_map<int, list<int>::iterator> KeyMap;
list<int> KeyList;//time-ordered, first -> end：new -> old;
Node(int frq, int key, Node* pre, Node* next){
    this->frq = frq;
    this->pre = pre;
    this->next = next;
    KeyList.push_front(key);
    KeyMap[key] = KeyList.begin();
}
};

class LFUCache{
Node* head = new Node(0, 0, nullptr, nullptr);
unordered_map<int, int> valueMap;// key, value
unordered_map<int, Node*> nodeMap;//key，Node*
int _size;
public:
LFUCache(int capacity){
    this->_size = capacity;
    RemoveKeyfromNode(head, 0);
}
int get(int key){
    if(valueMap.find(key) == valueMap.end()) return -1;
    UpdateKey(key);
    return valueMap[key];
}
void set(int key, int value){
    if(_size <= 0) return;
    if(valueMap.find(key) != valueMap.end()){
        valueMap[key] = value;
    }else{
        if(valueMap.size() == _size) popOldNode();
        valueMap[key] = value;
        InsertKeyToNode(head, key);
        nodeMap[key] = head;
    }
    UpdateKey(key);
}
void RemoveKeyfromNode(Node* node, int key){
    auto itr = node->KeyMap[key];
    node->KeyList.erase(itr);
    node->KeyMap.erase(key);
    if(node->frq != 0 && node->KeyMap.empty()) removeNode(node);
}
void InsertKeyToNode(Node* node, int key){
    node->KeyList.push_front(key);
    node->KeyMap[key] = node->KeyList.begin();
}
void UpdateKey(int key){
    Node* node = nodeMap[key];
    if(node->next == nullptr || node->next->frq > node->frq + 1){
        Node* tmp = new Node(node->frq + 1, key, node, node->next);
        if(node->next) node->next->pre = tmp;
        node->next = tmp;
    }else{//node->next->frq == node->frq + 1
        InsertKeyToNode(node->next, key);
    }
    nodeMap[key] = node->next;
    RemoveKeyfromNode(node, key);
}
void popOldNode(){
    auto tmp = head->next;
    if(tmp == nullptr) return;
    int n = tmp->KeyList.back();
    nodeMap.erase(n);
    valueMap.erase(n);
    RemoveKeyfromNode(tmp, n);
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
