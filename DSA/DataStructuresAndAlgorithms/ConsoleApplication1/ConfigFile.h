#pragma once


//
//	Header file for installation utility class to read/write properties to a MassTransit config file.
//
//	Copyright (c) 2006 Acronis, Inc.
//	All Rights Reserved.
//
// Author: D. Beachy, 1.18.2006
//
// Config file format: Lines beginning with %% are comments and are ignored.
// Do not quote values for tokens.
//
// Modified 08/01/2010 by GAP for multiple config file support in the MT installer
//

#ifndef GLI_CONFIGFILE_H
#define GLI_CONFIGFILE_H

#include <stdio.h>
#include <string>

// added by gap for new installers
typedef enum ConfigFileType
{
	eConfigFile_MTEngine,
	eConfigFile_Apache,
	eConfigFile_MTWebIni,
	eConfigFile_MTPHPIni,
	eConfigFile_MTConfig,
	eConfigFile_MTInstall,
	eConfigFile_MTLicense,
};

typedef std::basic_string<char> string;

class ConfigFile
{
public:
	ConfigFile(ConfigFileType type);
	virtual ~ConfigFile();// VT Comment: Why would we want to inharit from this ?

	int Read(const char* psFilePath);
	void SetProperty(const char* psName, const char* psValue);
	void SetPropertyForConfigFileMerge(const char* psName, const char* psValue);
	char* GetProperty(const char* psName);
	void Write();

protected:
	void ResetBuffer();
	int LocatePropertyValueIndex(const char* psName);
	int LocatePropertyValueIndexInCommentsForMerge(const char* psName);
	int FindEndOfLineIndex(const int iStartingIndex);
	void AdjustFileSizeAndReallocBuffer(int iDelta);

	// variables
	int					m_iFileSize;		// size of file in m_pBuffer (buffer includes trailing zero)
	int					m_iBufferSize;		// size of file buffer; >= m_iFileSize+1
	char* m_pBuffer;			// config file contents; zero-terminated
	const char* m_psFilePath;		// path/filename of config file
	ConfigFileType		m_ConfigFileType;	// type of config file we're working with
	char				m_cDelimiter;		// delimiter between 'keyword' and 'value', (e.g. '=' or space)
	const char* m_szComment;		// comment character (e.g. "#" or ";" or "%%")

	//
	// Exception class thrown when fatal error occurs
	//
public:
	class Exception
	{
	public:
		Exception(const string& sMessage, const int iErrorCode) :
			m_sMessage(sMessage),
			m_iErrorCode(iErrorCode) { }

		~Exception() { }

		int GetErrorCode() const { return m_iErrorCode; }
		const char* GetErrorMsg() const { return m_sMessage.c_str(); }

	protected:
		string m_sMessage;
		int    m_iErrorCode;
	};
};

#endif