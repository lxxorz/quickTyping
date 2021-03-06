#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Text.h"
#include <ctime>
#include <cstdlib>
#include <algorithm>

SDL_Window *window;
SDL_Renderer *renderer;

bool init_everything(); 
bool init_window();
bool init_render();
bool init_sdl();
bool init_words();
void read_next();

void run_game();
void render_input_word();
void render_words( SDL_Renderer *renderer, const std::vector<TextT> &text );  
void render_word( SDL_Renderer *render, const TextT & text);
void render_static_resource();
void render ();
void reset();
SDL_Color random_color(uint8_t range = 150);

void set_frame(uint32_t fram);
void word_move();
bool handle_keydown( SDL_Event *event);
void updata_game_data();

// read_string ==> render to screen ==> listen keydown ==> render character to screen ==> delete character by backspace or clean by space
const int size_x = 800;
const int size_y = 500;
const int pos_x = 50;
const int pos_y = 50;
const int speed_limit = 2;
const uint8_t frequency = 60;

uint32_t count_back_space = 0;
uint32_t count_success = 0;
uint32_t count_error = 0;
uint64_t count_speed = 0;
uint32_t count_char = 0;
uint16_t speed = 1;
uint64_t count_time = 0;
uint64_t fps = 0;
uint64_t frame = 1;


// 字体颜色
SDL_Color color = { 0, 80, 155, 255 };  
std::vector<TextT> words;
std::vector<std::string> words_str;
std::string input_word; 

void err() {
	std::cout << "error : " << std::endl;
}
int main(int argc, char *argv[]) {
	init_everything();
	run_game();
	
	SDL_Quit();
    return 0;
}

bool init_everything() {
    // initialize window  
	if( !init_sdl() ) {
		return false;
	}
	if( !init_window() ) {
		return false;
	}

	if( !init_render() ) {
		return false;
	}

	if( !init_words() ) {
		return false;
	}

	return true;
}

bool init_sdl() {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}
bool init_window() {
    window = SDL_CreateWindow(
        "quick typing", 
        pos_x, pos_y,
        size_x, size_y,
        SDL_WINDOW_SHOWN
    );
	if( window == nullptr ) {
		std::cout << "init_windwo failed : " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

bool init_render() {
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	if ( renderer == nullptr )
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}

	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, size_x, size_y );

	// Set color of renderer to red
	SDL_SetRenderDrawColor( renderer, 0,  0, 0, 255 );

	return true;
}


bool init_words() {
	std::srand((unsigned int)time(nullptr) + (SDL_GetPerformanceCounter() % 100000));
	const std::string file_name = "words.txt";
	std::ifstream input_file(file_name);	
	std::string str;
	while( input_file >> str) {
		words_str.push_back(str);
	}
	input_file.close();
	std::random_shuffle( words_str.begin(), words_str.end() );
	read_next();
	return true;
}
void read_next() {
	words.clear();
	//一次取出单词的数量
	const int num = 14;
	// font color
	SDL_Color color = {255, 155, 0, 255};
	// font position
	SDL_Rect rect = {0, 0, 0, 0};

	Font font = Font("simhei.ttf", 24);
	uint16_t w_interval = 40;
	uint16_t h_interval = 20;
	int x = 0;	
	int y = 0;	

	auto size = words_str.size()-1;
	//从单词列表中取出数据然后转换成texture数据
	//当单词列表为空或者
	for( auto i = size; (!words_str.empty()) && (i > size-num) ; --i ) {
		TextT t = TextT(renderer, words_str[i], color, rect, font.get_font());
		x = x-t.rect.w - (40- (abs(std::rand()) % 30 ));
		y = ( std::rand() % (size_y - t.rect.h) );
		t.rect.x = x;
		t.rect.y = y;
		t.color = random_color();
		words.push_back(t);
		words_str.pop_back(); 
	}
}

void set_frame(uint32_t fram) {
	SDL_Delay(1000/fram);
}

void run_game() {
	
	bool loop = true;
	while( loop ) {
		SDL_Event event;
		while( SDL_PollEvent( &event ) ) {
			switch( event.type ) {
				case SDL_QUIT:
					loop = false;
					break;
				case SDL_KEYDOWN:
					loop = handle_keydown( &event ); 
					// 每次输入时改便 input_word的颜色
					color = random_color( 200 );
					break;
			}
		}
			
		updata_game_data();
		render();
		set_frame( frequency );
	}
}


//渲染顺序
void render() {
	render_static_resource();
	render_words( renderer, words );
	render_input_word();
	SDL_RenderPresent( renderer );
	SDL_RenderClear( renderer );
}


void render_word( SDL_Renderer *renderer, const TextT & text) {
	SDL_RenderCopy( renderer, text.texture, nullptr, &text.rect );
}

void render_words( SDL_Renderer *renderer, const std::vector<TextT> &texts ) {
	for(auto &text: texts ) {
		render_word( renderer, text );
	}
}


// 单词每次移动
void word_move() {

	// 每4帧渲染一次
	speed = speed % speed_limit;
	if(speed == 0) speed++;
	// 当单词到达右边界是，从左边出现
	for( auto &w: words) {
		w.rect.x += speed;	
		if(w.rect.x >= ( size_x )) {
			w.rect.x = -w.rect.w;
		}
	}
}

