#include <opencv4/opencv2/opencv.hpp>
#include<opencv4/opencv2/core.hpp>
#include<cmath>
#include<climits>
#include<cfloat>
using namespace cv;
using namespace std;

void RGB_matrix(long long int*** RGB,Mat img,int rows,int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            Vec3b pixel = img.at<Vec3b>(i,j);
            RGB[i][j][0]=pixel[2];
            RGB[i][j][1]=pixel[1];
            RGB[i][j][2]=pixel[0];
        }
    }
}

void energyMatrix(long long int*** RGB,double** energy,int rows,int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            long long int left=(j-1+cols)%cols;
            long long int right=(j+1+cols)%cols;
            long long int up=(i-1+rows)%rows;
            long long int down=(i+1+rows)%rows;
            
            long long int rh=RGB[i][right][0]-RGB[i][left][0];
            long long int gh=RGB[i][right][1]-RGB[i][left][1];
            long long int bh=RGB[i][right][2]-RGB[i][left][2];

            long long int rv=RGB[down][j][0]-RGB[up][j][0];
            long long int gv=RGB[down][j][1]-RGB[up][j][1];
            long long int bv=RGB[down][j][2]-RGB[up][j][2];

            double e=rh*rh+gh*gh+bh*bh+rv*rv+gv*gv+bv*bv;
            energy[i][j]=sqrt(e);
        }
    }
}

void energyDPVertical(double** energy,double** energy1,int rows,int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(i==0){
                energy1[i][j]=energy[i][j];
            }else if(j==0){
                energy1[i][j]=energy[i][j]+min(energy1[i-1][j],energy1[i-1][j+1]);
            }else if(j==cols-1){
                energy1[i][j]=energy[i][j]+min(energy1[i-1][j-1],energy1[i-1][j]);
            }else{
                energy1[i][j]=energy[i][j]+min(energy1[i-1][j-1],min(energy1[i-1][j],energy1[i-1][j+1]));
            }
        }
    }
}

void verticalSeam(double** energy,double** energy1,int* seam,int index,int rows,int cols){
    for(int i=rows-1;i>0;i--){
        //cout<<i<<" "<<index<<endl;
        if(index==0){
            if(fabs(energy1[i-1][index] - (energy1[i][index]-energy[i][index])) < 1e-6){
                
            }else{
                index=index+1;
            }
        }else if(index==cols-1){
            if(fabs(energy1[i-1][index] - (energy1[i][index]-energy[i][index])) < 1e-6){

            }else{
                index=index-1;
            }
        }else{
            if(fabs(energy1[i-1][index] - (energy1[i][index]-energy[i][index])) < 1e-6){

            }else if(fabs(energy1[i-1][index-1] - (energy1[i][index]-energy[i][index])) < 1e-6){
                index=index-1;
            }else{
                index=index+1;
            }  
        }
        seam[i-1]=index;
        
    }
}

void removeVerticalSeam(int* seam,Mat img,Mat newImg,int rows,int cols){
    
    for(int i=0;i<rows;i++){
        int k=0; 
        for(int j=0;j<cols;j++){
            if(j==seam[i]) continue;
            newImg.at<Vec3b>(i,k)=img.at<Vec3b>(i,j);
            k++;
        }
    }
}

void seamCarvingVertical(Mat &img,int rows,int cols,int reduce){

    for(int z=0;z<reduce;z++){
        long long int*** RGB = new long long int**[rows];
        for (int i = 0; i < rows; i++) {
            RGB[i] = new long long int*[cols];
            for (int j = 0; j < cols; j++) {
                RGB[i][j] = new long long int[3];
            }
        }

        // int channels=img.channels();
        // cout<<"height:"<<rows<<endl;
        // cout<<"width:"<<cols<<endl;
        // cout<<"channels:"<<channels<<endl;
        RGB_matrix(RGB,img,rows,cols);
        double** energy = new double*[rows];
        for (int i = 0; i < rows; i++) {
            energy[i] = new double[cols];
        }
        energyMatrix(RGB,energy,rows,cols);
        
        double** energy1 = new double*[rows];
        for (int i = 0; i < rows; i++) {
            energy1[i] = new double[cols];
        }

        energyDPVertical(energy,energy1,rows,cols);

        double mini=DBL_MAX;
        int index;
        for(int i=0;i<cols;i++){
            if(mini>energy1[rows-1][i]){
                mini=energy1[rows-1][i];
                index=i;
            }
        }
        //cout<<mini<<endl;
        
        int* seam=new int[rows];
        seam[rows-1]=index;
        verticalSeam(energy,energy1,seam,index,rows,cols);
        Mat output(rows,cols, CV_8UC3, Scalar(0,0,0));
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(j==seam[i])output.at<Vec3b>(i,j) = Vec3b(0,0,255);
                else output.at<Vec3b>(i,j) = Vec3b(RGB[i][j][2],RGB[i][j][1],RGB[i][j][0]);
            }
        }

        Mat newImg(rows,cols-1,CV_8UC3);
        removeVerticalSeam(seam,img,newImg,rows,cols);

        imwrite("output.jpeg",output);
        

        delete[] seam;

        for(int i=0;i<rows;i++) {
            delete[] energy[i];
        }
        delete[] energy;

        for (int i=0;i<rows;i++) {
            delete[] energy1[i];
        }
        delete[] energy1;

        for (int i=0;i<rows;i++) {
            for (int j=0;j<cols;j++) {
                delete[] RGB[i][j];
            }
            delete[] RGB[i];
        }
        delete[] RGB;
        img=newImg;
        cols--;
    }
    imwrite("output.jpeg",img);
}

