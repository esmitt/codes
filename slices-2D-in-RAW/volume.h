#ifndef __VOLUME__H_
#define __VOLUME__H_

#include <malloc.h>
#include <string>

typedef unsigned char uchar;		//unsigned char
typedef unsigned char * puchar;		//pointer of unsigned char
typedef unsigned char ** dpuchar;	//double pointer of unsigned char
typedef unsigned char *** tpuchar;	//triple pointer of unsigned char

///
/// Construct a contiguos space of size row x cols in unsigned char 
/// @param row number of rows of the matrix
/// @param cols number of columns of the matrix
///
/// @return a pointer for dpuchar containing the 2D array (dpuchar = unsigned char **)
///
inline dpuchar malloc2D(unsigned int row,unsigned  int cols)
{
	dpuchar pointer;
	pointer = (dpuchar)malloc(sizeof(puchar)*row);
	for (unsigned int i = 0; i < row; i++)
	{
		pointer[i] = (puchar)malloc(sizeof(uchar)*cols);
		memset(pointer[i], 0, sizeof(uchar) * cols);
	}
	return pointer;
}

///
/// Construct a contiguos space of size row x cols x depth (3D) in unsigned char 
/// @param row number of rows of the matrix
/// @param cols number of columns of the matrix
/// @param depth number of columns of the matrix
///
/// @return a pointer for tpuchar containing the 3D array (tpuchar = unsigned char ***)
///
inline tpuchar malloc3D(unsigned int row, unsigned int cols, unsigned int depth)
{
	tpuchar pointer = (tpuchar)malloc(sizeof(dpuchar)*depth);

	for(unsigned int k = 0; k < depth; k++)
	{
		pointer[k] = (dpuchar)malloc2D(row, cols);
	}
	return pointer;
}

/// Handle a simple data volume of unsigned char values.
class CVolume
{
private:
	void clean();
	tpuchar m_data;

public:
	int m_nslices;
	int m_nrows;
	int m_ncols;

	bool readFile(std::string name, const int slices, const int rows, const int cols);
	puchar getAxialSlice(int iIndex);
	puchar getCoronalSlice(int iIndex);
	puchar getSagittalSlice(int iIndex);

	CVolume();
	~CVolume();
};

#endif	//__VOLUME__H_