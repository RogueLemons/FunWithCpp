// https://open.kattis.com/problems/safesecret

#include <iostream>
#include <vector>
#include <limits>
#include <string>
using std::string, std::vector;

struct Range {
    int start = 0;
    int end = 0;

    int length() const { return end - start; }
};

class Data {
public:
    static Data readInput() {
        int count;
        std::cin >> count;
        Data data(count);

        for (int i = 0; i < data.numbersCount; i++) {  // while (std::cin >> num && std::cin >> sym)
            int num;
            std::cin >> num;
            data.numbers.push_back(num);

            char op;
            std::cin >> op;
            data.operators.push_back(op);
        }

        return data;
    }

    const int numbersCount;
    int numberAt(int index) const { return getByOvershootIndex(numbers, index); }
    char operatorAt(int index) const { return getByOvershootIndex(operators, index); }

private:
    template<typename T>
    static T getByOvershootIndex(const std::vector<T>& vec, int index) {
        return vec[(index + vec.size()) % vec.size()];
    }

    Data(int numbersCount_)
        : numbersCount(numbersCount_)
    {  }
    vector<int> numbers;
    vector<char> operators;
};


struct Evaluation {
    int64_t lowest = std::numeric_limits<int64_t>::max();
    int64_t highest = std::numeric_limits<int64_t>::min();

    void update(int64_t eval) {
        if (eval < lowest)
            lowest = eval;
        if (eval > highest)
            highest = eval;
    }

    void update(const Evaluation& eval) {
        if (eval.lowest < lowest)
            lowest = eval.lowest;
        if (eval.highest > highest)
            highest = eval.highest;
    }

    bool sameLowHigh() const {
        return lowest == highest;
    }
};

Evaluation evaluateOneOp(int64_t leftNumber, char op, int64_t rightNumber) {
    Evaluation eval;
    switch (op)
    {
    case '+':
        eval.update(leftNumber + rightNumber);
        break;
    case '-':
        eval.update(leftNumber - rightNumber);
        break;
    case '*':
        eval.update(leftNumber * rightNumber);
        break;
    case '?':
        eval.update(leftNumber + rightNumber);
        eval.update(leftNumber - rightNumber);
        eval.update(leftNumber * rightNumber);
        break;
    }
    return eval;
}

Evaluation evaluateOneOp(const Evaluation& leftEval, char op, const Evaluation& rightEval) {
    bool leftSame = leftEval.sameLowHigh();
    bool rightSame = rightEval.sameLowHigh();

    if (leftSame && rightSame) {
        return evaluateOneOp(leftEval.lowest, op, rightEval.lowest);
    }

    if (leftSame) {
        Evaluation eval = evaluateOneOp(leftEval.lowest, op, rightEval.lowest);
        eval.update(evaluateOneOp(leftEval.lowest, op, rightEval.highest));
        return eval;
    }

    if (rightSame) {
        Evaluation eval = evaluateOneOp(leftEval.lowest, op, rightEval.lowest);
        eval.update(evaluateOneOp(leftEval.highest, op, rightEval.lowest));
        return eval;
    }

    Evaluation eval;
    auto subEval_1 = evaluateOneOp(leftEval.lowest, op, rightEval.lowest);
    eval.update(subEval_1);
    auto subEval_2 = evaluateOneOp(leftEval.lowest, op, rightEval.highest);
    eval.update(subEval_2);
    auto subEval_3 = evaluateOneOp(leftEval.highest, op, rightEval.lowest);
    eval.update(subEval_3);
    auto subEval_4 = evaluateOneOp(leftEval.highest, op, rightEval.highest);
    eval.update(subEval_4);
    return eval;
}

static Evaluation evaluations[400][400] = {};
Evaluation evaluate(const Range& numberIndices, const Data& inputData) {
    const auto memEval = evaluations[numberIndices.start][numberIndices.end];
    if (memEval.lowest != std::numeric_limits<int64_t>::max() && memEval.highest != std::numeric_limits<int64_t>::min())
    {
        return memEval;
    }

    if (numberIndices.length() == 1) {
        const auto val = inputData.numberAt(numberIndices.start);
        Evaluation eval{ .lowest = val, .highest = val };
        evaluations[numberIndices.start][numberIndices.end] = eval;
        return eval;
    }

    Evaluation eval;
    for (int i = numberIndices.start + 1; i < numberIndices.end; i++) {
        const Range leftNums = { numberIndices.start, i };
        const auto leftEval = evaluate(leftNums, inputData);

        const Range rightNums = { i, numberIndices.end };
        const auto rightEval = evaluate(rightNums, inputData);

        const auto op = inputData.operatorAt(i - 1);
        const auto subEval = evaluateOneOp(leftEval, op, rightEval);
        eval.update(subEval);
    }

    evaluations[numberIndices.start][numberIndices.end] = eval;
    return eval;
}

string determineCode(const Data& inputData) {
    string code = "";
    for (int i = 0; i < inputData.numbersCount; i++) {
        const Range range = { i, i + inputData.numbersCount };
        auto evaluation = evaluate(range, inputData);
        code += std::to_string(std::abs(evaluation.lowest)) + std::to_string(std::abs(evaluation.highest));
    }
    return code;
}

int main(void) {
    const auto inputData = Data::readInput();
    const auto code = determineCode(inputData);
    std::cout << code;
    return 0;
}