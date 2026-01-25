// DirectoryWatch.cpp : implementation file
//

#include "stdafx.h"
#include "FileModWatch.h"
#include "DirectoryWatch.h"
#include "FileModWatchDoc.h"

// CDirectoryWatch

CDirectoryWatch::CDirectoryWatch() : m_watching(FALSE), m_endHandle(NULL), m_changeHandle(NULL), m_thread(NULL), m_end(FALSE)
{
}

CDirectoryWatch::~CDirectoryWatch()
{
}

// CDirectoryWatch member functions
BOOL CDirectoryWatch::Srart(CDocument* doc)
{
	if (m_watching == FALSE) {
		CFileModWatchDoc *const myDoc = (CFileModWatchDoc*) doc;
		m_watchDir = myDoc->m_watchDir;
		m_copyToDir = myDoc->m_copyToDir;
		m_watchFiles.RemoveAll();
		for (int i = 0; i < myDoc->m_watchFiles.GetCount(); i++) {
			m_watchFiles.Add(myDoc->m_watchFiles.GetAt(i));
		}

		// check prameter.
		if (IsDirectoryExist(m_watchDir) == FALSE || IsDirectoryExist(m_copyToDir) == FALSE) {
			return FALSE;
		}

		// start.
		OpenFileWatchHandle();
		//const DWORD dwNotifyFilter = FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION;
		//m_changeHandle = ::FindFirstChangeNotification(m_watchDir, FALSE, dwNotifyFilter);
		m_endHandle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		m_end = FALSE;
		m_thread = AfxBeginThread((AFX_THREADPROC) ThreadProc, (LPVOID) this);
		m_thread->m_bAutoDelete = FALSE;

		m_watching = TRUE;
	}

	return m_watching;
}

void CDirectoryWatch::Finish()
{
	if (m_watching) {
		// terminate.
		m_end = TRUE;
		::SetEvent(m_endHandle);
		::WaitForSingleObject(m_thread->m_hThread, INFINITE);
		delete m_thread;
		m_thread = NULL;
		::CloseHandle(m_endHandle);
		m_watching = FALSE;
		RemoveFileMap(m_fileMaps);
	}
}

void CDirectoryWatch::ThreadFunc()
{
	RemoveFileMap(m_fileMaps);

	PopulateFileMap(m_fileMaps);

	// wait for notification.
	HANDLE handles[2];
	handles[1] = m_endHandle;
	while (m_end == FALSE) {
		handles[0] = m_changeHandle;
		const DWORD ev = ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);
		if (m_end) {
			break;	// ended.
		}
		if (ev == WAIT_OBJECT_0) {
			::Sleep(3000);
			CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*> fileMaps;
			PopulateFileMap(fileMaps);
			// compare file map.
			CompareFileMaps(fileMaps);
		}
		if (OpenFileWatchHandle() == NULL) {
			break;	// error.
		}
	}

	CloseFileWatchHandle();
}

void CDirectoryWatch::CompareFileMaps(CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps)
{
	// enumerate new map.
	POSITION pos = fileMaps.GetStartPosition();
	while (pos != NULL) {
		CString key;
		CWatchingFile *watchingFile = NULL;
		fileMaps.GetNextAssoc(pos, key, watchingFile);
		if (watchingFile != NULL) {
			// find in current map.
			CWatchingFile *current = NULL;
			if (m_fileMaps.Lookup(key, current)) {
				// compare.
				if (*watchingFile != *current) {
					// something modified.
					CopyFile(watchingFile->GetName());
				}
			}
			else {
				// new file found.
				CopyFile(watchingFile->GetName());
			}
		}
	}

	// clear member.
	RemoveFileMap(m_fileMaps);

	// move to member.
	pos = fileMaps.GetStartPosition();
	while (pos != NULL) {
		CString key;
		CWatchingFile *watchingFile = NULL;
		fileMaps.GetNextAssoc(pos, key, watchingFile);
		if (watchingFile != NULL) {
			CString name = watchingFile->GetName();
			m_fileMaps.SetAt(name, watchingFile);
		}
	}
}

