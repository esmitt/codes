#pragma once
#include <string>

//typedef 
typedef unsigned char uchar;
typedef unsigned int uint;
//namespaces

class CUtils
{
public:
	static CUtils* getInstance();
protected:
	CUtils();
	CUtils(const CUtils&);
	CUtils & operator = (const CUtils &);
private:
	static CUtils* m_pInstance;
public:	//methods
	bool setOpenGLExt();
	static uchar* getBytesFromTexture(std::string strFilename);
	inline uint getUnitTextureAvailable(){return m_uIdTexture++;}
	uchar* getInfoFromTexture(std::string strFilename, int* iWidth, int* iHeight);
	//inline uint* get
	uint m_uIdTexture;
};

