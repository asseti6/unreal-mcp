#include "Commands/UMG/WidgetAnimationCommands.h"
#include "Services/UMG/IUMGService.h"
#include "MCPErrorHandler.h"
#include "MCPError.h"
#include "WidgetBlueprint.h"
#include "Animation/WidgetAnimation.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

// ============================================================================
// FCreateWidgetAnimationCommand
// ============================================================================

FCreateWidgetAnimationCommand::FCreateWidgetAnimationCommand(TSharedPtr<IUMGService> InUMGService)
    : UMGService(InUMGService)
{
}

FString FCreateWidgetAnimationCommand::Execute(const FString& Parameters)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(TEXT("Invalid JSON parameters"));
        TSharedPtr<FJsonObject> ErrorResponse = CreateErrorResponse(Error);

        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(ErrorResponse.ToSharedRef(), Writer);
        return OutputString;
    }

    TSharedPtr<FJsonObject> Response = ExecuteInternal(JsonObject);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(Response.ToSharedRef(), Writer);
    return OutputString;
}

TSharedPtr<FJsonObject> FCreateWidgetAnimationCommand::ExecuteInternal(const TSharedPtr<FJsonObject>& Params)
{
    if (!UMGService.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateInternalError(TEXT("UMG service is not available"));
        return CreateErrorResponse(Error);
    }

    FString ValidationError;
    if (!ValidateParamsInternal(Params, ValidationError))
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(ValidationError);
        return CreateErrorResponse(Error);
    }

    FString WidgetName = Params->GetStringField(TEXT("widget_name"));
    FString AnimationName = Params->GetStringField(TEXT("animation_name"));
    float Duration = Params->HasField(TEXT("duration")) ? Params->GetNumberField(TEXT("duration")) : 1.0f;

    FString OutError;
    bool bSuccess = UMGService->CreateWidgetAnimation(WidgetName, AnimationName, Duration, OutError);

    if (!bSuccess)
    {
        FMCPError Error = FMCPErrorHandler::CreateExecutionFailedError(OutError);
        return CreateErrorResponse(Error);
    }

    return CreateSuccessResponse(AnimationName, WidgetName, Duration);
}

FString FCreateWidgetAnimationCommand::GetCommandName() const
{
    return TEXT("create_widget_animation");
}

bool FCreateWidgetAnimationCommand::ValidateParams(const FString& Parameters) const
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        return false;
    }

    FString ValidationError;
    return ValidateParamsInternal(JsonObject, ValidationError);
}

bool FCreateWidgetAnimationCommand::ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const
{
    if (!Params.IsValid())
    {
        OutError = TEXT("Invalid parameters object");
        return false;
    }

    if (!Params->HasField(TEXT("widget_name")) || Params->GetStringField(TEXT("widget_name")).IsEmpty())
    {
        OutError = TEXT("widget_name is required");
        return false;
    }

    if (!Params->HasField(TEXT("animation_name")) || Params->GetStringField(TEXT("animation_name")).IsEmpty())
    {
        OutError = TEXT("animation_name is required");
        return false;
    }

    return true;
}

TSharedPtr<FJsonObject> FCreateWidgetAnimationCommand::CreateSuccessResponse(const FString& AnimationName, const FString& WidgetName, float Duration) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), true);
    ResponseObj->SetStringField(TEXT("animation_name"), AnimationName);
    ResponseObj->SetStringField(TEXT("widget_name"), WidgetName);
    ResponseObj->SetNumberField(TEXT("duration"), Duration);
    ResponseObj->SetStringField(TEXT("message"), FString::Printf(TEXT("Created animation '%s' in widget '%s'"), *AnimationName, *WidgetName));
    return ResponseObj;
}

TSharedPtr<FJsonObject> FCreateWidgetAnimationCommand::CreateErrorResponse(const FMCPError& Error) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), false);
    ResponseObj->SetStringField(TEXT("error"), Error.ErrorMessage);
    ResponseObj->SetStringField(TEXT("error_details"), Error.ErrorDetails);
    ResponseObj->SetNumberField(TEXT("error_code"), Error.ErrorCode);
    ResponseObj->SetNumberField(TEXT("error_type"), static_cast<int32>(Error.ErrorType));
    return ResponseObj;
}

// ============================================================================
// FAddAnimationTrackCommand
// ============================================================================

