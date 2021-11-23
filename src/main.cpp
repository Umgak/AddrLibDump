#include "common/IDebugLog.h"  // IDebugLog
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION
#include "skse64/PluginAPI.h"  // SKSEInterface, PluginInfo
#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "version.h"  // VERSION_VERSTRING, VERSION_MAJOR
#include "versiondb.h" //meh's magical versioning header

bool DumpCurrentVersion(const SKSEInterface* skse)
{
	VersionDb db;

	if (!db.Load())
	{
		_FATALERROR("Failed to load version database for current executable!");
		return false;
	}
	else
	{
		_MESSAGE("Loaded database for %s version %s.", db.GetModuleName().c_str(), db.GetLoadedVersionString().c_str());
		_MESSAGE("Dumping database...");
		char fileName[256];
		_snprintf_s(fileName, 256, "offsets-%d.%d.%d.txt", GET_EXE_VERSION_MAJOR(skse->runtimeVersion), GET_EXE_VERSION_MINOR(skse->runtimeVersion), GET_EXE_VERSION_BUILD(skse->runtimeVersion));
		db.Dump(fileName);
		db.Clear();

	}
}



extern "C" {
	__declspec(dllexport) SKSEPluginVersionData SKSEPlugin_Version =
	{
		SKSEPluginVersionData::kVersion,
		ALD_VERSION_MAJOR,
		"Address Library Dumper",
		"Umgak",
		"",
		SKSEPluginVersionData::kVersionIndependent_AddressLibraryPostAE,
		0,
		0,
	};

	bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\AddressLibraryDumper.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);
		_MESSAGE("Address Library Dumper v%s", ALD_VERSION_VERSTRING);

		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "Address Library Dumper";
		info->version = ALD_VERSION_MAJOR;

		if(skse->isEditor)
		{
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!");
			return false;
		}
		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
		if (skse->runtimeVersion >= RUNTIME_VERSION_1_6_317)
		{
			gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\AddressLibraryDumper.log");
			gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
			gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);
			_MESSAGE("Address Library Dumper v%s", ALD_VERSION_VERSTRING);

		}
		DumpCurrentVersion(skse);
		return true;
	}
}