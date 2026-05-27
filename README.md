# Pigeon Evolution

Pigeon Evolution is a small terminal game inspired by games like Cow Evolution.
You start with simple pigeons from crates, merge matching pigeons together, and
slowly discover stranger and bigger pigeon forms.

The final goal is to discover the largest pigeon in the evolution chain. The
game starts with a Baby Pigeon and keeps going through many forms until the final
one, the Pigeostrich.

## Game Idea

The main idea is simple:

1. Open crates to get pigeons.
2. Merge two pigeons of the same type.
3. The merged pigeon evolves into a bigger one.
4. Bigger pigeons create better poop, which gives you more coins.
5. Use coins in the shop to buy pigeons or berries.
6. Keep discovering new pigeons until you reach the final evolution.

If you have played Cow Evolution, the flow should feel familiar: crates give you
basic animals, two matching animals merge into a better one, and the collection
keeps growing from there.

## Game Loop

The game runs in a loop in the terminal.

At the start, the game shows a menu. After every command, the board updates:

- pigeons can create poop over time
- poop is collected automatically
- coins are added to your balance
- berry effects are checked
- new discovered pigeons are added to the encyclopedia

The game does not end when you make a wrong command. Invalid commands are handled
with exceptions, the message is printed, and the menu continues. To stop playing,
type:

```text
exit
```

## Commands

When the menu appears, you can use these commands:

```text
1 - Open crate
2 - Merge pigeons
3 - Show encyclopedia
4 - Open shop
5 - Print board and coins
6 - Feed berry
exit - Quit game
```

### 1 - Open crate

Opens a crate and adds a new Baby Pigeon to your board.

### 2 - Merge pigeons

Shows your current board, then asks for two pigeon indexes.

If the two pigeons are the same type and can still evolve, they merge into the
next pigeon form.

Example:

```text
2
0 1
```

This tries to merge the pigeons at index `0` and index `1`.

### 3 - Show encyclopedia

Shows the pigeons you have discovered so far.

Each entry shows:

- pigeon name
- coins per second
- description

### 4 - Open shop

Opens the shop menu. The shop has two categories:

```text
1 - Pigeons
2 - Berries
```

Pigeons become available in the shop after you progress far enough. Berries can
be bought with coins and used for special effects.

If you try to buy something without enough coins, the game shows a message and
continues.

### 5 - Print board and coins

Shows all pigeons currently on your board and your current number of coins.

If a pigeon has an active berry effect, the board also shows how much time is
left for that effect.

### 6 - Feed berry

Shows your board and berry inventory, then asks for:

```text
berry_type pigeon_index
```

Example:

```text
6
1 0
```

This feeds berry type `1` to the pigeon at index `0`.

Only one berry effect can be active at a time. If you try to feed another berry
while one is already active, the game blocks it and continues.

## Berry Types

Berries give temporary special effects to pigeons.

### Red Berry

The Red Berry makes the pigeon poop faster for a short time.

This helps you collect coins more quickly.

### Yellow Berry

The Yellow Berry gives you bonus coins when the pigeon is merged.

If a pigeon with an active Yellow Berry effect is part of a successful merge, you
receive extra coins based on that pigeon.

### Purple Berry

The Purple Berry helps you get a bigger pigeon.

After the Purple Berry effect expires, the pigeon can evolve into its next form.
This is useful when you want to progress faster through the evolution chain.

## Coins

Coins come from poop. Pigeons create poop over time, and the game automatically
collects it during the update loop.

Better pigeons can create better poop, and better poop is worth more coins.

Coins are used to buy:

- pigeons from the shop
- berries from the shop

## Encyclopedia

The encyclopedia keeps track of pigeons you have discovered.

When a new pigeon appears on the board, it is added to the encyclopedia. This
helps you see your progress through the evolution chain.

## Final Goal

The main goal is to discover the largest pigeon.

You start with small pigeons from crates and keep merging them into bigger and
weirder forms. The final discovered pigeon is the end of the current evolution
chain.

In this version, the largest pigeon is the Pigeostrich.

## Building the Project

The project uses CMake.

Example build commands:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Then run the generated executable.

If you are using CLion, you can open the project and run the `oop` target.
