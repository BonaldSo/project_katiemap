#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <optional>
#include <memory>
#include <ctime>
#include <climits>
#include <cstdlib>

using std::vector;
using std::string;
using kvp = std::pair<string, int>;
using std::unique_ptr;

/*********************************************************************/
/* MyKatieMap
 *********************************************************************/


bool sortInt(kvp i, kvp j) {
    return i.second < j.second;
}

bool sortStr(kvp i, kvp j) {
    return i.first < j.first;
}

void sortkvp(std::vector<kvp>& kvps, int xy) {
    //std::cout << "once" << std::endl;
    for (int i = 0; i < kvps.size() - 1; i++) {
        for (int j = i + 1; j < kvps.size(); j++) {
	    bool (*sortxy) (kvp i, kvp j) = sortStr;
	    if (xy == 1) sortxy = sortInt;
            if (sortxy(kvps[j], kvps[i])) {
                std::pair<string, int> temp = kvps[i];
                kvps[i] = kvps[j];
                kvps[j] = temp;
            }
        }
    }
}

class MyKatieMap {
  private:
    string k;
    int value;
    int xy;
    //vector<kvp> lv;
    //vector<kvp> rv;
    MyKatieMap* left = NULL;
    MyKatieMap* right = NULL;
    MyKatieMap* createKM(vector<kvp>& kvps, int xys) {
      MyKatieMap* newkm = new MyKatieMap();
      int s = kvps.size();
      newkm->xy = xys;
      //std::cout << xy << std::endl;
      sortkvp(kvps, xys);
      kvp m = kvps[s/2];
      newkm->k = m.first;
      newkm->value = m.second;
      if (s <= 1) return newkm;
      vector<kvp> lv;
      vector<kvp> rv;
      for (auto i : kvps) {
        if (xys == 0 && i.first < m.first) {
          lv.push_back(i);      
        } else if (xys == 1 && i.second < m.second){
	  lv.push_back(i);
	} else {
          rv.push_back(i);
        }
      }
      newkm->left = createKM(lv, 1 - xys);
      newkm->right = createKM(rv, 1 - xys);
      return newkm;
    }

  public:

    MyKatieMap() {};

    // Constructor: Builds a new tree on the KVPs provided.
    MyKatieMap(const vector<kvp>& kvps) {
      vector<kvp> v(kvps);
      xy = 0;
      sortkvp(v, xy);
      int s = kvps.size();
      kvp m = v[s/2];
      k = m.first;
      value = m.second;
      vector<kvp> lv;
      vector<kvp> rv;
      if (s > 1) {
        for (auto i : kvps) {
          if (i.first < m.first) {
            lv.push_back(i);
         // } else if (xy == 1 && i.second < m.second){
         //   lv.push_back(i);
          } else {
            rv.push_back(i);
          }
        }
	//std::cout << lv.size() << rv.size() << std::endl;
	left = createKM(lv, 1 - xy);
	right = createKM(rv, 1 - xy);
      }
    };

   // MyKatieMap(const MyKatieMap& other) {
     // key = other.key;
  //    value = other.value;
    //  left = new MyKatieMap(other.left);
  //    right = new MyKatieMap(other.right);
   // }

    // all_points: Returns all KVPs in the tree, in the order they are
    //           visited in preorder traversal.
    vector<kvp> all_points() {
	    vector<kvp> kv;
	    if (left) {
	       vector<kvp> kl = left->all_points();
	       kv.insert(kv.end(), kl.begin(), kl.end());
	    } 
	    if (!left) {
	       kv.push_back({k, value});
	    } 
	    if (right) {
	       vector<kvp> kr = right->all_points();
               kv.insert(kv.end(), kr.begin(), kr.end());
	    }
	//    std::cout << r.size() << std::endl;
	    return kv;
    }

    // get: Returns the value, if any, stored at the given key. 
    //      Returns std::nullopt if there is no such key.
    //      Runs in O(sqrt(n)) time.
    std::optional<int> get(string key) {
      if (k == key) return value;
      if (!left && !right) return std::nullopt;	
      std::optional<int> la;
      std::optional<int> ra;
      if (xy == 1) { 
        la = left->get(key);
	ra = right->get(key);
	if (la != std::nullopt) {
	  return la;
	} else {
	  return ra;
	}
      } else if (xy == 0 && key < k) {
        return left->get(key);
      } else {
	return right->get(key);
      }
    };

