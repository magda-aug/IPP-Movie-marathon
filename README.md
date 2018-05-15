# IPP-Movie-marathon

Program enabling movie fans choosing movies for marathon. Each one of them is able to add or delete films. Every user is represented by unique ID. Every movie is represented by unique ID. Users form a tree. In the beginning there is user with ID 0 with no movie preferences. Each user can add new users as its descendant. Each user can delete oneself from the tree. Every movie's ID is its arbitrary rating. 

The program realizes these assumptions using list and tree structures. 

## Supported operations

* addUser &lt; parentUserId &gt; &lt; userId &gt;     
  User with ID parentUserId adds user with ID userId. Constant time operation.

* delUser &lt; userId &gt;      
  User with ID userId checks out. Its movie preferences are also deleted. Users added by user userId become descendants of userId's parent. Deleting user is constant time operation. Forgetting movie preferences is linear time operation. 

* addMovie &lt; userId &gt; &lt; movieRating &gt;      
  User with ID userId adds movie with ID movieRating to its movie preferences. It is at most linear time in regard to number of user's preferences operation.

* delMovie &lt; userId &gt; &lt; movieRating &gt;      
  User with ID userId deleted filmwith ID movieRating from its preferences. At most linear time in regard to number of user's preferences operation.

* marathon &lt; userId &gt; &lt; k &gt;    
  Finds at most k movie IDs with highest rating from:  
    + preferences of userId;

    + preferences reached by marathon operation perfomed for userId's descendants - resulting group contains only films with rating higher than maximal rating of userId's films

  It is at most linear time in regard to product of k and number of users for which operation is recursively performed.

## Makefile
Command make creates executable file main.
Command make clean removes executable file and all files created during compilation.
