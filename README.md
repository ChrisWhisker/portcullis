![portcullis-150](https://github.com/ChrisWhisker/portcullis/assets/6521800/07d6f12e-096e-4771-b38c-84b4dd459828)
# Portcullis: Procedural Dungeon Generator Plugin for Unreal Engine

This Unreal Engine plugin is designed to procedurally generate randomized 3D dungeons for use in video games. The goal is to provide game developers with a flexible and powerful tool to create unique dungeon environments.

## Features
### In Progress
- **[Binary Space Partitioning (BSP) Algorithm](https://en.wikipedia.org/wiki/Binary_space_partitioning)**: The initial development focus is on implementing the BSP algorithm to generate dungeon layouts. This method divides the space into smaller sections, ensuring a structured and efficient way to create rooms and corridors.

### BSP Process Demonstration
The images below demonstrate the dungeon generation process using the BSP approach.
1. Divide space into partitons.
![partitions](https://github.com/user-attachments/assets/b7c65d5e-ee4b-42c3-8d7c-aa920d78a9d6)
2. Generate rooms and hallways connecting them.
   
![UnrealEditor_05N9wqhYUW](https://github.com/user-attachments/assets/8314cca7-74bf-44c6-9131-ffe3405792bd) | ![UnrealEditor_2mkutORy23](https://github.com/user-attachments/assets/423b0e95-4268-47e2-8d4d-0017a93fa88d)
:-------------------------:|:-------------------------:
![UnrealEditor_FwaWfvy9lQ](https://github.com/user-attachments/assets/f7f13380-408b-447e-a274-9c0ca03f0120) | ![UnrealEditor_DilW1uS5Vf](https://github.com/user-attachments/assets/63c8a1fe-d9f9-472d-bbd4-d1d8132f102c)


## Planned
- **[Random Walk Algorithm](https://en.wikipedia.org/wiki/Random_walk)**: Following the implementation of BSP, the next phase will introduce the random walk algorithm. This method allows for more organic and less predictable dungeon designs, providing a different style and variety in dungeon generation.
- **[Cellular Automata](https://en.wikipedia.org/wiki/Cellular_automaton?oldformat=true#Maze_generation)**: As a stretch goal, I aim to implement dungeon generation using cellular automata. This method simulates cellular processes to create complex and intricate cavern-like structures.
- **Infinite Dungeons**: Another stretch goal is to generate dungeons dynamically as the player explores them. This feature will allow for infinitely expanding dungeons, enhancing replayability and exploration.
