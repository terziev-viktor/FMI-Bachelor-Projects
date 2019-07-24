#include "ConfigFile.h"
//
//	Installation utility class to read/write properties to a MassTransit config file
//
//	Copyright (c) 2006 Acronis, Inc.
//	All Rights Reserved.
//
// Author: D. Beachy, 1.18.2006
//
//
// Modified 08/01/2010 by GAP for multiple config file support in the MT installer
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "ConfigFile.h"

// Constructor
ConfigFile::ConfigFile(ConfigFileType type) :
	m_pBuffer(NULL),
	m_iBufferSize(0),
	m_iFileSize(0),
	m_psFilePath(NULL),
	m_ConfigFileType(eConfigFile_MTEngine),
	m_cDelimiter('=')
{
	m_ConfigFileType = type;

	switch (m_ConfigFileType)
	{
	case	eConfigFile_Apache:
		m_szComment = "#";			// set comment char
		m_cDelimiter = ' ';			// set delimiter char
		break;

	case	eConfigFile_MTWebIni:
	case	eConfigFile_MTPHPIni:
		m_szComment = ";";			// set comment char
		m_cDelimiter = '=';			// set delimiter char
		break;

	case	eConfigFile_MTInstall:
	case	eConfigFile_MTConfig:
	case 	eConfigFile_MTEngine:
	case	eConfigFile_MTLicense:
	default:
		m_szComment = "%%";			// set comment char
		m_cDelimiter = '=';			// set delimiter char
		break;
	}
}

// Destructor
ConfigFile::~ConfigFile()
{
	// free up buffer if any
	ResetBuffer();
}

// Free and reset the buffer, if any
void ConfigFile::ResetBuffer()
{
	if (m_pBuffer != NULL)
		free(m_pBuffer);

	m_pBuffer = NULL;
	m_iBufferSize = 0;
	m_iFileSize = 0;
	m_psFilePath = NULL;
}

// Read file into memory; must be called BEFORE any other methods.
//    psFilePath = path to config file to be read; may not be null
//  Throws: ConfigFile::Exception if file cannot be read
//  Returns: number of bytes read into memory (should match config file size)
int ConfigFile::Read(const char* psFilePath)
{
	// reset the buffer, if any
	ResetBuffer();

	// open config file
	m_psFilePath = psFilePath;
	FILE* pfConfigFile = fopen(psFilePath, "rb");
	if (pfConfigFile == NULL)
	{
		char sMsg[1024];
		sprintf(sMsg, "ConfigFile::Read: unable to open configuration file [%s]", psFilePath);
		throw Exception(sMsg, -2);
	}

	// figure out the size of the file and allocate a buffer to hold it
	fseek(pfConfigFile, 0, SEEK_END);
	m_iFileSize = (int)ftell(pfConfigFile);
	fseek(pfConfigFile, 0, SEEK_SET);   // reset to BOF
	m_iBufferSize = m_iFileSize + 1;      // add extra byte for 0 terminator
	m_pBuffer = (char*)malloc(m_iBufferSize);
	if (m_pBuffer == NULL)
	{
		ResetBuffer();
		fclose(pfConfigFile);  // clean up
		char sMsg[1024];
		sprintf(sMsg, "ConfigFile::Read: insufficient memory to read config file [%s]: %ld bytes required", psFilePath, m_iBufferSize);
		throw Exception(sMsg, -3);
	}

	// read config file into memory
	int iBytesRead = fread(m_pBuffer, 1, m_iFileSize, pfConfigFile);
	int iErrorCode = ferror(pfConfigFile);

	// check for error
	if (iErrorCode != 0)
	{
		ConfigFile::ResetBuffer();
		fclose(pfConfigFile);  // clean up
		char sMsg[1024];
		sprintf(sMsg, "ConfigFile::Read: error reading config file [%s]: errorcode=%d", psFilePath, iErrorCode);
		throw Exception(sMsg, iErrorCode);
	}

	// sanity-check the bytes read against what was expected
	if (iBytesRead != m_iFileSize)
	{
		ConfigFile::ResetBuffer();
		fclose(pfConfigFile);  // clean up
		char sMsg[1024];
		sprintf(sMsg, "ConfigFile::Read: INTERNAL CODE ERROR: m_iFileSize=%d bytes, but actually read %d bytes", m_iFileSize, iBytesRead);
		throw Exception(sMsg, iErrorCode);
	}

	// file read successfully!  Replace any bogus null characters with a space.
	for (char* p = m_pBuffer; p < m_pBuffer + m_iFileSize; p++)
	{
		if (*p == 0)
			* p = ' ';
	}

	// Clean up and exit.
	m_pBuffer[iBytesRead] = 0;  // should always be last byte in buffer
	fclose(pfConfigFile);

	return iBytesRead;
}

