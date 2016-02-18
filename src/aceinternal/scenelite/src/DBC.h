/*
|==========================================
|	DBC数据库文件类
|		(服务器/客户端通用)
|==========================================
|
|		--------------------
|		|  数据库文件格式  |
|		--------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|	Head
|		0X000	  int		DBC File Identity， always 0XDDBBCC00
|		0X004	  int      Number of records in the file 
|		0X008     int      Number of 4-byte fields per record
|		0X010     int      String block size 
|   FieldType
|		0X014     int[FieldNum]   
|							  The type fo fields(0-int, 1-float, 2-string)
|   FieldBlock
|				  int[FieldNum*RecordNum]
|							  DataBlock
|	StringBlock
|				  char[StringSize]
|							  StringBlock
|
*/
#ifndef _DBC_H_
#define _DBC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <map>

class DBCFile	
{
public:
	//文件头
	struct FILE_HEAD
	{
		unsigned int		m_Identify;				//标示	0XDDBBCC00
		int			m_nFieldsNum;			//列数
		int			m_nRecordsNum;			//行数
		int			m_nStringBlockSize;		//字符串区大小
	};

	//字段数据类型
	enum FIELD_TYPE
	{
		T_INT		= 0,	//整数
		T_FLOAT		= 1,	//浮点数
		T_STRING	= 2,	//字符串
		T_UNKNOW	= 3,	//不确定类型
	};

	//数据库格式描述
	typedef std::vector< FIELD_TYPE >	FILEDS_TYPE;

	//数据段
	struct FIELD
	{
		union {
		float		fValue;
		int			iValue;
		int		wValue;
		char		cValue;
		const char*	pString;	// Just for runtime!
		};

		bool isempty;

		//Construct
		FIELD() {}
		FIELD(int value) { iValue = value; }
		FIELD(float value) { fValue = value; }
		FIELD(const char* value) { pString = value; }
	};
	//数据区
	typedef std::vector< FIELD >		DATA_BUF;

public:
	//打开文本文件，生成一个数据库
	bool					OpenFromTXT(const char* szFileName);
	//根据内存中的文件打开
	bool					OpenFromMemory(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
protected:
	//读取文本格式内容
	bool					OpenFromMemoryImpl_Text(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
	//读取二进制格式内容
	bool					OpenFromMemoryImpl_Binary(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);

public:
	//按索引查找(第一列为索引)
	virtual const FIELD*	Get(int nValue) const;
	//按照位置查找
	virtual const FIELD*	Get(int nRecordLine, int nColumNum) const;
	//查找某列等于指定值的第一行
	virtual const FIELD*	Get(int nColumnNum, const FIELD& value) const;

public:
	//取得ID
	int GetID(void) const				{ return m_ID; }
	//取得列数
	int	GetFieldsNum(void) const	    { return m_nFieldsNum; }
	//取得记录的条数
	int GetRecordsNum(void) const		{ return m_nRecordsNum; }
	//生成索引列
	void CreateIndex(int nColum = 0, const char* szFileName=0);

protected:
	typedef std::map< int, FIELD*>	FIELD_HASHMAP;
	//数据库格式文件名
	int			m_ID;
	//数据库格式描述
	FILEDS_TYPE				m_theType;
	//行数
	int						m_nRecordsNum;
	//列数
	int						m_nFieldsNum;
	//数据区
	DATA_BUF				m_vDataBuf;		//size = m_nRecordsNum*m_nFieldsNum
	//字符串区
	char*					m_pStringBuf;
	//字符串区大小
	int						m_nStringBufSize;
	//索引表
	FIELD_HASHMAP			m_hashIndex;
	//索引列
	int						m_nIndexColum;

public:

	static int			_ConvertStringToVector(const char* strStrintgSource, std::vector< std::string >& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty);
	//从内存中字符串读取一行文本(按照换行符)
	static const char*	_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd);
	//比较两个值是否相等
	template < FIELD_TYPE T>	
	static bool			_FieldEqu(const FIELD& a, const FIELD& b);

public:
	DBCFile(int id);
	virtual ~DBCFile();
};

#endif