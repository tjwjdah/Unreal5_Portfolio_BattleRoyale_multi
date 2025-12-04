// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapComponent.h"
#include "POIW.h"

#include "../../Character/BRCharacter.h"
#include "../../World/Zone.h"
#include "Engine/TextureRenderTarget2D.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

void UMinimapComponent::NativePreConstruct()
{
    Super::NativeConstruct();
    if (!bInit)
        Init();
}

void UMinimapComponent::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (UpdateMinimap) {
        if (IsValid(OwnerW)) {
            UseLastKown = false;
            LastKnownOwnerLocation = IsBigMap ? FVector(0.f, 0.f, OwnerW->GetActorLocation().Z) : OwnerW->GetActorLocation();
        }
        else {
            UseLastKown = true; 
        }
        LastKnownOwnerControlRotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
        MPDynMat->SetScalarParameterValue(TEXT("X"), (LastKnownOwnerLocation.X / MapLengthCenter) * -1.f);
        MPDynMat->SetScalarParameterValue(TEXT("Y"), LastKnownOwnerLocation.Y / MapLengthCenter);
        UpdatePOIs();
        ViewAngleImage->SetRenderTransformAngle(LastKnownOwnerControlRotation.Yaw);
        if (!UseLastKown) 
            MyLocationImage->SetRenderTransformAngle(OwnerW->GetActorRotation().Yaw);
        OverlayMother->SetRenderTransformAngle(RotateMap ? LastKnownOwnerControlRotation.Yaw * -1.f : 0.f);
        BorderImage->SetRenderTransformAngle((BorderRotates && (RotateMap || IsCircular)) ? LastKnownOwnerControlRotation.Yaw * -1.f : 0.f);
    }
    if (!IsBigMap)return;
    if (OMapScale == 1.f || DoReset) {
        FVector2D Current = RenderTransform.Translation;
        float NewX = FMath::FInterpTo(Current.X, 0.f, InDeltaTime, 3.f);
        float NewY = FMath::FInterpTo(Current.Y, 0.f, InDeltaTime, 3.f);
        SetRenderTranslation(FVector2D(NewX, NewY));
    }
}

void UMinimapComponent::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
    MouseIn = true;
}

void UMinimapComponent::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
    MouseIn = false;
    IsMouseDown = false;

}

FReply UMinimapComponent::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    OnMinimapClick(InGeometry, InMouseEvent);
    LastKnowLoc = InMouseEvent.GetScreenSpacePosition();
 
    return FReply::Handled();
}

FReply UMinimapComponent::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    if (InMouseEvent.GetEffectingButton() == MouseButtonToDrag)
        IsMouseDown = false;

    return FReply::Handled();
}

FReply UMinimapComponent::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseMove(InGeometry, InMouseEvent);
    if (!(IsBigMap && IsMouseDown && CanDragMap))
        return FReply::Handled();
    if (OMapScale > 1.f) {
        FVector2D Loc = RenderTransform.Translation + InMouseEvent.GetScreenSpacePosition() - LastKnowLoc;
        SetRenderTranslation(FVector2D(FMath::Clamp(Loc.X, (OMapScale * 300.f * -1.f), (OMapScale * 300.f)),
            FMath::Clamp(Loc.Y, (OMapScale * 300.f * -1.f), (OMapScale * 300.f))));
        LastKnowLoc = InMouseEvent.GetScreenSpacePosition();
    }
    return FReply::Handled();
}

FReply UMinimapComponent::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
    if (!MouseWheelZoomOnHover || !MouseIn)     return FReply::Handled();

   
    if (InMouseEvent.GetWheelDelta() > 0.f) {
        if (IsBigMap) {
            DoReset = false;
            OMapScale = FMath::Clamp(GetRenderTransform().Scale.X + ZoomSteps, 1.f, 2.f);
            PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"), GetRenderTransform().Scale.X));
            SetRenderScale(FVector2D(OMapScale, OMapScale));
            return FReply::Handled();
        }
        else
           MinimapZoomIn();
    }
    else {
        if (IsBigMap) {

            OMapScale = FMath::Clamp(GetRenderTransform().Scale.X + (ZoomSteps * -1.f), 1.f, 2.f);
            PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("%f"), OMapScale));
            SetRenderScale(FVector2D(OMapScale, OMapScale));
            SetRenderTranslation(FVector2D(FMath::Clamp(RenderTransform.Translation.X, (OMapScale * 300.f * -1.f), (OMapScale * 300.f)),
                FMath::Clamp(RenderTransform.Translation.Y, (OMapScale * 300.f * -1.f), (OMapScale * 300.f))));

            return FReply::Handled();
        }
        else 
            MinimapZoomOut();
    }
  
    return FReply::Handled();

}

