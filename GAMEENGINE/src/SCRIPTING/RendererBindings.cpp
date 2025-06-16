#include "pch.h"

#include "SCRIPTING/RendererBindings.h"

#include "ECS/MainRegistry.h"
#include "LOGGER/log.h"
#include "RENDERER/CORE/Camera2D.h"
#include "RENDERER/CORE/Renderer.h"
#include "RENDERER/ESSENTIALS/Primitives.h"
#include "RESOURCES/AssetManager.h"

void RendererBinder::CreateRenderingBind( sol::state& lua, Registry& registry )
{
	// Get the Asset Manager
	auto& mainRegistry = MAIN_REGISTRY();
	auto& assetManager = mainRegistry.GetAssetManager();

	// Primitives Bind
	lua.new_usertype<Line>(
		"Line",
		sol::call_constructor,
		sol::factories(
			[]( const glm::vec2& p1, const glm::vec2& p2, const Color& color ) {
				return Line{ .p1 = p1, .p2 = p2, .color = color };
			} ),
		"p1", &Line::p1,
		"p2", &Line::p2,
		"color", &Line::color );

	lua.new_usertype<Rect>(
		"Rect",
		sol::call_constructor,
		sol::factories(
			[]( const glm::vec2& position, float width, float height, const Color& color ) {
				return Rect{ .position = position, .width = width, .height = height, .color = color };
			} ),
		"position", &Rect::position,
		"width", &Rect::width,
		"height", &Rect::height,
		"color", &Rect::color );

	lua.new_usertype<Circle>(
		"Circle",
		sol::call_constructor,
		sol::factories(
			[]( const glm::vec2& position, float lineThickness, float radius, const Color& color ) {
				return Circle{ .position = position, .lineThickness = lineThickness, .radius = radius, .color = color };
			} ),
		"position", &Circle::position,
		"lineThickness", &Circle::lineThickness,
		"radius", &Circle::radius,
		"color", &Circle::color );

	// Bind the renderer
	auto& renderer = mainRegistry.GetContext<std::shared_ptr<Renderer>>();
	if ( !renderer )
	{
		LOG_ERROR( "Failed to bind the Renderer to Lua! Not in the registry context!" );
		return;
	}

	lua.set_function(
		"DrawRect", sol::overload(
						[&]( const Rect& rect ) {
							renderer->DrawRect( rect );
						},
						[&]( const glm::vec2& position, float width, float height, const Color& color ) {
							renderer->DrawRect( position, width, height, color );
						} ) );

	lua.set_function(
		"DrawLine", sol::overload(
						[&]( const Line& line ) {
							renderer->DrawLine( line );
						},
						[&]( const glm::vec2& p1, const glm::vec2& p2, const Color& color ) {
							renderer->DrawLine( p1, p2, color );
						} ) );

	lua.set_function(
		"DrawFilledRect", [&]( const Rect& rect ) {
			renderer->DrawFilledRect( rect );
		} );

	lua.set_function(
		"DrawCircle", sol::overload(
						  [&]( const Circle& circle ) {
							  renderer->DrawCircle( circle );
						  },
						  [&]( const glm::vec2& pos, float lineThickness, float radius, const Color& color ) {
							  renderer->DrawCircle( pos, radius, color, lineThickness );
						  } ) );

	auto& camera = registry.GetContext<std::shared_ptr<Camera2D>>();
	if ( !camera )
	{
		LOG_ERROR( "Failed to bind the camera to lua - Failed to get the camera from the registry context!" );
		return;
	}

	// Bind The Camera!
	lua.new_usertype<Camera2D>(
		"Camera",
		sol::no_constructor,
		"get", [&] { return *camera; },
		"position", [&] { return camera->GetPosition(); },
		"set_position", [&]( const glm::vec2 newPosition ) { camera->SetPosition( newPosition ); },
		"scale", [&] { return camera->GetScale(); },
		"set_scale", [&]( float scale ) { camera->SetScale( scale ); },
		"get_world_coords", [&]( const glm::vec2& screenCoords ) { return camera->ScreenCoordsToWorld( screenCoords ); },
		"get_screen_coords", [&]( const glm::vec2& worldCoords ) { return camera->WorldCoordsToScreen( worldCoords ); },
		"width", [&] { return camera->GetWidth(); },
		"height", [&] { return camera->GetHeight(); } );

	// Text RendererBinding:
	lua.new_usertype<Text>(
		"Text",
		sol::call_constructor,
		sol::factories(
			[&]( const glm::vec2& position, const std::string& textStr, const std::string& fontName, float wrap,
				const Color& color ) {
				auto pFont = assetManager.GetFont( fontName );
				if ( !pFont )
				{
					LOG_ERROR( "Failed to get font [{}] -- Does not exist in asset manager!", fontName );
					return Text{};
				}

				return Text{
					.position = position,
					.textStr = textStr,
					.wrap = wrap,
					.pFont = pFont,
					.color = color };
			} ),
		"position", &Text::position,
		"textStr", &Text::textStr,
		"wrap", &Text::wrap,
		"color", &Text::color );

	lua.set_function(
		"DrawText", [&]( const Text& text ) {
			renderer->DrawText2D( text );
		} );
}
