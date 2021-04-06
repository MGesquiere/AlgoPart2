Algorithms Project 2

The zipped file contains 2 files labeled *convexHull.cpp*, *README.txt*

**convexHull.cpp:**

To compile this file on a terminal, you need to type gcc convexHull.cpp -o convexHull -lstdc++. The -lstdc++ is included to tell the compiler that the file is a c++ file. 

To run this file after you compiled it, you need to type ./convexHull A test.txt. The A represents which algorithm you wish to use. You type J for Jarvis, G for Graham, and Q for quick hull. You can only test them one at a time.

To ensure it works with the java file, you need to make sure n has the right amount of points needed to test an algorithm. So, if you have the java file generating a test.txt with 100 points, you need to make sure n = 100; or else the containers will not have enough space to hold the points. 
* Note: you may still receive a segmentation fault for 1 million points

In the java file, there is a section labeled main. This is where you would determine what shape and how many points you wish to generate. You simply choose circle, onCircle, triangle, or rectangle. Then, you choose between 10 and 1 million points. To write to the test file, you must ensure the boolean in main is set to true. Them compile using javac ConvexHull_GUI.java and run using java ConvexHull_GUI.
Then you run your cpp file. After that, you set the boolean to false and recompile and run the java file. When you do this, you should see a visual of what the code has done. 

You must also make sure that the java file is reading from the right hull. If you're using Jarvis, change "hull.txt" to "hull_J.txt". For Graham, "hull_G.txt". For Quick Hull, "hull_Q.txt".

These are the sources I used in working on the 3 algorithms
jarvis https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/amp/
graham https://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/
quick hull https://tutorialspoint.dev/algorithm/geometric-algorithms/quickhull-algorithm-convex-hull

**what the program does**

This program creates a convex hull with a set of given points. The three algorithms used to create the hull are Graham Scan, Jarvis March, and Quick Hull.
