#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <queue>
#include <random>

using namespace std;

struct node {
    int num, x, y;
};


struct solution {
    vector<int> path;
    double sum, F, P;
    int gen;
};


int curGen = 1;
bool isFound = false;
const int maxGen = 10000;
const int start = 1;
int packNum = 20;
double mutationP = 0.04;
double crossP = 0.7;
double curCost = 0;
int Hayflick = 6;
vector<node> city;
vector<int> num;
vector<solution> pack;
unordered_map<int, unordered_map<int, double> > dis;
queue<double> record;

regex pattern("^(\\d+) (\\d+) (\\d+)$");


double distanceBetween(node &a, node &b) {
    int x = a.x - b.x;
    int y = a.y - b.y;
    return sqrt(x * x + y * y);
}


double sum(vector<int> &v) {
    double sum = 0;
    for (auto it = v.begin() + 1; it != v.end(); it++) {
        sum += dis[*(it - 1)][*it];
    }
    return sum;
}


bool cmp(const solution &a, const solution &b) {
    return a.sum < b.sum;
}


void initData() {
    ifstream f("/Users/pavel/Desktop/tsp_783_1", ios::in);


    if (!f) {
        cout << "File Not Found" << endl;
    }
    string s;
    while (getline(f, s)) {
        if (regex_match(s, pattern)) {
            sregex_iterator it(s.begin(), s.end(), pattern);
            city.push_back(node{stoi(it->str(1)), stoi(it->str(2)), stoi(it->str(3))});
        }
    }
    f.close();


    for (auto it = city.begin(); it != city.end(); it++) {
        unordered_map<int, double> temp;
        for (auto iterator = city.begin(); iterator != city.end(); iterator++) {
            temp[iterator->num] = distanceBetween(*it, *iterator);
        }
        dis[it->num] = temp;
    }

    for (auto it = city.begin(); it != city.end(); it++) {
        if (it->num != start) {
            num.push_back(it->num);
        }
    }
}

void initPack(int gen) {
    auto rd = random_device{};
    auto rng = default_random_engine {rd()};
    for (int i = 0; i < packNum; i++) {
        solution temp;
        temp.path.push_back(start);
        shuffle(num.begin(), num.end(), rng);
        for (auto it = num.begin(); it != num.end(); it++) {
            temp.path.push_back(*it);
        }
        temp.path.push_back(start);

        temp.gen = gen;
        temp.sum = sum(temp.path);
        pack.push_back(temp);
    }
    if (gen == 1) {
        solution greed;
        unordered_map<int, bool> isVisited;
        greed.path.push_back(start);
        isVisited[start] = true;
        for (int i = 0; i < num.size(); i++) {
            double min = -1;
            for (auto it = dis[greed.path.back()].begin(); it != dis[greed.path.back()].end(); it++) {
                if (isVisited.count(it->first) == 0 && (min == -1 || it->second < dis[greed.path.back()][min])) {
                    min = it->first;
                }
            }
            greed.path.push_back(min);
            isVisited[min] = true;
        }
        greed.path.push_back(start);
        greed.gen = gen;
        greed.sum = sum(greed.path);
        pack[0] = greed;
    }
}

void passOn() {
    record.push(pack[0].sum);

    if (record.size() > 500) {
        record.pop();

        if (curCost != pack[0].sum && record.front() == record.back()) {
            vector<int> Prometheus = pack[0].path;
            int gen = pack[0].gen;
            double sum = pack[0].sum;
            pack.clear();

            if (Hayflick > 0) {
                Hayflick--;
                packNum *= log(packNum) / log(10);
            }

            initPack(gen);
            pack[0].path = Prometheus;
            pack[0].sum = sum;
            sort(pack.begin(), pack.end(), cmp);
            curCost = sum;
            mutationP += 0.1;

            while (!record.empty()) {
                record.pop();
            }
        }
    }
}


