/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KBE_ENTITY_TABLE_MYSQL_HPP
#define KBE_ENTITY_TABLE_MYSQL_HPP
#include "db_interface_mysql.hpp"
#include "common.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/singleton.hpp"
#include "helper/debug_helper.hpp"
#include "dbmgr_lib/entity_table.hpp"

namespace KBEngine { 

class ScriptDefModule;
class EntityTableMysql;

#define MYSQL_ENGINE_TYPE "InnoDB"

/*
	维护entity在数据库表中的一个字段
*/
class EntityTableItemMysqlBase : public EntityTableItem
{
public:
	EntityTableItemMysqlBase(std::string itemDBType, uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItem(itemDBType, datalength, flags),
	  mysqlItemtype_(mysqlItemtype)
	{
		memset(db_item_name_, 0, MAX_BUF);
	};

	virtual ~EntityTableItemMysqlBase()
	{
	};

	uint8 type()const{ return TABLE_ITEM_TYPE_UNKONWN; }

	/**
		初始化
	*/
	virtual bool initialize(const PropertyDescription* pPropertyDescription, 
		const DataType* pDataType, std::string name);

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL) = 0;

	/**
		更新数据
	*/
	virtual bool writeItem(DBInterface* dbi, DBID dbid, MemoryStream* s, ScriptDefModule* pModule){ return true; }

	/**
		查询表
	*/
	virtual bool queryTable(DBInterface* dbi, DBID dbid, MemoryStream* s, ScriptDefModule* pModule){ return true; }

	/**
		获取某个表所有的数据放到流中
	*/
	virtual void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID){};

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox) = 0;
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox) = 0;

	virtual void init_db_item_name(const char* exstrFlag = "");
	const char* db_item_name(){ return db_item_name_; }

	virtual bool isSameKey(std::string key){ return key == db_item_name(); }
protected:
	char db_item_name_[MAX_BUF];
	enum_field_types mysqlItemtype_;
};

class EntityTableItemMysql_DIGIT : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_DIGIT(std::string dataSType, std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype),
		  dataSType_(dataSType)
	{
	};

	virtual ~EntityTableItemMysql_DIGIT(){};

	uint8 type()const{ return TABLE_ITEM_TYPE_DIGIT; }

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
protected:
	std::string dataSType_;
};

class EntityTableItemMysql_STRING : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_STRING(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype)
	  {
	  }

	virtual ~EntityTableItemMysql_STRING(){};

	uint8 type()const{ return TABLE_ITEM_TYPE_STRING; }

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
};

class EntityTableItemMysql_UNICODE : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_UNICODE(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype)
	  {
	  }

	virtual ~EntityTableItemMysql_UNICODE(){};

	uint8 type()const{ return TABLE_ITEM_TYPE_UNICODE; }

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
};

class EntityTableItemMysql_PYTHON : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_PYTHON(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype)
	  {
	  }

	virtual ~EntityTableItemMysql_PYTHON(){};

	uint8 type()const{ return TABLE_ITEM_TYPE_PYTHON; }

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
};

class EntityTableItemMysql_BLOB : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_BLOB(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype)
	  {
	  }

	virtual ~EntityTableItemMysql_BLOB(){};

	uint8 type()const{ return TABLE_ITEM_TYPE_BLOB; }

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
};

class EntityTableItemMysql_VECTOR2 : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_VECTOR2(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype)
	  {
	  }

	virtual ~EntityTableItemMysql_VECTOR2(){};

	uint8 type()const{ return TABLE_ITEM_TYPE_VECTOR2; }
	
	virtual bool isSameKey(std::string key);

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);

	virtual void init_db_item_name(const char* exstrFlag = "")
	{
		for(int i=0; i<2; i++)
			kbe_snprintf(db_item_names_[i], MAX_BUF, TABLE_ITEM_PERFIX"_%d_%s%s", i, exstrFlag, itemName());
	}

protected:
	char db_item_names_[2][MAX_BUF];
};

class EntityTableItemMysql_VECTOR3 : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_VECTOR3(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype)
	  {
	  }

	virtual ~EntityTableItemMysql_VECTOR3(){};

	uint8 type()const{ return TABLE_ITEM_TYPE_VECTOR3; }

	virtual bool isSameKey(std::string key);

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);

	virtual void init_db_item_name(const char* exstrFlag = "")
	{
		for(int i=0; i<3; i++)
			kbe_snprintf(db_item_names_[i], MAX_BUF, TABLE_ITEM_PERFIX"_%d_%s%s", i, exstrFlag, itemName());
	}

