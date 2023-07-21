//Alex Zandstra 4/20/21

#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

//Class to manage ruleset
class CYKLanguage{
    //list of terminal characters
    vector<string> characters;
    //list of strings that are equivelent to terminal characters
    vector<string> strings;
    //number of  rules
    int ruleNum;
    public:
        //constructor with filename to get ruleset
        CYKLanguage(string);
        //given a vector of 2 string pairs check if any of them are in the language and return the termninal characters
        vector<string> inLanguageValues(vector<string>);
        //given a string iteate through it and check if the character is in the language and return the termninal characters
        vector<string> inLanguageValues(string stringList);
        //break apart rule in format *->*
        void parse(string input);
    //lol nope
    private:
        
};

//structure of data for each cell/cell in grid
struct cell{
    //string to print
    string cellString;
    //terminal characters
    vector<string> strings;
    //unused placeholder for list of strings that matched
    vector<string> branches;

};

//method for printing the grid
void printGrid(cell **grid, int size);
//method for converting a vector to a formatted string
string vector_toString(vector<string> stringList);
//method for combining vectors
vector<string> concatVectors(vector<string> base,vector<string> addon);
//method for concatinating each string in a vector with each string in the other vector
vector<string> mixVectors(vector<string> x,vector<string> y);

//main method  
int main(int argc, char* argv[]){

    if(argc != 3){

        cout << "Error invaild command. \nUseage: CYK [Filename] [String of size > 0]\n";
        exit(1);

    }

    string filename = argv[1];
    string word = argv[2];
    int size = word.size();


    cout << "String Size: " << size << endl;
    cout << "Filename: " << filename << "\n" << "String: " << word << endl;
    CYKLanguage *language = new CYKLanguage(filename);
    

    //build matrix of sting size with cell structure type
    cell **grid = new cell*[size];
    for(int i = 0; i < size; i++){
        grid[i] = new cell[size];}


    //get diagonal set of cells from initional string
    for (int i = 0; i < size; i++){

        grid[i][i].strings = language -> inLanguageValues(word.substr(i,1));
        
    }
    
    //printGrid(grid,size);
    
    //algorithm for itterrating through the matrix and concatinating the strings
    for (int l = 2; l <= size; l++){
        
        for (int i = 1; i <= size - l + 1; i++){
            int j = i+l-1;
            //create or reset strings
            vector<string> strings;
            //vector<string> group;
            for (int k = i; k <= j -1; k++){

                //group = mixVectors( grid[i-1][k-1].strings , grid[k][j-1].strings );

                strings = concatVectors(strings ,mixVectors( grid[i-1][k-1].strings , grid[k][j-1].strings ) );
    
            }
            

            
            if(language -> inLanguageValues(strings).size() > 0){
                grid[i-1][j-1].strings = language -> inLanguageValues(strings);
            }
            //printGrid(grid,size);

        }
        //cout << endl;
        
    }
    
    //print the final grid
    printGrid(grid,size);

    //check if string passed
    bool passed = false;
    for (int i = 0; i < grid[0][size-1].strings.size(); i++)
    {
        if(grid[0][size-1].strings.at(i) == "S"){
            passed = true;
        }
    }

    if(passed){
        cout << "\nSTRING PASSED!\n";
    }else{
        cout << "\nSTRING FAILED\n";
    }
    
    return 0;

}


vector<string> mixVectors(vector<string> x,vector<string> y){

    vector<string> out;
   
    for (int i = 0; i < x.size(); i++)
    {
        for (int j = 0; j < y.size(); j++)
        {
            
            out.push_back(x.at(i) + y.at(j));
        }
        
    }
    
    return out;

}
vector<string> concatVectors(vector<string> base,vector<string> addon){

    

    for (int i = 0; i < addon.size(); i++)
    {
        base.push_back(addon.at(i));
    }
    

    return base;

}


void printGrid(cell **grid, int size){

    int largestString = 2;
    int smallestString = 2;
    
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            
            if(grid[i][j].strings.size() > largestString){
                largestString = grid[i][j].strings.size();
            }
        }
        
    }
    
    
    int temp = largestString + 2 + (largestString -1);
    
    cout << "\n";
    for (int i = 0; i < size; i++)
    {
        
        for (int j = 0; j < size; j++)
        {
            //set the cellstring
            grid[i][j].cellString = vector_toString(grid[i][j].strings);
            //print cell with padding
            printf("%-*s ",temp,(grid[i][j].cellString).c_str());

        }

        cout << "\n";
        
    }
    
}

string vector_toString(vector<string> stringList){

    
    string out = "";
    out += "{";
    for (int i = 0; i < stringList.size(); i++)
    {
        out += stringList.at(i);
        if(i != stringList.size()-1)
            out += ",";
    }
    out += "}";

    return out;

}


CYKLanguage::CYKLanguage(string filename){

    ifstream file;
    file.open(filename);
    if(file.is_open()){

        string input;
        file >> ruleNum;

        cout << "\n" << ruleNum << "\n";


        for(int i = 0; i < ruleNum; i++){

            file >> input;
            parse(input);

            cout << characters.at(i) << "->" << strings.at(i) << "\n";

        }

    }else{

        cout << "\nInvalid Filename Language Could not be built. Exiting Program\n";
        exit(1);

    }

    file.close();




}




vector<string> CYKLanguage::inLanguageValues(vector<string> stringList){

    vector<string> out;

    if(stringList.size() == 0){
        out.push_back("");
        return out;
    }
        

    for(int i = 0; i < stringList.size(); i++){

        for (int j = 0; j < strings.size(); j++){
            
            if(stringList.at(i) == strings.at(j)){

                out.push_back(characters.at(j));
                

            }

        }

    }
    

    if(out.size() == 0){
        out.push_back("");
        return out;
    }


    sort(out.begin(),out.end());

    

    //remove duplicates
    for (int i = 0; i < out.size() - 1; i++)
    {
        while(i < out.size() - 1 && out.at(i) == out.at(i + 1)){
            out.erase(out.begin()+i+1);
        }
    }

    /*cout <<  endl;
    for (int i = 0; i < out.size(); i++)
    {
        cout << out.at(i) << " ";
    }
    cout <<  endl;*/
    

    return out;
}

vector<string> CYKLanguage::inLanguageValues(string stringList){

    vector<string> out;

    if(stringList.size() == 0)
        return out;

    
    for (int j = 0; j < strings.size(); j++){
        
        if(stringList == strings.at(j)){

            out.push_back(characters.at(j));
            

        }

    }
    

    sort(out.begin(),out.end());

    

    //remove duplicates
    for (int i = 0; i < out.size() - 1; i++)
    {
        while(i < out.size() - 1 && out.at(i) == out.at(i + 1)){
            out.erase(out.begin()+i+1);
        }
    }

    /*cout <<  endl;
    for (int i = 0; i < out.size(); i++)
    {
        cout << out.at(i) << " ";
    }
    cout <<  endl;*/

    return out;
}


void CYKLanguage::parse(string input){

    if(input.size() == 4){

        characters.push_back(input.substr(0,1));
        strings.push_back(input.substr(3,1));

    }else if(input.size() == 5){

        characters.push_back(input.substr(0,1));
        strings.push_back(input.substr(3,2));

    }else{

        cout << "\n Invalid Rule Entry, must be 4 or 5 characterse in length with format *->* or *->**. Exiting Program\n";
        exit(1);


    }

}
