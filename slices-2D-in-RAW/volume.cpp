#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include "volume.h"

// Release volume data if exists
void CVolume::clean()
{
	if(m_data)
	{
		free(m_data);
		m_data = nullptr;
	}
}

// Constructor
CVolume::CVolume()
{
	m_data = nullptr;
}

// Destructor
CVolume::~CVolume()
{
	clean();
}

// Read a RAW/plain format (not ASCII-based) volume
// @param name name of the file to open. 
// @param slices number of slices of the volume
// @param rows number of slices of the volume
// @param cols number of slices of the volume
//
// @return true if is valid, otherwise false
bool CVolume::readFile(std::string name, const int slices, const int rows, const int cols)
{
	m_nslices = slices;
	m_nrows = rows;
	m_ncols = cols;
	clean();
	m_data = malloc3D(m_nrows, m_ncols, m_nslices);
	
	FILE* open;
	if(!(open = fopen(name.c_str(),"rb")))return false;	//binary files

	for(int k = 0; k < m_nslices; k++)
		for(int i = 0; i < m_nrows; i++)
			for(int j = 0; j < m_ncols; j++)
				fread(&m_data[k][i][j],sizeof(uchar),1,open);
	fclose(open);
	return true;
}

// Obtain a slice [i][INDEX][j] from volume. It is not a "coronal" view, it depends of the volume orientation
// @param iIndex number of slice to extract from volume
//
// @return a RGB (gray scale) of a coronal slice
puchar CVolume::getCoronalSlice(int iIndex)
{
	puchar data = (puchar)malloc(sizeof(uchar)*m_nslices*m_ncols * 3);
	int x = 0;
	for (int k = 0; k < m_nslices; k++)
			for (int j = 0; j < m_ncols; j++)
			{
				data[x + 0] = m_data[k][iIndex][j];
				data[x + 1] = m_data[k][iIndex][j];
				data[x + 2] = m_data[k][iIndex][j];
				x += 3;
			}
	_ASSERT(x == m_nslices * m_ncols * 3);
	return data;
}

// Obtain a slice [i][j][INDEX] from volume. It is not a "sagittal" view, it depends of the volume orientation
// @param iIndex number of slice to extract from volume
//
// @return a RGB (gray scale) of a sagittal slice
puchar CVolume::getSagittalSlice(int iIndex)
{
	puchar data = (puchar)malloc(sizeof(uchar)*m_nslices*m_nrows * 3);
	int x = 0;
	for (int k = 0; k < m_nslices; k++)
		for (int i = 0; i < m_nrows; i++)
		{
			data[x + 0] = m_data[k][i][iIndex];
			data[x + 1] = m_data[k][i][iIndex];
			data[x + 2] = m_data[k][i][iIndex];
			x += 3;
		}
	_ASSERT(x == m_nslices * m_ncols * 3);
	return data;
}

// Obtain a slice [INDEX][i][j] from volume. It is not a "axial" view, it depends of the volume orientation
// @param iIndex number of slice to extract from volume
//
// @return a RGB (gray scale) of a axial slice
puchar CVolume::getAxialSlice(int iIndex)
{
	puchar data = (puchar)malloc(sizeof(uchar)*m_nrows*m_ncols*3);
	int x = 0;
	for (int i = 0; i < m_nrows; i++)
		for (int j = 0; j < m_ncols; j++)
		{
			data[x + 0] = m_data[iIndex][i][j];
			data[x + 1] = m_data[iIndex][i][j];
			data[x + 2] = m_data[iIndex][i][j];
			x += 3;
		}
	_ASSERT(x == m_nrows * m_ncols * 3);
	return data;
}