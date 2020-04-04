#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <vector>
#include <random>
#include <unordered_set>
using namespace std;


//unordered_set<int> choosedIndex;
//int *choosedIndex;
class Kmean {
public:
    class Point {
    public:
        double Xcoord;//convert to int when plotting onto 2D displayAry
        double Ycoord;
        int label;
        double distance;

        Point(){
            Xcoord = 0;
            Ycoord = 0;
            label = 0;
            distance = 99999.00; //the distance to its own cluster centroid.
        }
        Point(double Xcoord, double Ycoord, int label, double distance){
            this->Xcoord = Xcoord;
            this->Ycoord = Ycoord;
            this->label = label;
            this->distance = distance;
        }
    };

    int K; // K clusters given in argv[2]
    int numPts; //The total number of points
    //Point* pointSet;//1D array of Point class of size numPts
    int numRows;
    int numCols;
    //int** displayAry;//a 2D array, size of numRows by numCols for displaying purposes
    Point* KcentroidAry;//No 0 as cluster label, cluster label from 1 to K
    int change = 0; // for tracking the label changes, initialize to 0

    Kmean(){
//       // int K = 0;
//        int numPts = 0;
//      //  pointSet = new Point[numPts];
//        int numRows = 0;
//        int numCols = 0;
//       // displayAry = new int* [numRows];
//        for(int i=0; i<numRows; i++){
//            displayAry[i] = new int[numCols];
//        }
//       KcentroidAry = new Point[K+1];
//        change = 0;
    }

//    Kmean( int numPts, int numRows, int numCols, int change){
//        //this->K = K;
//        this->numPts = numPts;
//       // pointSet = new Point[numPts];
//        this->numRows = numRows;
//        this->numCols = numCols;
//        displayAry = new int *[numRows];
//        for(int i=0; i<numRows; i++){
//            displayAry[i] = new int[numCols];
//        }
//        KcentroidAry = new Point[K+1];
//        this->change = change;
//    }

    //Methods

    void loadPointSet(ifstream& inFile, Point* pointSet){
        int index = 0;
        while(!inFile.eof()){
            int x, y;
            inFile >> x;
            inFile >> y;
            //cout << "x: " << x << " y: " << y << endl;
            pointSet[index].Xcoord = (double) x;
            pointSet[index].Ycoord = (double) y;
            pointSet[index].label = 0; //meaning there is no lable yet
            //cout <<  pointSet[index].Xcoord << " " <<   pointSet[index].Ycoord << " " <<pointSet[index].label << endl;
            pointSet[index].distance = 99999.00;
            index++;

        }
    }
     void KMeansClustering(Point* pointSet, int K, int** displayAry, ofstream& outFile){
        int iteration = 0;
        int *choosedIndex = new int[K]; //store the centroid index that already choosed
         selectKcentroids(pointSet, K, choosedIndex);
         while(change <= 2){
             int index = 0;
             iteration++;
             cout << "Test" << endl;
             plotDisplayAry(pointSet, displayAry);
             prettyPrint(displayAry, outFile, iteration);
             while(index < numPts){
                 Point pt = pointSet[index];
                 double minDist = 0.0;//pt's distance
                 int minLabel = distanceMinLable(pt, KcentroidAry,minDist);
                 if(pointSet[index].label != minLabel){
                     pointSet[index].label = minLabel;
                     pointSet[index].distance = minDist;
                     change++;
                 }
                 index++;
             }


             if(change > 2){
                 computeCentroids(pointSet, KcentroidAry);
                 change = 0;
             }
         }
    }
    bool checkRepeat(int index, int* choosedIndex){ //returns true is the index has been generated prior
        for(int i=0; i<K; i++){
            if(choosedIndex[i] == index){
                return true;//the index has been generated
            }
        }
        return false;
    }
    void selectKcentroids(Point* pointSet, int K, int* choosedIndex) {

        int Kcnt = 0;
        bool repeatYN = true;
        int index = 0;

        while(Kcnt <= K) {
            Kcnt++;
            index = rand() % numPts;
            while(repeatYN){
                repeatYN = checkRepeat(index, choosedIndex);
                index = rand() % numPts;
            }
            choosedIndex[Kcnt] = index;
            cout << "Index: " << index << endl;

            //choosedIndex[Kcnt++] = index;
            KcentroidAry[Kcnt].Xcoord = pointSet[index].Xcoord;
            KcentroidAry[Kcnt].Ycoord = pointSet[index].Ycoord;
            KcentroidAry[Kcnt].label = Kcnt;
            KcentroidAry[Kcnt].distance = 0.0;
            cout <<  KcentroidAry[Kcnt].Xcoord << " " <<    KcentroidAry[Kcnt].Ycoord << " " <<  KcentroidAry[Kcnt].label << endl;

        }
    }