// Set a property to the config file buffered in memory.  If property does not exist, it is appended to the end of the buffer.
//    psName = property name to set; may not be null
//    psValue = property value to set; may not be null, but may be empty
// Throws: ConfigFile::Exception if Read not called first
void ConfigFile::SetProperty(const char* psName, const char* psValue)
{
	if (m_pBuffer == NULL)
		throw Exception("ConfigFile::SetProperty: CALLER CODE ERROR: 'Read' method was never invoked, or 'Read' method failed.", -1);

	int iPropertyValueIndex = LocatePropertyValueIndex(psName);    // points to first character of value
	if (iPropertyValueIndex == -1)  // property does not exist?
	{
		// property does not exist; append a new line
		// construct new line
		char sNewLine[1024];
		sNewLine[0] = 0;			// null terminate line

		if (m_ConfigFileType == eConfigFile_MTEngine)
		{
			sprintf(sNewLine, "\r\n%%%%\r\n%%%% Property added by the installation program.\r\n%%%%\r\n    %s=%s\r\n", psName, psValue);
		}
		else if (m_ConfigFileType == eConfigFile_MTConfig)
		{
			sprintf(sNewLine, "\r\n%%%%\r\n%%%% Property added by the installation program.\r\n%%%%\r\n    %s=%s\r\n", psName, psValue);
		}
		else if (m_ConfigFileType == eConfigFile_MTInstall)
		{
			sprintf(sNewLine, "\r\n%%%%\r\n%%%% Property added by the installation program.\r\n%%%%\r\n    %s=%s\r\n", psName, psValue);
		}
		else if (m_ConfigFileType == eConfigFile_Apache)
		{
			sprintf(sNewLine, "\r\n#\r\n# Property added by the installation program.\r\n#\r\n%s %s\r\n", psName, psValue);
		}
		else if (m_ConfigFileType == eConfigFile_MTWebIni)
		{
			sprintf(sNewLine, "\r\n;\r\n; Property added by the installation program.\r\n;\r\n%s %s\r\n", psName, psValue);
		}
		else if (m_ConfigFileType == eConfigFile_MTPHPIni)
		{
			sprintf(sNewLine, "\r\n;\r\n; Property added by the installation program.\r\n;\r\n%s %s\r\n", psName, psValue);
		}
		else if (m_ConfigFileType == eConfigFile_MTLicense)
		{
			sprintf(sNewLine, "\r\n;\r\n; Property added by the installation program.\r\n;\r\n%s %s\r\n", psName, psValue);
		}

		int iLineLength = strlen(sNewLine);

		// adjust the required file size reallocate the buffer if necessary to allow room for the new lines
		AdjustFileSizeAndReallocBuffer(iLineLength);

		// NOTE: buffer is still zero-terminated
		strcat(m_pBuffer, sNewLine);
	}
	else  // property already exists
	{
		int iEndOfLineIndex = FindEndOfLineIndex(iPropertyValueIndex);  // points to CR or LF @ end of line

		// compute the size delta between the current value and the new value
		int iNewValueSize = strlen(psValue) + 2;   // allow 2 bytes for CR/LF line terminator
		int iCurrentValueSize = iEndOfLineIndex - iPropertyValueIndex + 1;  // indexes are inclusive
		int iSizeDelta = iNewValueSize - iCurrentValueSize;

		// increase the buffer to hold the new value if necessary
		int iOrgFileSize = m_iFileSize; // save 
		AdjustFileSizeAndReallocBuffer(iSizeDelta);

		// shift all data at and beyond the current value to adjust for the new value length
		// the current value will then be overwritten after the data is moved
		char* pValue = m_pBuffer + iPropertyValueIndex;  // points to current value

		if (iSizeDelta != 0)  // has the value's length changed?
		{
			// move everything beyond the current value UP or DOWN to insert or delete space
			// copy the zero buffer terminator as well
			memmove(pValue + iNewValueSize,         // dest
				pValue + iCurrentValueSize,     // src
				iOrgFileSize - iPropertyValueIndex - iCurrentValueSize + 1);  // size of block
		}

		// plug in the new value, overwriting the old value
		memcpy(pValue, psValue, iNewValueSize - 2);   // adjust for CR/LF to be added next

		// add CR/LF
		memcpy(pValue + iNewValueSize - 2, "\r\n", 2);
	}
}