protected:
	char db_item_names_[3][MAX_BUF];
};

class EntityTableItemMysql_VECTOR4 : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_VECTOR4(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype)
	  {
	  }

	virtual ~EntityTableItemMysql_VECTOR4(){};

	uint8 type()const{ return TABLE_ITEM_TYPE_VECTOR4; }

	virtual bool isSameKey(std::string key);

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);

	virtual void init_db_item_name(const char* exstrFlag = "")
	{
		for(int i=0; i<4; i++)
			kbe_snprintf(db_item_names_[i], MAX_BUF, TABLE_ITEM_PERFIX"_%d_%s%s", i, exstrFlag, itemName());
	}

protected:
	char db_item_names_[4][MAX_BUF];
};

class EntityTableItemMysql_MAILBOX : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_MAILBOX(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype)
	  {
	  }

	virtual ~EntityTableItemMysql_MAILBOX(){};

	uint8 type()const{ return TABLE_ITEM_TYPE_MAILBOX; }

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
};

class EntityTableItemMysql_ARRAY : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_ARRAY(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype),
	  pChildTable_(NULL)
	  {
	  }

	virtual ~EntityTableItemMysql_ARRAY(){};

	virtual bool isSameKey(std::string key);

	/**
		初始化
	*/
	virtual bool initialize(const PropertyDescription* pPropertyDescription, 
		const DataType* pDataType, std::string name);

	uint8 type()const{ return TABLE_ITEM_TYPE_FIXEDARRAY; }

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);

	virtual void init_db_item_name(const char* exstrFlag = "");

protected:
	EntityTable* pChildTable_;
};

class EntityTableItemMysql_FIXED_DICT : public EntityTableItemMysqlBase
{
public:
	EntityTableItemMysql_FIXED_DICT(std::string itemDBType, 
		uint32 datalength, uint32 flags, enum_field_types mysqlItemtype):
	  EntityTableItemMysqlBase(itemDBType, datalength, flags, mysqlItemtype)
	  {
	  }

	virtual ~EntityTableItemMysql_FIXED_DICT(){};

	typedef std::vector< std::pair< std::string, KBEShared_ptr<EntityTableItem> > > FIXEDDICT_KEYTYPES;

	uint8 type()const{ return TABLE_ITEM_TYPE_FIXEDDICT; }

	virtual bool isSameKey(std::string key);

	/**
		初始化
	*/
	virtual bool initialize(const PropertyDescription* pPropertyDescription, 
		const DataType* pDataType, std::string name);

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi, void* pData = NULL);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);

	virtual void init_db_item_name(const char* exstrFlag = "");
protected:
	EntityTableItemMysql_FIXED_DICT::FIXEDDICT_KEYTYPES			keyTypes_;		// 这个固定字典里的各个key的类型
};


/*
	维护entity在数据库中的表
*/
class EntityTableMysql : public EntityTable
{
public:
	EntityTableMysql();
	virtual ~EntityTableMysql();
	
	/**
		初始化
	*/
	virtual bool initialize(ScriptDefModule* sm, std::string name);

	/**
		同步entity表到数据库中
	*/
	virtual bool syncToDB(DBInterface* dbi);

	/** 
		创建一个表item
	*/
	virtual EntityTableItem* createItem(std::string type);

	DBID writeTable(DBInterface* dbi, DBID dbid, MemoryStream* s, ScriptDefModule* pModule);

	/**
		从数据库删除entity
	*/
	bool removeEntity(DBInterface* dbi, DBID dbid, ScriptDefModule* pModule);

	/**
		获取所有的数据放到流中
	*/
	virtual bool queryTable(DBInterface* dbi, DBID dbid, MemoryStream* s, ScriptDefModule* pModule);

	/**
		获取某个表所有的数据放到流中
	*/
	void addToStream(MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox, DBID resultDBID);

	/**
		获取需要存储的表名， 字段名和转换为sql存储时的字符串值
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);
	virtual void getReadSqlItem(DB_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox);

	void init_db_item_name();
protected:
	
};


}

#ifdef CODE_INLINE
#include "entity_table_mysql.ipp"
#endif
#endif // KBE_ENTITY_TABLE_MYSQL_HPP
