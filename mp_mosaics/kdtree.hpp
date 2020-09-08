/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
     if(first[curDim] < second[curDim]){
       return true;
     }
     else if(first[curDim] > second[curDim]){
       return false;
     }
    return first < second;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
     double euclidPtoT = 0.0;
     double euclidPtoC = 0.0;

     for(int i = 0; i < Dim; i++){
       euclidPtoT += (target[i] - potential[i])*(target[i] - potential[i]);
       euclidPtoC += (target[i]-currentBest[i])*(target[i]-currentBest[i]);
     }
     if(euclidPtoT < euclidPtoC){
       return true;
     }
     else if(euclidPtoT > euclidPtoC){
       return false;
     }
     return potential < currentBest;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
     size = 0;
     if(newPoints.empty()){ //check empty case
       root = NULL;
     }
     else {
       int dimension = 0;
       vector<Point<Dim>> tempPoints; //copy into new vector so we can edit the values
       for (unsigned long i = 0; i < newPoints.size(); i++) {
       tempPoints.push_back(newPoints[i]);
       }
       root = split(tempPoints, 0, tempPoints.size() - 1, dimension);
     }
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::split(vector<Point<Dim>>& newPoints, int left, int right, int dimension){
  if(left > right){
    return NULL;
  }
  int medianIndex = (left + right)/2;
  Point<Dim> median = select(newPoints, left, right, medianIndex, dimension);
  KDTreeNode* subRoot = new KDTreeNode(median);
  size += 1;
  subRoot->left = split(newPoints, left, medianIndex - 1, (dimension + 1) % Dim);
  subRoot->right = split(newPoints, medianIndex + 1, right, (dimension + 1) % Dim);
  return subRoot;
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& newPoints, int left, int right, int pivotIndex, int dimension){
  Point<Dim> pivotValue = newPoints[pivotIndex];
  swap(newPoints, pivotIndex, right);
  int storeIndex = left;
  for(int i = left; i < right; i++){
    if(smallerDimVal(newPoints[i], pivotValue, dimension)){
      swap(newPoints, storeIndex, i);
      storeIndex++;
    }
  }
  swap(newPoints, right, storeIndex);
  return storeIndex;
}

template <int Dim>
void KDTree<Dim>::swap(vector<Point<Dim>>& newPoints, int leftswap, int rightswap){
  Point<Dim> c = newPoints[leftswap];
  newPoints[leftswap] = newPoints[rightswap];
  newPoints[rightswap] = c;
}

template <int Dim>
Point<Dim> KDTree<Dim>::select(vector<Point<Dim>>& newPoints, int left, int right, int k, int dimension){
  if(left == right){
    return newPoints[left];
  }
  int pivotIndex = (left + right)/2;
  pivotIndex = partition(newPoints, left, right, pivotIndex, dimension);
  if(k == pivotIndex) {
    return newPoints[k];
  }
  else if(k < pivotIndex){
    return select(newPoints, left, pivotIndex - 1, k, dimension);
  }
  else{
    return select(newPoints, pivotIndex + 1, right, k, dimension);
  }
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  copy_(root, other->root);
  size = other.size;
}

template <int Dim>
void KDTree<Dim>::copy_(KDTreeNode*& curr, KDTreeNode*& other){
  if(other == NULL){
    return;
  }
  curr = new KDTreeNode(other->print);
  copy_(curr->left, other->left);
  copy_(curr->right, other->right);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  delete(root);
  copy_(root, rhs->root);
  size = rhs.size;
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   delete(root);
}

template <int Dim>
void KDTree<Dim>::delete_(KDTreeNode*& subroot){
  if(subroot == NULL){
    return;
  }
  if(subroot->left){
    delete_(subroot->left);
  }
  if(subroot->right){
    delete_(subroot->right);
  }
  delete subroot;
  subroot = NULL;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
     //variables needed
     return findNearestNeighbor_(query, root, 0);
}

template <int Dim> //helper function for findNearestNeighbor
Point<Dim> KDTree<Dim>::findNearestNeighbor_(const Point<Dim>&query, KDTreeNode* subRoot, int dimension) const{
  //recursive depth first search to find the leaf node with the same splitting plane as the target
  bool flag;
  Point<Dim> currentBest = subRoot->point;
  if(subRoot->left == NULL & subRoot->right == NULL){ //base case
    return subRoot->point;
  }
  //if less, traverse left
  if(smallerDimVal(query, subRoot->point, dimension)){
    if (subRoot->left == NULL) {
      currentBest = findNearestNeighbor_(query, subRoot->right, (dimension + 1) % Dim);
    }
    if (subRoot->left != NULL) {
      currentBest = findNearestNeighbor_(query, subRoot->left, (dimension + 1) % Dim);
    }
    flag = true;
  }
  else {
    if (subRoot->right == NULL) {
      currentBest = findNearestNeighbor_(query, subRoot->left, (dimension + 1) % Dim);
    }
    if (subRoot->right != NULL) {
      currentBest = findNearestNeighbor_(query, subRoot->right, (dimension + 1) % Dim);
    }
    flag = false;
  }
  if(shouldReplace(query, currentBest, subRoot->point)){
    currentBest = subRoot->point;
  }

  //calculate radius
  double radius = 0;
  for(int i = 0; i < Dim; i++){
    radius += (currentBest[i] - query[i])*(currentBest[i] - query[i]);
  }
  double distance = (subRoot->point[dimension] - query[dimension])*(subRoot->point[dimension] - query[dimension]);
  if(radius >= distance) {
    if (flag == true && subRoot->right != NULL) {
      Point<Dim> pointCheck = findNearestNeighbor_(query, subRoot->right, (dimension + 1) % Dim);
      if (shouldReplace(query, currentBest, pointCheck)) {
        currentBest = pointCheck;
      }
    }
  else if (flag == false && subRoot->left != NULL) {
    Point<Dim> pointCheck = findNearestNeighbor_(query, subRoot->left, (dimension + 1) % Dim);
    if (shouldReplace(query, currentBest, pointCheck)) {
      currentBest = pointCheck;
    }
  }
 }
  return currentBest;
}
