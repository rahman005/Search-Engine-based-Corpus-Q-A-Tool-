#include <assert.h>
#include <sstream>
#include "qna_tool.h"

class ParaNode {
    public:
        Node* Nd;
        long double priority; //Paragraph Ka Score
        ParaNode(Node * node, double priority){
            Nd = node;
            this->priority = priority;
        }
};
int pow_selfk1(int a, int b)
{
    if (b == 0)
        return 1;
    return a * pow_selfk1(a, b - 1);
}
class HashTable
{
public:
    vector<vector<ParaNode *>> table;
    int size;
    int get_hashVaue(string string_word)
    {
        int h = 0;
        int i = 0;
        while (i < 5)
        {
            int sum = 0;
            for (int j = i; j < string_word.size(); j = j + 5)
            {
                sum += string_word[j];
            }
            h = h + (sum % 10) * pow_selfk1(10, i);
            i++;
        }
        return h%100000;
    }
    HashTable(){
        size = 0;
        table.resize(100000);
    }
    void insert(string string_word, ParaNode * node)
    {
        int hash_value = get_hashVaue(string_word);
        vector<ParaNode *> vect = table[hash_value];
        for(int i=0;i<table[hash_value].size();i++){
            if(((table[hash_value][i]->Nd->paragraph == node->Nd->paragraph) && (table[hash_value][i]->Nd->book_code == node->Nd->book_code)) && (table[hash_value][i]->Nd->page == node->Nd->page)){
                table[hash_value][i]->priority += node->priority;
                return;
            }
        }
        table[hash_value].push_back(node);
        size++;
    }
    vector<ParaNode *> getTable()
    {
        vector<ParaNode *> vect;
        for(int i=0;i<table.size();i++){
            for(int j=0;j<table[i].size();j++){
                vect.push_back(table[i][j]);
            }
        }
        return vect;
    }
};
int get_right(int i){
    return 2*i +2;
}
int get_left(int i){
    return 2*i+1;
}
void heapify_down(vector <ParaNode *> & vect,int i){
    int left = get_left(i);
    int right = get_right(i);
    if(left>=vect.size()) return;
    int min_index = i;
    if(left<vect.size()){
        if(vect[left]->priority<vect[i]->priority){
            min_index = left;
        }
    }
    if(right < vect.size()){
        if(vect[right]->priority < vect[min_index]->priority){
            min_index = right;
        }
    }
    if(min_index != i){
        ParaNode *  temp= vect[min_index];
        vect[min_index] = vect[i];
        vect[i] = temp;
        heapify_down(vect,min_index); 
    }
}
void build_heap(vector <ParaNode * > & vect){
    int si_ze= vect.size();
    for(int i=(si_ze/2)-1;i>=0;i--){
        heapify_down(vect,i);
    }
}
vector<string> sentence_splitter1(string sentence)
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
vector<string> splitter(string str){
    vector<string> ans;
    string temp="";
    for (int i = 0; i < str.size(); i++)
    {
        if(str[i]!=','){
            temp+=str[i];
        }
        else{
            ans.push_back(temp);
            temp = "";
        }
    }
    if(temp!=""){
        ans.push_back(temp);
        temp="";
    }
    return ans;
}
Dict* csv_reader()
{
    Dict* dct = new Dict();
    fstream fin;
    fin.open("unigram_freq.csv", ios::in);
    string temp;
    int mm = 0;
    while (fin >> temp)
    {
        if(mm==0){
            mm++;
            continue;
        }
        vector<string> temp1 = splitter(temp);
        dct->set_word_count(temp1[0], std::stod(temp1[1]));
    }
    return dct;
}
QNA_tool::QNA_tool()
{
    // Implement your function here
    engine = new SearchEngine();
    dict = new Dict();
    csvdict = new Dict();
    csvdict = csv_reader();
}

