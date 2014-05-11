//
//  ContentValue.h
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//

#ifndef __CCX_Test__ContentValue__
#define __CCX_Test__ContentValue__

#include <string>
using namespace std;

class ContentValue {
public:
    std::string SQL_TABLE = "game2048_table";
    std::string create_table = "CREATE TABLE student(ID INT PRIMARY KEY AUTOINCREMENT, score INTEGER, hScore INTEGER, map NVARCHAR(2048), lastHelp INTEGER, lastHelpTime UNSIGNED BIG INT)";
    
    int score;
    int hScore;
    string map;
    int lastHelp;
    int lastHelpTime;
    
    void setValue(char * column_name, char * column_value);
    
    string getAllKey();
    
    string getAllValue();
    
    string getAll();
    
    ContentValue();
    ~ContentValue();
};
#endif /* defined(__CCX_Test__ContentValue__) */
