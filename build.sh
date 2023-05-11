mkdir out
rm out/JChessV2
g++ JChessV2.cpp \
  scenes/*.cpp \
  scenes/Game/*.cpp scenes/Game/UI/*.cpp scenes/Game/Player/*.cpp \
  scenes/StartMenu/*.cpp \
  chessboard/*.cpp chessboard/move_gen/*.cpp \
-lncursesw -lglfw -lX11 -lGL -g -o out/JChessV2
./out/JChessV2
