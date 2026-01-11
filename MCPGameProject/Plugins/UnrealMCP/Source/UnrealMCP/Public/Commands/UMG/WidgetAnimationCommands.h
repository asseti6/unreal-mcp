#pragma once

#include "CoreMinimal.h"
#include "Commands/IUnrealMCPCommand.h"
#include "Dom/JsonObject.h"

// Forward declarations
class IUMGService;
class UWidgetBlueprint;
class UWidgetAnimation;
struct FMCPError;

/**
 * Command for creating a new animation in a Widget Blueprint
 */
class UNREALMCP_API FCreateWidgetAnimationCommand : public IUnrealMCPCommand
{
public:
    explicit FCreateWidgetAnimationCommand(TSharedPtr<IUMGService> InUMGService);

    // IUnrealMCPCommand interface
    virtual FString Execute(const FString& Parameters) override;
    virtual FString GetCommandName() const override;
    virtual bool ValidateParams(const FString& Parameters) const override;

private:
    TSharedPtr<IUMGService> UMGService;

    TSharedPtr<FJsonObject> ExecuteInternal(const TSharedPtr<FJsonObject>& Params);
    bool ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const;
    TSharedPtr<FJsonObject> CreateSuccessResponse(const FString& AnimationName, const FString& WidgetName, float Duration) const;
    TSharedPtr<FJsonObject> CreateErrorResponse(const FMCPError& Error) const;
};

/**
 * Command for adding a property track to a widget animation
 */
class UNREALMCP_API FAddAnimationTrackCommand : public IUnrealMCPCommand
{
public:
    explicit FAddAnimationTrackCommand(TSharedPtr<IUMGService> InUMGService);

    // IUnrealMCPCommand interface
    virtual FString Execute(const FString& Parameters) override;
    virtual FString GetCommandName() const override;
    virtual bool ValidateParams(const FString& Parameters) const override;

private:
    TSharedPtr<IUMGService> UMGService;

    TSharedPtr<FJsonObject> ExecuteInternal(const TSharedPtr<FJsonObject>& Params);
    bool ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const;
    TSharedPtr<FJsonObject> CreateSuccessResponse(const FString& TrackType, const FString& TargetComponent, const FString& PropertyName) const;
    TSharedPtr<FJsonObject> CreateErrorResponse(const FMCPError& Error) const;
};

/**
 * Command for adding a keyframe to an animation track
 */
class UNREALMCP_API FAddAnimationKeyframeCommand : public IUnrealMCPCommand
{
public:
    explicit FAddAnimationKeyframeCommand(TSharedPtr<IUMGService> InUMGService);

    // IUnrealMCPCommand interface
    virtual FString Execute(const FString& Parameters) override;
    virtual FString GetCommandName() const override;
    virtual bool ValidateParams(const FString& Parameters) const override;

private:
    TSharedPtr<IUMGService> UMGService;

    TSharedPtr<FJsonObject> ExecuteInternal(const TSharedPtr<FJsonObject>& Params);
    bool ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const;
    TSharedPtr<FJsonObject> CreateSuccessResponse(float Time, const FString& PropertyName) const;
    TSharedPtr<FJsonObject> CreateErrorResponse(const FMCPError& Error) const;
};

/**
 * Command for setting animation properties (duration, loop mode, etc.)
 */
class UNREALMCP_API FSetAnimationPropertiesCommand : public IUnrealMCPCommand
{
public:
    explicit FSetAnimationPropertiesCommand(TSharedPtr<IUMGService> InUMGService);

    // IUnrealMCPCommand interface
    virtual FString Execute(const FString& Parameters) override;
    virtual FString GetCommandName() const override;
    virtual bool ValidateParams(const FString& Parameters) const override;

private:
    TSharedPtr<IUMGService> UMGService;

    TSharedPtr<FJsonObject> ExecuteInternal(const TSharedPtr<FJsonObject>& Params);
    bool ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const;
    TSharedPtr<FJsonObject> CreateSuccessResponse(const FString& AnimationName, const TArray<FString>& ModifiedProperties) const;
    TSharedPtr<FJsonObject> CreateErrorResponse(const FMCPError& Error) const;
};

/**
 * Command for getting all animations in a Widget Blueprint
 */
class UNREALMCP_API FGetWidgetAnimationsCommand : public IUnrealMCPCommand
{
public:
    explicit FGetWidgetAnimationsCommand(TSharedPtr<IUMGService> InUMGService);

    // IUnrealMCPCommand interface
    virtual FString Execute(const FString& Parameters) override;
    virtual FString GetCommandName() const override;
    virtual bool ValidateParams(const FString& Parameters) const override;

private:
    TSharedPtr<IUMGService> UMGService;

    TSharedPtr<FJsonObject> ExecuteInternal(const TSharedPtr<FJsonObject>& Params);
    bool ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const;
    TSharedPtr<FJsonObject> CreateSuccessResponse(const FString& WidgetName, const TArray<TSharedPtr<FJsonValue>>& Animations) const;
    TSharedPtr<FJsonObject> CreateErrorResponse(const FMCPError& Error) const;
};

/**
 * Command for deleting an animation from a Widget Blueprint
 */
class UNREALMCP_API FDeleteWidgetAnimationCommand : public IUnrealMCPCommand
{
public:
    explicit FDeleteWidgetAnimationCommand(TSharedPtr<IUMGService> InUMGService);

    // IUnrealMCPCommand interface
    virtual FString Execute(const FString& Parameters) override;
    virtual FString GetCommandName() const override;
    virtual bool ValidateParams(const FString& Parameters) const override;

private:
    TSharedPtr<IUMGService> UMGService;

    TSharedPtr<FJsonObject> ExecuteInternal(const TSharedPtr<FJsonObject>& Params);
    bool ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const;
    TSharedPtr<FJsonObject> CreateSuccessResponse(const FString& AnimationName, const FString& WidgetName) const;
    TSharedPtr<FJsonObject> CreateErrorResponse(const FMCPError& Error) const;
};
