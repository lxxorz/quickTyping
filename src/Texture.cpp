#include "Texture.h"

bool Font::set_font( const std::string &font_name, uint8_t font_size)
{

	if( font != nullptr ) {
		TTF_CloseFont( font );
	}
	// SDL2_TTF needs to be initialized just like SDL2
	if ( TTF_Init() == -1 )
	{
		std::cout << " Failed to initialize TTF : " << SDL_GetError() << std::endl;
		return false;
	}

	// Load our fonts, with a huge size
	font = TTF_OpenFont( font_name.c_str(), font_size);
	
	// Error check
	if ( font == nullptr )
	{
		std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}


// 字符串渲染为texture
SDL_Texture *string_to_texture(const SDL_Renderer *renderer, const std::string &str, TTF_Font *font, SDL_Color color, SDL_Rect &rect ) {
	SDL_Surface* solid = TTF_RenderUTF8_Solid( font, str.c_str(), color );

	SDL_Texture *solidTexture = SurfaceToTexture( renderer, solid );

	SDL_QueryTexture( solidTexture, NULL, NULL, &(rect.w), &(rect.h) );
	return solidTexture;
}

SDL_Texture *SurfaceToTexture(const SDL_Renderer *renderer, SDL_Surface *surface) {
	SDL_Texture* text;

	text = SDL_CreateTextureFromSurface( const_cast<SDL_Renderer*>(renderer), surface );

	SDL_FreeSurface( surface );

	return text;
}
