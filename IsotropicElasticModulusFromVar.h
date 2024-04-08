#pragma once

#include "ComputeElasticityTensorBase.h"

/**
 * ComputeIsotropicElasticityTensor defines an elasticity tensor material for
 * isotropic materials.
 */
template <bool is_ad, typename T>
class IsotropicElasticModulusFromVarTempl : public ComputeElasticityTensorBaseTempl<is_ad, T>
{
public:
  static InputParameters validParams();

  IsotropicElasticModulusFromVarTempl(const InputParameters & parameters);

  virtual void initialSetup() override { residualSetup(); }

  virtual void residualSetup() override;

protected:
  virtual void computeQpElasticityTensor() override;

  /// Elastic constants
  const VariableValue & _bulk_modulus;
  const VariableValue & _shear_modulus;

  /// Individual elasticity tensor
  T _Cijkl;


  /// Effective stiffness of the element: function of material properties
  Real _effective_stiffness_local;

  using ComputeElasticityTensorBaseTempl<is_ad, T>::name;
  using ComputeElasticityTensorBaseTempl<is_ad, T>::_elasticity_tensor_name;
  using ComputeElasticityTensorBaseTempl<is_ad, T>::issueGuarantee;
  using ComputeElasticityTensorBaseTempl<is_ad, T>::isParamValid;
  using ComputeElasticityTensorBaseTempl<is_ad, T>::_elasticity_tensor;
  using ComputeElasticityTensorBaseTempl<is_ad, T>::_qp;
  using ComputeElasticityTensorBaseTempl<is_ad, T>::_effective_stiffness;
};

typedef IsotropicElasticModulusFromVarTempl<false, RankFourTensor> IsotropicElasticModulusFromVar;
typedef IsotropicElasticModulusFromVarTempl<true, RankFourTensor> ADIsotropicElasticModulusFromVar;