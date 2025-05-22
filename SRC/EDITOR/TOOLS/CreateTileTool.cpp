#include "CreateTileTool.h"

CreateTileTool::CreateTileTool()
	:
	TileTool()
{
}

CreateTileTool::~CreateTileTool()
{
}

void CreateTileTool::Create()
{
}

void CreateTileTool::Draw()
{
	if (!SpriteValid() || !IsActivated() || OutOfBounds() || !m_pCamera)
	{
		return;
	}

	auto& pShader = MAIN_REGISTRY().GetAssetManager().GetShader("basic");
	assert(pShader.getID() != 0 && "Basic shader doesn't exist!");

	pShader.use();
	auto camMat = m_pCamera->GetCameraMatrix();
	pShader.setMat4("uProjection", camMat);

	DrawMouseSprite();

	pShader.unuse();

}
