# legendary_octo_waddle
SDL2 potato game

Dependencies:
* SDL2
* SDL2_image
* SDL2_ttf
  
Notes:
1. SDL2_ttf isn't used currently, but it probably will be when I start working on UI\
2. There are some stupid dependencies between the files. For example, Game.hpp depends on Character.hpp for Timer.hpp and Character.hpp is included with Player.hpp and Enemy.hpp so if you don't include Player.hpp and Enemy.hpp then 1) ??? and 2) you'll have to include Timer.hpp, assuming you're still using the timers I made.

You have to install the libraries yourself because I don't know how to make an installer. Also I made this program on a 2012 Macbook Air (good lord please save my dying soul) and I was told that installing/including the libraries is a bit different on MacOs??? I don't really know.

It feels like such a mess right now. I was in the process of updating the animations and making attacks that flowed better but I didn't feel like I was making any actual progress on the game itself. Making animations takes up a lot of time and effort that I think could be put to better use implementing game mechanics. Eventually I will get around to it, just like how I will eventually get around to creating a story. SoonTM.

Kevin Cheng
