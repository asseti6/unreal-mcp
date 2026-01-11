#include "Services/UMG/WidgetAnimationService.h"
#include "WidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Widget.h"
#include "Animation/WidgetAnimation.h"
#include "MovieScene.h"
#include "MovieScenePossessable.h"
#include "Tracks/MovieSceneFloatTrack.h"
#include "Animation/MovieScene2DTransformTrack.h"
#include "Tracks/MovieSceneColorTrack.h"
#include "Sections/MovieSceneFloatSection.h"
#include "Animation/MovieScene2DTransformSection.h"
#include "Sections/MovieSceneColorSection.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "Channels/MovieSceneChannelProxy.h"

bool FWidgetAnimationService::CreateWidgetAnimation(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                                     float Duration, FString& OutError)
{
    if (!WidgetBlueprint)
    {
        OutError = TEXT("Widget Blueprint is null");
        return false;
    }

    // Check if animation with this name already exists
    if (FindAnimationByName(WidgetBlueprint, AnimationName) != nullptr)
    {
        OutError = FString::Printf(TEXT("Animation '%s' already exists"), *AnimationName);
        return false;
    }

    // Create a new widget animation
    UWidgetAnimation* NewAnimation = NewObject<UWidgetAnimation>(WidgetBlueprint, *AnimationName, RF_Transactional);
    if (!NewAnimation)
    {
        OutError = TEXT("Failed to create UWidgetAnimation object");
        return false;
    }

    // UWidgetAnimation doesn't automatically create its MovieScene when using NewObject
    // We need to create and assign it explicitly
    UMovieScene* MovieScene = NewObject<UMovieScene>(NewAnimation, NAME_None, RF_Transactional);
    if (!MovieScene)
    {
        OutError = TEXT("Failed to create MovieScene for animation");
        return false;
    }

    // Assign the MovieScene to the animation
    NewAnimation->MovieScene = MovieScene;

    // Set the playback range based on duration
    FFrameRate TickResolution = MovieScene->GetTickResolution();
    FFrameNumber StartFrame = FFrameNumber(0);
    FFrameNumber EndFrame = FFrameNumber(static_cast<int32>(FMath::RoundToInt(Duration * TickResolution.AsDecimal())));

    MovieScene->SetPlaybackRange(TRange<FFrameNumber>(StartFrame, EndFrame + 1));

    // Add the animation to the widget blueprint
    WidgetBlueprint->Animations.Add(NewAnimation);

    // Mark the blueprint as modified
    WidgetBlueprint->MarkPackageDirty();

    return true;
}

