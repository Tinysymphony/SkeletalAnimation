
// skeletal animationDoc.h : CskeletalanimationDoc ��Ľӿ�
//


#pragma once

#include "FrameList.h"


class CskeletalanimationDoc : public CDocument
{
protected: // �������л�����
	CskeletalanimationDoc();
	DECLARE_DYNCREATE(CskeletalanimationDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CskeletalanimationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	FrameList framelist;
	FrameList anime;

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