FAddAnimationTrackCommand::FAddAnimationTrackCommand(TSharedPtr<IUMGService> InUMGService)
    : UMGService(InUMGService)
{
}

FString FAddAnimationTrackCommand::Execute(const FString& Parameters)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(TEXT("Invalid JSON parameters"));
        TSharedPtr<FJsonObject> ErrorResponse = CreateErrorResponse(Error);

        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(ErrorResponse.ToSharedRef(), Writer);
        return OutputString;
    }

    TSharedPtr<FJsonObject> Response = ExecuteInternal(JsonObject);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(Response.ToSharedRef(), Writer);
    return OutputString;
}

TSharedPtr<FJsonObject> FAddAnimationTrackCommand::ExecuteInternal(const TSharedPtr<FJsonObject>& Params)
{
    if (!UMGService.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateInternalError(TEXT("UMG service is not available"));
        return CreateErrorResponse(Error);
    }

    FString ValidationError;
    if (!ValidateParamsInternal(Params, ValidationError))
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(ValidationError);
        return CreateErrorResponse(Error);
    }

    FString WidgetName = Params->GetStringField(TEXT("widget_name"));
    FString AnimationName = Params->GetStringField(TEXT("animation_name"));
    FString TargetComponent = Params->GetStringField(TEXT("target_component"));
    FString PropertyName = Params->GetStringField(TEXT("property_name"));
    FString TrackType = Params->HasField(TEXT("track_type")) ? Params->GetStringField(TEXT("track_type")) : TEXT("Float");

    FString OutError;
    bool bSuccess = UMGService->AddAnimationTrack(WidgetName, AnimationName, TargetComponent, PropertyName, TrackType, OutError);

    if (!bSuccess)
    {
        FMCPError Error = FMCPErrorHandler::CreateExecutionFailedError(OutError);
        return CreateErrorResponse(Error);
    }

    return CreateSuccessResponse(TrackType, TargetComponent, PropertyName);
}

FString FAddAnimationTrackCommand::GetCommandName() const
{
    return TEXT("add_animation_track");
}

bool FAddAnimationTrackCommand::ValidateParams(const FString& Parameters) const
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        return false;
    }

    FString ValidationError;
    return ValidateParamsInternal(JsonObject, ValidationError);
}

bool FAddAnimationTrackCommand::ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const
{
    if (!Params.IsValid())
    {
        OutError = TEXT("Invalid parameters object");
        return false;
    }

    if (!Params->HasField(TEXT("widget_name")) || Params->GetStringField(TEXT("widget_name")).IsEmpty())
    {
        OutError = TEXT("widget_name is required");
        return false;
    }

    if (!Params->HasField(TEXT("animation_name")) || Params->GetStringField(TEXT("animation_name")).IsEmpty())
    {
        OutError = TEXT("animation_name is required");
        return false;
    }

    if (!Params->HasField(TEXT("target_component")) || Params->GetStringField(TEXT("target_component")).IsEmpty())
    {
        OutError = TEXT("target_component is required");
        return false;
    }

    if (!Params->HasField(TEXT("property_name")) || Params->GetStringField(TEXT("property_name")).IsEmpty())
    {
        OutError = TEXT("property_name is required");
        return false;
    }

    return true;
}

TSharedPtr<FJsonObject> FAddAnimationTrackCommand::CreateSuccessResponse(const FString& TrackType, const FString& TargetComponent, const FString& PropertyName) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), true);
    ResponseObj->SetStringField(TEXT("track_type"), TrackType);
    ResponseObj->SetStringField(TEXT("target_component"), TargetComponent);
    ResponseObj->SetStringField(TEXT("property_name"), PropertyName);
    ResponseObj->SetStringField(TEXT("message"), FString::Printf(TEXT("Added %s track for %s.%s"), *TrackType, *TargetComponent, *PropertyName));
    return ResponseObj;
}

TSharedPtr<FJsonObject> FAddAnimationTrackCommand::CreateErrorResponse(const FMCPError& Error) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), false);
    ResponseObj->SetStringField(TEXT("error"), Error.ErrorMessage);
    ResponseObj->SetStringField(TEXT("error_details"), Error.ErrorDetails);
    ResponseObj->SetNumberField(TEXT("error_code"), Error.ErrorCode);
    ResponseObj->SetNumberField(TEXT("error_type"), static_cast<int32>(Error.ErrorType));
    return ResponseObj;
}

