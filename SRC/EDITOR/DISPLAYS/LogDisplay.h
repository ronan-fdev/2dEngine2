#pragma once

#include <ranges>

#include "ENGINE/LOGGER/log.h"
#include "../../ImGui/imgui.h"

#include "IDisplay.h"

class LogDisplay : public IDisplay
{
private:
	ImGuiTextBuffer m_TextBuffer;
	ImVector<int> m_TextOffsets;
	bool m_bAutoScroll;

private:
	void GetLogs();

public:
	LogDisplay();
	~LogDisplay() = default;

	void Clear();
	virtual void Draw() override;
};

