#pragma once
#include "EditorModes.h"

class AExampleTargetPoint;

struct HExamplePointProxy : public HHitProxy
{
    DECLARE_HIT_PROXY();

    HExamplePointProxy(UObject* InRefObject, int32 InIndex)
        : HHitProxy(HPP_UI), RefObject(InRefObject), Index(InIndex)
    {}

    UObject* RefObject;
    int32 Index;
};

class FExampleEdMode : public FEdMode
{
public :
	const static FEditorModeID EM_Example;
    FExampleEdMode();
    ~FExampleEdMode();

	virtual void Enter() override;
	virtual void Exit() override;

    void AddPoint();
    bool CanAddPoint() const;
    void RemovePoint();
    bool CanRemovePoint() const;
    bool HasValidSelection() const;
    void SelectPoint(AExampleTargetPoint* actor, int32 index);

    TWeakObjectPtr<AExampleTargetPoint> currentSelectedTarget;
    int32 currentSelectedIndex = -1;

    virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
    virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;

    virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;
    virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
    virtual bool ShowModeWidgets() const override;
    virtual bool ShouldDrawWidget() const override;
    virtual bool UsesTransformWidget() const override;
    virtual FVector GetWidgetLocation() const override;

    TSharedPtr<FUICommandList> ExampleEdModeActions;
    void MapCommands();
    TSharedPtr<SWidget> GenerateContextMenu(FEditorViewportClient* InViewportClient) const;
};