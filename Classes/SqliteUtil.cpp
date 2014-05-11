//
//  SqliteUtil.cpp
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//

#include "SqliteUtil.h"

int selectCallBack(void * para, int n_column, char ** column_value, char ** column_name )
{
    ContentValue* value = (ContentValue *)para;
    for (int i = 0; i < n_column; i++)
    {
        value->setValue(column_name[i],column_value[i]);
    }
    return 0;
}
int selectAllCallBack(void * para, int n_column, char ** column_value, char ** column_name )
{
    list<ContentValue*>* value = (list<ContentValue*>*)para;
    ContentValue* raw = new ContentValue();
    for (int i = 0; i < n_column; i++)
    {
        raw->setValue(column_name[i],column_value[i]);
    }
    value->push_back(raw);
    return 0;
}

SqliteUtil::SqliteUtil(void)
{
    usable = openDB();
}


SqliteUtil::~SqliteUtil(void)
{
    if (usable) {
        closeDB();
    }
}

SqliteUtil* SqliteUtil::getInstance()
{
    
    if(instance == NULL)
    {
        instance = new SqliteUtil();
    }
    return instance;
}

void SqliteUtil::releaseInstance()
{
    if(SqliteUtil::getInstance() != NULL)
    {
        delete SqliteUtil::getInstance();
    }
}

/**
 * CCFileUtils::sharedFileUtils()->getWriteablePath()+"save.db3";
 */
bool SqliteUtil::openDB()
{
    std::string path ;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    path=CCFileUtils::sharedFileUtils()->getWritablePath()+ this->dbName;
#else
    path= this->dbName;
#endif
    int result = sqlite3_open(path.c_str(),&(SqliteUtil::getInstance()->pDB));
    if(result != SQLITE_OK)
    {
        CCLOG("create  database failed，error code:%d,error reason：%s",result,errMsg);
        return false;
    }
    return true;
}

bool SqliteUtil::closeDB()
{
    int result = sqlite3_close(SqliteUtil::getInstance()->pDB);
    if(result != SQLITE_OK)
    {
        CCLOG("close  database failed，error code:%d,error reason：%s",result,errMsg);
        return false;
    }
    return true;
}

//tableIsExist的回调函数
int isExisted( void * para, int n_column, char ** column_value, char ** column_name )
{
    bool *isExisted_=(bool*)para;
    *isExisted_=(**column_value)!='0';
    return 0;
}

//判断表格是否存在
bool SqliteUtil::tableIsExist( string name )
{
    if (pDB!=NULL)
    {
        //判断表是否存在
        bool tableIsExisted;
        string sqlstr = "select count(type) from "+ dbName +" where type='table' and name ='"+name+"'";
        sqlite3_exec(pDB,sqlstr.c_str(),isExisted,&tableIsExisted,&errMsg);
        return tableIsExisted;
    }
    return false;
}

/**
 * "create table student(ID integer primary key autoincrement,name text,sex text)"
 */
bool SqliteUtil::createTable(const char* execSql)
{
    return execSQL(execSql);
}

/**
 * "insert into student (name,sex) values(1,'zhycheng','male')"
 */
bool SqliteUtil::insert(const string tableName,ContentValue* content)
{
    string sql;
    sql = "insert into "+tableName +"("+content->getAllKey()+") values ("+content->getAllValue()+")";
    return execSQL(sql);
}

/**
 * "update student set name=value1,sex=value2 where ID=1"
 */
bool SqliteUtil::update(std::string tableName,ContentValue* content,string whereKey,string whereArg)
{
    string sql;
    sql = "update "+tableName +" set "+content->getAll()+ " where "+whereKey +"=" +whereArg;
    return execSQL(sql);
}

/**
 * "delete from student where ID=1"
 */
bool SqliteUtil::remove(std::string tableName,string whereKey,string whereArg)
{
    string sql;
    sql = "delete from "+tableName +" where "+whereKey +"=" +whereArg;
    return execSQL(sql);
}

bool SqliteUtil::removeAll(std::string tableName)
{
    string sql;
    sql = "delete from "+tableName ;
    return execSQL(sql);
}

bool SqliteUtil::select(std::string tableName,ContentValue* content,string whereKey,string whereArg)
{
    string sql;
    sql = "select * from "+tableName + "where "+whereKey +"='" +whereArg+"';";
    return execSQL(sql,selectCallBack,(void *)content);
}

bool SqliteUtil::selectAll(std::string tableName,list<ContentValue*>* content)
{
    string sql;
    sql = "select * from "+tableName;
    return execSQL(sql,selectAllCallBack,(void *)content);
}

bool SqliteUtil::select(std::string tableName,ContentValue* content,string whereKey,string whereArg, int (*callback)(void*,int,char**,char**))
{
    string sql;
    sql = "select * from "+tableName + "where "+whereKey +"=" +whereArg;
    return execSQL(sql,callback,(void *)content);
}

bool SqliteUtil::selectAll(std::string tableName,list<ContentValue*>* content,int (*callback)(void*,int,char**,char**))
{
    string sql;
    sql = "select * from "+tableName;
    return execSQL(sql,callback,(void *)content);
}

bool SqliteUtil::execSQL(string sql,int (*callback)(void*,int,char**,char**), void * param)
{
    int result = sqlite3_exec(SqliteUtil::getInstance()->pDB,sql.c_str(),callback,param,&errMsg);
    if(result != SQLITE_OK)
    {
        CCLOG("sqlite error，error code:%d,error reason：%s",result,errMsg);
        return false;
    }
    return true;
}

bool SqliteUtil::execSQL(string sql)
{
    int result = sqlite3_exec(SqliteUtil::getInstance()->pDB,sql.c_str(),NULL,NULL,&errMsg);
    if(result != SQLITE_OK)
    {
        CCLOG("sqlite error,error code:%d,error reason：%s",result,errMsg);
        return false;
    }
    return true;
}