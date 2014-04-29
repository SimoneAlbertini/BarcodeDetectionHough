
#include "MLP.hpp"

namespace artelab
{

    MLP::MLP() {}

    MLP::MLP(cv::Mat layers)
    {
        _model.create(layers, CvANN_MLP::SIGMOID_SYM, 1, 1);
    }

    MLP::~MLP() {}

    void MLP::load(std::string file) 
    {
        _model.load(file.c_str());
    }

    void MLP::save(std::string file) 
    {
        _model.save(file.c_str());
    }

    int MLP::train(const cv::Mat& patterns_in, const cv::Mat& targets, const int max_iter) 
    {
        CV_Assert(patterns_in.rows == targets.rows);

        CvTermCriteria criteria;
        criteria.max_iter = max_iter;
        criteria.epsilon = 0.001f;
        criteria.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;

        cv::ANN_MLP_TrainParams params;
        params.train_method = CvANN_MLP_TrainParams::RPROP;
        params.bp_dw_scale = 0.1f;
        params.bp_moment_scale = 0.1f;
        params.rp_dw0 = 0.1f;
        params.rp_dw_plus = 1.2f;
        params.rp_dw_minus = 0.5f;
        params.rp_dw_min = FLT_EPSILON;
        params.rp_dw_max = 50;
        params.term_crit = criteria;

        int iterations = _model.train(patterns_in, targets, cv::Mat(), cv::Mat(), params);
        return iterations;
    }

    void MLP::predict(const cv::Mat& samples, cv::Mat& outPredictions) 
    {
        _model.predict(samples, outPredictions);
    }
}