solution cross(solution &firstParent, solution &secondParent) {
    int length = int((rand() % 1000 / 1000.0) * city.size());
    int off = rand() % city.size() + 1;
    vector<int> nextGen(firstParent.path.size());
    unordered_map<int, bool> selected;
    nextGen[0] = start;
    for (int i = off; i < nextGen.size() - 1 && i < off + length; i++) {
        nextGen[i] = firstParent.path[i];
        selected[nextGen[i]] = true;
    }
    for (int i = 1, j = 1; i < nextGen.size(); i++) {
        if (nextGen[i] == 0) {
            for (; j < secondParent.path.size(); j++) {
                if (selected.count(secondParent.path[j]) == 0) {
                    nextGen[i] = secondParent.path[j];
                    selected[secondParent.path[j]] = true;
                    break;
                }
            }
        }
    }
    return solution{nextGen, sum(nextGen), 0, 0, firstParent.gen + 1};
}

void mutation(solution &cur) {
    int length = int((rand() % 1000 / 1000.0) * city.size());
    int off = rand() % city.size() + 1;
    vector<int> m;
    unordered_map<int, bool> selected;
    m.push_back(start);
    for (int i = off; i < cur.path.size() - 1 && i < off + length; i++) {
        m.push_back(cur.path[i]);
        selected[cur.path[i]] = true;
    }
    for (int i = 1; i < cur.path.size(); i++) {
        if (selected.count(cur.path[i]) == 0) {
            m.push_back(cur.path[i]);
        }
    }
    for (int i = 0; i < m.size(); i++) {
        cur.path[i] = m[i];
    }
    cur.sum = sum(cur.path);
}

void gmutation(solution &cur) {
    int selected = rand() % (city.size() - 4) + 2, min = 1;
    int selectedCity = cur.path[selected];
    int begin = 0, end = 0;
    for (auto it = dis[selectedCity].begin(); it != dis[selectedCity].end(); it++) {
        if (it->first != selectedCity && it->second < dis[selectedCity][min]) {
            min = it->first;
        }
    }
    for (int i = 1; i < cur.path.size() - 1; i++) {
        if (cur.path[i] == min) {
            if (i > selected + 1) {
                begin = selected + 1;
                end = i;
            } else if (i < selected - 1) {
                begin = i;
                end = selected - 1;
            }
            break;
        }
    }
    vector<int> stack;
    for (int i = begin; i <= end; i++) {
        stack.push_back(cur.path[i]);
    }
    for (int i = begin; i <= end; i++) {
        cur.path[i] = stack.back();
        stack.pop_back();
    }
    cur.sum = sum(cur.path);
}

vector<solution> process() {
    double total = 0;
    vector<solution> nextGenPack;
    sort(pack.begin(), pack.end() - 1, cmp);
    printf("%d %d\n", pack[0].gen, (int) pack[0].sum);
    if (pack[0].sum == 10628) {
        isFound = true;
    }
    passOn();

    for (auto it = pack.begin(); it != pack.end() - 1; it++) {
        it->F = 10000 / it->sum;
        it->P = (it == pack.begin() ? 0 : (it - 1)->P) + it->F;
        total += it->F;
    }
    nextGenPack.push_back(pack[0]);
    nextGenPack[0].gen++;
    for (auto firstParent = pack.begin(); firstParent != pack.end() - 1; firstParent++) {
        if (rand() % 10000 / 10000.0 < crossP) {
            double selected = (rand() % 10000 / 10000.0) * total;
            for (auto secondParent = pack.begin(); secondParent != pack.end() - 1; secondParent++) {
                if (selected < secondParent->P) {
                    nextGenPack.push_back(cross(*firstParent, *secondParent));
                    break;
                }
            }
        } else {
            firstParent->gen++;
            nextGenPack.push_back(*firstParent);
        }
        if (rand() % 10000 / 10000.0 < mutationP) {
            Hayflick < 6 ? gmutation(nextGenPack.back()) : mutation(nextGenPack.back());
        }
    }
    return nextGenPack;
}

int main() {
    clock_t start = clock();
    srand(unsigned(time(NULL)));
    initData();
    initPack(1);
    while (!isFound && curGen <= maxGen) {
        pack = process();
        curGen++;
    }
    cout << "Total time: " << double(clock() - start) / CLOCKS_PER_SEC << endl;
    cout << "The best: " << pack[0].sum << endl;
    for (auto it = pack[0].path.begin(); it != pack[0].path.end(); it++) {
        cout << *it << " ";
    }
    return 0;
}
