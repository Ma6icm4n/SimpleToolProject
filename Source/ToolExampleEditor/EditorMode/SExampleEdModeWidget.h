#pragma once
#include "Framework/Application/SlateApplication.h"

class SExampleEdModeWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SExampleEdModeWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	class FExampleEdMode* GetEdMode() const;

	FReply OnAddPoint();
	bool CanAddPoint() const;
	FReply OnRemovePoint();
	bool CanRemovePoint() const;
};