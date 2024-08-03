Forked from https://github.com/eliasdaler/cmake-fetchcontent-tutorial-code.

# Introduction

'Islands' project developed with `SFML`, `imgui` and `imgui-sfml`.

Detection of islands and unique number of islands in 2D-grid worlds.

[islands-screenshot]: images/overview.png

[![Islands screenshot][islands-screenshot]]()

# Features

- Set program title.
- FPS shown in UI.
- Set background color of program.
- Create custom and random worlds.
- Edit worlds by adding/removing rows and colums.
- Change tile types with primary and secondary mouse clicks.
- Clear worlds.
- Save worlds to .txt files and load them back.
- Automatic live detection of islands and unique number of islands.

# TODO

- Maybe typedef for some complex types.
- Islands summary with vertical and horizontal scroll bars.
- FPS is "too quick" -> run it at fixed 60 fps.
- Long press on tile for neighbours semi-transparent overlay (green for land, red for water).
- Max. recommended level of recursion?
- Too small on some displays (i.e. laptop display).
- Ability to create SFML window in a specific screen.
- Add unit tests using /src/worlds.
- Create a 'mouse manager' with a design pattern.

# Building and running

```sh
cd islands
cmake -S . -B build
cmake --build build
./build/src/islands
```

Alternatively, you can build and run 'Islands' using Visual Studio Code with the launch configuration provided.
