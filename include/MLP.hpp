/* 
 * File:   MLP.h
 * Author: ignazio
 *
 * Created on May 25, 2013, 8:02 AM
 */

#ifndef MLP_H
#define	MLP_H

#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

class MLP 
{
public:
    MLP();
    MLP(cv::Mat layers);
    MLP(const MLP& orig);
    MLP& operator=(const MLP& other);
    virtual ~MLP();
    
    void load(std::string file);
    void save(std::string file);
    
    int train(const cv::Mat& trainingData, const cv::Mat& targets, const int max_iter=10000);
    void predict(const cv::Mat& samples, cv::Mat& outPredictions);
    
private:
    cv::NeuralNet_MLP _model;
};

#endif	/* MLP_H */

