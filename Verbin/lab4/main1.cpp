#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <climits>
#include <algorithm>


bool INTER = true;
//перегруженны оператор вывода вектора в поток
std::ostream& operator<<(std::ostream& stream, std::vector<int> vect) {
    stream << vect[0];
    for (int i = 1; i < vect.size(); i++) {
        stream << "," << vect[i];
    }
    return stream;
}


void printInterInfo(std::string& splice, std::vector<int> vect, int n) {
    std::cout << "\nСостояние префикс-функции для строки: " << std::endl;
    for (int i = 0; i <= n; i++)
        std::cout << splice[i] << " ";
    std::cout << std::endl;
    for (int i = 0; i <= n; i++)
        std::cout << vect[i] << " ";
    std::cout << std::endl;
}


void prefixFunction(std::string& sample, std::vector<int>& prefix) {
    prefix[0] = 0;
    std::cout << "Начало рассчета префикс-функции" << std::endl;
    int n = 1;
    for (int i = 1; i < sample.length(); i++)
    {   // поиск какой префикс-суффикс можно расширить
        int k = prefix[i - 1];                      // длина предыдущего префикса
        while (k > 0 && sample[i] != sample[k])   // этот нельзя расширить,
            k = prefix[k - 1];                    // берем ранее рассчитанное значение

        if (sample[k] == sample[i])
            k = k + 1;                          // расширяем найденный префикс-суффикс

        prefix[i] = k;

        if (INTER)
            printInterInfo(sample, prefix, n++);
    }

}


std::vector< std::pair<std::string, int >> toFlows(std::string text, const int uFlow, int maxFlow) {

    std::vector< std::pair<std::string, int >> tmp;
    int step = maxFlow / uFlow;
    int ind = 0;
    int freeSize = text.length() - (step * uFlow + text.length() - maxFlow);

    for (int i = 0; i < uFlow; i++) {
        int len;
        if (uFlow - i == 1) {
            len = text.length() - ind;
        }
        else {
            len = step + text.length() - maxFlow;
            if (freeSize > 0) {
                len++;
            }
        }
        tmp.push_back(std::make_pair(text.substr(ind, len), ind));
        if (freeSize > 0) {
            ind++; freeSize--;
        }
        ind += step;
        std::cout << "Добавлен поток: " << tmp[tmp.size() - 1].first << " Начальный индекс: " << tmp[tmp.size() - 1].second << std::endl;
    }

    return tmp;
}


//функция для сортировки по возрастанию
int comp1(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}


//функция определения сдвига
bool findEntry(std::string& text, std::string& sample, std::vector<int>& prefix) {
    int k = 0;   //индекс сравниваемого символа в sample
    bool flag = false;
    std::vector<int> result;
    if (text.length() <= sample.length()) {
        std::cout << "\nНе может быть образов: ";
        std::cout << "-1";
        return false;
    }
    int maxFlow = text.length() - (sample.length() - 1);
    std::cout << "Максимальное колличество потоков: " << maxFlow << " |Введите колличество потоков: ";
    int uFlow;
    std::cin >> uFlow;
    if (uFlow > maxFlow || uFlow < 0) {
        std::cout << "Ошибка в вводе! | Введите колличество потоков: ";
        std::cin >> uFlow;
    }
    else {
        std::vector< std::pair<std::string, int >> flows = toFlows(text, uFlow, maxFlow);
        for (int j = 0; j < flows.size(); j++) {
            k = 0;
            std::cout << "\nПоиск образа в потокe: " << flows[j].first << ":\n\n";
            for (int i = 0; i < flows[j].first.length(); i++) {
                while (k > 0 && sample[k] != flows[j].first[i])
                    k = prefix[k - 1];
                std::cout << "Symbols: " << flows[j].first[i] << ", " << sample[k] << "\t";
                if (sample[k] == flows[j].first[i]) {
                    std::cout << "\ttext[" << i << "] == sample[" << k << "]\n";
                    k = k + 1;
                }
                else {
                    std::cout << "\ttext[" << i << "] != sample[" << k << "]";
                    if (k != 0) {
                        std::cout << "; k = " << prefix[k - 1] << std::endl;
                        //     k = prefix[k - 1];
                    }
                    else {
                        std::cout << std::endl;
                    }
                }
                if (k == sample.length()) {
                    std::cout << "\n\tНайден образ! позиция: " << flows[j].second + i - sample.length() + 1 << "\n\n";
                    bool flag1 = true;
                    for (int t = 0; t < result.size(); t++) {
                        if (result[t] == flows[j].second + i - sample.length() + 1) {
                            flag1 = false;
                            break;
                        }
                    }

                    if(flag1) result.push_back(flows[j].second + i - sample.length() + 1);
                    flag = true;
                }

            }
        }
        if (!flag) std::cout << "\nНет образовм: " << -1;
        if (flag) {
            std::sort(result.begin(), result.end());
            std::cout << "\nResult: ";
            for (int i = 0; i < result.size(); i++) {
                if (i > 0) std::cout << ", ";
                std::cout << result[i];
            }
            std::cout << std::endl;
        }
        return flag;

    }

}


int main()
{
    setlocale(LC_ALL, "Russian");
    std::string text;
    std::string sample;
    std::cin >> sample; std::cin >> text;
    std::vector<int> prefix(sample.length());
    prefixFunction(sample, prefix);

    findEntry(text, sample, prefix);
    return 0;
}
