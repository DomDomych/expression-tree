#pragma once

#include <iostream>
#include <string>
#include <vector>

template <typename Key, typename Value>
class AVLTreeBase
{
protected:
    struct Node
    {
        Key key;
        Value value;
        Node *left;
        Node *right;
        unsigned int h;
        int count;
        Node(const Key &key, const Value &value) : key(key),
                                                   value(value),
                                                   left(nullptr),
                                                   right(nullptr),
                                                   h(1),
                                                   count(0)
                                                   {}
        ~Node() = default;
    };
    Node *root;

        unsigned int height(Node *p)
    {
        return p ? p->h : 0;
    }

    int bfactor(Node *p)
    {
        return static_cast<int>(height(p->right)) - static_cast<int>(height(p->left));
    }

    void fixheight(Node *p)
    {
        unsigned int hl = height(p->left);
        unsigned int hr = height(p->right);

        p->h = (hl > hr ? hl : hr) + 1;
    }

    Node *rotateRight(Node *p)
    {
        Node *q = p->left;
        p->left = q->right;
        q->right = p;
        fixheight(p);
        fixheight(q);
        return q;
    }

    Node *rotateLeft(Node *p)
    {
        Node *q = p->right;
        p->right = q->left;
        q->left = p;
        fixheight(p);
        fixheight(q);
        return q;
    }

    Node *balance(Node *p)
    {
        fixheight(p);
        if (bfactor(p) == 2)
        {
            if (bfactor(p->right) < 0)
            {
                p->right = rotateRight(p->right);
            }
            return rotateLeft(p);
        }
        if (bfactor(p) == -2)
        {
            if (bfactor(p->left) > 0)
            {
                p->left = rotateLeft(p->left);
            }
            return rotateRight(p);
        }
        return p;
    }

    Node *insert(Node *p, const Key &key, const Value &value)
    {
        if (!p)
            return new Node(key, value);
        if (key < p->key)
        {
            p->left = insert(p->left, key, value);
        }
        else if (key == p->key)
        {
            p->value = value;
            return p;
        }
        else
        {
            p->right = insert(p->right, key, value);
        }
        return balance(p);
    }

        Node *find(Node *p, const Key &key) const
    {
        if (!p)
        {
            return nullptr;
        }
        if (key < p->key)
        {
            return find(p->left, key);
        }
        if (key > p->key)
        {
            return find(p->right, key);
        }
        return p;
    }
    Node *mostPopular(Node *p, Node *best) const
    {
        if (!p)
            return best;

        if (!best || p->count > best->count)
        {
            best = p;
        }

        best = mostPopular(p->left, best);
        best = mostPopular(p->right, best);

        return best;
    }

    Node *findmin(Node *p)
    {
        return p->left ? findmin(p->left) : p;
    }

    Node *removemin(Node *p)
    {
        if (p->left == nullptr)
        {
            return p->right;
        }

        p->left = removemin(p->left);
        return balance(p);
    }

    Node *remove(Node *p, const Key &key)
    {
        if (!p)
            return nullptr;

        if (key < p->key)
        {
            p->left = remove(p->left, key);
        }
        else if (key > p->key)
        {
            p->right = remove(p->right, key);
        }
        else
        {
            Node *l = p->left;
            Node *r = p->right;
            delete p;
            if (!r)
                return l;
            Node *min = findmin(r);
            min->right = removemin(r);
            min->left = l;
            return balance(min);
        }
        return balance(p);
    }

    void clear(Node *p)
    {
        if (!p)
            return;

        clear(p->left);
        clear(p->right);

        delete p;
    }

    void print(Node *p, int &number) const
    {
        if (!p)
            return;
        print(p->left, number);
        std::cout << number << ". " << p->key << "->" << p->value << ' ' << p->count << '\n';
        number++;
        print(p->right, number);
    }

    AVLTreeBase():root(nullptr){}
    ~AVLTreeBase()
    {
        clear(root);
    }
};

template<typename Key,typename Value>
class AVLTreeCommon : public AVLTreeBase<Key,Value>
{

    using Node = typename AVLTreeBase<Key,Value>::Node;
    public:
    bool remove(const Key &key)
    {
        if (!this->find(this->root, key))
            return false;

        this->root = AVLTreeBase<Key,Value>::remove(this->root, key);
        return true;
    }

    bool get(const Key &key, Value &value) const
    {
        Node *found = this->find(this->root, key);

        if (!found)
        {
            return false;
        }

        value = found->value;

        return true;
    }

    bool getMostPopular(Key &key, Value &value, int &count) const
    {
        Node *best = this->mostPopular(this->root, nullptr);

        if (!best)
            return false;

        key = best->key;
        value = best->value;
        count = best->count;

        return true;
    }

    Value &operator[](const Key &key)
    {
        Node *found = this->find(this->root, key);

        if (!found)
        {
            this->root = this->insert(this->root, key, Value{});
            found = this->find(this->root, key);
        }

        return found->value;
    }

    void print() const
    {
        int number = 1;
        AVLTreeBase<Key,Value>::print(this->root, number);
    }

    void increment(const Key &key)
    {
        Node *found = this->find(this->root, key);

        if (found)
        {
            found->count++;
        }

    }
};

template <typename Key, typename Value>
class AVLTree:public AVLTreeCommon<Key,Value>
{
public:

    AVLTree() = default;
    AVLTree(const AVLTree &) = delete;
    AVLTree &operator=(const AVLTree &) = delete;
};

// Specialization for std::string
template <>
class AVLTree<std::string, std::string>: public AVLTreeCommon<std::string,std::string>
{

using Node = typename AVLTreeBase<std::string,std::string>::Node; 


private:

    std::vector<int> prefixfunction(const std::string &s) const
    {
        std::vector<int> pi(s.size(), 0);
        for (size_t i = 1; i < s.size(); i++)
        {
            int j = pi[i - 1];
            while (j > 0 && s[i] != s[j])
            {
                j = pi[j - 1];
            }
            if (s[i] == s[j])
            {
                j++;
            }
            pi[i] = j;
        }

        return pi;
    }

    bool kmp(const std::string &value, const std::string &word) const
    {
        if (word.empty())
            return true;

        std::vector<int> pi = prefixfunction(word);

        int j = 0;

        for (size_t i = 0; i < value.size(); i++)
        {
            while (j > 0 && value[i] != word[j])
            {
                j = pi[j - 1];
            }

            if (value[i] == word[j])
            {
                j++;
            }

            if (j == word.size())
            {
                return true;
            }
        }
        return false;
    }
    
    void find_by_word(Node *p, const std::string &word, std::vector<std::pair<std::string, std::string>> &found) const
    {
        if (!p)
            return;
        find_by_word(p->left, word, found);
        if (kmp(p->value, word))
        {
            found.push_back({p->key, p->value});
        }
        find_by_word(p->right, word, found);
    }

public:

    std::vector<std::pair<std::string, std::string>> find_by_word(const std::string &word) const
    {
        std::vector<std::pair<std::string, std::string>> found;
        find_by_word(this->root, word, found);
        return found;
    }
    AVLTree() = default;
    AVLTree(const AVLTree &) = delete;
    AVLTree &operator=(const AVLTree &) = delete;
};