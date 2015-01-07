This program was created to find the shortest path between two points on a gridded maze, while meeting all checkpoints. 
The maze would be inputted in the beginning, with the length and width and the shape of the maze as such:
"#" = borders
"." = empty spaces
"@" = checkpoints
"S" = start
"G" = goal

Test cases were made such that a border (#) would exist around the maze, creating a boundary.
The program was evaluated on efficiency, then accuracy. Utilizing a modified flood fill algorithm to 
find the shortest path between all points, then  finding the shortest path via bounded brute force search 
or an ant colony algorithm (depending on the number of checkpoints).
