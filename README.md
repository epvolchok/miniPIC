# Mini Particle-in-Cell code for plasma simulations

(⚠ This project is under active development)

- The [Yee](https://ieeexplore.ieee.org/abstract/document/1138693) solver of Maxwell equations:
  
  Field components are separated in time and space, forming two independent groups $`[E_x, E_y, B_z]`$, $`[B_x, B_y, B_z]`$;
- Periodic boundary conditions;
- Visualisation with Python, matplotlib.

## Structure of the results

```
data
├── Fields
|   ├── 2D # binary files recorded at the diagnostic step
           # the entire computational domain
|   |   ├── ElectricFields
|   |   ├── MagneticFields
|   |   ├── Current
|   ├── 1D # text files recorded at each simulation step
           # slices along X and Y
|   |   ├── ElectricFields
|   |   ├── MagneticFields
|   |   ├── Current
|   ├── Point # point diagnostic at each step, txt
```


## Dependecies
- C++-20
- CMake 3.10+
- matplotlib
- numpy
