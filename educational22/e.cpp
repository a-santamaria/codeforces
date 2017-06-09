#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct TSegmentTree {
  struct Node {
    int sum;
    Node* left;
    Node* right;

    Node() {
      sum = 0;
      left = right = NULL;
    }

    Node(int sum) : sum(sum) {
      left = right = NULL;
    }
  };

  vector<Node*> versions;
  int N;

  Node* build(int lo, int hi) {
    if(lo == hi) {
      return new Node(0);
    }
    int mid = (lo + hi) / 2;
    Node* p = new Node(0);

    p->left = build(lo, mid);
    p->right = build(mid+1, hi);
    return p;
  }

  void build() {
    Node* root = build(0, N-1);
    versions.push_back( root );
  }

  TSegmentTree(int n) {
    N = n;
    build();
  }

  Node* update(int lo, int hi, int ind, Node* curr) {
    // cout << lo << " " << hi << " = " << curr->sum << "\t";
    if(curr->left == NULL) {
        // cout << "voy a cambiar"<< endl;
        return new Node( curr->sum == 0 ? 1 : 0);
    }
    int mid = (lo + hi) / 2;
    if(ind <= mid) {
      Node* n = new Node();
      n->right = curr->right;
      n->left = update(lo, mid, ind, curr->left);
      n->sum = n->left->sum + n->right->sum;
      return n;
    }
    else {
      Node* n = new Node();
      n->left = curr->left;
      n->right = update(mid+1, hi, ind, curr->right);
      n->sum = n->left->sum + n->right->sum;
      return n;
    }
  }
  
  Node* update(int ind) {
    return update(0, N-1, ind, versions[versions.size()-1]);
  }

  int getSum(int lo, int hi, int ql, int qh, Node* curr) {
    if(curr->left == NULL) return curr->sum;
    if(ql <= lo && qh >= hi) {
      return curr->sum;
    }
    int sum = 0;
    int mid = (lo + hi) / 2;
    if(ql <= mid) {
      sum += getSum(lo, mid, ql, qh, curr->left);
    }
    if(qh > mid) {
      sum += getSum(mid+1, hi, ql, qh, curr->right);
    }
    return sum;
  }

  int getSum(int ql, int qh, int ver) {
    ver = N-ver;
    return getSum(0, N-1, ql, qh, versions[ver]);
  }

  void printTree(Node* n) {
    if(n->left != NULL) printTree(n->left);
    if(n->left == NULL && n->right == NULL)
      std::cout << n->sum;
    if(n->right != NULL) printTree(n->right);
  }

  void printAll() {
    for(int i = 0; i < versions.size(); i++) {
      printTree(versions[i]);
      cout<<"\n";
    }
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n, k, q;

  cin >> n >> k;
  int arr[n];

  for (int i = 0; i < n; i++) {
    cin >> arr[i];
  }
  vector< queue<int> > pos(100005, queue<int>());

  TSegmentTree st(n);
  for (int i = n-1; i >= 0; i--) {
    if (pos[arr[i]].size() == k) {
      int indexTop = pos[arr[i]].front();
      pos[arr[i]].pop();
      // remove indexTop
      TSegmentTree::Node* currRoot = st.update(indexTop);
      pos[arr[i]].push(i);
      // add i
      st.versions.push_back( st.update(0, n-1, i, currRoot) );
    } else {
      pos[arr[i]].push(i);
      // add i
      st.versions.push_back(st.update(i));
    }
  }
  // st.printAll();
  cin >> q;
  int lo, hi, last = 0;
  for(int i = 0; i < q; i++) {
    cin >> lo >> hi;
    lo = ((lo + (last)) % n);
    hi = ((hi + (last)) % n);
    if(lo > hi) swap(lo, hi);
    // cout << "real " << lo << " "<< hi << endl; 
    last = st.getSum(lo, hi, lo);
    cout << last << '\n';
  }
  return 0;
}