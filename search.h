// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include "Node.h"
using namespace std;
class list_node
{
public:
    list_node *left;
    list_node *right;
    int book_code, page, paragraph, sentence_no;
    string sentence;
    list_node()
    {
        left = NULL;
        right = NULL;
        book_code = -1;
        sentence="";
        page=-1;
        paragraph=-1;
        sentence_no=-1;
    };
    string lower_converter(string sentence)
    {
        string ans = "";
        int k = sentence.size();
        for (int i = 0; i < k; i++)
        {
            char temp = sentence[i];
            ans += char(tolower(int(temp)));
        }
        return ans;
    }
    list_node(int book_code, int page, int paragraph, int sentence_no, string sentence)
    {
        left = NULL;
        right = NULL;
        this->book_code = book_code;
        this->page = page;
        this->paragraph = paragraph;
        this->sentence_no = sentence_no;
        string lower_sentence = this->lower_converter(sentence);
        this->sentence = lower_sentence;
    };
    ~list_node()
    {
        if (right)
            delete right;
    };
};
class linked_list
{
public:
    int size;
    list_node *sen_head;
    list_node *sen_tail;
    linked_list()
    {
        size = 0;
        sen_head = new list_node();
        sen_tail = new list_node();
        sen_head->right = sen_tail;
        sen_tail->left = sen_head;
    };
    void insert(int book_code, int page, int paragraph, int sentence_no, string sentence)
    {
        list_node *temp = new list_node(book_code, page, paragraph, sentence_no, sentence);
        temp->left = sen_tail->left;
        sen_tail->left->right = temp;
        temp->right = sen_tail;
        sen_tail->left = temp;
        size++;
    };
    ~linked_list()
    {
        delete sen_head;
    };
};
class SearchEngine
{
private:
    // You can add attributes/helper functions here
    linked_list *storing_sentences;

public:
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node *search(string pattern, int &n_matches);

    /* -----------------------------------------*/
};