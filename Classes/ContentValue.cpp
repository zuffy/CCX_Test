//
//  ContentValue.cpp
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//

#include "ContentValue.h"
ContentValue::ContentValue(){};
ContentValue::~ContentValue(){};

void ContentValue::setValue(char * column_name, char * column_value){
    if (0 == strncmp(column_name, "score", sizeof("score"))) {
        this->score = stoi( column_value );
    }
    if (0 == strncmp(column_name, "hScore", sizeof("hScore"))) {
        this->hScore = stoi( column_value );
    }
    if (0 == strncmp(column_name, "map", sizeof("map"))) {
        this->map = string(column_value);
    }
    if (0 == strncmp(column_name, "lastHelp", sizeof("lastHelp"))) {
        this->lastHelp = stoi( column_value );
    }
    if (0 == strncmp(column_name, "lastHelpTime", sizeof("lastHelpTime"))) {
        this->lastHelpTime = stoi( column_value );
    }
}

string ContentValue::getAllKey()
{
    return "score,hScore,map,lastHelp,lastHelpTime";
}

string ContentValue::getAllValue()
{
    return "100,200,\"0/0/2\",2,102588888";
}

string ContentValue::getAll()
{
    return "";
}