int32 UMinimapComponent::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
    FPaintContext Context(AllottedGeometry, MyCullingRect,
        OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
    float LocalSize = GetCachedGeometry().GetLocalSize().X;
    if (!IsValid(Zone)) 
        return 0;
    float ScaleRatio = Zone->GetZoneMesh()->Bounds.BoxExtent.X / Zone->GetActorTransform().GetScale3D().X;
    float Size = Zone->GetZoneMesh()->Bounds.BoxExtent.X / MapLengthInTexture * LocalSize;
    FVector2D Coord((MapLengthCenter+Zone->GetActorTransform().GetLocation().Y) / MapLengthInTexture*LocalSize, 
        (MapLengthCenter + (Zone->GetActorTransform().GetLocation().X*-1.f))/MapLengthInTexture*LocalSize);
    MDrawCircle(Context, Coord, Size, Zone->GetZoneCircleColor());
    if (!Zone->GetDrawNextPoint())return 0;
    Size = (Zone->GetMinimapTargetScale().X * ScaleRatio) / MapLengthInTexture * LocalSize;
    Coord = FVector2D((MapLengthCenter +Zone->GetMinimapNextLocation().Y) / MapLengthInTexture * LocalSize,
        (MapLengthCenter + (Zone->GetMinimapNextLocation().X * -1.f)) / MapLengthInTexture * LocalSize);
    MDrawCircle(Context, Coord, Size, Zone->GetZoneNextPointColor());
    return int32();
}


