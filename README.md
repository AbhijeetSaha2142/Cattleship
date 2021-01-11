# Cattleship :cow2:
## Period 10: Abhijeet Saha, Steven Lee, Kristoff Misquitta
### Description
Farmer David and Farmer Jun are missing their cattle! Luckily, they’re on the other’s farms, hiding in the tall grass. Help your farmer secure their cows first!

A Battleship clone. Cows (not battleships because that is too violent) of different lengths (5x1, 4x1, 3x1, 3x1, 2x1) are placed on farms (10x10 alphanumeric grid). Each farmer guesses where on the board their cattle is. If they hit their cattle, they are given another turn. Otherwise, it goes to the next player. The winner is the one who finds all of their cattle first. 

Inspiration: http://en.battleship-game.org/id37867548/classic
### User Interface
At the beginning each player is shown two boards: their board with randomly placed cows of varying length, either horizontally or vertically and nonadjacent, and the enemy board where they guess the square that might. If they guess correctly they are able to guess again until they get it wrong or find all cows. After they are done guessing their interface is cleared so that the other player doesn’t see it in their peripheral. Your own board is updated to show the places searched by the other player, as is the enemy board for the places searched by you. The game will end when either player wins or one of them uses ctrl-c, which is counted as a forfeit making the other player win automatically.
### Design
There are two programs: the clients and the server. The server manages the game, meaning it handles all the game logic, and sends the boards to the clients using a pipe. Each client displays the boards/takes user input and transmits the move to the server using a pipe. 

To initialize the server and client, the handshake method is used (which works with files). The first client that connects to the server is player 1 and the second client that connects is player 2. Within the server, two boards (with 5x1, 4x1, 3x1, 3x1, 2x1 pieces) are generated using an algorithm (our own we still have to figure out) and in 2D char arrays, and allocated into dynamic memory so that functions (which check for winning, modify according to moves) can modify them. The server then sends the arrays to the first player client. 

The first player client receives their array (their board), and displays it. Another board displays their hits and misses. Then, it will prompt the user to make a move. The user types in an alphanumeric number (e.g. A1) and that number is sent to the server through a pipe. The server applies that move to the second player’s board, and if player 1 makes a hit, then the player makes another move. Otherwise, it sends the arrays to the player 2 client and asks player 2 to make a move. When a player is making a move, the other player’s terminal is cleared using a fork and exec clear. 

The game will stop either when one player wins, or when one player does ctrl-c. Using ctrl-c will send a signal to the server, then sends a message to the other player that the player has quit. Then the other player’s client exits. 

There are three parts to this project: client, server, and algorithm. Abhijeet will work on the algorithm that creates the boards and determines game logic. Steven will work on the server component. Kristoff will work on the client component. 
### Timeline
1/13 - Client is able to display sample boards.
1/15 - Algorithm developed and working, able to create randomized boards.
1/19 - 2 Clients and Server are able to handshake and execute game logic.
1/21 - Finished project, with most bugs fixed.
