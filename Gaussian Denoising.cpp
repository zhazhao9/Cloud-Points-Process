#include <iostream> 


#include <pcl/io/pcd_io.h>


#include <pcl/point_types.h> 


#include <pcl/point_cloud.h>


#include <boost/random.hpp>


#include <pcl/console/time.h>


#include <pcl/filters/convolution_3d.h>


#include <pcl/kdtree/kdtree_flann.h>


using namespace std;


vector<vector<double> > DenoisingPD_GaussianKernel(vector<vector<double> > pointCloudOriginal,double radius) {


  cout << "Gaussian denoising start:" << endl;


  pcl::PointCloud<pcl::PointXYZ>::Ptr inputcloud(new pcl::PointCloud<pcl::PointXYZ>);


  pcl::PointCloud<pcl::PointXYZ>::Ptr outputcloud(new pcl::PointCloud<pcl::PointXYZ>);


  //Read point cloud in the input file


  for (int i = 0; i < pointCloudOriginal.size(); i++)


  {


    pcl::PointXYZ cloud_i;


    cloud_i.x = pointCloudOriginal[i][0];


    cloud_i.y = pointCloudOriginal[i][1];


    cloud_i.z = pointCloudOriginal[i][2];


    inputcloud->push_back(cloud_i);


  }


  //Set up the Gaussian Kernel


  pcl::filters::GaussianKernel<pcl::PointXYZ, pcl::PointXYZ>::Ptr kernel(new pcl::filters::GaussianKernel<pcl::PointXYZ, pcl::PointXYZ>);


  (*kernel).setSigma(4);


  (*kernel).setThresholdRelativeToSigma(4);


  std::cout << "Kernel made" << std::endl;


  //Set up the KDTree


  pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);


  (*kdtree).setInputCloud(inputcloud);


  std::cout << "KdTree made" << std::endl;


  //Set up the Convolution Filter


  pcl::filters::Convolution3D <pcl::PointXYZ, pcl::PointXYZ, pcl::filters::GaussianKernel<pcl::PointXYZ, pcl::PointXYZ> > convolution;


  convolution.setKernel(*kernel);


  convolution.setInputCloud(inputcloud);


  convolution.setSearchMethod(kdtree);


  convolution.setRadiusSearch(radius);


  convolution.setNumberOfThreads(10);//important! Set Thread number for openMP


  std::cout << "Convolution Start" << std::endl;


  convolution.convolve(*outputcloud);


  std::cout << "Convoluted" << std::endl;


  vector<vector<double>> pCDenosing(pointCloudOriginal.size());


  for (int i = 0; i < outputcloud->size(); i++) {


    vector<double> pi(3);


    pi[0] = outputcloud->at(i).x;


    pi[1] = outputcloud->at(i).y;


    pi[2] = outputcloud->at(i).z;


    pCDenosing[i] = pi;


  }


  cout << "Gaussian denoising finished!" << endl;


}