// ============================================================================
// FAddAnimationKeyframeCommand
// ============================================================================

FAddAnimationKeyframeCommand::FAddAnimationKeyframeCommand(TSharedPtr<IUMGService> InUMGService)
    : UMGService(InUMGService)
{
}

FString FAddAnimationKeyframeCommand::Execute(const FString& Parameters)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(TEXT("Invalid JSON parameters"));
        TSharedPtr<FJsonObject> ErrorResponse = CreateErrorResponse(Error);

        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(ErrorResponse.ToSharedRef(), Writer);
        return OutputString;
    }

    TSharedPtr<FJsonObject> Response = ExecuteInternal(JsonObject);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(Response.ToSharedRef(), Writer);
    return OutputString;
}

TSharedPtr<FJsonObject> FAddAnimationKeyframeCommand::ExecuteInternal(const TSharedPtr<FJsonObject>& Params)
{
    if (!UMGService.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateInternalError(TEXT("UMG service is not available"));
        return CreateErrorResponse(Error);
    }

    FString ValidationError;
    if (!ValidateParamsInternal(Params, ValidationError))
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(ValidationError);
        return CreateErrorResponse(Error);
    }

    FString WidgetName = Params->GetStringField(TEXT("widget_name"));
    FString AnimationName = Params->GetStringField(TEXT("animation_name"));
    FString TargetComponent = Params->GetStringField(TEXT("target_component"));
    FString PropertyName = Params->GetStringField(TEXT("property_name"));
    float Time = Params->GetNumberField(TEXT("time"));

    // Get the value - can be float, vector, or color
    TSharedPtr<FJsonValue> ValueField = Params->TryGetField(TEXT("value"));

    FString OutError;
    bool bSuccess = UMGService->AddAnimationKeyframe(WidgetName, AnimationName, TargetComponent, PropertyName, Time, ValueField, OutError);

    if (!bSuccess)
    {
        FMCPError Error = FMCPErrorHandler::CreateExecutionFailedError(OutError);
        return CreateErrorResponse(Error);
    }

    return CreateSuccessResponse(Time, PropertyName);
}

FString FAddAnimationKeyframeCommand::GetCommandName() const
{
    return TEXT("add_animation_keyframe");
}

bool FAddAnimationKeyframeCommand::ValidateParams(const FString& Parameters) const
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        return false;
    }

    FString ValidationError;
    return ValidateParamsInternal(JsonObject, ValidationError);
}

bool FAddAnimationKeyframeCommand::ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const
{
    if (!Params.IsValid())
    {
        OutError = TEXT("Invalid parameters object");
        return false;
    }

    if (!Params->HasField(TEXT("widget_name")) || Params->GetStringField(TEXT("widget_name")).IsEmpty())
    {
        OutError = TEXT("widget_name is required");
        return false;
    }

    if (!Params->HasField(TEXT("animation_name")) || Params->GetStringField(TEXT("animation_name")).IsEmpty())
    {
        OutError = TEXT("animation_name is required");
        return false;
    }

    if (!Params->HasField(TEXT("target_component")) || Params->GetStringField(TEXT("target_component")).IsEmpty())
    {
        OutError = TEXT("target_component is required");
        return false;
    }

    if (!Params->HasField(TEXT("property_name")) || Params->GetStringField(TEXT("property_name")).IsEmpty())
    {
        OutError = TEXT("property_name is required");
        return false;
    }

    if (!Params->HasField(TEXT("time")))
    {
        OutError = TEXT("time is required");
        return false;
    }

    if (!Params->HasField(TEXT("value")))
    {
        OutError = TEXT("value is required");
        return false;
    }

    return true;
}

TSharedPtr<FJsonObject> FAddAnimationKeyframeCommand::CreateSuccessResponse(float Time, const FString& PropertyName) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), true);
    ResponseObj->SetNumberField(TEXT("time"), Time);
    ResponseObj->SetStringField(TEXT("property_name"), PropertyName);
    ResponseObj->SetStringField(TEXT("message"), FString::Printf(TEXT("Added keyframe at time %.3f for property %s"), Time, *PropertyName));
    return ResponseObj;
}

