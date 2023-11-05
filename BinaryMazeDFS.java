import java.util.Stack;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class BinaryMazeDFS {
    private static final int[] dx = { -1, 1, 0, 0 };
    private static final int[] dy = { 0, 0, -1, 1 };

    public static List<String> solveMaze(boolean[][] maze, int startX, int startY, int endX, int endY) {
        int rows = maze.length;
        int cols = maze[0].length;
        boolean[][] visited = new boolean[rows][cols];

        Stack<int[]> stack = new Stack<>();
        stack.push(new int[] { startX, startY });

        // Create a 2D array to store the moves
        String[][] moves = new String[rows][cols];

        while (!stack.isEmpty()) {
            int[] current = stack.pop();
            int x = current[0];
            int y = current[1];

            // Check if we've reached the destination
            if (x == endX && y == endY) {
                return extractPath(moves, startX, startY, endX, endY);
            }

            // Mark the current cell as visited
            visited[x][y] = true;

            for (int i = 0; i < 4; i++) {
                int newX = x + dx[i];
                int newY = y + dy[i];

                // Check if the move is within the maze bounds and is not a wall and has not
                // been visited
                if (isValidMove(newX, newY, rows, cols) && !maze[newX][newY] && !visited[newX][newY]) {
                    stack.push(new int[] { newX, newY });
                    // Store the move in the moves array for backtracking
                    switch (i) {
                        case 0:
                            moves[newX][newY] = "up";
                            break;
                        case 1:
                            moves[newX][newY] = "down";
                            break;
                        case 2:
                            moves[newX][newY] = "left";
                            break;
                        case 3:
                            moves[newX][newY] = "right";
                            break;
                    }
                }
            }
        }

        // If no path to the end was found, return an empty list
        return Collections.emptyList();
    }

    public static boolean isValidMove(int x, int y, int rows, int cols) {
        // Check if the move is within the maze bounds
        return (x >= 0 && x < rows && y >= 0 && y < cols);
    }

    public static List<String> extractPath(String[][] moves, int startX, int startY, int endX, int endY) {
        List<String> path = new ArrayList<>();
        int x = endX;
        int y = endY;

        // Backtrack from the destination to the start and add moves to the path list
        while (x != startX || y != startY) {
            String move = moves[x][y];
            path.add(move);

            // Update the current position based on the move
            if (move.equals("up")) {
                x++;
            } else if (move.equals("down")) {
                x--;
            } else if (move.equals("left")) {
                y++;
            } else if (move.equals("right")) {
                y--;
            }
        }

        // Reverse the path to get it in the correct order
        Collections.reverse(path);
        return path;
    }

    public static void main(String[] args) {
        boolean[][] maze = {
                { false, false, true, true, false },
                { false, true, false, false, false },
                { false, false, true, true, true },
                { true, false, false, false, false },
                { false, false, false, true, false }
        };

        int startRow = 0;
        int startCol = 0;
        int endRow = 3;
        int endCol = 4;

        List<String> path = solveMaze(maze, startRow, startCol, endRow, endCol);

        if (!path.isEmpty()) {
            System.out.print(
                    "Shortest path from (" + startRow + "," + startCol + ") to (" + endRow + "," + endCol + "): ");
            for (String move : path) {
                System.out.print(move + ", ");
            }
        } else {
            System.out.println(
                    "No path found from (" + startRow + "," + startCol + ") to (" + endRow + "," + endCol + ")");
        }
    }
}
