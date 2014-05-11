//
//  SqliteUtil.h
//  CCX_Test
//
//  Created by zuffy on 14-5-11.
//
//


#ifndef __CCX_Test__SqliteUtil__
#define __CCX_Test__SqliteUtil__

#include "sqlite3.h"
#include <string>
#include <list>
#include <functional>
#include "ContentValue.h"

#include "cocos2d.h"
USING_NS_CC;

typedef std::function<void(Ref*)> ccSqlCallback;

using namespace std;

class SqliteUtil
{
private:
    std::string dbName = "game2048db.db3";
    sqlite3* pDB;
    char* errMsg;
    static SqliteUtil* instance;
    SqliteUtil(void);
    ~SqliteUtil(void);
public:
    /**获取实例
     */
    static SqliteUtil* getInstance();
    
    bool usable;
    
    /**释放*/
    static void releaseInstance();
    /**
     * 开启数据库
     * @param dbName 数据库名称
     */
    bool openDB();
    /**
     * 关闭数据库
     */
    bool closeDB();
    
    bool tableIsExist( string name );
    /**
     * 创建数据表
     * @param createSql 创建sql语句
     */
    bool createTable(const char* createSql);
    /**
     插入数据
     @param tableName 表名
     @param content  插入的数据
     */
    bool insert(std::string tableName,ContentValue* content);
    /**
     更新数据
     @param tableName 表名
     @param content  需要更新的数据
     @param whereKey 列名
     @param whereArg 值
     */
    bool update(std::string tableName,ContentValue* content,std::string whereKey,std::string whereArg);
    /**
     删除数据
     @param tableName 表名
     @param whereKey 列名
     @param whereArg 值
     */
    bool remove(std::string tableName,std::string whereKey,std::string whereArg);
    bool removeAll(std::string tableName);
    /**
     查询数据
     @param tableName 表名
     @param content  查询到的数据
     @param whereKey 列名
     @param whereArg 值
     */
    bool select(std::string tableName,ContentValue* content,std::string whereKey,std::string whereArg);
    /**
     查询数据
     @param tableName 表名
     @param content  查询到的数据
     */
    bool selectAll(std::string tableName,std::list<ContentValue*>* content);
    
    /**
     查询数据
     @param tableName 表名
     @param content  查询到的数据
     @param whereKey 列名
     @param whereArg 值
     @param callback 回调函数
     */
    bool select(std::string tableName,ContentValue* content,string whereKey,string whereArg, int (*callback)(void*,int,char**,char**));
    /**
     查询数据
     @param tableName 表名
     @param content  查询到的数据
     @param callback 回调函数
     */
    bool selectAll(std::string tableName,list<ContentValue*>* content,int (*callback)(void*,int,char**,char**));
    /**
     执行SQL语句
     @param sql 需要执行的SQL语句
     */
    bool execSQL(std::string sql);
    /**
     执行SQL语句
     @param sql 需要执行的SQL语句
     @param callback 回调函数
     @param 回调函数的第一个参数
     */
    bool execSQL(std::string sql,int (* callback)(void*,int,char**,char**), void *);
};

#endif /* defined(__CCX_Test__SqliteUtil__) */