void energyDPHorizontal(double** energy,double** energy1,int rows,int cols){
    for(int i=0;i<cols;i++){
        for(int j=0;j<rows;j++){
            if(i==0){
                energy1[j][i]=energy[j][i];
            }else if(j==0){
                energy1[j][i]=energy[j][i]+min(energy1[j][i-1],energy1[j+1][i-1]);
            }else if(j==rows-1){
                energy1[j][i]=energy[j][i]+min(energy1[j-1][i-1],energy1[j][i-1]);
            }else{
                energy1[j][i]=energy[j][i]+min(energy1[j-1][i-1],min(energy1[j][i-1],energy1[j+1][i-1]));
            }
        }
    }
}

void horizontalSeam(double** energy,double** energy1,int* seam,int index,int rows,int cols){
     for(int j=cols-1; j>0; j--){
        if(index==0){
            if(fabs(energy1[index][j-1] - (energy1[index][j]-energy[index][j])) < 1e-6){
            
            } else {
                index++;
            }
        } else if(index==rows-1){
            if(fabs(energy1[index][j-1] - (energy1[index][j]-energy[index][j])) < 1e-6){
                
            } else {
                index--;
            }
        } else {
            if(fabs(energy1[index][j-1] - (energy1[index][j]-energy[index][j])) < 1e-6){
                
            } else if(fabs(energy1[index-1][j-1] - (energy1[index][j]-energy[index][j])) < 1e-6){
                index--;
            } else {
                index++;
            }
        }
        seam[j-1] = index;
    }
}

void removeHorizontalSeam(int* seam,Mat img,Mat newImg,int rows,int cols){
      for(int j=0; j<cols; j++){
        int k=0;
        for(int i=0; i<rows; i++){
            if(i == seam[j]) continue;
            newImg.at<Vec3b>(k,j) = img.at<Vec3b>(i,j);
            k++;
        }
    }
}

void seamCarvingHorizontal(Mat &img,int rows,int cols,int reduce){
    for(int z=0;z<reduce;z++){
        long long int*** RGB = new long long int**[rows];
        for (int i=0;i<rows;i++) {
            RGB[i]=new long long int*[cols];
            for (int j=0;j<cols;j++) {
                RGB[i][j]=new long long int[3];
            }
        }

        // int channels=img.channels();
        // cout<<"height:"<<rows<<endl;
        // cout<<"width:"<<cols<<endl;
        // cout<<"channels:"<<channels<<endl;
        RGB_matrix(RGB,img,rows,cols);
        double** energy = new double*[rows];
        for (int i = 0; i < rows; i++) {
            energy[i] = new double[cols];
        }
        energyMatrix(RGB,energy,rows,cols);
        
        double** energy1 = new double*[rows];
        for (int i = 0; i < rows; i++) {
            energy1[i] = new double[cols];
        }

        energyDPHorizontal(energy,energy1,rows,cols);

        double mini=DBL_MAX;
        int index;
        for(int i=0;i<rows;i++){
            if(mini>energy1[i][rows-1]){
                mini=energy1[i][rows-1];
                index=i;
            }
        }
        //cout<<mini<<endl;
        
        int* seam=new int[cols];
        seam[cols-1]=index;
        horizontalSeam(energy,energy1,seam,index,rows,cols);
        Mat output(rows,cols, CV_8UC3, Scalar(0,0,0));
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(i==seam[j])output.at<Vec3b>(i,j) = Vec3b(0,0,255);
                else output.at<Vec3b>(i,j) = Vec3b(RGB[i][j][2],RGB[i][j][1],RGB[i][j][0]);
            }
        }

        Mat newImg(rows-1,cols,CV_8UC3);
        removeHorizontalSeam(seam,img,newImg,rows,cols);

        imwrite("output.jpeg",output);
        
        
        //imshow("Image", output);
        //imshow("Image",newImg);
        //waitKey(0);
        
        delete[] seam;

        for(int i=0;i<rows;i++) {
            delete[] energy[i];
        }
        delete[] energy;

        for (int i=0;i<rows;i++) {
            delete[] energy1[i];
        }
        delete[] energy1;

        for (int i=0;i<rows;i++) {
            for (int j=0;j<cols;j++) {
                delete[] RGB[i][j];
            }
            delete[] RGB[i];
        }
        delete[] RGB;
        img=newImg;
        rows--;
    }
    imwrite("output.jpeg",img);
}


int main() {
    string s;
    cin>>s;
    int height;
    int width;
    cin>>width>>height;
    Mat img = imread(s);
    if(img.empty()) {
        cout << "Could not read the image" << endl;
        return -1;
    }
    int rows=img.rows;
    int cols=img.cols;
    if(height<=0 || width<=0){
        cout<<"invalid input";
        return -1;
    }
    if(width>cols){
        cout<<"Given width is greater than the width of the original image.";
        return -1;
    }
    if(height>rows){
        cout<<"Given height is greater than the height of the original image.";
        return -1;
    }
    seamCarvingVertical(img,rows,cols,cols-width);
    seamCarvingHorizontal(img,rows,cols-(cols-width),rows-height);
    //waitKey(0);
    return 0;
}