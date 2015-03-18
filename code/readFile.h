#include <windows.h>
#include <iostream>
#include <fstream>


#ifndef UINT8
#define unsigned char  UINT8 ;
#endif // !UINT8


class readfile{
protected:
	char *m_iofileName;	//�ļ���
	char *m_iofile;	//����
	long m_lFileSize;//���ݴ�С
	long m_lBufferSize;//��������С��Ԥ����һ�����ڴ������壬�Ա��������ݡ�
public:
	enum OPEN_TYPE
	{
		ReadOnly,
		Write
	};
	enum readFileERROR
	{
		OK = 0
		,MEM_ERR = -1	//�ڴ�����ʧ��
		,FILE_ERR = -2	//�ļ������ڣ���ʧ�ܣ���ȡ����ʧ��
		,ARGC_ERR = -3	//��������
		//,READ_OVER = -4	//���Ķ���̫���ˡ������ﲢû����ô�ࡣ
		,FILE_W_ERR = -5	//д���ļ�����

	};

public:
	readfile(LPCSTR pstr,OPEN_TYPE =ReadOnly);
	~readfile();
	virtual int OpenFile(LPCSTR,OPEN_TYPE =ReadOnly);		//���ļ������鷵��ֵ
	virtual int SaveFile(LPCSTR=NULL);						//�����ļ�

	int WriteData(char* p,int length,int offset);		//��
	int DeleteData(int length,int offset);				//ɾ
	int UpdateData(char* p,int length,int offset);		//��
	int ReadData(char* p,int length,int offset);		//��.�˺���һ��Ҫ�鿴����ֵ�ǲ��Ǻ����󳤶�һ���������һ����ʾ�ļ���û����ô��������ˡ�
	const char * ReadData(int offset,int& leftLength);	//�飬ֱ�ӷ����ڲ��ڴ��ַ,����ֵ����ʥ�����ַ��ġ�

	long GetSize(){return m_lFileSize;};		//�ļ���С
	const char * GetFilePoint(){return m_iofile;};//�ļ���һ��ָ��
protected:
	bool addbuf(int);		//����buf
	void cleanFile();	//�����ļ�
};

//int GetData(char* p,int length);//��Ĭ�ϵ�ƫ�ƣ�����Ϊlengthһ�ε�pָ����
//int GetData(char* p,int length,int seek_,int offset);//ָ����ƫ�ƣ����ƫ�ƣ�����Ϊlengthһ�ε�pָ����