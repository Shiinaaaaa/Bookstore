//
// Created by lenovo on 2021/1/25.
//

#include "Blocklist.h"
#include <cstring>
#include <iostream>

Node::Node(const string &_key, int _offset) {
    strcpy(key , _key.c_str());
    offset = _offset;
}

bool Node::operator<(const Node &o) {
    if (strcmp(key , o.key) < 0) return true;
    else return false;
}

bool Node::operator==(const Node &o) {
    if (strcmp(key , o.key) == 0) return true;
    else return false;
}

string Node::showKey() const {
    return key;
}

int Node::showOffset() const {
    return offset;
}

void Node::clearNode() {
    offset = -1;
    for (int i = 0 ; i < 60 ; ++i) key[i] = '\0';
}

Node &Node::operator=(const Node &o) {
    if (this == &o) return *this;
    else {
        strcpy(key , o.key);
        offset = o.offset;
        return *this;
    }
}

Block::Block() = default;

BlockList::BlockList(const string& _filename) {
    strcpy(filename , _filename.c_str());
}

void BlockList::AddNode(Node &o) {
    fstream  r, w;
    r.open(filename , ios::in | ios::out |ios::binary);
    w.open(filename , ios::in | ios::out |ios::binary);
    if (!r || !w) throw "error open";
    r.seekg(0 , ios::end);
    int p = r.tellg();
    if (p == 0) {
        Block block;
        w.seekp(0);
        w.write(reinterpret_cast<char*>(&block) , sizeof(Block));
    }
    Block tmp;
    w.seekp(0);
    r.seekg(0);
    r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
    while (tmp.next != -1){
        if (o < tmp.nodes[0] && tmp.previous != -1) {
            r.seekg(tmp.previous);
            w.seekp(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
        r.seekg(tmp.next);
        w.seekp(tmp.next);
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
        if (tmp.next == -1 && o < tmp.nodes[0]) {
            r.seekg(tmp.previous);
            w.seekp(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
    }
    int position = 0;
    bool flag = false;
    for (int i = 0 ; i < tmp.count; ++i){
        if(o < tmp.nodes[i]) {
            position = i;
            flag = true;
            break;
        }
    }
    if (!flag) position = tmp.count;
    for (int i = tmp.count - 1 ; i >= position ; --i){
        tmp.nodes[i + 1] = tmp.nodes[i];
    }
    tmp.nodes[position] = o;
    tmp.count++;
    int offset = w.tellp();
    w.write(reinterpret_cast<char*>(&tmp) , sizeof(Block));
    if(tmp.count > 95) SplitBlock(offset);
    r.close();
    w.close();
}

void BlockList::DeleteNode(Node &o) {
    fstream r , w;
    r.open(filename , ios::in | ios::out |ios::binary);
    w.open(filename , ios::in | ios::out |ios::binary);
    if (!r || !w) throw "open fail";
    if (r.eof()) throw "empty file";
    r.seekg(0);
    w.seekp(0);
    Block tmp;
    r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
    while (tmp.next != -1){
        if (o < tmp.nodes[0] && tmp.previous != -1) {
            r.seekg(tmp.previous);
            w.seekp(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
        r.seekg(tmp.next);
        w.seekp(tmp.next);
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
        if (tmp.next == -1 && o < tmp.nodes[0]) {
            r.seekg(tmp.previous);
            w.seekp(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
    } //同add
    if (tmp.previous != -1 && o == tmp.nodes[0] && strcmp(filename , "userid.dat") != 0){
        fstream r2 , w2;
        r2.open(filename , ios::in | ios::out |ios::binary);
        w2.open(filename , ios::in | ios::out |ios::binary);
        r2.seekg(tmp.previous);
        w2.seekp(tmp.previous);
        Block tmp2;
        r2.read(reinterpret_cast<char*>(&tmp2) , sizeof(Block));
        int p = 0;
        bool flag = false;
        for (int i = 0 ; i < tmp2.count ; ++i){
            if (o == tmp2.nodes[i] && o.offset == tmp2.nodes[i].offset) {
                p = i;
                flag = true;
                break;
            }
        }
        if (flag) {
            for (int i = p ; i < tmp2.count - 1 ; ++i){
                tmp2.nodes[i] = tmp2.nodes[i + 1];
            }
            tmp2.count--;
            int offset = w2.tellp();
            w2.write(reinterpret_cast<char*>(&tmp2) , sizeof(Block));
            //if(tmp2.next != -1) MergeBlock(offset);//判断后面有没有
            r2.close();
            w2.close();
            return;
        }
    }
    int position = 0;
    for (int i = 0 ; i < tmp.count ; ++i){
        if (strcmp(filename , "userid.dat") == 0) {
            if(o == tmp.nodes[i]) {
                position = i;
                break;
            }
        }
        else if (o == tmp.nodes[i] && o.offset == tmp.nodes[i].offset) {
            position = i;
            break;
        }
    }
    for (int i = position ; i < tmp.count - 1 ; ++i){
        tmp.nodes[i] = tmp.nodes[i + 1];
    }
    tmp.count--;
    int offset = w.tellp();
    w.write(reinterpret_cast<char*>(&tmp) , sizeof(Block));
    //if(tmp.next != -1) MergeBlock(offset);//判断后面有没有
    r.close();
    w.close();
}

int BlockList::FindNode(const string& _key) {
    fstream r;
    r.open(filename , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    if (r.eof()) throw "empty file";
    r.seekg(0);
    Block tmp;
    r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
    while (tmp.next != -1){
        if (strcmp(_key.c_str() , tmp.nodes[0].key) < 0) {
            r.seekg(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
        r.seekg(tmp.next);
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
        if (tmp.next == -1 && strcmp(_key.c_str() , tmp.nodes[0].key) < 0) {
            r.seekg(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
    }//同add
    for (int i = 0 ; i < tmp.count ; ++i){
        if(strcmp(_key.c_str() , tmp.nodes[i].key) == 0) {
            int o =  tmp.nodes[i].showOffset();
            return o;
        }
    }
    r.close();
    throw "nofind";
}

vector<Node> BlockList::showNode(const string& _key , vector<Node> &container) {
    fstream r;
    r.open(filename , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    if (r.eof()) throw "empty file";
    r.seekg(0);
    Block tmp;
    r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
    while (tmp.next != -1){
        if (strcmp(_key.c_str() , tmp.nodes[0].key) < 0) {
            r.seekg(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
        r.seekg(tmp.next);
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
        if (tmp.next == -1 && strcmp(_key.c_str() , tmp.nodes[0].key) < 0) {
            r.seekg(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
    } //同add
    if (strcmp(_key.c_str() , tmp.nodes[0].key) == 0) {
        Block tmp2;
        r.seekg(tmp.previous);
        r.read(reinterpret_cast<char*>(&tmp2) , sizeof(Block));
        for (int i = 0 ; i < tmp2.count ; ++i){
            if(strcmp(_key.c_str() , tmp2.nodes[i].key) == 0) {
                container.push_back(tmp2.nodes[i]);
            }
        }
    }//处理前一个block含有的特殊情况
    for (int i = 0 ; i < tmp.count ; ++i){
        if(strcmp(_key.c_str() , tmp.nodes[i].key) == 0) {
            container.push_back(tmp.nodes[i]);
        }
    }
    //以下处理后接情况
    if (tmp.next != -1 && strcmp(_key.c_str() , tmp.nodes[tmp.count - 1].key) == 0){
        int p = tmp.next;
        r.seekg(p);
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
        while(strcmp(_key.c_str() , tmp.nodes[0].key) == 0){
            for (int i = 0 ; i < tmp.count ; ++i){
                if(strcmp(_key.c_str() , tmp.nodes[i].key) == 0) {
                    container.push_back(tmp.nodes[i]);
                }
            }
            if (tmp.next == -1 && strcmp(_key.c_str() , tmp.nodes[tmp.count - 1].key) != 0) break;
            int position = tmp.next;
            r.seekg(position);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
        }
    }
    r.close();
    return container;
}

void BlockList::SplitBlock(int p) {
    fstream r , w;
    r.open(filename , ios::in | ios::out |ios::binary);
    w.open(filename , ios::in | ios::out |ios::binary);
    if (!r || !w) throw "open fail";
    r.seekg(p);
    Block tmp , tmp1 , tmp2;
    r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
    for (int i = 0 ; i < 50 ; ++i){
        tmp1.nodes[i] = tmp.nodes[i];
    }
    for (int i = 50 ; i < tmp.count ; ++i){
        tmp2.nodes[i - 50] = tmp.nodes[i];
    }
    tmp2.count = tmp.count - 50;
    tmp1.count = 50;
    tmp2.next = tmp.next;
    tmp2.previous = p;
    w.seekp(0 , ios::end);
    tmp1.previous = tmp.previous;
    tmp1.next = w.tellp();
    w.seekp(p);
    w.write(reinterpret_cast<char*>(&tmp1) , sizeof(Block));
    w.seekp(tmp1.next);
    w.write(reinterpret_cast<char*>(&tmp2) , sizeof(Block));//tmp2
    if (tmp2.next != -1){
        r.seekg(tmp2.next);
        Block t;
        r.read(reinterpret_cast<char*>(&t) , sizeof(Block));
        t.previous = tmp1.next;
        w.seekp(tmp2.next);
        w.write(reinterpret_cast<char*>(&t) , sizeof(Block));
    }
    r.close();
    w.close();
}

void BlockList::MergeBlock(int p) {
    fstream r , w;
    r.open(filename , ios::in | ios::out |ios::binary);
    w.open(filename , ios::in | ios::out |ios::binary);
    if (!r || !w) throw "open fail";
    Block tmp1 , tmp2;
    r.seekg(p);
    r.read(reinterpret_cast<char*>(&tmp1) , sizeof(Block));
    r.seekg(tmp1.next);
    r.read(reinterpret_cast<char*>(&tmp2) , sizeof(Block));
    //if (tmp1.count + tmp2.count <= 85 || tmp1.count + tmp2.count >= 95) return;
    if (tmp1.count + tmp2.count >= 10 || tmp1.next == -1) return;
    for (int i = 0 ; i <= tmp2.count - 1 ; ++i){
        tmp1.nodes[i + tmp1.count] = tmp2.nodes[i];
    }
    tmp1.next = tmp2.next;
    tmp1.count += tmp2.count;
    if (tmp2.next != -1){
        r.seekg(tmp2.next);
        Block tmp;
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
        tmp.previous = p;
        w.seekp(tmp2.next);
        w.write(reinterpret_cast<char*>(&tmp) , sizeof(Block));
    }
    w.seekp(p);
    w.write(reinterpret_cast<char*>(&tmp1) , sizeof(Block));
    r.close();
    w.close();
}

bool BlockList::existNode(const string &_key) {
    fstream r;
    r.open(filename , ios::in | ios::out | ios::binary);
    if (!r) throw "open fail";
    if (r.eof()) throw "empty file";
    r.seekg(0);
    Block tmp;
    r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
    while (tmp.next != -1){
        if (strcmp(_key.c_str() , tmp.nodes[0].key) < 0) {
            r.seekg(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
        r.seekg(tmp.next);
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
        if (tmp.next == -1 && strcmp(_key.c_str() , tmp.nodes[0].key) < 0) {
            r.seekg(tmp.previous);
            r.read(reinterpret_cast<char*>(&tmp) , sizeof(Block));
            break;
        }
    } //同add
    for (int i = 0 ; i < tmp.count ; ++i){
        if(strcmp(_key.c_str() , tmp.nodes[i].key) == 0) {
            return true;
        }
    }
    r.close();
    return false;
}