// Adjust m_iFileSize and automatically reallocate the buffer (increase its size) if necessary.
// Note that the buffer's size is never *decreased*.
//    iDelta = positive or negative delta to adjust m_iFileSize.
void ConfigFile::AdjustFileSizeAndReallocBuffer(int iDelta)
{
	m_iFileSize += iDelta;

	int iRequiredBufferSize = m_iFileSize + 1;
	if (iRequiredBufferSize > m_iBufferSize)  // allow space for trailing zero
	{
		m_pBuffer = (char*)realloc(m_pBuffer, iRequiredBufferSize);
		m_iBufferSize = iRequiredBufferSize;
	}
}

// Find the buffer index at end of line
//     iStartingIndex = index to begin searching for end-of-line
// Returns: 0-based index in buffer of last character on line; this will be the trailing CR or LF.
int ConfigFile::FindEndOfLineIndex(const int iStartingIndex)
{
	int iRetVal;

	// lines may end in CR and/or LF
	for (iRetVal = iStartingIndex; iRetVal < m_iFileSize; iRetVal++)
	{
		char c = m_pBuffer[iRetVal];

		// check for CR and CR/LF line end
		if (c == 13)
		{
			// check for trailing LF as well
			if (m_pBuffer[iRetVal + 1] == 10)
				iRetVal++;    // count as part of this line
			break;
		}

		// check for LF-only line end
		if (c == 10)
		{
			break;
		}
	}

	// we either found EOL or we reached the end of the buffer
	return iRetVal;
}

// Locate a given property's value string and return its index in the buffer
//     psName = property name to locate; may not be null.  Name is case-sensitive.
// Returns: 0-based index in buffer of value's first character, or -1 if property not found.  This index
//          points to the first character beyond the '=' on the line.
int ConfigFile::LocatePropertyValueIndex(const char* psName)
{
	for (int iBeginningOfLineIndex = 0; iBeginningOfLineIndex < m_iFileSize; )
	{
		int iEndOfLineIndex = FindEndOfLineIndex(iBeginningOfLineIndex);  // points to CR or LF @ very end of line
		int iLineLength = iEndOfLineIndex - iBeginningOfLineIndex + 1;    // indexes are inclusive; also, length includes trailing CR/LF
		const char* pLineStart = m_pBuffer + iBeginningOfLineIndex; // points to start of line
		const char* pEquals;

		if (memcmp(pLineStart, m_szComment, strlen(m_szComment)) == 0) // is this a comment line?
		{
			iBeginningOfLineIndex = iEndOfLineIndex + 1;    // skip it
			continue;
		}

		// this is not a comment line -- find the delimter on the line
		pEquals = (char*)memchr(pLineStart, m_cDelimiter, iLineLength);
		if (pEquals == NULL)    // invalid property line!  no delimiter found
		{
			iBeginningOfLineIndex = iEndOfLineIndex + 1;    // skip this line
			continue;
		}

		// we found a valid line; check whether this property name matches the one we want
		const char* pWorkingNameChar = psName;   // points into psName at next letter to check
		for (const char* p = pLineStart; p < pEquals; p++)
		{
			char c = *p;
			// ignore whitespace
			if ((c > 32) && (c < 127))
			{
				// found a valid property name character; see whether it matches our target
				if (toupper(c) == toupper(*pWorkingNameChar++))
				{
					// character matches -- check whether all characters matched
					if (*pWorkingNameChar == 0)
					{
						// we found our property!  Return the buffer index of the value to the caller.
						return (pEquals - m_pBuffer + 1);  // index of first character beyond the '='
					}
				}
				else
				{
					break;  // no match on this property
				}
			}
		}

		// property not found on this line; jump to the next line
		iBeginningOfLineIndex = iEndOfLineIndex + 1;
	} // main line loop

	return -1;  // property not found
}

