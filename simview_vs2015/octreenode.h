#pragma once
#include "stat_info.h"
class VolumeBlock;
class octreeNode
{
public:
	octreeNode():
		_parent(NULL),
		_data(NULL),
		_level(-1)
	{
		for (int i = 0; i < 8; i++)
			_children[i] = NULL;
	}

	~octreeNode()// let octree function to recursively remove nodes
	{
		SAFE_DELETE(_data);
	}


public:
	octreeNode*   _children[8];
	octreeNode*   _parent;
	StatInfo      _statInfo; // stat info
	// pointer to data
	VolumeBlock* _data;
	int          _level;
	FLOATVECTOR3 _volStartPos; // the portion of volume this node covers
};