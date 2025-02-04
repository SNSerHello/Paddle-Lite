#include "paddle_api.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#if defined(WIN64) || defined(WIN32)
#else
#include <hiredis/hiredis.h>
#endif

#define lite paddle::lite_api
// using lite = paddle::lite_api;

int main(void) {
  //   ___                    ______     __
  //  / _ \ _ __   ___ _ __  / ___\ \   / /
  // | | | | '_ \ / _ \ '_ \| |    \ \ / /
  // | |_| | |_) |  __/ | | | |___  \ V /
  //  \___/| .__/ \___|_| |_|\____|  \_/
  //       |_|
  //

  // Step1: Preprocess image
  const std::string fname = "cat.jpg";
  cv::Mat img = cv::imread(fname, cv::IMREAD_COLOR);
  if (img.empty()) {
    std::cout << "Fail to read " << fname << std::endl;
    return -1;
  }
  cv::Size img_size = img.size();
  int img_channels = img.channels();
  std::cout << "Original image size[width, height]: " << img_size << std::endl;
  std::cout << "Original image channels: " << img_channels << std::endl;
  if (img_size.width > img_size.height) {
    int x_offset = (img_size.width - img_size.height) / 2;
    // Crop: (Range(y1:y2), Range(x1:x2))
    img = img(cv::Range(0, img_size.height),
              cv::Range(x_offset, x_offset + img_size.height));
  } else if (img_size.width < img_size.height) {
    int y_offset = (img_size.height - img_size.width) / 2;
    //  Crop: (Range(y1:y2), Range(x1:x2))
    img = img(cv::Range(y_offset, y_offset + img_size.width),
              cv::Range(0, img_size.width));
  }
  cv::resize(img, img, cv::Size(224, 224), 0, 0, cv::INTER_AREA);
  // HWC: uint8 -> float32
  img.convertTo(img, CV_32FC3, 1.0 / 255.0, 0);
  // BGR -> RGB
  // cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

  // Step2: Create Paddle Predicator
  const std::string nbfname = "mobilenet_v1_opencl.nb";
  lite::MobileConfig config;
  config.set_model_from_file(nbfname);
  std::shared_ptr<lite::PaddlePredictor> predictor =
    lite::CreatePaddlePredictor<lite::MobileConfig>(config);

  // Step3: Load data into predictor
  std::unique_ptr<lite::Tensor> input_tensor(std::move(predictor->GetInput(0)));
  input_tensor->Resize({1, 3, 224, 224});
  float *input_data = input_tensor->mutable_data<float>();
  {
    std::vector<cv::Mat> channels(3);
    cv::split(img, channels);
    for (int c = 0; c < 3; c++) {
      memcpy(input_data + c * 224 * 224, channels[c].data,
             224 * 224 * sizeof(float));
    }
  }

  // Step4: Run it
  predictor->Run();

  // Step5: Read labels from synset.txt
  std::vector<std::string> labels;
  {
    std::ifstream ifs("synset.txt");
    // std::copy(std::istream_iterator<std::string>(ifs),
    //           std::istream_iterator<std::string>(),
    //           std::back_inserter(labels));
    std::string label;
    while (std::getline(ifs, label)) {
      labels.emplace_back(label);
    }
  }

  {
    // Step6: Get results
    std::unique_ptr<const lite::Tensor> output_tensor(
      std::move(predictor->GetOutput(0)));
    const float *output_data = output_tensor->data<float>();
    int o_index[1000];
    for (int i = 0; i < 1000; i++) {
      o_index[i] = i;
    }
    std::sort(&o_index[0], &o_index[1000], [&output_data](int pos1, int pos2) {
      return (output_data[pos1] > output_data[pos2]);
    });

    // Step7: Print results
    std::cout << "TOP5 index : ";
    for (int i = 0; i < 5; i++) {
      if (i == 0)
        std::cout << "[";
      else
        std::cout << ", ";
      std::cout << o_index[i];
    }
    std::cout << "]" << std::endl;
    std::cout << "TOP5 probs : ";
    for (int i = 0; i < 5; i++) {
      if (i == 0)
        std::cout << "[";
      else
        std::cout << ", ";
      std::cout << output_data[o_index[i]];
    }
    std::cout << "]" << std::endl;
    std::cout << "TOP5 labels: ";
    for (int i = 0; i < 5; i++) {
      if (i == 0)
        std::cout << "[";
      else
        std::cout << std::endl << std::string(14, ' ');
      std::cout << labels[o_index[i]];
    }
    std::cout << "]" << std::endl;
  }

  //  ____          _ _
  // |  _ \ ___  __| (_)___
  // | |_) / _ \/ _` | / __|
  // |  _ <  __/ (_| | \__ \
  // |_| \_\___|\__,_|_|___/
  //
#if defined(WIN64) || defined(WIN32)
#else
  std::cout << std::string(69, '-') << std::endl;

  redisContext *conn = redisConnect("127.0.0.1", 6379);
  redisReply *r = (redisReply *)redisCommand(conn, "hget cv cat");
  assert(r != nullptr);
  if (r->type == REDIS_REPLY_STRING) {
    if (r->len > 0) {
      memcpy(input_data, r->str, 3 * 224 * 224 * sizeof(float));
      predictor->Run();
      std::unique_ptr<const lite::Tensor> output_tensor(
        std::move(predictor->GetOutput(0)));
      const float *output_data = output_tensor->data<float>();
      int o_index[1000];
      for (int i = 0; i < 1000; i++) {
        o_index[i] = i;
      }
      std::sort(&o_index[0], &o_index[1000],
      [&output_data](int pos1, int pos2) {
        return (output_data[pos1] > output_data[pos2]);
      });
      std::cout << "TOP5 index : ";
      for (int i = 0; i < 5; i++) {
        if (i == 0)
          std::cout << "[";
        else
          std::cout << ", ";
        std::cout << o_index[i];
      }
      std::cout << "]" << std::endl;
      std::cout << "TOP5 probs : ";
      for (int i = 0; i < 5; i++) {
        if (i == 0)
          std::cout << "[";
        else
          std::cout << ", ";
        std::cout << output_data[o_index[i]];
      }
      std::cout << "]" << std::endl;
      std::cout << "TOP5 labels: ";
      for (int i = 0; i < 5; i++) {
        if (i == 0)
          std::cout << "[";
        else
          std::cout << std::endl << std::string(14, ' ');
        std::cout << labels[o_index[i]];
      }
      std::cout << "]" << std::endl;
    } else {
      std::cout << "unknow len: " << r->len << std::endl;
    }
  } else {
    switch (r->type) {
    case REDIS_REPLY_STATUS:
      std::cout << "REDIS_REPLY_STATUS: " << REDIS_REPLY_STATUS << std::endl;
      break;
    case REDIS_REPLY_ERROR:
      std::cout << "REDIS_REPLY_ERRORL " << REDIS_REPLY_ERROR << std::endl;
      break;
    case REDIS_REPLY_INTEGER:
      std::cout << "REDIS_REPLY_INTEGER: " << REDIS_REPLY_INTEGER << std::endl;
      break;
    case REDIS_REPLY_NIL:
      std::cout << "REDIS_REPLY_NIL: " << REDIS_REPLY_NIL << std::endl;
      break;
    case REDIS_REPLY_STRING:
      std::cout << "REDIS_REPLY_STRING: " << REDIS_REPLY_STRING << std::endl;
      break;
    case REDIS_REPLY_ARRAY:
      std::cout << "REDIS_REPLY_ARRAY: " << REDIS_REPLY_ARRAY << std::endl;
      break;
    default:
      std::cout << "unknown type: " << r->type << std::endl;
    }
  }
  freeReplyObject(r);
  redisFree(conn);
#endif

  return 0;
}