BOOL CDirectoryWatch::CopyFile(LPCTSTR fileName)
{
	// source file.
	CString src = m_watchDir;
	if (src.GetAt(src.GetLength() - 1) != _T('\\')) {
		src += _T('\\');
	}
	src += fileName;
	// destination file.
	CString dst = m_copyToDir;
	if (dst.GetAt(dst.GetLength() - 1) != _T('\\')) {
		dst += _T('\\');
	}
	dst += fileName;

	// copy.
	const BOOL ret = ::CopyFile(src, dst, FALSE);
	if (ret == FALSE) {
		// copy error.
	}
	return ret;
}

void CDirectoryWatch::PopulateFileMap(CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps)
{
	if (m_watchFiles.GetCount() < 1) {
		BuildDirectoryContents(m_watchDir, fileMaps, _T("*"));
	}
	else {
		for (int i = 0; i < m_watchFiles.GetCount(); i++) {
			BuildDirectoryContents(m_watchDir, fileMaps, m_watchFiles.GetAt(i));
		}
	}
}

void CDirectoryWatch::BuildDirectoryContents(LPCTSTR watchDir, CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps, LPCTSTR watchFile)
{
	WIN32_FIND_DATA ffd;
	BOOL found = FALSE;
	::ZeroMemory(&ffd, sizeof(ffd));
	CString dir = watchDir;
	if (dir.GetAt(dir.GetLength() - 1) != _T('\\')) {
		dir += _T('\\');
	}
	dir += watchFile;
	HANDLE handle = ::FindFirstFile(dir, &ffd);
	do {
		if (ffd.dwFileAttributes == 0) {
			break;
		}

		CString buff = ffd.cFileName;
		buff += _T("\n");
		::OutputDebugString(buff);

		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			// file found.
			AddFileToMap(fileMaps, ffd);
		}

		found = ::FindNextFile(handle, &ffd);
	} while (found);
	::FindClose(handle);
}

void CDirectoryWatch::AddFileToMap(CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps, WIN32_FIND_DATA& fdd)
{
	CWatchingFile *const watching = new CWatchingFile(fdd);
	CWatchingFile *dummy = NULL;
	CString name = watching->GetName();
	if (fileMaps.Lookup(name, dummy) == FALSE) {
		fileMaps.SetAt(name, watching);
	}
	else {
		delete watching;	// just in case..
	}
}

void CDirectoryWatch::ThreadProc(LPVOID lpParam)
{
	CDirectoryWatch *const watch = (CDirectoryWatch*)lpParam;
	watch->ThreadFunc();
}

BOOL CDirectoryWatch::IsDirectoryExist(LPCTSTR dirName)
{
	const DWORD dwAttrib = GetFileAttributes(dirName);
	if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
		return FALSE;
	}
	if ((dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
		return FALSE;
	}
	return TRUE;
}

void CDirectoryWatch::RemoveFileMap(CMap<CString, LPCTSTR, CWatchingFile*, CWatchingFile*>& fileMaps)
{
	POSITION pos = fileMaps.GetStartPosition();
	while (pos != NULL) {
		CString key;
		CWatchingFile *watchingFile = NULL;
		fileMaps.GetNextAssoc(pos, key, watchingFile);
		delete watchingFile;
	}

	fileMaps.RemoveAll();
}

void CDirectoryWatch::CloseFileWatchHandle()
{
	if (m_changeHandle != NULL) {
		::FindCloseChangeNotification(m_changeHandle);
		m_changeHandle = NULL;
	}
}

HANDLE CDirectoryWatch::OpenFileWatchHandle()
{
	CloseFileWatchHandle();
	const DWORD dwNotifyFilter = FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME;
	m_changeHandle = ::FindFirstChangeNotification(m_watchDir, FALSE, dwNotifyFilter);
	return m_changeHandle;
}
