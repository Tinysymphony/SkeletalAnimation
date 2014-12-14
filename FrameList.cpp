#include "stdafx.h"
#include "FrameList.h"


FrameList::FrameList(void)
{
	head=new framenode;
	end = head;
}


FrameList::~FrameList(void)
{
}

void FrameList::playlist()
{


}

void FrameList::insert(framenode* p){
	framenode* pre;
	framenode* now;
	pre=head;
	now=head;
	while(now!=NULL){
		if(p->m_frameno>now->m_frameno){
			pre=now;
			now=now->next;
		}
		else if(p->m_frameno<now->m_frameno){
			p->next=now;
			pre->next=p;
			return;
		}
		else{
			pre->next = p;
			p->next = now->next;
			delete now;
			return;
		}
	}
	pre->next = p;
}

void FrameList::Delete(int frameno)
{
	
	framenode* pre;
	framenode* now;
	pre=head;
	now=head;
	while(now!=NULL){
		if(now->m_frameno!=frameno){
			pre=now;
			now=now->next;
		}
		else{
			pre->next=now->next;
			delete now;
			return;
		}
	}
	if (now == NULL)AfxMessageBox(_T("该关键帧不存在！"), MB_OK | MB_SYSTEMMODAL);
	
}

void FrameList::modify(int frameno)
{
	
	framenode* pre;
	framenode* now;
	pre=head;
	now=head;
	while(now!=NULL){
		if(now->m_frameno!=frameno){
			pre=now;
			now=now->next;
		}
		else{
		}
	}
}

framenode FrameList::find(int frameno)
{
	framenode* pre;
	framenode* now;
	pre=head;
	now=head;
	framenode WantToFind;
	while(now!=NULL)
	{
		if(frameno>now->m_frameno){
			pre=now;
			now=now->next;
		}
		else if(frameno<now->m_frameno){
			WantToFind.Hx = pre->Hx +(frameno - pre->m_frameno)*(now->Hx - pre->Hx)/(now->m_frameno - pre->m_frameno);
			WantToFind.Hy = pre->Hy +(frameno - pre->m_frameno)*(now->Hy - pre->Hy)/(now->m_frameno - pre->m_frameno);
			WantToFind.Hz = pre->Hz +(frameno - pre->m_frameno)*(now->Hz - pre->Hz)/(now->m_frameno - pre->m_frameno);

			WantToFind.lcx = pre->lcx +(frameno - pre->m_frameno)*(now->lcx - pre->lcx)/(now->m_frameno - pre->m_frameno);
			WantToFind.lcy = pre->lcy +(frameno - pre->m_frameno)*(now->lcy - pre->lcy)/(now->m_frameno - pre->m_frameno);
			WantToFind.lcz = pre->lcz +(frameno - pre->m_frameno)*(now->lcz - pre->lcz)/(now->m_frameno - pre->m_frameno);

			WantToFind.lfx = pre->lfx +(frameno - pre->m_frameno)*(now->lfx - pre->lfx)/(now->m_frameno - pre->m_frameno);
			WantToFind.lfy = pre->lfy +(frameno - pre->m_frameno)*(now->lfy - pre->lfy)/(now->m_frameno - pre->m_frameno);
			WantToFind.lfz = pre->lfz +(frameno - pre->m_frameno)*(now->lfz - pre->lfz)/(now->m_frameno - pre->m_frameno);

			WantToFind.ltx = pre->ltx +(frameno - pre->m_frameno)*(now->ltx - pre->ltx)/(now->m_frameno - pre->m_frameno);
			WantToFind.lty = pre->lty +(frameno - pre->m_frameno)*(now->lty - pre->lty)/(now->m_frameno - pre->m_frameno);
			WantToFind.ltz = pre->ltz +(frameno - pre->m_frameno)*(now->ltz - pre->ltz)/(now->m_frameno - pre->m_frameno);

			WantToFind.luax = pre->luax +(frameno - pre->m_frameno)*(now->luax - pre->luax)/(now->m_frameno - pre->m_frameno);
			WantToFind.luay = pre->luay +(frameno - pre->m_frameno)*(now->luay - pre->luay)/(now->m_frameno - pre->m_frameno);
			WantToFind.luaz = pre->luaz +(frameno - pre->m_frameno)*(now->luaz - pre->luaz)/(now->m_frameno - pre->m_frameno);

			WantToFind.rcx = pre->rcx +(frameno - pre->m_frameno)*(now->rcx - pre->rcx)/(now->m_frameno - pre->m_frameno);
			WantToFind.rcy = pre->rcy +(frameno - pre->m_frameno)*(now->rcy - pre->rcy)/(now->m_frameno - pre->m_frameno);
			WantToFind.rcz = pre->rcz +(frameno - pre->m_frameno)*(now->rcz - pre->rcz)/(now->m_frameno - pre->m_frameno);

			WantToFind.rfx = pre->rfx +(frameno - pre->m_frameno)*(now->rfx - pre->rfx)/(now->m_frameno - pre->m_frameno);
			WantToFind.rfy = pre->rfy +(frameno - pre->m_frameno)*(now->rfy - pre->rfy)/(now->m_frameno - pre->m_frameno);
			WantToFind.rfz = pre->rfz +(frameno - pre->m_frameno)*(now->rfz - pre->rfz)/(now->m_frameno - pre->m_frameno);

			WantToFind.rtx = pre->rtx +(frameno - pre->m_frameno)*(now->rtx - pre->rtx)/(now->m_frameno - pre->m_frameno);
			WantToFind.rty = pre->rty +(frameno - pre->m_frameno)*(now->rty - pre->rty)/(now->m_frameno - pre->m_frameno);
			WantToFind.rtz = pre->rtz +(frameno - pre->m_frameno)*(now->rtz - pre->rtz)/(now->m_frameno - pre->m_frameno);

			WantToFind.ruax = pre->ruax +(frameno - pre->m_frameno)*(now->ruax - pre->ruax)/(now->m_frameno - pre->m_frameno);
			WantToFind.ruay = pre->ruay +(frameno - pre->m_frameno)*(now->ruay - pre->ruay)/(now->m_frameno - pre->m_frameno);
			WantToFind.ruaz = pre->ruaz +(frameno - pre->m_frameno)*(now->ruaz - pre->ruaz)/(now->m_frameno - pre->m_frameno);
			
			break;
		}
		else{
			WantToFind = *now;
			break;
		}
	}
	return WantToFind;
}

void FrameList::add(framenode* p)
{
	end->next = p;
	end = end->next;
}