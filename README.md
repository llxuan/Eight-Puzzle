# Eight-Puzzle 
## Introduction
This project solved the eight puzzle based on IDA\* algorithm, and we generalize the algorithm to solve not only 8-puzzle, but also 3-puzzle, 15-puzzle,and 24-puzzle. And we demonstrate our idea as a Jigsaw Puzzle to add more fun to our work. In our project, you can choose the game type ,or rather, the difficulty you want. Apart from this, you can also make a choice between player mode and automatic mode.

Firstly, we will show you a puzzle accroding to the difficulty you choose. In player mode, the recover of the random picture is supposed to be completed by user, and there will be credit if you succeed, and consequently, criticize if you failed. While in the automatic mode, the program will finish the recoverment automaticly.

This project is written by Qt(C++) and python 3.4.
## Algorithm
### IDA\*
There exit many algorithms to solve the eight puzzle problem, one of the most efficient algorithm is IDA\*,iterative deepening algorithm.

In the A\* algorithm, we need to select the reasonable **evaluation function**, and then select the min f(Cost) node in the sub node to expand, in order to search the optimal solution. But in the A\* algorithm, we need to maintain the **closed list** and **open list**. It is necessary to detect the node that is extended out, ignore the node has entered the closed list. What's more, we should also check whether the repeated nodes have extended, if so, than we had better to update the corresponding operations. So the cost of the main A\* algorithm lies in state detection and the minimum cost sorting node, the process memory is relatively large.

Iterative deepening algorithm(IDA\*) is the combination of DFS and A\* algorithm.This algorithm is based on the DFS search algorithm,and gradually increase the depth of the search, which avoids using too much of the search space, but also reduces the depth of the blind search.

The basic idea of IDA\* is: 

1. Firstly, set the initial state node of the H threshold value to maxH.

2. Then, conduct the depth first search.

  Ignoring all the node whose H value that is greater than the maxH in the search process. If no solution is found, then increase the threshold of maxH, and then repeat the search until you find a solution. 

  It can be proved that the solution must be the optimal solution when the H value is satisfied with the A\* algorithm. 

  In the implementation of the program, IDA\* is more convenient than A\*, because you do not need to save the node, do not need to repeat the sentence, do not need to sort the node according to the value of H.

### Evaluation Function
The evaluation function can be represented as: F(x)=H(x)+G(x)

1. H(x)——The manhattan distance from the current state to the target state \* the dimension of puzzle.

  (For 3-puzzle,the dimension is 2; For 8-puzzle,the dimension is 3;For 15-puzzle,the dimension is 4;For 24-puzzle,the dimension is 5......)
  
2. G(x)——The depth of current status.
