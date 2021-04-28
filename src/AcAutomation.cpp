#include "AcAutomation.h"
#include <algorithm>
#include <queue>

AcAutomation::AcAutomation(bool is_case_sensitive) {
    this->root = std::make_shared<Node>();
    this->case_sensitive_flag = is_case_sensitive;
}

// insert a word into ac tree
void AcAutomation::Insert(std::string word) {
    if (!this->case_sensitive_flag)
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    std::shared_ptr<Node> temp = this->root;
    for (char & w : word) {
        if (temp->next.find(w) == temp->next.end()) {
            std::shared_ptr<Node> new_node;
            new_node = std::make_shared<Node>();
            new_node->path = temp->path + w;
            temp->next.insert({w, new_node});
        }
        temp = temp->next[w];
    }
    temp->end_flag = true; // last char's flag of a word should be set true
}

// use BFS algorithm to build all fail pointers in this tree
void AcAutomation::BuildFailPointer() {
    std::queue<std::shared_ptr<Node>> q;
    q.push(this->root); // 根节点失配指针为 nullptr
    while (!q.empty()) {
        std::shared_ptr<Node> current_node = q.front(); // 获取队头结点
        q.pop();
        for (const auto &iter: current_node->next) { // 遍历当前节点的所有子节点
            char w = iter.first;
            std::shared_ptr<Node> son_node = iter.second;
            q.push(son_node); // 将当前节点的各个子节点加入队列
            if (current_node == this->root) {
                son_node->fail = this->root;
            } else {
                std::shared_ptr<Node> p = current_node->fail;
                while (p) {
                    if (p->next.find(w) != p->next.end()) {
                        son_node->fail = p->next[w];
                        break;
                    }
                    p = p->fail;
                }
                if (!p)
                    son_node->fail = this->root;
            }
        }
    }
}

std::vector<std::string> AcAutomation::Search(std::string text) {
    if (!case_sensitive_flag)
        std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    std::vector<std::string> matched_words;
    std::shared_ptr<Node> p = this->root;
    for (char &w: text) {
        // p==nullptr 说明当前指针p为root节点的失配指针，因为只有root节点的是失配指针为nullptr
        while (p && p->next.find(w) == p->next.end())
            p = p->fail;
        if (!p)
            p = this->root;
        else
            p = p->next[w];

        // 找到当前所有匹配的单词
        std::shared_ptr<Node> temp = p;
        while (temp != this->root) {
            if (temp->end_flag)
                matched_words.push_back(temp->path);
            temp = temp->fail;
        }
    }
    return matched_words;
}
