/* 
   Madeline Gesquiere
   Used resources on geeksforgeeks as a reference
   to the 3 algorithms used in this project
   (specific links listed in my readme.txt)

   convexHull.cpp
*/

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <utility>
#include <set>

// integer pairs for quick hull
#define quickPair std::pair<int,int>

// store quick hull points
std::set<quickPair> quick;

struct Point
{
   int x, y;
};

// needed in order to sort points in reference to first point in qsort()
Point m0;

// find the orientation of (m,i,o)
// we want to know if it will be counterclockwise or not
// used with JARVIS and GRAHAM
int orient(Point m, Point i, Point o);

// swaps two points
void swap(Point &t1, Point &t2);

// find next to top
Point nextToTop(std::stack<Point> &S);

// find distance squared between points
int distSq(Point t1, Point t2);

// sorts array in reference to first point
int compare(const void *vt1, const void *vt2);

// finds the side of point m in reference to points t1 and t2
int findSide(quickPair t1, quickPair t2, quickPair m);

// return a value proportional to the distance between
// point m and t1 and t2
int lineDist(quickPair t1, quickPair t2, quickPair m);

// quick hull function
// t1 and t2 are endpoints of a given line
// side can be either 1 or -1
void quickHull(quickPair quickPoints[], int n, quickPair t1, quickPair t2, int side);

int main(int argc, char *argv[])
{   
   if (argc < 3) 
      std::cout << "wrong format! should be \"a.exe algType dataFile\"\n";
   else {
      std::string algType = argv[1];
      std::string dataFilename = argv[2];

      std::string outputFile = "";

      // array to hold points from test.txt
      // *NOTE: must change the size of the array depending on how many points
      // you wish to read in. For example, if you want to read in 10 points, the
      // array must be big enough to hold 10 points
      int n = 1000; // was originally 10
      Point points[n] = {};
      quickPair quickPoints[n] = {};
      int x, y;
      // variable used to read in test files
      std::ifstream in;
      // counter to place points in the right spot in array
      int i = 0;
      // conatiner for points
      std::vector<Point> hull;
      
      // open test.txt and read in the points
      in.open(dataFilename);

      // if file cannot be opened or does not exist, do not continue
      if(in.fail())
      {
         std::cout << "File does not exist.\n";
         std::exit(1);
      }

      // while there is data to be read in, read it in to the container
      while(in >> x >> y)
      {
         points[i].x = x;
         points[i].y = y;
         quickPoints[i].first = x;
         quickPoints[i].second = y;
         ++i;
      }
      
      // close test.txt when done
      in.close();

      // variable that will print out to the different hull text files
      std::ofstream out;
      
      // GRAHAM ALGORITHM
      if (algType[0]=='G') {

      // Find the lowest point
      int mini = points[0].y;
      int min = 0;
      for (int i=1; i<n; i++)
      {
         int y = points[i].y;
   
         // Pick the lowest or leftmost point if tied
         if ((y < mini) || (mini == y && points[i].x < points[min].x))
         {
            mini = points[i].y;
            min = i;
         }
      }
   
      // Place the lowest point at first position
      swap(points[0], points[min]);
   
      // sort n-1 points in reference to the first point
      m0 = points[0];
      std::qsort(&points[1], n-1, sizeof(Point), compare);

      // if two or more points have same angle with m0, keep the one 
      // farthest from m0 and remove the rest
      int m = 1; // new size of array
      for (int i=1; i<n; i++)
      {
         // remove all points while i and i+1 have same angle 
         // in reference to m0
         while (i < n-1 && orient(m0, points[i], points[i+1]) == 0)
            i++;
         points[m] = points[i];
         m++;  // Update size of modified array
      }
   
      // if new array has less than 3 points, convex hull is impossible
      if (m < 3)
         std::exit(1);

      // new empty stack and push first 3 points on
      std::stack<Point> S;
      S.push(points[0]);
      S.push(points[1]);
      S.push(points[2]);

      // add the rest
      for (int i = 3; i < m; i++)
      {
         // keep removing the top while the points do not go left
         while (S.size()>1 && orient(nextToTop(S), S.top(), points[i]) != 2)
            S.pop();
         S.push(points[i]);
      }

         // print contents to text file
         outputFile = "hull_G.txt";
         out.open(outputFile);
         while (!S.empty())
         {
            Point l = S.top();
            out << l.x << "\t" << l.y << std::endl;
            S.pop();
         }
         out.close();
      } 

      // JARVIS ALGORITHM
      else if (algType[0]=='J') {
         
         int left = 0;

         // find the leftmost point in the hull
         for(int i=1; i<n; i++)
         {
            if(points[i].x < points[left].x)
               left = i;
         }

         int m = left, o;

         do
         {
            // add current points to hull
            hull.push_back(points[m]);

            o = (m+1)%n;
            // if i is more leftthan o
            // set o = to i
            for(int i=0;i<n;i++)
            {
               if(orient(points[m],points[i],points[o]) == 2)
                  o = i;
            }

            // y is now leftmost wise point,
            // so we set m equal to o for the next iteration
            m = o;
         } while (m != left); // while not reaching first point
         
         // print contents to text file
         outputFile = "hull_J.txt";
         int j = 0;
         out.open(outputFile);
         while(j < hull.size())
         {
            out << hull[j].x << "\t" << hull[j].y << "\n";
            j++;
         }
         out.close();
      }

      // QUICK HULL
      else if (algType[0]=='Q')
      {

         // find the minimum and maximum X points
         int minix = 0;
         int maxix = 0;
         for (int i=1; i<n; i++)
         {
            if (quickPoints[i].first < quickPoints[minix].first)
                  minix = i;
            if (quickPoints[i].first > quickPoints[maxix].first)
                  maxix = i;
         }
      
         // Recursively find convex hull points on
         // one side of line joining quickPoints[minix] and
         // quickPoints[maxix]
         quickHull(quickPoints, n, quickPoints[minix], quickPoints[maxix], 1);
      
         // Recursively find convex hull points on
         // other side of line joining quickPoints[minix] and
         // quickPoints[maxix]
         quickHull(quickPoints, n, quickPoints[minix], quickPoints[maxix], -1);

         // print outputs to text file
         outputFile = "hull_Q.txt";
         out.open(outputFile);
         while(!quick.empty())
         {
            out << (*quick.begin()).first << "\t" << (*quick.begin()).second << "\n";
            quick.erase(quick.begin());
         }
         out.close();
      }
	}
	return 0;
}