bool FWidgetAnimationService::AddAnimationTrack(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                                 const FString& TargetComponent, const FString& PropertyName,
                                                 const FString& TrackType, FString& OutError)
{
    if (!WidgetBlueprint)
    {
        OutError = TEXT("Widget Blueprint is null");
        return false;
    }

    // Find the animation
    UWidgetAnimation* Animation = FindAnimationByName(WidgetBlueprint, AnimationName);
    if (!Animation)
    {
        OutError = FString::Printf(TEXT("Animation '%s' not found"), *AnimationName);
        return false;
    }

    // Find the target widget
    UWidget* TargetWidget = FindWidgetByName(WidgetBlueprint, TargetComponent);
    if (!TargetWidget)
    {
        OutError = FString::Printf(TEXT("Widget component '%s' not found"), *TargetComponent);
        return false;
    }

    UMovieScene* MovieScene = Animation->GetMovieScene();
    if (!MovieScene)
    {
        OutError = TEXT("Failed to get MovieScene from animation");
        return false;
    }

    // Get or create binding for the widget
    FGuid BindingGuid;
    bool bFoundBinding = false;

    // Check existing bindings - UE 5.7: Use FindPossessable to get name instead of deprecated Binding.GetName()
    // UE 5.7: Use const access to GetBindings() as non-const access is deprecated
    for (const FMovieSceneBinding& Binding : static_cast<const UMovieScene*>(MovieScene)->GetBindings())
    {
        FMovieScenePossessable* Possessable = MovieScene->FindPossessable(Binding.GetObjectGuid());
        if (Possessable && Possessable->GetName() == TargetComponent)
        {
            BindingGuid = Binding.GetObjectGuid();
            bFoundBinding = true;
            break;
        }
    }

    // Create new binding if not found
    if (!bFoundBinding)
    {
        // UE 5.7: AddPossessable now takes 2 args and returns the GUID
        BindingGuid = MovieScene->AddPossessable(TargetComponent, TargetWidget->GetClass());

        // Bind the widget to the animation
        FWidgetAnimationBinding AnimBinding;
        AnimBinding.WidgetName = FName(*TargetComponent);
        AnimBinding.AnimationGuid = BindingGuid;
        Animation->AnimationBindings.Add(AnimBinding);
    }

    // Create the appropriate track type
    UMovieSceneTrack* NewTrack = nullptr;

    if (TrackType.Equals(TEXT("Float"), ESearchCase::IgnoreCase))
    {
        UMovieSceneFloatTrack* FloatTrack = MovieScene->AddTrack<UMovieSceneFloatTrack>(BindingGuid);
        if (FloatTrack)
        {
            FloatTrack->SetPropertyNameAndPath(FName(*PropertyName), PropertyName);
            FloatTrack->AddSection(*FloatTrack->CreateNewSection());
            NewTrack = FloatTrack;
        }
    }
    else if (TrackType.Equals(TEXT("Transform"), ESearchCase::IgnoreCase) ||
             TrackType.Equals(TEXT("Vector2D"), ESearchCase::IgnoreCase))
    {
        UMovieScene2DTransformTrack* TransformTrack = MovieScene->AddTrack<UMovieScene2DTransformTrack>(BindingGuid);
        if (TransformTrack)
        {
            TransformTrack->SetPropertyNameAndPath(FName(*PropertyName), PropertyName);
            TransformTrack->AddSection(*TransformTrack->CreateNewSection());
            NewTrack = TransformTrack;
        }
    }
    else if (TrackType.Equals(TEXT("Color"), ESearchCase::IgnoreCase))
    {
        UMovieSceneColorTrack* ColorTrack = MovieScene->AddTrack<UMovieSceneColorTrack>(BindingGuid);
        if (ColorTrack)
        {
            ColorTrack->SetPropertyNameAndPath(FName(*PropertyName), PropertyName);
            ColorTrack->AddSection(*ColorTrack->CreateNewSection());
            NewTrack = ColorTrack;
        }
    }
    else
    {
        OutError = FString::Printf(TEXT("Unsupported track type: '%s'. Supported types: Float, Transform, Vector2D, Color"), *TrackType);
        return false;
    }

    if (!NewTrack)
    {
        OutError = FString::Printf(TEXT("Failed to create %s track"), *TrackType);
        return false;
    }

    WidgetBlueprint->MarkPackageDirty();
    return true;
}

