// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UE5_TimeTravleHunter/Public/TTH_GameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTTH_GameMode() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UE5_TIMETRAVLEHUNTER_API UClass* Z_Construct_UClass_ATTH_GameMode();
UE5_TIMETRAVLEHUNTER_API UClass* Z_Construct_UClass_ATTH_GameMode_NoRegister();
UPackage* Z_Construct_UPackage__Script_UE5_TimeTravleHunter();
// End Cross Module References

// Begin Class ATTH_GameMode
void ATTH_GameMode::StaticRegisterNativesATTH_GameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATTH_GameMode);
UClass* Z_Construct_UClass_ATTH_GameMode_NoRegister()
{
	return ATTH_GameMode::StaticClass();
}
struct Z_Construct_UClass_ATTH_GameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "TTH_GameMode.h" },
		{ "ModuleRelativePath", "Public/TTH_GameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATTH_GameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ATTH_GameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_UE5_TimeTravleHunter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATTH_GameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ATTH_GameMode_Statics::ClassParams = {
	&ATTH_GameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ATTH_GameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_ATTH_GameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ATTH_GameMode()
{
	if (!Z_Registration_Info_UClass_ATTH_GameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATTH_GameMode.OuterSingleton, Z_Construct_UClass_ATTH_GameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ATTH_GameMode.OuterSingleton;
}
template<> UE5_TIMETRAVLEHUNTER_API UClass* StaticClass<ATTH_GameMode>()
{
	return ATTH_GameMode::StaticClass();
}
ATTH_GameMode::ATTH_GameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(ATTH_GameMode);
ATTH_GameMode::~ATTH_GameMode() {}
// End Class ATTH_GameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Programmer_UE_5_UE5_TimeTravleHunter_Source_UE5_TimeTravleHunter_Public_TTH_GameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ATTH_GameMode, ATTH_GameMode::StaticClass, TEXT("ATTH_GameMode"), &Z_Registration_Info_UClass_ATTH_GameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATTH_GameMode), 10098260U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Programmer_UE_5_UE5_TimeTravleHunter_Source_UE5_TimeTravleHunter_Public_TTH_GameMode_h_3395874764(TEXT("/Script/UE5_TimeTravleHunter"),
	Z_CompiledInDeferFile_FID_Programmer_UE_5_UE5_TimeTravleHunter_Source_UE5_TimeTravleHunter_Public_TTH_GameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Programmer_UE_5_UE5_TimeTravleHunter_Source_UE5_TimeTravleHunter_Public_TTH_GameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
