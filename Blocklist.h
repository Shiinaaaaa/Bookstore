//
// Created by lenovo on 2021/1/25.
//

#include <cstring>
#include <vector>
#include <fstream>
using namespace std;

#define sqrt_n 100

#ifndef BOOKSTORE_BLOCKLIST_H
#define BOOKSTORE_BLOCKLIST_H

class Node{
private:
    char key[61] = "";
    int offset = -1;
public:
    explicit Node(const string &_key = "", int _offset = -1);
    string showKey() const;
    int showOffset() const;
    void clearNode();
    Node &operator=(const Node &o);
    bool operator<(const Node &o);
    bool operator==(const Node &o);

    friend class BlockList;
};

class Block{
private:
    int count = 0;
public:
    int previous = -1;
    int next = -1;
    Node nodes[sqrt_n];
    Block();
    friend class BlockList;
};

class BlockList{
private:
    char filename[60] = "";
public:
    explicit BlockList(const string& _filename);
    void AddNode(Node &o);
    void DeleteNode(Node &o);
    int FindNode(const string& _key);
    vector<Node> showNode(const string& _key , vector<Node> &container);
    bool existNode(const string& _key);
    void SplitBlock(int p);
    void MergeBlock(int p);
};

#endif //BOOKSTORE_BLOCKLIST_H
