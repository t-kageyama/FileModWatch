// WatchingFile.cpp : implementation file
//

#include "stdafx.h"
#include "FileModWatch.h"
#include "WatchingFile.h"

// CWatchingFile

CWatchingFile::CWatchingFile()
{
	::ZeroMemory(&m_fdd, sizeof(WIN32_FIND_DATA));
}

CWatchingFile::CWatchingFile(CWatchingFile& watching)
{
	::CopyMemory(&m_fdd, &watching.m_fdd, sizeof(WIN32_FIND_DATA));
}

CWatchingFile::CWatchingFile(WIN32_FIND_DATA& fdd)
{
	::CopyMemory(&m_fdd, &fdd, sizeof(WIN32_FIND_DATA));
}

CWatchingFile::~CWatchingFile()
{
}

// CWatchingFile member functions
BOOL CWatchingFile::operator==(CWatchingFile& watching)
{
	return (*this == watching.m_fdd);
}

BOOL CWatchingFile::operator!=(CWatchingFile& watching)
{
	return (*this != watching.m_fdd);
}

BOOL CWatchingFile::operator==(WIN32_FIND_DATA& fdd)
{
	if (m_fdd.dwFileAttributes != fdd.dwFileAttributes) {
		return FALSE;
	}
	if (::memcmp(&m_fdd.ftLastWriteTime, &fdd.ftLastWriteTime, sizeof(FILETIME)) != 0) {
		return FALSE;
	}
	if (m_fdd.nFileSizeLow != fdd.nFileSizeLow) {
		return FALSE;
	}
	if (m_fdd.nFileSizeHigh != fdd.nFileSizeHigh) {
		return FALSE;
	}

	return TRUE;
}

BOOL CWatchingFile::operator!=(WIN32_FIND_DATA& fdd)
{
	return (*this == fdd) ? FALSE : TRUE;
}
