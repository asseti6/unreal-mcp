#pragma once

#include "CoreMinimal.h"
#include "Json.h"

// Forward declarations
class UWidgetBlueprint;
class UWidgetAnimation;
class UWidget;

/**
 * Static service class for UMG Widget Animation operations
 * Handles creation, modification, and querying of widget animations
 */
class UNREALMCP_API FWidgetAnimationService
{
public:
    /**
     * Create a new animation in a Widget Blueprint
     * @param WidgetBlueprint - The widget blueprint to add the animation to
     * @param AnimationName - Name for the new animation
     * @param Duration - Duration of the animation in seconds
     * @param OutError - Error message if creation failed
     * @return true if animation was created successfully
     */
    static bool CreateWidgetAnimation(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                      float Duration, FString& OutError);

    /**
     * Add a property track to an animation
     * @param WidgetBlueprint - The widget blueprint containing the animation
     * @param AnimationName - Name of the animation to add the track to
     * @param TargetComponent - Name of the widget component to animate
     * @param PropertyName - Name of the property to animate (e.g., "RenderOpacity")
     * @param TrackType - Type of track: "Float", "Vector2D", "Color", "Transform"
     * @param OutError - Error message if track creation failed
     * @return true if track was added successfully
     */
    static bool AddAnimationTrack(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                  const FString& TargetComponent, const FString& PropertyName,
                                  const FString& TrackType, FString& OutError);

    /**
     * Add a keyframe to an animation track
     * @param WidgetBlueprint - The widget blueprint containing the animation
     * @param AnimationName - Name of the animation
     * @param TargetComponent - Name of the widget component
     * @param PropertyName - Name of the property to keyframe
     * @param Time - Time in seconds for the keyframe
     * @param Value - Value at this keyframe (as JSON value)
     * @param OutError - Error message if keyframe creation failed
     * @return true if keyframe was added successfully
     */
    static bool AddAnimationKeyframe(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                     const FString& TargetComponent, const FString& PropertyName,
                                     float Time, const TSharedPtr<FJsonValue>& Value, FString& OutError);

    /**
     * Set properties on an animation (duration, loop mode, etc.)
     * @param WidgetBlueprint - The widget blueprint containing the animation
     * @param AnimationName - Name of the animation
     * @param Duration - New duration (-1 to skip)
     * @param LoopMode - Loop mode: "Once", "Loop", "PingPong" (empty to skip)
     * @param LoopCount - Number of loops (-1 for infinite, 0 to skip)
     * @param OutModifiedProperties - List of properties that were modified
     * @param OutError - Error message if setting properties failed
     * @return true if at least one property was set successfully
     */
    static bool SetAnimationProperties(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                       float Duration, const FString& LoopMode, int32 LoopCount,
                                       TArray<FString>& OutModifiedProperties, FString& OutError);

    /**
     * Get all animations in a Widget Blueprint
     * @param WidgetBlueprint - The widget blueprint to query
     * @param OutAnimations - Array of animation info objects as JSON
     * @param OutError - Error message if retrieval failed
     * @return true if animations were retrieved successfully
     */
    static bool GetWidgetAnimations(UWidgetBlueprint* WidgetBlueprint, TArray<TSharedPtr<FJsonValue>>& OutAnimations,
                                    FString& OutError);

    /**
     * Delete an animation from a Widget Blueprint
     * @param WidgetBlueprint - The widget blueprint containing the animation
     * @param AnimationName - Name of the animation to delete
     * @param OutError - Error message if deletion failed
     * @return true if animation was deleted successfully
     */
    static bool DeleteWidgetAnimation(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                      FString& OutError);

private:
    /**
     * Find an animation by name in a Widget Blueprint
     * @param WidgetBlueprint - The widget blueprint to search
     * @param AnimationName - Name of the animation to find
     * @return Found animation or nullptr if not found
     */
    static UWidgetAnimation* FindAnimationByName(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName);

    /**
     * Find a widget component by name in a Widget Blueprint
     * @param WidgetBlueprint - The widget blueprint to search
     * @param ComponentName - Name of the component to find
     * @return Found widget or nullptr if not found
     */
    static UWidget* FindWidgetByName(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName);

    /**
     * Get animation info as JSON object
     * @param Animation - The animation to serialize
     * @return JSON object with animation info
     */
    static TSharedPtr<FJsonObject> GetAnimationInfoAsJson(UWidgetAnimation* Animation);
};
