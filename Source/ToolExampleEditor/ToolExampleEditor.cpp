#include "ToolExampleEditor.h"
#include "MenuTool/MenuTool.h"
#include "TabTool/TabTool.h"
#include "EditorMode/ExampleEdModeTool.h"

#include "SimpleToolProject/DetailsCustomization/ExampleActor.h"
#include "DetailsCustomization/ExampleActorDetails.h"

#include "ToolExampleEditor/CustomDataType/ExampleDataTypeActions.h"
#include "IExampleModuleInterface.h"

#include "ISettingsModule.h"
#include "ISettingsContainer.h"
#include "CustomProjectSettings/ExampleSettings.h"


IMPLEMENT_GAME_MODULE(FToolExampleEditor, ToolExampleEditor)

void FToolExampleEditor::AddModuleListeners()
{
    ModuleListeners.Add(MakeShareable(new MenuTool));
    ModuleListeners.Add(MakeShareable(new TabTool));
    ModuleListeners.Add(MakeShareable(new ExampleEdModeTool));
}

void FToolExampleEditor::StartupModule()
{
    if (!IsRunningCommandlet())
    {
        FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
        LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
        MenuExtender = MakeShareable(new FExtender);
        MenuExtender->AddMenuBarExtension("Window", EExtensionHook::After, NULL, FMenuBarExtensionDelegate::CreateRaw(this, &FToolExampleEditor::MakePulldownMenu));
        LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);
    }

    {
        static FName PropertyEditor("PropertyEditor");
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
        PropertyModule.RegisterCustomClassLayout(AExampleActor::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FExampleActorDetails::MakeInstance));
    }

    {
        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        // add custom category
        EAssetTypeCategories::Type ExampleCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Example")), FText::FromString("Example"));
        // register our custom asset with example category
        TSharedPtr<IAssetTypeActions> Action = MakeShareable(new FExampleDataTypeActions(ExampleCategory));
        AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
        // saved it here for unregister later
        CreatedAssetTypeActions.Add(Action);
    }

    {
        ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
        if (SettingsModule)
        {
            TSharedPtr<ISettingsContainer> ProjectSettingsContainer = SettingsModule->GetContainer("Project");
            ProjectSettingsContainer->DescribeCategory("ExampleCategory", FText::FromString("Example Category"), FText::FromString("Example settings description text here"));

            SettingsModule->RegisterSettings("Project", "ExampleCategory", "ExampleSettings",
                FText::FromString("Example Settings"),
                FText::FromString("Configure Example Settings"),
                GetMutableDefault<UExampleSettings>()
            );
        }
    }

    IExampleModuleInterface::StartupModule();
}

void FToolExampleEditor::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomClassLayout(AExampleActor::StaticClass()->GetFName());
    }

    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
        {
            AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
        }
    }
    CreatedAssetTypeActions.Empty();

    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if (SettingsModule)
    {
        SettingsModule->UnregisterSettings("Project", "ExampleCategory", "ExampleSettings");
    }

    IExampleModuleInterface::ShutdownModule();
}

TSharedRef<FWorkspaceItem> FToolExampleEditor::MenuRoot = FWorkspaceItem::NewGroup(FText::FromString("Menu Root"));


void FToolExampleEditor::AddMenuExtension(const FMenuExtensionDelegate& extensionDelegate, FName extensionHook, const TSharedPtr<FUICommandList>& CommandList, EExtensionHook::Position position)
{
    MenuExtender->AddMenuExtension(extensionHook, position, CommandList, extensionDelegate);
}

void FToolExampleEditor::MakePulldownMenu(FMenuBarBuilder& menuBuilder)
{
    menuBuilder.AddPullDownMenu(
        FText::FromString("SimpleTool"),
        FText::FromString("Open the Example menu"),
        FNewMenuDelegate::CreateRaw(this, &FToolExampleEditor::FillPulldownMenu),
        "SimpleTool",
        FName(TEXT("SimpleTool"))
    );
}

void FToolExampleEditor::FillPulldownMenu(FMenuBuilder& menuBuilder)
{
    // just a frame for tools to fill in
    menuBuilder.BeginSection("ExampleSection", FText::FromString("Section 1"));
    menuBuilder.AddMenuSeparator(FName("Section_1"));
    menuBuilder.EndSection();

    menuBuilder.BeginSection("ExampleSection", FText::FromString("Section 2"));
    menuBuilder.AddMenuSeparator(FName("Section_2"));
    menuBuilder.EndSection();
}