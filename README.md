
# Project_1: Clustering
This repository contains an exact (continuous) Fréchet Distance (FD) algorithm and an approximation Geometric Edit Distance (GED) algorithm. Given two polygonal curves $P$ and $Q$ on the Euclidean plane, these algorithms return the similarity between $P$ and $Q$ based on FD and GED.

Note that we implemented these algorithms to use them in the experiments for the main research and development of our project.

# Requirements
- [boost](https://www.boost.org/)
- [CGAL](https://www.cgal.org/)

Note that CGAL has the dependency on the boost library since it uses the boost graph library. Thus, we recommend to download both libraries together following the package manager of CGAL.

# Algorithms
## Fréchet Distance Algorithm
The _Fréchet Distance_ (FD) is one of the most well-known similarity metrics between two (polygonal) curves. It is often described metaphorically as the minimum length of a leash required for a person to walk a dog along two separate paths, where the person walks along one curve and the dog along the other.
Formally, The FD for two polygonal curves $P$ and $Q$ is defined as the infimum over all reparameterizations $\alpha$ and $\beta$ of $[0,1]$ of the maximum over all $t\in [0,1]$ of the distance between $P(\alpha(t)$ and $Q(\beta(t)$, i.e., $FD(P,Q)=\text{inf}_{\alpha, \beta}\text{max}_{t\in [0,1]}\{ d(P(\alpha(t)),Q(\beta(t)) \}$, where $d(\cdot, \cdot)$ is the distance function.
Our implementation is based on the work of [[Alt and Godau 95]](https://www.worldscientific.com/doi/abs/10.1142/S0218195995000064). In particular, we implement the non-parallel version of Alt and Godau's. We implement the decision version and the optimization version of the algorithm.



## Approximation Geometric Edit Distance Algorithm
The _Geometric Edit Distance_ (GED) is one of the well-known similarity metrics between two curves. It can be handled as the special case of the (string) edit distance. The GED for two curves $P$ and $Q$ is defined as the minimum sum of the distance between the matched points of $P$ and $Q$. For the matching, we allow some unmatched points with the penalty cost. Formally, $GED(P,Q)=min_{M \in \mathcal M}\{ \sum_{(i,j)\in M} d(P(i),Q(j)) + |\Gamma(M)| \}$, where $\mathcal M$ is the set of all possible matching, $\Gamma(M)$ is the set of unmatched points of a matching $M$, and $d(\cdot, \cdot)$ is the distance function.
We implement the $\sqrt n$-approximation GED algorithm, where $n$ is the length of the given curves. Our implementation is based on the work of [[Fox and Li 22]](https://link.springer.com/article/10.1007/s00453-022-00966-4).



# How to run(On Mac)
We use [brew](https://brew.sh/) to install the requirements.
1. install(terminal)
    ```
   brew install cmake
   brew install cgal
   ```
   
2. (If you did not clone this repository, then this step may be required.) Configure CMakeLists.txt(Refer to CMakeLists.txt at the root folder)

4. create and move to build folder(on Project folder)
   ```
   mkdir build
   cd build
   ```
5. run cmake
   ```
   cmake ..
   ```
6. run make to create the execution
   ```
   make
   ```
   
7. run execution.(named Project1)
   ```
   ./Project1
   ```
   
