#include "Services/UMG/WidgetComponentService.h"
#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "Components/Spacer.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Throbber.h"
#include "Components/ExpandableArea.h"
#include "Components/RichTextBlock.h"
#include "Components/MultiLineEditableText.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/GridPanel.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "EditorAssetLibrary.h"
#include "Components/CircularThrobber.h"
#include "Components/SpinBox.h"
#include "Components/WrapBox.h"
#include "Components/ScaleBox.h"
#include "Components/NamedSlot.h"
#include "Components/RadialSlider.h"
#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Components/TreeView.h"
#include "Components/SafeZone.h"
#include "Components/MenuAnchor.h"
#include "Components/NativeWidgetHost.h"
#include "Components/BackgroundBlur.h"
#include "Components/UniformGridPanel.h"
#include "AssetRegistry/AssetRegistryModule.h"

FWidgetComponentService::FWidgetComponentService()
{
}

TArray<FString> FWidgetComponentService::GetSupportedComponentTypes()
{
    return TArray<FString>{
        TEXT("TextBlock"), TEXT("Button"), TEXT("Image"), TEXT("CheckBox"), TEXT("Slider"),
        TEXT("ProgressBar"), TEXT("Border"), TEXT("ScrollBox"), TEXT("Spacer"), TEXT("WidgetSwitcher"),
        TEXT("Throbber"), TEXT("ExpandableArea"), TEXT("RichTextBlock"), TEXT("MultiLineEditableText"),
        TEXT("VerticalBox"), TEXT("HorizontalBox"), TEXT("Overlay"), TEXT("GridPanel"), TEXT("SizeBox"),
        TEXT("CanvasPanel"), TEXT("ComboBox"), TEXT("EditableText"), TEXT("EditableTextBox"),
        TEXT("CircularThrobber"), TEXT("SpinBox"), TEXT("WrapBox"), TEXT("ScaleBox"), TEXT("NamedSlot"),
        TEXT("RadialSlider"), TEXT("ListView"), TEXT("TileView"), TEXT("TreeView"), TEXT("SafeZone"),
        TEXT("MenuAnchor"), TEXT("NativeWidgetHost"), TEXT("BackgroundBlur"), TEXT("UniformGridPanel")
    };
}

