
// Do NOT add any other includes
#include "search.h"

vector<string> sentence_splitter6(string sentence)
{
    string seperator = " .,-:!\"\'()?[];@";
    vector<string> words;
    string word = "";
    int ln = sentence.size();
    for (int i = 0; i < ln; i++)
    {
        char temp = sentence[i];
        if (seperator.find(temp) == string::npos)
        {
            word += char(tolower(int(temp)));
        }
        else
        {
            if (word != "")
            {
                words.push_back(word);
                word = "";
            }
        }
    }
    if (word != "")
    {
        words.push_back(word);
        word = "";
    }
    return words;
}

SearchEngine::SearchEngine()
{
    // Implement your function here
    storing_sentences = new linked_list();
}

SearchEngine::~SearchEngine()
{
    // Implement your function here
    delete storing_sentences;
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    // Implement your function here
    storing_sentences->insert(book_code, page, paragraph, sentence_no, sentence);
    return;
}

Node *SearchEngine::search(string pattern, int &n_matches)
{
    list_node * node = storing_sentences->sen_head->right;
    vector<Node*> previous;
    while (node != NULL)
    {
        string snt = node->sentence;
        vector<string>v=sentence_splitter6(snt);
        for(string str:v){
            if (pattern==str)
            {
                Node* temp = new Node(node->book_code, node->page, node->paragraph, node->sentence_no, -1);
                previous.push_back(temp);
                n_matches++;
                // break;
            }   
        }
        node=node->right;
    }
    for (int i = 0; i < previous.size()-1; i++)
    {
        previous[i]->right = previous[i + 1];
        previous[i + 1]->left = previous[i];
    }
    
    if(previous.size()==0){
        return NULL;
    }
    return previous[0];
}