bool FWidgetAnimationService::AddAnimationKeyframe(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                                    const FString& TargetComponent, const FString& PropertyName,
                                                    float Time, const TSharedPtr<FJsonValue>& Value, FString& OutError)
{
    if (!WidgetBlueprint)
    {
        OutError = TEXT("Widget Blueprint is null");
        return false;
    }

    if (!Value.IsValid())
    {
        OutError = TEXT("Keyframe value is null");
        return false;
    }

    // Find the animation
    UWidgetAnimation* Animation = FindAnimationByName(WidgetBlueprint, AnimationName);
    if (!Animation)
    {
        OutError = FString::Printf(TEXT("Animation '%s' not found"), *AnimationName);
        return false;
    }

    UMovieScene* MovieScene = Animation->GetMovieScene();
    if (!MovieScene)
    {
        OutError = TEXT("Failed to get MovieScene from animation");
        return false;
    }

    // Find the binding for this component
    FGuid BindingGuid;
    bool bFoundBinding = false;

    // UE 5.7: Use FindPossessable to get name instead of deprecated Binding.GetName()
    // UE 5.7: Use const access to GetBindings() as non-const access is deprecated
    for (const FMovieSceneBinding& Binding : static_cast<const UMovieScene*>(MovieScene)->GetBindings())
    {
        FMovieScenePossessable* Possessable = MovieScene->FindPossessable(Binding.GetObjectGuid());
        if (Possessable && Possessable->GetName() == TargetComponent)
        {
            BindingGuid = Binding.GetObjectGuid();
            bFoundBinding = true;
            break;
        }
    }

    if (!bFoundBinding)
    {
        OutError = FString::Printf(TEXT("No binding found for component '%s'. Add a track first."), *TargetComponent);
        return false;
    }

    // Find the track for this property
    UMovieSceneTrack* FoundTrack = nullptr;
    for (UMovieSceneTrack* Track : MovieScene->FindTracks(UMovieSceneFloatTrack::StaticClass(), BindingGuid))
    {
        if (UMovieScenePropertyTrack* PropTrack = Cast<UMovieScenePropertyTrack>(Track))
        {
            if (PropTrack->GetPropertyName() == FName(*PropertyName))
            {
                FoundTrack = Track;
                break;
            }
        }
    }

    // Also check transform and color tracks
    if (!FoundTrack)
    {
        for (UMovieSceneTrack* Track : MovieScene->FindTracks(UMovieScene2DTransformTrack::StaticClass(), BindingGuid))
        {
            if (UMovieScenePropertyTrack* PropTrack = Cast<UMovieScenePropertyTrack>(Track))
            {
                if (PropTrack->GetPropertyName() == FName(*PropertyName))
                {
                    FoundTrack = Track;
                    break;
                }
            }
        }
    }

    if (!FoundTrack)
    {
        for (UMovieSceneTrack* Track : MovieScene->FindTracks(UMovieSceneColorTrack::StaticClass(), BindingGuid))
        {
            if (UMovieScenePropertyTrack* PropTrack = Cast<UMovieScenePropertyTrack>(Track))
            {
                if (PropTrack->GetPropertyName() == FName(*PropertyName))
                {
                    FoundTrack = Track;
                    break;
                }
            }
        }
    }

    if (!FoundTrack)
    {
        OutError = FString::Printf(TEXT("No track found for property '%s' on component '%s'"), *PropertyName, *TargetComponent);
        return false;
    }

    // Calculate frame number from time
    FFrameRate TickResolution = MovieScene->GetTickResolution();
    FFrameNumber FrameNumber = FFrameNumber(static_cast<int32>(FMath::RoundToInt(Time * TickResolution.AsDecimal())));

    // Add keyframe based on track type
    if (UMovieSceneFloatTrack* FloatTrack = Cast<UMovieSceneFloatTrack>(FoundTrack))
    {
        if (FloatTrack->GetAllSections().Num() > 0)
        {
            UMovieSceneFloatSection* Section = Cast<UMovieSceneFloatSection>(FloatTrack->GetAllSections()[0]);
            if (Section)
            {
                // Expand section range if needed
                Section->SetRange(TRange<FFrameNumber>::All());

                FMovieSceneFloatChannel* Channel = Section->GetChannelProxy().GetChannel<FMovieSceneFloatChannel>(0);
                if (Channel)
                {
                    float FloatValue = 0.0f;
                    if (Value->Type == EJson::Number)
                    {
                        FloatValue = static_cast<float>(Value->AsNumber());
                    }

                    Channel->AddCubicKey(FrameNumber, FloatValue);
                    WidgetBlueprint->MarkPackageDirty();
                    return true;
                }
            }
        }
    }
    else if (UMovieScene2DTransformTrack* TransformTrack = Cast<UMovieScene2DTransformTrack>(FoundTrack))
    {
        if (TransformTrack->GetAllSections().Num() > 0)
        {
            UMovieScene2DTransformSection* Section = Cast<UMovieScene2DTransformSection>(TransformTrack->GetAllSections()[0]);
            if (Section)
            {
                Section->SetRange(TRange<FFrameNumber>::All());

                // Parse array value [X, Y] or object {X, Y, Rotation, Scale, etc.}
                if (Value->Type == EJson::Array)
                {
                    const TArray<TSharedPtr<FJsonValue>>& Arr = Value->AsArray();
                    if (Arr.Num() >= 2)
                    {
                        // Get translation channels (indices 0 and 1)
                        FMovieSceneFloatChannel* TranslationX = Section->GetChannelProxy().GetChannel<FMovieSceneFloatChannel>(0);
                        FMovieSceneFloatChannel* TranslationY = Section->GetChannelProxy().GetChannel<FMovieSceneFloatChannel>(1);

                        if (TranslationX && TranslationY)
                        {
                            TranslationX->AddCubicKey(FrameNumber, static_cast<float>(Arr[0]->AsNumber()));
                            TranslationY->AddCubicKey(FrameNumber, static_cast<float>(Arr[1]->AsNumber()));
                        }
                    }
                }

                WidgetBlueprint->MarkPackageDirty();
                return true;
            }
        }
    }
    else if (UMovieSceneColorTrack* ColorTrack = Cast<UMovieSceneColorTrack>(FoundTrack))
    {
        if (ColorTrack->GetAllSections().Num() > 0)
        {
            UMovieSceneColorSection* Section = Cast<UMovieSceneColorSection>(ColorTrack->GetAllSections()[0]);
            if (Section)
            {
                Section->SetRange(TRange<FFrameNumber>::All());

                // Parse RGBA array [R, G, B, A]
                if (Value->Type == EJson::Array)
                {
                    const TArray<TSharedPtr<FJsonValue>>& Arr = Value->AsArray();
                    if (Arr.Num() >= 3)
                    {
                        FMovieSceneFloatChannel* RedChannel = Section->GetChannelProxy().GetChannel<FMovieSceneFloatChannel>(0);
                        FMovieSceneFloatChannel* GreenChannel = Section->GetChannelProxy().GetChannel<FMovieSceneFloatChannel>(1);
                        FMovieSceneFloatChannel* BlueChannel = Section->GetChannelProxy().GetChannel<FMovieSceneFloatChannel>(2);
                        FMovieSceneFloatChannel* AlphaChannel = Section->GetChannelProxy().GetChannel<FMovieSceneFloatChannel>(3);

                        if (RedChannel && GreenChannel && BlueChannel)
                        {
                            RedChannel->AddCubicKey(FrameNumber, static_cast<float>(Arr[0]->AsNumber()));
                            GreenChannel->AddCubicKey(FrameNumber, static_cast<float>(Arr[1]->AsNumber()));
                            BlueChannel->AddCubicKey(FrameNumber, static_cast<float>(Arr[2]->AsNumber()));

                            if (AlphaChannel && Arr.Num() >= 4)
                            {
                                AlphaChannel->AddCubicKey(FrameNumber, static_cast<float>(Arr[3]->AsNumber()));
                            }
                        }
                    }
                }

                WidgetBlueprint->MarkPackageDirty();
                return true;
            }
        }
    }

    OutError = TEXT("Failed to add keyframe to track");
    return false;
}