// find the orientation of (m,i,o)
// we want to know if it will be counterclockwise or not
// used with JARVIS and GRAHAM
int orient(Point m, Point i, Point o)
{
   // finding orientation of the points
   int val = (i.y - m.y) * (o.x - i.x) - (i.x - m.x) * (o.y - i.y);

   // is counterclockwise
   if (val > 0) 
      return 2; 
   // isn't counterclockwise
   else
      return 0;
}

// swaps two points
void swap(Point &t1, Point &t2)
{
    Point temp = t1;
    t1 = t2;
    t2 = temp;
}

// find next to top
Point nextToTop(std::stack<Point> &S)
{
    Point m = S.top();
    S.pop();
    Point res = S.top();
    S.push(m);
    return res;
}

// find distance between points
int distSq(Point t1, Point t2)
{
    return (t1.x - t2.x)*(t1.x - t2.x) + (t1.y - t2.y)*(t1.y - t2.y);
}

// sorts array in reference to first point
int compare(const void *vt1, const void *vt2)
{
   Point *t1 = (Point *)vt1;
   Point *t2 = (Point *)vt2;
 
   // Find orientation
   int o = orient(m0, *t1, *t2);

   // not counterclockwise
   if(o == 0)
   {
      // if 2 t2 is greater than t1, keep it
      if(distSq(m0, *t2) >= distSq(m0, *t1))
         return 1;
      else
         return -1;
   }
   if(o < 0)
        return -1;
   else
        return 1;
}

// finds the side of point m in reference to points t1 and t2
int findSide(quickPair t1, quickPair t2, quickPair m)
{
    int val = (m.second - t1.second) * (t2.first - t1.first) - (t2.second - t1.second) * (m.first - t1.first);
  
    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}

// return a value proportional to the distance between
// point m and t1 and t2
int lineDist(quickPair t1, quickPair t2, quickPair m)
{
    return abs ((m.second - t1.second) * (t2.first - t1.first) - (t2.second - t1.second) * (m.first - t1.first));
}

// quick hull function
// t1 and t2 are endpoints of a given line
// side can be either 1 or -1
void quickHull(quickPair quickPoints[], int n, quickPair t1, quickPair t2, int side)
{
    int ind = -1;
    int maxd = 0;
  
    // find max distance from point to specific line side
    for (int i=0; i<n; i++)
    {
        int temp = lineDist(t1, t2, quickPoints[i]);
        if (findSide(t1, t2, quickPoints[i]) == side && temp > maxd)
        {
            ind = i;
            maxd = temp;
        }
    }

    // no points found = add end points to line of convex hull
    if (ind == -1)
    {
        quick.insert(t1);
        quick.insert(t2);
        return;
    }
  
    // Recur for the two parts divided by quickPoints[ind]
    quickHull(quickPoints, n, quickPoints[ind], t1, -findSide(quickPoints[ind], t1, t2));
    quickHull(quickPoints, n, quickPoints[ind], t2, -findSide(quickPoints[ind], t2, t1));
} 
