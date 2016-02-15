#include <assert.h>
#include <string>
#include "DBC.h"

#define MAX_FILE_PATH  260

int	DBCFile::_ConvertStringToVector(const char* strStrintgSource, std::vector< std::string >& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty)
{
	vRet.clear();
	
	std::string strSrc = strStrintgSource;
	if(strSrc.empty())
	{
		return 0;
	}

	std::string::size_type nLeft = 0;
	std::string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}
	while(true)
	{
		std::string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));
		
		if(bOneOfKey)
		{
			std::string strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != std::string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (int)vRet.size();
}

template < DBCFile::FIELD_TYPE T>
bool DBCFile::_FieldEqu(const FIELD& a, const FIELD& b)
{
	if(T == T_INT)
	{
		return a.iValue==b.iValue;
	}
	else if(T == T_FLOAT)
	{
		return a.fValue==b.fValue;
	}
	else
	{
		try
		{
			return (strcmp(a.pString, b.pString)==0);
		}
		catch(...)
		{
			return false;
		}
	}
}

DBCFile::DBCFile(int id)
{
	m_ID = id;
	m_pStringBuf = NULL;
	m_nIndexColum = -1;
}

DBCFile::~DBCFile()
{
	if(m_pStringBuf) delete[] m_pStringBuf;
	m_pStringBuf = NULL;
}

//从内存中字符串读取一行文本(按照换行符)
const char* DBCFile::_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd)
{
	//寻找下一个换行符
	register const char* pMem = pMemory;
	if(pMem >= pDeadEnd || *pMem==0) return 0;

	while(pMem < pDeadEnd && pMem-pMemory+1<nBufSize && 
			*pMem != 0 && *pMem != '\r' && *pMem != '\n') *(pStringBuf++) = *(pMem++);
	//add 'null' end
	*pStringBuf = 0;

	//skip all next \r and \n
	while(pMem < pDeadEnd && *pMem != 0 && (*pMem=='\r' || *pMem=='\n')) pMem++;

	return pMem;
}

bool DBCFile::OpenFromMemory(const char* pMemory, const char* pDeadEnd, const char* szFileName)
{
	assert(pMemory && pDeadEnd);
	//----------------------------------------------------
	//判断是否是二进制格式
	if(pDeadEnd - pMemory >=sizeof(FILE_HEAD) && *((int*)pMemory)==0XDDBBCC00)
	{
		return OpenFromMemoryImpl_Binary(pMemory, pDeadEnd, szFileName);
	}
	else
	{
		return OpenFromMemoryImpl_Text(pMemory, pDeadEnd, szFileName);
	}
}

bool DBCFile::OpenFromMemoryImpl_Binary(const char* pMemory, const char* pDeadEnd, const char* szFileName)
{
	register const char* pMem = pMemory;

	//----------------------------------------------------
	//Read Head
	FILE_HEAD theHead;
	memcpy(&theHead, pMem, sizeof(FILE_HEAD));
	if(theHead.m_Identify != 0XDDBBCC00 )
	{
		return false;
	}
	//check memory size
	if(	sizeof(FILE_HEAD) + 
		sizeof(int)*theHead.m_nFieldsNum + 
		sizeof(FIELD)*theHead.m_nRecordsNum * theHead.m_nFieldsNum +
		theHead.m_nStringBlockSize > (int)(pDeadEnd-pMemory))
	{
		return false;
	}

	pMem += sizeof(FILE_HEAD);

	//----------------------------------------------------
	//Init 
	m_nRecordsNum = theHead.m_nRecordsNum;
	m_nFieldsNum  = theHead.m_nFieldsNum;
	m_nStringBufSize = theHead.m_nStringBlockSize;

	//---------------------------------------------
	//Create String Blok
	m_pStringBuf = new char[theHead.m_nStringBlockSize];
	if(!m_pStringBuf) return false;

	//------------------------------------------------------
	// Read Field Types
	std::vector< int > vFieldType;
	vFieldType.resize(theHead.m_nFieldsNum);
	memcpy(&(vFieldType[0]), pMem, sizeof(int)*theHead.m_nFieldsNum);
	pMem += sizeof(int)*theHead.m_nFieldsNum;

	//Check it!
	m_theType.resize(theHead.m_nFieldsNum);
	for(int i=0; i<(int)theHead.m_nFieldsNum; i++)
	{
		switch(vFieldType[i])
		{
		case T_INT:
		case T_FLOAT:
		case T_STRING:
			m_theType[i] = (FIELD_TYPE)vFieldType[i];
			break;

		default:
			delete[] (m_pStringBuf);
			return false;
		}
	}

	//------------------------------------------------------
	// Read All Field
	m_vDataBuf.resize(theHead.m_nRecordsNum * theHead.m_nFieldsNum);
	memcpy(&(m_vDataBuf[0]), pMem, sizeof(FIELD)*theHead.m_nRecordsNum * theHead.m_nFieldsNum);
	pMem += sizeof(FIELD)*theHead.m_nRecordsNum * theHead.m_nFieldsNum;

	//------------------------------------------------------
	// Read String Block
	memcpy(m_pStringBuf, pMem, m_nStringBufSize);
	m_pStringBuf[m_nStringBufSize-1]=0;

	//------------------------------------------------------
	// TO runtime address
	for(int j=0; j<(int)theHead.m_nFieldsNum; j++)
	{
		if(vFieldType[j] != T_STRING) continue;

		for(int i=0; i<(int)theHead.m_nRecordsNum; i++)
		{
			
			m_vDataBuf[i*GetFieldsNum()+j].pString += reinterpret_cast<unsigned long long>(m_pStringBuf);
		}
	}

	//------------------------------------------------------
	//生成索引列
	CreateIndex(0, szFileName);

	return true;
}

