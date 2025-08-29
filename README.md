# The k-labelled spanning forest problem

In this repo you find the source code and instances for the paper "The k-labeled spanning forest problem: instance analysis and effective heuristic solution" (see the citation below).

## Source code

The code is contained in the subdirectory `src`. You need the [Boost libraries](https://www.boost.org) to compile. To build:
```bash
git clone https://github.com/mrpritt/klsf
cd klsf/src
git submodule init
git submodule update
cmake .
make -j
```

## Running

The two instances sets are contained in the subdirectory `data`. Both programs have default setting that correspond to those in the paper, so you can simply run them, providing the name of an instance. For example: 
```bash
./cbfs ../data/set1/100_990_25_3_1
./exhaust ../data/set1/100_990_25_3_1
```
To see the options, run with "--help".

## How to cite

```bibtex
@InProceedings{Ritt/2024,
  author = 	  {Marcus Ritt},
  title = 	  {The $k$-labeled spanning forest problem: instance analysis and effective heuristic solution},
  booktitle = {Anais do LVI Simpósio Brasileiro de Pesquisa Operacional},
  volume =	  {56},
  year = 	 {2024},
  address =  {Fortaleza, Brasil},
  doi = 	 {10.59254/sbpo-2024-193454}
}
```
