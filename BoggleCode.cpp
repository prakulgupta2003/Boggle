//
//  main.cpp
//  boggle
//
//  Created by Shishir Garg on 10/7/23.
//
#include <iostream>
#include <fstream> 
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>


using namespace std;

class TrieNode{
public:
    char value;
    TrieNode* child[26];
    int endWord;                            // to check if a word is ending here
    TrieNode(char value){
        this->value=value;
        for(int i=0;i<26;i++){
            child[i]=0;
        }
        endWord=0;
    }
};

void Trie(string word,TrieNode* root){      // function insert a word into a trie
    int wordsize=word.size();
    for(auto i=0;i<wordsize;i++){
        if(root->child[word[i]-'a']==NULL){
            root->child[word[i]-'a']=new TrieNode(word[i]);     // creating a new node
        }
        root=root->child[word[i]-'a'];
        
    }
    root->endWord++;
    return;
}
void boggleCreator(char boggle[4][4]){       // function to create a boggle
    char dice[16][6]={{'r','i','f','o','b','x'},{'i','f','e','h','e','y'},{'d','e','n','o','w','s'},{'u','t','o','k','n','d'},{'h','m','s','r','a','o'},{'l','u','p','e','t','s'},{'a','c','i','t','o','a'},{'y','l','g','k','u','e'},{'q','b','m','j','o','a'},{'e','h','i','s','p','n'},{'v','e','t','i','g','n'},{'b','a','l','i','y','t'},{'e','z','a','v','n','d'},{'r','a','l','e','s','c'},{'u','w','i','l','r','g'},{'p','a','c','e','m','d'}};
    int spot=0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            boggle[i][j]=dice[spot][rand()%6];      // rand() produces a random integer
            spot++;
        }
    }

}
bool isValid(int i,int j){                      // function to check if the index we want to go to is valid or not
    if(i<4 && j<4 && i>=0 && j>=0){
        return true;
    }
    return false;
}
void isWordPresentHelper(TrieNode* temp,char boggle[4][4],string str,int i, int j, int visited[4][4],vector<string> &ans){
    if(temp->endWord>0){                        // if word is found then it is pushed into the answer vector
        ans.push_back(str);
        temp->endWord--;                         // this is done to prevent saving a word more than once
    }
    visited[i][j]=1;                            // marking the letter as visited
    for(auto k=0;k<26;k++){                      // checking children nodes to see if word is present
        if(temp->child[k]!=NULL){
            char ch=temp->child[k]->value;
            if(isValid(i-1,j-1) && !visited[i-1][j-1] && boggle[i-1][j-1]==ch){
                isWordPresentHelper(temp->child[boggle[i-1][j-1]-'a'],boggle,str+ch,i-1,j-1,visited,ans);
            }
            if(isValid(i,j-1) && !visited[i][j-1] && boggle[i][j-1]==ch){
                isWordPresentHelper(temp->child[boggle[i][j-1]-'a'],boggle,str+ch,i,j-1,visited,ans);
            }
            if(isValid(i-1,j) && !visited[i-1][j] && boggle[i-1][j]==ch){
                isWordPresentHelper(temp->child[boggle[i-1][j]-'a'],boggle,str+ch,i-1,j,visited,ans);
            }
            if(isValid(i+1,j+1) && !visited[i+1][j+1] && boggle[i+1][j+1]==ch){
                isWordPresentHelper(temp->child[boggle[i+1][j+1]-'a'],boggle,str+ch,i+1,j+1,visited,ans);
            }
            if(isValid(i,j+1) && !visited[i][j+1] && boggle[i][j+1]==ch){
                isWordPresentHelper(temp->child[boggle[i][j+1]-'a'],boggle,str+ch,i,j+1,visited,ans);
            }
            if(isValid(i+1,j) && !visited[i+1][j] && boggle[i+1][j]==ch){
                isWordPresentHelper(temp->child[boggle[i+1][j]-'a'],boggle,str+ch,i+1,j,visited,ans);
            }
            if(isValid(i-1,j+1) && !visited[i-1][j+1] && boggle[i-1][j+1]==ch){
                isWordPresentHelper(temp->child[boggle[i-1][j+1]-'a'],boggle,str+ch,i-1,j+1,visited,ans);
            }
            if(isValid(i+1,j-1) && !visited[i+1][j-1] && boggle[i+1][j-1]==ch){
                isWordPresentHelper(temp->child[boggle[i+1][j-1]-'a'],boggle,str+ch,i+1,j-1,visited,ans);
            }
        }
    }
    visited[i][j]=0;

}
vector<string> isWordPresent(TrieNode* root, char boggle[4][4]){     // function to check which words are present in the boggle
    vector<string> ans;
    TrieNode* tempNode=root;
    string str="";
    int visited[4][4];                                               // an array to check if an index has been visited or not
    for(int i=0;i<4;i++){                                            
        for(int j=0;j<4;j++){
            visited[i][j]=0;
        }
    }
    for(auto i=0;i<4;i++){
        for(auto j=0;j<4;j++){
            if(tempNode->child[boggle[i][j]-'a']!=NULL){
                str+=boggle[i][j];
                tempNode=tempNode->child[boggle[i][j]-'a'];
                isWordPresentHelper(tempNode,boggle,str,i,j,visited,ans);
                str="";
                tempNode=root;
            }
        }
    }
    return ans;
}
int main(){ 
    srand((int)time(0));                // so that random digits are produced each time code is executed
    cout<<" Welcome To BOGGLE! "<<endl;
    cout << "Press Enter to generate words";
    getchar(); 

    TrieNode* root=new TrieNode(' ');
    TrieNode* temp=NULL;
    ifstream dictionary;
    dictionary.open("english.txt");       // opening file containing all words,letters and abbreviations
    vector<string> words;
    int count=0;
    string files;
    while(getline(dictionary,files)){                        // copying all words into vector from file
        words.push_back(files);
        count++;
    }
    for(auto i=0;i<count;i++){                               // creating a trie
        bool check=true;
        temp=root;
        for(auto j=0;j<words[i].size();j++){                  // to check that word contains only letters
            if(words[i][j]-'a'<26 && words[i][j]-'a'>=0 ){
                continue;
            }
            else{
                check=false;
                break;

            }
            
        }
        if(check==true){
                Trie(words[i],temp);
            }
    }
    dictionary.close();                                     // closing file
    cout<<endl;
    char boggle[4][4];
    boggleCreator(boggle);                                  // creating boggle
    cout<<"Current Boggle is :"<<endl;
    for(int i=0;i<4;i++){                   
        for(int j=0;j<4;j++){
            cout<<boggle[i][j]<<" ";                 // printing boggle       
        }
        cout<<endl;
    }
    temp=root;
    vector<string> ans=isWordPresent(temp,boggle);       // getting all words,letter,abbrevaitions present in boggle
    if(ans.size()==0){                                    // checking if no words are present
        cout<<"\n No Words Present \n"<<endl;
    }
    else{
        cout<< "Words and abbreviations that are present :"<<endl;      // prinintg all words and abbreviations
        for(auto i=0;i<ans.size();i++){
            if(ans[i].size()>2){
                cout<<ans[i]<<", ";

            }
        }
    }
    delete temp;
    return 0;
}



 
