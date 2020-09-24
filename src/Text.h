#pragma once

#include "Texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class TextT {
    public:
        SDL_Color color = {255, 255, 255, 255};
        SDL_Texture* texture = nullptr;
        SDL_Rect rect = {0, 0, 200, 200};
        TTF_Font *font = nullptr;
        std::string content;
        SDL_Renderer* renderer = nullptr;

        // constructor of Text's Texture 
        TextT(SDL_Renderer *renderer_, const std::string &content_, const SDL_Color &color_, const SDL_Rect rect_, TTF_Font *font_)
        :renderer( renderer_ ), content( content_ ), color( color_ ), rect( rect_ ), font( font_ ) 
        { update_texture(); };
        TextT(const TextT &t)
        :renderer( t.renderer ), content( t.content ), color( t.color ), rect( t.rect ), font( t.font ) 
        { update_texture(); };
        ~TextT() {
            SDL_DestroyTexture(texture);
        }

        void setContent( const std::string &content_ );

        std::string getContent();

        void setPos( const uint16_t &x, const uint16_t &y );

    private:
        void update_texture();
};
