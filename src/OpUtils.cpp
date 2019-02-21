#include "tp_caffe2_utils/OpUtils.h"
#include "tp_caffe2_utils/FillOps.h"
#include "tp_caffe2_utils/ModelDetails.h"
#include "tp_caffe2_utils/Ops.h"

namespace tp_caffe2_utils
{

//##################################################################################################
void removeOpByOutput(caffe2::NetDef& net,const std::string& opOutputName)
{
  for(int i=0; i<net.op_size(); i++)
  {
    auto op = net.op(i);

    bool remove=false;
    for(int j=0; j<op.output_size(); j++)
    {
      if(op.output(j) == opOutputName)
      {
        remove = true;
        break;
      }
    }

    if(remove)
    {
      op.Clear();
      break;
    }
  }
}

//##################################################################################################
void addGradientOps(ModelDetails& model)
{
  for (size_t i=model.gradientOps.size()-1; i<model.gradientOps.size(); i--)
  {
    auto op = model.gradientOps[i];

    std::vector<caffe2::GradientWrapper> output(size_t(op->output_size()));
    for (size_t j = 0; j < output.size(); j++)
    {
      output[j].dense_ = op->output(int(j)) + "_grad";
      model.dataBlobNames.push_back(output[j].dense_);
    }
    caffe2::GradientOpsMeta meta = caffe2::GetGradientForOp(*op, output);

    auto grad = model.trainNet.add_op();
    grad->CopyFrom(meta.ops_[0]);
    grad->set_is_gradient_op(true);
  }
}

//##################################################################################################
void addApplyGradientsOps_simple(ModelDetails& model, float lr)
{
  addConstantFillOp(model.initTrainNet, {1}, lr, "lr");

  for(const auto& name : model.learntBlobNames)
    addWeightedSumOP(model.trainNet, name, "one", name + "_grad", "lr", name);
}

//##################################################################################################
void addApplyGradientsOps_momentum(ModelDetails& model, float lr, float momentum)
{
  addConstantFillOp(model.initTrainNet, {1}, lr, "lr");
  addConstantFillOp(model.initTrainNet, {1}, momentum, "momentum");

  for(const auto& name : model.learntBlobNames)
  {
    tp_caffe2_utils::addConstantFillOp_copy(model.initTrainNet, name, 0.0f, name + "_momentum");

    addWeightedSumOP(model.trainNet, name              , "one"     , name + "_grad"    , "lr" , name              );
    addWeightedSumOP(model.trainNet, name              , "one"     , name + "_momentum", "lr" , name              );
    addWeightedSumOP(model.trainNet, name + "_momentum", "momentum", name + "_grad"    , "one", name + "_momentum");
  }
}

}