bool FWidgetAnimationService::SetAnimationProperties(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                                      float Duration, const FString& LoopMode, int32 LoopCount,
                                                      TArray<FString>& OutModifiedProperties, FString& OutError)
{
    if (!WidgetBlueprint)
    {
        OutError = TEXT("Widget Blueprint is null");
        return false;
    }

    UWidgetAnimation* Animation = FindAnimationByName(WidgetBlueprint, AnimationName);
    if (!Animation)
    {
        OutError = FString::Printf(TEXT("Animation '%s' not found"), *AnimationName);
        return false;
    }

    UMovieScene* MovieScene = Animation->GetMovieScene();
    if (!MovieScene)
    {
        OutError = TEXT("Failed to get MovieScene from animation");
        return false;
    }

    OutModifiedProperties.Empty();

    // Set duration if specified (Duration >= 0 means set it)
    if (Duration >= 0.0f)
    {
        FFrameRate TickResolution = MovieScene->GetTickResolution();
        FFrameNumber StartFrame = FFrameNumber(0);
        FFrameNumber EndFrame = FFrameNumber(static_cast<int32>(FMath::RoundToInt(Duration * TickResolution.AsDecimal())));

        MovieScene->SetPlaybackRange(TRange<FFrameNumber>(StartFrame, EndFrame + 1));
        OutModifiedProperties.Add(TEXT("Duration"));
    }

    // Set loop mode if specified
    if (!LoopMode.IsEmpty())
    {
        // Note: UWidgetAnimation doesn't have built-in loop mode property
        // Loop behavior is typically controlled at playback time via Blueprint
        // We'll log this as a warning but not fail
        OutModifiedProperties.Add(TEXT("LoopMode (runtime only)"));
    }

    // LoopCount is also typically runtime behavior
    if (LoopCount != 0)
    {
        OutModifiedProperties.Add(TEXT("LoopCount (runtime only)"));
    }

    if (OutModifiedProperties.Num() > 0)
    {
        WidgetBlueprint->MarkPackageDirty();
        return true;
    }

    OutError = TEXT("No properties were specified to modify");
    return false;
}

bool FWidgetAnimationService::GetWidgetAnimations(UWidgetBlueprint* WidgetBlueprint,
                                                   TArray<TSharedPtr<FJsonValue>>& OutAnimations, FString& OutError)
{
    if (!WidgetBlueprint)
    {
        OutError = TEXT("Widget Blueprint is null");
        return false;
    }

    OutAnimations.Empty();

    for (UWidgetAnimation* Animation : WidgetBlueprint->Animations)
    {
        if (Animation)
        {
            TSharedPtr<FJsonObject> AnimInfo = GetAnimationInfoAsJson(Animation);
            if (AnimInfo.IsValid())
            {
                OutAnimations.Add(MakeShareable(new FJsonValueObject(AnimInfo)));
            }
        }
    }

    return true;
}

bool FWidgetAnimationService::DeleteWidgetAnimation(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName,
                                                     FString& OutError)
{
    if (!WidgetBlueprint)
    {
        OutError = TEXT("Widget Blueprint is null");
        return false;
    }

    UWidgetAnimation* Animation = FindAnimationByName(WidgetBlueprint, AnimationName);
    if (!Animation)
    {
        OutError = FString::Printf(TEXT("Animation '%s' not found"), *AnimationName);
        return false;
    }

    // Remove from animations array
    WidgetBlueprint->Animations.Remove(Animation);

    // Mark the blueprint as modified
    WidgetBlueprint->MarkPackageDirty();

    return true;
}

