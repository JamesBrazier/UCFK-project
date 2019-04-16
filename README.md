Through this was a group project I ended up doing all the work

Author: James Brazier (jbr185)
Date: 29  September 2018
Game type: Rock Paper Scissors

To run our game, please open up a terminal inside our 
team430 directory and enter command "make program".

How it works:
 - Firstly a welcome screen will appear, requiring you push the navswitch down to start the game
 - Then the connection phase will occur, which the board sends a connect signal 'c' to another board, the game starts when both boards have received a 'c' signal
 - Then players can make thier choices of either Rock, Paper or Scissors using the NavSwitch.
 - After a choice is made, players exchange IR signals that contains the information of their choice, either 'R', 'P', or 'S'
 - Once the signal is received, each player will receive a feedback of either Win, Lose or Draw once the IR signal is processed, number of lives will decrement by 1 if Lose, no change if Draw or Win
 - Each Player will also be able to view their remaining lives by pushing the up or down on the navswitch, players start with 3 lives currently
 - The game completes when one player reaches 0 lives, they will then send a defeat signal of 'D' to the other player to complete the game on thier board
 - Then the game returns to the welcome screen

If, at any point, the partner board hasn't connected or hasn't received your move, press s2 button to resend signal