    int  distanceMinLable(Point pt, Point* KcentroidAry, int minDist) {
        minDist = 99999.00;
        int minLabel = 0;
        int label = 1;
        while(label<=K){
            Point whichCentroid = KcentroidAry[label];
            int dist = computeDist(pt, whichCentroid);
            if(dist < minDist){
                minLabel = label;
                minDist = dist;
            }
            label++;
        }
        return minLabel;
    }
    int computeCentroids(Point* pointSet, Point* KcentroidAry){
//        for(int i=0; i<=K; i++){
//            cout << "center: ";
//            cout << KcentroidAry[i].Xcoord << " " << KcentroidAry[i].Ycoord << endl;
//        }
      double *sumX = new double[K+1]{0.0};
      double *sumY = new double[K+1]{0.0};
      int    *totalPt = new int[K+1]{0};
      int index = 0;
      while(index < numPts){
          int label = pointSet[index].label;
          sumX[label] += pointSet[index].Xcoord;
          sumY[label] += pointSet[index].Ycoord;
          totalPt[label]++;
          index++;
      }
      int label = 1;
      while(label <= K){
          if(totalPt[label]>0.0){
              KcentroidAry[label].Xcoord = sumX[label]/totalPt[label];
              KcentroidAry[label].Ycoord = sumY[label]/totalPt[label];
              label++;
          }
      }
    }
    double computeDist(Point pt, Point whichCentroid){
        int x1 = pt.Xcoord;
        int y1 = pt.Ycoord;
        int x2 = whichCentroid.Xcoord;
        int y2 = whichCentroid.Ycoord;
        return sqrt(pow(x2-x1,2)+ pow(y2-y1,2) * 1.0);
    }

    void plotDisplayAry(Point* pointSet, int** displayAry){
        cout << "Test11" << endl;
        for(int i=0; i<numPts; i++){
            int x = (int) pointSet[i].Xcoord;
            int y = (int) pointSet[i].Ycoord;
            displayAry[x][y] = pointSet[i].label;
            //cout <<  displayAry[x][y]  << endl;
        }
    }
    void prettyPrint(int** displayAry, ofstream& outFile, int iteration){
      cout << "Test22" << endl;
      outFile << " K : " << K  << " iteration :" << iteration;
      for(int i=0; i<numRows; i++){
          for(int j=0; j<numCols; j++){
              if(displayAry[i][j] > 0){
                  outFile << displayAry[i][j];
              }else{
                  outFile << " ";
              }
          }
          outFile << endl;
       }
    }
   void printResult(Point* pointSet, ofstream& outFile){
        outFile << numRows << " " << numCols << endl;
        outFile << numPts << endl;
        int i = 0;
        while(i<numPts){
            outFile << pointSet[i].Xcoord << " " << pointSet[i].Ycoord;
            i++;
        }
    }

};

int main(int argc, char *argv[]) {
    srand((unsigned)time(0));

    ifstream inFile1(argv[1]);
    int K = stoi(argv[2]);

    ofstream outFile1(argv[3]);
    ofstream outFile2(argv[4]);

    int numRows, numCols, numPts;
    inFile1 >> numRows;
    inFile1 >> numCols;
    inFile1 >> numPts;
    cout << "numRows:" << numRows << " numCols:" << numCols << endl;
    cout << numPts << endl;

    Kmean::Point *pointSet = new Kmean::Point[numPts];

    int** displayAry = new int*[numRows];
    for(int i=0; i<numRows; i++){
        displayAry[i] = new int[numCols];
    }


    int change = 0;

    Kmean *kmean = new Kmean();
    kmean->numPts = numPts;
    kmean->numRows = numRows;
    kmean->numCols = numCols;
    kmean->KcentroidAry = new Kmean::Point[K+1];
    kmean->K = K;
    kmean->change = change;
    kmean->loadPointSet(inFile1, pointSet);
    kmean->KMeansClustering(pointSet, K, displayAry , outFile1);


   // kmean->printResult(pointSet, outFile2);
    inFile1.close();
    outFile1.close();
    outFile2.close();
    return 0;
}