UWidgetAnimation* FWidgetAnimationService::FindAnimationByName(UWidgetBlueprint* WidgetBlueprint, const FString& AnimationName)
{
    if (!WidgetBlueprint)
    {
        return nullptr;
    }

    for (UWidgetAnimation* Animation : WidgetBlueprint->Animations)
    {
        if (Animation && Animation->GetName() == AnimationName)
        {
            return Animation;
        }
    }

    return nullptr;
}

UWidget* FWidgetAnimationService::FindWidgetByName(UWidgetBlueprint* WidgetBlueprint, const FString& ComponentName)
{
    if (!WidgetBlueprint || !WidgetBlueprint->WidgetTree)
    {
        return nullptr;
    }

    // Search in the widget tree
    UWidget* FoundWidget = nullptr;
    WidgetBlueprint->WidgetTree->ForEachWidget([&](UWidget* Widget)
    {
        if (Widget && Widget->GetName() == ComponentName)
        {
            FoundWidget = Widget;
        }
    });

    return FoundWidget;
}

TSharedPtr<FJsonObject> FWidgetAnimationService::GetAnimationInfoAsJson(UWidgetAnimation* Animation)
{
    if (!Animation)
    {
        return nullptr;
    }

    TSharedPtr<FJsonObject> AnimInfo = MakeShareable(new FJsonObject());

    AnimInfo->SetStringField(TEXT("name"), Animation->GetName());

    // Get duration from movie scene
    UMovieScene* MovieScene = Animation->GetMovieScene();
    if (MovieScene)
    {
        FFrameRate TickResolution = MovieScene->GetTickResolution();
        TRange<FFrameNumber> PlaybackRange = MovieScene->GetPlaybackRange();

        float Duration = 0.0f;
        if (PlaybackRange.HasLowerBound() && PlaybackRange.HasUpperBound())
        {
            FFrameNumber RangeDuration = PlaybackRange.GetUpperBoundValue() - PlaybackRange.GetLowerBoundValue();
            Duration = RangeDuration.Value / static_cast<float>(TickResolution.AsDecimal());
        }

        AnimInfo->SetNumberField(TEXT("duration"), Duration);

        // Get track info
        TArray<TSharedPtr<FJsonValue>> TracksArray;

        // UE 5.7: Use FindPossessable to get name instead of deprecated Binding.GetName()
        // UE 5.7: Use const access to GetBindings() as non-const access is deprecated
        for (const FMovieSceneBinding& Binding : static_cast<const UMovieScene*>(MovieScene)->GetBindings())
        {
            FString ComponentName;
            FMovieScenePossessable* Possessable = MovieScene->FindPossessable(Binding.GetObjectGuid());
            if (Possessable)
            {
                ComponentName = Possessable->GetName();
            }

            for (UMovieSceneTrack* Track : Binding.GetTracks())
            {
                if (UMovieScenePropertyTrack* PropTrack = Cast<UMovieScenePropertyTrack>(Track))
                {
                    TSharedPtr<FJsonObject> TrackInfo = MakeShareable(new FJsonObject());
                    TrackInfo->SetStringField(TEXT("component"), ComponentName);
                    TrackInfo->SetStringField(TEXT("property"), PropTrack->GetPropertyName().ToString());
                    TrackInfo->SetStringField(TEXT("type"), Track->GetClass()->GetName());

                    // Count keyframes
                    int32 KeyframeCount = 0;
                    for (UMovieSceneSection* Section : Track->GetAllSections())
                    {
                        FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
                        for (const FMovieSceneChannelEntry& Entry : ChannelProxy.GetAllEntries())
                        {
                            for (FMovieSceneChannel* Channel : Entry.GetChannels())
                            {
                                if (FMovieSceneFloatChannel* FloatChannel = static_cast<FMovieSceneFloatChannel*>(Channel))
                                {
                                    KeyframeCount += FloatChannel->GetNumKeys();
                                }
                            }
                        }
                    }
                    TrackInfo->SetNumberField(TEXT("keyframe_count"), KeyframeCount);

                    TracksArray.Add(MakeShareable(new FJsonValueObject(TrackInfo)));
                }
            }
        }

        AnimInfo->SetArrayField(TEXT("tracks"), TracksArray);
    }

    // Get animation bindings count
    AnimInfo->SetNumberField(TEXT("binding_count"), Animation->AnimationBindings.Num());

    return AnimInfo;
}