UWidget* FWidgetComponentService::CreateWidgetComponent(
    UWidgetBlueprint* WidgetBlueprint,
    const FString& ComponentName,
    const FString& ComponentType,
    const FVector2D& Position,
    const FVector2D& Size,
    const TSharedPtr<FJsonObject>& KwargsObject,
    FString& OutError)
{
    OutError.Empty();

    // Log the received KwargsObject
    FString JsonString = TEXT("null");
    if (KwargsObject.IsValid())
    {
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
        FJsonSerializer::Serialize(KwargsObject.ToSharedRef(), Writer);
    }
    UE_LOG(LogTemp, Log, TEXT("FWidgetComponentService::CreateWidgetComponent Received Kwargs for %s (%s): %s"), *ComponentName, *ComponentType, *JsonString);

    // Create the appropriate widget based on component type
    UWidget* CreatedWidget = nullptr;

    // TextBlock
    if (ComponentType.Equals(TEXT("TextBlock"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateTextBlock(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // Button
    else if (ComponentType.Equals(TEXT("Button"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateButton(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // Image
    else if (ComponentType.Equals(TEXT("Image"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateImage(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // CheckBox
    else if (ComponentType.Equals(TEXT("CheckBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateCheckBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // Slider
    else if (ComponentType.Equals(TEXT("Slider"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateSlider(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // ProgressBar
    else if (ComponentType.Equals(TEXT("ProgressBar"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateProgressBar(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // Border
    else if (ComponentType.Equals(TEXT("Border"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateBorder(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // ScrollBox
    else if (ComponentType.Equals(TEXT("ScrollBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateScrollBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // Spacer
    else if (ComponentType.Equals(TEXT("Spacer"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateSpacer(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // WidgetSwitcher
    else if (ComponentType.Equals(TEXT("WidgetSwitcher"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateWidgetSwitcher(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // Throbber
    else if (ComponentType.Equals(TEXT("Throbber"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateThrobber(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // ExpandableArea
    else if (ComponentType.Equals(TEXT("ExpandableArea"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateExpandableArea(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // RichTextBlock
    else if (ComponentType.Equals(TEXT("RichTextBlock"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateRichTextBlock(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // MultiLineEditableText
    else if (ComponentType.Equals(TEXT("MultiLineEditableText"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateMultiLineEditableText(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // VerticalBox
    else if (ComponentType.Equals(TEXT("VerticalBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateVerticalBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // HorizontalBox
    else if (ComponentType.Equals(TEXT("HorizontalBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateHorizontalBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // Overlay
    else if (ComponentType.Equals(TEXT("Overlay"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateOverlay(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // GridPanel
    else if (ComponentType.Equals(TEXT("GridPanel"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateGridPanel(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // SizeBox
    else if (ComponentType.Equals(TEXT("SizeBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateSizeBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // CanvasPanel
    else if (ComponentType.Equals(TEXT("CanvasPanel"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateCanvasPanel(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // ComboBox
    else if (ComponentType.Equals(TEXT("ComboBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateComboBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // EditableText
    else if (ComponentType.Equals(TEXT("EditableText"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateEditableText(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // EditableTextBox
    else if (ComponentType.Equals(TEXT("EditableTextBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateEditableTextBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // CircularThrobber
    else if (ComponentType.Equals(TEXT("CircularThrobber"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateCircularThrobber(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // SpinBox
    else if (ComponentType.Equals(TEXT("SpinBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateSpinBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // WrapBox
    else if (ComponentType.Equals(TEXT("WrapBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateWrapBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // ScaleBox
    else if (ComponentType.Equals(TEXT("ScaleBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateScaleBox(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // NamedSlot
    else if (ComponentType.Equals(TEXT("NamedSlot"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateNamedSlot(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // RadialSlider
    else if (ComponentType.Equals(TEXT("RadialSlider"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateRadialSlider(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // ListView
    else if (ComponentType.Equals(TEXT("ListView"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateListView(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // TileView
    else if (ComponentType.Equals(TEXT("TileView"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateTileView(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // TreeView
    else if (ComponentType.Equals(TEXT("TreeView"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateTreeView(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // SafeZone
    else if (ComponentType.Equals(TEXT("SafeZone"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateSafeZone(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // MenuAnchor
    else if (ComponentType.Equals(TEXT("MenuAnchor"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateMenuAnchor(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // NativeWidgetHost
    else if (ComponentType.Equals(TEXT("NativeWidgetHost"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateNativeWidgetHost(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // BackgroundBlur
    else if (ComponentType.Equals(TEXT("BackgroundBlur"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateBackgroundBlur(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // StackBox (not a standard UE widget, use VerticalBox instead)
    else if (ComponentType.Equals(TEXT("StackBox"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateVerticalBox(WidgetBlueprint, ComponentName, KwargsObject);
        UE_LOG(LogTemp, Warning, TEXT("StackBox is not available in this UE version. Using VerticalBox instead for '%s'."), *ComponentName);
    }
    // UniformGridPanel
    else if (ComponentType.Equals(TEXT("UniformGridPanel"), ESearchCase::IgnoreCase))
    {
        CreatedWidget = CreateUniformGridPanel(WidgetBlueprint, ComponentName, KwargsObject);
    }
    // Check for custom Widget Blueprint path (e.g., "WBP_MyWidget" or "/Game/UI/WBP_MyWidget")
    else if (IsCustomWidgetBlueprintPath(ComponentType))
    {
        FString ResolvedPath = ResolveWidgetBlueprintPath(ComponentType);
        if (ResolvedPath.IsEmpty())
        {
            OutError = FString::Printf(TEXT("Could not find Widget Blueprint: %s"), *ComponentType);
            UE_LOG(LogTemp, Error, TEXT("%s"), *OutError);
            return nullptr;
        }
        CreatedWidget = CreateCustomWidgetBlueprint(WidgetBlueprint, ComponentName, ResolvedPath, KwargsObject);
    }
    // Default case for unsupported types
    else
    {
        // Build a helpful error message listing supported types
        TArray<FString> SupportedTypes = GetSupportedComponentTypes();
        FString SupportedTypesStr = FString::Join(SupportedTypes, TEXT(", "));

        OutError = FString::Printf(
            TEXT("Unsupported component type '%s'. Note: You can use Widget Blueprint paths like 'WBP_MyWidget' or '/Game/UI/WBP_MyWidget'. Supported built-in types: %s"),
            *ComponentType, *SupportedTypesStr);
        UE_LOG(LogTemp, Error, TEXT("%s"), *OutError);
        return nullptr;
    }

    // If widget creation failed, return nullptr
    if (!CreatedWidget)
    {
        OutError = FString::Printf(TEXT("Widget factory returned null for component '%s' of type '%s'. The widget type is supported but creation failed internally."), *ComponentName, *ComponentType);
        UE_LOG(LogTemp, Error, TEXT("%s"), *OutError);
        return nullptr;
    }

    // Add the widget to the widget tree
    if (!AddWidgetToTree(WidgetBlueprint, CreatedWidget, Position, Size))
    {
        OutError = FString::Printf(TEXT("Failed to add widget '%s' to the widget tree. The widget was created but could not be added to the blueprint's root canvas."), *ComponentName);
        UE_LOG(LogTemp, Error, TEXT("%s"), *OutError);
        return nullptr;
    }

    // Save the blueprint
    SaveWidgetBlueprint(WidgetBlueprint);

    UE_LOG(LogTemp, Log, TEXT("Successfully created and added widget component: %s"), *ComponentName);
    return CreatedWidget;
}

bool FWidgetComponentService::GetJsonArray(const TSharedPtr<FJsonObject>& JsonObject, 
                                         const FString& FieldName, 
                                         TArray<TSharedPtr<FJsonValue>>& OutArray)
{
    if (!JsonObject->HasField(FieldName))
    {
        return false;
    }

    // In UE 5.5, TryGetArrayField API has changed
    const TArray<TSharedPtr<FJsonValue>>* ArrayPtr;
    bool bSuccess = JsonObject->TryGetArrayField(FStringView(*FieldName), ArrayPtr);
    if (bSuccess)
    {
        OutArray = *ArrayPtr;
        return true;
    }
    return false;
}

TSharedPtr<FJsonObject> FWidgetComponentService::GetKwargsToUse(const TSharedPtr<FJsonObject>& KwargsObject, const FString& ComponentName, const FString& ComponentType)
{
    // Debug: Print the KwargsObject structure
    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(KwargsObject.ToSharedRef(), Writer);
    UE_LOG(LogTemp, Display, TEXT("KwargsObject for %s '%s': %s"), *ComponentType, *ComponentName, *JsonString);
    
    // Determine if we're using nested kwargs
    if (KwargsObject->HasField(TEXT("kwargs")))
    {
        TSharedPtr<FJsonObject> NestedKwargs = KwargsObject->GetObjectField(TEXT("kwargs"));
        UE_LOG(LogTemp, Display, TEXT("Using nested kwargs for %s '%s'"), *ComponentType, *ComponentName);
        return NestedKwargs;
    }
    
    return KwargsObject;
}

UWidget* FWidgetComponentService::CreateTextBlock(UWidgetBlueprint* WidgetBlueprint, 
                                               const FString& ComponentName, 
                                               const TSharedPtr<FJsonObject>& KwargsObject)
{
    // Get the proper kwargs object (direct or nested)
    TSharedPtr<FJsonObject> KwargsToUse = GetKwargsToUse(KwargsObject, ComponentName, TEXT("TextBlock"));
    return BasicWidgetFactory.CreateTextBlock(WidgetBlueprint, ComponentName, KwargsToUse);
}

UWidget* FWidgetComponentService::CreateButton(UWidgetBlueprint* WidgetBlueprint, 
                                           const FString& ComponentName, 
                                           const TSharedPtr<FJsonObject>& KwargsObject)
{
    // Get the proper kwargs object (direct or nested)
    TSharedPtr<FJsonObject> KwargsToUse = GetKwargsToUse(KwargsObject, ComponentName, TEXT("Button"));
    return BasicWidgetFactory.CreateButton(WidgetBlueprint, ComponentName, KwargsToUse);
}

UWidget* FWidgetComponentService::CreateImage(UWidgetBlueprint* WidgetBlueprint, 
                                          const FString& ComponentName, 
                                          const TSharedPtr<FJsonObject>& KwargsObject)
{
    // Get the proper kwargs object (direct or nested)
    TSharedPtr<FJsonObject> KwargsToUse = GetKwargsToUse(KwargsObject, ComponentName, TEXT("Image"));
    return BasicWidgetFactory.CreateImage(WidgetBlueprint, ComponentName, KwargsToUse);
}

UWidget* FWidgetComponentService::CreateCheckBox(UWidgetBlueprint* WidgetBlueprint, 
                                              const FString& ComponentName, 
                                              const TSharedPtr<FJsonObject>& KwargsObject)
{
    // Get the proper kwargs object (direct or nested)
    TSharedPtr<FJsonObject> KwargsToUse = GetKwargsToUse(KwargsObject, ComponentName, TEXT("CheckBox"));
    return BasicWidgetFactory.CreateCheckBox(WidgetBlueprint, ComponentName, KwargsToUse);
}

UWidget* FWidgetComponentService::CreateSlider(UWidgetBlueprint* WidgetBlueprint, 
                                            const FString& ComponentName, 
                                            const TSharedPtr<FJsonObject>& KwargsObject)
{
    // Get the proper kwargs object (direct or nested)
    TSharedPtr<FJsonObject> KwargsToUse = GetKwargsToUse(KwargsObject, ComponentName, TEXT("Slider"));
    return BasicWidgetFactory.CreateSlider(WidgetBlueprint, ComponentName, KwargsToUse);
}

UWidget* FWidgetComponentService::CreateProgressBar(UWidgetBlueprint* WidgetBlueprint, 
                                                 const FString& ComponentName, 
                                                 const TSharedPtr<FJsonObject>& KwargsObject)
{
    // Get the proper kwargs object (direct or nested)
    TSharedPtr<FJsonObject> KwargsToUse = GetKwargsToUse(KwargsObject, ComponentName, TEXT("ProgressBar"));
    return BasicWidgetFactory.CreateProgressBar(WidgetBlueprint, ComponentName, KwargsToUse);
}

UWidget* FWidgetComponentService::CreateBorder(UWidgetBlueprint* WidgetBlueprint, 
                                            const FString& ComponentName, 
                                            const TSharedPtr<FJsonObject>& KwargsObject)
{
    // Get the proper kwargs object (direct or nested)
    TSharedPtr<FJsonObject> KwargsToUse = GetKwargsToUse(KwargsObject, ComponentName, TEXT("Border"));
    return AdvancedWidgetFactory.CreateBorder(WidgetBlueprint, ComponentName, KwargsToUse);
}

UWidget* FWidgetComponentService::CreateScrollBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return LayoutWidgetFactory.CreateScrollBox(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateSpacer(UWidgetBlueprint* WidgetBlueprint, 
                                            const FString& ComponentName, 
                                            const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateSpacer(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateWidgetSwitcher(UWidgetBlueprint* WidgetBlueprint, 
                                                    const FString& ComponentName, 
                                                    const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateWidgetSwitcher(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateThrobber(UWidgetBlueprint* WidgetBlueprint, 
                                              const FString& ComponentName, 
                                              const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateThrobber(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateExpandableArea(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateExpandableArea(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateRichTextBlock(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateRichTextBlock(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateMultiLineEditableText(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateMultiLineEditableText(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateVerticalBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return LayoutWidgetFactory.CreateVerticalBox(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateHorizontalBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return LayoutWidgetFactory.CreateHorizontalBox(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateOverlay(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return LayoutWidgetFactory.CreateOverlay(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateGridPanel(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return LayoutWidgetFactory.CreateGridPanel(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateSizeBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return LayoutWidgetFactory.CreateSizeBox(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateCanvasPanel(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return LayoutWidgetFactory.CreateCanvasPanel(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateComboBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateComboBox(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateEditableText(UWidgetBlueprint* WidgetBlueprint, 
                                                  const FString& ComponentName, 
                                                  const TSharedPtr<FJsonObject>& KwargsObject)
{
    return BasicWidgetFactory.CreateEditableText(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateEditableTextBox(UWidgetBlueprint* WidgetBlueprint, 
                                                     const FString& ComponentName, 
                                                     const TSharedPtr<FJsonObject>& KwargsObject)
{
    return BasicWidgetFactory.CreateEditableTextBox(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateCircularThrobber(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateCircularThrobber(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateSpinBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateSpinBox(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateWrapBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return LayoutWidgetFactory.CreateWrapBox(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateScaleBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateScaleBox(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateNamedSlot(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateNamedSlot(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateRadialSlider(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateRadialSlider(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateListView(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateListView(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateTileView(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateTileView(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateTreeView(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateTreeView(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateSafeZone(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateSafeZone(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateMenuAnchor(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateMenuAnchor(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateNativeWidgetHost(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateNativeWidgetHost(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateBackgroundBlur(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return AdvancedWidgetFactory.CreateBackgroundBlur(WidgetBlueprint, ComponentName, KwargsObject);
}

UWidget* FWidgetComponentService::CreateUniformGridPanel(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject)
{
    return LayoutWidgetFactory.CreateUniformGridPanel(WidgetBlueprint, ComponentName, KwargsObject);
}

bool FWidgetComponentService::AddWidgetToTree(UWidgetBlueprint* WidgetBlueprint, UWidget* Widget, const FVector2D& Position, const FVector2D& Size)
{
    if (!WidgetBlueprint || !Widget)
    {
        return false;
    }

    if (!WidgetBlueprint->WidgetTree)
    {
        UE_LOG(LogTemp, Error, TEXT("Widget blueprint has no WidgetTree"));
        return false;
    }

    // Get the root widget (should be a CanvasPanel)
    UWidget* RootWidget = WidgetBlueprint->WidgetTree->RootWidget;
    if (!RootWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("Widget blueprint has no root widget"));
        return false;
    }

    // Try to cast to CanvasPanel (most common case)
    if (UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(RootWidget))
    {
        // Add the widget as a child of the canvas panel
        UCanvasPanelSlot* Slot = CanvasPanel->AddChildToCanvas(Widget);
        if (Slot)
        {
            // Set position and size
            Slot->SetPosition(Position);
            Slot->SetSize(Size);
            Slot->SetAlignment(FVector2D(0.0f, 0.0f)); // Top-left alignment

            // CRITICAL FIX: Automatically expose widget as variable so it can be referenced in Blueprint graphs
            // This solves the issue where widget components cannot be accessed by node_tools because
            // they're not marked as variables by default
            Widget->bIsVariable = true;
            UE_LOG(LogTemp, Log, TEXT("Exposed widget '%s' as variable (bIsVariable = true)"), *Widget->GetName());

            UE_LOG(LogTemp, Log, TEXT("Added widget '%s' to canvas panel at position [%f, %f] with size [%f, %f]"),
                *Widget->GetName(), Position.X, Position.Y, Size.X, Size.Y);
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to add widget to canvas panel"));
            return false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Root widget is not a CanvasPanel, cannot set position/size. Root widget type: %s"), 
            *RootWidget->GetClass()->GetName());
        
        // For non-canvas panels, we can still try to add the widget but without position/size control
        // This would require different logic based on the panel type
        return false;
    }
}

void FWidgetComponentService::SaveWidgetBlueprint(UWidgetBlueprint* WidgetBlueprint)
{
    if (!WidgetBlueprint)
    {
        return;
    }

    // Mark the blueprint as dirty
    WidgetBlueprint->MarkPackageDirty();

    // Compile the blueprint
    FKismetEditorUtilities::CompileBlueprint(WidgetBlueprint);

    // Save the asset
    UEditorAssetLibrary::SaveAsset(WidgetBlueprint->GetPathName(), false);

    UE_LOG(LogTemp, Log, TEXT("Saved widget blueprint: %s"), *WidgetBlueprint->GetName());
}

bool FWidgetComponentService::IsCustomWidgetBlueprintPath(const FString& ComponentType)
{
    // Check if this looks like a widget blueprint reference:
    // - Starts with WBP_ (widget blueprint naming convention)
    // - Starts with /Game/ (full path)
    // - Contains "Widget" in the name

    if (ComponentType.StartsWith(TEXT("WBP_"), ESearchCase::IgnoreCase))
    {
        return true;
    }

    if (ComponentType.StartsWith(TEXT("/Game/")))
    {
        return true;
    }

    // Also accept paths that look like widget blueprints even without WBP_ prefix
    // This allows using shorter names like "InventorySlot" if the asset exists
    if (ComponentType.Contains(TEXT("Widget"), ESearchCase::IgnoreCase))
    {
        return true;
    }

    return false;
}

FString FWidgetComponentService::ResolveWidgetBlueprintPath(const FString& ComponentType)
{
    // If it's already a full path, check if it exists
    if (ComponentType.StartsWith(TEXT("/Game/")))
    {
        FString AssetPath = ComponentType;
        // Ensure the path includes the asset name properly
        if (!AssetPath.Contains(TEXT(".")))
        {
            AssetPath = FString::Printf(TEXT("%s.%s"), *AssetPath, *FPaths::GetBaseFilename(AssetPath));
        }

        if (UEditorAssetLibrary::DoesAssetExist(AssetPath))
        {
            return AssetPath;
        }

        // Try without the .AssetName suffix
        FString PathWithoutSuffix = ComponentType;
        if (UEditorAssetLibrary::DoesAssetExist(PathWithoutSuffix))
        {
            return PathWithoutSuffix;
        }

        return FString();
    }

    // Search for the widget blueprint in common locations
    TArray<FString> SearchPaths = {
        TEXT("/Game/UI/Widgets"),
        TEXT("/Game/UI/Widgets/Menus"),
        TEXT("/Game/UI/Widgets/Menus/Inventory"),
        TEXT("/Game/UI/Widgets/HUD"),
        TEXT("/Game/UI"),
        TEXT("/Game/Widgets"),
        TEXT("/Game")
    };

    FString SearchName = ComponentType;
    // Remove any file extension if present
    if (SearchName.Contains(TEXT(".")))
    {
        SearchName = FPaths::GetBaseFilename(SearchName);
    }

    for (const FString& BasePath : SearchPaths)
    {
        FString FullPath = FString::Printf(TEXT("%s/%s"), *BasePath, *SearchName);
        FString AssetPath = FString::Printf(TEXT("%s.%s"), *FullPath, *SearchName);

        if (UEditorAssetLibrary::DoesAssetExist(AssetPath))
        {
            UE_LOG(LogTemp, Log, TEXT("Resolved widget blueprint '%s' to '%s'"), *ComponentType, *AssetPath);
            return AssetPath;
        }

        // Also try without the .AssetName suffix
        if (UEditorAssetLibrary::DoesAssetExist(FullPath))
        {
            UE_LOG(LogTemp, Log, TEXT("Resolved widget blueprint '%s' to '%s'"), *ComponentType, *FullPath);
            return FullPath;
        }
    }

    // If not found in common paths, try using Asset Registry to search
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    FARFilter Filter;
    Filter.ClassPaths.Add(UWidgetBlueprint::StaticClass()->GetClassPathName());
    Filter.bRecursivePaths = true;
    Filter.PackagePaths.Add(FName(TEXT("/Game")));

    TArray<FAssetData> AssetDataList;
    AssetRegistry.GetAssets(Filter, AssetDataList);

    for (const FAssetData& AssetData : AssetDataList)
    {
        if (AssetData.AssetName.ToString().Equals(SearchName, ESearchCase::IgnoreCase))
        {
            FString FoundPath = AssetData.GetObjectPathString();
            UE_LOG(LogTemp, Log, TEXT("Found widget blueprint '%s' via Asset Registry: '%s'"), *ComponentType, *FoundPath);
            return FoundPath;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Could not resolve widget blueprint path: %s"), *ComponentType);
    return FString();
}

UWidget* FWidgetComponentService::CreateCustomWidgetBlueprint(
    UWidgetBlueprint* ParentWidgetBlueprint,
    const FString& ComponentName,
    const FString& WidgetBlueprintPath,
    const TSharedPtr<FJsonObject>& KwargsObject)
{
    UE_LOG(LogTemp, Log, TEXT("Creating custom widget blueprint instance: %s from %s"), *ComponentName, *WidgetBlueprintPath);

    // Load the widget blueprint
    UObject* LoadedAsset = UEditorAssetLibrary::LoadAsset(WidgetBlueprintPath);
    UWidgetBlueprint* SourceWidgetBlueprint = Cast<UWidgetBlueprint>(LoadedAsset);

    if (!SourceWidgetBlueprint)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load widget blueprint: %s"), *WidgetBlueprintPath);
        return nullptr;
    }

    // Get the generated class from the source widget blueprint
    UClass* WidgetClass = SourceWidgetBlueprint->GeneratedClass;
    if (!WidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Widget blueprint has no generated class: %s"), *WidgetBlueprintPath);
        return nullptr;
    }

    // Create an instance of the widget using the widget tree
    UWidgetTree* WidgetTree = ParentWidgetBlueprint->WidgetTree;
    if (!WidgetTree)
    {
        UE_LOG(LogTemp, Error, TEXT("Parent widget blueprint has no widget tree"));
        return nullptr;
    }

    // Use WidgetTree->ConstructWidget to create the widget instance
    UWidget* NewWidget = WidgetTree->ConstructWidget<UWidget>(WidgetClass, FName(*ComponentName));

    if (!NewWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to construct widget instance from class: %s"), *WidgetClass->GetName());
        return nullptr;
    }

    // Mark as variable so it can be referenced in Blueprint graphs
    NewWidget->bIsVariable = true;

    UE_LOG(LogTemp, Log, TEXT("Successfully created custom widget blueprint instance: %s (class: %s)"),
        *ComponentName, *WidgetClass->GetName());

    return NewWidget;
}