    // range: Query points in the "rectangle" defined by start and end.
    //        Specifically, if start = (key_start, value_start) and
    //                         end = (key_end, value_end)
    //        returns all points in the range
    //              [key_start, key_end) x [value_start, value_end)
    //        Runs in O(sqrt(n) + s) time
    //              where s is the size of the returned vector.
    vector<kvp> range(kvp start, kvp end) {
	vector<kvp> kv;
	if (!left && !right) {
	  if (k < start.first || k >= end.first || value < start.second || value >= end.second) return kv;
	  kv.push_back({k, value});
	  return kv;
	}
	if (left && xy == 0 && k >= start.first) {
	  vector<kvp> kvl = left->range(start, end);
	 // vector<kvp> kvr = right->range(start, end);
	  kv.insert(kv.end(), kvl.begin(), kvl.end());
	//  kv.insert(kv.end(), lvr.begin(), kvr.end());
	}
	if (right && xy == 0 && k < end.first) {
        //  vector<kvp> kvl = left->range(start, end);
          vector<kvp> kvr = right->range(start, end);
         // kv.insert(kv.end(), kvl.begin(), kvl.end());
          kv.insert(kv.end(), kvr.begin(), kvr.end());
        } 
	if (left && xy == 1 && value >= start.second) {
          vector<kvp> kvl = left->range(start, end);
         // vector<kvp> kvr = right->range(start, end);
          kv.insert(kv.end(), kvl.begin(), kvl.end());
        //  kv.insert(kv.end(), lvr.begin(), kvr.end());
        }
        if (right && xy == 1 && value < end.second) {
        //  vector<kvp> kvl = left->range(start, end);
          vector<kvp> kvr = right->range(start, end);
         // kv.insert(kv.end(), kvl.begin(), kvl.end());
          kv.insert(kv.end(), kvr.begin(), kvr.end());
        }	
        return kv;	
    };

    // key_splits: Return all keys (strings) that are used for splits,
    //             in the order they are visited in preorder traversal.
    //             Runs in O(n) time.
    vector<string> key_splits() {
	    vector<string> s;
	    if (xy == 0 && (left || right)) s.push_back(k);
	    vector<string> sl;
	    vector<string> sr;
	    if (left) sl = left->key_splits();
	    if (right) sr = right->key_splits();
	    s.insert(s.end(), sl.begin(), sl.end());
	    s.insert(s.end(), sr.begin(), sr.end());
	    return s;
    }

    // value_splits: Return all values (ints) that are used for splits,
    //               in the order they are visited in preorder traversal.
    //               Runs in O(n) time.
    vector<int> value_splits() {
	vector<int> v;
	//std::cout << xy << std::endl;
	if (xy == 1 && (left || right)) v.push_back(value);
        vector<int> vl;
	vector<int> vr;
	if (left) vl = left->value_splits();
        if (right) vr = right->value_splits();
	v.insert(v.end(), vl.begin(), vl.end());
	v.insert(v.end(), vr.begin(), vr.end());
	return v;	
    };

    // Destructor: Destroys the kd-tree, cleaning up all resources.
    //             Must not leak memory.
    ~MyKatieMap() {
	    if (left) delete left;
	    if (right) delete right;
    }
};

/*********************************************************************/
/* main
 * ----
 *********************************************************************/

int main() {
  unique_ptr<MyKatieMap> km;
  std::srand(std::time(NULL));

  while (true) {
    string command;
    std::cin >> command;

    if (command == "n") {
      // Construct a new kd-tree.
      vector<kvp> points;
      int n;
      for (std::cin >> n; n > 0; --n) {
        string key;
 	int value;
        std::cin >> key >> value;
        points.push_back({key, value});
      }

      km = std::make_unique<MyKatieMap>(points);
    } else if (command == "g") {
      // Lookup a value by its key.
      if (km == nullptr) {
        std::cerr << "Error: not initialized" << std::endl;
        continue;
      }
      string key;
      std::cin >> key;
      std::optional<int> value = km->get(key);
      if (value == std::nullopt) {
        std::cout << "Not found" << std::endl;
      } else {
        std::cout << value.value() << std::endl;
      }
    } else if (command == "p") {
      // Print all elements in the tree.
      if (km == nullptr) {
        std::cerr << "Error: not initialized" << std::endl;
        continue;
      }
      vector<kvp> points = km->all_points();
      for (auto p = points.begin(); p != points.end(); ++p) {
        std::cout << p->first << " " << p->second << std::endl;
      }
    } else if (command == "r") {
      // Range search
      string k1, k2;
      int v1, v2;
      std::cin >> k1 >> v1 >> k2 >> v2;
      vector<kvp> points = km->range(std::make_pair(k1, v1), std::make_pair(k2, v2));
      for (auto p = points.begin(); p != points.end(); ++p) {
        std::cout << p->first << " " << p->second << std::endl;
      }
    } else if (command == "k") {
      // Print the string splits.
      if (km == nullptr) {
        std::cerr << "Error: not initialized" << std::endl;
        continue;
      }
      vector<string> keys = km->key_splits();
      for (auto k = keys.begin(); k != keys.end(); ++k) {
        std::cout << *k << std::endl;
      }
    } else if (command == "v") {
      // Print the integer splits.
      if (km == nullptr) {
        std::cerr << "Error: not initialized" << std::endl;
        continue;
      }
      vector<int> values = km->value_splits();
      for (auto v = values.begin(); v != values.end(); ++v) {
        std::cout << *v << std::endl;
      }
    } else if (command == "q" || std::cin.eof()) {
      // Quit.
      break;
    } else {
      std::cerr << "Error: invalid command" << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
