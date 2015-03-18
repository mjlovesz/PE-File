#include <windows.h>
#include <iostream>
#include <fstream>


#ifndef UINT8
#define unsigned char  UINT8 ;
#endif // !UINT8


class readfile{
protected:
	char *m_iofileName;	//文件名
	char *m_iofile;	//内容
	long m_lFileSize;//内容大小
	long m_lBufferSize;//缓冲区大小，预留了一部分内存做缓冲，以便增加数据。
public:
	enum OPEN_TYPE
	{
		ReadOnly,
		Write
	};
	enum readFileERROR
	{
		OK = 0
		,MEM_ERR = -1	//内存申请失败
		,FILE_ERR = -2	//文件不存在，打开失败，读取内容失败
		,ARGC_ERR = -3	//参数出错
		//,READ_OVER = -4	//读的东西太多了。我这里并没有那么多。
		,FILE_W_ERR = -5	//写入文件出错

	};

public:
	readfile(LPCSTR pstr,OPEN_TYPE =ReadOnly);
	~readfile();
	virtual int OpenFile(LPCSTR,OPEN_TYPE =ReadOnly);		//打开文件，请检查返回值
	virtual int SaveFile(LPCSTR=NULL);						//保存文件

	int WriteData(char* p,int length,int offset);		//增
	int DeleteData(int length,int offset);				//删
	int UpdateData(char* p,int length,int offset);		//改
	int ReadData(char* p,int length,int offset);		//查.此函数一定要查看返回值是不是和请求长度一样。如果不一样表示文件中没有那么多的内容了。
	const char * ReadData(int offset,int& leftLength);	//查，直接返回内部内存地址,返回值是神圣不可侵犯的。

	long GetSize(){return m_lFileSize;};		//文件大小
	const char * GetFilePoint(){return m_iofile;};//文件第一个指针
protected:
	bool addbuf(int);		//增加buf
	void cleanFile();	//清理文件
};

//int GetData(char* p,int length);//在默认的偏移，读长为length一段到p指针中
//int GetData(char* p,int length,int seek_,int offset);//指定的偏移，相对偏移，读长为length一段到p指针中