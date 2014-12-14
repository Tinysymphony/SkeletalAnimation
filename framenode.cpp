#include "stdafx.h"
#include "framenode.h"


framenode::framenode(void)
{
	next=NULL;
	m_frameno=0;
	is_key_frame = false;

	Hx=0,Hy=0,Hz=0;
	luax=0,luay=0,luaz=0;
	ruax=0,ruay=0,ruaz=0;
	lfx=0,lfy=0,lfz=0;
	rfx=0,rfy=0,rfz=0;
	ltx=0,lty=0,ltz=0;
	rtx=0,rty=0,rtz=0;
	lcx=0,lcy=0,lcz=0;
	rcx=0,rcy=0,rcz=0;
}


framenode::~framenode(void)
{
}