TSharedPtr<FJsonObject> FAddAnimationKeyframeCommand::CreateErrorResponse(const FMCPError& Error) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), false);
    ResponseObj->SetStringField(TEXT("error"), Error.ErrorMessage);
    ResponseObj->SetStringField(TEXT("error_details"), Error.ErrorDetails);
    ResponseObj->SetNumberField(TEXT("error_code"), Error.ErrorCode);
    ResponseObj->SetNumberField(TEXT("error_type"), static_cast<int32>(Error.ErrorType));
    return ResponseObj;
}

// ============================================================================
// FSetAnimationPropertiesCommand
// ============================================================================

FSetAnimationPropertiesCommand::FSetAnimationPropertiesCommand(TSharedPtr<IUMGService> InUMGService)
    : UMGService(InUMGService)
{
}

FString FSetAnimationPropertiesCommand::Execute(const FString& Parameters)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(TEXT("Invalid JSON parameters"));
        TSharedPtr<FJsonObject> ErrorResponse = CreateErrorResponse(Error);

        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(ErrorResponse.ToSharedRef(), Writer);
        return OutputString;
    }

    TSharedPtr<FJsonObject> Response = ExecuteInternal(JsonObject);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(Response.ToSharedRef(), Writer);
    return OutputString;
}

TSharedPtr<FJsonObject> FSetAnimationPropertiesCommand::ExecuteInternal(const TSharedPtr<FJsonObject>& Params)
{
    if (!UMGService.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateInternalError(TEXT("UMG service is not available"));
        return CreateErrorResponse(Error);
    }

    FString ValidationError;
    if (!ValidateParamsInternal(Params, ValidationError))
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(ValidationError);
        return CreateErrorResponse(Error);
    }

    FString WidgetName = Params->GetStringField(TEXT("widget_name"));
    FString AnimationName = Params->GetStringField(TEXT("animation_name"));

    // Optional properties
    float Duration = Params->HasField(TEXT("duration")) ? Params->GetNumberField(TEXT("duration")) : -1.0f;
    FString LoopMode = Params->HasField(TEXT("loop_mode")) ? Params->GetStringField(TEXT("loop_mode")) : TEXT("");
    int32 LoopCount = Params->HasField(TEXT("loop_count")) ? static_cast<int32>(Params->GetNumberField(TEXT("loop_count"))) : -1;

    TArray<FString> ModifiedProperties;
    FString OutError;
    bool bSuccess = UMGService->SetAnimationProperties(WidgetName, AnimationName, Duration, LoopMode, LoopCount, ModifiedProperties, OutError);

    if (!bSuccess)
    {
        FMCPError Error = FMCPErrorHandler::CreateExecutionFailedError(OutError);
        return CreateErrorResponse(Error);
    }

    return CreateSuccessResponse(AnimationName, ModifiedProperties);
}

FString FSetAnimationPropertiesCommand::GetCommandName() const
{
    return TEXT("set_animation_properties");
}

bool FSetAnimationPropertiesCommand::ValidateParams(const FString& Parameters) const
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        return false;
    }

    FString ValidationError;
    return ValidateParamsInternal(JsonObject, ValidationError);
}

bool FSetAnimationPropertiesCommand::ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const
{
    if (!Params.IsValid())
    {
        OutError = TEXT("Invalid parameters object");
        return false;
    }

    if (!Params->HasField(TEXT("widget_name")) || Params->GetStringField(TEXT("widget_name")).IsEmpty())
    {
        OutError = TEXT("widget_name is required");
        return false;
    }

    if (!Params->HasField(TEXT("animation_name")) || Params->GetStringField(TEXT("animation_name")).IsEmpty())
    {
        OutError = TEXT("animation_name is required");
        return false;
    }

    // At least one property to set should be provided
    if (!Params->HasField(TEXT("duration")) && !Params->HasField(TEXT("loop_mode")) && !Params->HasField(TEXT("loop_count")))
    {
        OutError = TEXT("At least one property (duration, loop_mode, or loop_count) must be provided");
        return false;
    }

    return true;
}

