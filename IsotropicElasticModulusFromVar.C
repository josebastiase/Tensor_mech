#include "IsotropicElasticModulusFromVar.h"

registerMooseObject("TensorMechanicsApp", IsotropicElasticModulusFromVar);
registerMooseObject("TensorMechanicsApp", ADIsotropicElasticModulusFromVar);

template <bool is_ad, typename T>
InputParameters IsotropicElasticModulusFromVarTempl<is_ad, T>::validParams()
{
  InputParameters params = ComputeElasticityTensorBase::validParams();
  params.addClassDescription("Compute a isotropic, heterogeneous elasticity tensor using the "
			     "spatially-varying bulk_modulus and shear_modulus values.");
  params.addRequiredCoupledVar("bulk_modulus", "The bulk modulus for the material, which can "
			       "vary throughout the mesh");
  params.addRequiredCoupledVar("shear_modulus", "The shear modulus of the material, which can "
			       "vary throughout the mesh");
  return params;
}

template <bool is_ad, typename T>
IsotropicElasticModulusFromVarTempl<is_ad, T>::IsotropicElasticModulusFromVarTempl(const InputParameters & parameters)
  : ComputeElasticityTensorBaseTempl<is_ad, T>(parameters),
    _bulk_modulus(coupledValue("bulk_modulus")),
    _shear_modulus(coupledValue("shear_modulus"))
{
   issueGuarantee(_elasticity_tensor_name, Guarantee::ISOTROPIC);
   issueGuarantee("effective_stiffness", Guarantee::ISOTROPIC);
}

template <bool is_ad, typename T>
void IsotropicElasticModulusFromVarTempl<is_ad, T>::computeQpElasticityTensor()
{
 // Fill elasticity tensor

   const Real E = (9 * _bulk_modulus[_qp] * _shear_modulus[_qp]) / (3 * _bulk_modulus[_qp] + _shear_modulus[_qp]);
   const Real nu = (3 * _bulk_modulus[_qp] - 2 * _shear_modulus[_qp]) / (2 * (3 * _bulk_modulus[_qp] + _shear_modulus[_qp]));
   _effective_stiffness_local =
        std::max(std::sqrt((E * (1 - nu)) /
                           ((1 + nu) * (1 - 2 * nu))),
                 std::sqrt(E / (2 * (1 + nu))));
  _Cijkl.fillSymmetricIsotropicEandNu(E, nu);
  _elasticity_tensor[_qp] = _Cijkl;
  _effective_stiffness[_qp] = _effective_stiffness_local;
}

template class IsotropicElasticModulusFromVarTempl<false, RankFourTensor>;
template class IsotropicElasticModulusFromVarTempl<true, RankFourTensor>;