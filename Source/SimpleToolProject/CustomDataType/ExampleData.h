#pragma once
#include "ExampleData.generated.h"

UCLASS(Blueprintable)
class SIMPLETOOLPROJECT_API UExampleData : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Properties")
        FString ExampleString;

#if WITH_EDITORONLY_DATA
    UPROPERTY(Category = SourceAsset, VisibleAnywhere)
        FString SourceFilePath;
#endif
};