// Locate a given property's value string and return its index in the buffer, even if it is a comment.
// we'll also move the string over to the left and uncomment it.
//     psName = property name to locate; may not be null.  Name is case-sensitive.
// Returns: 0-based index in buffer of value's first character, or -1 if property not found.  This index
//          points to the first character beyond the '=' on the line.
int ConfigFile::LocatePropertyValueIndexInCommentsForMerge(const char* psName)
{
	for (int iBeginningOfLineIndex = 0; iBeginningOfLineIndex < m_iFileSize; )
	{
		int iEndOfLineIndex = FindEndOfLineIndex(iBeginningOfLineIndex);	// points to CR or LF @ very end of line
		int iLineLength = iEndOfLineIndex - iBeginningOfLineIndex + 1;		// indexes are inclusive; also, length includes trailing CR/LF
		char* pLineStart = m_pBuffer + iBeginningOfLineIndex;				// points to start of line

		// find the "=" on the line, if present
		const char* pEquals = (char*)memchr(pLineStart, '=', iLineLength);
		if (pEquals == NULL)    // invalid property line!  '=' not found
		{
			iBeginningOfLineIndex = iEndOfLineIndex + 1;    // skip this line
			continue;
		}

		// we found a "keyword = value" on the line. It could be a comment.  Check whether this property name matches the one we want
		const char* pWorkingNameChar = psName;   // points into psName at next letter to check

		for (const char* p = pLineStart; p < pEquals; p++)
		{
			char c = *p;
			// ignore whitespace and comment chars at beginning
			if ((c > 32) && (c < 127) && (c != m_szComment[0]))
			{
				// found a valid property name character; see whether it matches our target
				if (toupper(c) == toupper(*pWorkingNameChar++))
				{
					// character matches -- check whether all characters matched
					if (*pWorkingNameChar == 0)
					{
						// we found our property!  
						if (memcmp(pLineStart, m_szComment, strlen(m_szComment)) == 0) // is this a comment line?
						{
							// uncomment it by moving it over a few bytes.
							int i = 0, j = 0;
							bool bStrippedBlanks = false;

							for (i = 0; i < iLineLength - 2; i++)	// lineLength includes length of cr/lf
							{
								if (!bStrippedBlanks)
								{
									if (pLineStart[i] == m_szComment[0] || pLineStart[i] == ' ')
										continue;
									else
										bStrippedBlanks = true;
								}
								pLineStart[j++] = pLineStart[i];
							}
							memset((void*)(pLineStart + j), ' ', i - j);
							// recalculate pEquals because the '=' might have moved.
							pEquals = (char*)memchr(pLineStart, '=', iLineLength);
						}
						// Return the buffer index of the value to the caller.
						return (pEquals - m_pBuffer + 1);  // index of first character beyond the '='
					}
				}
				else
				{
					break;  // no match on this property
				}
			}
		}

		// property not found on this line; jump to the next line
		iBeginningOfLineIndex = iEndOfLineIndex + 1;
	} // main line loop

	return -1;  // property not found
}

// Retrieve a property value from the config file buffered in memory.
//    psName = property name to read; may not be null
// Returns: pointer to property value string, zero-terminated, or NULL if property does not exist.
//          NOTE: IT IS THE CALLER'S RESPONSIBILITY TO FREE THIS BUFFER WITH free().
// Throws:  ConfigFile::Exception if Read not called first
char* ConfigFile::GetProperty(const char* psName)
{
	if (m_pBuffer == NULL)
		throw Exception("ConfigFile::GetProperty: CALLER CODE ERROR: 'Read' method was never invoked, or 'Read' method failed.", -1);

	int iPropertyValueIndex = LocatePropertyValueIndex(psName);    // points to first character of value
	if (iPropertyValueIndex == -1)  // property does not exist?
		return NULL;

	// locate the end of the line
	int iEndOfLineIndex = FindEndOfLineIndex(iPropertyValueIndex);  // points to CR or LF @ very end of line
	int iValueLength = iEndOfLineIndex - iPropertyValueIndex + 1;   // indexes are inclusive; also, value length including trailing CR/LF

	// allocate space for value 
	// iValueLength includes space for trailing CR/LF (it may be one or two bytes), but we do not require this
	char* pRetVal = (char*)malloc(iValueLength + 1);   // allow room for tailing zero
	memcpy(pRetVal, m_pBuffer + iPropertyValueIndex, iValueLength);

	// trim trailing CR/LF and zero-terminate string
	char* p = pRetVal + iValueLength - 1;  // points to last character of value
	if (*p == 10)   // linefeed
		p--;        // skip it and fall through to CR check

	if (*p == 13)  // CR
		p--;       // skip it

	p[1] = 0;      // zero-terminate value

	return pRetVal;
}

