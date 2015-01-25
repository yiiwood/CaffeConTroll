//
//  ConvolutionBridge.h
//  moka
//
//  Created by Ce Zhang on 1/12/15.
//  Copyright (c) 2015 Hazy Research. All rights reserved.
//

#include "PhysicalOperator.h"
#include "AbstractBridge.h"

#ifndef moka_Convolution_Bridge_h
#define moka_Convolution_Bridge_h

enum ConvolutionBridgeType {
  CPU_CONV_LOWERINGTYPE1 = 0
};

/**
 * A ConvolutionBridge object connects two Layers, each of which is a (Data, Model, Gradient) triple.
 * A ConvolutionBridge contains two functions:
 *   - forward()
 *   - backward()
 * In the template,
 *   - LAYERTYPE defines {GPU, CPU} x {Types of lowering}
 *   - FUNC defines the non-linear function that will be applied to the output
 *     - {No func, TANH}
 *     - TODO: We need ReLU, etc.
 **/
template
<ConvolutionBridgeType LAYERTYPE, NonLinearFunction FUNC,
  typename InputLayerDataType, LayoutType InputLayerLayout,
  typename OutputLayerDataType, LayoutType OutputLayerLayout>
class ConvolutionBridge : public AbstractBridge<InputLayerDataType, InputLayerLayout, OutputLayerDataType, OutputLayerLayout> {
  public:

    typedef Layer<InputLayerDataType, InputLayerLayout> InputLayerType;
    typedef Layer<OutputLayerDataType, OutputLayerLayout> OutputLayerType;

    ConvolutionBridge(InputLayerType * const _p_input_layer,
        OutputLayerType * const _p_output_layer) {
      std::cerr << "ERROR: Using a bridge with unsupported Layout or DataType." << std::endl;
      assert(false);
    }

    void forward() {
      std::cerr << "ERROR: Using a bridge with unsupported Layout or DataType." << std::endl;
      assert(false);
    }

    void backward() {
      std::cerr << "ERROR: Using a bridge with unsupported Layout or DataType." << std::endl;
      assert(false);
    }
};

/******
 * Specializations
 */
template<typename DataType, NonLinearFunction FUNC>
class ConvolutionBridge<CPU_CONV_LOWERINGTYPE1, FUNC, DataType, Layout_CRDB, DataType, Layout_CRDB>
: public AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB> {
  public:
    /* Re-declare these member fields so that they don't have to be resolved using vtable lookups */
    using PhysicalOperator::report_forward_constructor;
    using PhysicalOperator::report_forward_last_transfer;
    using PhysicalOperator::report_forward_history;
    using PhysicalOperator::run_with_n_threads;
    using PhysicalOperator::report_backward_updateweight_constructor;
    using PhysicalOperator::report_backward_updateweight_last_transfer;
    using PhysicalOperator::report_backward_updateweight_history;

    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::i1R;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::i1C;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::i1D;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::i1B;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::i2R;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::i2C;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::i2D;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::i2B;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::oR;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::oC;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::oD;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::oB;

    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::p_input_layer;
    using AbstractBridge<DataType, Layout_CRDB, DataType, Layout_CRDB>::p_output_layer;

    /* Re-declare these typedefs */
    typedef Layer<DataType, Layout_CRDB> InputLayerType;
    typedef Layer<DataType, Layout_CRDB> OutputLayerType;

    float stepsize;

    Scanner<DataType, Layout_CRDB, FUNC> * p_forward_applyfunc_scanner;

    Connector<DataType, Layout_CRDB, DataType, Layout_CRDB, LOWERING_TYPE1> *
      p_forward_lower_connector;

    LogicalCube<DataType, Layout_CRDB> * p_forward_lowered_data;

    LoweringConfig lconfig_forward;

    Kernel<DataType, Layout_CRDB, DataType, Layout_CRDB, DataType, Layout_CRDB,
      Kernel_GEMM_OpenBlas, KernelConfig_GEMM_NOTRANS_NOTRANS> * p_forward_gemm_kernel;

    LogicalCube<DataType, Layout_CRDB> * p_backward_outputgrad;
    LogicalCube<DataType, Layout_CRDB> * p_backward_inputgrad;

    Kernel<DataType_SFFloat, Layout_CRDB, DataType_SFFloat, Layout_CRDB, DataType_SFFloat,
      Layout_CRDB, Kernel_ELEMENTWISEMUL_CPU, KernelConfig_NONE> * p_backward_element_mul_kernel;

    Kernel<DataType_SFFloat, Layout_CRDB, DataType_SFFloat, Layout_CRDB, DataType_SFFloat,
      Layout_CRDB, Kernel_GEMM_OpenBlas, KernelConfig_GEMM_NOTRANS_TRANS> * p_backward_gemm_updateweight_kernel;

    Kernel<DataType_SFFloat, Layout_CRDB, DataType_SFFloat, Layout_CRDB, DataType_SFFloat,
      Layout_CRDB, Kernel_GEMM_OpenBlas, KernelConfig_GEMM_TRANS_NOTRANS> * p_backward_gemm_updategrad_kernel;

    ConvolutionBridge(InputLayerType * const _p_input_layer,
        OutputLayerType * const _p_output_layer);

    void forward();

    void backward();
};

#include "ConvolutionBridge_impl.hxx"

#endif