QNA_tool::~QNA_tool()
{
    // Implement your function here
    delete engine;
    delete dict;
    delete csvdict;
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{   
    // Implement your function here
    engine->insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    dict->insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    // cout<<"exiting"<<endl;
    return;
}
void mergesort(vector<ParaNode*> &paradict,int l, int r){
    if(l>=r) return;
    int mid = (l+r)/2;
    mergesort(paradict,l,mid);
    mergesort(paradict,mid+1,r);
    vector<ParaNode*> temp;
    int i=l;
    int j=mid+1;
    while(i<=mid && j<=r){
        if(paradict[i]->priority<paradict[j]->priority){
            temp.push_back(paradict[i]);
            i++;
        }
        else{
            temp.push_back(paradict[j]);
            j++;
        }
    }
    while(i<=mid){
        temp.push_back(paradict[i]);
        i++;
    }
    while(j<=r){
        temp.push_back(paradict[j]);
        j++;
    }
    for(int i=l;i<=r;i++){
        paradict[i]=temp[i-l];
    }
}
Node *QNA_tool::get_top_k_para(string question, int k)
{
    // Implement your function here
    vector<string> words = sentence_splitter1(question);
    
    HashTable* table = new HashTable(); // Here Use sentence number as Paragraph score
    for (string st : words)
    {   
        long long freq_in_general_corpus = csvdict->get_word_count(st);
        long long freq_in_corpus = dict->get_word_count(st);
        long double scr = ((long double)freq_in_corpus + (long double)1)/((long double)freq_in_general_corpus + (long double)1);
        // cout<<"freq_in_general_corpus "<<freq_in_general_corpus<<" freq_in_corpus "<<freq_in_corpus<<" scr "<<scr<<endl; // "
        // cout<<scr<<" Score hai "<<endl;
        int matches = 0;
        Node *temp = engine->search(st, matches);
        while (temp && matches)
        {
            string para = to_string(temp->book_code) +" "+ to_string(temp->page) +" "+ to_string(temp->paragraph);
            ParaNode *nd = new ParaNode(temp, scr);
            table->insert(para, nd);
            temp = temp->right;
            matches--;
        }
        // cout<<"Idhar to aa raha hun"<<endl;
    }
    // cout<<"Idhar Hun"<<endl;
    long long cur_size = table->size;
    // cout<<"cur_size "<<cur_size<<endl;
    k=min((long long)k,cur_size);
    // cout<<"k "<<k<<endl;
    vector<ParaNode *> paraDict = table->getTable();
    // cout<<"paraDict size "<<paraDict.size()<<endl;
    mergesort(paraDict,0,paraDict.size()-1);
    
    // for (int i = 0; i < paraDict.size(); i++)
    // {
    //     cout<<paraDict[i]->priority<<" "<<paraDict[i]->Nd->book_code<<" "<<paraDict[i]->Nd->page<<" "<<paraDict[i]->Nd->paragraph<<endl;
    // }
    // cout<<"size "<<paraDict.size()<<endl;
    vector <ParaNode * > heap(k);
    for (int i = 0; i < k; i++)
    {
        heap[i] = paraDict[paraDict.size()-1-i];
        // cout<<heap[i]->priority<<endl;
    }    
    Node * head;
    Node *temp = heap[0]->Nd;
    head = temp;
    for(int i=1;i<k;i++){
        temp->right=heap[i]->Nd;
        temp= temp->right;
        if(i==k-1){
            temp->right = NULL;
        }
    }
    return head;
}

void QNA_tool::query(string question, string filename)
{
    // Implement your function here
    
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: "
              << "Studying COL106 :)" << std::endl;
    
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph)
{

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;

    std::string filename = "txtfiles/mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence))
    {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ','))
        {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos)
            {
                token = token.substr(start, end - start + 1);
            }

            // Check if the element is a number or a string
            if (token[0] == '\'')
            {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            }
            else
            {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if (
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph))
        {
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node *root, int k, string API_KEY, string question)
{

    // first write the k paragraphs into different files

    Node *traverse = root;
    int num_paragraph = 0;

    while (num_paragraph < k)
    {
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();

    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}