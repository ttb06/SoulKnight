# SpiritWarrior
My Advanced Programming project - Soul Knight clone (with Spirit == Soul, Warrior == Knight) built with SDL2 and C++.

## Window Installation (My mentor said that I don't have to write Linux Installation)

- First, install [`Mingw-w64`](https://sourceforge.net/projects/mingw-w64/files/), my version is `GCC-7.3.0 i686-posix-sjlj` (32-bit).
- Second, download [`SDL2`](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/mingw/index.php), [`SDL2_mixer`](https://github.com/libsdl-org/SDL_mixer/releases), [`SDL2_image`](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.6.3), [`SDL2_ttf`](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.20.2). Put all this stuff to a folder named `src`, or just download [my installed library](https://drive.google.com/file/d/1gbsMh4Yuyc_eN8NP2nNgdsBJBu8GiNmS/view?usp=sharing).
- Third, install [`GNU MAKE`](https://stackoverflow.com/a/57042516/21271990).
- Fourth, clone this repo. Cut folder from step 2 and paste it into this folder. Open folder `src/bin` and copy all `.dll` files to this repo folder. 
- Open `cmd` and type `make`. It will compile into a program named `main.exe`.
- Now enjoy the game!!! 

## Linux Installation (tested on Ubuntu)
Clone this repo. In the folder of repo, firstly, delete file `Makefile` and rename `Makefile (Linux)` to `Makefile`. Then, open Terminal and type respectively:
- `sudo apt install make`
- `sudo apt install g++`
- `sudo apt-get install libsdl2-dev`
- `sudo apt-get install libsdl2-image-dev`
- `sudo apt-get install libsdl2-mixer-dev`
- `sudo apt-get install libsdl2-ttf-dev`
- `make`
 - Now enjoy the game!!!

## Gameplay
- You have to control player to clear enermies in each room. Enermies can shoot projectiles, and have collider damage.
- You can control player by `right-click` to go to the position or just use `WASD`/`←↑→↓`.
- You can slash projectiles and enermies by pressing `SPACE`.

## About Game
This Game is built with ECS system which I learned from [`here`](https://www.youtube.com/@CarlBirch). We can add Component to Entity (each Entity is a vector of Component), and use a Manager to manage all Entities. Here are some type of Entity:

### Player
* Can move by clicking right mouse (using BFS) or by WASD or arrow.
* Has armor and health, armor can restore after a short time.

### Weapon
* Make Damage to Enermies.

### Enermy/Enermies
* Can shoot projectiles.
  
### Projectiles
* Deal damage upon hitting player.

### Tile
* Of map, which can be animated ( so nice :> )

## Knowledge used
- OOP (for Component)
- BFS (to move by right-click mouse)
- Using .h and .cpp logically.
- Math (and VERY VERY MATH :< )

##Bug
- When compile file in Ubuntu, or use winlibs to compile file in Window might cause crash.

## Self-Assessment
9 - 9.5
