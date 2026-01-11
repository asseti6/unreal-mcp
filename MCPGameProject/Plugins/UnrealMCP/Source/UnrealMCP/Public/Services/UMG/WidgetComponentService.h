#pragma once

#include "CoreMinimal.h"
#include "Json.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Components/Widget.h"
#include "Services/UMG/Widgets/BasicWidgetFactory.h"
#include "Services/UMG/Widgets/AdvancedWidgetFactory.h"
#include "Services/UMG/Widgets/LayoutWidgetFactory.h"

// Forward declarations
class UWidgetBlueprint;

/**
 * Service class for creating various UMG widget components
 * This class handles the creation and configuration of all widget component types
 */
class UNREALMCP_API FWidgetComponentService
{
public:
    FWidgetComponentService();

    /**
     * Creates a widget component of the specified type
     * @param WidgetBlueprint - The widget blueprint to add the component to
     * @param ComponentName - Name for the new component
     * @param ComponentType - Type of the component to create
     * @param Position - Position in the canvas
     * @param Size - Size of the component
     * @param KwargsObject - Additional parameters for the component
     * @param OutError - Output error message if creation failed
     * @return Created widget or nullptr if creation failed
     */
    UWidget* CreateWidgetComponent(
        UWidgetBlueprint* WidgetBlueprint,
        const FString& ComponentName,
        const FString& ComponentType,
        const FVector2D& Position,
        const FVector2D& Size,
        const TSharedPtr<FJsonObject>& KwargsObject,
        FString& OutError);

    /**
     * Get the list of supported component types
     * @return Array of supported component type names
     */
    static TArray<FString> GetSupportedComponentTypes();

    /**
     * Checks if a component type string represents a custom Widget Blueprint path
     * @param ComponentType - The component type to check
     * @return true if this looks like a Widget Blueprint reference
     */
    static bool IsCustomWidgetBlueprintPath(const FString& ComponentType);

    /**
     * Resolves a Widget Blueprint path, handling partial paths like "WBP_MyWidget"
     * @param ComponentType - The component type/path to resolve
     * @return Full asset path or empty string if not found
     */
    static FString ResolveWidgetBlueprintPath(const FString& ComponentType);

private:
    // Component creation methods for each type
    UWidget* CreateTextBlock(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateButton(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateImage(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateCheckBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateSlider(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateProgressBar(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateBorder(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateScrollBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateSpacer(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateWidgetSwitcher(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateThrobber(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateExpandableArea(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateRichTextBlock(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateMultiLineEditableText(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateVerticalBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateHorizontalBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateOverlay(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateGridPanel(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateSizeBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateCanvasPanel(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateComboBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateEditableText(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateEditableTextBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateCircularThrobber(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateSpinBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateWrapBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateScaleBox(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateNamedSlot(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateRadialSlider(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateListView(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateTileView(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateTreeView(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateSafeZone(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateMenuAnchor(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateNativeWidgetHost(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateBackgroundBlur(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);
    UWidget* CreateUniformGridPanel(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName, const TSharedPtr<FJsonObject>& KwargsObject);

    /**
     * Creates an instance of a custom Widget Blueprint and adds it as a child
     * @param ParentWidgetBlueprint - The widget blueprint to add the component to
     * @param ComponentName - Name for the new component instance
     * @param WidgetBlueprintPath - Path to the Widget Blueprint to instantiate (e.g., "/Game/UI/WBP_MyWidget" or "WBP_MyWidget")
     * @param KwargsObject - Additional parameters (currently unused for custom widgets)
     * @return Created widget instance or nullptr if creation failed
     */
    UWidget* CreateCustomWidgetBlueprint(UWidgetBlueprint* ParentWidgetBlueprint, const FString& ComponentName, const FString& WidgetBlueprintPath, const TSharedPtr<FJsonObject>& KwargsObject);

    // Helper functions
    bool GetJsonArray(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, TArray<TSharedPtr<FJsonValue>>& OutArray);
    
    /**
     * Helper method to extract the correct kwargs object (either direct or nested)
     * @param KwargsObject - The original kwargs object that might contain a nested "kwargs" field
     * @param ComponentName - Name of the component (for logging)
     * @param ComponentType - Type of the component (for logging)
     * @return The actual kwargs object to use
     */
    TSharedPtr<FJsonObject> GetKwargsToUse(const TSharedPtr<FJsonObject>& KwargsObject, const FString& ComponentName, const FString& ComponentType);
    
    /**
     * Add a widget to the widget tree and set its position/size
     * @param WidgetBlueprint - The widget blueprint
     * @param Widget - The widget to add
     * @param Position - Position in the canvas
     * @param Size - Size of the widget
     * @return true if successful
     */
    bool AddWidgetToTree(UWidgetBlueprint* WidgetBlueprint, UWidget* Widget, const FVector2D& Position, const FVector2D& Size);
    
    /**
     * Save the widget blueprint
     * @param WidgetBlueprint - The widget blueprint to save
     */
    void SaveWidgetBlueprint(UWidgetBlueprint* WidgetBlueprint);
    
    // Widget factories
    FBasicWidgetFactory BasicWidgetFactory;
    FAdvancedWidgetFactory AdvancedWidgetFactory;
    FLayoutWidgetFactory LayoutWidgetFactory;
}; 
