# Project Title: CP 386: Assignment 4 - Winter 2022

### About Developers: 
* This project was developed by Jacob Cabral and Christopher Rossi. We are both in our third year of our computer science major at Wilfrid Laurier University (date is currently April 2022). In addition, we both like to play videogames and place squares on reddit r/place as a fun pass time. 

### Makefile: 
* It is a set of commands with variable names and targets to create an object file and to remove them. However, in this project, we did not make use of a makefile and just used the sample4_in text file as our input.

### Test Cases: 
* For our program, We did not use any test cases, just used the sample4_in text file as our testing

### Question 1 Banker's Algorithm: 
* We have made a multi-threaded program that implements the banker's algorithm. Customers request and release resources from the bank. The banker will keep track of the resources. The banker will grant a request if it satisfies the safety algorithm. If a request does not leave the system in a safe state, the banker will deny it. 

### Q1 Features:
* For question 1, we used the following data structures as mentioned in chapter 8 of the text book:
  * Available amount of each resource
  * Maximum demand of each customer
  * Amount currently allocated to each customer
  * Remaining need of each customer
  * Safety algorithm to grant a request, if it leaves the system in a safe state, otherwise destroy it
  * Allows the user to interactively enter a request for resources or to output the values of the different data structures (available, maximum, allocation, and need) used with the banker's algorithm and executes customers as threads in a safe sequence.

#### Expected output for Q1:
![image](https://user-images.githubusercontent.com/61894684/161637617-d00dc682-9798-4eca-96ea-f9b226e0820d.png)
![image](https://user-images.githubusercontent.com/61894684/161637737-1cc9f2d7-07a3-4d38-ab08-35fb787c4eea.png)







