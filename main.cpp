#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <string>
#include<windows.h>
#include <cstdlib>

using json = nlohmann::json;

json get_name(const std::string& key) {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.freecurrencyapi.com/v1/currencies?apikey=" + key});
    if (r.status_code != 200) throw std::runtime_error("currencies request failed: " + std::to_string(r.status_code));
    return json::parse(r.text);
}

json get_rates(const std::string& key) {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.freecurrencyapi.com/v1/latest?apikey=" + key});
    if (r.status_code != 200) throw std::runtime_error("latest request failed: " + std::to_string(r.status_code));
    return json::parse(r.text);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    const char* APIk = std::getenv("API_KEY");
    if (!APIk) { std::cerr << "API_KEY not set\n"; return 1; }
    std::string key(APIk);

    json name_data, data;
    try {
        name_data = get_name(key);
        data = get_rates(key);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    while (true) {
        std::string fromCurrency, toCurrency;
        double amount;

        std::cout << "Enter the currency you want to convert from ";
        std::cin >> fromCurrency;
        std::transform(fromCurrency.begin(), fromCurrency.end(), fromCurrency.begin(),
                       [](unsigned char c){ return std::toupper(c); });

        if (fromCurrency == "EXIT") {
            std::cout << "Exiting the program...\nThanks for Using the Currency Converter!\n";
            break;
        }

        std::cout << "Enter the amount you want to convert ";
        std::cin >> amount;

        std::cout << "Enter the currency you want to convert to ";
        std::cin >> toCurrency;
        std::transform(toCurrency.begin(), toCurrency.end(), toCurrency.begin(),
                       [](unsigned char c){ return std::toupper(c); });

        if (toCurrency == "EXIT") {
            std::cout << "Exiting the program...\nThanks for Using the Currency Converter!\n";
            break;
        }

        if (!data["data"].contains(fromCurrency) || !data["data"].contains(toCurrency)) {
            std::cout << "Unknown currency code.\n";
            continue;
        }

        double toRate = data["data"][toCurrency].get<double>() / data["data"][fromCurrency].get<double>();
        std::string symbol = name_data["data"][toCurrency]["symbol_native"].get<std::string>();
        std::cout << "The converted amount is: " << symbol << " " << toRate * amount << std::endl;
    }
}