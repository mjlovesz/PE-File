#include "readFile.h"
using namespace std;


readfile::readfile(LPCSTR pstr,OPEN_TYPE openType)
{
	m_iofileName = NULL;
	m_iofile = NULL;
	m_lFileSize = 0;
	OpenFile(pstr);
}
readfile::~readfile()
{
	cleanFile();
}
int readfile::OpenFile(LPCSTR pstr,OPEN_TYPE openType)
{
	cleanFile();//先清空文件
	ifstream iofile = std::ifstream(pstr,ios::binary);
	if (!iofile)
	{
		return FILE_ERR;
	}

	//获得文件大小
	iofile.seekg(0,ios::end);
	m_lFileSize = iofile.tellg();//如果文件超过2^32（4G）,那就算了吧

	//将文件保存在内存中：分配内存
	if (openType == ReadOnly)
	{
		m_lBufferSize = m_lFileSize;
	}
	else
	{
		m_lBufferSize = m_lFileSize << 1;
	}
	m_iofile = (char*)malloc(sizeof(char) * m_lBufferSize);
	if(!m_iofile)
	{
		m_lFileSize = 0;
		iofile.close();
		return MEM_ERR;//内存申请失败
	}

	//将文件保存在内存中：将文件读取出来
	iofile.seekg(ios::beg);
	iofile.read(m_iofile,m_lFileSize);
	if (iofile.good())
	{
		m_iofileName = (char*)malloc((strlen(pstr)+1)*sizeof(char));
		strcpy_s(m_iofileName,strlen(pstr)+1,pstr);
		iofile.close();
		return OK;
	}

	m_lFileSize = 0;
	iofile.close();
	return FILE_ERR;//读取失败
}

int readfile::SaveFile(LPCSTR pstr)
{
	if(!m_iofile)
		return FILE_ERR;

	if (pstr==NULL)
	{
		pstr = m_iofileName;
	}
	ofstream iofile = ofstream(pstr,ios::binary|ios::trunc);
	if (!iofile)
	{
		return FILE_W_ERR;
	}

	//将保存在文件中
	iofile.seekp(ios::beg);
	iofile.write(m_iofile,m_lFileSize);
	if (iofile.good())
	{
		iofile.close();
		return OK;
	}
	iofile.close();
	return FILE_W_ERR;//读取失败
}

int readfile::WriteData(char* p,int length,int offset)	//增
{
	if(!m_iofile)
		return FILE_ERR;

	if (offset < 0 ||offset > m_lFileSize || p == NULL || length < 0)
	{
		return ARGC_ERR;
	}

	if (m_lFileSize + length > m_lBufferSize)//如果内存不够了的话就分配内存
	{
		if (!addbuf(m_lFileSize + length))
		{
			return MEM_ERR;//失败
		}
	}
	memmove(m_iofile + offset + length,m_iofile + offset,m_lFileSize - offset);//内存向后移
	memmove(m_iofile + offset, p ,length);
	m_lFileSize += length;
	return length;
}
int readfile::DeleteData(int length,int offset)		//删
{
	if(!m_iofile)
	{
		return FILE_ERR;
	}
	if (offset < 0 || offset + length > m_lFileSize || length < 0)
	{
		return ARGC_ERR;
	}
	memmove(m_iofile + offset, m_iofile + offset + length, m_lFileSize - offset - length);
	m_lFileSize -= length; 
	return length;
}
int readfile::UpdateData(char* p,int length,int offset)	//改
{
	if(!m_iofile)
	{
		return FILE_ERR;
	}

	if (offset < 0 ||offset > m_lFileSize || p == NULL || length < 0)
	{
		return ARGC_ERR;
	}

	if (offset + length > m_lBufferSize)//如果内存不够了的话就分配内存
	{
		if (!addbuf(offset + length))
		{
			return MEM_ERR;//失败
		}
	}

	memmove((void*)(m_iofile+offset), (void*)p, length);

	if (offset + length > m_lFileSize)//改变长度
	{
		m_lFileSize = offset + length;
	}

	return length;
}
int readfile::ReadData(char* p,int length,int offset)	//查
{
	if(!m_iofile)
	{
		return FILE_ERR;
	}
	if (offset < 0 || length < 0 || offset >m_lFileSize)
	{
		return ARGC_ERR;
	}
	if (offset + length > m_lFileSize )
	{
		length = m_lFileSize - offset;
	}
	memmove(p, m_iofile + offset, length);

	return length;
}

const char * readfile::ReadData(int offset,int& leftLength)//查，直接返回内部内存地址
{
	if(!m_iofile)
	{
		leftLength = FILE_ERR;
		return 0;
	}
	if (offset < 0 || offset >m_lFileSize)
	{
		leftLength = ARGC_ERR;
		return 0;
	}
	
	leftLength = m_lFileSize - offset;

	return m_iofile + offset;
}

bool readfile::addbuf(int nsize)		//增加buf
{
	if (!m_iofile)
	{
		return false;
	}
	int n = 1;
	while ((nsize > m_lBufferSize << n ) && (n < 30))
		n++;
	void * new_ptr = (char*)realloc(m_iofile,m_lBufferSize<<n);
	if (new_ptr == NULL)
	{
		return false;
	}
	m_iofile = (char*)new_ptr;
	m_lBufferSize <<=n;
	return true;
}

void readfile::cleanFile()	//清理文件
{
	if (m_iofile)
	{
		delete m_iofile;
	}
	if (m_iofileName)
	{
		delete m_iofileName;
	}
}


