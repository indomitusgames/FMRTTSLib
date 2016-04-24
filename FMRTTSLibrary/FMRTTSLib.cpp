#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override something,
//but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <sapi.h>
#pragma warning (disable:4996)
#include <sphelper.h>

#include "FMRTTSLib.h"
#include <iostream>

namespace FMRTTSLib
{
	void FMRTTSLibMain::testHr(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw 10;
		}
	}
	
	char* FMRTTSLibMain::GeneratePCMData(wchar_t* textToRender, ULONG* bytesRead)
	{
		HRESULT hr = S_OK;
		CComPtr<ISpVoice> cpVoice; //Will send data to ISpStream
		CComPtr<ISpStream> cpStream; //Will contain IStream
		CComPtr<IStream> cpBaseStream; //Will contain raw data
		ISpObjectToken* cpToken(NULL); //Will set voice characteristics

		GUID guidFormat;
		WAVEFORMATEX* pWavFormatEx = nullptr;

		try
		{
			if (FAILED(::CoInitialize(NULL)))
			{
				throw 10;
			}

			// Create voice
			hr = cpVoice.CoCreateInstance(CLSID_SpVoice);
			testHr(hr);

			// Set Voice characteristics
			wchar_t* szRequiredAttribs = L"vendor=microsoft;language=409";
			SpFindBestToken(SPCAT_VOICES, szRequiredAttribs, L"", &cpToken);
			hr = cpVoice->SetVoice(cpToken);
			cpToken->Release();
			testHr(hr);

			// Create Stream
			hr = cpStream.CoCreateInstance(CLSID_SpStream);
			testHr(hr);

			// Create BaseStream
			hr = CreateStreamOnHGlobal(NULL, TRUE, &cpBaseStream);
			testHr(hr);

			// Set audio format
			hr = SpConvertStreamFormatEnum(SPSF_44kHz16BitMono, &guidFormat, &pWavFormatEx);
			testHr(hr);

			// Set BaseStream on Stream
			hr = cpStream->SetBaseStream(cpBaseStream, guidFormat, pWavFormatEx);
			cpBaseStream.Release();
			testHr(hr);

			// Set Voice output
			hr = cpVoice->SetOutput(cpStream, FALSE);
			testHr(hr);
			
			// Set voice falgs and generate audio
			SpeechVoiceSpeakFlags voiceFlags = SpeechVoiceSpeakFlags::SVSFlagsAsync;
			hr = cpVoice->Speak(textToRender, voiceFlags, NULL);
			testHr(hr);
			hr = cpVoice->WaitUntilDone(1000);
			testHr(hr);

			// Uncomment below to directly output speech

			//cpVoice->SetOutput(NULL, FALSE);
			//cpVoice->SpeakStream(cpStream, SPF_DEFAULT, NULL);

			// Reset stream position
			_LARGE_INTEGER a = { 0 };
			hr = cpStream->Seek(a, STREAM_SEEK_SET, NULL);

			// Get the base istream from the ispstream
			IStream* pIstream;
			hr = cpStream->GetBaseStream(&pIstream);

			// Calculate the size that is to be read
			STATSTG stats;
			pIstream->Stat(&stats, STATFLAG_NONAME);
			ULONG sSize = stats.cbSize.QuadPart;
			
			// Read the data in a buffer to be returned
			char* pBuffer = new char[sSize];
			pIstream->Read(pBuffer, sSize, bytesRead);

			// Release allocated COM
			cpStream.Release();
			cpVoice.Release();
			::CoUninitialize();
			return pBuffer;
		}
		catch (int e)
		{
			if (10 == e)
			{
				// In case of errors, release allocations and return empty buffer
				char* pBuffer = new char[0];
				cpStream.Release();
				cpVoice.Release();
				::CoUninitialize();
				return pBuffer;
			}
			else
			{
				throw e;
			}
		}
	}
}