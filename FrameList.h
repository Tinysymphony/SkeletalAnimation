#pragma once
#include"framenode.h"
class FrameList
{

public:
	friend class framenode;
	framenode* head;
	framenode* end;
	int totalframe=1;


public:
	FrameList(void);
	~FrameList(void);
	void playlist();
	void insert(framenode* p);
	void add(framenode* p);
	void Delete(int frameno);
	void modify(int frameno);
	framenode find(int frameno);
};

