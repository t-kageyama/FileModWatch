#pragma once

// CWatchingFile command target

class CWatchingFile : public CObject
{
public:
	CWatchingFile();
	CWatchingFile(CWatchingFile& watching);
	CWatchingFile(WIN32_FIND_DATA& fdd);
	virtual ~CWatchingFile();

public:
	LPCTSTR GetName() { return m_fdd.cFileName; }

public:
	// operators.
	BOOL operator==(CWatchingFile& watching);
	BOOL operator!=(CWatchingFile& watching);
	BOOL operator==(WIN32_FIND_DATA& fdd);
	BOOL operator!=(WIN32_FIND_DATA& fdd);

private:
	WIN32_FIND_DATA m_fdd;
};


