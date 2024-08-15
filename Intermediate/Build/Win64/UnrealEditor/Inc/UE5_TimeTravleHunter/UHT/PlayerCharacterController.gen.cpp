// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UE5_TimeTravleHunter/Public/Controller/PlayerCharacterController.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePlayerCharacterController() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_APlayerController();
UE5_TIMETRAVLEHUNTER_API UClass* Z_Construct_UClass_APlayerCharacterController();
UE5_TIMETRAVLEHUNTER_API UClass* Z_Construct_UClass_APlayerCharacterController_NoRegister();
UPackage* Z_Construct_UPackage__Script_UE5_TimeTravleHunter();
// End Cross Module References

// Begin Class APlayerCharacterController
void APlayerCharacterController::StaticRegisterNativesAPlayerCharacterController()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(APlayerCharacterController);
UClass* Z_Construct_UClass_APlayerCharacterController_NoRegister()
{
	return APlayerCharacterController::StaticClass();
}
struct Z_Construct_UClass_APlayerCharacterController_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "Controller/PlayerCharacterController.h" },
		{ "ModuleRelativePath", "Public/Controller/PlayerCharacterController.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<APlayerCharacterController>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_APlayerCharacterController_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerController,
	(UObject* (*)())Z_Construct_UPackage__Script_UE5_TimeTravleHunter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_APlayerCharacterController_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_APlayerCharacterController_Statics::ClassParams = {
	&APlayerCharacterController::StaticClass,
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
	0x009002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_APlayerCharacterController_Statics::Class_MetaDataParams), Z_Construct_UClass_APlayerCharacterController_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_APlayerCharacterController()
{
	if (!Z_Registration_Info_UClass_APlayerCharacterController.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_APlayerCharacterController.OuterSingleton, Z_Construct_UClass_APlayerCharacterController_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_APlayerCharacterController.OuterSingleton;
}
template<> UE5_TIMETRAVLEHUNTER_API UClass* StaticClass<APlayerCharacterController>()
{
	return APlayerCharacterController::StaticClass();
}
APlayerCharacterController::APlayerCharacterController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(APlayerCharacterController);
APlayerCharacterController::~APlayerCharacterController() {}
// End Class APlayerCharacterController

// Begin Registration
struct Z_CompiledInDeferFile_FID_Programmer_UE_5_UE5_TimeTravleHunter_Source_UE5_TimeTravleHunter_Public_Controller_PlayerCharacterController_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_APlayerCharacterController, APlayerCharacterController::StaticClass, TEXT("APlayerCharacterController"), &Z_Registration_Info_UClass_APlayerCharacterController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(APlayerCharacterController), 2634435587U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Programmer_UE_5_UE5_TimeTravleHunter_Source_UE5_TimeTravleHunter_Public_Controller_PlayerCharacterController_h_1153815044(TEXT("/Script/UE5_TimeTravleHunter"),
	Z_CompiledInDeferFile_FID_Programmer_UE_5_UE5_TimeTravleHunter_Source_UE5_TimeTravleHunter_Public_Controller_PlayerCharacterController_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Programmer_UE_5_UE5_TimeTravleHunter_Source_UE5_TimeTravleHunter_Public_Controller_PlayerCharacterController_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
