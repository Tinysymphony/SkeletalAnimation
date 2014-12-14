#pragma once
class framenode
{
public:
	framenode(void);
	~framenode(void);
public:
	framenode* next;

	BOOL is_key_frame;
	int m_frameno;
	float Hx;
	float Hy;
	float Hz;
	float luax;
	float luay;
	float luaz;
	float lfx;
	float lfy;
	float lfz;
	float ruax;
	float ruay;
	float ruaz;
	float rfx;
	float rfy;
	float rfz;
	float ltx;
	float lty;
	float ltz;
	float lcx;
	float lcy;
	float lcz;
	float rtx;
	float rty;
	float rtz;
	float rcx;
	float rcy;
	float rcz;
};

