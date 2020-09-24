#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

// 封装SDL2 TTF_font
class Font {
	private:
		TTF_Font *font = nullptr;
		std::string font_name = "simhei.ttf" ;
		uint8_t font_size = 10;

	public:
		// 初始化字体
		bool set_font( const std::string &font_name, uint8_t size);
		Font( const std::string &font_name_, uint8_t size_ )
		:font_name( font_name_ ), font_size( size_ ) 
		{
			set_font( font_name_, size_ );
		}
		
		~Font() {
			// 关闭字体
			TTF_CloseFont( font );
		}
		// 返回SDL2中的字体
		// TTF_Font *const font sdl_font();

		const uint16_t get_font_size() {
			return font_size;
		}
		const std::string &get_font_name() {
			return font_name;
		}
	    TTF_Font *const get_font() {
			return font;
		}
		
};


// surface渲染为texture
SDL_Texture *SurfaceToTexture(const SDL_Renderer *renderer, SDL_Surface *surface);

// 字符串渲染为textur
SDL_Texture *string_to_texture(const SDL_Renderer *renderer,  const std::string &str, TTF_Font *font, SDL_Color color, SDL_Rect &rect );