TSharedPtr<FJsonObject> FSetAnimationPropertiesCommand::CreateSuccessResponse(const FString& AnimationName, const TArray<FString>& ModifiedProperties) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), true);
    ResponseObj->SetStringField(TEXT("animation_name"), AnimationName);

    TArray<TSharedPtr<FJsonValue>> PropertiesArray;
    for (const FString& Prop : ModifiedProperties)
    {
        PropertiesArray.Add(MakeShared<FJsonValueString>(Prop));
    }
    ResponseObj->SetArrayField(TEXT("modified_properties"), PropertiesArray);
    ResponseObj->SetStringField(TEXT("message"), FString::Printf(TEXT("Modified %d properties on animation '%s'"), ModifiedProperties.Num(), *AnimationName));
    return ResponseObj;
}

TSharedPtr<FJsonObject> FSetAnimationPropertiesCommand::CreateErrorResponse(const FMCPError& Error) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), false);
    ResponseObj->SetStringField(TEXT("error"), Error.ErrorMessage);
    ResponseObj->SetStringField(TEXT("error_details"), Error.ErrorDetails);
    ResponseObj->SetNumberField(TEXT("error_code"), Error.ErrorCode);
    ResponseObj->SetNumberField(TEXT("error_type"), static_cast<int32>(Error.ErrorType));
    return ResponseObj;
}

// ============================================================================
// FGetWidgetAnimationsCommand
// ============================================================================

FGetWidgetAnimationsCommand::FGetWidgetAnimationsCommand(TSharedPtr<IUMGService> InUMGService)
    : UMGService(InUMGService)
{
}

FString FGetWidgetAnimationsCommand::Execute(const FString& Parameters)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(TEXT("Invalid JSON parameters"));
        TSharedPtr<FJsonObject> ErrorResponse = CreateErrorResponse(Error);

        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(ErrorResponse.ToSharedRef(), Writer);
        return OutputString;
    }

    TSharedPtr<FJsonObject> Response = ExecuteInternal(JsonObject);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(Response.ToSharedRef(), Writer);
    return OutputString;
}

TSharedPtr<FJsonObject> FGetWidgetAnimationsCommand::ExecuteInternal(const TSharedPtr<FJsonObject>& Params)
{
    if (!UMGService.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateInternalError(TEXT("UMG service is not available"));
        return CreateErrorResponse(Error);
    }

    FString ValidationError;
    if (!ValidateParamsInternal(Params, ValidationError))
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(ValidationError);
        return CreateErrorResponse(Error);
    }

    FString WidgetName = Params->GetStringField(TEXT("widget_name"));

    TArray<TSharedPtr<FJsonValue>> Animations;
    FString OutError;
    bool bSuccess = UMGService->GetWidgetAnimations(WidgetName, Animations, OutError);

    if (!bSuccess)
    {
        FMCPError Error = FMCPErrorHandler::CreateExecutionFailedError(OutError);
        return CreateErrorResponse(Error);
    }

    return CreateSuccessResponse(WidgetName, Animations);
}

FString FGetWidgetAnimationsCommand::GetCommandName() const
{
    return TEXT("get_widget_animations");
}

bool FGetWidgetAnimationsCommand::ValidateParams(const FString& Parameters) const
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        return false;
    }

    FString ValidationError;
    return ValidateParamsInternal(JsonObject, ValidationError);
}

bool FGetWidgetAnimationsCommand::ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const
{
    if (!Params.IsValid())
    {
        OutError = TEXT("Invalid parameters object");
        return false;
    }

    if (!Params->HasField(TEXT("widget_name")) || Params->GetStringField(TEXT("widget_name")).IsEmpty())
    {
        OutError = TEXT("widget_name is required");
        return false;
    }

    return true;
}

TSharedPtr<FJsonObject> FGetWidgetAnimationsCommand::CreateSuccessResponse(const FString& WidgetName, const TArray<TSharedPtr<FJsonValue>>& Animations) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), true);
    ResponseObj->SetStringField(TEXT("widget_name"), WidgetName);
    ResponseObj->SetArrayField(TEXT("animations"), Animations);
    ResponseObj->SetNumberField(TEXT("animation_count"), Animations.Num());
    return ResponseObj;
}

TSharedPtr<FJsonObject> FGetWidgetAnimationsCommand::CreateErrorResponse(const FMCPError& Error) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), false);
    ResponseObj->SetStringField(TEXT("error"), Error.ErrorMessage);
    ResponseObj->SetStringField(TEXT("error_details"), Error.ErrorDetails);
    ResponseObj->SetNumberField(TEXT("error_code"), Error.ErrorCode);
    ResponseObj->SetNumberField(TEXT("error_type"), static_cast<int32>(Error.ErrorType));
    return ResponseObj;
}

