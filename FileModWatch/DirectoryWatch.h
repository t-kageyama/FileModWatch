#pragma once

#include "WatchingFile.h"

// CDirectoryWatch command target

class CDirectoryWatch : public CObject
{
public:
	CDirectoryWatch();
	virtual ~CDirectoryWatch();

	BOOL Srart(CDocument* doc);
	void Finish();
	BOOL IsWatching() { return m_watching; }

private:
	static void ThreadProc(LPVOID lpParam);
	void ThreadFunc();
	static BOOL IsDirectoryExist(LPCTSTR dirName);
	static void BuildDirectoryContents(LPCTSTR watchDir, CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps, LPCTSTR watchFile);
	static void AddFileToMap(CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps, WIN32_FIND_DATA& fdd);
	static void RemoveFileMap(CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps);
	void PopulateFileMap(CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps);
	void CompareFileMaps(CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps);
	BOOL CopyFile(LPCTSTR fileName);
	void CloseFileWatchHandle();
	HANDLE OpenFileWatchHandle();

private:
	BOOL m_watching;
	BOOL m_end;
	HANDLE m_endHandle;
	HANDLE m_changeHandle;
	CWinThread* m_thread;
	CString m_watchDir;
	CString m_copyToDir;
	CStringArray m_watchFiles;
	CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*> m_fileMaps;
};
