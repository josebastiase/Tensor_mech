
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 1
  ny = 99 # So that the number of nodes = 100
  xmin = -1
  xmax = 1
  ymin = 0
  ymax = 100
[]

# The easiest way to use PorousFlow, is using actions/dictator
[GlobalParams]
 displacements = 'disp_x disp_y'
 PorousFlowDictator = dictator
 multiply_by_density = false
 biot_coefficient = 0.9
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [porepressure]
    scaling = 1E11
  []
[]

[AuxVariables]
  [porosity]
    family = MONOMIAL
    order = CONSTANT
  []
  [bulk]
    family = MONOMIAL
    order = CONSTANT
  []
  [shear]
    family = MONOMIAL
    order = CONSTANT
  []
  [perm]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[ICs]
  [porosity]
    type = RandomIC
    variable = porosity
    min = 0.25
    max = 0.275
    seed = 0
  []
  [bulk]
    type = RandomIC
    variable = bulk
    min = 1E9
    max = 1.5E9
    seed = 0
  []
  [shear]
    type = RandomIC
    variable = shear
    min = 1E8
    max = 1.5E8
    seed = 0
  []
  [perm]
    type = RandomIC
    variable = perm
    min = 1E-4
    max = 1E-3
    seed = 0
  []
[]


# Boundary conditions. System open only at the top
[BCs]
  [no_x_disp]
    type = DirichletBC
    preset = true
    variable = disp_x
    value = 0
    boundary = 'left right'
  []
  [no_y_disp]
    type = DirichletBC
    preset = true
    variable = disp_y
    value = 0
    boundary = 'bottom'
  []
  [top_drained]
    type = DirichletBC
    variable = porepressure
    value = 0
    boundary = 'top'
  []
  [./top_load]
    type = NeumannBC
    variable = disp_y
    value = -1000
    boundary = 'top'
  []
[]

[Modules]
  [FluidProperties]
    [the_simple_fluid]
      type = SimpleFluidProperties
      thermal_expansion = 0.0
      bulk_modulus = 2.2E9
      viscosity = 1E-3
      density0 = 1000.0
    []
  []
[]

[PorousFlowBasicTHM]
  coupling_type = HydroMechanical
  displacements = 'disp_x disp_y'
  porepressure = porepressure
  gravity = '0 0 0'
  fp = the_simple_fluid
[]

[Materials]
  [elasticity_tesnors]
    type = IsotropicElasticModulusFromVar
    bulk_modulus = bulk
    shear_modulus = shear
  []
  [strain]
    type = ComputeSmallStrain
  []
  [stress]
    type = ComputeLinearElasticStress
  []
  [porosity]
    type = PorousFlowPorosityConst
    porosity = porosity
  []
  [biot_modulus]
    type = PorousFlowConstantBiotModulus
    solid_bulk_compliance = 1E-10
    fluid_bulk_modulus = 2E9
  []
  [permeability]
    type = PorousFlowPermeabilityConstFromVar
    perm_xx = perm
    perm_yy = perm
    perm_zz = perm
  []
[]


[Preconditioning]
  [default]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  solve_type = Newton
  start_time = -1
  end_time = 1000
  dt = 1E1
[]

[Outputs]
  [./csv]
    type = CSV
    sync_times = '0 10 50 200 1000'
    sync_only = true
  [../]
[]