// ============================================================================
// FDeleteWidgetAnimationCommand
// ============================================================================

FDeleteWidgetAnimationCommand::FDeleteWidgetAnimationCommand(TSharedPtr<IUMGService> InUMGService)
    : UMGService(InUMGService)
{
}

FString FDeleteWidgetAnimationCommand::Execute(const FString& Parameters)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(TEXT("Invalid JSON parameters"));
        TSharedPtr<FJsonObject> ErrorResponse = CreateErrorResponse(Error);

        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(ErrorResponse.ToSharedRef(), Writer);
        return OutputString;
    }

    TSharedPtr<FJsonObject> Response = ExecuteInternal(JsonObject);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(Response.ToSharedRef(), Writer);
    return OutputString;
}

TSharedPtr<FJsonObject> FDeleteWidgetAnimationCommand::ExecuteInternal(const TSharedPtr<FJsonObject>& Params)
{
    if (!UMGService.IsValid())
    {
        FMCPError Error = FMCPErrorHandler::CreateInternalError(TEXT("UMG service is not available"));
        return CreateErrorResponse(Error);
    }

    FString ValidationError;
    if (!ValidateParamsInternal(Params, ValidationError))
    {
        FMCPError Error = FMCPErrorHandler::CreateValidationFailedError(ValidationError);
        return CreateErrorResponse(Error);
    }

    FString WidgetName = Params->GetStringField(TEXT("widget_name"));
    FString AnimationName = Params->GetStringField(TEXT("animation_name"));

    FString OutError;
    bool bSuccess = UMGService->DeleteWidgetAnimation(WidgetName, AnimationName, OutError);

    if (!bSuccess)
    {
        FMCPError Error = FMCPErrorHandler::CreateExecutionFailedError(OutError);
        return CreateErrorResponse(Error);
    }

    return CreateSuccessResponse(AnimationName, WidgetName);
}

FString FDeleteWidgetAnimationCommand::GetCommandName() const
{
    return TEXT("delete_widget_animation");
}

bool FDeleteWidgetAnimationCommand::ValidateParams(const FString& Parameters) const
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Parameters);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        return false;
    }

    FString ValidationError;
    return ValidateParamsInternal(JsonObject, ValidationError);
}

bool FDeleteWidgetAnimationCommand::ValidateParamsInternal(const TSharedPtr<FJsonObject>& Params, FString& OutError) const
{
    if (!Params.IsValid())
    {
        OutError = TEXT("Invalid parameters object");
        return false;
    }

    if (!Params->HasField(TEXT("widget_name")) || Params->GetStringField(TEXT("widget_name")).IsEmpty())
    {
        OutError = TEXT("widget_name is required");
        return false;
    }

    if (!Params->HasField(TEXT("animation_name")) || Params->GetStringField(TEXT("animation_name")).IsEmpty())
    {
        OutError = TEXT("animation_name is required");
        return false;
    }

    return true;
}

TSharedPtr<FJsonObject> FDeleteWidgetAnimationCommand::CreateSuccessResponse(const FString& AnimationName, const FString& WidgetName) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), true);
    ResponseObj->SetStringField(TEXT("animation_name"), AnimationName);
    ResponseObj->SetStringField(TEXT("widget_name"), WidgetName);
    ResponseObj->SetStringField(TEXT("message"), FString::Printf(TEXT("Deleted animation '%s' from widget '%s'"), *AnimationName, *WidgetName));
    return ResponseObj;
}

TSharedPtr<FJsonObject> FDeleteWidgetAnimationCommand::CreateErrorResponse(const FMCPError& Error) const
{
    TSharedPtr<FJsonObject> ResponseObj = MakeShared<FJsonObject>();
    ResponseObj->SetBoolField(TEXT("success"), false);
    ResponseObj->SetStringField(TEXT("error"), Error.ErrorMessage);
    ResponseObj->SetStringField(TEXT("error_details"), Error.ErrorDetails);
    ResponseObj->SetNumberField(TEXT("error_code"), Error.ErrorCode);
    ResponseObj->SetNumberField(TEXT("error_type"), static_cast<int32>(Error.ErrorType));
    return ResponseObj;
}
