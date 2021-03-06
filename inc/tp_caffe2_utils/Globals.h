#ifndef tp_caffe2_utils_Globals_h
#define tp_caffe2_utils_Globals_h

#include "tp_utils/StringID.h"

#if defined(TP_CAFFE2_UTILS_LIBRARY)
#  define TP_CAFFE2_UTILS_SHARED_EXPORT TP_EXPORT
#else
#  define TP_CAFFE2_UTILS_SHARED_EXPORT TP_IMPORT
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpragma-system-header-outside-header"
#pragma GCC system_header

#undef slots

#ifdef TP_CUDA
#include <caffe2/core/context_gpu.h>
#endif

#include <caffe2/core/init.h>
#include <caffe2/core/operator.h>
#include <caffe2/core/operator_gradient.h>

#include <caffe2/core/context.h>

#pragma GCC diagnostic pop

//##################################################################################################
//! Utils for working with Caffe2 in c++
namespace tp_caffe2_utils
{

//##################################################################################################
#ifdef TP_CUDA
constexpr auto defaultDeviceType = caffe2::PROTO_CUDA;
#else
constexpr auto defaultDeviceType = caffe2::PROTO_CPU;
#endif

//##################################################################################################
void initCaffe2(int argc, char *argv[]);

}

#endif
