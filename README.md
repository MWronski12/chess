# chess

Chess engine with console gui written in C++. It implements generation of legal moves and all other rules of a chess game. 

Window gui and MiniMax algorithm for choosing the best move are currently under development.

### To run the application:

- Make sure You have cmake, make and g++ installed
- `git clone https://github.com/MWronski12/chess`
- `cd chess`
- `./install-packages.sh`
- `cmake -B build .`
- `cd build`
- `make chess`
- `./chess`

Game tutorial:
1. Make move
2. press space and wait for computer to make move
3. repeat 1. and 2.

UML class diagram:

![Chess application UML class diagram](https://user-images.githubusercontent.com/66621445/229904436-fbe6475a-2f64-4f3a-afb5-91c9fce12d30.png)
