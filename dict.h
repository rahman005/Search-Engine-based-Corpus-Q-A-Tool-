// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
class Object1{
    public:
    double count;
    string word_obj;
    Object1(double cou , string s);
    Object1();
};


class Dict {
private:
    // You can add attributes/helper functions here
    

public: 
    vector <vector <Object1> > hashtable;
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    

    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    double get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
    void set_word_count(string word, double count);
};