#pragma once

#include <algorithm>
#include "Batcher.h"
#include "../ESSENTIALS/BatchTypes.h"
#include "../ESSENTIALS/Primitives.h"

class LineBatchRenderer : public Batcher<LineBatch,LineGlyph>
{
private:
	virtual void GenerateBatches() override;
	void Initialize();

public:
    LineBatchRenderer();
    ~LineBatchRenderer() = default;

    virtual void End() override;
    virtual void Render() override;

    void AddLine(const Line& line);
};

