/*******************************************************************
*
*    DESCRIPTION:
*      AILIA yolox sample
*    AUTHOR:
*
*    DATE:2022/07/20
*
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

#undef UNICODE

#include "ailia.h"
#include "ailia_detector.h"
#include "utils.h"
#include "detector_utils.h"
#include "webcamera_utils.h"


// ======================
// Parameters
// ======================

#define WEIGHT_PATH "best_ckpt.onnx"
#define MODEL_PATH  "best_ckpt.onnx.prototxt"

#define IMAGE_PATH      "uploaded_image.png"
#define SAVE_IMAGE_PATH "output_1.png"

#define MODEL_INPUT_WIDTH  640
#define MODEL_INPUT_HEIGHT 640
#define IMAGE_WIDTH        640 // for video mode
#define IMAGE_HEIGHT       640 // for video mode

#define THRESHOLD 0.4f
#define IOU       0.45f

#if defined(_WIN32) || defined(_WIN64)
#define PRINT_OUT(...) fprintf_s(stdout, __VA_ARGS__)
#define PRINT_ERR(...) fprintf_s(stderr, __VA_ARGS__)
#else
#define PRINT_OUT(...) fprintf(stdout, __VA_ARGS__)
#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)
#endif

#define BENCHMARK_ITERS 5

static std::string weight_support(WEIGHT_PATH);
static std::string model_support(MODEL_PATH);

static std::string image_path_support(IMAGE_PATH);
static std::string video_path_support("0");
static std::string save_image_path_support(SAVE_IMAGE_PATH);

static const std::vector<const char*> COCO_CATEGORY = {
	"sunglass", "hat", "jacket", "shirt", "pants", "shorts", "skirt", "dress", "bag", "shoe"
};

static bool benchmark_support = false;
static bool video_mode_support = true;
static int args_env_id = -1;


// ======================
// Arguemnt Parser
// ======================

static void print_usage_support()
{
	PRINT_OUT("usage: yolox [-h] [-i IMAGE] [-v VIDEO] [-s SAVE_IMAGE_PATH] [-b] [-e ENV_ID]\n");
	return;
}


static void print_help_support()
{
	PRINT_OUT("\n");
	PRINT_OUT("yolox model\n");
	PRINT_OUT("\n");
	PRINT_OUT("optional arguments:\n");
	PRINT_OUT("  -h, --help            show this help message and exit\n");
	PRINT_OUT("  -i IMAGE, --input IMAGE\n");
	PRINT_OUT("                        The input image path.\n");
	PRINT_OUT("  -v VIDEO, --video VIDEO\n");
	PRINT_OUT("                        The input video path. If the VIDEO argument is set to\n");
	PRINT_OUT("                        0, the webcam input will be used.\n");
	PRINT_OUT("  -s SAVE_IMAGE_PATH, --savepath SAVE_IMAGE_PATH\n");
	PRINT_OUT("                        Save path for the output image.\n");
	PRINT_OUT("  -b, --benchmark       Running the inference on the same input 5 times to\n");
	PRINT_OUT("                        measure execution performance. (Cannot be used in\n");
	PRINT_OUT("                        video mode)\n");
	PRINT_OUT("  -e ENV_ID, --env_id ENV_ID\n");
	PRINT_OUT("                        The backend environment id.\n");
	return;
}


static void print_error_support(std::string arg)
{
	PRINT_ERR("yolov3-tiny: error: unrecognized arguments: %s\n", arg.c_str());
	return;
}


static int argument_parser_support(int argc, char** argv)
{
	int status = 0;

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (status == 0) {
			if (arg == "-i" || arg == "--input") {
				status = 1;
			}
			else if (arg == "-v" || arg == "--video") {
				video_mode_support = true;
				status = 2;
			}
			else if (arg == "-s" || arg == "--savepath") {
				status = 3;
			}
			else if (arg == "-b" || arg == "--benchmark") {
				benchmark_support = true;
			}
			else if (arg == "-h" || arg == "--help") {
				print_usage_support();
				print_help_support();
				return -1;
			}
			else if (arg == "-e" || arg == "--env_id") {
				status = 4;
			}
			else {
				print_usage_support();
				print_error_support(arg);
				return -1;
			}
		}
		else if (arg[0] != '-') {
			switch (status) {
			case 1:
				image_path_support = arg;
				break;
			case 2:
				video_path_support = arg;
				break;
			case 3:
				save_image_path_support = arg;
				break;
			case 4:
				args_env_id = atoi(arg.c_str());
				break;
			default:
				print_usage_support();
				print_error_support(arg);
				return -1;
			}
			status = 0;
		}
		else {
			print_usage_support();
			print_error_support(arg);
			return -1;
		}
	}

	return AILIA_STATUS_SUCCESS;
}


// ======================
// Main functions
// ======================

static int recognize_from_image_support(cv::Mat& img, AILIADetector* detector)
{
	int status;
	PRINT_OUT("input image shape: (%d, %d, %d)\n",
		img.cols, img.rows, img.channels());

	// inference
	PRINT_OUT("Start inference...\n");
	if (benchmark_support) {
		PRINT_OUT("BENCHMARK mode\n");
		for (int i = 0; i < BENCHMARK_ITERS; i++) {
			clock_t start = clock();
			status = ailiaDetectorCompute(detector, img.data,
				img.cols * 4, img.cols, img.rows,
				AILIA_IMAGE_FORMAT_BGRA, THRESHOLD, IOU);
			clock_t end = clock();
			if (status != AILIA_STATUS_SUCCESS) {
				PRINT_ERR("ailiaDetectorCompute failed %d\n", status);
				return -1;
			}
			PRINT_OUT("\tailia processing time %ld ms\n", ((end - start) * 1000) / CLOCKS_PER_SEC);
		}
	}
	else {
		status = ailiaDetectorCompute(detector, img.data,
			img.cols * 4, img.cols, img.rows,
			AILIA_IMAGE_FORMAT_BGRA, THRESHOLD, IOU);
		if (status != AILIA_STATUS_SUCCESS) {
			PRINT_ERR("ailiaDetectorCompute failed %d\n", status);
			return -1;
		}
	}

	status = plot_result(detector, img, COCO_CATEGORY);
	if (status != AILIA_STATUS_SUCCESS) {
		return -1;
	}

	cv::imwrite(save_image_path_support.c_str(), img);

	PRINT_OUT("Program finished successfully.\n");

	return AILIA_STATUS_SUCCESS;
}


static int recognize_from_video(AILIADetector* detector)
{
	// inference
	cv::VideoCapture capture;
	if (video_path_support == "0") {
		PRINT_OUT("[INFO] webcamera mode is activated\n");
		capture = cv::VideoCapture(0);
		if (!capture.isOpened()) {
			PRINT_ERR("[ERROR] webcamera not found\n");
			return -1;
		}
	}
	else {
		if (check_file_existance(video_path_support.c_str())) {
			capture = cv::VideoCapture(video_path_support.c_str());
		}
		else {
			PRINT_ERR("[ERROR] \"%s\" not found\n", video_path_support.c_str());
			return -1;
		}
	}

	while (1) {
		cv::Mat frame;
		capture >> frame;
		if ((char)cv::waitKey(1) == 'q' || frame.empty()) {
			break;
		}
		cv::Mat resized_img, img;
		adjust_frame_size(frame, resized_img, IMAGE_WIDTH, IMAGE_HEIGHT);
		cv::cvtColor(resized_img, img, cv::COLOR_BGR2BGRA);

		int status = ailiaDetectorCompute(detector, img.data,
			MODEL_INPUT_WIDTH * 4, MODEL_INPUT_WIDTH, MODEL_INPUT_HEIGHT,
			AILIA_IMAGE_FORMAT_BGRA, THRESHOLD, IOU);
		if (status != AILIA_STATUS_SUCCESS) {
			PRINT_ERR("ailiaDetectorCompute failed %d\n", status);
			return -1;
		}

		status = plot_result(detector, resized_img, COCO_CATEGORY, false);
		if (status != AILIA_STATUS_SUCCESS) {
			return -1;
		}
		cv::imshow("frame", resized_img);
	}
	capture.release();
	cv::destroyAllWindows();

	PRINT_OUT("Program finished successfully.\n");

	return AILIA_STATUS_SUCCESS;
}


//int main(int argc, char** argv)
//{
//	int status = argument_parser_support(argc, argv);
//	if (status != AILIA_STATUS_SUCCESS) {
//		return -1;
//	}
//
//	// env list
//	unsigned int env_count;
//	status = ailiaGetEnvironmentCount(&env_count);
//	if (status != AILIA_STATUS_SUCCESS) {
//		PRINT_ERR("ailiaGetEnvironmentCount Failed %d", status);
//		return -1;
//	}
//
//	int env_id = AILIA_ENVIRONMENT_ID_AUTO;
//	for (unsigned int i = 0; i < env_count; i++) {
//		AILIAEnvironment* env;
//		status = ailiaGetEnvironment(&env, i, AILIA_ENVIRONMENT_VERSION);
//		PRINT_OUT("env_id : %d type : %d name : %s\n", env->id, env->type, env->name);
//		if (args_env_id == env->id) {
//			env_id = env->id;
//		}
//		if (args_env_id == -1 && env_id == AILIA_ENVIRONMENT_ID_AUTO) {
//			if (env->type == AILIA_ENVIRONMENT_TYPE_GPU) {
//				env_id = env->id;
//			}
//		}
//	}
//	if (args_env_id == -1) {
//		PRINT_OUT("you can select environment using -e option\n");
//	}
//
//	// net initialize
//	AILIANetwork* ailia;
//	status = ailiaCreate(&ailia, env_id, AILIA_MULTITHREAD_AUTO);
//	if (status != AILIA_STATUS_SUCCESS) {
//		PRINT_ERR("ailiaCreate failed %d\n", status);
//		if (status == AILIA_STATUS_LICENSE_NOT_FOUND || status == AILIA_STATUS_LICENSE_EXPIRED) {
//			PRINT_OUT("License file not found or expired : please place license file (AILIA.lic)\n");
//		}
//		return -1;
//	}
//
//	AILIAEnvironment* env_ptr = nullptr;
//	status = ailiaGetSelectedEnvironment(ailia, &env_ptr, AILIA_ENVIRONMENT_VERSION);
//	if (status != AILIA_STATUS_SUCCESS) {
//		PRINT_ERR("ailiaGetSelectedEnvironment failed %d\n", status);
//		ailiaDestroy(ailia);
//		return -1;
//	}
//
//	//    PRINT_OUT("env_id: %d\n", env_ptr->id);
//	PRINT_OUT("selected env name : %s\n", env_ptr->name);
//
//	status = ailiaOpenStreamFile(ailia, model_support.c_str());
//	if (status != AILIA_STATUS_SUCCESS) {
//		PRINT_ERR("ailiaOpenStreamFile failed %d\n", status);
//		PRINT_ERR("ailiaGetErrorDetail %s\n", ailiaGetErrorDetail(ailia));
//		ailiaDestroy(ailia);
//		return -1;
//	}
//
//	status = ailiaOpenWeightFile(ailia, weight_support.c_str());
//	if (status != AILIA_STATUS_SUCCESS) {
//		PRINT_ERR("ailiaOpenWeightFile failed %d\n", status);
//		ailiaDestroy(ailia);
//		return -1;
//	}
//	const unsigned int flags = AILIA_DETECTOR_FLAG_NORMAL;
//
//	AILIADetector* detector;
//	status = ailiaCreateDetector(&detector, ailia,
//		AILIA_NETWORK_IMAGE_FORMAT_BGR,
//		AILIA_NETWORK_IMAGE_CHANNEL_FIRST,
//		AILIA_NETWORK_IMAGE_RANGE_UNSIGNED_INT8,
//		AILIA_DETECTOR_ALGORITHM_YOLOX,
//		COCO_CATEGORY.size(), flags);
//	if (status != AILIA_STATUS_SUCCESS) {
//		PRINT_ERR("ailiaCreateDetector failed %d\n", status);
//		ailiaDestroy(ailia);
//		return -1;
//	}
//
//	status = ailiaDetectorSetInputShape(detector, MODEL_INPUT_WIDTH, MODEL_INPUT_HEIGHT);
//	if (status != AILIA_STATUS_SUCCESS) {
//		PRINT_ERR("ailiaDetectorSetInputShape(w=%u, h=%u) failed %d\n",
//			MODEL_INPUT_WIDTH, MODEL_INPUT_HEIGHT, status);
//		ailiaDestroyDetector(detector);
//		ailiaDestroy(ailia);
//		return -1;
//	}
//
//	if (video_mode_support) {
//		status = recognize_from_video(detector);
//	}
//	else {
//		//status = recognize_from_image(detector);
//	}
//
//	ailiaDestroyDetector(detector);
//	ailiaDestroy(ailia);
//
//	return status;
//}