// 处理keydown事件
bool handle_keydown( SDL_Event *event ) {
	// 控制游戏加速和减速,以及退出，重置游戏
	if( event->key.keysym.mod & KMOD_LCTRL) {
		if( event->key.keysym.sym == SDLK_q ){
			return false;
		}else if( event->key.keysym.sym == SDLK_r ) {
			reset();
			input_word.clear();
			return true;
		}else if( event->key.keysym.sym == SDLK_EQUALS ) {
			speed++;
			speed %= speed_limit;
			if( !speed ) ++speed;
			std::cout <<"move speed: " << speed << std::endl;
		}
	}

	// 检查单词输入 
	uint32_t sym = event->key.keysym.sym;
	if( (sym <= 'z' && sym >= 'a' )  ) {
		if( event->key.keysym.mod & KMOD_LSHIFT ){
			sym = sym - 'a' + 'A';
		}
		input_word.push_back(sym);
		++count_char;	
	}else if( sym == '-' ) {
		input_word.push_back((char)sym);
	}else if( sym == SDLK_SPACE ){
		// 当键入空格时，清楚之前输入的单词
		input_word.clear();
		++count_error;
	}else if(sym == SDLK_BACKSPACE && !input_word.empty()){
		// 当键入退格时，删除的单词最后一个字符
		input_word.pop_back();
		++count_back_space;
	}
	else if( sym == SDLK_RETURN) {
	// 当输入Enter时检测当前输入单词是否在屏幕上
		auto size = words.size();
		for( auto iter = words.begin(); iter != words.end(); ++iter) {
			if( iter->content == input_word ) {
				iter = words.erase( iter );
				count_success++;
				break;
			}
		}
		
		if( !input_word.empty() && size == words.size()) {
			++count_error;
		}
		input_word.clear();
	}
	if( words.size() <= 1 ) {
		read_next();
	}

	// 计算用户打字的速度，包括:
	// wpm(每分钟单词数), kpm(每分钟字符数)
	// 正确率
	uint64_t time_clip = count_time / 60000;
	if(time_clip == 0) time_clip = 1;	
	count_speed = count_success /  time_clip; 
	return true;
}


void render_input_word() {
	// 左下方显示输入字符
	SDL_Rect rect = { 50, size_y - 50, 0, 0 };
	Font font = Font( "simhei.ttf", 26 );

	TextT text = TextT( renderer, input_word, color, rect, font.get_font() );
	render_word( renderer, text );
}


void render_default_word(std::string content, SDL_Rect *rect ) {
	SDL_Color color = { 0, 155, 255 , 255 };  
	Font font = Font( "simhei.ttf", 30 );
	TextT text = TextT( renderer, content, color, *rect, font.get_font() );
	if(rect->x == -1) {
		text.rect.x = size_x - text.rect.w -  100;
		text.rect.y = size_y - text.rect.h -  100;
		rect->x = text.rect.x; 
		rect->y = text.rect.y; 
	}else {
		text.rect.x = rect->x; 
		text.rect.y = rect->y; 
	}
	render_word( renderer, text );
}

void render_static_resource() {

	const uint8_t interval = 30; 
	SDL_Rect rect = { -1, -1, 0, 0 };

	std::string count_time_str = "time: " + std::to_string( count_time / 1000 ) + "S";
	render_default_word(count_time_str, &rect ); 
	rect.y -= interval;

	std::string count_back_space_str = "backspace: " + std::to_string( count_back_space ) ;
	render_default_word(count_back_space_str, &rect ); 
	rect.y -= interval;

	std::string count_error_str = "error: " + std::to_string( count_error ) + "";
	render_default_word( count_error_str, &rect ); 
	rect.y -= interval;

	std::string count_success_str = "score: " + std::to_string( count_success ) + "";
	render_default_word( count_success_str, &rect ); 
	rect.y -= interval;

	// std::string count_speed_str = "speed: " + std::to_string( count_speed ) + " wpm";
	// render_default_word( count_speed_str, &rect ); 

	rect.y -= interval;
	std::string fps_str = "FPS: " + std::to_string( fps );
	render_default_word( fps_str, &rect ); 
	rect.y -= interval;

	// std::string count_char_str = "KPM: " + std::to_string( count_char / (count_time / 60000 + 1));
	// render_default_word( count_char_str, &rect ); 
	rect.y -= interval;
}


void reset() {
	words.clear();
	input_word.clear();
	count_back_space = 0;
	count_error = 0;
	count_speed = 0;
	count_success = 0;
	count_time = 0;
	frame = 1;
	fps = 0;
	speed = 1;
}


/* Game Data Update */
void updata_game_data() {
	// 更新游戏时间
	count_time =  SDL_GetTicks(); 

	// 更新速度控制
	if((frame > 100) && (frame % 2000 == 0) &&( speed < speed_limit)) {
		++speed;
		if(!speed) ++speed;
	}

    // 已经渲染的帧数量
	frame++;

	// 当前FPS(frame per second)
	fps = frame / (count_time / 1000 + 1) ;

	// 控制单词移动
	word_move();
}


/* ===========UI============*/
SDL_Color random_color(uint8_t range) {
	range %= 255;
	return { 
		(uint8_t)(std::rand() % range + 255 - range),
		(uint8_t)(std::rand() % range + 255 - range),
		(uint8_t)(std::rand() % range + 255 - range),
		255
	};  
}
