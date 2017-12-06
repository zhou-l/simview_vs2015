#pragma once
#include "MyVectors.h"
#include <limits>

class spatialDataPt
{
public:
	spatialDataPt() 
	{
		_pos = FLOATVECTOR3(0, 0, 0);
		_id = std::numeric_limits<UINT64>::max();
	}

	spatialDataPt(float x, float y, float z, UINT64 id):
		_pos(x,y,z),
		_id(id)
	{}

	spatialDataPt(FLOATVECTOR3 pos, UINT64 id) :
		_pos(pos),
		_id(id)
	{}

	UINT64 id() const { return _id; }
	void id(const UINT64& val) { _id = val; }

	FLOATVECTOR3 pos() const { return _pos;  }
	void pos(const FLOATVECTOR3& val) { _pos = val; }

	FLOATVECTOR3 coord() const { return _pos; }
	
private:
	FLOATVECTOR3  _pos;
	UINT64        _id;
};
