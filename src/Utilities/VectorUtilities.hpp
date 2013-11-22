#ifndef VECTORUTILITIES_HPP
#define VECTORUTILITIES_HPP

#include <vector>
#include <algorithm>

//return a vector of indices, sorted on v
template <typename T>
std::vector<unsigned int> sortIndices(const std::vector<T> &v){

      std::vector<unsigned int> index(v.size());
      for (unsigned int i = 0; i != index.size(); ++i){
        index[i] = i;
      }

      // sort indexes based on comparing values in v
      std::sort(index.begin(), index.end(),
                [&v](unsigned int i1, unsigned int i2) {
                        return v[i1] < v[i2];
                 });

      return index;
}


#endif