bool DBCFile::OpenFromMemoryImpl_Text(const char* pMemory, const char* pDeadEnd, const char* szFileName)
{
	//----------------------------------------------------
	//分析列数和类型
	char szLine[1024*10] = {0};
	//读第一行
	register const char* pMem = pMemory;
	pMem = _GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem) return false;

	//分解
	std::vector< std::string > vRet;
	_ConvertStringToVector(szLine, vRet, "\t", true, true);
	if(vRet.empty()) return false;
	//生成Field Types
	FILEDS_TYPE vFieldsType;
	vFieldsType.resize(vRet.size());

	for(int i=0; i<(int)vRet.size(); i++) 
	{
		if(vRet[i] == "INT") vFieldsType[i] = T_INT;
		else if(vRet[i] == "FLOAT") vFieldsType[i] = T_FLOAT;
		else if(vRet[i] == "STRING") vFieldsType[i] = T_STRING;
		else
		{
			vFieldsType[i] = T_UNKNOW;
			//return false;
		}
	}

	//--------------------------------------------------------------
	//初始化
	int nRecordsNum	= 0;
	int nFieldsNum	= (int)vFieldsType.size();

	//临时字符串区
	std::vector< std::pair< std::string, int > >	vStringBuf;
	//检索表
	std::map< std::string, int >					mapStringBuf;

	//--------------------------------------------------------------
	//开始读取

	//空读一行
	pMem = _GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem) return false;

	int nStringBufSize = 0;
	do
	{
		//读取一行
		pMem = _GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
		if(!pMem) break;;

		//是否是注释行
		if(szLine[0] == '#') continue;

		//分解
		_ConvertStringToVector(szLine, vRet, "\t", true, false);

		//列数不对
		if(vRet.empty()) continue;
        if(vRet.size() != nFieldsNum) 
		{
			//补上空格
			if((int)vRet.size() < nFieldsNum)
			{
				int nSubNum = nFieldsNum-(int)vRet.size();
				for(int i=0; i<nSubNum; i++)
				{
					vRet.push_back("");
				}
			}
		}

		//第一列不能为空
		if(vRet[0].empty()) continue;

		for(register int i=0; i<nFieldsNum; i++)
		{
			FIELD newField;

			newField.isempty = false;
			
			switch(vFieldsType[i])
			{
			case T_INT:
				if(vRet[i].empty())
				{
					newField.isempty = true;
					newField.iValue = 0;
				}
				else
				{
					newField.iValue = atoi(vRet[i].c_str());
				}

				m_vDataBuf.push_back(newField);
				break;

			case T_FLOAT:

				if(vRet[i].empty())
				{
					newField.isempty = true;
					newField.fValue = 0.0f;
				}
				else
				{
					newField.fValue = (float)atof(vRet[i].c_str());
				}
				m_vDataBuf.push_back(newField);
				break;

			case T_UNKNOW:
			case T_STRING:
				if(vRet[i].empty())
				{
					newField.isempty = true;
					newField.iValue = 0;
				}
				else
				{
					const char * p = vRet[i].c_str();
					std::map< std::string, int >::iterator it = mapStringBuf.find(vRet[i]);
					if(it == mapStringBuf.end())
					{
						vStringBuf.push_back(std::make_pair(vRet[i], nStringBufSize));
						mapStringBuf.insert(std::make_pair(vRet[i], (int)vStringBuf.size()-1));
						newField.iValue = nStringBufSize + 1; // first char is '\0' for blank string
	
						nStringBufSize += (int)strlen(vRet[i].c_str()) + 1;
					}
					else
					{
						newField.iValue = vStringBuf[it->second].second + 1;
					}
				}

				m_vDataBuf.push_back(newField);
				break;
			}
		}

		nRecordsNum++;
	}while(true);

	//--------------------------------------------------------
	//生成正式数据库
	m_nRecordsNum = nRecordsNum;
	m_nFieldsNum  = nFieldsNum;
	m_nStringBufSize = nStringBufSize+1;

	//Create String Blok
	m_pStringBuf = new char[m_nStringBufSize];
	if(!m_pStringBuf) return false;

	//------------------------------------------------------
	// Create Field Types
	m_theType = vFieldsType;

	//------------------------------------------------------
	// Create String Block
	unsigned char byBlank = '\0';
	m_pStringBuf[0] = '\0';

	register char* p = m_pStringBuf + 1;
	for(int i=0; i<(int)vStringBuf.size(); i++)
	{
		memcpy(p, vStringBuf[i].first.c_str(), vStringBuf[i].first.size());	
		p += vStringBuf[i].first.size();

		*(p++) = '\0';
	}

	//------------------------------------------------------
	// Relocate String Block
	for(register int i=0; i<nFieldsNum; i++)
	{
		if(vFieldsType[i] != T_STRING) continue;

		for(register int j=0; j<nRecordsNum; j++)
		{
			FIELD& theField = m_vDataBuf[j*nFieldsNum+i];
			theField.pString = m_pStringBuf + theField.iValue;
		}
	}

	//------------------------------------------------------
	//生成索引列
	CreateIndex(0, szFileName);

	return true;
}