// Write file in memory back to disk
//  throws ConfigFile::Exception if 1) Read not called first, or 2) file cannot be written
void ConfigFile::Write()
{
	if (m_pBuffer == NULL)
		throw Exception("ConfigFile::Write: CALLER CODE ERROR: 'Read' method was never invoked, or 'Read' method failed.", -1);

	// open config file
	FILE* pfConfigFile = fopen(m_psFilePath, "wb");
	if (pfConfigFile == NULL)
	{
		char sMsg[1024];
		int iErrorCode = ferror(pfConfigFile);
		sprintf(sMsg, "ConfigFile::Write: unable to open configuration file [%s] for writing, errorcode=%d", m_psFilePath, iErrorCode);
		throw Exception(sMsg, iErrorCode);
	}

	// write the buffer minus the trailing zero
	int iBytesWritten = fwrite(m_pBuffer, 1, m_iFileSize, pfConfigFile);
	int iErrorCode = ferror(pfConfigFile);
	fclose(pfConfigFile);

	if (iBytesWritten != m_iFileSize)
	{
		char sMsg[1024];
		sprintf(sMsg, "ConfigFile::Write: unable to write configuration file [%s], errorcode=%d: Buffer size=%d bytes, but only %d bytes successfully written.",
			m_psFilePath, iErrorCode, m_iFileSize, iBytesWritten);
		throw Exception(sMsg, iErrorCode);
	}
}

