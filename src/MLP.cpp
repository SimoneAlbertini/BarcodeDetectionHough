
#include "MLP.hpp"

namespace artelab
{

    MLP::MLP() {}

    MLP::MLP(cv::Mat layers)
    {
      std::cout<<layers<<std::endl;
        _model = cv::ml::ANN_MLP::create();
        _model->setLayerSizes(layers);
        _model->setActivationFunction(cv::ml::ANN_MLP::ActivationFunctions::SIGMOID_SYM);
    }

    MLP::~MLP() {}

    void MLP::load(std::string file) 
    {

        _model = cv::Algorithm::load<cv::ml::ANN_MLP>(file.c_str());
    }

    void MLP::save(std::string file) 
    {
        _model->save(file.c_str());
    }

    int MLP::train(const cv::Mat& patterns_in, const cv::Mat& targets, const int max_iter) 
    {
        CV_Assert(patterns_in.rows == targets.rows);

        cv::TermCriteria criteria;
        criteria.maxCount = max_iter;
        criteria.epsilon = 0.001f;
        criteria.type = cv::TermCriteria::Type::COUNT | cv::TermCriteria::Type::EPS;

        _model->setTrainMethod(cv::ml::ANN_MLP::TrainingMethods::RPROP);
        _model->setBackpropMomentumScale(0.1f);
        _model->setBackpropWeightScale(0.1f);
        _model->setRpropDW0(0.1f);
        _model->setRpropDWMax(50);
        _model->setRpropDWMin(FLT_EPSILON);
        _model->setRpropDWMinus(0.5f);
        _model->setRpropDWPlus(1.2f);
        _model->setTermCriteria(criteria);

	//TODO: might have to change to COL_SAMPLE
    cv::Ptr<cv::ml::TrainData> training_data = cv::ml::TrainData::create(patterns_in, cv::ml::SampleTypes::ROW_SAMPLE, targets);
        int iterations = _model->train(training_data);
        return iterations;
    }

    void MLP::predict(const cv::Mat& samples, cv::Mat& outPredictions)
    {
        _model->predict(samples, outPredictions);
    }
}
