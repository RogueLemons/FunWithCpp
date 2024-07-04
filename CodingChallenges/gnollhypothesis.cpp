// https://open.kattis.com/problems/gnollhypothesis

#include <iostream>
#include <vector>
#include <iomanip>
using Number = double;
using Numbers = std::vector<Number>;

struct Data {
    int N;
    int newNumberOfProbabilities;
    Numbers numbers;
};

Data readInput() {
    Data data;
    std::cin >> data.N;
    std::cin >> data.newNumberOfProbabilities;

    Number number;
    while (std::cin >> number) {
        data.numbers.push_back(number);
    }

    return data;
}

Number getByOvershootIndex(const Numbers& probabilities, int index) {
    return probabilities[(index + probabilities.size()) % probabilities.size()];
}

Numbers newProbabilities(const Data& data) {
    Numbers newEnemyProbs = data.numbers;

    for (int i = 0; i < newEnemyProbs.size(); i++) {
        auto oddsIncluded = (double)data.newNumberOfProbabilities / (double)data.N;
        auto cumulativeOdds = oddsIncluded;
        auto cumulativeEnemyProb = newEnemyProbs[i] * cumulativeOdds;

        int exclusions = data.N - data.newNumberOfProbabilities;
        for (int j = 1; j <= exclusions; j++) {
            auto oddsExcluded = (exclusions + 1 - j) / (double)(data.N - j);
            cumulativeOdds *= oddsExcluded;
            auto enemyProb = getByOvershootIndex(data.numbers, i - j);
            cumulativeEnemyProb += enemyProb * cumulativeOdds;
        }

        newEnemyProbs[i] = cumulativeEnemyProb;
    }

    return newEnemyProbs;
}

void printProbabilities(const Numbers& probabilities) {
    for (auto& prob : probabilities) {
        std::cout << std::setprecision(10) << prob << " ";
    }
}

int main(void) {
    const auto input = readInput();
    const auto newProbs = newProbabilities(input);
    printProbabilities(newProbs);
    return 0;
}