// Set a property to the config file buffered in memory.  If property does not exist, it is appended to the end of the buffer.
//    psName = property name to set; may not be null
//    psValue = property value to set; may not be null, but may be empty
// Throws: ConfigFile::Exception if Read not called first
void ConfigFile::SetPropertyForConfigFileMerge(const char* psName, const char* psValue)
{
	// Look thru the whole input config file twice.
	// First pass is to see if the property exists and is not commented out.  If so, update the value, and we're done.
	// Second pass is to find a commented out property, and if it is commented out, then uncomment it and update the value.
	// If the property was not found in either pass, add the property and value to the end of the file.

	if (m_pBuffer == NULL)
		throw Exception("ConfigFile::SetProperty: CALLER CODE ERROR: 'Read' method was never invoked, or 'Read' method failed.", -1);

	// first try to find the uncommented property.
	int iPropertyValueIndex = LocatePropertyValueIndex(psName);		// points to first character of value
	if (iPropertyValueIndex == -1)									// property does not exist?
	{
		// The property is not in the output file uncommented, so look for a commented out one...
		// This next call returns a pointer to the first character of the value, even if it is in a comment... 
		// and the line gets uncommented after this call if the keyword is found.

		iPropertyValueIndex = LocatePropertyValueIndexInCommentsForMerge(psName);
		if (iPropertyValueIndex == -1)								// property still does not exist?
		{
			// construct new line
			char sNewLine[1024];
			sNewLine[0] = 0;			// null terminate line

			if (m_ConfigFileType == eConfigFile_MTEngine)
			{
				sprintf(sNewLine, "\r\n%%%%\r\n%%%% Property added by the installation program.\r\n%%%%\r\n    %s=%s\r\n", psName, psValue);
			}
			else if (m_ConfigFileType == eConfigFile_MTConfig)
			{
				sprintf(sNewLine, "\r\n%%%%\r\n%%%% Property added by the installation program.\r\n%%%%\r\n    %s=%s\r\n", psName, psValue);
			}
			else if (m_ConfigFileType == eConfigFile_MTInstall)
			{
				sprintf(sNewLine, "\r\n%%%%\r\n%%%% Property added by the installation program.\r\n%%%%\r\n    %s=%s\r\n", psName, psValue);
			}
			else if (m_ConfigFileType == eConfigFile_Apache)
			{
				sprintf(sNewLine, "\r\n#\r\n# Property added by the installation program.\r\n#\r\n%s %s\r\n", psName, psValue);
			}
			else if (m_ConfigFileType == eConfigFile_MTWebIni)
			{
				sprintf(sNewLine, "\r\n;\r\n; Property added by the installation program.\r\n;\r\n%s %s\r\n", psName, psValue);
			}
			else if (m_ConfigFileType == eConfigFile_MTPHPIni)
			{
				sprintf(sNewLine, "\r\n;\r\n; Property added by the installation program.\r\n;\r\n%s %s\r\n", psName, psValue);
			}
			else if (m_ConfigFileType == eConfigFile_MTLicense)
			{
				sprintf(sNewLine, "\r\n;\r\n; Property added by the installation program.\r\n;\r\n%s %s\r\n", psName, psValue);
			}

			int iLineLength = strlen(sNewLine);

			// adjust the required file size reallocate the buffer if necessary to allow room for the new lines
			AdjustFileSizeAndReallocBuffer(iLineLength);

			// NOTE: buffer is still zero-terminated
			strcat(m_pBuffer, sNewLine);
		}
		else
		{
			int iEndOfLineIndex = FindEndOfLineIndex(iPropertyValueIndex);		// points to CR or LF @ end of line

			// compute the size delta between the current value and the new value
			int iNewValueSize = strlen(psValue) + 2;							// allow 2 bytes for CR/LF line terminator
			int iCurrentValueSize = iEndOfLineIndex - iPropertyValueIndex + 1;  // indexes are inclusive
			int iSizeDelta = iNewValueSize - iCurrentValueSize;

			// increase the buffer to hold the new value if necessary
			int iOrgFileSize = m_iFileSize; // save 
			AdjustFileSizeAndReallocBuffer(iSizeDelta);

			// shift all data at and beyond the current value to adjust for the new value length
			// the current value will then be overwritten after the data is moved
			char* pValue = m_pBuffer + iPropertyValueIndex;  // points to current value

			if (iSizeDelta != 0)  // has the value's length changed?
			{
				// move everything beyond the current value UP or DOWN to insert or delete space
				// copy the zero buffer terminator as well
				memmove(pValue + iNewValueSize,         // dest
					pValue + iCurrentValueSize,     // src
					iOrgFileSize - iPropertyValueIndex - iCurrentValueSize + 1);  // size of block
			}

			// plug in the new value, overwriting the old value
			memcpy(pValue, psValue, iNewValueSize - 2);   // adjust for CR/LF to be added next

			// add CR/LF
			memcpy(pValue + iNewValueSize - 2, "\r\n", 2);
		}
	}
	else  // property already exists
	{
		int iEndOfLineIndex = FindEndOfLineIndex(iPropertyValueIndex);		// points to CR or LF @ end of line

		// compute the size delta between the current value and the new value
		int iNewValueSize = strlen(psValue) + 2;							// allow 2 bytes for CR/LF line terminator
		int iCurrentValueSize = iEndOfLineIndex - iPropertyValueIndex + 1;  // indexes are inclusive
		int iSizeDelta = iNewValueSize - iCurrentValueSize;

		// increase the buffer to hold the new value if necessary
		int iOrgFileSize = m_iFileSize; // save 
		AdjustFileSizeAndReallocBuffer(iSizeDelta);

		// shift all data at and beyond the current value to adjust for the new value length
		// the current value will then be overwritten after the data is moved
		char* pValue = m_pBuffer + iPropertyValueIndex;						// points to current value

		if (iSizeDelta != 0)  // has the value's length changed?
		{
			// move everything beyond the current value UP or DOWN to insert or delete space
			// copy the zero buffer terminator as well
			memmove(pValue + iNewValueSize,         // dest
				pValue + iCurrentValueSize,     // src
				iOrgFileSize - iPropertyValueIndex - iCurrentValueSize + 1);  // size of block
		}

		// plug in the new value, overwriting the old value
		memcpy(pValue, psValue, iNewValueSize - 2);   // adjust for CR/LF to be added next

		// add CR/LF
		memcpy(pValue + iNewValueSize - 2, "\r\n", 2);
	}
}
