std::vector< std::pair<std::string, int >> toFlows(std::string text, const int uFlow, int maxFlow) {

    std::vector< std::pair<std::string, int >> tmp;
    int step = maxFlow / uFlow;
    int ind = 0;
    int freeSize = text.length() - ( step * uFlow + text.length() - maxFlow );
    
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
