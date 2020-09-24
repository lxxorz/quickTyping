# quickTyping

## introduce
自定义、简洁打字练习软件
使用`c++11`采用`SDL2.0`编写

## compile 

需要自定义编译的可以添加一下编译参数

```bash
g++ *.cpp -o main_v2 -lmingw32 -lSDL2main -lSDL2  -lSDL2_ttf -lSDL2_image
```
或者直接下载二进制文件

## Usage

词库请放入本地目录`words.txt`中， 会**自动随机**选择单词进行联系，默认CET-4 词库
  
* words.txt
![截图](https://cdn.jsdelivr.net/gh/lxxorz/pics/word.png)


* 使用截图
![截图](https://cdn.jsdelivr.net/gh/lxxorz/pics/shoot.png)


## problem

* 如果提示**缺少xxxx.dll**则可能缺少`SDL2.0`lib需去官网下载[https://www.libsdl.org/](https://www.libsdl.org/)
* 缺少ttf lib 请去[https://www.libsdl.org/projects/SDL_ttf/](https://www.libsdl.org/projects/SDL_ttf/)下载
* 缺少image lib 请去[https://www.libsdl.org/projects/SDL_image/](https://www.libsdl.org/projects/SDL_image/)
