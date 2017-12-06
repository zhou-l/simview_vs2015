#ifndef VOLUME_DATA_H
#define VOLUME_DATA_H
#include "MyVectors.h"

const int MAX_VOL_VAL_INT = 1024 - 1;
const int MAX_VOL_VAL_UINT = 2048 - 1;
const int MAX_VOL_VAL_SHORT = 512 - 1;
const int MAX_VOL_VAL_USHORT = 1024 - 1;
const int MAX_VOL_VAL_CHAR = 128 - 1;
const int MAX_VOL_VAL_UCHAR = 256 - 1;


struct voxelTypeDesc{
	int byteSize;
	bool isSigned;
	bool isFloat;
};

class VolumeData
{
public:
	VolumeData();
	VolumeData(UINT64VECTOR3 dim, int voxelByteSize, int value, bool isSigned = false, bool isFloat = false);
	VolumeData(UINT64VECTOR3 dim, int voxelByteSize, void* data, bool isSigned = false, bool isFloat = false);
	VolumeData(UINT64VECTOR3 dim, int voxelByteSize, const char* filename, bool isSigned = false, bool isFloat = false);
	virtual ~VolumeData();

	void setDataPtr(void* ptr) { _data = ptr; }
	void setDataPtrNull() {_data = NULL;}
	void getSubVol(void* buf, UINT64VECTOR3 pos, UINT64VECTOR3 sz) const;
	void setSubVol(const void* buf, UINT64VECTOR3 pos, UINT64VECTOR3 sz);
	// Accessors
	int getVoxelByteSize() const { return _voxelByteSize; }
	bool isSigned() const { return _isSigned; }
	bool isFloat() const { return _isFloat; }

	void setSigned(bool isSigned) {_isSigned = isSigned; }
	void setFloat(bool isFloat) {_isFloat = isFloat; }

	float getVoxel(UINT64VECTOR3 pos) const;
	void         setVoxel(UINT64VECTOR3 pos, float value); // value is a normalized float, in [0,1]
	void         getLayer(void* buf, UINT64 z) const; // get entire Z layer	
	void         setLayer(const void* buf, UINT64 z); // set entire Z layer
	void         copySlice(void** ppbuf, int& slicew, int& sliceh, UINT64 sliceId, int sliceDir); // create a slice buffer and get its content!
	void         getSlice(void* buf, UINT64 sliceId, int sliceDir) const; // Get slice given slice Id and slice Dir

	UINT64VECTOR3 getDim() const { return _dim; }
	//////////////////////////////////////////
	// Normalization
	void normalize(float& minVal, float& maxVal);
	//////////////////////////////////////////
	// Differential operations
	/////////////////////////////////////////
	// compute gradient magnitude:
	VolumeData* computeGradientMagnitude();
	// compute second derivative magnitude
	VolumeData* computeSecondDerivMag(); 
	// compute gradient, gradient magnitude and also directional second derivative
	//void computeGradMagandDirSecondDeriv(VolumeData** volGradMag, VolumeData** volDirSecDeriv, bool normalize = true);
	//void genSyntheticVols(VolumeData** volGradMag, VolumeData** volDirSecDeriv, bool normalize = true);
	float computeDataSampleOnPlane(const PlaneCoeffs& P, const float& x, const float& y); // Given coefficients of a plane and two elements of a value, and compute the third element
	void  rotateDataSampleToPlane(const PlaneCoeffs& P, float& x, float& y, float& z);
	/////////////////////////////////////////
	// pad the volume
	void pad(UINT64VECTOR3 newVolSize);

	// create a 1/8 volume
	VolumeData* downSampleOneLevel();

	void* getDataBuf() const { return _data; }
	// histograms
	void computeHistogram1D();
	void computeHistogram2D(VolumeData* gradMagVol);

	int  findBackgrndVal(); // Finds the background value in integer
	// Converte to float (by default normalize values as well!)
	void convert2Float(float** ppFloatVol, bool normalize = true) const; 

	bool writeToFile(const char* filename, char* finalName);
//	void writePPMFiles(const char* filename);
private:
	std::string            _inputFileName;
	void*                  _data;
	UINT64VECTOR3          _dim;
	std::vector<UINT64>    _histogram1D;
	int                    _voxelByteSize;
	int                    _backgrndVal;
	UINT64*                _histogram2D;

	bool                   _isSigned;
	bool                   _isFloat;
};

#endif