bool UMinimapComponent::Initialize()
{
    if (!Super::Initialize())
        return false;   
    
    if (MinimapTextures.Num() > 0) return true;

    UTexture2D* Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Widget/Minimap/Images/ArrDown.ArrDown'"));
    if(Texture)
    MinimapTextures.Add(Texture);

    Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Widget/Minimap/Images/ArrUP.ArrUP'"));
    if (Texture)
        MinimapTextures.Add(Texture);

   Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Widget/Minimap/Images/BorderXT.BorderXT'"));
    if (Texture)
        MinimapTextures.Add(Texture);

     Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Widget/Minimap/Images/LocationPointer.LocationPointer'"));
    if (Texture)
        MinimapTextures.Add(Texture);

     Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Widget/Minimap/Images/MyPlayerPosition.MyPlayerPosition'"));
    if (Texture)
        MinimapTextures.Add(Texture);

    Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Widget/Minimap/Images/OutOfMapIndicator2.OutOfMapIndicator2'"));
    if (Texture)
        MinimapTextures.Add(Texture);

    Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Widget/Minimap/Images/PlaneT.PlaneT'"));
    if (Texture)
        MinimapTextures.Add(Texture);

    Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Widget/Minimap/Images/PositionIndic.PositionIndic'"));
    if (Texture)
        MinimapTextures.Add(Texture);
 
    Texture = LoadObject<UTexture2D>(nullptr,
        TEXT("Texture2D'/Game/Widget/Minimap/Images/ZoneT.ZoneT'"));
    if (Texture)
        MinimapTextures.Add(Texture);
    
    
    return true;
}
void UMinimapComponent::ToggleMyLocationVisibility()
{
    if (MyLocationImage->GetVisibility() == ESlateVisibility::Hidden) {
        if (IsBigMap)
            MyLocVisibility = !MyLocVisibility;
        else
            MyLocationImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    else {
        if (IsBigMap)
            MyLocVisibility = !MyLocVisibility;
        else
            MyLocationImage->SetVisibility(ESlateVisibility::Hidden);
    }

}

void UMinimapComponent::AddZone(AZone* _Zone)
{
    Zone = _Zone;
}

UPOIW* UMinimapComponent::AddActor(AActor* _actor, bool _FollowActorRotation, EMinimapImages _Image , FVector2D _TextureSize, FSlateColor _Color, bool _ShowHeightIndicator, FText _ToolTip)
{
    return AddPointInterest(_Color, _actor, false, _FollowActorRotation, MinimapTextures[static_cast<int32>(_Image)], _TextureSize, nullptr, false, false, false, FVector::ZeroVector, _ShowHeightIndicator, _ToolTip);
}

UPOIW* UMinimapComponent::AddActor(AActor* _actor, bool _FollowActorRotation, UTexture2D* _Textur, FVector2D _TextureSize, FSlateColor _Color, bool _ShowHeightIndicator, FText _ToolTip)
{
    return AddPointInterest(_Color, _actor, false, _FollowActorRotation, _Textur, _TextureSize, nullptr, false, false, false, FVector::ZeroVector, _ShowHeightIndicator, _ToolTip);
}

UPOIW* UMinimapComponent::AddPersistentActor(AActor* _actor, bool _FollowActorRotation, UTexture2D* _Texture, FVector2D _TextureSize, FSlateColor _Color, UTexture2D* _OutOfMapTexture,
    bool _KeepImageStraight, bool _OnlyOffMap, bool _ShowHeightIndicator, FText _ToolTip)
{
    return AddPointInterest(_Color, _actor, false, _FollowActorRotation, _Texture, _TextureSize, _OutOfMapTexture, _KeepImageStraight, _OnlyOffMap, false, FVector::ZeroVector, _ShowHeightIndicator, _ToolTip);
}



void UMinimapComponent::ChangeMinimapTexture(UTexture2D* _Texture, UTextureRenderTarget2D* _Target, float _MapLength, bool _DesignTime, bool _Construct)
{
    MapLengthInTexture = _MapLength;
    MapLengthCenter = _MapLength / 2.f;
    MapTexture = _Texture;
    OrRenderTargetTexture = _Target;
    UTexture* Tex = (IsValid(_Texture))? static_cast<UTexture*>(_Texture): static_cast<UTexture*>(_Target);
    if (IsValid(Tex))
        MinimapImage->SetBrushSize(FVector2D(Tex->GetSurfaceWidth(), Tex->GetSurfaceHeight()));
    UMaterialInstanceDynamic* Dym = MinimapImage->GetDynamicMaterial();
    Dym->SetTextureParameterValue(TEXT("Map"), Tex);
    Zoom = FMath::Clamp(Zoom, 0.01, 1.f);
    Dym->SetScalarParameterValue(TEXT("Zoom"), Zoom);
   
    if (!_DesignTime)
        MPDynMat = Dym;
}

void UMinimapComponent::SetZoomValue(float _Zoom)
{
    Zoom = FMath::Clamp(_Zoom, 0.1f, MaxZoom);
    MPDynMat->SetScalarParameterValue(TEXT("Zoom"), Zoom);
}

bool UMinimapComponent::MinimapStartUpdate(APawn* _Pawn)
{
    if (!IsValid(_Pawn))return false;
    if (IsBigMap && IsValid(OwnerW)) {
        if (OwnerW != _Pawn)
            RemoveActor(OwnerW);
    }
    OwnerW = _Pawn;
    if (IsBigMap && !IsDesignTime() && IsValid(MyLocationTexture)) {
        if (!IsOwnerAdd())
          OwnerWidget = AddActor(OwnerW, true, MyLocationTexture, MyLocationTextureSize, MyLocationColor, false, FText::FromString(TEXT("")));
    }
    UpdateMinimap = true;
    return true;
}

bool UMinimapComponent::IsOwnerAdd()
{
    for (int32 i = 0; i < POIWS.Num(); i++) {
        if (POIWS[i]->ActorRef == OwnerW)
            return true;
    }
    return false;
}

bool UMinimapComponent::RemoveActor(AActor* _Actor)
{
    bool Removed;
    int32 Index;
    for (int32 i = 0; i < POIWS.Num(); i++) {
        Index = i;
        if (POIWS[i]->ActorRef == _Actor) {
            POIWS[i]->RemoveFromParent();
            Removed = true;
            break;
        }
    }
    if (Removed)
        POIWS.RemoveAt(Index);
    return Removed;
}


void UMinimapComponent::Init()
{
    bInit = true;
    MinimapImage->SetBrushFromMaterial(MapMaterial);
    ChangeMinimapTexture(MapTexture, OrRenderTargetTexture, MapLengthInTexture,IsDesignTime(),true);
    ViewAngleImage->SetVisibility(IsValid(ViewAngleTexture) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
    if (IsValid(ViewAngleTexture))
    {
        ViewAngleImage->SetBrushFromTexture(ViewAngleTexture);
        ViewAngleImage->SetBrushTintColor(ViewAngleColor);
        ViewAngleImage->SetBrushSize(ViewAngleTextureSize);
    }
    MyLocationImage->SetVisibility(IsValid(MyLocationImage) && (!IsBigMap || IsDesignTime()) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
    if (IsValid(MyLocationImage) && (!IsBigMap || IsDesignTime())) {
        MyLocationImage->SetBrushFromTexture(MyLocationTexture);
        MyLocationImage->SetBrushTintColor(MyLocationColor);
        MyLocationImage->SetBrushSize(MyLocationTextureSize);
    }
    if (IsCircular) 
        MinimapImage->GetDynamicMaterial()->SetTextureParameterValue(TEXT("Round"), OpacityMask);
    else
        MinimapImage->GetDynamicMaterial()->SetTextureParameterValue(TEXT("Round"), NoMask);

    if(IsValid(MinimapBorderTexture))
        BorderImage->SetBrushFromTexture(MinimapBorderTexture);
    BorderImage->SetBrushSize(FVector2D(BorderSize, BorderSize));
    BorderImage->SetColorAndOpacity(IsValid(MinimapBorderTexture) ? FLinearColor::White : FLinearColor(0.f, 0.f, 0.f, 0.f));
}

void UMinimapComponent::UpdatePOIs()
{
    ToBeDeleted.Empty();
    float X; float Y; float LeftPadd; float BorderS; float BottomPadd;

    for (int32 i = 0; i < POIWS.Num();i++) {
        if (!IsValid(POIWS[i]->GetActorRef())) {
                ToBeDeleted.Add(POIWS[i]);
            continue;
        }

        if (OwnerWidget == POIWS[i] && !MyLocVisibility) {
            POIWS[i]->SetVisibility(ESlateVisibility::Hidden);
            continue;
        }

        if (TheOverlay->GetCachedGeometry().GetLocalSize().X <= 0.f)
            continue;
        BorderS = TheOverlay->GetCachedGeometry().GetLocalSize().X / 2.f;
        X = (POIWS[i]->GetActorRef()->GetActorLocation().X - LastKnownOwnerLocation.X) * BorderS / (Zoom *MapLengthCenter) ;
        Y = (POIWS[i]->GetActorRef()->GetActorLocation().Y - LastKnownOwnerLocation.Y) * BorderS / (Zoom *MapLengthCenter) ;
        if (IsCircular) {
            ClampXY(X, Y, BorderS, 0.f);
            if (IsOutsideCircle(X / BorderS, Y / BorderS, UKismetSystemLibrary::MakeLiteralFloat(1.f))) {
                if (POIWS[i]->GetIsMinimapPersistent()) {
                    RoundMapAdjsuter(X, Y, BorderS, 0.f,LeftPadd,BottomPadd);
                    POIWS[i]->GetMyOverlaySlot()->SetPadding(FMargin(LeftPadd, 0.f, 0.f, BottomPadd));
                    POIWS[i]->OutOfMap(true, RotateMap, LastKnownOwnerLocation, LastKnownOwnerControlRotation);
                    if (IsBigMap && !TempSkipOnce)
                        TempSkipOnce = true;
                    else
                        POIWS[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
                }
                else {
                    POIWS[i]->SetVisibility(ESlateVisibility::Hidden);
                }
            }
            else {
                if (POIWS[i]->GetIsMinimapPersistent() && POIWS[i]->GetOnlyShowOnOutOfMinimap()) {
                    POIWS[i]->SetVisibility(ESlateVisibility::Hidden);
                }
                else {
                    POIWS[i]->GetMyOverlaySlot()->SetPadding(FMargin(Y, 0.f, 0.f, X));
                    POIWS[i]->OutOfMap(false, RotateMap, LastKnownOwnerLocation, LastKnownOwnerControlRotation);
                    if (IsBigMap && !TempSkipOnce)
                        TempSkipOnce = true;
                    else
                        POIWS[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
                }
            }
        }
        else {
            if(!POIWS[i]->GetIsMinimapPersistent() && (abs(X) > BorderS || abs(Y) > BorderS))
                POIWS[i]->SetVisibility(ESlateVisibility::Hidden);
            else {
                bool Clamp = ClampXY(X, Y, BorderS, 0.f);
                if(!Clamp && POIWS[i]->GetIsMinimapPersistent() && POIWS[i]->GetOnlyShowOnOutOfMinimap())
                    POIWS[i]->SetVisibility(ESlateVisibility::Hidden);
                else
                {
                    POIWS[i]->GetMyOverlaySlot()->SetPadding(FMargin(Y, 0.f, 0.f, X));
                    POIWS[i]->OutOfMap(Clamp, RotateMap, LastKnownOwnerLocation, LastKnownOwnerControlRotation);
                    if (IsBigMap && !TempSkipOnce)
                        TempSkipOnce = true;
                    else 
                        POIWS[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
                   
                }
            }
        }
    }
    
    for (int32 i = 0; i < ToBeDeleted.Num(); i++) {
        POIWS.Remove(ToBeDeleted[i]);
        ToBeDeleted[i]->RemoveFromParent();
    }
}

bool UMinimapComponent::ClampXY(float& X, float& Y, float _BorderSize, float _ExtraClamp)
{
    X = FMath::Clamp(X, _BorderSize * -1.f + _ExtraClamp, _BorderSize - _ExtraClamp);
    Y = FMath::Clamp(Y, _BorderSize * -1.f + _ExtraClamp, _BorderSize - _ExtraClamp);

    return abs(X)== (_BorderSize - _ExtraClamp) || (_BorderSize -_ExtraClamp) == abs(Y);
}

bool UMinimapComponent::IsOutsideCircle(float _X, float _Y, float _R)
{
    return FMath::Sqrt(_X*_X + _Y*_Y) >_R ;
}

void UMinimapComponent::RoundMapAdjsuter(float _X, float _Y, float _BorderSize, float _Clamp, float& Left, float& Bottom)
{
    if (!IsCircular)
    {
        Left = _Y;
        Bottom = _X;
    
    }
    else {
        float XCircle; float YCircle;
        SquareCoorCircle(_Y / BorderSize, _X / BorderSize, XCircle, YCircle);
        Left = (BorderSize * XCircle) - _Clamp;
        Bottom = (BorderSize * YCircle) - _Clamp;
    }
}

void UMinimapComponent::EValidateMinimapClick(FVector _Loc, FKey _Button)
{
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this,_Loc,_Button] {
        if (!IsMouseDown) {
            CV(ABRCharacter, Char, OwnerW);
            if (Char)
                Char->OverviewMapClicked(_Loc,_Button);
        }
        }, 0.2f, false);
}

FReply UMinimapComponent::OnMinimapClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    
    FVector Loc = IsBigMap ? LastKnownOwnerLocation : (IsValid(OwnerW) ? OwnerW->GetActorLocation() : LastKnownOwnerLocation);
    if (MouseButtonToDrag == InMouseEvent.GetEffectingButton()) {
        IsMouseDown = true;
        OnClickScreenPos = InMouseEvent.GetScreenSpacePosition();
    }
    auto Size = TheOverlay->GetCachedGeometry().GetLocalSize();
    FVector2D Center = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) - (Size / 2.f);
    
    BigMapMouseLocation =FVector(((Center.Y * Zoom * (MapLengthCenter / Size.X)*2.f) - Loc.X) * -1.f, ((Center.X * Zoom * (MapLengthCenter / Size.Y) * 2.f) + Loc.Y), Loc.Z);
    
    //PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("LastKnownOwnerLocation %f %f %f"),LastKnownOwnerLocation.X, LastKnownOwnerLocation.Y, LastKnownOwnerLocation.Z));
    //PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("MapLengthCenter %f"), MapLengthCenter));
   

    if (IsBigMap) 
        EValidateMinimapClick(BigMapMouseLocation, InMouseEvent.GetEffectingButton());
    else
    {
        CV(ABRCharacter, Char, OwnerW);
        if (Char)
            Char->OverviewMapClicked(BigMapMouseLocation, InMouseEvent.GetEffectingButton());
    }
    return FReply::Handled();
}

void UMinimapComponent::MinimapZoomIn()
{
    float CurrentZoom = Zoom;
    Zoom -=(ZoomSteps <= 1.f && ZoomSteps >= 0.f) ? ZoomSteps : 0.25f;
    if (Zoom <= 0.f) {
        Zoom = CurrentZoom;
        return;
    }

    if (Zoom != CurrentZoom) 
        MPDynMat->SetScalarParameterValue(TEXT("Zoom"), Zoom);
   
}

void UMinimapComponent::MinimapZoomOut()
{
    float CurrentZoom = Zoom;
    Zoom += (ZoomSteps <= 1.0 && ZoomSteps >= 0.f) ? ZoomSteps : 0.25f;
    if (Zoom > MaxZoom) {
        Zoom = CurrentZoom;
        return;
    }
    
    if (Zoom != CurrentZoom) 
        MPDynMat->SetScalarParameterValue(TEXT("Zoom"), Zoom);
}


void UMinimapComponent::MDrawCircle(FPaintContext _Context, FVector2D _Coord, float _Size, FLinearColor _Tint) const
{
    for (int32 i = 0; i <= 360; i++) {
        float nexti = (i + 1) % 360;
        FVector2D PosA=_Coord+ FVector2D(UKismetMathLibrary::DegCos(i)*_Size, UKismetMathLibrary::DegSin(i)*_Size);
       
        FVector2D PosB = _Coord + FVector2D(UKismetMathLibrary::DegCos(nexti) * _Size, UKismetMathLibrary::DegSin(nexti) * _Size);
        UWidgetBlueprintLibrary::DrawLine(_Context, PosA, PosB,_Tint);
    }
}

UPOIW* UMinimapComponent::AddPointInterest(FSlateColor _Color, AActor* _POIActor, bool _IsPersistent, bool _FollowActorRotation, UTexture2D* _Texture, FVector2D _TextureSize,
    UTexture2D* _OutOfMapTexture, bool _KeepImageStraight, bool _OnlyOffMap, bool _IsLocation, FVector _Location, bool _ShowHeightIndicator, FText _ToolTip)
{
    if (!IsValid(POIWClass) || !IsValid(TheOverlay)) return nullptr;

    
    UPOIW* POIW = CreateWidget<UPOIW>(GetWorld(), POIWClass);
    
    if (!IsValid(POIW)) return nullptr;
    
    POIW->PlayerOwner = OwnerW;
    POIW->ColorX = _Color;
    POIW->ActorRef = _POIActor;
    POIW->IsMinimapPersistent = _IsPersistent;
    POIW->FollowActorRotation = _FollowActorRotation;
    POIW->Texture = _Texture;
    POIW->TextureSize = _TextureSize;
    POIW->OutOfMapTexture = _OutOfMapTexture;
    POIW->RemoveIfActorDestroyed = true;
    POIW->KeepImageStraight = _KeepImageStraight;
    POIW->OnlyShowOnOutOfMinimap = _OnlyOffMap;
    POIW->IsLocation = _IsLocation;
    POIW->Location = _Location;
    POIW->ShowHeightIndicator = _ShowHeightIndicator;
    POIW->ToolTip = _ToolTip;
    
    POIW->ChangeHeightIndicSettings(HIArrowUp, HIArrowDown, HIColor1, HIColor2, HISize, HIAccepDiff);
    
    if(UOverlaySlot* OverlaySlot = TheOverlay->AddChildToOverlay(POIW))
    {
        OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
        OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
        POIW->MyOverlaySlot = OverlaySlot;
    }
    
    POIW->SetVisibility(ESlateVisibility::Hidden);
    if (!POIWS.Contains(POIW))
        POIWS.Add(POIW);
      
    return POIW;
}

void UMinimapComponent::SquareCoorCircle(float _X, float _Y, float& XCircle, float& YCircle)
{
    float Xterm = 1.0f - 0.5f * _Y * _Y;
    XCircle = FMath::IsNearlyZero(_X) ? 0.f : _X * FMath::Sqrt(FMath::Max(0.0f, Xterm));

    float Yterm = 1.0f - 0.5f * _X * _X;
    YCircle = FMath::IsNearlyZero(_Y) ? 0.f : _Y * FMath::Sqrt(FMath::Max(0.0f, Yterm));
}

