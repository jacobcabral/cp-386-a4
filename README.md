# Project Title: CP 386: Assignment 4 - Winter 2022

### About Developers: 
* This project was developed by Jacob Cabral and Christopher Rossi. We are both in our third year of our computer science major at Wilfrid Laurier University (date is currently April 2022). In addition, we both like to play videogames and place squares on reddit r/place as a fun pass time. 

### Makefile: 
* It is a set of commands with variable names and targets to create an object file and to remove them. However, in this project, we did not make use of a makefile and just used the sample4_in text file as our input.

### Test Cases: 
* For our program, We did not use any test cases, just used the sample4_in text file as our testing

### Sample Input:
![image](https://user-images.githubusercontent.com/61894684/161653777-60b54a31-71eb-4dfa-9357-b8eeab1564a6.png)


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

### Question 2: Best-Fit Algorithm (we did not complete Q2)
* In this program, you would be using Best-Fit algorithm for contiguous memory allocation (refer to section 9.2 from text). This project will involve managing a contiguous region of memory of size MAX where addresses may range from 0 ... MAX − 1.

### Q2 Features:
* Your program must respond to three different requests:
  * Request for a contiguous block of memory
  * Release of a contiguous block of memory
  * Report the regions of free and allocated memory
* Your program will allocate memory using Best-Fit algorithm. This will require that your program keep track of the different allocations and holes representing available memory

#### Expected output for Q2:
![image](https://user-images.githubusercontent.com/61894684/161638240-367c26d4-e9eb-46b2-8c38-38b3463f7242.png)




