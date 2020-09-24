#include "Text.h"

void TextT::setContent( const std::string &content_ ) {
    content = content_;
    update_texture();
}
 std::string TextT::getContent() {
    return content;
}
void TextT::setPos(const uint16_t &x, const uint16_t &y) {
    rect.x = x;
    rect.y = y; 
    // update_texture();
}

void TextT::update_texture() {
    if( texture != nullptr ) {
        SDL_DestroyTexture( texture );
    }
    texture = string_to_texture(renderer, content, font, color, rect);
}