bool DBCFile::OpenFromTXT(const char* szFileName)
{
	assert(szFileName);

	//----------------------------------------------------
	//打开文件
	FILE* fp = fopen(szFileName, "rb");
	if(NULL == fp) return false;

	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//读入内存
	char* pMemory = new char[nFileSize+1];
	fread(pMemory, 1, nFileSize, fp);
	pMemory[nFileSize] = 0;

	bool bRet = OpenFromMemory(pMemory, pMemory+nFileSize+1, szFileName);

	delete[] pMemory; pMemory = 0;
	return bRet;
}

void DBCFile::CreateIndex(int nColum, const char* szFileName)
{
	if(nColum <0 || nColum >= m_nFieldsNum || m_nIndexColum==nColum) return;

	m_hashIndex.clear();

	for(int i=0; i<(int)m_nRecordsNum; i++)
	{
		FIELD* p = &(m_vDataBuf[i*m_nFieldsNum]);

		FIELD_HASHMAP::iterator itFind = m_hashIndex.find(p->iValue);
		if(itFind != m_hashIndex.end())
		{
//			char szTemp[260];
//			tsnprintf(szTemp, 260, "[%s]Multi index at line %d(SAME:value=%d)", szFileName, i+1, p->iValue);
//#ifdef GAME_CLIENT
//			throw std::string(szTemp);
//#else
//			AssertEx(false,szTemp);
//#endif
			return;
		}
		m_hashIndex.insert(std::make_pair(p->iValue, p));
	}
}

const DBCFile::FIELD* DBCFile::Get(int iIndex) const
{
	FIELD_HASHMAP::const_iterator itFind = m_hashIndex.find(iIndex);
	if(itFind == m_hashIndex.end()) return NULL;

	return itFind->second;
}

const DBCFile::FIELD* DBCFile::Get(int nRecordLine, int nColumNum) const
{
	int nPosition = nRecordLine*GetFieldsNum() + nColumNum;

	if(nPosition <0 || nPosition >= (int)m_vDataBuf.size()) 
	{
		char szTemp[260];
//		tsnprintf(szTemp, MAX_FILE_PATH, "DBC:Invalid search request[rec=%d, col=%d]", nRecordLine, nColumNum);
//#ifdef GAME_CLIENT
//		throw std::string(szTemp);
//#else
//		AssertEx(false, szTemp);
//#endif
		return NULL;
	}

	return &(m_vDataBuf[nPosition]);
}

//查找某列等于指定值的第一行
const DBCFile::FIELD* DBCFile::Get(int nColumnNum, const FIELD& value) const
{
	if(nColumnNum < 0 || nColumnNum >= m_nFieldsNum) return 0;
	FIELD_TYPE type = m_theType[nColumnNum];

	for(register int i=0; i<m_nRecordsNum; i++)
	{
		const FIELD& theField = m_vDataBuf[m_nFieldsNum*i + nColumnNum];
		
		bool bRet;
		if(T_INT == type) 
			bRet = _FieldEqu<T_INT>(theField, value);
		else if(T_FLOAT == type)
			bRet = _FieldEqu<T_FLOAT>(theField, value);
		else
			bRet = _FieldEqu<T_STRING>(theField, value);

		if(bRet) return &(m_vDataBuf[m_nFieldsNum*i]);
	}

	return 0;
}